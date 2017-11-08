#include <modules/lights/point.h>
#include <core/integrator/sampling.h>

namespace phyr {

// PointLight Method Definitions
Spectrum PointLight::sample_li(const Interaction& ref, const Point2f &u,
                               Vector3f* wi, Real* pdf,
                               VisibilityTester* vis) const {
    *wi = normalize(pLight - ref.p);
    *pdf = 1.f;
    *vis = VisibilityTester(ref, Interaction(pLight));
    return I / distanceSquared(pLight, ref.p);
}

Spectrum PointLight::power() const { return 4 * Pi * I; }

Real PointLight::pdf_li(const Interaction&, const Vector3f&) const { return 0; }

Spectrum PointLight::sample_le(const Point2f& u1, const Point2f& u2,
                               Ray* ray, Normal3f* nLight, Real* pdfPos,
                               Real* pdfDir) const {
    *ray = Ray(pLight, uniformSampleSphere(u1), Infinity);
    *nLight = (Normal3f)ray->d;
    *pdfPos = 1;
    *pdfDir = uniformSpherePdf();
    return I;
}

void PointLight::pdf_le(const Ray&, const Normal3f&, Real* pdfPos,
                        Real* pdfDir) const {
    *pdfPos = 0;
    *pdfDir = uniformSpherePdf();
}

std::shared_ptr<PointLight> createPointLight(const Transform& light2world,
                                             Real I, Real scale, const Point3f& point) {
    Spectrum _I = Spectrum(I);
    Spectrum sc = Spectrum(scale);
    Point3f P = point;

    Transform l2w = Transform::translate(Vector3f(P.x, P.y, P.z)) * light2world;
    return std::make_shared<PointLight>(l2w, I * sc);
}

}  // namespace phyr
