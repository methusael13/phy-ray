#ifndef PHYRAY_MODULES_DIFFUSELIGHT_H
#define PHYRAY_MODULES_DIFFUSELIGHT_H

#include <core/phyr.h>
#include <core/scene.h>
#include <core/light/light.h>
#include <core/geometry/geometry.h>
#include <core/color/spectrum.h>

#include <memory>

namespace phyr {

// DiffuseAreaLight Declarations
class DiffuseAreaLight : public AreaLight {
  public:
    DiffuseAreaLight(const Transform& lightToWorld, const Spectrum& Le,
                     int nSamples, const std::shared_ptr<Shape>& shape,
                     bool twoSided = false);

    // Interface
    Spectrum l(const Interaction& intr, const Vector3f& w) const override {
        return (twoSided || dot(intr.n, w) > 0) ? Lemit : Spectrum(0.f);
    }

    Spectrum power() const override;
    Spectrum sample_li(const Interaction& ref, const Point2f& u, Vector3f* wo,
                       Real* pdf, VisibilityTester* vis) const override;
    Real pdf_li(const Interaction&, const Vector3f&) const override;

    Spectrum sample_le(const Point2f& u1, const Point2f& u2,
                       Ray* ray, Normal3f* nLight, Real* pdfPos,
                       Real* pdfDir) const override;
    void pdf_le(const Ray&, const Normal3f&, Real* pdfPos, Real* pdfDir) const override;

  protected:
    const Spectrum Lemit;
    std::shared_ptr<Shape> shape;
    const bool twoSided;
    const Real area;
};

std::shared_ptr<AreaLight> createDiffuseAreaLight(const Transform& light2world,
                                                  const std::shared_ptr<Shape>& shape,
                                                  Real L = 1, Real scale = 1, int nSamples = 1,
                                                  bool twoSided = false);
}  // namespace phyr

#endif
