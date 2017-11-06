#ifndef PHYRAY_CORE_PHYR_H
#define PHYRAY_CORE_PHYR_H

#include <cmath>
#include <limits>
#include <string>
#include <vector>

#include <cstddef>
#include <stdint.h>
#include <string.h>

#include <core/debug.h>

namespace phyr {

// Global defines
// Comment out following to use short precision
#define PHYRAY_USE_LONG_P
#ifdef PHYRAY_USE_LONG_P
#define Real double
#define Int int64_t
#else
#define Real float
#define Int int32_t
#endif

#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wdouble-promotion"

// #define PHYRAY_OPTIMIZE

// Forward class declarations
// Geometry
template <typename T> class Vector2;
template <typename T> class Vector3;
template <typename T> class Point2;
template <typename T> class Point3;
template <typename T> class Normal3;
template <typename T> class Bounds2;
template <typename T> class Bounds3;
class Ray;

// Transformations
class Transform;

// Interactions
class Interaction;
class SurfaceInteraction;

// Objects
class Shape;
class Object;
class Sphere;
class AreaLight;

class Material;
class MemoryPool;
class BSDF;

// Spectrum
template <int sampleSize>
class CoefficientSpectrum;
class SampledSpectrum;

typedef SampledSpectrum Spectrum;

// Camera
class Camera;

// Film
class Film;

class NaNCandidate {
  public:
    virtual bool hasNaNs() const;
    virtual ~NaNCandidate();
};

// Global constants
static constexpr Int MaxInt = std::numeric_limits<Int>::max();
static constexpr Real MaxReal = std::numeric_limits<Real>::max();
static constexpr Real MinReal = std::numeric_limits<Real>::min();
static constexpr Real Infinity = std::numeric_limits<Real>::infinity();

// Machine epsilon defined by the C/C++ standard is twice
// as large as the value of the term used in numerical analysis
static constexpr Real MachineEpsilon = std::numeric_limits<Real>::epsilon() * 0.5;
// The distance factor from an area light surface before which a
// shadow ray must stop before reporting an intersection with it
static constexpr Real ShadowEpsilon = 1e-5f;

// Max values used for sampling purposes
#ifdef PHYRAY_USE_LONG_P
static const Real OneMinusEpsilon = 0.99999999999999989;
#else
static const Real OneMinusEpsilon = 0.99999994;
#endif

// Minimum machine alignment
#if __GNUC__ == 4 && __GNUC_MINOR__ < 9
static const int MachineAlignment = alignof(max_align_t);
#else
static const int MachineAlignment = alignof(std::max_align_t);
#endif

static constexpr Real Pi = 3.14159265358979323846;
static constexpr Real InvPi = 0.31830988618379067154;
static constexpr Real Inv2Pi = 0.15915494309189533577;
static constexpr Real Inv4Pi = 0.07957747154594766788;
static constexpr Real PiOver2 = 1.57079632679489661923;
static constexpr Real PiOver4 = 0.78539816339744830961;
static constexpr Real Sqrt2 = 1.41421356237309504880;

static constexpr Real UnitDegree = Pi / 180;
static constexpr Real UnitRadian = 180 / Pi;

// Global utility functions
template <typename T>
inline bool isNaN(const T x) { return std::isnan(x); }
template<>
inline bool isNaN(const int x) { return false; }

template <typename T>
inline bool isZero(const T x) { return x == 0; }

inline Real lerp(Real f, Real v0, Real v1) { return (1 - f) * v0 + f * v1; }

inline Real radians(Real degrees) { return degrees * UnitDegree; }
inline Real degrees(Real radians) { return radians * UnitRadian; }

Int ulpsDistance(const Real v1, const Real v2);

inline bool epsEqual(Real v1, Real v2, Int ulpsEpsilon) {
    // Near zero case
    const Real diff = std::abs(v1 - v2);
    if (diff <= std::max(v1, v2) * MachineEpsilon) return true;

    return ulpsDistance(v1, v2) <= ulpsEpsilon;
}

/**
 * Tighter upper bound for rounding errors in
 * floating point operations, as defined in Higham (2002, Section 3.1)
 * @param n Number of unit operations to return error bound for
 */
inline constexpr Real gamma(int n) {
    return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
}

inline Real clamp(Real v, Real minv, Real maxv) {
    return std::min(std::max(v, minv), maxv);
}

inline uint32_t floatToBits(float f) {
    uint32_t i;
    memcpy(&i, &f, sizeof(float));
    return i;
}
inline uint64_t floatToBits(double f) {
    uint64_t i;
    memcpy(&i, &f, sizeof(double));
    return i;
}

inline float bitsToFloat(uint32_t i) {
    float f;
    memcpy(&f, &i, sizeof(i));
    return f;
}
inline double bitsToFloat(uint64_t i) {
    double f;
    memcpy(&f, &i, sizeof(i));
    return f;
}

inline float nextFloatUp(float f) {
    // Return positive infinity unchanged
    if (std::isinf(f) && f > 0.f) return f;
    // Promote negative zero to positive, as
    // their binary representations are not adjacent
    if (f == -0.f) f = 0.f;

    uint32_t i = floatToBits(f);
    // Increment i for positive values and decrement otherwise
    if (f >= 0) i++; else i--;
    return bitsToFloat(i);
}
inline double nextFloatUp(double f, int delta = 1) {
    // Return positive infinity unchanged
    if (std::isinf(f) && f > 0.) return f;
    // Promote negative zero to positive, as
    // their binary representations are not adjacent
    if (f == -0.) f = 0.;

    uint64_t i = floatToBits(f);
    // Increment i for positive values and decrement otherwise
    if (f >= 0) i += delta; else i -= delta;
    return bitsToFloat(i);
}

inline float nextFloatDown(float f) {
    // Return negative infinity unchanged
    if (std::isinf(f) && f < 0.f) return f;
    // Promote positive zero to negative, as
    // their binary representations are not adjacent
    if (f == 0.f) f = -0.f;

    uint32_t i = floatToBits(f);
    // Decrement i for positive values and increment otherwise
    if (f > 0) i--; else i++;
    return bitsToFloat(i);
}
inline double nextFloatDown(double f, int delta = 1) {
    // Return negative infinity unchanged
    if (std::isinf(f) && f < 0) return f;
    // Promote positive zero to negative, as
    // their binary representations are not adjacent
    if (f == 0.) f = -0.;

    uint64_t i = floatToBits(f);
    // Decrement i for positive values and increment otherwise
    if (f > 0) i -= delta; else i += delta;
    return bitsToFloat(i);
}

inline Real erf(Real x) {
    // Define constants
    Real a1 = 0.254829592f;
    Real a2 = -0.284496736f;
    Real a3 = 1.421413741f;
    Real a4 = -1.453152027f;
    Real a5 = 1.061405429f;
    Real p = 0.3275911f;

    // Save the sign of x
    int sign = 1;
    if (x < 0) sign = -1;
    x = std::abs(x);

    Real t = 1 / (1 + p * x);
    Real y = 1 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * std::exp(-x * x);

    return sign * y;
}

inline Real erfInv(Real x) {
    Real w, p;
    x = clamp(x, -.99999f, .99999f);
    w = -std::log((1 - x) * (1 + x));
    if (w < 5) {
        w = w - 2.5f;
        p = 2.81022636e-08f;
        p = 3.43273939e-07f + p * w;
        p = -3.5233877e-06f + p * w;
        p = -4.39150654e-06f + p * w;
        p = 0.00021858087f + p * w;
        p = -0.00125372503f + p * w;
        p = -0.00417768164f + p * w;
        p = 0.246640727f + p * w;
        p = 1.50140941f + p * w;
    } else {
        w = std::sqrt(w) - 3;
        p = -0.000200214257f;
        p = 0.000100950558f + p * w;
        p = 0.00134934322f + p * w;
        p = -0.00367342844f + p * w;
        p = 0.00573950773f + p * w;
        p = -0.0076224613f + p * w;
        p = 0.00943887047f + p * w;
        p = 1.00167406f + p * w;
        p = 2.83297682f + p * w;
    }

    return p * x;
}

} // namespace phyr

#endif
