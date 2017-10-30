#ifndef PHYRAY_MODULES_TRIANGLEFILTER_H
#define PHYRAY_MODULES_TRIANGLEFILTER_H

#include <core/integrator/filter.h>

namespace phyr {

class TriangleFilter : public Filter {
  public:
    TriangleFilter(const Vector2f& radius) : Filter(radius) {}

    Real evaluate(const Point2f& pt) const override;
};

}  // namespace phyr

#endif

