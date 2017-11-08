#ifndef PHYRAY_CORE_LIGHT_H
#define PHYRAY_CORE_LIGHT_H

#include <core/phyr.h>
#include <core/geometry/transform.h>
#include <core/geometry/interaction.h>

namespace phyr {

// LightFlags declarations
enum class LightFlags : int {
    DELTA_POSITION      = 1 << 0,
    DELTA_DIRECTION     = 1 << 1,
    AREA                = 1 << 2,
    INFINITE            = 1 << 3
};

// LightFlags inline functions
inline bool isDeltaLight(int flags) {
    return (flags & int(LightFlags::DELTA_POSITION)) ||
           (flags & int(LightFlags::DELTA_DIRECTION));
}

class VisibilityTester;

class Light {
  public:
    // Constructor
    Light(int flags, const Transform& lightToWorld, int nSamples = 1) :
        flags(flags), nSamples(std::min(1, nSamples)),
        lightToWorld(lightToWorld), worldToLight(Transform::inverse(lightToWorld)) {}

    virtual ~Light();

    // Interface
    virtual Spectrum power() const = 0;
    virtual void preprocess(const Scene& scene) {}

    virtual Spectrum sample_li(const Interaction& ref, const Point2f& u,
                               Vector3f* wi, Real* pdf,
                               VisibilityTester* vis) const = 0;

    virtual Spectrum le(const Ray& ray) const;
    virtual Spectrum sample_le(const Point2f& u1, const Point2f& u2,
                               Ray* ray, Normal3f* nLight, Real* pdfPos,
                               Real* pdfDir) const = 0;

    virtual Real pdf_li(const Interaction& ref, const Vector3f& wi) const = 0;
    virtual void pdf_le(const Ray& ray, const Normal3f& nLight, Real* pdfPos,
                        Real* pdfDir) const = 0;

    /**
     * Indicates the fundamental light source type - for instance,
     * whether or not the light is described by a delta distribution.
     * Monte-Carlo uses this info to apply appropriate computations.
     */
    const int flags;

    /**
     * Used for area lights when it is desirable to trace multiple
     * shadow rays to the light to compute soft shadows.
     */
    const int nSamples;

    /**
     * Transform to convert from local space to world space and vice versa
     */
    const Transform lightToWorld, worldToLight;
};

// VisibilityTester declarations
class VisibilityTester {
  public:
    VisibilityTester() {}
    VisibilityTester(const Interaction& p0, const Interaction& p1) :
        p0(p0), p1(p1) {}

    // Interface
    const Interaction& getP0() const { return p0; }
    const Interaction& getP1() const { return p1; }

    bool unoccluded(const Scene& scene) const;
    Spectrum tr(const Scene& scene, Sampler& sampler) const;

  private:
    Interaction p0, p1;
};

// AreaLight declarations
class AreaLight : public Light {
  public:
    // Interface
    AreaLight(const Transform& lightToWorld, int nSamples) :
        Light((int)LightFlags::AREA, lightToWorld, nSamples) {}

    // Interface
    virtual Spectrum l(const Interaction& intr, const Vector3f& w) const = 0;
};

}  // namespace phyr

#endif
