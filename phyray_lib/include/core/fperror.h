#ifndef PHYRAY_CORE_FPERROR_H
#define PHYRAY_CORE_FPERROR_H

#include <core/phyr.h>

namespace phyr {

class FPError {
  public:
    FPError() {}
    FPError(Real _v, Real _error = 0) {
        v = _v;
        if (_error == 0)
            lBound = uBound = v;
        else {
            lBound = nextFloatDown(v - _error);
            uBound = nextFloatUp(v + _error);
        }
    }

    inline bool operator==(const FPError& fp) const { return v == fp.v; }
    inline bool operator!=(const FPError& fp) const { return v != fp.v; }

    explicit operator float() const { return v; }
    explicit operator double() const { return v; }

    // Getters
    Real lowerBound() const { return lBound; }
    Real upperBound() const { return uBound; }
    Real absoluteError() const { return uBound - lBound; }

    // Arithemetic operations
    inline FPError operator-() const { return FPError(-v, -lBound, -uBound); }
    inline FPError operator+(const FPError& fp) const {
        return FPError(v + fp.v, nextFloatDown(lBound + fp.lBound),
                       nextFloatUp(uBound + fp.uBound));
    }
    inline FPError operator-(const FPError& fp) const {
        return FPError(v - fp.v, nextFloatDown(lBound - fp.lBound),
                       nextFloatUp(uBound - fp.uBound));
    }

    FPError operator*(const FPError& fp) const {
        Real _b[4] = {
            lBound * fp.lBound, lBound * fp.uBound,
            uBound * fp.lBound, uBound * fp.uBound
        };

        return FPError(v * fp.v,
                       nextFloatDown(std::min(std::min(_b[0], _b[1]), std::min(_b[2], _b[3]))),
                       nextFloatUp(std::max(std::max(_b[0], _b[1]), std::max(_b[2], _b[3]))));
    }

    FPError operator/(const FPError& fp) const {
        FPError ret;
        ret.v = v / fp.v;

        if (fp.lBound < 0 && fp.uBound > 0) {
            ret.lBound = -Infinity; ret.uBound = Infinity;
        } else {
            Real _b[4] = {
                lBound / fp.lBound, lBound / fp.uBound,
                uBound / fp.lBound, uBound / fp.uBound
            };
            ret.lBound = nextFloatDown(std::min(std::min(_b[0], _b[1]), std::min(_b[2], _b[3])));
            ret.uBound = nextFloatUp(std::max(std::max(_b[0], _b[1]), std::max(_b[2], _b[3])));
        }

        return ret;
    }

  private:
    Real v, lBound, uBound;

    // Convenience constructor, for internal use only
    FPError(Real _v, Real _lb, Real _ub) : v(_v), lBound(_lb), uBound(_ub) {}
};

// Global inline operator overloads
inline FPError operator+(Real f, const FPError& fp) { return FPError(f) + fp; }
inline FPError operator-(Real f, const FPError& fp) { return FPError(f) - fp; }
inline FPError operator*(Real f, const FPError& fp) { return FPError(f) * fp; }
inline FPError operator/(Real f, const FPError& fp) { return FPError(f) / fp; }

/**
 * Solves a given quadratic equation with the given parameters.
 * Stores the solutions in {t1} and {t2}. The function returns true
 * or false depending on the existence of any solution.
 *
 * Error bound checks are automatically done by the FPError class
 */
inline bool solveQuadraticSystem(FPError a, FPError b, FPError c, FPError* t1, FPError* t2) {
    // Default improbable solutions
    *t1 = MinReal; *t2 = MaxReal;

    FPError det = b*b - 4*a*c, den = 2 * a;
    // No real solutions exist
    if (Real(det) < 0) return false;
    // Single solution
    else if (Real(det) == 0) { *t1 = -b / den; }
    // Two solutions
    else {
        det = std::sqrt(Real(det));
        *t1 = (-b - det) / den; *t2 = (-b + det) / den;
    }

    if (Real(*t1) > Real(*t2)) std::swap(*t1, *t2);
    return true;
}

} // namespace phyr

#endif
