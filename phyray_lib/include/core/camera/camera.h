#ifndef PHYRAY_CORE_CAMERA_H
#define PHYRAY_CORE_CAMERA_H

#include <core/phyr.h>
#include <core/film.h>
#include <core/geometry/transform.h>

namespace phyr {

// Indicates the points on the camera film
// and lens that a ray must pass through.
struct CameraSample { Point2f pLens, pFilm; };

/**
 * @brief The Camera class
 * @todo  Add support for motion blur by considering non-zero shutter speeds.
 */
class Camera {
  public:
    Camera(const Transform& cameraToWorld, Film* film) :
    cameraToWorld(cameraToWorld), film(film) {}

    // Interface

    /**
     * Computes the ray corresponding to the given sample.
     * The direction component of the returned ray is guaranteed to be normalized.
     *
     * Returns a scalar factor that determines to what extent the arriving
     * radiance at the film contributes to the final image.
     */
    virtual Real generateRay(const CameraSample& sample, Ray* ray) const = 0;

    Transform cameraToWorld;
    Film* film;
};


class ProjectiveCamera : public Camera {
  public:
    ProjectiveCamera(const Transform& cameraToWorld, const Transform& cameraToScreen,
                     const Bounds2f& screenBounds, Real lensRadius, Real focalDistance,
                     Film* film) :
        Camera(cameraToWorld, film), cameraToScreen(cameraToScreen) {
        // Init lens params
        this->lensRadius = lensRadius; this->focalDistance = focalDistance;

        // Compute projective camera screen transformations
        screenToRaster = Transform::scale(film->resolution.x, film->resolution.y, 1) *
                         Transform::scale(1 / (screenBounds.pMax.x - screenBounds.pMin.x),
                                          1 / (screenBounds.pMin.y - screenBounds.pMax.y), 1) *
                         Transform::translate(Vector3f(-screenBounds.pMin.x, -screenBounds.pMax.y, 0));
        rasterToScreen = Transform::inverse(screenToRaster);

        // Compute projective camera transformations
        rasterToCamera = Transform::inverse(cameraToScreen) * rasterToScreen;
    }

  protected:
    Transform cameraToScreen, rasterToCamera;
    Transform screenToRaster, rasterToScreen;

    // DOF data
    Real lensRadius, focalDistance;
};

}  // namespace phyr

#endif
