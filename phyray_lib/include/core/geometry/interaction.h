#ifndef PHYRAY_CORE_INTERACTION_H
#define PHYRAY_CORE_INTERACTION_H

#include <core/phyr.h>
#include <core/geometry/geometry.h>

namespace phyr {

class Interaction {
  public:
    Interaction() {}
    Interaction(const Point3f& p) : p(p) {}
    Interaction(const Point3f& p, const Normal3f& n,
                const Vector3f& wo, const Vector3f& pfError) :
        p(p), n(n), wo(wo), pfError(pfError) {}

    bool isSurfaceInteraction() const { return n != Normal3f(); }

    // The ray-shape interaction point on the surface
    Point3f p;
    // The surface normal at point p
    Normal3f n;
    // The outgoing ray direction (negative ray direction)
    Vector3f wo;
    // Floating point error introduced in p
    Vector3f pfError;
};

class SurfaceInteraction : public Interaction {
 public:
    SurfaceInteraction() {}
    SurfaceInteraction(const Point3f& p, const Vector3f& wo, const Vector3f& fpError,
                       const Point2f& uv, const Vector3f& dpdu, const Vector3f& dpdv,
                       const Normal3f& dndu, const Normal3f& dndv, const Shape* shape);

    void setShadingGeomerty(const Vector3f& dpdus, const Vector3f& dpdvs,
                            const Normal3f& dndus, const Normal3f& dndvs,
                            bool overridesOrientation);

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
