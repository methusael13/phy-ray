#include <core/phyr_math.h>
#include <core/geometry/geometry.h>
#include <core/geometry/interaction.h>

#include <modules/shapes/sphere.h>

namespace phyr {

/**
 * @todo: Reimplement with proper error bound checks
 */
bool Sphere::intersectRay(const Ray& ray, Real* t0, SurfaceInteraction* si) const {
    // Transform ray to local space
    Ray lr = (*worldToLocal)(ray);

    // Compute quadratic sphere coefficients
    Real dx = lr.d.x, dy = lr.d.y, dz = lr.d.z;
    Real ox = lr.o.x, oy = lr.o.y, oz = lr.o.z;
    Real a = dx * dx + dy * dy + dz * dz;
    Real b = 2 * (ox * dx + oy * dy + oz * dz);
    Real c = ox * ox + oy * oy + oz * oz - radius * radius;

    // Solve quadratic equation for t
    Real t1, t2;
    // No intersections
    if (!solveQuadraticSystem(a, b, c, &t1, &t2)) return false;

    // Check for trivial range exclusion
    if (t1 > ray.tMax || t2 <= 0) return false;
    *t0 = t1;
    if (t1 <= 0) { *t0 = t2; if (t2 > ray.tMax) return false; }

    Point3f hpt = ray(*t0);
    if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax)) {
        // If ray origin is inside the sphere and t2 intersects a clipped region
        if (*t0 == t2) return false;
        // We're looking at a hole in the sphere on the near side.
        // Test intersection with the farther side
        if (t2 > ray.tMax) return false;

        // Test with t1 failed, try t2
        *t0 = t2; hpt = ray(t2);
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
    // Compute coefficients for fundamental forms
    Real E = dot(dpdu, dpdu), F = dot(dpdu, dpdv), G = dot(dpdv, dpdv);
    Vector3f n = normalize(cross(dpdu, dpdv));
    Real e = dot(n, d2pduu), f = dot(n, d2pduv), g = dot(n, d2pdvv);

    // Compute dndu, dndv
    Real invFac = 1 / (E * G - F * F);
    Normal3f dndu = Normal3f((f * F - e * G) * invFac * dpdu +
                             (e * F - f * E) * invFac * dpdv);
    Normal3f dndv = Normal3f((g * f - f * G) * invFac * dpdu +
                             (f * F - g * E) * invFac * dpdv);

    // Initiate the SurfaceInteraction object
    *si = (*worldToLocal)(SurfaceInteraction(hpt, -ray.d, Point2f(u, v), dpdu, dpdv, dndu, dndv, this));

    return true;
}

/**
 * @todo: Reimplmenet with proper error bound checks
 */
bool Sphere::intersectRay(const Ray& ray) const {
    // Transform ray to local space
    Ray lr = (*worldToLocal)(ray);

    // Compute quadratic sphere coefficients
    Real dx = lr.d.x, dy = lr.d.y, dz = lr.d.z;
    Real ox = lr.o.x, oy = lr.o.y, oz = lr.o.z;
    Real a = dx * dx + dy * dy + dz * dz;
    Real b = 2 * (ox * dx + oy * dy + oz * dz);
    Real c = ox * ox + oy * oy + oz * oz - radius * radius;

    // Solve quadratic equation for t
    Real t0, t1, t2;
    // No intersections
    if (!solveQuadraticSystem(a, b, c, &t1, &t2)) return false;

    // Check for trivial range exclusion
    if (t1 > ray.tMax || t2 <= 0) return false;
    t0 = t1;
    if (t1 <= 0) { t0 = t2; if (t2 > ray.tMax) return false; }

    Point3f hpt = ray(t0);
    if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax)) {
        // If ray origin is inside the sphere and t2 intersects a clipped region
        if (t0 == t2) return false;
        // We're looking at a hole in the sphere on the near side.
        // Test intersection with the farther side
        if (t2 > ray.tMax) return false;

        // Test with t1 failed, try t2
        t0 = t2; hpt = ray(t2);
        if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax))
            return false;
    }

    return true;
}

} // namespace phyr
