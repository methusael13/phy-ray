#ifndef PHYRAY_SHAPES_SPHERE_H
#define PHYRAY_SHAPES_SPHERE_H

#include <core/phyr.h>
#include <core/geometry/shape.h>

#include <memory>

namespace phyr {

class Sphere : public Shape {
  public:
    Sphere(const Transform* localToWorld, const Transform* worldToLocal,
           Real radius, const bool reverseNormals = false,
           Real zMin = -Infinity, Real zMax = Infinity) :
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

    Bounds3f objectBounds() const override {
        return Bounds3f(Point3f(-radius, -radius, zMin),
                        Point3f( radius,  radius, zMax));
    }
    Real surfaceArea() const override { return 2 * Pi * radius * (zMax - zMin); }

    bool intersectRay(const Ray& ray, Real* t0, SurfaceInteraction* si,
                      bool testAlpha = true) const override;
    bool intersectRay(const Ray& ray, bool testAlpha = true) const override;

    // Sampling
    Interaction sample(const Point2f& u, Real* pdf) const override;
    Interaction sample(const Interaction& ref, const Point2f& u,
                       Real* pdf) const override;
    Real pdf(const Interaction& ref, const Vector3f& wi) const override;
    Real solidAngle(const Point3f& p, int nSamples) const;

  private:
    Real radius;
    Real zMin, zMax;
    Real thetaMin, thetaMax;
};

std::shared_ptr<Shape> createSphereShape(const Transform* o2w,
                                         const Transform* w2o,
                                         bool reverseOrientation,
                                         Real radius = 1);

} // namespace phyr

#endif
