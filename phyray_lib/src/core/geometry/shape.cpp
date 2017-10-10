#include <core/geometry/shape.h>
#include <core/geometry/interaction.h>

namespace phyr {

/**
 * Just checks for a ray intersection without having to fill in any details
 */
bool Shape::intersectRay(const Ray& ray) const {
    Real t0 = ray.tMax;
    SurfaceInteraction si;
    return intersectRay(ray, &t0, &si);
}

} // namespace phyr
