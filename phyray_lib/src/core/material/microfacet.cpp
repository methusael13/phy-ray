#include <core/geometry/geometry.h>
#include <core/material/microfacet.h>
#include <core/material/reflectance.h>

namespace phyr {

// Microfacet Utility Functions
static void beckmannSample11(Real cosThetaI, Real U1, Real U2,
                             Real* slope_x, Real* slope_y) {
    /* Special case (normal incidence) */
    if (cosThetaI > .9999) {
        Real r = std::sqrt(-std::log(1.0f - U1));
        Real sinPhi = std::sin(2 * Pi * U2);
        Real cosPhi = std::cos(2 * Pi * U2);
        *slope_x = r * cosPhi;
        *slope_y = r * sinPhi;
        return;
    }

    /* The original inversion routine from the paper contained
       discontinuities, which causes issues for QMC integration
       and techniques like Kelemen-style MLT. The following code
       performs a numerical inversion with better behavior */
    Real sinThetaI = std::sqrt(std::max((Real)0, (Real)1 - cosThetaI * cosThetaI));
    Real tanThetaI = sinThetaI / cosThetaI;
    Real cotThetaI = 1 / tanThetaI;

    /* Search interval -- everything is parameterized
       in the Erf() domain */
    Real a = -1, c = erf(cotThetaI);
    Real sample_x = std::max(U1, (Real)1e-6f);

    /* Start with a good initial guess */
    // Real b = (1-sample_x) * a + sample_x * c;

    /* We can do better (inverse of an approximation computed in Mathematica) */
    Real thetaI = std::acos(cosThetaI);
    Real fit = 1 + thetaI * (-0.876f + thetaI * (0.4265f - 0.0594f * thetaI));
    Real b = c - (1 + c) * std::pow(1 - sample_x, fit);

    /* Normalization factor for the CDF */
    static const Real SQRT_PI_INV = 1.f / std::sqrt(Pi);
    Real normalization = 1 / (1 + c + SQRT_PI_INV * tanThetaI * std::exp(-cotThetaI * cotThetaI));

    int it = 0;
    while (++it < 10) {
        /* Bisection criterion -- the oddly-looking
           Boolean expression are intentional to check
           for NaNs at little additional cost */
        if (!(b >= a && b <= c)) b = 0.5f * (a + c);

        /* Evaluate the CDF and its derivative
           (i.e. the density function) */
        Real invErf = erfInv(b);
        Real value =
            normalization *
                (1 + b + SQRT_PI_INV * tanThetaI * std::exp(-invErf * invErf)) -
            sample_x;
        Real derivative = normalization * (1 - invErf * tanThetaI);

        if (std::abs(value) < 1e-5f) break;

        /* Update bisection intervals */
        if (value > 0)
            c = b;
        else
            a = b;

        b -= value / derivative;
    }

    /* Now convert back into a slope value */
    *slope_x = erfInv(b);

    /* Simulate Y component */
    *slope_y = erfInv(2.0f * std::max(U2, (Real)1e-6f) - 1.0f);

    ASSERT(!std::isinf(*slope_x));
    ASSERT(!std::isnan(*slope_x));
    ASSERT(!std::isinf(*slope_y));
    ASSERT(!std::isnan(*slope_y));
}

static Vector3f beckmannSample(const Vector3f& wi, Real alpha_x, Real alpha_y,
                               Real U1, Real U2) {
    // 1. stretch wi
    Vector3f wiStretched =
        normalize(Vector3f(alpha_x * wi.x, alpha_y * wi.y, wi.z));

    // 2. simulate P22_{wi}(x_slope, y_slope, 1, 1)
    Real slope_x, slope_y;
    beckmannSample11(cosTheta(wiStretched), U1, U2, &slope_x, &slope_y);

    // 3. rotate
    Real tmp = cosPhi(wiStretched) * slope_x - sinPhi(wiStretched) * slope_y;
    slope_y = sinPhi(wiStretched) * slope_x + cosPhi(wiStretched) * slope_y;
    slope_x = tmp;

    // 4. unstretch
    slope_x = alpha_x * slope_x;
    slope_y = alpha_y * slope_y;

    // 5. compute normal
    return normalize(Vector3f(-slope_x, -slope_y, 1.f));
}

// MicrofacetDistribution Method Definitions
MicrofacetDistribution::~MicrofacetDistribution() {}

Real BeckmannDistribution::d(const Vector3f& wh) const {
    Real tan2Theta = tanSqTheta(wh);
    if (std::isinf(tan2Theta)) return 0.;
    Real cos4Theta = cosSqTheta(wh) * cosSqTheta(wh);
    return std::exp(-tan2Theta * (cosSqPhi(wh) / (alphax * alphax) +
                                  sinSqPhi(wh) / (alphay * alphay))) / (Pi * alphax * alphay * cos4Theta);
}

Real TrowbridgeReitzDistribution::d(const Vector3f& wh) const {
    Real tan2Theta = tanSqTheta(wh);
    if (std::isinf(tan2Theta)) return 0.;
    const Real cos4Theta = cosSqTheta(wh) * cosSqTheta(wh);
    Real e = (cosSqPhi(wh) / (alphax * alphax) + sinSqPhi(wh) / (alphay * alphay)) * tan2Theta;
    return 1 / (Pi * alphax * alphay * cos4Theta * (1 + e) * (1 + e));
}

Real BeckmannDistribution::lambda(const Vector3f& w) const {
    Real absTanTheta = std::abs(tanTheta(w));
    if (std::isinf(absTanTheta)) return 0.;
    // Compute _alpha_ for direction {w}
    Real alpha = std::sqrt(cosSqPhi(w) * alphax * alphax + sinSqPhi(w) * alphay * alphay);
    Real a = 1 / (alpha * absTanTheta);
    if (a >= 1.6f) return 0;
    return (1 - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
}

Real TrowbridgeReitzDistribution::lambda(const Vector3f& w) const {
    Real absTanTheta = std::abs(tanTheta(w));
    if (std::isinf(absTanTheta)) return 0.;
    // Compute {alpha} for direction {w}
    Real alpha = std::sqrt(cosSqPhi(w) * alphax * alphax + sinSqPhi(w) * alphay * alphay);
    Real alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
    return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
}

Vector3f BeckmannDistribution::sample_wh(const Vector3f& wo, const Point2f& u) const {
    if (!sampleVisibleArea) {
        // Sample full distribution of normals for Beckmann distribution

        // Compute tan^2 theta and phi$ for Beckmann distribution sample
        Real tan2Theta, phi;
        if (alphax == alphay) {
            Real logSample = std::log(1 - u[0]);
            ASSERT(!std::isinf(logSample));
            tan2Theta = -alphax * alphax * logSample;
            phi = u[1] * 2 * Pi;
        } else {
            // Compute _tan2Theta_ and _phi_ for anisotropic Beckmann
            // distribution
            Real logSample = std::log(1 - u[0]);
            ASSERT(!std::isinf(logSample));
            phi = std::atan(alphay / alphax * std::tan(2 * Pi * u[1] + 0.5f * Pi));
            if (u[1] > 0.5f) phi += Pi;
            Real sinPhi = std::sin(phi), cosPhi = std::cos(phi);
            Real alphax2 = alphax * alphax, alphay2 = alphay * alphay;
            tan2Theta = -logSample / (cosPhi * cosPhi / alphax2 + sinPhi * sinPhi / alphay2);
        }

        // Map sampled Beckmann angles to normal direction _wh_
        Real cosTheta = 1 / std::sqrt(1 + tan2Theta);
        Real sinTheta = std::sqrt(std::max((Real)0, 1 - cosTheta * cosTheta));
        Vector3f wh = sphericalDirection(sinTheta, cosTheta, phi);
        if (!sameHemisphere(wo, wh)) wh = -wh;
        return wh;
    } else {
        // Sample visible area of normals for Beckmann distribution
        Vector3f wh;
        bool flip = wo.z < 0;
        wh = beckmannSample(flip ? -wo : wo, alphax, alphay, u[0], u[1]);
        if (flip) wh = -wh;
        return wh;
    }
}

static void trowbridgeReitzSample11(Real cosTheta, Real U1, Real U2,
                                    Real* slope_x, Real* slope_y) {
    // Special case (normal incidence)
    if (cosTheta > .9999) {
        Real r = sqrt(U1 / (1 - U1));
        Real phi = 6.28318530718 * U2;
        *slope_x = r * cos(phi);
        *slope_y = r * sin(phi);
        return;
    }

    Real sinTheta = std::sqrt(std::max((Real)0, (Real)1 - cosTheta * cosTheta));
    Real tanTheta = sinTheta / cosTheta;
    Real a = 1 / tanTheta;
    Real G1 = 2 / (1 + std::sqrt(1.f + 1.f / (a * a)));

    // Sample slope_x
    Real A = 2 * U1 / G1 - 1;
    Real tmp = 1.f / (A * A - 1.f);
    if (tmp > 1e10) tmp = 1e10;
    Real B = tanTheta;
    Real D = std::sqrt(std::max(Real(B * B * tmp * tmp - (A * A - B * B) * tmp), Real(0)));
    Real slope_x_1 = B * tmp - D;
    Real slope_x_2 = B * tmp + D;
    *slope_x = (A < 0 || slope_x_2 > 1.f / tanTheta) ? slope_x_1 : slope_x_2;

    // Sample slope_y
    Real S;
    if (U2 > 0.5f) {
        S = 1.f;
        U2 = 2.f * (U2 - .5f);
    } else {
        S = -1.f;
        U2 = 2.f * (.5f - U2);
    }
    Real z = (U2 * (U2 * (U2 * 0.27385f - 0.73369f) + 0.46341f)) /
             (U2 * (U2 * (U2 * 0.093073f + 0.309420f) - 1.000000f) + 0.597999f);
    *slope_y = S * z * std::sqrt(1.f + *slope_x * *slope_x);

    ASSERT(!std::isinf(*slope_y));
    ASSERT(!std::isnan(*slope_y));
}

static Vector3f trowbridgeReitzSample(const Vector3f& wi, Real alpha_x,
                                      Real alpha_y, Real U1, Real U2) {
    // 1. Stretch wi
    Vector3f wiStretched = normalize(Vector3f(alpha_x * wi.x, alpha_y * wi.y, wi.z));

    // 2. Simulate P22_{wi}(x_slope, y_slope, 1, 1)
    Real slope_x, slope_y;
    trowbridgeReitzSample11(cosTheta(wiStretched), U1, U2, &slope_x, &slope_y);

    // 3. Rotate
    Real tmp = cosPhi(wiStretched) * slope_x - sinPhi(wiStretched) * slope_y;
    slope_y = sinPhi(wiStretched) * slope_x + cosPhi(wiStretched) * slope_y;
    slope_x = tmp;

    // 4. Unstretch
    slope_x = alpha_x * slope_x;
    slope_y = alpha_y * slope_y;

    // 5. compute normal
    return normalize(Vector3f(-slope_x, -slope_y, 1.));
}

Vector3f TrowbridgeReitzDistribution::sample_wh(const Vector3f& wo, const Point2f& u) const {
    Vector3f wh;
    if (!sampleVisibleArea) {
        Real cosTheta = 0, phi = (2 * Pi) * u[1];
        if (alphax == alphay) {
            Real tanTheta2 = alphax * alphax * u[0] / (1.0f - u[0]);
            cosTheta = 1 / std::sqrt(1 + tanTheta2);
        } else {
            phi =
                std::atan(alphay / alphax * std::tan(2 * Pi * u[1] + .5f * Pi));
            if (u[1] > .5f) phi += Pi;
            Real sinPhi = std::sin(phi), cosPhi = std::cos(phi);
            const Real alphax2 = alphax * alphax, alphay2 = alphay * alphay;
            const Real alpha2 =
                1 / (cosPhi * cosPhi / alphax2 + sinPhi * sinPhi / alphay2);
            Real tanTheta2 = alpha2 * u[0] / (1 - u[0]);
            cosTheta = 1 / std::sqrt(1 + tanTheta2);
        }
        Real sinTheta =
            std::sqrt(std::max((Real)0., (Real)1. - cosTheta * cosTheta));
        wh = sphericalDirection(sinTheta, cosTheta, phi);
        if (!sameHemisphere(wo, wh)) wh = -wh;
    } else {
        bool flip = wo.z < 0;
        wh = trowbridgeReitzSample(flip ? -wo : wo, alphax, alphay, u[0], u[1]);
        if (flip) wh = -wh;
    }
    return wh;
}

Real MicrofacetDistribution::pdf(const Vector3f& wo, const Vector3f& wh) const {
    if (sampleVisibleArea)
        return d(wh) * g1(wo) * absDot(wo, wh) / absCosTheta(wo);
    else
        return d(wh) * absCosTheta(wh);
}

}  // namespace phyr
