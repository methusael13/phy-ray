#include <core/phyr.h>
#include <core/scene.h>
#include <core/debug.h>
#include <core/lowdiscrepancy.h>
#include <core/integrator/integrator.h>
#include <core/integrator/lightdistrib.h>

#include <numeric>

/**
 * Light Distribution classes adapted from pbrt-v3 for use with PhyRay
 */

namespace phyr {

LightDistribution::~LightDistribution() {}

std::unique_ptr<LightDistribution> createLightSampleDistribution(
    const std::string& name, const Scene& scene) {
    if (name == "uniform" || scene.lights.size() == 1)
        return std::unique_ptr<LightDistribution>(new UniformLightDistribution(scene));
    else if (name == "power")
        return std::unique_ptr<LightDistribution>(new PowerLightDistribution(scene));
    else if (name == "spatial")
        return std::unique_ptr<LightDistribution>(new SpatialLightDistribution(scene));
    else {
        return std::unique_ptr<LightDistribution>(new SpatialLightDistribution(scene));
    }
}

UniformLightDistribution::UniformLightDistribution(const Scene& scene) {
    std::vector<Real> prob(scene.lights.size(), Real(1));
    distrib.reset(new Distribution1D(&prob[0], int(prob.size())));
}

const Distribution1D* UniformLightDistribution::lookup(const Point3f& p) const {
    return distrib.get();
}

PowerLightDistribution::PowerLightDistribution(const Scene& scene)
    : distrib(computeLightPowerDistribution(scene)) {}

const Distribution1D *PowerLightDistribution::lookup(const Point3f& p) const {
    return distrib.get();
}

// SpatialLightDistribution

// Voxel coordinates are packed into a uint64_t for hash table lookups;
// 10 bits are allocated to each coordinate.  invalidPackedPos is an impossible
// packed coordinate value, which we use to represent
static const uint64_t invalidPackedPos = 0xffffffffffffffff;

SpatialLightDistribution::SpatialLightDistribution(const Scene& scene, int maxVoxels)
    : scene(scene) {
    // Compute the number of voxels so that the widest scene bounding box
    // dimension has maxVoxels voxels and the other dimensions have a number
    // of voxels so that voxels are roughly cube shaped.
    Bounds3f b = scene.getWorldBounds();
    Vector3f diag = b.diagonal();
    Real bmax = diag[b.maximumExtent()];

    for (int i = 0; i < 3; ++i) {
        nVoxels[i] = std::max(1, int(std::round(diag[i] / bmax * maxVoxels)));
        // In the Lookup() method, we require that 20 or fewer bits be
        // sufficient to represent each coordinate value. It's fairly hard
        // to imagine that this would ever be a problem.
        ASSERT(nVoxels[i] <= 1 << 20);
    }

    hashTableSize = 4 * nVoxels[0] * nVoxels[1] * nVoxels[2];
    hashTable.reset(new HashEntry[hashTableSize]);

    for (int i = 0; i < hashTableSize; ++i) {
        hashTable[i].packedPos.store(invalidPackedPos);
        hashTable[i].distribution.store(nullptr);
    }
}

SpatialLightDistribution::~SpatialLightDistribution() {
    // Gather statistics about how well the computed distributions are across
    // the buckets.
    for (size_t i = 0; i < hashTableSize; ++i) {
        HashEntry &entry = hashTable[i];
        if (entry.distribution.load())
            delete entry.distribution.load();
    }
}

const Distribution1D* SpatialLightDistribution::lookup(const Point3f& p) const {
    // First, compute integer voxel coordinates for the given point |p|
    // with respect to the overall voxel grid.
    Vector3f offset = scene.getWorldBounds().offset(p);  // offset in [0,1].
    Point3i pi;

    for (int i = 0; i < 3; ++i)
        // The clamp should almost never be necessary, but is there to be
        // robust to computed intersection points being slightly outside
        // the scene bounds due to floating-point roundoff error.
        pi[i] = clamp(int(offset[i] * nVoxels[i]), 0, nVoxels[i] - 1);

    // Pack the 3D integer voxel coordinates into a single 64-bit value.
    uint64_t packedPos = (uint64_t(pi[0]) << 40) | (uint64_t(pi[1]) << 20) | pi[2];
    ASSERT(packedPos != invalidPackedPos);

    // Compute a hash value from the packed voxel coordinates.  We could
    // just take packedPos mod the hash table size, but since packedPos
    // isn't necessarily well distributed on its own, it's worthwhile to do
    // a little work to make sure that its bits values are individually
    // fairly random. For details of and motivation for the following, see:
    // http://zimbry.blogspot.ch/2011/09/better-bit-mixing-improving-on.html
    uint64_t hash = packedPos;
    hash ^= (hash >> 31);
    hash *= 0x7fb5d329728ea185;
    hash ^= (hash >> 27);
    hash *= 0x81dadef4bc2dd44d;
    hash ^= (hash >> 33);
    hash %= hashTableSize;
    ASSERT(hash >= 0);

    // Now, see if the hash table already has an entry for the voxel. We'll
    // use quadratic probing when the hash table entry is already used for
    // another value; step stores the square root of the probe step.
    int step = 1, nProbes = 0;

    while (true) {
        ++nProbes;
        HashEntry& entry = hashTable[hash];
        // Does the hash table entry at offset |hash| match the current point?
        uint64_t entryPackedPos = entry.packedPos.load(std::memory_order_acquire);
        if (entryPackedPos == packedPos) {
            // Yes! Most of the time, there should already by a light
            // sampling distribution available.
            Distribution1D* dist = entry.distribution.load(std::memory_order_acquire);
            if (dist == nullptr) {
                // Rarely, another thread will have already done a lookup
                // at this point, found that there isn't a sampling
                // distribution, and will already be computing the
                // distribution for the point.  In this case, we spin until
                // the sampling distribution is ready.  We assume that this
                // is a rare case, so don't do anything more sophisticated
                // than spinning.
                while ((dist = entry.distribution.load(std::memory_order_acquire)) == nullptr)
                    // spin :-(. If we were fancy, we'd have any threads
                    // that hit this instead help out with computing the
                    // distribution for the voxel...
                    ;
            }
            // We have a valid sampling distribution.
            return dist;
        } else if (entryPackedPos != invalidPackedPos) {
            // The hash table entry we're checking has already been
            // allocated for another voxel. Advance to the next entry with
            // quadratic probing.
            hash += step * step;
            if (hash >= hashTableSize)
                hash %= hashTableSize;
            ++step;
        } else {
            // We have found an invalid entry. (Though this may have
            // changed since the load into entryPackedPos above.)  Use an
            // atomic compare/exchange to try to claim this entry for the
            // current position.
            uint64_t invalid = invalidPackedPos;
            if (entry.packedPos.compare_exchange_weak(invalid, packedPos)) {
                // Success; we've claimed this position for this voxel's
                // distribution. Now compute the sampling distribution and
                // add it to the hash table. As long as packedPos has been
                // set but the entry's distribution pointer is nullptr, any
                // other threads looking up the distribution for this voxel
                // will spin wait until the distribution pointer is
                // written.
                Distribution1D* dist = computeDistribution(pi);
                entry.distribution.store(dist, std::memory_order_release);
                return dist;
            }
        }
    }
}

Distribution1D* SpatialLightDistribution::computeDistribution(Point3i pi) const {
    // Compute the world-space bounding box of the voxel corresponding to |pi|.
    Point3f p0(Real(pi[0]) / Real(nVoxels[0]),
               Real(pi[1]) / Real(nVoxels[1]),
               Real(pi[2]) / Real(nVoxels[2]));
    Point3f p1(Real(pi[0] + 1) / Real(nVoxels[0]),
               Real(pi[1] + 1) / Real(nVoxels[1]),
               Real(pi[2] + 1) / Real(nVoxels[2]));
    Bounds3f voxelBounds(scene.getWorldBounds().lerp(p0),
                         scene.getWorldBounds().lerp(p1));

    // Compute the sampling distribution. Sample a number of points inside
    // voxelBounds using a 3D Halton sequence; at each one, sample each
    // light source and compute a weight based on Li/pdf for the light's
    // sample (ignoring visibility between the point in the voxel and the
    // point on the light source) as an approximation to how much the light
    // is likely to contribute to illumination in the voxel.
    int nSamples = 128;
    std::vector<Real> lightContrib(scene.lights.size(), Real(0));
    for (int i = 0; i < nSamples; ++i) {
        Point3f po = voxelBounds.lerp(Point3f(
            radicalInverse(0, i), radicalInverse(1, i), radicalInverse(2, i)));
        Interaction intr(po, Normal3f(), Vector3f(1, 0, 0), Vector3f());

        // Use the next two Halton dimensions to sample a point on the
        // light source.
        Point2f u(radicalInverse(3, i), radicalInverse(4, i));
        for (size_t j = 0; j < scene.lights.size(); ++j) {
            Real pdf; Vector3f wi;
            VisibilityTester vis;
            Spectrum Li = scene.lights[j]->sample_li(intr, u, &wi, &pdf, &vis);

            if (pdf > 0) {
                // TODO: look at tracing shadow rays / computing beam
                // transmittance.  Probably shouldn't give those full weight
                // but instead e.g. have an occluded shadow ray scale down
                // the contribution by 10 or something.
                lightContrib[j] += Li.getYConstant() / pdf;
            }
        }
    }

    // We don't want to leave any lights with a zero probability; it's
    // possible that a light contributes to points in the voxel even though
    // we didn't find such a point when sampling above.  Therefore, compute
    // a minimum (small) weight and ensure that all lights are given at
    // least the corresponding probability.
    Real sumContrib = std::accumulate(lightContrib.begin(), lightContrib.end(), Real(0));
    Real avgContrib = sumContrib / (nSamples * lightContrib.size());
    Real minContrib = (avgContrib > 0) ? .001 * avgContrib : 1;

    for (size_t i = 0; i < lightContrib.size(); ++i) {
        lightContrib[i] = std::max(lightContrib[i], minContrib);
    }

    // Compute a sampling distribution from the accumulated contributions.
    return new Distribution1D(&lightContrib[0], int(lightContrib.size()));
}

}  // namespace phyr
