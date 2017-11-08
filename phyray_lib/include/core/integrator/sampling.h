#ifndef PHYRAY_CORE_SAMPLING_H
#define PHYRAY_CORE_SAMPLING_H

#include <core/rng.h>
#include <core/phyr.h>
#include <core/geometry/geometry.h>

namespace phyr {

// Sampling a unit disk
Point2f uniformSampleDisk(const Point2f& pt);
Point2f concentricSampleDisk(const Point2f& pt);

template <typename T>
void shuffle(T* sample, int nSamples, int nDimensions, RNG& rng) {
    for (int i = 0; i < nSamples; i++) {
        int swapIdx = i + rng.uniformUInt32(nSamples - i);
        for (int j = 0; j < nDimensions; j++) {
            std::swap(sample[nDimensions * i + j], sample[nDimensions * swapIdx + j]);
        }
    }
}

// Stratified samples
void stratifiedSample1D(Real* sample, int nSamples, bool jitter, RNG& rng);
void stratifiedSample2D(Point2f* sample, int nxSamples, int nySamples, bool jitter, RNG& rng);

void latinHypercube(Real* sample, int nSamples, int nDimensions, RNG& rng);

// Hemisphere sampling
Vector3f uniformSampleHemisphere(const Point2f& u);
Real uniformHemispherePdf();

// Sphere sampling
Vector3f uniformSampleSphere(const Point2f& u);
Real uniformSpherePdf();

// Cone sampling
Vector3f uniformSampleCone(const Point2f &u, Real thetamax);
Vector3f uniformSampleCone(const Point2f &u, Real thetamax, const Vector3f &x,
                           const Vector3f &y, const Vector3f &z);
Real uniformConePdf(Real cosThetaMax);

/* Code adapted for PhyRay from pbrt */
struct Distribution1D {
    // Distribution1D Public Methods
    Distribution1D(const Real* f, int n) : func(f, f + n), cdf(n + 1) {
        // Compute integral of step function at {x_i}
        cdf[0] = 0;
        for (int i = 1; i < n + 1; ++i)
            cdf[i] = cdf[i - 1] + func[i - 1] / n;

        // Transform step function integral into CDF
        funcInt = cdf[n];
        if (funcInt == 0) {
            for (int i = 1; i < n + 1; ++i) cdf[i] = Real(i) / Real(n);
        } else {
            for (int i = 1; i < n + 1; ++i) cdf[i] /= funcInt;
        }
    }

    int count() const { return (int)func.size(); }

    Real sampleContinuous(Real u, Real* pdf, int* off = nullptr) const {
        // Find surrounding CDF segments and {offset}
        int offset = findInterval((int)cdf.size(),
                                  [&](int index) { return cdf[index] <= u; });
        if (off) *off = offset;
        // Compute offset along CDF segment
        Real du = u - cdf[offset];
        if ((cdf[offset + 1] - cdf[offset]) > 0) {
            ASSERT(cdf[offset + 1] >= cdf[offset]);
            du /= (cdf[offset + 1] - cdf[offset]);
        }
        ASSERT(!std::isnan(du));

        // Compute PDF for sampled offset
        if (pdf) *pdf = (funcInt > 0) ? func[offset] / funcInt : 0;

        // Return $x\in{}[0,1)$ corresponding to sample
        return (offset + du) / count();
    }

    int sampleDiscrete(Real u, Real* pdf = nullptr,
                       Real* uRemapped = nullptr) const {
        // Find surrounding CDF segments and {offset}
        int offset = findInterval((int)cdf.size(),
                                  [&](int index) { return cdf[index] <= u; });
        if (pdf) *pdf = (funcInt > 0) ? func[offset] / (funcInt * count()) : 0;
        if (uRemapped)
            *uRemapped = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);
        if (uRemapped) ASSERT(*uRemapped >= 0.f && *uRemapped <= 1.f);
        return offset;
    }

    Real discretePDF(int index) const {
        ASSERT(index >= 0 && index < count());
        return func[index] / (funcInt * count());
    }

    // Distribution1D Public Data
    std::vector<Real> func, cdf;
    Real funcInt;
};

// Inline sampling functions

inline Real balanceHeuristic(int nf, Real fPdf, int ng, Real gPdf) {
    return (nf * fPdf) / (nf * fPdf + ng * gPdf);
}

inline Real powerHeuristic(int nf, Real fPdf, int ng, Real gPdf) {
    Real f = nf * fPdf, g = ng * gPdf;
    return (f * f) / (f * f + g * g);
}

// Sampling a unit hemisphere
inline Vector3f cosineSampleHemisphere(const Point2f& pt) {
    Point2f d = concentricSampleDisk(pt);
    Real z = std::sqrt(std::max(Real(0), 1 - d.x * d.x - d.y * d.y));
    return Vector3f(d.x, d.y, z);
}

inline Real cosineHemispherePdf(Real cosTheta) { return cosTheta * InvPi; }

}  // namespace phyr

#endif
