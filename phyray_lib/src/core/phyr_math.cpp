#include <core/phyr_math.h>

namespace phyr {

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
 *
 * @todo Reimplement with proper error bound checks
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

    return true;
}

} // namespace phyr
