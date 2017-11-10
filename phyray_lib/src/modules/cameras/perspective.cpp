#include <core/integrator/sampling.h>
#include <modules/cameras/perspective.h>

namespace phyr {

Real PerspectiveCamera::generateRay(const CameraSample& sample, Ray* ray) const {
    // Compute raster and camera sample positions
    Point3f pFilm(sample.pFilm.x, sample.pFilm.y, 0);
    Point3f pCamera = rasterToCamera(pFilm);

    *ray = Ray(Point3f(0, 0, 0), normalize(Vector3f(pCamera)));
    // Modify ray for DOF
    if (lensRadius > 0) {
        Point2f pLens = lensRadius * concentricSampleDisk(sample.pLens);
        // Compute unperturbed point on the plane of focus
        Real ft = focalDistance / ray->d.z;
        Point3f fpt = (*ray)(ft);
        // Update ray
        ray->o = Point3f(pLens.x, pLens.y, 0);
        ray->d = normalize(fpt - ray->o);
    }

    *ray = cameraToWorld(*ray);
    return 1;
}

PerspectiveCamera* createPerspectiveCamera(const Transform& cameraToWorld,
                                           Film* film, Real lensRadius,
                                           Real focalDistance, Real fov) {
    // Calculate screen bounds
    Bounds2f screen;
    Real aspect = Real(film->resolution.x) / film->resolution.y;
    if (aspect > 1) {
        // Landscape
        screen.pMin.x = -aspect; screen.pMax.x = aspect;
        screen.pMin.y = -1.0f; screen.pMax.y = 1.0f;
    } else {
        screen.pMin.x = -1.0f; screen.pMax.x = 1.0f;
        screen.pMin.y = -1.0f / aspect; screen.pMax.y = 1.0f / aspect;
    }

    return new PerspectiveCamera(cameraToWorld, screen, lensRadius,
                                 focalDistance, fov, film);
}

}  // namespace phyr
