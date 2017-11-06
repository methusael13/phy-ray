#ifndef PHYRAY_CORE_MICROFACET_H
#define PHYRAY_CORE_MICROFACET_H

#include <core/phyr.h>
#include <core/geometry/geometry.h>

namespace phyr {

// MicrofacetDistribution Declarations
class MicrofacetDistribution {
  public:
    // Interface
    virtual ~MicrofacetDistribution();

    /**
     * Returns the differential area of microfacets oriented
     * with the given given normal vector {wh}
     */
    virtual Real d(const Vector3f& wh) const = 0;
    virtual Real lambda(const Vector3f& w) const = 0;

    Real g1(const Vector3f& w) const { return 1 / (1 + lambda(w)); }
    virtual Real g(const Vector3f& wo, const Vector3f& wi) const {
        return 1 / (1 + lambda(wo) + lambda(wi));
    }
    virtual Vector3f sample_wh(const Vector3f& wo, const Point2f& u) const = 0;

    Real pdf(const Vector3f& wo, const Vector3f& wh) const;

  protected:
    MicrofacetDistribution(bool sampleVisibleArea)
        : sampleVisibleArea(sampleVisibleArea) {}

    const bool sampleVisibleArea;
};

/**
 * @brief The BeckmannDistribution class
 * Implements the microfacet distribution function based on a
 * gaussian distribution of microfacet slopes by Beckmann and
 * Spizzichino (1963).
 */
class BeckmannDistribution : public MicrofacetDistribution {
  public:
    // Interface
    static Real roughnessToAlpha(Real roughness) {
        roughness = std::max(roughness, (Real)1e-3);
        Real x = std::log(roughness);
        return 1.62142f + 0.819955f * x + 0.1734f * x * x +
               0.0171201f * x * x * x + 0.000640711f * x * x * x * x;
    }

    BeckmannDistribution(Real alphax, Real alphay, bool samplevis = true)
        : MicrofacetDistribution(samplevis), alphax(alphax), alphay(alphay) {}

    Real d(const Vector3f& wh) const override;
    Vector3f sample_wh(const Vector3f& wo, const Point2f& u) const override;

  private:
    Real lambda(const Vector3f& w) const override;

    const Real alphax, alphay;
};

class TrowbridgeReitzDistribution : public MicrofacetDistribution {
  public:
    // Interface
    static inline Real roughnessToAlpha(Real roughness);

    TrowbridgeReitzDistribution(Real alphax, Real alphay,
                                bool samplevis = true)
        : MicrofacetDistribution(samplevis), alphax(alphax), alphay(alphay) {}

    Real d(const Vector3f& wh) const override;
    Vector3f sample_wh(const Vector3f& wo, const Point2f& u) const override;

  private:
    Real lambda(const Vector3f& w) const override;

    const Real alphax, alphay;
};

// MicrofacetDistribution inline methods
inline Real TrowbridgeReitzDistribution::roughnessToAlpha(Real roughness) {
    roughness = std::max(roughness, (Real)1e-3);
    Real x = std::log(roughness);
    return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
           0.000640711f * x * x * x * x;
}

}  // namespace phyr

#endif
