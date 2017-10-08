
#include <core/phyr.h>
#include <core/geometry/transform.h>

namespace phyr {

inline Transform Transform::operator*(const Transform& t) const {
    return Transform(mat * t.mat, invMat * t.invMat);
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
                   0, cosTheta, -sinTheta, 0,
                   0, sinTheta,  cosTheta, 0,
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
        m = Mat4x4( cosTheta, 0, sinTheta, 0,
                           0, 1,        0, 0,
                   -sinTheta, 0, cosTheta, 0,
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
        m = Mat4x4(cosTheta, -sinTheta, 0, 0,
                   sinTheta,  cosTheta, 0, 0,
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
    m.d[0][3] = 0;

    m.d[1][0] = axis.x * axis.y * (1 - cosTheta) + axis.z * sinTheta;
    m.d[1][1] = cosTheta + axis.y * axis.y * (1 - cosTheta);
    m.d[1][2] = axis.y * axis.z * (1 - cosTheta) - axis.x * sinTheta;
    m.d[1][3] = 0;

    m.d[2][0] = axis.x * axis.z * (1 - cosTheta) - axis.y * sinTheta;
    m.d[2][1] = axis.y * axis.z * (1 - cosTheta) + axis.x * sinTheta;
    m.d[2][2] = cosTheta + axis.z * axis.z * (1 - cosTheta);
    m.d[2][3] = 0;

    return Transform(m, Mat4x4::transpose(m));
}

} // namespace phyr
