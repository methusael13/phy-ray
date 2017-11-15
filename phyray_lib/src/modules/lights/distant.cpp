#include <modules/lights/distant.h>
#include <core/integrator/sampling.h>

namespace phyr {

// DistantLight Method Definitions
DistantLight::DistantLight(const Transform& lightToWorld, const Spectrum& L,
                           const Vector3f& wLight)
    : Light((int)LightFlags::DELTA_DIRECTION, lightToWorld), L(L),
      wLight(normalize(lightToWorld(wLight))) {}

Spectrum DistantLight::sample_li(const Interaction& ref, const Point2f& u,
                                 Vector3f* wi, Real* pdf,
                                 VisibilityTester* vis) const {
    *wi = wLight; *pdf = 1;
    Point3f pOutside = ref.p + wLight * (2 * worldRadius);
    *vis = VisibilityTester(ref, Interaction(pOutside));
    return L;
}

Spectrum DistantLight::power() const { return L * Pi * worldRadius * worldRadius; }

Real DistantLight::pdf_li(const Interaction&, const Vector3f&) const { return 0.f; }

Spectrum DistantLight::sample_le(const Point2f& u1, const Point2f& u2,
                                 Ray* ray, Normal3f* nLight,
                                 Real* pdfPos, Real* pdfDir) const {
    // Choose point on disk oriented toward infinite light direction
    Vector3f v1, v2;
    coordinateSystem(wLight, &v1, &v2);
    Point2f cd = concentricSampleDisk(u1);
    Point3f pDisk = worldCenter + worldRadius * (cd.x * v1 + cd.y * v2);

    // Set ray origin and direction for infinite light ray
    *ray = Ray(pDisk + worldRadius * wLight, -wLight, Infinity);
    *nLight = (Normal3f)ray->d;
    *pdfPos = 1 / (Pi * worldRadius * worldRadius);
    *pdfDir = 1;

    return L;
}

void DistantLight::pdf_le(const Ray&, const Normal3f&, Real* pdfPos,
                          Real* pdfDir) const {
    *pdfPos = 1 / (Pi * worldRadius * worldRadius);
    *pdfDir = 0;
}

std::shared_ptr<DistantLight> createDistantLight(Real L, Real scale,
                                                 const Point3f& from,
                                                 const Point3f& to) {
    Spectrum _L(L), sc(scale);
    Vector3f dir = to - from;

    return std::make_shared<DistantLight>(Transform(), _L * sc, dir);
}

}  // namespace phyr
