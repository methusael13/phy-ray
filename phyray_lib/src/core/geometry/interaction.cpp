#include <core/geometry/shape.h>
#include <core/geometry/interaction.h>
#include <core/object/object.h>

namespace phyr {

SurfaceInteraction::SurfaceInteraction(const Point3f& p,
                   const Vector3f& wo, const Vector3f& pfError,
                   const Point2f& uv, const Vector3f& dpdu, const Vector3f& dpdv,
                   const Normal3f& dndu, const Normal3f& dndv, const Shape* shape) :
    Interaction(p, normalize(Normal3f(cross(dpdu, dpdv))), wo, pfError), uv(uv), dpdu(dpdu),
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

void SurfaceInteraction::setShadingGeomerty(const Vector3f& dpdus, const Vector3f& dpdvs,
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

void SurfaceInteraction::computeScatteringFunctions(const Ray &ray, MemoryPool &arena,
                                                    bool allowMultipleLobes,
                                                    TransportMode mode) {
    object->computeScatteringFunctions(this, arena, mode, allowMultipleLobes);
}

} // namespace phyr
