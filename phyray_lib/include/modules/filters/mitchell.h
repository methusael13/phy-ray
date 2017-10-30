#ifndef PHYRAY_MODULES_MITCHELLFILTER_H
#define PHYRAY_MODULES_MITCHELLFILTER_H

#include <core/integrator/filter.h>

namespace phyr {

class MitchellFilter : public Filter {
  public:
    MitchellFilter(const Vector2f& radius, Real B, Real C) :
        Filter(radius), B(B), C(C),
        // Initialize coefficients for x > 1
        COEFF_PRIM_D3(-B - 6*C), COEFF_PRIM_D2(6*B + 30*C),
        COEFF_PRIM_D1(-12*B - 48*C), COEFF_PRIM_D0(8*B + 24*C),
        // Initialize coefficients for x <= 1
        COEFF_SEC_D3(12 - 9*B - 6*C), COEFF_SEC_D2(-18 + 12*B + 6*C),
        COEFF_SEC_D0(6 - 2*B) {}

    Real evaluate(const Point2f& pt) const override;

  private:
    Real mitchell(Real x) const {
        x = std::abs(2 * x);

        // Evaluate Mitchell polynomial
        if (x > 1) {
            return (COEFF_PRIM_D3 * x*x*x + COEFF_PRIM_D2 * x*x +
                    COEFF_PRIM_D1 * x + COEFF_PRIM_D0) * OneOverSix;
        } else {
            return (COEFF_SEC_D3 * x*x*x + COEFF_SEC_D2 * x*x + COEFF_SEC_D0) * OneOverSix;
        }
    }

    const Real B, C;

    // Coefficients for x > 1
    const Real COEFF_PRIM_D3, COEFF_PRIM_D2, COEFF_PRIM_D1, COEFF_PRIM_D0;
    // Coefficients for x <= 1
    const Real COEFF_SEC_D3, COEFF_SEC_D2, COEFF_SEC_D0;

    static constexpr Real OneOverSix = Real(1) / Real(6);
};

}  // namespace phyr

#endif

