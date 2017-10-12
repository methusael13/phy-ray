#ifndef PHYRAY_CORE_MATH_H
#define PHYRAY_CORE_MATH_H

#include <ostream>
#include <algorithm>

#include <core/phyr.h>
#include <core/geometry/geometry.h>

namespace phyr {

typedef struct _Mat4x4 Mat4x4;

struct _Mat4x4 {
    // Build an identity martix by default
    _Mat4x4() {
        d[0][0] = d[1][1] = d[2][2] = d[3][3] = Real(1.);
        d[0][1] = d[0][2] = d[0][3] = d[1][0] = d[1][2] = d[1][3] =
        d[2][0] = d[2][1] = d[2][3] = d[3][0] = d[3][1] = d[3][2] = Real(0.);
    }
    _Mat4x4(const Real _d[4][4]) { std::copy_n(&_d[0][0], 16, &d[0][0]); }
    _Mat4x4(Real d00, Real d01, Real d02, Real d03,
            Real d10, Real d11, Real d12, Real d13,
            Real d20, Real d21, Real d22, Real d23,
            Real d30, Real d31, Real d32, Real d33) {
        d[0][0] = d00; d[0][1] = d01; d[0][2] = d02; d[0][3] = d03;
        d[1][0] = d10; d[1][1] = d11; d[1][2] = d12; d[1][3] = d13;
        d[2][0] = d20; d[2][1] = d21; d[2][2] = d22; d[2][3] = d23;
        d[3][0] = d30; d[3][1] = d31; d[3][2] = d32; d[3][3] = d33;
    }

    bool operator==(const Mat4x4& m) const {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (d[i][j] != m.d[i][j]) return false;
        return true;
    }
    bool operator!=(const Mat4x4& m) const {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (d[i][j] == m.d[i][j]) return false;
        return true;
    }

    inline Mat4x4 operator*(const Mat4x4& m) const {
        Mat4x4 _m;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                _m.d[i][j] = d[i][0] * m.d[0][j] + d[i][1] * m.d[1][j] +
                             d[i][2] * m.d[2][j] + d[i][3] * m.d[3][j];
        return _m;
    }

    // Data access operators
    inline Real operator()(int i, int j) const { return d[i][j]; }
    inline Real& operator()(int i, int j) { return d[i][j]; }

    // Mat4x4 static factory methods
    static Mat4x4 zeroes() {
        return Mat4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static Mat4x4 ones() {
        return Mat4x4(1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                      1., 1., 1., 1.);
    }
    static Mat4x4 transpose(const Mat4x4& m) {
        return Mat4x4(m.d[0][0], m.d[1][0], m.d[2][0], m.d[3][0],
                      m.d[0][1], m.d[1][1], m.d[2][1], m.d[3][1],
                      m.d[0][2], m.d[1][2], m.d[2][2], m.d[3][2],
                      m.d[0][3], m.d[1][3], m.d[2][3], m.d[3][3]);
    }
    static Mat4x4 inverse(const Mat4x4& m);

    Real d[4][4];
};


inline std::ostream& operator<<(std::ostream& os, const Mat4x4& mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++)
            os << mat(i, j) << ", ";
        os << mat(i, 3); if (i < 3) os << "\n";
    }

    return os;
}

// Global math functions

/**
 * Solves a linear system of two equations.
 * Returns true if solutions are valid, false otherwise.
 */
inline bool solveLinearSystem(const Real A[2][2], const Real B[2], Real* x, Real* y) {
    Real det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
    if (epsEqual(std::abs(det), 0, 100)) return false;

    // Calculate solutions
    *x = (A[1][1] * B[0] - A[0][1] * B[1]) / det;
    *y = (A[0][0] * B[1] - A[1][0] * B[0]) / det;
    // Don't allow illegal solutions
    if (isNaN(*x) || isNaN(*y)) return false;
    return true;
}

/**
 * Solves a given quadratic equation with the given parameters.
 * Stores the solutions in {t1} and {t2}. The function returns true
 * or false depending on the existence of any solution.
 */
inline bool solveQuadraticSystem(Real a, Real b, Real c, Real* t1, Real* t2) {
    // Default improbable solutions
    *t1 = MinReal; *t2 = MaxReal;

    Real det = b*b - 4*a*c, den = 2 * a;
    // No real solutions exist
    if (det < 0) return false;
    // Single solution
    else if (det == 0) { *t1 = -b / den; }
    // Two solutions
    else {
        det = std::sqrt(det);
        *t1 = (-b - det) / den; *t2 = (-b + det) / den;
    }

    if (*t1 > *t2) std::swap(*t1, *t2);
    return true;
}

/**
 * Calculates the surface normal derivatives of a parametric shape
 * defined by f(u, v) using Weingarten equations
 */
inline void solveSurfaceNormal(const Vector3f& dpdu, const Vector3f& dpdv,
                               const Vector3f& d2pduu, const Vector3f& d2pduv,
                               const Vector3f& d2pdvv, Normal3f* dndu, Normal3f* dndv) {
    // Compute coefficients for fundamental forms
    Real E = dot(dpdu, dpdu), F = dot(dpdu, dpdv), G = dot(dpdv, dpdv);
    Vector3f n = normalize(cross(dpdu, dpdv));
    Real e = dot(n, d2pduu), f = dot(n, d2pduv), g = dot(n, d2pdvv);

    // Compute dndu, dndv
    Real invFac = 1 / (E * G - F * F);
    *dndu = Normal3f((f * F - e * G) * invFac * dpdu +
                     (e * F - f * E) * invFac * dpdv);
    *dndv = Normal3f((g * f - f * G) * invFac * dpdu +
                     (f * F - g * E) * invFac * dpdv);
}

} // namespace phyr

#endif
