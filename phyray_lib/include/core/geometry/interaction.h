#ifndef PHYRAY_CORE_INTERACTION_H
#define PHYRAY_CORE_INTERACTION_H

#include <core/phyr.h>
#include <core/geometry/shape.h>
#include <core/geometry/geometry.h>

namespace phyr {

class Interaction {
  public:
    Interaction() {}
    Interaction(const Point3f& p) : p(p) {}
    Interaction(const Point3f& p, const Vector3f& wo) : p(p), wo(wo) {}
    Interaction(const Point3f& p, const Normal3f& n, const Vector3f& wo) :
        p(p), n(n), wo(wo) {}

    bool isSurfaceInteraction() const { return n != Normal3f(); }

    // The ray-shape interaction point on the surface
    Point3f p;
    // The surface normal at point p
    Normal3f n;
    // The outgoing ray direction (negative ray direction)
    Vector3f wo;
};

class SurfaceInteraction : public Interaction {
 public:
    SurfaceInteraction() {}
    SurfaceInteraction(const Point3f& p, const Vector3f& wo,
                       const Point2f& uv, const Vector3f& dpdu, const Vector3f& dpdv,
                       const Normal3f& dndu, const Normal3f& dndv, const Shape* shape) :
        Interaction(p, normalize(Normal3f(cross(dpdu, dpdv))), wo), uv(uv), dpdu(dpdu),
        dpdv(dpdv), dndu(dndu), dndv(dndv), shape(shape) {
        // Initialize shading geometry from true geometry
        shadingGeom.n = n;
        shadingGeom.dpdu = dpdu; shadingGeom.dpdv = dpdv;
        shadingGeom.dndu = dndu; shadingGeom.dndv = dndv;
        shadingGeom.overridesOrientation = false;

        // Adjust normals if either shape assumes reversed normals
        // or shape transformation changes coordinate system but not both
        if (shape && (shape->reverseNormals ^ shape->transformChangesCoordSys)) {
            // Flip normals
            n = -n; shadingGeom.n = -shadingGeom.n;
        }
    }

    void setShadingGeomerty(const Vector3f& dpdus, const Vector3f& dpdvs,
                            const Normal3f& dndus, const Normal3f& dndvs,
                            bool overridesOrientation) {
        shadingGeom.n = normalize(Normal3f(cross(dpdus, dpdvs)));

        // Check if normal needs to be flipped
        if (shape && (shape->reverseNormals ^ shape->transformChangesCoordSys))
            shadingGeom.n = -shadingGeom.n;
        if (overridesOrientation) {
            // Shading normal has higher priority.
            // Flip true geometry normal to align with shading normal
            n = faceForward(n, shadingGeom.n);
        } else {
            // True geometry normal has higher priority.
            // Flip shading normal to align with true geometry normal
            shadingGeom.n = faceForward(shadingGeom.n, n);
        }

        shadingGeom.dpdu = dpdus; shadingGeom.dpdv = dpdvs;
        shadingGeom.dndu = dndus; shadingGeom.dndv = dndvs;
        shadingGeom.overridesOrientation = overridesOrientation;
    }

    // The (u, v) coordinates from the parameterization of the surface
    Point2f uv;
    // Partial derivatives of the surface point
    Vector3f dpdu, dpdv;
    // Partial derivatives of the surface normal
    Normal3f dndu, dndv;

    const Shape* shape = nullptr;

    // Separate geometry data for shading
    // required for bump-mapping, etc.
    struct {
        Normal3f n;
        Vector3f dpdu, dpdv;
        Normal3f dndu, dndv;
        bool overridesOrientation;
    } shadingGeom;
};

} // namespace phyr

#endif
