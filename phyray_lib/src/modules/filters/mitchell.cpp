#include <modules/filters/mitchell.h>

namespace phyr {

Real MitchellFilter::evaluate(const Point2f& pt) const {
    return mitchell(pt.x * invRadius.x) * mitchell(pt.y * invRadius.y);
}

}  // namespace phyr
