#include <core/geometry/interaction.h>
#include <core/integrator/sampling.h>

#include <modules/shapes/disk.h>

namespace phyr {

Bounds3f Disk::objectBounds() const {
    return Bounds3f(Point3f(-radius, -radius, height),
                    Point3f(radius, radius, height));
}

bool Disk::intersectRay(const Ray& r, Real* t0, SurfaceInteraction* si,
                        bool testAlpha) const {
    // Transform {Ray} to local space
    Vector3f oErr, dErr;
    Ray ray = (*worldToLocal)(r, &oErr, &dErr);

    // Compute plane intersection for disk

    // Reject disk intersections for rays parallel to the disk's plane
    if (ray.d.z == 0) return false;
    Real tShapeHit = (height - ray.o.z) / ray.d.z;
    if (tShapeHit <= 0 || tShapeHit >= ray.tMax) return false;

    // See if hit point is inside disk radii and {phiMax}
    Point3f pHit = ray(tShapeHit);
    Real dist2 = pHit.x * pHit.x + pHit.y * pHit.y;
    if (dist2 > radius * radius || dist2 < innerRadius * innerRadius)
        return false;

    // Test disk {phi} value against {phiMax}
    Real phi = std::atan2(pHit.y, pHit.x);
    if (phi < 0) phi += 2 * Pi;
    if (phi > phiMax) return false;

    // Find parametric representation of disk hit
    Real u = phi / phiMax;
    Real rHit = std::sqrt(dist2);
    Real oneMinusV = ((rHit - innerRadius) / (radius - innerRadius));
    Real v = 1 - oneMinusV;
    Vector3f dpdu(-phiMax * pHit.y, phiMax * pHit.x, 0);
    Vector3f dpdv = Vector3f(pHit.x, pHit.y, 0.) * (innerRadius - radius) / rHit;
    Normal3f dndu(0, 0, 0), dndv(0, 0, 0);

    // Refine disk intersection point
    pHit.z = height;

    // Compute error bounds for disk intersection
    Vector3f pError(0, 0, 0);

    // Initialize _SurfaceInteraction_ from parametric information
    *si = (*localToWorld)(SurfaceInteraction(pHit, -ray.d, pError, Point2f(u, v),
                                             dpdu, dpdv, dndu, dndv, this));

    // Update _tHit_ for quadric intersection
    *t0 = Real(tShapeHit);
    return true;
}

bool Disk::intersectRay(const Ray& r, bool testAlpha) const {
    // Transform {Ray} to object space
    Vector3f oErr, dErr;
    Ray ray = (*worldToLocal)(r, &oErr, &dErr);

    // Compute plane intersection for disk

    // Reject disk intersections for rays parallel to the disk's plane
    if (ray.d.z == 0) return false;
    Real tShapeHit = (height - ray.o.z) / ray.d.z;
    if (tShapeHit <= 0 || tShapeHit >= ray.tMax) return false;

    // See if hit point is inside disk radii and {phiMax}
    Point3f pHit = ray(tShapeHit);
    Real dist2 = pHit.x * pHit.x + pHit.y * pHit.y;
    if (dist2 > radius * radius || dist2 < innerRadius * innerRadius)
        return false;

    // Test disk {phi} value against {phiMax}
    Real phi = std::atan2(pHit.y, pHit.x);
    if (phi < 0) phi += 2 * Pi;
    if (phi > phiMax) return false;
    return true;
}

Real Disk::surfaceArea() const {
    return phiMax * 0.5 * (radius * radius - innerRadius * innerRadius);
}

Interaction Disk::sample(const Point2f& u, Real* pdf) const {
    Point2f pd = concentricSampleDisk(u);
    Point3f pObj(pd.x * radius, pd.y * radius, height);

    Interaction it;
    it.n = normalize((*localToWorld)(Normal3f(0, 0, 1)));
    if (reverseNormals) it.n *= -1;
    it.p = (*localToWorld)(pObj, Vector3f(0, 0, 0), &it.pfError);
    *pdf = 1 / surfaceArea();

    return it;
}

std::shared_ptr<Disk> createDiskShape(const Transform* o2w, const Transform* w2o,
                                      Real height, Real radius, Real innerRadius,
                                      Real phiMax, bool reverseOrientation) {
    return std::make_shared<Disk>(o2w, w2o, height, radius, innerRadius,
                                  phiMax, reverseOrientation);
}

}  // namespace phyr
