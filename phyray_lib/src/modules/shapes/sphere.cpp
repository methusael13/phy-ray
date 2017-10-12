#include <core/fperror.h>
#include <core/phyr_math.h>
#include <core/geometry/geometry.h>
#include <core/geometry/interaction.h>

#include <modules/shapes/sphere.h>

namespace phyr {

bool Sphere::intersectRay(const Ray& ray, Real* t0, SurfaceInteraction* si) const {
    Vector3f roErr, rdErr;
    // Transform ray to local space
    Ray lr = (*worldToLocal)(ray, &roErr, &rdErr);

    // Compute quadratic sphere coefficients
    FPError ox(lr.o.x, roErr.x), oy(lr.o.y, roErr.y), oz(lr.o.z, roErr.z);
    FPError dx(lr.d.x, rdErr.x), dy(lr.d.y, rdErr.y), dz(lr.d.z, rdErr.z);
    FPError a = dx * dx + dy * dy + dz * dz;
    FPError b = 2 * (ox * dx + oy * dy + oz * dz);
    FPError c = ox * ox + oy * oy + oz * oz - radius * radius;

    // Solve quadratic equation for t
    FPError t1, t2;
    // No intersections
    if (!solveQuadraticSystem(a, b, c, &t1, &t2)) return false;

    // Check for trivial range exclusion
    if (t1.upperBound() > ray.tMax || t2.lowerBound() <= 0) return false;
    *t0 = Real(t1);
    if (t1.lowerBound() <= 0) {
        *t0 = Real(t2); if (t2.upperBound() > ray.tMax) return false;
    }

    Point3f hpt = ray(*t0);
    // Reproject the point onto the surface to refine it
    hpt *= radius / distance(hpt, Point3f(0, 0, 0));

    if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax)) {
        // If ray origin is inside the sphere and t2 intersects a clipped region
        if (*t0 == Real(t2)) return false;
        // We're looking at a hole in the sphere on the near side.
        // Test intersection with the farther side
        if (t2.upperBound() > ray.tMax) return false;

        // Test with t1 failed, try t2
        *t0 = Real(t2); hpt = ray(Real(t2));
        // Reproject the point onto the surface to refine it
        hpt *= radius / distance(hpt, Point3f(0, 0, 0));

        if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax))
            return false;
    }

    Real twoPi = 2 * Pi, delTheta = thetaMax - thetaMin;

    // Make sure phi = atan(y / x) doesn't result in NaN
    if (hpt.x == 0 && hpt.y == 0) hpt.x = 1e-5f * radius;
    // Compute phi and theta values
    Real phi = std::atan2(hpt.y, hpt.x);
    // Cover the full range [0, 2*Pi] in a continuous cycle
    if (phi < 0) phi += twoPi;
    Real theta = std::acos(clamp(hpt.z / radius, -1, 1));

    // Compute (u, v) parameters from phi and theta
    Real u = phi / twoPi;
    Real v = (theta - thetaMin) / delTheta;

    // Compute partial derivatives for the sphere
    Real hypot = std::sqrt(hpt.x * hpt.x + hpt.y * hpt.y);
    Real invHypot = 1 / hypot;
    Real cosPhi = hpt.x * invHypot, sinPhi = hpt.y * invHypot;

    Vector3f dpdu = Vector3f(-twoPi * hpt.y, twoPi * hpt.x, 0);
    Vector3f dpdv = delTheta * Vector3f(hpt.z * cosPhi, hpt.z * sinPhi, -radius * std::sin(theta));

    // Compute second derivatives
    Vector3f d2pduu = -twoPi * twoPi * Vector3f(hpt.x, hpt.y, 0);
    Vector3f d2pduv = delTheta * hpt.z * twoPi * Vector3f(-sinPhi, cosPhi, 0);
    Vector3f d2pdvv = -delTheta * delTheta * Vector3f(hpt.x, hpt.y, hpt.z);

    // Compute surface normals
    Normal3f dndu, dndv;
    solveSurfaceNormal(dpdu, dpdv, d2pduu,d2pduv, d2pdvv, &dndu, &dndv);

    // Compute error bounds for ray-sphere intersection
    Vector3f pfError = abs(Vector3f(hpt)) * gamma(5);
    // Initiate the SurfaceInteraction object
    *si = (*worldToLocal)(SurfaceInteraction(hpt, -ray.d, pfError,
                                             Point2f(u, v), dpdu, dpdv, dndu, dndv, this));

    return true;
}

/**
 * @todo: Reimplmenet with proper error bound checks
 */
bool Sphere::intersectRay(const Ray& ray) const {
    Vector3f roErr, rdErr;
    // Transform ray to local space
    Ray lr = (*worldToLocal)(ray, &roErr, &rdErr);

    // Compute quadratic sphere coefficients
    FPError ox(lr.o.x, roErr.x), oy(lr.o.y, roErr.y), oz(lr.o.z, roErr.z);
    FPError dx(lr.d.x, rdErr.x), dy(lr.d.y, rdErr.y), dz(lr.d.z, rdErr.z);
    FPError a = dx * dx + dy * dy + dz * dz;
    FPError b = 2 * (ox * dx + oy * dy + oz * dz);
    FPError c = ox * ox + oy * oy + oz * oz - radius * radius;

    // Solve quadratic equation for t
    FPError t1, t2;
    // No intersections
    if (!solveQuadraticSystem(a, b, c, &t1, &t2)) return false;

    // Check for trivial range exclusion
    if (t1.upperBound() > ray.tMax || t2.lowerBound() <= 0) return false;
    FPError t0 = t1;
    if (t1.lowerBound() <= 0) {
        t0 = t2; if (t2.upperBound() > ray.tMax) return false;
    }

    Point3f hpt = ray(Real(t0));
    // Reproject the point onto the surface to refine it
    hpt *= radius / distance(hpt, Point3f(0, 0, 0));

    if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax)) {
        // If ray origin is inside the sphere and t2 intersects a clipped region
        if (t0 == t2) return false;
        // We're looking at a hole in the sphere on the near side.
        // Test intersection with the farther side
        if (t2.upperBound() > ray.tMax) return false;

        // Test with t1 failed, try t2
        t0 = t2; hpt = ray(Real(t2));
        // Reproject the point onto the surface to refine it
        hpt *= radius / distance(hpt, Point3f(0, 0, 0));

        if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax))
            return false;
    }

    return true;
}

} // namespace phyr
