
#include <core/phyr.h>

namespace phyr {

Int ulpsDistance(const Real v1, const Real v2) {
    // Handle +0 == -0
    if (v1 == v2) return 0;

    // Handle NaN and Inf
    if (isNaN(v1) || isNaN(v2)) return MaxInt;
    if (std::isinf(v1) || std::isinf(v2)) return MaxInt;

    // Convert to integers
    Int iv1, iv2;
    memcpy(&iv1, &v1, sizeof(Real));
    memcpy(&iv2, &v2, sizeof(Real));

    // Differnt signs are not equal
    if ((iv1 < 0) != (iv2 < 0)) return MaxInt;
    return std::abs(iv1 - iv2);
}

} // namespace phyr
