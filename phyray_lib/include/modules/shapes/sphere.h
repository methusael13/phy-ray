#ifndef PHYRAY_SHAPES_SPHERE_H
#define PHYRAY_SHAPES_SPHERE_H

#include <core/phyr.h>
#include <core/geometry/shape.h>

namespace phyr {

class Sphere : public Shape {
  public:
    Sphere(const Transform* localToWorld,
           const Transform* worldToLocal, const bool reverseNormals,
           Real radius, Real zMin, Real zMax) :
           Shape(localToWorld, worldToLocal, reverseNormals) {
        this->radius = radius;
        // Clip z visibility
        this->zMin = clamp(std::min(zMin, zMax), -radius, radius);
        this->zMax = clamp(std::max(zMin, zMax), -radius, radius);

        // Theta can have values in the range [-1, 1]
        // subject to being clipped by the min and max z values
        thetaMin = std::acos(clamp(zMin / radius, -1, 1));
        thetaMax = std::acos(clamp(zMax / radius, -1, 1));
    }

    /**
     * @todo: Implement tighter bounds for phiMax < 3*Pi/2
     */
    Bounds3f objectBounds() const override {
        return Bounds3f(Point3f(-radius, -radius, zMin),
                        Point3f( radius,  radius, zMax));
    }

    /**
     * @todo: Reimplement with proper error bound checks
     */
    bool intersectRay(const Ray& ray, Real* t0, SurfaceInteraction* si) const override;

  private:
    Real radius;
    Real zMin, zMax;
    Real thetaMin, thetaMax;
};

} // namespace phyr

#endif
