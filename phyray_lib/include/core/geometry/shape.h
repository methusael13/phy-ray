#ifndef PHYRAY_CORE_SHAPE_H
#define PHYRAY_CORE_SHAPE_H

#include <core/phyr.h>
#include <core/geometry/transform.h>

namespace phyr {

class Shape {
  public:
    Shape(const Transform* localToWorld,
          const Transform* worldToLocal, const bool reverseNormals) :
        localToWorld(localToWorld),
        worldToLocal(worldToLocal), reverseNormals(reverseNormals),
        transformChangesCoordSys(localToWorld->changesCoordinateSystem()) {}
    virtual ~Shape() {}

    /**
     * Returns the object bounds in its local space
     */
    virtual Bounds3f objectBounds() const = 0;
    /**
     * Returns the object bounds in world space
     */
    virtual Bounds3f worldBounds() const { return (*localToWorld)(objectBounds()); }
    virtual Real surfaceArea() const = 0;

    /**
     * Tests for interaction with the given Ray and returns true or false
     * depending on if there was a hit or not. In case of a hit, the
     * geometric details of the intersection is filled as a SurfaceInteraction
     * and the parametric distance of hit point from the ray as Real value (t0).
     * Rays are assumed to be in the world space.
     */
    virtual bool intersectRay(const Ray& ray, Real* t0, SurfaceInteraction* si) const = 0;
    /**
     * Just checks for a ray intersection without having to fill in any details
     */
    virtual bool intersectRay(const Ray& ray) const;

    const Transform *localToWorld, *worldToLocal;
    const bool reverseNormals;
    const bool transformChangesCoordSys;
};

} // namespace phyr

#endif
