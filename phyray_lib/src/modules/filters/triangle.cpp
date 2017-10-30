#include <modules/filters/triangle.h>

namespace phyr {

Real TriangleFilter::evaluate(const Point2f& pt) const {
    return std::max(Real(0), radius.x - std::abs(pt.x)) *
           std::max(Real(0), radius.y - std::abs(pt.y));
}

}  // namespace phyr
