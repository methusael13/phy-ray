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

} // namespace phyr

#endif
