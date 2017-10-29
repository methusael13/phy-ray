#include <core/integrator/sampling.h>

namespace phyr {

// Sampling a unit disk
Point2f uniformSampleDisk(const Point2f& pt) {
    Real rad = std::sqrt(pt.x);
    Real theta = 2 * Pi * pt.y;
    return Point2f(rad * std::cos(theta), rad * std::sin(theta));
}

Point2f concentricSampleDisk(const Point2f& pt) {
    // Map uniform random point to [-1, 1]
    Point2f nrand = 2.0 * pt - Vector2f(1, 1);
    // Handle origin
    if (nrand.x == 0 && nrand.y == 0)
        return Point2f(0, 0);

    Real rad, theta;
    // Apply concentric mapping
    if (std::abs(nrand.x) > std::abs(nrand.y)) {
        rad = nrand.x;
        theta = PiOver4 * (nrand.y / nrand.x);
    } else {
        rad = nrand.y;
        theta = PiOver4 * (nrand.x / nrand.y);
    }

    return Point2f(rad * std::cos(theta), rad * std::sin(theta));
}

template <typename T>
void shuffle(T* sample, int nSamples, int nDimensions, RNG& rng) {
    for (int i = 0; i < nSamples; i++) {
        int swapIdx = i + rng.uniformUInt32(nSamples - i);
        for (int j = 0; j < nDimensions; j++) {
            std::swap(sample[nDimensions * i + j], sample[nDimensions * swapIdx + j]);
        }
    }
}

void stratifiedSample1D(Real* sample, int nSamples, bool jitter, RNG& rng) {
    Real invNSamples = Real(1) / nSamples;
    for (int i = 0; i < nSamples; i++) {
        Real offset = jitter ? rng.uniformReal() : 0.5;
        sample[i] = std::min((i + offset) * invNSamples, OneMinusEpsilon);
    }
}

void stratifiedSample2D(Point2f* sample, int nxSamples, int nySamples, bool jitter, RNG& rng) {
    Real invXSamples = Real(1) / nxSamples, invYSamples = Real(1) / nySamples;
    for (int y = 0; y < nySamples; y++) {
        for (int x = 0; x < nxSamples; x++) {
            Real offX = jitter ? rng.uniformReal() : 0.5;
            Real offY = jitter ? rng.uniformReal() : 0.5;
            sample->x = std::min((x + offX) * invXSamples, OneMinusEpsilon);
            sample->y = std::min((y + offY) * invYSamples, OneMinusEpsilon);
        }
    }
}

void latinHypercube(Real* sample, int nSamples, int nDimensions, RNG& rng) {
    // Generate LHS samples along the diagonal
    Real invSamples = Real(1) / nSamples;
    for (int i = 0; i < nSamples; i++) {
        for (int j = 0; j < nDimensions; j++) {
            Real offset = (i + rng.uniformReal()) * invSamples;
            sample[nDimensions * i + j] = std::min(offset, OneMinusEpsilon);
        }
    }

    // Permute LHS samples in each dimension
    for (int i = 0; i < nDimensions; i++) {
        for (int j = 0; j < nSamples; j++) {
            int swapIdx = j + rng.uniformUInt32(nSamples - j);
            std::swap(sample[nDimensions * j + i], sample[nDimensions * swapIdx + i]);
        }
    }
}

}  // namespace phyr
