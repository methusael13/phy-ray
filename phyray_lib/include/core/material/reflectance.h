#ifndef PHYRAY_CORE_REFLECTANCE_H
#define PHYRAY_CORE_REFLECTANCE_H

#include <core/phyr.h>
#include <core/color/spectrum.h>
#include <core/geometry/geometry.h>

namespace phyr {

// BSDF inline functions
inline Real cosTheta(const Vector3f& w) { return w.z; }
inline Real cosSqTheta(const Vector3f& w) { return w.z * w.z; }
inline Real absCosTheta(const Vector3f& w) { return std::abs(w.z); }

inline Real sinSqTheta(const Vector3f& w) {
    return std::max(Real(0), 1 - cosSqTheta(w));
}
inline Real sinTheta(const Vector3f& w) { return std::sqrt(sinSqTheta(w)); }

inline Real tanTheta(const Vector3f& w) { return sinTheta(w) / cosTheta(w); }
inline Real tanSqTheta(const Vector3f& w) { return sinSqTheta(w) / cosSqTheta(w); }

inline Real cosPhi(const Vector3f& w) {
    Real _sinTheta = sinTheta(w);
    return (_sinTheta == 0) ? 1 : clamp(w.x / _sinTheta, -1, 1);
}
inline Real sinPhi(const Vector3f& w) {
    Real _sinTheta = sinTheta(w);
    return (_sinTheta == 0) ? 0 : clamp(w.y / _sinTheta, -1, 1);
}

inline Real cosSqPhi(const Vector3f& w) {
    Real _cosPhi = cosPhi(w);
    return _cosPhi * _cosPhi;
}
inline Real sinSqPhi(const Vector3f& w) {
    Real _sinPhi = sinPhi(w);
    return _sinPhi * _sinPhi;
}

inline Real cosDeltaPhi(const Vector3f& wa, const Vector3f& wb) {
    Real dotp = wa.x * wb.x + wa.y * wb.y;
    Real waLen = wa.x * wa.x + wa.y * wa.y;
    Real wbLen = wb.x * wb.x + wb.y * wb.y;

    return clamp(dotp / std::sqrt(waLen * wbLen), -1, 1);
}

inline Vector3f reflect(const Vector3f& wo, const Vector3f& n) {
    return -wo + 2 * dot(wo, n) * n;
}

inline bool refract(const Vector3f& wi, const Normal3f& n, Real eta,
                    Vector3f* wt) {
    Real cosThetaI = dot(n, wi);
    Real sinSqThetaI = std::max(Real(0), 1 - cosThetaI * cosThetaI);
    Real sinSqThetaT = eta * eta * sinSqThetaI;

    if (sinSqThetaT >= 1) return false;
    Real cosThetaT = std::sqrt(std::max(Real(0), 1 - sinSqThetaT));
    *wt = eta * -wi + (eta * cosThetaI - cosThetaT) * Vector3f(n);
    return true;
}

// BxDF utility functions
Real frDielectric(Real cosThetaI, Real etaI, Real etaT);
Spectrum frConductor(Real cosThetaI, const Spectrum& etaI,
                     const Spectrum& etaT, const Spectrum& k);

// BxDF Types
enum BxDFType {
    BSDF_REFLECTION     = 1 << 0,
    BSDF_TRANSMISSION   = 1 << 1,
    BSDF_DIFFUSE        = 1 << 2,
    BSDF_GLOSSY         = 1 << 3,
    BSDF_SPECULAR       = 1 << 4,
    BSDF_ALL            = BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_DIFFUSE |
                          BSDF_GLOSSY | BSDF_SPECULAR
};

// BxDF declarations
class BxDF {
  public:
    // Constructor
    BxDF(BxDFType type) : type(type) {}

    // Interface
    bool matchesFlags(BxDFType _type) const { return (type & _type) == type; }

    /**
     * Returns the value of the distribution function for the given pair of directions
     */
    virtual Spectrum f(const Vector3f& wo, const Vector3f& wi) const = 0;

    /**
     * Computes the direction of incident light {wi} given an outgoing
     * direction {wo} and returns the value of the BxDF for the pair of directions.
     */
    virtual Spectrum sample_f(const Vector3f& wo, Vector3f* wi, const Point2f& sample,
                              Real* pdf, BxDFType* sampledType = nullptr) const;

    /**
     * Computes the hemispherical-direction reflectance function that gives the total
     * reflection in a given direction due to constant illumination over the hemisphere.
     * The parameters {nSamples} and {samples} are used by implementations that use
     * Monte-Carlo integration to calculate an approximation for this function.
     */
    virtual Spectrum rho(const Vector3f& wo, int nSamples, const Point2f* samples) const;

    /**
     * Computes the hemispherical-hemispherical spectral value that gives the fraction of
     * incident light reflected by a surface when the incident light is same from all directions.
     */
    virtual Spectrum rho(int nSamples, const Point2f* samples1, const Point2f* samples2) const;

    /**
     * Computes the probability density function for a given pair of directions
     */
    virtual Real pdf(const Vector3f& wo, const Vector3f& wi) const;

    // Member data
    const BxDFType type;
};


// ScaledBxDF declarations
/**
 * The ScaledBxDF class takes a given BxDF and scales its contribution
 * with a Spectrum value. This class is later used by MixMaterial which
 * creates BSDFs based on a weighted combination of two other materials
 */
class ScaledBxDF : public BxDF {
  public:
    ScaledBxDF(BxDF* bxdf, const Spectrum& scale) :
        BxDF(bxdf->type), bxdf(bxdf), scale(scale) {}

    Spectrum f(const Vector3f& wo, const Vector3f& wi) const {
        return scale * bxdf->f(wo, wi);
    }
    Spectrum sample_f(const Vector3f& wo, Vector3f* wi, const Point2f& sample,
                      Real* pdf, BxDFType* sampledType) const {
        return scale * bxdf->sample_f(wo, wi, sample, pdf, sampledType);
    }

    Real pdf(const Vector3f& wo, const Vector3f& wi) const {
        return bxdf->pdf(wo, wi);
    }

    Spectrum rho(const Vector3f& wo, int nSamples, const Point2f* samples) const {
        return scale * bxdf->rho(wo, nSamples, samples);
    }

    Spectrum rho(int nSamples, const Point2f* samples1, const Point2f* samples2) const {
        return scale * bxdf->rho(nSamples, samples1, samples2);
    }

  private:
    BxDF* bxdf;
    Spectrum scale;
};


// Fresnel declarations
class Fresnel {
  public:
    /**
     * Returns the amount of light reflected by the surface given the cosine
     * of the angle made by the incoming direction and the surface normal.
     */
    virtual Spectrum evaluate(Real cosThetaI) const = 0;
    virtual ~Fresnel() {}
};

class FresnelConductor : public Fresnel {
  public:
    FresnelConductor(const Spectrum& etaI, const Spectrum etaT, const Spectrum& k) :
        etaI(etaI), etaT(etaT), k(k) {}

    // Interaface
    Spectrum evaluate(Real cosThetaI) const override;

  private:
    Spectrum etaI, etaT, k;
};

class FresnelDielectric : public Fresnel {
  public:
    FresnelDielectric(Real etaI, Real etaT) :
        etaI(etaI), etaT(etaT) {}

    // Interface
    Spectrum evaluate(Real cosThetaI) const override;

  private:
    Real etaI, etaT;
};

/**
 * The FresnelPureReflect class returns 100% reflection for all incoming directions
 */
class FresnelPureReflect : public Fresnel {
  public:
    // Interface
    Spectrum evaluate(Real) const { return Spectrum(1); }
};


// SpecularReflection declarations
class SpecularReflection : public BxDF {
  public:
    SpecularReflection(const Spectrum& R, const Fresnel* fresnel) :
        BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR)),
        R(R), fresnel(fresnel) {}

    // Interface

    /**
     * No scaterring is returned, since for an arbitrary pair of directions the
     * delta function returns no scattering. This certainly isn't true for the case
     * when {wo} is a perfect mirror reflection direction of {wi}. It's alright however,
     * since such reflectance functions involving singularities with delta distributions
     * receive special handling by the light transport routines.
     */
    Spectrum f(const Vector3f& wo, const Vector3f& wi) const {
        return Spectrum(Real(0));
    }

    Spectrum sample_f(const Vector3f& wo, Vector3f* wi, const Point2f& sample,
                      Real* pdf, BxDFType* sampledType) const;

  private:
    // Used to scale the reflected color
    const Spectrum R;
    // Describes dielectric or conductor fresnel properties
    const Fresnel* fresnel;
};

}  // namespace phyr

#endif