#include <core/fperror.h>
#include <core/phyr_math.h>
#include <core/geometry/geometry.h>
#include <core/geometry/interaction.h>
#include <core/integrator/sampling.h>

#include <modules/shapes/sphere.h>

namespace phyr {

bool Sphere::intersectRay(const Ray& ray, Real* t0, SurfaceInteraction* si,
                          bool testAlpha) const {
    Vector3f roErr, rdErr;
    // Transform ray to local space
    Ray lr = (*worldToLocal)(ray, &roErr, &rdErr);

    // Compute quadratic sphere coefficients
    FPError ox(lr.o.x, roErr.x), oy(lr.o.y, roErr.y), oz(lr.o.z, roErr.z);
    FPError dx(lr.d.x, rdErr.x), dy(lr.d.y, rdErr.y), dz(lr.d.z, rdErr.z);
    FPError a = dx * dx + dy * dy + dz * dz;
    FPError b = 2 * (ox * dx + oy * dy + oz * dz);
    FPError c = ox * ox + oy * oy + oz * oz - FPError(radius) * FPError(radius);

    // Solve quadratic equation for t
    FPError t1, t2;
    // No intersections
    if (!solveQuadraticSystem(a, b, c, &t1, &t2)) return false;

    // Check for trivial range exclusion
    if (t1.upperBound() > lr.tMax || t2.lowerBound() <= 0) return false;
    FPError tt0 = t1;
    if (tt0.lowerBound() <= 0) {
        tt0 = t2; if (tt0.upperBound() > lr.tMax) return false;
    }

    Point3f hpt = lr(Real(tt0));
    // Reproject the point onto the surface to refine it
    hpt *= radius / distance(hpt, Point3f(0, 0, 0));

    if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax)) {
        // If ray origin is inside the sphere and t2 intersects a clipped region
        if (tt0 == t2) return false;
        // We're looking at a hole in the sphere on the near side.
        // Test intersection with the farther side
        if (tt0.upperBound() > lr.tMax) return false;

        // Test with t1 failed, try t2
        tt0 = t2; hpt = lr(Real(tt0));
        // Reproject the point onto the surface to refine it
        hpt *= radius / distance(hpt, Point3f(0, 0, 0));

        if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax))
            return false;
    }

    *t0 = Real(tt0);
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
    *si = (*localToWorld)(SurfaceInteraction(hpt, -lr.d, pfError,
                                             Point2f(u, v), dpdu, dpdv, dndu, dndv, this));

    return true;
}

bool Sphere::intersectRay(const Ray& ray, bool testAlpha) const {
    Vector3f roErr, rdErr;
    // Transform ray to local space
    Ray lr = (*worldToLocal)(ray, &roErr, &rdErr);

    // Compute quadratic sphere coefficients
    FPError ox(lr.o.x, roErr.x), oy(lr.o.y, roErr.y), oz(lr.o.z, roErr.z);
    FPError dx(lr.d.x, rdErr.x), dy(lr.d.y, rdErr.y), dz(lr.d.z, rdErr.z);
    FPError a = dx * dx + dy * dy + dz * dz;
    FPError b = 2 * (ox * dx + oy * dy + oz * dz);
    FPError c = ox * ox + oy * oy + oz * oz - FPError(radius) * FPError(radius);

    // Solve quadratic equation for t
    FPError t1, t2;
    // No intersections
    if (!solveQuadraticSystem(a, b, c, &t1, &t2)) return false;

    // Check for trivial range exclusion
    if (t1.upperBound() > lr.tMax || t2.lowerBound() <= 0) return false;
    FPError t0 = t1;
    if (t0.lowerBound() <= 0) {
        t0 = t2; if (t0.upperBound() > lr.tMax) return false;
    }

    Point3f hpt = lr(Real(t0));
    // Reproject the point onto the surface to refine it
    hpt *= radius / distance(hpt, Point3f(0, 0, 0));

    if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax)) {
        // If ray origin is inside the sphere and t2 intersects a clipped region
        if (t0 == t2) return false;
        // We're looking at a hole in the sphere on the near side.
        // Test intersection with the farther side
        if (t0.upperBound() > lr.tMax) return false;

        // Test with t1 failed, try t2
        t0 = t2; hpt = lr(Real(t0));
        // Reproject the point onto the surface to refine it
        hpt *= radius / distance(hpt, Point3f(0, 0, 0));

        if ((zMin > -radius && hpt.z < zMin) || (zMax < radius && hpt.z > zMax))
            return false;
    }

    return true;
}

// Sampling definitions for Sphere
Interaction Sphere::sample(const Point2f& u, Real* pdf) const {
    Point3f pObj = Point3f(0, 0, 0) + radius * uniformSampleSphere(u);
    Interaction it;

    it.n = normalize((*localToWorld)(Normal3f(pObj.x, pObj.y, pObj.z)));
    if (reverseNormals) it.n *= -1;

    // Reproject _pObj_ to sphere surface and compute _pObjError_
    pObj *= radius / distance(pObj, Point3f(0, 0, 0));
    Vector3f pObjError = gamma(5) * abs((Vector3f)pObj);
    it.p = (*localToWorld)(pObj, pObjError, &it.pfError);
    *pdf = 1 / surfaceArea();

    return it;
}

Interaction Sphere::sample(const Interaction& ref, const Point2f& u,
                           Real* pdf) const {
    Point3f pCenter = (*localToWorld)(Point3f(0, 0, 0));

    // Sample uniformly on sphere if $\pt{}$ is inside it
    Point3f pOrigin = offsetRayOrigin(ref.p, ref.n, pCenter - ref.p, ref.pfError);

    if (distanceSquared(pOrigin, pCenter) <= radius * radius) {
        Interaction intr = sample(u, pdf);
        Vector3f wi = intr.p - ref.p;
        if (wi.lengthSquared() == 0) {
            *pdf = 0;
        } else {
            // Convert from area measure returned by Sample() call above to
            // solid angle measure.
            wi = normalize(wi);
            *pdf *= distanceSquared(ref.p, intr.p) / absDot(intr.n, -wi);
        }

        if (std::isinf(*pdf)) *pdf = 0.f;
        return intr;
    }

    // Compute coordinate system for sphere sampling
    Vector3f wc = normalize(pCenter - ref.p);
    Vector3f wcX, wcY;
    coordinateSystem(wc, &wcX, &wcY);

    // Sample sphere uniformly inside subtended cone

    // Compute $\theta$ and $\phi$ values for sample in cone
    Real sinThetaMax2 = radius * radius / distanceSquared(ref.p, pCenter);
    Real cosThetaMax = std::sqrt(std::max((Real)0, 1 - sinThetaMax2));
    Real cosTheta = (1 - u[0]) + u[0] * cosThetaMax;
    Real sinTheta = std::sqrt(std::max((Real)0, 1 - cosTheta * cosTheta));
    Real phi = u[1] * 2 * Pi;

    // Compute angle $\alpha$ from center of sphere to sampled point on surface
    Real dc = distance(ref.p, pCenter);
    Real ds = dc * cosTheta -
               std::sqrt(std::max(
                   (Real)0, radius * radius - dc * dc * sinTheta * sinTheta));
    Real cosAlpha = (dc * dc + radius * radius - ds * ds) / (2 * dc * radius);
    Real sinAlpha = std::sqrt(std::max((Real)0, 1 - cosAlpha * cosAlpha));

    // Compute surface normal and sampled point on sphere
    Vector3f nWorld = sphericalDirection(sinAlpha, cosAlpha, phi, -wcX, -wcY, -wc);
    Point3f pWorld = pCenter + radius * Point3f(nWorld.x, nWorld.y, nWorld.z);

    // Return _Interaction_ for sampled point on sphere
    Interaction it; it.p = pWorld;
    it.pfError = gamma(5) * abs((Vector3f)pWorld);
    it.n = Normal3f(nWorld);
    if (reverseNormals) it.n *= -1;

    // Uniform cone PDF.
    *pdf = 1 / (2 * Pi * (1 - cosThetaMax));

    return it;
}

Real Sphere::pdf(const Interaction& ref, const Vector3f& wi) const {
    Point3f pCenter = (*localToWorld)(Point3f(0, 0, 0));
    // Return uniform PDF if point is inside sphere
    Point3f pOrigin =
        offsetRayOrigin(ref.p, ref.n, pCenter - ref.p, ref.pfError);
    if (distanceSquared(pOrigin, pCenter) <= radius * radius)
        return Shape::pdf(ref, wi);

    // Compute general sphere PDF
    Real sinThetaMax2 = radius * radius / distanceSquared(ref.p, pCenter);
    Real cosThetaMax = std::sqrt(std::max((Real)0, 1 - sinThetaMax2));
    return uniformConePdf(cosThetaMax);
}

Real Sphere::solidAngle(const Point3f& p, int nSamples) const {
    Point3f pCenter = (*localToWorld)(Point3f(0, 0, 0));
    if (distanceSquared(p, pCenter) <= radius * radius) return 4 * Pi;

    Real sinTheta2 = radius * radius / distanceSquared(p, pCenter);
    Real cosTheta = std::sqrt(std::max((Real)0, 1 - sinTheta2));
    return (2 * Pi * (1 - cosTheta));
}

std::shared_ptr<Shape> createSphereShape(const Transform* o2w,
                                         const Transform* w2o,
                                         bool reverseNormals, Real radius) {
    return std::make_shared<Sphere>(o2w, w2o, radius, reverseNormals);
}

} // namespace phyr
