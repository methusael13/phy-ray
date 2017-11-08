#ifndef PHYRAY_CORE_OBJECT_H
#define PHYRAY_CORE_OBJECT_H

#include <core/phyr.h>
#include <core/geometry/geometry.h>
#include <core/geometry/transform.h>
#include <core/material/material.h>

#include <memory>

namespace phyr {

class Object {
  public:
    virtual ~Object() {}

    /**
     * Returns the bounds of the object in world space
     */
    virtual Bounds3f worldBounds() const = 0;
    virtual bool intersectRay(const Ray& ray) const = 0;
    virtual bool intersectRay(const Ray& ray, SurfaceInteraction* si) const = 0;

    /**
     * Returns a pointer to the AreaLight describing the object's emission
     * distribution. If the object is non-emissive, the function returns nullptr
     */
    virtual const AreaLight* getAreaLight() const = 0;
    /**
     * Returns a pointer to the Material instance assigned to this object.
     * If nullptr is returned, ray intersection with this object should be ignored
     */
    virtual const Material* getMaterial() const = 0;

    /**
     * Compute the light scattering functions modelled by the object's surface
     */
    virtual void computeScatteringFunctions(SurfaceInteraction* si,
                                            MemoryPool& mem, TransportMode mode,
                                            bool allowMultiLobes) const = 0;
};

class GeometricObject : public Object {
  public:
    GeometricObject(const std::shared_ptr<Shape>& shape,
                    const std::shared_ptr<Material>& material,
                    const std::shared_ptr<AreaLight>& areaLight) :
        shape(shape), material(material), areaLight(areaLight) {}

    Bounds3f worldBounds() const;
    bool intersectRay(const Ray& ray) const;
    bool intersectRay(const Ray& ray, SurfaceInteraction* si) const;

    const AreaLight* getAreaLight() const;
    const Material* getMaterial() const;

    void computeScatteringFunctions(SurfaceInteraction* si,
                                    MemoryPool& mem, TransportMode mode,
                                    bool allowMultiLobes) const;
  private:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    std::shared_ptr<AreaLight> areaLight;
};

class InstancedObject : public Object {
  public:
    InstancedObject(const std::shared_ptr<Object>& object,
                    const Transform& objectToInstanceWorld) :
        object(object), objectToInstanceWorld(objectToInstanceWorld) {}

    Bounds3f worldBounds() const;
    bool intersectRay(const Ray& ray) const;
    bool intersectRay(const Ray& ray, SurfaceInteraction* si) const;

    const AreaLight* getAreaLight() const { return nullptr; }
    const Material* getMaterial() const { return nullptr; }

  private:
    std::shared_ptr<Object> object;
    const Transform objectToInstanceWorld;

    // Function has no purpose in this context
    void computeScatteringFunctions(SurfaceInteraction* si,
                                    MemoryPool& mem, TransportMode mode,
                                    bool allowMultiLobes) const {}
};

class ObjectGroup : public Object {
  private:
    // Functions have no purpose in this context
    const AreaLight* getAreaLight() const { return nullptr; }
    const Material* getMaterial() const { return nullptr; }

    void computeScatteringFunctions(SurfaceInteraction* si,
                                    MemoryPool& mem, TransportMode mode,
                                    bool allowMultiLobes) const {}
};

}  // namespace phyr

#endif
