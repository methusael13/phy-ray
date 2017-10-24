#include <core/integrator/sampling.h>

namespace phyr {

// Sampling a unit disk
Point2f uniformSampleDisk(const Point2f& pt) {
    Real rad = std::sqrt(pt.x);
    Real theta = 2 * Pi * pt.y;
    return Point2f(rad * std::cos(theta), rad * std::sin(theta));
}

Point2f concentricSampleDisk(const Point2f& pt) {
    // Map uniform random point to [-1, 1]
    Point2f nrand = 2.0 * pt - Vector2f(1, 1);
    // Handle origin
    if (nrand.x == 0 && nrand.y == 0)
        return Point2f(0, 0);

    Real rad, theta;
    // Apply concentric mapping
    if (std::abs(nrand.x) > std::abs(nrand.y)) {
        rad = nrand.x;
        theta = PiOver4 * (nrand.y / nrand.x);
    } else {
        rad = nrand.y;
        theta = PiOver4 * (nrand.x / nrand.y);
    }

    return Point2f(rad * std::cos(theta), rad * std::sin(theta));
}

}  // namespace phyr
