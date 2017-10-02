
#ifndef PHYRAY_CORE_PHYR_H
#define PHYRAY_CORE_PHYR_H

#include <cmath>
#include <limits>
#include <string>
#include <vector>
#include <cstdint>

#include <core/debug.h>

namespace phyr {

// Global defines
// Comment out following to use short precision
// #define PHYRAY_USE_LONG_P
#ifdef PHYRAY_USE_LONG_P
#define Real double
#else
#define Real float
#endif

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
class RayDifferential;

// Transformations


// Global utility functions
template <typename T>
inline bool isNaN(const T x) { return std::isnan(x); }
template<>
inline bool isNaN(const int x) { return false; }

template <typename T>
inline bool isZero(const T x) { return x == 0; }

}

#endif
