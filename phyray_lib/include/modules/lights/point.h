#ifndef PHYRAY_MODULES_POINTLIGHT_H
#define PHYRAY_MODULES_POINTLIGHT_H

#include <core/phyr.h>
#include <core/light/light.h>
#include <core/geometry/geometry.h>
#include <core/color/spectrum.h>

#include <memory>

namespace phyr {

// PointLight Declarations
class PointLight : public Light {
  public:
    PointLight(const Transform& lightToWorld, const Spectrum& I)
        : Light((int)LightFlags::DELTA_POSITION, lightToWorld),
          pLight(lightToWorld(Point3f(0, 0, 0))), I(I) {}

    // Interface
    Spectrum power() const override;

    Spectrum sample_li(const Interaction& ref, const Point2f& u, Vector3f* wi,
                       Real* pdf, VisibilityTester* vis) const override;
    Spectrum sample_le(const Point2f& u1, const Point2f& u2,
                       Ray* ray, Normal3f* nLight, Real* pdfPos,
                       Real* pdfDir) const override;

    Real pdf_li(const Interaction&, const Vector3f&) const override;
    void pdf_le(const Ray&, const Normal3f&, Real* pdfPos,
                Real* pdfDir) const override;

  private:
    // PointLight Private Data
    const Point3f pLight;
    const Spectrum I;
};

std::shared_ptr<PointLight> createPointLight(const Transform& lightToworld,
                                             Real I = 1, Real scale = 1,
                                             const Point3f& point = Point3f(0, 0, 0));

}  // namespace phyr

#endif
