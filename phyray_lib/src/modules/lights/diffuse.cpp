#include <modules/lights/diffuse.h>
#include <core/integrator/sampling.h>
#include <core/geometry/shape.h>

namespace phyr {

// DiffuseAreaLight Method Definitions
DiffuseAreaLight::DiffuseAreaLight(const Transform& lightToWorld,
                                   const Spectrum& Lemit, int nSamples,
                                   const std::shared_ptr<Shape>& shape,
                                   bool twoSided)
    : AreaLight(lightToWorld, nSamples), Lemit(Lemit),
      shape(shape), twoSided(twoSided), area(shape->surfaceArea()) {}

Spectrum DiffuseAreaLight::power() const { return (twoSided ? 2 : 1) * Lemit * area * Pi; }

Spectrum DiffuseAreaLight::sample_li(const Interaction& ref, const Point2f& u,
                                     Vector3f* wi, Real* pdf,
                                     VisibilityTester* vis) const {
    Interaction pShape = shape->sample(ref, u, pdf);
    if (*pdf == 0 || (pShape.p - ref.p).lengthSquared() == 0) {
        *pdf = 0;
        return 0.f;
    }
    *wi = normalize(pShape.p - ref.p);
    *vis = VisibilityTester(ref, pShape);
    return l(pShape, -*wi);
}

Real DiffuseAreaLight::pdf_li(const Interaction& ref, const Vector3f& wi) const {
    return shape->pdf(ref, wi);
}

Spectrum DiffuseAreaLight::sample_le(const Point2f& u1, const Point2f& u2,
                                     Ray* ray, Normal3f* nLight,
                                     Real* pdfPos, Real* pdfDir) const {
    // Sample a point on the area light's {Shape}, {pShape}
    Interaction pShape = shape->sample(u1, pdfPos);
    *nLight = pShape.n;

    // Sample a cosine-weighted outgoing direction {w} for area light
    Vector3f w;
    if (twoSided) {
        Point2f u = u2;
        // Choose a side to sample and then remap u[0] to [0,1] before
        // applying cosine-weighted hemisphere sampling for the chosen side.
        if (u[0] < .5) {
            u[0] = std::min(u[0] * 2, OneMinusEpsilon);
            w = cosineSampleHemisphere(u);
        } else {
            u[0] = std::min((u[0] - .5f) * 2, OneMinusEpsilon);
            w = cosineSampleHemisphere(u);
            w.z *= -1;
        }
        *pdfDir = 0.5f * cosineHemispherePdf(std::abs(w.z));
    } else {
        w = cosineSampleHemisphere(u2);
        *pdfDir = cosineHemispherePdf(w.z);
    }

    Vector3f v1, v2, n(pShape.n);
    coordinateSystem(n, &v1, &v2);
    w = w.x * v1 + w.y * v2 + w.z * n;
    *ray = pShape.emitRay(w);
    return l(pShape, w);
}

void DiffuseAreaLight::pdf_le(const Ray& ray, const Normal3f& n, Real* pdfPos,
                              Real* pdfDir) const {
    Interaction it(ray.o, n, Vector3f(n), Vector3f());
    *pdfPos = shape->pdf(it);
    *pdfDir = twoSided ? (.5 * cosineHemispherePdf(absDot(n, ray.d)))
                       : cosineHemispherePdf(dot(n, ray.d));
}

std::shared_ptr<AreaLight> createDiffuseAreaLight(const Transform& light2world,
                                                  const std::shared_ptr<Shape>& shape,
                                                  Real L, Real scale, int nSamples,
                                                  bool twoSided) {
    Spectrum _L(L), sc(scale);
    nSamples = std::max(1, nSamples);
    return std::make_shared<DiffuseAreaLight>(light2world, _L * sc,
                                              nSamples, shape, twoSided);
}

}  // namespace phyr
