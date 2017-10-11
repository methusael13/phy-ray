
#include <core/phyr.h>
#include <core/geometry/transform.h>
#include <core/geometry/interaction.h>

namespace phyr {

inline Transform Transform::operator*(const Transform& t) const {
    // inverse(AB) = inverse(B)inverse(A)
    return Transform(mat * t.mat, t.invMat * invMat);
}

// Transform creation functions
Transform Transform::translate(const Vector3f& v) {
    Mat4x4 m =  Mat4x4(1, 0, 0,  v.x,
                       0, 1, 0,  v.y,
                       0, 0, 1,  v.z,
                       0, 0, 0,    1);
    Mat4x4 mi = Mat4x4(1, 0, 0, -v.x,
                       0, 1, 0, -v.y,
                       0, 0, 1, -v.z,
                       0, 0, 0,    1);
    return Transform(m, mi);
}

Transform Transform::scale(Real sx, Real sy, Real sz) {
    Mat4x4 m = Mat4x4(sx,  0,  0, 0,
                       0, sy,  0, 0,
                       0,  0, sz, 0,
                       0,  0,  0, 1);
    Mat4x4 mi = Mat4x4(1 / sx,      0,      0, 0,
                            0, 1 / sy,      0, 0,
                            0,      0, 1 / sz, 0,
                            0,      0,      0, 1);
    return Transform(m, mi);
}

/**
 * Rotate by {theta} degrees around the x-axis
 */
Transform Transform::rotateX(Real theta) {
    Mat4x4 m;
    Real cosTheta = std::cos(radians(theta));
    Real sinTheta = std::sin(radians(theta));

    if (theta == 0.0)
        m = Mat4x4(); // Identity
    else {
        m = Mat4x4(1,        0,         0, 0,
                   0,  cosTheta, sinTheta, 0,
                   0, -sinTheta, cosTheta, 0,
                   0,        0,         0, 1);
    }

    return Transform(m, Mat4x4::transpose(m));
}

/**
 * Rotate by {theta} degrees around the y-axis
 */
Transform Transform::rotateY(Real theta) {
    Mat4x4 m;
    Real cosTheta = std::cos(radians(theta));
    Real sinTheta = std::sin(radians(theta));

    if (theta == 0.0)
        m = Mat4x4(); // Identity
    else {
        m = Mat4x4( cosTheta, 0, -sinTheta, 0,
                           0, 1,        0, 0,
                    sinTheta, 0,  cosTheta, 0,
                           0, 0,        0, 1);
    }

    return Transform(m, Mat4x4::transpose(m));
}

/**
 * Rotate by {theta} degrees around the z-axis
 */
Transform Transform::rotateZ(Real theta) {
    Mat4x4 m;
    Real cosTheta = std::cos(radians(theta));
    Real sinTheta = std::sin(radians(theta));

    if (theta == 0.0)
        m = Mat4x4(); // Identity
    else {
        m = Mat4x4( cosTheta, sinTheta, 0, 0,
                   -sinTheta, cosTheta, 0, 0,
                          0,         0, 1, 0,
                          0,         0, 0, 1);
    }

    return Transform(m, Mat4x4::transpose(m));
}

/**
 * Rotation by theta angles around the axis given by {axis}.
 * {axis} is assumed to be normalized
 */
Transform Transform::rotate(const Vector3f& axis, Real theta) {
    Real cosTheta = std::cos(radians(theta));
    Real sinTheta = std::sin(radians(theta));
    Mat4x4 m;

    // Axis angle to rotation matrix
    m.d[0][0] = cosTheta + axis.x * axis.x * (1 - cosTheta);
    m.d[0][1] = axis.x * axis.y * (1 - cosTheta) - axis.z * sinTheta;
    m.d[0][2] = axis.x * axis.z * (1 - cosTheta) + axis.y * sinTheta;

    m.d[1][0] = axis.x * axis.y * (1 - cosTheta) + axis.z * sinTheta;
    m.d[1][1] = cosTheta + axis.y * axis.y * (1 - cosTheta);
    m.d[1][2] = axis.y * axis.z * (1 - cosTheta) - axis.x * sinTheta;

    m.d[2][0] = axis.x * axis.z * (1 - cosTheta) - axis.y * sinTheta;
    m.d[2][1] = axis.y * axis.z * (1 - cosTheta) + axis.x * sinTheta;
    m.d[2][2] = cosTheta + axis.z * axis.z * (1 - cosTheta);

    return Transform(m, Mat4x4::transpose(m));
}

Transform Transform::lookAt(const Point3f& loc, const Point3f& targetLoc, const Vector3f& up) {
    // Camera looks down the +Z axis
    Vector3f zAxis = normalize(targetLoc - loc);
    Vector3f xAxis = cross(zAxis, up);

    if (xAxis.length() == 0) {
        // Front and up vectors align; return identity transform
        return Transform();
    }

    xAxis = normalize(xAxis);
    // Right and front already normalized
    Vector3f yAxis = cross(xAxis, zAxis);
    Mat4x4 camMatrix = Mat4x4(xAxis.x, yAxis.x, zAxis.x, loc.x,
                              xAxis.y, yAxis.y, zAxis.y, loc.y,
                              xAxis.z, yAxis.z, zAxis.z, loc.z,
                                    0,       0,       0,     1);

    // LookAt transform creates a viewMatrix which is the inverse of cameraMatrix
    return Transform(Mat4x4::inverse(camMatrix), camMatrix);
}

// Transform application functions

/**
 * @todo Reimplement with proper error bound checks
 */
SurfaceInteraction Transform::operator()(const SurfaceInteraction& si) const {
    SurfaceInteraction nsi;
    const Transform& tr = *this;

    // Interaction members
    nsi.p = tr(si.p); nsi.n = normalize(tr(si.n));
    nsi.wo = normalize(tr(si.wo));

    // SurfaceInteraction members
    nsi.uv = si.uv; nsi.shape = si.shape;
    nsi.dpdu = tr(si.dpdu); nsi.dpdv = tr(si.dpdv);
    nsi.dndu = tr(si.dndu); nsi.dndv = tr(si.dndv);
    // SurfaceInteractio::shadingGeometry members
    nsi.shadingGeom.n = normalize(tr(si.shadingGeom.n));
    nsi.shadingGeom.dpdu = tr(si.shadingGeom.dpdu);
    nsi.shadingGeom.dpdv = tr(si.shadingGeom.dpdv);
    nsi.shadingGeom.dndu = tr(si.shadingGeom.dndu);
    nsi.shadingGeom.dndv = tr(si.shadingGeom.dndv);

    // Check if normals should be readjusted
    nsi.shadingGeom.overridesOrientation = si.shadingGeom.overridesOrientation;
    if (nsi.shadingGeom.overridesOrientation)
        nsi.n = faceForward(nsi.n, nsi.shadingGeom.n);
    else
        nsi.shadingGeom.n = faceForward(nsi.shadingGeom.n, nsi.n);

    return nsi;
}

} // namespace phyr
