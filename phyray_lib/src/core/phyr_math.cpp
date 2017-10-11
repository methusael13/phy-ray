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

} // namespace phyr
