#include <core/geometry/shape.h>
#include <core/geometry/interaction.h>

namespace phyr {

/**
 * Just checks for a ray intersection without having to fill in any details
 */
bool Shape::intersectRay(const Ray& ray, bool testAlpha) const {
    Real t0 = ray.tMax;
    SurfaceInteraction si;
    return intersectRay(ray, &t0, &si, testAlpha);
}

Interaction Shape::sample(const Interaction& ref, const Point2f& u,
                          Real* pdf) const {
    Interaction intr = sample(u, pdf);
    Vector3f wi = intr.p - ref.p;
    if (wi.lengthSquared() == 0)
        *pdf = 0;
    else {
        wi = normalize(wi);
        // Convert from area measure, as returned by the Sample() call
        // above, to solid angle measure.
        *pdf *= distanceSquared(ref.p, intr.p) / absDot(intr.n, -wi);
        if (std::isinf(*pdf)) *pdf = 0.f;
    }
    return intr;
}

Real Shape::pdf(const Interaction& ref, const Vector3f& wi) const {
    // Intersect sample ray with area light geometry
    Ray ray = ref.emitRay(wi);
    Real tHit;
    SurfaceInteraction isectLight;

    // Ignore any alpha textures used for trimming the shape when performing
    // this intersection.
    if (!intersectRay(ray, &tHit, &isectLight)) return 0;

    // Convert light sample weight to solid angle measure
    Real pdf = distanceSquared(ref.p, isectLight.p) / (absDot(isectLight.n, -wi) * surfaceArea());
    if (std::isinf(pdf)) pdf = 0.f;
    return pdf;
}

} // namespace phyr
