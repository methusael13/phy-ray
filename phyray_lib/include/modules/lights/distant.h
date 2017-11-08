#ifndef PHYRAY_MODULES_DISTANTLIGHT_H
#define PHYRAY_MODULES_DISTANTLIGHT_H

#include <core/phyr.h>
#include <core/scene.h>
#include <core/light/light.h>
#include <core/geometry/geometry.h>
#include <core/color/spectrum.h>

#include <memory>

namespace phyr {

// DistantLight Declarations
class DistantLight : public Light {
  public:
    DistantLight(const Transform& lightToWorld, const Spectrum& L, const Vector3f& w);

    void preprocess(const Scene& scene) override {
        scene.getWorldBounds().boundingSphere(&worldCenter, &worldRadius);
    }

    Real pdf_li(const Interaction&, const Vector3f&) const override;
    Spectrum sample_li(const Interaction& ref, const Point2f& u, Vector3f* wi,
                       Real* pdf, VisibilityTester* vis) const override;

    Spectrum power() const override;

    void pdf_le(const Ray&, const Normal3f&, Real* pdfPos, Real* pdfDir) const override;
    Spectrum sample_le(const Point2f& u1, const Point2f& u2,
                       Ray* ray, Normal3f* nLight, Real* pdfPos,
                       Real* pdfDir) const override;

  private:
    const Spectrum L;
    const Vector3f wLight;

    Point3f worldCenter;
    Real worldRadius;
};

std::shared_ptr<DistantLight> createDistantLight(const Transform& light2world,
                                                 Real L = 1, Real scale = 1,
                                                 const Point3f& from = Point3f(0, 0, 0),
                                                 const Point3f& to = Point3f(0, 0, 1));

}  // namespace phyr

#endif
