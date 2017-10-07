#ifndef PHYRAY_CORE_MATH_H
#define PHYRAY_CORE_MATH_H

#include <ostream>
#include <algorithm>

#include <core/phyr.h>

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
 * Calculates inverse of a given Matrix using Gauss-Jordan elimination.
 * Note: Not very numerically stable.
 */
Mat4x4 Mat4x4::inverse(const Mat4x4& mat) {
    Mat4x4 m = mat, inv;

    // Reduce m to identity
    for (int i = 0; i < 4; i++) {
        ASSERT(!isZero(m.d[i][i]));
        // Anchor row is i
        Real coeff = 1.0 / m.d[i][i];
        // Normalize anchor row
        for (int j = 0; j < 4; j++) {
            m.d[i][j] *= coeff;
            // Repeat operation on inverse
            inv.d[i][j] *= coeff;
        }

        // Reduce other rows
        for (int j = 0; j < 4; j++) {
            // Skip anchor row
            if (j == i) continue;
            // For every other row,
            // Apply R[j] = R[j] - C[R[j], i] * R[i]
            Real anc = m.d[j][i];
            for (int k = 0; k < 4; k++) {
                m.d[j][k] -= anc * m.d[i][k];
                // Repeat operation on inverse
                inv.d[j][k] -= anc * inv.d[i][k];
            }
        }
    }

    return inv;
}

} // namespace phyr

#endif
