
#include <core/phyr.h>
#include <core/geometry/transform.h>

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
 * @todo Reimplement with proper error bounds checks
 */
template <typename T>
inline Point3<T> Transform::operator()(const Point3<T>& p) const {
    Real npx = mat.d[0][0] * p.x + mat.d[0][1] * p.y + mat.d[0][2] * p.z + mat.d[0][3];
    Real npy = mat.d[1][0] * p.x + mat.d[1][1] * p.y + mat.d[1][2] * p.z + mat.d[1][3];
    Real npz = mat.d[2][0] * p.x + mat.d[2][1] * p.y + mat.d[2][2] * p.z + mat.d[2][3];
    Real npw = mat.d[3][0] * p.x + mat.d[3][1] * p.y + mat.d[3][2] * p.z + mat.d[3][3];

    if (npw == 1) return Point3<T>(npx, npy, npz);
    else return Point3<T>(npx, npy, npz) / npw;
}

/**
 * @todo Reimplement with proper error bounds checks
 */
template <typename T>
inline Vector3<T> Transform::operator()(const Vector3<T>& v) const {
    return Vector3<T>(
        mat.d[0][0] * v.x + mat.d[0][1] * v.y + mat.d[0][2] * v.z,
        mat.d[1][0] * v.x + mat.d[1][1] * v.y + mat.d[1][2] * v.z,
        mat.d[2][0] * v.x + mat.d[2][1] * v.y + mat.d[2][2] * v.z
    );
}

/**
 * Applying transforms on normals require special treatment.
 * Normals should maintain the invariant that they're always perpendicular
 * to the shape surface. This invariant can be defined by dot(N,T) = 0,
 * where N is the normal and T the tangent at a point on the surface.
 *
 * A surface point transformed by M also transforms T by M.
 * So, we have T' = MT. Let N be transformed by an arbitrary transformation, say S.
 * Then, N' = SN and dot(N',T') = 0; dot(SN, MT) = 0;
 * transpose(SN) * MT = 0; transpose(N) * transpose(S) * MT = 0;
 * Since, transpose(N) * T = 0, it follows that, transpose(S) * M = 1;
 * transpose(S) = inverse(M); S = transpose(inverse(M))
 *
 * Thus a normal N should be multiplied by the transpose of the inverse of the
 * transformation matrix.
 */
template <typename T>
inline Normal3<T> Transform::operator()(const Normal3<T>& n) const {
    // Inverse is already maintained by the Transform class
    return Normal3<T>(invMat.d[0][0] * n.x + invMat.d[1][0] * n.y + invMat.d[2][0] * n.z,
                      invMat.d[0][1] * n.x + invMat.d[1][1] * n.y + invMat.d[2][1] * n.z,
                      invMat.d[0][2] * n.x + invMat.d[1][2] * n.y + invMat.d[2][2] * n.z);
}

/**
 * @todo Reimplement with proper error bounds checks
 */
inline Ray Transform::operator()(const Ray& r) const {
    Point3f o = (*this)(r.o);  // Transform origin
    Vector3f d = (*this)(r.d); // Transform direction
    return Ray(o, d, r.tMax, r.time);
}

inline Bounds3f Transform::operator()(const Bounds3f& b) const {
    // Transform the bounds points
    Point3f p1 = (*this)(b.pMin), p2 = (*this)(b.pMax);
    // The Bounds constructor will create the necessary limits
    return Bounds3f(p1, p2);
}

/**
 * @todo Reimplement with proper error bounds checks
 */
inline SurfaceInteraction Transform::operator()(const SurfaceInteraction& si) const {
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
