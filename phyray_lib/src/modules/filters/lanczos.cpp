#include <modules/filters/lanczos.h>

namespace phyr {

Real LanczosSincFilter::evaluate(const Point2f& pt) const {
    return windowedSinc(pt.x, radius.x) * windowedSinc(pt.y, radius.y);
}

}  // namespace phyr
