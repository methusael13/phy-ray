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
        theta = PiOver2 - PiOver4 * (nrand.x / nrand.y);
    }

    return Point2f(rad * std::cos(theta), rad * std::sin(theta));
}

// Hemisphere sampling definitions
Vector3f uniformSampleHemisphere(const Point2f& u) {
    Real z = u[0];
    Real r = std::sqrt(std::max(Real(0), Real(1.) - z * z));
    Real phi = 2 * Pi * u[1];

    return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
}

// Cone sampling definitions
Real uniformConePdf(Real cosThetaMax) {
    return 1 / (2 * Pi * (1 - cosThetaMax));
}

Vector3f uniformSampleCone(const Point2f& u, Real cosThetaMax) {
    Real cosTheta = ((Real)1 - u[0]) + u[0] * cosThetaMax;
    Real sinTheta = std::sqrt((Real)1 - cosTheta * cosTheta);
    Real phi = u[1] * 2 * Pi;
    return Vector3f(std::cos(phi) * sinTheta, std::sin(phi) * sinTheta, cosTheta);
}

Vector3f uniformSampleCone(const Point2f& u, Real cosThetaMax,
                           const Vector3f& x, const Vector3f& y,
                           const Vector3f& z) {
    Real cosTheta = lerp(u[0], cosThetaMax, 1.f);
    Real sinTheta = std::sqrt((Real)1. - cosTheta * cosTheta);
    Real phi = u[1] * 2 * Pi;
    return std::cos(phi) * sinTheta * x + std::sin(phi) * sinTheta * y + cosTheta * z;
}

Real uniformHemispherePdf() { return Inv2Pi; }

// Sphere sampling definitions
Vector3f uniformSampleSphere(const Point2f& u) {
    Real z = 1 - 2 * u[0];
    Real r = std::sqrt(std::max((Real)0, (Real)1 - z * z));
    Real phi = 2 * Pi * u[1];
    return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
}

Real uniformSpherePdf() { return Inv4Pi; }

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
            ++sample;
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
