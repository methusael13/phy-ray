#ifndef PHYRAY_MODULES_LANCZOSFILTER_H
#define PHYRAY_MODULES_LANCZOSFILTER_H

#include <core/integrator/filter.h>

namespace phyr {

class LanczosSincFilter : public Filter {
  public:
    LanczosSincFilter(const Vector2f& radius, Real tau) :
        Filter(radius), tau(tau) {}

    Real evaluate(const Point2f& pt) const override;

  private:
    Real sinc(Real x) const {
        x = std::abs(x);
        if (x < 1e-5) return Real(1);
        return std::sin(Pi * x) / (Pi * x);
    }

    Real windowedSinc(Real x, Real radius) const {
        x = std::abs(x);
        if (x > radius) return 0;
        return sinc(x) * sinc(x / tau);
    }

    const Real tau;
};

}  // namespace phyr

#endif

