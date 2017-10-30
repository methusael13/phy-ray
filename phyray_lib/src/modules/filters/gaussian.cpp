#include <modules/filters/gaussian.h>

namespace phyr {

Real GaussianFilter::evaluate(const Point2f& pt) const {
    return gaussian(pt.x, expx) * gaussian(pt.y, expy);
}

}  // namespace phyr
