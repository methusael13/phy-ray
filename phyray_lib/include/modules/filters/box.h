#ifndef PHYRAY_MODULES_BOXFILTER_H
#define PHYRAY_MODULES_BOXFILTER_H

#include <core/integrator/filter.h>

namespace phyr {

class BoxFilter : public Filter {
  public:
    BoxFilter(const Vector2f& radius) : Filter(radius) {}

    Real evaluate(const Point2f& pt) const override;
};

}  // namespace phyr

#endif
