#include <core/phyr.h>
#include <core/geometry/shape.h>
#include <core/geometry/geometry.h>
#include <core/geometry/interaction.h>

#include <core/object/object.h>

namespace phyr {

// GeometricObject definitions
Bounds3f GeometricObject::worldBounds() const { return shape->worldBounds(); }

// GeometricObject intersections
bool GeometricObject::intersectRay(const Ray& ray) const {
    return shape->intersectRay(ray);
}
bool GeometricObject::intersectRay(const Ray& ray, SurfaceInteraction* si) const {
    Real t0;
    // Check for intersection
    if (!shape->intersectRay(ray, &t0, si)) return false;
    // Update ray and interaction
    ray.tMax = t0; si->object = this;

    return true;
}

void GeometricObject::computeScatteringFunctions(SurfaceInteraction* si,
                                                 MemoryPool& pool, TransportMode mode,
                                                 bool allowMultiLobes) const {
    if (material)
        material->computeScatteringFunctions(si, pool, mode,
                                             allowMultiLobes);
    ASSERT(dot(si->n, si->shadingGeom.n) >= 0.);
}

const AreaLight* GeometricObject::getAreaLight() const {
    return areaLight.get();
}
const Material* GeometricObject::getMaterial() const {
    return material.get();
}


// InstancedObject declarations
Bounds3f InstancedObject::worldBounds() const {
    return objectToInstanceWorld(object->worldBounds());
}

bool InstancedObject::intersectRay(const Ray& ray) const {
    return object->intersectRay(Transform::inverse(objectToInstanceWorld)(ray));
}
bool InstancedObject::intersectRay(const Ray& ray, SurfaceInteraction* si) const {
    // Transform ray to instance local space
    Ray r = Transform::inverse(objectToInstanceWorld)(ray);
    if (!object->intersectRay(r, si)) return false;
    ray.tMax = r.tMax;

    // Transform surface interaction data back to instance world space
    if (!objectToInstanceWorld.isIdentity())
        *si = objectToInstanceWorld(*si);
    return true;
}

}  // namespace phyr
