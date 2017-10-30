#ifndef PHYRAY_MODULES_GAUSSIANFILTER_H
#define PHYRAY_MODULES_GAUSSIANFILTER_H

#include <core/integrator/filter.h>

namespace phyr {

class GaussianFilter : public Filter {
  public:
    GaussianFilter(const Vector2f& radius, Real alpha) :
        Filter(radius), alpha(alpha),
        expx(std::exp(-alpha * radius.x * radius.x)),
        expy(std::exp(-alpha * radius.y * radius.y)) {}

    Real evaluate(const Point2f& pt) const override;

  private:
    Real gaussian(Real p, Real expr) const {
        return std::max(Real(0), Real(std::exp(-alpha * p * p) - expr));
    }

    const Real alpha, expx, expy;
};

}  // namespace phyr

#endif
