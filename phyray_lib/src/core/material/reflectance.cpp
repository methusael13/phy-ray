#include <core/integrator/sampling.h>
#include <core/material/reflectance.h>

namespace phyr {

// BxDF utility functions
Real frDielectric(Real cosThetaI, Real etaI, Real etaT) {
    // Make sure cosThetaI is within expected range
    cosThetaI = clamp(cosThetaI, -1, 1);

    // Potentially swap indices of refraction.
    // If {cosThetaI} is between 0 and 1, the ray is entering
    // the medium and it's exiting the medium if the value is
    // between -1 and 0, which is when we need to swap the eta values.
    bool rayEntering = cosThetaI > 0;
    if (!rayEntering) {
        std::swap(etaI, etaT);
        cosThetaI = std::abs(cosThetaI);
    }

    // Compute {cosThetaT} using Snell's law
    Real sinThetaI = std::sqrt(std::max(Real(0), 1 - cosThetaI * cosThetaI));
    Real sinThetaT = etaI * sinThetaI / etaT;

    // No fresnel effect on total internal reflection
    if (sinThetaT >= 1) return 1;
    Real cosThetaT = std::sqrt(std::max(Real(0), 1 - sinThetaT * sinThetaT));

    // Apply the fresnel reflectance formulae for dielectrics
    Real rParl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                 ((etaT * cosThetaI) + (etaI * cosThetaT));
    Real rPerp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
                 ((etaI * cosThetaI) + (etaT * cosThetaT));

    return (rParl * rParl + rPerp * rPerp) * 0.5;
}

// Implementation from
// https://seblagarde.wordpress.com/2013/04/29/memo-on-fresnel-equations
Spectrum frConductor(Real cosThetaI, const Spectrum& etaI, const Spectrum& etaT,
                     const Spectrum& k) {
    cosThetaI = clamp(cosThetaI, -1, 1);
    Spectrum eta = etaT / etaI, etaK = k / etaI;

    Real cosThetaISq = cosThetaI * cosThetaI;
    Real sinThetaISq = 1 - cosThetaISq;
    Spectrum etaSq = eta * eta, etaKSq = etaK * etaK;

    Spectrum t0 = etaSq - etaKSq - sinThetaISq;
    Spectrum aSqPlusbSq = Spectrum::sqrt(t0 * t0 + 4 * etaSq + etaKSq);
    Spectrum t1 = aSqPlusbSq + cosThetaISq;
    Spectrum a = Spectrum::sqrt(0.5f * (aSqPlusbSq + t0));
    Spectrum t2 = Real(2) * cosThetaI * a;
    Spectrum Rs = (t1 - t2) / (t1 + t2);

    Spectrum t3 = cosThetaISq * aSqPlusbSq + sinThetaISq * sinThetaISq;
    Spectrum t4 = t2 * sinThetaISq;
    Spectrum Rp = Rs * (t3 - t4) / (t3 + t4);

    return (Rs + Rp) * 0.5;
}


// Fresnel definitions
Spectrum FresnelConductor::evaluate(Real cosThetaI) const {
    return frConductor(std::abs(cosThetaI), etaI, etaT, k);
}

Spectrum FresnelDielectric::evaluate(Real cosThetaI) const {
    return frDielectric(cosThetaI, etaI, etaT);
}


// SpecularReflection definitions
Spectrum SpecularReflection::sample_f(const Vector3f& wo, Vector3f* wi,
                                      const Point2f& sample, Real* pdf,
                                      BxDFType* sampledType) const {
    // Compute perfect specular reflection direction.
    // According to the shading coordinate space, n = (0, 0, 1),
    // so calculating the reflection vector for {wo} over {n}
    // is simplified to the following.
    *wi = Vector3f(-wo.x, -wo.y, wo.z);
    *pdf = 1;
    return fresnel->evaluate(cosTheta(*wi)) * R / absCosTheta(*wi);
}

// SpecularTransmission definitions
Spectrum SpecularTransmission::sample_f(const Vector3f& wo, Vector3f* wi,
                                        const Point2f& sample, Real* pdf,
                                        BxDFType* sampledType) const {
    // Determine which direction is incident and which is transmitted
    bool rayEntering = cosTheta(wo) > 0;
    Real _etaI = rayEntering ? etaI : etaT;
    Real _etaT = rayEntering ? etaT : etaI;

    // Compute ray drection for specular transmission
    if (!refract(wo, faceForward(Normal3f(0, 0, 1), wo), _etaI / _etaT, wi))
        return 0;

    *pdf = 1;
    Spectrum ft = T * (Spectrum(Real(0)) - fresnel.evaluate(cosTheta(*wi)));
    // Account for non-symmetry with transmission to different medium
    if (mode == TransportMode::Radiance)
        ft *= (_etaI * _etaI) / (_etaT * _etaT);
    return ft / absCosTheta(*wi);
}

// FresnelSpecular definitions
Spectrum FresnelSpecular::sample_f(const Vector3f& wo, Vector3f* wi, const Point2f& sample,
                                   Real* pdf, BxDFType* sampledType) const {
    Real f = frDielectric(cosTheta(wo), etaI, etaT);
    if (sample[0] < f) {
        // Compute specular reflection for {FresnelSpecular}

        // Compute perfect specular reflection direction
        *wi = Vector3f(-wo.x, -wo.y, wo.z);
        if (sampledType)
            *sampledType = BxDFType(BSDF_REFLECTION | BSDF_SPECULAR);
        *pdf = f;
        return f * R / absCosTheta(*wi);
    } else {
        // Compute specular transmission for {FresnelSpecular}

        // Determine which direction is incident and which is transmitted
        bool rayEntering = cosTheta(wo) > 0;
        Real _etaI = rayEntering ? etaI : etaT;
        Real _etaT = rayEntering ? etaT : etaI;

        // Compute ray drection for specular transmission
        if (!refract(wo, faceForward(Normal3f(0, 0, 1), wo), _etaI / _etaT, wi))
            return 0;
        Spectrum ft = T * (1 - f);

        // Account for non-symmetry with transmission to different medium
        if (mode == TransportMode::Radiance)
            ft *= (_etaI * _etaI) / (_etaT * _etaT);
        if (sampledType)
            *sampledType = BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR);
        *pdf = 1 - f;
        return ft / absCosTheta(*wi);
    }
}

// LambertianTransmission definitions
Real LambertianTransmission::pdf(const Vector3f& wo, const Vector3f& wi) const {
    return !sameHemisphere(wo, wi) ? absCosTheta(wi) * InvPi : 0;
}

Spectrum LambertianTransmission::sample_f(const Vector3f& wo, Vector3f* wi, const Point2f& sample,
                                          Real* pdf, BxDFType* sampledType) const {
    *wi = cosineSampleHemisphere(sample);
    // Make sure {wi} and {wo} are on different sides of the surface
    if (wo.z > 0) wi->z *= -1;

    *pdf = LambertianTransmission::pdf(wo, *wi);
    return f(wo, *wi);
}

}  // namepspace phyr
