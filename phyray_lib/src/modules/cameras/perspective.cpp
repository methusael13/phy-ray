#include <core/integrator/sampling.h>
#include <modules/cameras/perspective.h>

namespace phyr {

Real PerspectiveCamera::generateRay(const CameraSample& sample, Ray* ray) const {
    // Compute raster and camera sample positions
    Point3f pFilm(sample.pFilm.x, sample.pFilm.y, 0);
    Point3f pCamera = rasterToCamera(pFilm);

    *ray = cameraToWorld(Ray(Point3f(0, 0, 0), normalize(Vector3f(pCamera))));
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

    return 1;
}

}  // namespace phyr
