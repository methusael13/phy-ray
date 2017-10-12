#ifndef PHYRAY_CORE_GEOMETRY_H
#define PHYRAY_CORE_GEOMETRY_H

#include <ostream>
#include <core/phyr.h>

namespace phyr {

// Core geometry primitive class declarations
class NaNCandidate {
  public:
    virtual bool hasNaNs() const;
    virtual ~NaNCandidate();
};

// Vector declarations
template <typename T>
class Vector2 : public NaNCandidate {
  public:
    // Constructor definition
    Vector2() : x(0), y(0) {}
    Vector2(T _x, T _y) : x(_x), y(_y) { ASSERT(!hasNaNs()); }

    explicit Vector2(const Point2<T>& p) { ASSERT(!p.hasNaNs()); x = p.x; y = p.y; }
    explicit Vector2(const Point3<T>& p) { ASSERT(!p.hasNaNs()); x = p.x; y = p.y; }
    bool hasNaNs() const override { return isNaN(x) || isNaN(y); }

#ifndef PHYRAY_OPTIMIZE
    Vector2(const Vector2<T>& ref) {
        ASSERT(!ref.hasNaNs());
        x = ref.x; y = ref.y;
    }

    Vector2<T>& operator=(const Vector2<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x = rhs.x; y = rhs.y;
        return *this;
    }
#endif

    // Vector operations
    Vector2<T> operator+(const Vector2<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Vector2<T>(x + rhs.x, y + rhs.y);
    }
    Vector2<T>& operator+=(const Vector2<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x += rhs.x; y += rhs.y;
        return *this;
    }

    Vector2<T> operator-(const Vector2<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Vector2<T>(x - rhs.x, y - rhs.y);
    }
    Vector2<T>& operator-=(const Vector2<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x -= rhs.x; y -= rhs.y;
        return *this;
    }

    template <typename U>
    Vector2<T> operator*(U v) const {
        ASSERT(!isNaN(v));
        return Vector2<T>(x * v, y * v);
    }
    template <typename U>
    Vector2<T>& operator*=(U v) {
        ASSERT(!isNaN(v));
        x *= v; y *= v;
        return *this;
    }

    template <typename U>
    Vector2<T> operator/(U v) const {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        return Vector2<T>(x * inv, y * inv);
    }
    template <typename U>
    Vector2<T>& operator/=(U v) {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        x *= inv; y *= inv;
        return *this;
    }

    Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

    bool operator==(const Vector2<T>& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Vector2<T>& rhs) const { return x != rhs.x || y != rhs.y; }

    inline T operator[](int i) const { ASSERT(i == 0 || i == 1); return i ? y : x; }
    inline T& operator[](int i) { ASSERT(i == 0 || i == 1); return i ? y : x; }

    Real lengthSquared() const { return x * x + y * y; }
    Real length() const { return std::sqrt(lengthSquared()); }

    T x, y;
};


template <typename T>
class Vector3 : public NaNCandidate {
  public:
    // Constructor definition
    Vector3() : x(0), y(0), z(0) {}
    Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { ASSERT(!hasNaNs()); }

    explicit Vector3(const Point3<T>& p) {
        ASSERT(!p.hasNaNs()); x = p.x; y = p.y; z = p.z;
    }
    explicit Vector3(const Normal3<T>& n) {
        ASSERT(!n.hasNaNs()); x = n.x; y = n.y; z = n.z;
    }
    bool hasNaNs() const override { return isNaN(x) || isNaN(y) || isNaN(z); }

#ifndef PHYRAY_OPTIMIZE
    Vector3(const Vector3<T>& ref) {
        ASSERT(!ref.hasNaNs());
        x = ref.x; y = ref.y; z = ref.z;
    }

    Vector3<T>& operator=(const Vector3<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x = rhs.x; y = rhs.y; z = rhs.z;
        return *this;
    }
#endif

    // Vector operations
    Vector3<T> operator+(const Vector3<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    Vector3<T>& operator+=(const Vector3<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }

    Vector3<T> operator-(const Vector3<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    Vector3<T>& operator-=(const Vector3<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
        return *this;
    }

    template <typename U>
    Vector3<T> operator*(U v) const {
        ASSERT(!isNaN(v));
        return Vector3<T>(x * v, y * v, z * v);
    }
    template <typename U>
    Vector3<T>& operator*=(U v) {
        ASSERT(!isNaN(v));
        x *= v; y *= v; z *= v;
        return *this;
    }

    template <typename U>
    Vector3<T> operator/(U v) const {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        return Vector3<T>(x * inv, y * inv, z * inv);
    }
    template <typename U>
    Vector3<T>& operator/=(U v) {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }

    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

    bool operator==(const Vector3<T>& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=(const Vector3<T>& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

    inline T operator[](int i) const {
        ASSERT(i >= 0 && i <= 2);
        return i == 0 ? x : i == 1 ? y : z;
    }
    inline T& operator[](int i) {
        ASSERT(i >= 0 && i <= 2);
        return i == 0 ? x : i == 1 ? y : z;
    }

    Real lengthSquared() const { return x * x + y * y + z * z; }
    Real length() const { return std::sqrt(lengthSquared()); }

    T x, y, z;
};

// Vector template typedefs
typedef Vector2<int> Vector2i;
typedef Vector2<Real> Vector2f;
typedef Vector3<int> Vector3i;
typedef Vector3<Real> Vector3f;

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec) {
    return (os << "[" << vec.x << ", " << vec.y << "]");
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec) {
    return (os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]");
}


// Point declarations
template <typename T>
class Point2 : public NaNCandidate {
  public:
    Point2() : x(0), y(0) {}
    Point2(T _x, T _y) : x(_x), y(_y) { ASSERT(!hasNaNs()); }

    explicit Point2(const Point3<T>& p) { ASSERT(!p.hasNaNs()); x = p.x; y = p.y; }
    bool hasNaNs() const override { return isNaN(x) || isNaN(y); }

    // Construction from other types
    template <typename U>
    explicit Point2(const Point2<U>& up) {
        x = T(up.x); y = T(up.y);
        ASSERT(!hasNaNs());
    }

    template <typename U>
    explicit Point2(const Vector2<U>& uv) {
        x = T(uv.x); y = T(uv.y);
        ASSERT(!hasNaNs());
    }

    template <typename U>
    explicit operator Vector2<U>() const { return Vector2<U>(x, y); }

#ifndef PHYRAY_OPTIMIZE
    Point2(const Point2<T>& ref) {
        ASSERT(!ref.hasNaNs());
        x = ref.x; y = ref.y;
    }

    Point2<T>& operator=(const Point2<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x = rhs.x; y = rhs.y;
        return *this;
    }
#endif

    // Point operations
    Point2<T> operator+(const Point2<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Point2<T>(x + rhs.x, y + rhs.y);
    }
    Point2<T> operator+(const Vector2<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Point2<T>(x + rhs.x, y + rhs.y);
    }

    Point2<T>& operator+=(const Point2<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x += rhs.x; y += rhs.y;
        return *this;
    }
    Point2<T>& operator+=(const Vector2<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x += rhs.x; y += rhs.y;
        return *this;
    }

    Vector2<T> operator-(const Point2<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Vector2<T>(x - rhs.x, y - rhs.y);
    }
    Point2<T> operator-(const Vector2<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Point2<T>(x - rhs.x, y - rhs.y);
    }

    Point2<T> operator-=(const Vector2<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x -= rhs.x; y -= rhs.y;
        return *this;
    }

    Point2<T> operator-() const { return Point2<T>(-x, -y); }

    template <typename U>
    Point2<T> operator*(U v) const {
        ASSERT(!isNaN(v));
        return Point2<T>(x * v, y * v);
    }
    template <typename U>
    Point2<T>& operator*=(U v) {
        ASSERT(!isNaN(v));
        x *= v; y *= v;
        return *this;
    }

    template <typename U>
    Point2<T> operator/(U v) const {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        return Point2<T>(x * inv, y * inv);
    }
    template <typename U>
    Point2<T>& operator/=(U v) {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        x *= inv; y *= inv;
        return *this;
    }

    bool operator==(const Point2<T>& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Point2<T>& rhs) const { return x != rhs.x || y != rhs.y; }

    inline T operator[](int i) const { ASSERT(i == 0 || i == 1); return i ? y : x; }
    inline T& operator[](int i) { ASSERT(i == 0 || i == 1); return i ? y : x; }

    T x, y;
};


template <typename T>
class Point3 : public NaNCandidate {
  public:
    Point3() : x(0), y(0), z(0) {}
    Point3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { ASSERT(!hasNaNs()); }

    bool hasNaNs() const override { return isNaN(x) || isNaN(y); }

    // Construction from other types
    template <typename U>
    explicit Point3(const Point3<U>& up) {
        x = T(up.x); y = T(up.y), z = T(up.z);
        ASSERT(!hasNaNs());
    }

    template <typename U>
    explicit Point3(const Vector3<U>& uv) {
        x = T(uv.x); y = T(uv.y), z = T(uv.z);
        ASSERT(!hasNaNs());
    }

    template <typename U>
    explicit operator Vector3<U>() const { return Vector3<U>(x, y, z); }

#ifndef PHYRAY_OPTIMIZE
    Point3(const Point3<T>& ref) {
        ASSERT(!ref.hasNaNs());
        x = ref.x; y = ref.y; z = ref.z;
    }

    Point3<T>& operator=(const Point3<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x = rhs.x; y = rhs.y; z = rhs.z;
        return *this;
    }
#endif

    // Point operations
    Point3<T> operator+(const Point3<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Point3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    Point3<T> operator+(const Vector3<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Point3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Point3<T>& operator+=(const Point3<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }
    Point3<T>& operator+=(const Vector3<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }

    Vector3<T> operator-(const Point3<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    Point3<T> operator-(const Vector3<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Point3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Point3<T> operator-=(const Vector3<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
        return *this;
    }

    Point3<T> operator-() const { return Point3<T>(-x, -y, -z); }

    template <typename U>
    Point3<T> operator*(U v) const {
        ASSERT(!isNaN(v));
        return Point3<T>(x * v, y * v, z * v);
    }
    template <typename U>
    Point3<T>& operator*=(U v) {
        ASSERT(!isNaN(v));
        x *= v; y *= v; z *= v;
        return *this;
    }

    template <typename U>
    Point3<T> operator/(U v) const {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        return Point3<T>(x * inv, y * inv, z * inv);
    }
    template <typename U>
    Point3<T>& operator/=(U v) {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }

    bool operator==(const Point3<T>& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=(const Point3<T>& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

    inline T operator[](int i) const {
        ASSERT(i >= 0 && i <= 2);
        return i == 0 ? x : i == 1 ? y : z;
    }
    inline T& operator[](int i) {
        ASSERT(i >= 0 && i <= 2);
        return i == 0 ? x : i == 1 ? y : z;
    }

    T x, y, z;
};

// Point template typedefs
typedef Point2<int> Point2i;
typedef Point2<Real> Point2f;
typedef Point3<int> Point3i;
typedef Point3<Real> Point3f;

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Point2<T>& p) {
    return (os << "[" << p.x << ", " << p.y << "]");
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Point3<T>& p) {
    return (os << "[" << p.x << ", " << p.y << ", " << p.z << "]");
}


// Normal declarations
template <typename T>
class Normal3 : public NaNCandidate {
  public:
    Normal3() : x(0), y(0), z(0) {}
    Normal3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { ASSERT(!hasNaNs()); }

    explicit Normal3(const Vector3<T>& ref) {
        ASSERT(!ref.hasNaNs());
        x = ref.x; y = ref.y; z = ref.z;
    }
    bool hasNaNs() const override { return isNaN(x) || isNaN(y) || isNaN(z); }

#ifndef PHYRAY_OPTIMIZE
    Normal3(const Normal3<T>& ref) {
        ASSERT(!ref.hasNaNs());
        x = ref.x; y = ref.y; z = ref.z;
    }

    Normal3<T>& operator=(const Normal3<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x = rhs.x; y = rhs.y; z = rhs.z;
        return *this;
    }
#endif

    // Normal operations
    Normal3<T> operator+(const Normal3<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Normal3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    Normal3<T>& operator+=(const Normal3<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }

    Normal3<T> operator-(const Normal3<T>& rhs) const {
        ASSERT(!rhs.hasNaNs());
        return Normal3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    Normal3<T>& operator-=(const Normal3<T>& rhs) {
        ASSERT(!rhs.hasNaNs());
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
        return *this;
    }

    template <typename U>
    Normal3<T> operator*(U v) const {
        ASSERT(!isNaN(v));
        return Normal3<T>(x * v, y * v, z * v);
    }
    template <typename U>
    Normal3<T>& operator*=(U v) {
        ASSERT(!isNaN(v));
        x *= v; y *= v; z *= v;
        return *this;
    }

    template <typename U>
    Normal3<T> operator/(U v) const {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        return Normal3<T>(x * inv, y * inv, z * inv);
    }
    template <typename U>
    Normal3<T>& operator/=(U v) {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }

    Normal3<T> operator-() const { return Normal3<T>(-x, -y, -z); }

    bool operator==(const Normal3<T>& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=(const Normal3<T>& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

    inline T operator[](int i) const {
        ASSERT(i >= 0 && i <= 2);
        return i == 0 ? x : i == 1 ? y : z;
    }
    inline T& operator[](int i) {
        ASSERT(i >= 0 && i <= 2);
        return i == 0 ? x : i == 1 ? y : z;
    }

    Real lengthSquared() const { return x * x + y * y + z * z; }
    Real length() const { return std::sqrt(lengthSquared()); }

    T x, y, z;
};

// Normal template typedefs
typedef Normal3<Real> Normal3f;

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Normal3<T>& n) {
    return (os << "[" << n.x << ", " << n.y << ", " << n.z << "]");
}


// Ray declarations
class Ray : public NaNCandidate {
  public:
    Ray() : tMax(Infinity) {}
    Ray(const Point3f& origin, const Vector3f& direction, Real tMax = Infinity) :
        o(origin), d(direction), tMax(tMax) {}

    Point3f operator()(Real t) const { return o + d * t; }
    bool hasNaNs() const override { return o.hasNaNs() || d.hasNaNs() || isNaN(tMax); }

    Point3f o;   // Origin
    Vector3f d;  // Direction
    mutable Real tMax;
};

// Bounds declarations
template <typename T>
class Bounds2 {
  public:
    Bounds2() {
        T minN = std::numeric_limits<T>::lowest();
        T maxN = std::numeric_limits<T>::max();
        pMin = Point2<T>(minN, minN); pMax = Point2<T>(maxN, maxN);
    }

    explicit Bounds2(const Point2<T>& p) : pMin(p), pMax(p) {}
    explicit Bounds2(const Point2<T>& p1, const Point2<T>& p2) {
        pMin = Point2<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
        pMax = Point2<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
    }

    template <typename U>
    explicit operator Bounds2<U>() const {
        return Bounds2<U>(Point2<U>(pMin), Point2<U>(pMax));
    }

    Vector2<T> diagonal() const { return pMax - pMin; }
    T area() const { Vector2<T> d = diagonal(); return d.x * d.y; }
    int maximumExtent() const { Vector2<T> d = diagonal(); return d.x > d.y ? 0 : 1; }

    Point2<T> lerp(const Point2<T>& p) const {
        return Point2<T>(phyr::lerp(p.x, pMin.x, pMax.x),
                         phyr::lerp(p.y, pMin.y, pMax.y));
    }

    Vector2<T> offset(const Point2<T>& p) const {
        Vector2<T> off = p - pMin;
        if (pMax.x > pMin.x) off.x /= pMax.x - pMin.x;
        if (pMax.y > pMin.y) off.y /= pMax.y - pMin.y;
        return off;
    }

    void boundingSphere(Point2<T>* center, Real* rad) const {
        *center = (pMin + pMax) / 2;
        *rad = inside(*center, *this) ? distance(*center, pMax) : 0;
    }

    bool isValid() const { return pMin.x <= pMax.x && pMin.y <= pMax.y; }
    bool operator==(const Bounds2<T>& b) const { return b.pMin == pMin && b.pMax == pMax; }
    bool operator!=(const Bounds2<T>& b) const { return b.pMin != pMin || b.pMax != pMax; }

    inline const Point2<T>& operator[](int i) const {
        ASSERT(i == 0 || i == 1);
        return i == 0 ? pMin : pMax;
    }
    inline Point2<T>& operator[](int i) {
        ASSERT(i == 0 || i == 1);
        return i == 0 ? pMin : pMax;
    }

    Point2<T> pMin, pMax;
};


template <typename T>
class Bounds3 {
  public:
    Bounds3() {
        T minN = std::numeric_limits<T>::lowest();
        T maxN = std::numeric_limits<T>::max();
        pMin = Point3<T>(minN, minN, minN); pMax = Point3<T>(maxN, maxN, maxN);
    }

    explicit Bounds3(const Point3<T>& p) : pMin(p), pMax(p) {}
    explicit Bounds3(const Point3<T>& p1, const Point3<T>& p2) {
        pMin = Point3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
        pMax = Point3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
    }

    template <typename U>
    explicit operator Bounds3<U>() const {
        return Bounds3<U>(Point3<U>(pMin), Point3<U>(pMax));
    }

    Vector3<T> diagonal() const { return pMax - pMin; }
    T volume() const { Vector3<T> d = diagonal(); return d.x * d.y * d.z; }
    T surfaceArea() const {
        Vector3<T> d = diagonal();
        return 2 * (d.x * d.y + d.y * d.z + d.z * d.x);
    }
    int maximumExtent() const {
        Vector3<T> d = diagonal();
        return d.x > d.y ? (d.x > d.z) ? 0 : 2 : (d.y > d.z) ? 1 : 2;
    }

    /**
     * Corner indexes of the box are labelled 0 to 7.
     * Each corner can be specified by the individual components
     * of pMin and pMax. The individual bits of the given corner
     * index is tested to determine whether a component of the
     * resultant corner point should be included from pMin or pMax.
     * See the examples below:
     *
     * 5 => 101:
     *      0th bit is 1, choose pMax.x as x component
     *      1st bit is 0, choose pMin.y as y component
     *      2nd bit is 1, choose pMax.z as z component
     *
     * 2 => 010:
     *      0th bit is 0, choose pMin.x as x component
     *      1st bit is 1, choose pMax.y as y component
     *      2nd bit is 0, choose pMin.z as z component
     */
    Point3<T> corner(int i) const {
        ASSERT(i >= 0 && i <= 7);
        return Point3<T>((*this)[(i & 1)].x,
                         (*this)[(i & 2) ? 1 : 0].y, (*this)[(i & 2) ? 1 : 0].z);
    }

    Point3<T> lerp(const Point3<T>& p) const {
        return Point3<T>(phyr::lerp(p.x, pMin.x, pMax.x),
                         phyr::lerp(p.y, pMin.y, pMax.y),
                         phyr::lerp(p.z, pMin.z, pMax.z));
    }

    Vector3<T> offset(const Point3<T>& p) const {
        Vector3<T> off = p - pMin;
        if (pMax.x > pMin.x) off.x /= pMax.x - pMin.x;
        if (pMax.y > pMin.y) off.y /= pMax.y - pMin.y;
        if (pMax.z > pMin.z) off.z /= pMax.z - pMin.z;
        return off;
    }

    void boundingSphere(Point3<T>* center, Real* rad) const {
        *center = (pMin + pMax) / 2;
        *rad = inside(*center, *this) ? distance(*center, pMax) : 0;
    }

    inline bool intersectRay(const Ray& ray, Real* tnear = nullptr, Real* tfar = nullptr) const;
    inline bool intersectRay(const Ray& ray, const Vector3<T>& invDir, const int isDirNeg[3]) const;

    bool isValid() const { return pMin.x <= pMax.x && pMin.y <= pMax.y && pMin.z <= pMax.z; }
    bool operator==(const Bounds3<T>& b) const { return b.pMin == pMin && b.pMax == pMax; }
    bool operator!=(const Bounds3<T>& b) const { return b.pMin != pMin || b.pMax != pMax; }

    inline const Point3<T>& operator[](int i) const {
        ASSERT(i == 0 || i == 1);
        return i == 0 ? pMin : pMax;
    }
    inline Point3<T>& operator[](int i) {
        ASSERT(i == 0 || i == 1);
        return i == 0 ? pMin : pMax;
    }

    Point3<T> pMin, pMax;
};

// Bounds template typedefs
typedef Bounds2<int> Bounds2i;
typedef Bounds2<Real> Bounds2f;
typedef Bounds3<int> Bounds3i;
typedef Bounds3<Real> Bounds3f;

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Bounds2<T>& b) {
    return (os << "[ " << b.pMin << " - " << b.pMax << " ]");
}
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Bounds3<T>& b) {
    return (os << "[ " << b.pMin << " - " << b.pMax << " ]");
}


// Geometry inline functions
template <typename T, typename U>
inline Vector2<T> operator*(U s, const Vector2<T>& v) { return v * s; }
template <typename T, typename U>
inline Vector3<T> operator*(U s, const Vector3<T>& v) { return v * s; }
template <typename T, typename U>
inline Point2<T> operator*(U s, const Point2<T>& p) { return p * s; }
template <typename T, typename U>
inline Point3<T> operator*(U s, const Point3<T>& p) { return p * s; }
template <typename T, typename U>
inline Normal3<T> operator*(U s, const Normal3<T>& n) { return n * s; }

// Dot products
template <typename T>
inline T dot(const Vector2<T>& v1, const Vector2<T>& v2) {
    ASSERT(!v1.hasNaNs() && !v2.hasNaNs());
    return v1.x * v2.x + v1.y * v2.y;
}
template <typename T>
inline T dot(const Vector3<T>& v1, const Vector3<T>& v2) {
    ASSERT(!v1.hasNaNs() && !v2.hasNaNs());
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template <typename T>
inline T dot(const Vector3<T>& v, const Normal3<T>& n) {
    ASSERT(!v.hasNaNs() && !n.hasNaNs());
    return v.x * n.x + v.y * n.y + v.z * n.z;
}
template <typename T>
inline T dot(const Normal3<T>& n, const Vector3<T>& v) {
    ASSERT(!n.hasNaNs() && !v.hasNaNs());
    return n.x * v.x + n.y * v.y + n.z * v.z;
}
template <typename T>
inline T dot(const Normal3<T>& n1, const Normal3<T>& n2) {
    ASSERT(!n1.hasNaNs() && !n2.hasNaNs());
    return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
}

// AbsDot products
template <typename T>
inline T absDot(const Vector2<T>& v1, const Vector2<T>& v2) {
    return std::abs(dot(v1, v2));
}
template <typename T>
inline T absDot(const Vector3<T>& v1, const Vector3<T>& v2) {
    return std::abs(dot(v1, v2));
}
template <typename T>
inline T absDot(const Vector3<T>& v, const Normal3<T>& n) {
    return std::abs(dot(v, n));
}
template <typename T>
inline T absDot(const Normal3<T>& n, const Vector3<T>& v) {
    return std::abs(dot(n, v));
}
template <typename T>
inline T absDot(const Normal3<T>& n1, const Normal3<T>& n2) {
    return std::abs(dot(n1, n2));
}

// Abs operations
template <typename T>
inline Vector2<T> abs(const Vector2<T>& v) {
    return Vector2<T>(std::abs(v.x), std::abs(v.y));
}
template <typename T>
inline Vector3<T> abs(const Vector3<T>& v) {
    return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}
template <typename T>
inline Point2<T> abs(const Point2<T>& p) {
    return Point2<T>(std::abs(p.x), std::abs(p.y));
}
template <typename T>
inline Point3<T> abs(const Point3<T>& p) {
    return Point3<T>(std::abs(p.x), std::abs(p.y), std::abs(p.z));
}
template <typename T>
inline Normal3<T> abs(const Normal3<T>& n) {
    return Normal3<T>(std::abs(n.x), std::abs(n.y), std::abs(n.z));
}

// Cross products
template <typename T>
inline Vector3<T> cross(const Vector3<T>& v1, const Vector3<T>& v2) {
    ASSERT(!v1.hasNaNs() && !v2.hasNaNs());
    double x1 = v1.x, y1 = v1.y, z1 = v1.z;
    double x2 = v2.x, y2 = v2.y, z2 = v2.z;
    return Vector3<T>((y1 * z2 - z1 * y2), (z1 * x2 - x1 * z2), (x1 * y2 - y1 * x2));
}
template <typename T>
inline Vector3<T> cross(const Vector3<T>& v, const Normal3<T>& n) {
    ASSERT(!v.hasNaNs() && !n.hasNaNs());
    double x1 = v.x, y1 = v.y, z1 = v.z;
    double x2 = n.x, y2 = n.y, z2 = n.z;
    return Vector3<T>((y1 * z2 - z1 * y2), (z1 * x2 - x1 * z2), (x1 * y2 - y1 * x2));
}
template <typename T>
inline Vector3<T> cross(const Normal3<T>& n, const Vector3<T>& v) {
    ASSERT(!n.hasNaNs() && !v.hasNaNs());
    double x1 = n.x, y1 = n.y, z1 = n.z;
    double x2 = v.x, y2 = v.y, z2 = v.z;
    return Vector3<T>((y1 * z2 - z1 * y2), (z1 * x2 - x1 * z2), (x1 * y2 - y1 * x2));
}
template <typename T>
inline Normal3<T> cross(const Normal3<T>& n1, const Normal3<T>& n2) {
    ASSERT(!n1.hasNaNs() && !n2.hasNaNs());
    double x1 = n1.x, y1 = n1.y, z1 = n1.z;
    double x2 = n2.x, y2 = n2.y, z2 = n2.z;
    return Normal3<T>((y1 * z2 - z1 * y2), (z1 * x2 - x1 * z2), (x1 * y2 - y1 * x2));
}

// Normalize operations
template <typename T>
inline Vector2<T> normalize(const Vector2<T>& v) { return v / v.length(); }
template <typename T>
inline Vector3<T> normalize(const Vector3<T>& v) { return v / v.length(); }
template <typename T>
inline Normal3<T> normalize(const Normal3<T>& n) { return n / n.length(); }

// Miscellaneous operations
template <typename T>
T minComponent(const Vector3<T>& v);
template <typename T>
T maxComponent(const Vector3<T>& v);

template <typename T>
int maxDimension(const Vector3<T>& v);
template <typename T>
Vector3<T> permute(const Vector3<T>& v, int i, int j, int k);
template <typename T>
Point3<T> permute(const Point3<T>& v, int i, int j, int k);

/**
 * To create a coordinate system, we need to calculate two other
 * orthogonal vectors from v1.
 * The second vector can be created by zeroing one of the components
 * of v1, swapping the remaining two, and then negating one of them.
 * The third vector then is simply the cross product of the previous two vectors.
 *
 * The input vector v1 is assumed to be already normalized.
 */
template <typename T>
inline void coordinateSystem(const Vector3<T>& v1, Vector3<T>* v2, Vector3<T>* v3) {
    ASSERT(v2 != nullptr && v3 != nullptr);
    if (std::abs(v1.x) > std::abs(v1.y))
        *v2 = Vector3<T>(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
    else
        *v2 = Vector3<T>(0, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
    *v3 = cross(v1, *v2);
}

template <typename T>
inline Real distanceSquared(const Point2<T>& p1, const Point2<T>& p2) {
    return (p1 - p2).lengthSquared();
}
template <typename T>
inline Real distanceSquared(const Point3<T>& p1, const Point3<T>& p2) {
    return (p1 - p2).lengthSquared();
}

template <typename T>
inline Real distance(const Point2<T>& p1, const Point2<T>& p2) {
    return (p1 - p2).length();
}
template <typename T>
inline Real distance(const Point3<T>& p1, const Point3<T>& p2) {
    return (p1 - p2).length();
}

template <typename T>
Point2<T> lerp(Real f, const Point2<T>& p1, const Point2<T>& p2);
template <typename T>
Point3<T> lerp(Real f, const Point3<T>& p1, const Point3<T>& p2);

template <typename T>
Point2<T> floor(const Point2<T>& p);
template <typename T>
Point3<T> floor(const Point3<T>& p);

template <typename T>
Point2<T> ceil(const Point2<T>& p);
template <typename T>
Point3<T> ceil(const Point3<T>& p);

template <typename T>
inline Vector3<T> faceForward(const Vector3<T>& v1, const Vector3<T>& v2) {
    return (dot(v1, v2) < 0.0f) ? -v1 : v1;
}
template <typename T>
inline Vector3<T> faceForward(const Vector3<T>& v, const Normal3<T>& n) {
    return (dot(v, n) < 0.0f) ? -v : v;
}
template <typename T>
inline Normal3<T> faceForward(const Normal3<T>& n, const Vector3<T>& v) {
    return (dot(n, v) < 0.0f) ? -n : n;
}
template <typename T>
inline Normal3<T> faceForward(const Normal3<T>& n1, const Normal3<T>& n2) {
    return (dot(n1, n2) < 0.0f) ? -n1 : n1;
}

// Min, Max
template <typename T>
Vector3<T> min(const Vector3<T>& v1, const Vector3<T>& v2);
template <typename T>
Point2<T> min(const Point2<T>& p1, const Point2<T>& p2);
template <typename T>
Point3<T> min(const Point3<T>& p1, const Point3<T>& p2);
template <typename T>
Vector3<T> max(const Vector3<T>& v1, const Vector3<T>& v2);
template <typename T>
Point2<T> max(const Point2<T>& p1, const Point2<T>& p2);
template <typename T>
Point3<T> max(const Point3<T>& p1, const Point3<T>& p2);


// Bounds operations
template <typename T>
Bounds2<T> unionBounds(const Bounds2<T>& b, const Point2<T>& p);
template <typename T>
Bounds2<T> unionBounds(const Bounds2<T>& b1, const Bounds2<T>& b2);
template <typename T>
Bounds3<T> unionBounds(const Bounds3<T>& b, const Point3<T>& p);
template <typename T>
Bounds3<T> unionBounds(const Bounds3<T>& b1, const Bounds3<T>& b2);

template <typename T>
inline Bounds2<T> intersect(const Bounds2<T>& b1, const Bounds2<T>& b2) {
    // Assign the values directly to pMin and pMax.
    // Using the Bounds2 constructor will recalculate
    // the min/max of the supplied points, thus nullifying
    // the result we want to achieve.
    Bounds2<T> res;
    res.pMin = max(b1.pMin, b2.pMin);
    res.pMax = min(b1.pMax, b2.pMax);
    return res;
}
template <typename T>
inline Bounds3<T> intersect(const Bounds3<T>& b1, const Bounds3<T>& b2) {
    // Assign the values directly to pMin and pMax.
    // Using the Bounds3 constructor will recalculate
    // the min/max of the supplied points, thus nullifying
    // the result we want to achieve.
    Bounds3<T> res;
    res.pMin = max(b1.pMin, b2.pMin);
    res.pMax = min(b1.pMax, b2.pMax);
    return res;
}

template <typename T>
inline bool overlaps(const Bounds2<T>& b1, const Bounds2<T>& b2) {
    bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
    bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
    return (x && y);
}
template <typename T>
inline bool overlaps(const Bounds3<T>& b1, const Bounds3<T>& b2) {
    bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
    bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
    bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
    return (x && y && z);
}

template <typename T>
inline bool inside(const Point2<T>& p, const Bounds2<T>& b) {
    return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
            p.y <= b.pMax.y);
}
template <typename T>
inline bool inside(const Point3<T>& p, const Bounds3<T>& b) {
    return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
            p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
}

template <typename T>
inline bool insideExclusive(const Point2<T>& p, const Bounds2<T>& b) {
    return (p.x >= b.pMin.x && p.x < b.pMax.x && p.y >= b.pMin.y && p.y < b.pMax.y);
}
template <typename T>
inline bool insideExclusive(const Point3<T>& p, const Bounds3<T>& b) {
    return (p.x >= b.pMin.x && p.x < b.pMax.x && p.y >= b.pMin.y &&
            p.y < b.pMax.y && p.z >= b.pMin.z && p.z < b.pMax.z);
}

template <typename T, typename U>
inline Bounds2<T> expand(const Bounds2<T>& b, U delta) {
    return Bounds2<T>(b.pMin - Vector2<T>(delta, delta),
                      b.pMax + Vector2<T>(delta, delta));
}
template <typename T, typename U>
inline Bounds3<T> expand(const Bounds3<T>& b, U delta) {
    return Bounds3<T>(b.pMin - Vector3<T>(delta, delta, delta),
                      b.pMax + Vector3<T>(delta, delta, delta));
}

/**
 * Minimum squared distance from a point to a bounding box.
 * Returns 0 if the point lies inside the box
 */
template <typename T, typename U>
inline Real distanceSquared(const Point3<T>& p, const Bounds3<U>& b) {
#if __cplusplus >= 201402L
    Real dx = std::max({ Real(0), b.pMin.x - p.x, p.x - b.pMax.x });
    Real dy = std::max({ Real(0), b.pMin.y - p.y, p.y - b.pMax.y });
    Real dz = std::max({ Real(0), b.pMin.z - p.z, p.z - b.pMax.z });
#else
    Real dx = std::max(Real(0), std::max(b.pMin.x - p.x, p.x - b.pMax.x));
    Real dy = std::max(Real(0), std::max(b.pMin.y - p.y, p.y - b.pMax.y));
    Real dz = std::max(Real(0), std::max(b.pMin.z - p.z, p.z - b.pMax.z));
#endif
    return dx * dx + dy * dy + dz * dz;
}

template <typename T, typename U>
inline Real distance(const Point3<T>& p, const Bounds3<U>& b) {
    return std::sqrt(distanceSquared(p, b));
}

template <typename T>
inline bool Bounds3<T>::intersectRay(const Ray& ray, Real* tnear, Real* tfar) const {
    Real tn = 0., tf = ray.tMax;
    // Test for intersection against each
    // axis-aligned slab of the bounding box
    for (int i = 0; i < 3; i++) {
        Real dInv = 1.0 / ray.d[i];
        Real _tn = (pMin[i] - ray.o[i]) * dInv;
        Real _tf = (pMax[i] - ray.o[i]) * dInv;

        // Make sure _tn <= _tf
        if (_tn > _tf) std::swap(_tn, _tf);
        // Increase _tf's error bounds to conservatively return true
        // in case of comparisons when there's an overlap in their error bounds
        _tf *= 1 + 2 * gamma(3);
        // Make sure the comparisons don't result in retention of NaN.
        // Using the fact that comparison of any sorts with NaN is always false
        tn = _tn > tn ? _tn : tn;
        tf = _tf < tf ? _tf : tf;
        if (tn > tf) return false;
    }

    if (tnear) *tnear = tn; if (tfar) *tfar = tf;
    return true;
}

/**
 * Slightly optimized version of the function above.
 * Requires inverse and sign of direction vectors to be precomputed
 */
template <typename T>
inline bool Bounds3<T>::intersectRay(const Ray& ray, const Vector3<T>& invDir,
                                     const int isDirNeg[3]) const {
    const Bounds3<T>& b = *this;
    // Test against x and y slabs
    Real tmin =  (b[  isDirNeg[0]].x - ray.o.x) * invDir.x;
    Real tmax =  (b[1-isDirNeg[0]].x - ray.o.x) * invDir.x;
    Real tymin = (b[  isDirNeg[1]].y - ray.o.y) * invDir.y;
    Real tymax = (b[1-isDirNeg[1]].y - ray.o.y) * invDir.y;

    // Adjust for cases of overlapping error bounds
    tmax *= 1 + 2 * gamma(3); tymax *= 1 + 2 * gamma(3);
    if (tymin > tmax || tymax < tmin) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    // Test against z slab
    Real tzmin = (b[  isDirNeg[2]].z - ray.o.z) * invDir.z;
    Real tzmax = (b[1-isDirNeg[2]].z - ray.o.z) * invDir.z;

    // Adjust for cases of overlapping error bounds
    tzmax *= 1 + 2 * gamma(3);
    if (tzmin > tmax || tzmax < tmin) return false;
    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    return (tmax < ray.tMax) && (tmin > 0);
}

inline Point3f offsetRayOrigin(const Point3f& p, const Normal3f& n,
                               const Vector3f& w, const Vector3f& fpError) {
    Real d = dot(abs(n), fpError);
#ifdef PHYRAY_USE_LONG_P
    d *= 512;  // Use higher ulps factor for double
#endif

    Vector3f offset = Vector3f(n) * d;
    if (dot(w, n) < 0) offset = -offset;
    Point3f op = p + offset;

    // Round indiviual components up or down depending on the sign
    for (int i = 0; i < 3; i++) {
        if (offset[i] > 0) op[i] = nextFloatUp(op[i]);
        else if (offset[i] < 0) op[i] = nextFloatDown(op[i]);
    }

    return op;
}

} // namespace phyr

#endif
