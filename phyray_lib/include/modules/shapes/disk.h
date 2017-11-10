#ifndef PHYRAY_SHAPES_DISK_H
#define PHYRAY_SHAPES_DISK_H

#include <core/phyr.h>
#include <core/geometry/shape.h>

namespace phyr {

class Disk : public Shape {
  public:
    Disk(const Transform* localToWorld, const Transform* worldToLocal,
         Real height, Real radius, Real innerRadius, Real phiMax,
         bool reverseNormals) :
        Shape(localToWorld, worldToLocal, reverseNormals),
        height(height), radius(radius), innerRadius(innerRadius),
        phiMax(radians(clamp(phiMax, 0, 360))) {}

    // Interface
    Bounds3f objectBounds() const override;
    Real surfaceArea() const override;

    bool intersectRay(const Ray& ray, Real* t0, SurfaceInteraction* si,
                      bool testAlpha = true) const override;
    bool intersectRay(const Ray& ray, bool testAlpha = true) const override;

    // Sampling
    Interaction sample(const Point2f& u, Real* pdf) const override;

  private:
    const Real height, radius, innerRadius, phiMax;
};

std::shared_ptr<Disk> createDiskShape(const Transform* o2w, const Transform* w2o,
                                      Real height = 0, Real radius = 1,
                                      Real innerRadius = 0, Real phiMax = 360,
                                      bool reverseOrientation = true);

}  // namespace phyr

#endif
