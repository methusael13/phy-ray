#ifndef PHYRAY_CORE_FILTER_H
#define PHYRAY_CORE_FILTER_H

#include <core/phyr.h>
#include <core/geometry/geometry.h>

namespace phyr {

class Filter {
  public:
    Filter(const Vector2f& radius) :
        radius(radius),
        invRadius(Vector2f(Real(1) / radius.x, Real(1) / radius.y)) {}

    // Interface
    virtual Real evaluate(const Point2f& pt) const = 0;

    const Vector2f radius, invRadius;
};

}  // namespace phyr

#endif
