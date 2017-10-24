#ifndef PHYRAY_CAMERAS_PERSPECTIVE_H
#define PHYRAY_CAMERAS_PERSPECTIVE_H

#include <core/phyr.h>
#include <core/camera/camera.h>

namespace phyr {

class PerspectiveCamera : public ProjectiveCamera {
  public:
    PerspectiveCamera(const Transform& cameraToWorld, const Bounds2f& screenBounds,
                      Real lensRadius, Real focalDistance, Real fov, Film* film) :
        ProjectiveCamera(cameraToWorld, Transform::perspective(fov, 1e-2f, 1000.0f),
                         screenBounds, lensRadius, focalDistance, film) {
        // Compute image plane bounds
        Point2i res = film->resolution;
        Point3f pMin = rasterToCamera(Point3f(0, 0, 0));
        Point3f pMax = rasterToCamera(Point3f(res.x, res.y, 0));

        // Z - divide
        pMin /= pMin.z; pMax /= pMax.z;
        imagePlaneArea = std::abs((pMax.x - pMin.x) * (pMax.y - pMin.y));
    }

    Real generateRay(const CameraSample& sample, Ray* ray) const override;

  private:
    Real imagePlaneArea;
};

}  // namespace phyr

#endif
