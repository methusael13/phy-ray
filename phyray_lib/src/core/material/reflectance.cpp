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

// BxDF definitions
Spectrum BxDF::sample_f(const Vector3f& wo, Vector3f* wi, const Point2f& u,
                        Real* pdf, BxDFType* sampledType) const {
    // Cosine-sample the hemisphere, flipping the direction if necessary
    *wi = cosineSampleHemisphere(u);
    if (wo.z < 0) wi->z *= -1;

    *pdf = BxDF::pdf(wo, *wi);
    return f(wo, *wi);
}

Real BxDF::pdf(const Vector3f& wo, const Vector3f& wi) const {
    return sameHemisphere(wo, wi) ? absCosTheta(wi) * InvPi : 0;
}

Spectrum BxDF::rho(const Vector3f& w, int nSamples, const Point2f* u) const {
    Spectrum r(0.);
    for (int i = 0; i < nSamples; ++i) {
        Vector3f wi; Real pdf = 0;
        Spectrum f = sample_f(w, &wi, u[i], &pdf);
        if (pdf > 0) r += f * absCosTheta(wi) / pdf;
    }

    return r / nSamples;
}

Spectrum BxDF::rho(int nSamples, const Point2f* u1, const Point2f* u2) const {
    Spectrum r(0.f);

    for (int i = 0; i < nSamples; ++i) {
        Vector3f wo, wi;
        wo = uniformSampleHemisphere(u1[i]);
        Real pdfo = uniformHemispherePdf(), pdfi = 0;

        Spectrum f = sample_f(wo, &wi, u2[i], &pdfi);
        if (pdfi > 0)
            r += f * absCosTheta(wi) * absCosTheta(wo) / (pdfo * pdfi);
    }

    return r / (Pi * nSamples);
}

// BSDF definitions
Spectrum BSDF::rho(int nSamples, const Point2f* samples1,
                   const Point2f* samples2, BxDFType flags) const {
    Spectrum ret(0.f);
    for (int i = 0; i < nBxDFs; ++i)
        if (bxdfs[i]->matchesFlags(flags))
            ret += bxdfs[i]->rho(nSamples, samples1, samples2);
    return ret;
}

Spectrum BSDF::rho(const Vector3f& wo, int nSamples, const Point2f* samples,
                   BxDFType flags) const {
    Spectrum ret(0.f);
    for (int i = 0; i < nBxDFs; ++i)
        if (bxdfs[i]->matchesFlags(flags))
            ret += bxdfs[i]->rho(wo, nSamples, samples);
    return ret;
}

Spectrum BSDF::f(const Vector3f& woW, const Vector3f& wiW, BxDFType flags) const {
    Vector3f wi = worldToLocal(wiW), wo = worldToLocal(woW);
    if (wo.z == 0) return 0.;

    bool reflect = dot(wiW, ng) * dot(woW, ng) > 0;
    Spectrum f(0.f);

    for (int i = 0; i < nBxDFs; ++i)
        if (bxdfs[i]->matchesFlags(flags) &&
            ((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
             (!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
            f += bxdfs[i]->f(wo, wi);
    return f;
}

Spectrum BSDF::sample_f(const Vector3f& woWorld, Vector3f* wiWorld,
                        const Point2f& u, Real* pdf, BxDFType type,
                        BxDFType* sampledType) const {
    // Choose which {BxDF} to sample
    int matchingComps = numComponents(type);
    if (matchingComps == 0) {
        *pdf = 0;
        if (sampledType) *sampledType = BxDFType(0);
        return Spectrum(0);
    }
    int comp = std::min((int)std::floor(u[0] * matchingComps), matchingComps - 1);

    // Get {BxDF} pointer for chosen component
    int count = comp;
    BxDF *bxdf = nullptr;
    for (int i = 0; i < nBxDFs; ++i) {
        if (bxdfs[i]->matchesFlags(type) && count-- == 0) {
            bxdf = bxdfs[i];
            break;
        }
    }
    ASSERT(bxdf != nullptr);

    // Remap {BxDF} sample {u} to [0,1)^2
    Point2f uRemapped(std::min(u[0] * matchingComps - comp, OneMinusEpsilon), u[1]);

    // Sample chosen {BxDF}
    Vector3f wi, wo = worldToLocal(woWorld);
    if (wo.z == 0) return 0.;
    *pdf = 0;

    if (sampledType) *sampledType = bxdf->type;
    Spectrum f = bxdf->sample_f(wo, &wi, uRemapped, pdf, sampledType);

    if (*pdf == 0) {
        if (sampledType) *sampledType = BxDFType(0);
        return 0;
    }
    *wiWorld = localToWorld(wi);

    // Compute overall PDF with all matching {BxDF}s
    if (!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1)
        for (int i = 0; i < nBxDFs; ++i)
            if (bxdfs[i] != bxdf && bxdfs[i]->matchesFlags(type))
                *pdf += bxdfs[i]->pdf(wo, wi);
    if (matchingComps > 1) *pdf /= matchingComps;

    // Compute value of BSDF for sampled direction
    if (!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1) {
        bool reflect = dot(*wiWorld, ng) * dot(woWorld, ng) > 0;
        f = 0.;
        for (int i = 0; i < nBxDFs; ++i) {
            if (bxdfs[i]->matchesFlags(type) &&
                ((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
                 (!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
                f += bxdfs[i]->f(wo, wi);
        }
    }

    return f;
}

Real BSDF::pdf(const Vector3f& woWorld, const Vector3f& wiWorld, BxDFType flags) const {
    if (nBxDFs == 0.f) return 0.f;
    Vector3f wo = worldToLocal(woWorld), wi = worldToLocal(wiWorld);
    if (wo.z == 0) return 0.;

    Real pdf = 0.f;
    int matchingComps = 0;

    for (int i = 0; i < nBxDFs; ++i) {
        if (bxdfs[i]->matchesFlags(flags)) {
            ++matchingComps;
            pdf += bxdfs[i]->pdf(wo, wi);
        }
    }

    Real v = matchingComps > 0 ? pdf / matchingComps : 0.f;
    return v;
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

// OrenNayar definitions
Spectrum OrenNayar::f(const Vector3f& wo, const Vector3f& wi) const {
    Real sinThetaI = sinTheta(wi);
    Real sinThetaO = sinTheta(wo);

    // Compute cosine term of Oren-Nayar model
    Real maxCos = 0;
    if (sinThetaI > 1e-4 && sinThetaO > 1e-4) {
        Real sinPhiI = sinPhi(wi), cosPhiI = cosPhi(wi);
        Real sinPhiO = sinPhi(wo), cosPhiO = cosPhi(wo);
        Real dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
        maxCos = std::max((Real)0, dCos);
    }

    // Compute sine and tangent terms of Oren-Nayar model
    Real sinAlpha, tanBeta;
    if (absCosTheta(wi) > absCosTheta(wo)) {
        sinAlpha = sinThetaO;
        tanBeta = sinThetaI / absCosTheta(wi);
    } else {
        sinAlpha = sinThetaI;
        tanBeta = sinThetaO / absCosTheta(wo);
    }

    return R * InvPi * (A + B * maxCos * sinAlpha * tanBeta);
}

// Microfacet definitions
Spectrum MicrofacetReflection::f(const Vector3f& wo, const Vector3f& wi) const {
    Real cosThetaO = absCosTheta(wo), cosThetaI = absCosTheta(wi);
    Vector3f wh = wi + wo;

    // Handle degenerate cases for microfacet reflection
    if (cosThetaI == 0 || cosThetaO == 0) return Spectrum(0.);
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return Spectrum(0.);
    wh = normalize(wh);
    Spectrum F = fresnel->evaluate(dot(wi, wh));

    return R * distribution->d(wh) * distribution->g(wo, wi) * F / (4 * cosThetaI * cosThetaO);
}

Spectrum MicrofacetReflection::sample_f(const Vector3f& wo, Vector3f* wi,
                                        const Point2f& u, Real* pdf,
                                        BxDFType* sampledType) const {
    // Sample microfacet orientation {wh} and reflected direction {wi}
    if (wo.z == 0) return 0.;
    Vector3f wh = distribution->sample_wh(wo, u);
    *wi = reflect(wo, wh);
    if (!sameHemisphere(wo, *wi)) return Spectrum(0.f);

    // Compute PDF of {wi} for microfacet reflection
    *pdf = distribution->pdf(wo, wh) / (4 * dot(wo, wh));
    return f(wo, *wi);
}

Real MicrofacetReflection::pdf(const Vector3f& wo, const Vector3f& wi) const {
    if (!sameHemisphere(wo, wi)) return 0;
    Vector3f wh = normalize(wo + wi);
    return distribution->pdf(wo, wh) / (4 * dot(wo, wh));
}

Spectrum MicrofacetTransmission::f(const Vector3f& wo, const Vector3f& wi) const {
    // Allow transmission only
    if (sameHemisphere(wo, wi)) return 0;

    Real cosThetaO = cosTheta(wo);
    Real cosThetaI = cosTheta(wi);
    if (cosThetaI == 0 || cosThetaO == 0) return Spectrum(0);

    // Compute {wh} from {wo} and {wi} for microfacet transmission
    Real eta = cosTheta(wo) > 0 ? (etaB / etaA) : (etaA / etaB);
    Vector3f wh = normalize(wo + wi * eta);
    if (wh.z < 0) wh = -wh;

    Spectrum F = fresnel.evaluate(dot(wo, wh));
    Real sqrtDenom = dot(wo, wh) + eta * dot(wi, wh);
    Real factor = (mode == TransportMode::Radiance) ? (1 / eta) : 1;

    return (Spectrum(1.f) - F) * T *
           std::abs(distribution->d(wh) * distribution->g(wo, wi) * eta * eta *
                    absDot(wi, wh) * absDot(wo, wh) * factor * factor /
                    (cosThetaI * cosThetaO * sqrtDenom * sqrtDenom));
}

Spectrum MicrofacetTransmission::sample_f(const Vector3f& wo, Vector3f* wi,
                                          const Point2f& u, Real* pdf,
                                          BxDFType* sampledType) const {
    if (wo.z == 0) return 0.;
    Vector3f wh = distribution->sample_wh(wo, u);
    Real eta = cosTheta(wo) > 0 ? (etaA / etaB) : (etaB / etaA);

    if (!refract(wo, (Normal3f)wh, eta, wi)) return 0;
    *pdf = MicrofacetTransmission::pdf(wo, *wi);
    return f(wo, *wi);
}

Real MicrofacetTransmission::pdf(const Vector3f& wo, const Vector3f& wi) const {
    if (sameHemisphere(wo, wi)) return 0;
    // Compute {wh} from {wo} and {wi} for microfacet transmission
    Real eta = cosTheta(wo) > 0 ? (etaB / etaA) : (etaA / etaB);
    Vector3f wh = normalize(wo + wi * eta);

    // Compute change of variables {dwh_dwi} for microfacet transmission
    Real sqrtDenom = dot(wo, wh) + eta * dot(wi, wh);
    Real dwh_dwi = std::abs((eta * eta * dot(wi, wh)) / (sqrtDenom * sqrtDenom));

    return distribution->pdf(wo, wh) * dwh_dwi;
}

}  // namepspace phyr
