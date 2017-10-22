#ifndef PHYRAY_CORE_CAMERA_H
#define PHYRAY_CORE_CAMERA_H

#include <core/phyr.h>
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

}  // namespace phyr

#endif
