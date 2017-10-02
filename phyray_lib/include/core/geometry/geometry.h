#ifndef PHYRAY_CORE_GEOMETRY_H
#define PHYRAY_CORE_GEOMETRY_H

#include <ostream>

#include <core/phyr.h>

namespace phyr {

// Core geometry primitive class declarations

// Vector declarations
template <typename T>
class Vector2 {
  public:
    // Constructor definition
    Vector2() : x(0), y(0) {}
    Vector2(T _x, T _y) : x(_x), y(_y) { ASSERT(!hasNaNs()); }

    explicit Vector2(const Point2<T>& p);
    explicit Vector2(const Point3<T>& p);
    bool hasNaNs() const { return isNaN(x) || isNaN(y); }

#ifndef PHYRAY_OPTIMIZE
    // Following should be used instead of the
    // default when compiling a debug build
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
        return Vector2<T>(x * v, y * v);
    }
    template <typename U>
    Vector2<T>& operator/=(U v) {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        x *= v; y *= v;
        return *this;
    }

    Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

    bool operator==(const Vector2<T>& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Vector2<T>& rhs) const { return x != rhs.x || y == rhs.y; }

    T operator[](int i) const { ASSERT(i == 0 || i == 1); return i ? y : x; }
    T& operator[](int i) { ASSERT(i == 0 || i == 1); return i ? y : x; }

    Real lengthSquared() const { return x * x + y * y; }
    Real length() const { return std::sqrt(lengthSquared()); }

    T x, y;
};


template <typename T>
class Vector3 {
  public:
    // Constructor definition
    Vector3() : x(0), y(0), z(0) {}
    Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { ASSERT(!hasNaNs()); }

    explicit Vector3(const Point3<T>& p);
    explicit Vector3(const Normal3<T>& n);
    bool hasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }

#ifndef PHYRAY_OPTIMIZE
    // Following should be used instead of the
    // default when compiling a debug build
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
        return Vector3<T>(x * v, y * v, z * v);
    }
    template <typename U>
    Vector3<T>& operator/=(U v) {
        ASSERT(!isNaN(v) && !isZero(v));
        Real inv = Real(1) / v;
        x *= v; y *= v; z *= v;
        return *this;
    }

    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

    bool operator==(const Vector3<T>& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=(const Vector3<T>& rhs) const { return x != rhs.x || y == rhs.y || z == rhs.z; }

    T operator[](int i) const {
        ASSERT(i >= 0 && i <= 2);
        return i == 0 ? x : i == 1 ? y : z;
    }
    T& operator[](int i) {
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
inline std::ostream operator<<(std::ostream& os, const Vector2<T>& vec) {
    return (os << "[" << vec.x << ", " << vec.y << "]");
}

template <typename T>
inline std::ostream operator<<(std::ostream& os, const Vector3<T>& vec) {
    return (os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]");
}


// Point declarations
template <typename T>
class Point2 {
  public:
    Point2() : x(0), y(0) {}
    Point2(T _x, T _y) : x(_x), y(_y) { ASSERT(!hasNaNs()); }

    explicit Point2(const Point3<T>& p);
    bool hasNaNs() const { return isNaN(x) || isNaN(y); }

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
    explicit operator Vector2<U>() const {
        return Vector2<U>(x, y);
    }

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
        return Point2<T>(x / v, y / v);
    }
    template <typename U>
    Point2<T>& operator/=(U v) {
        ASSERT(!isNaN(v) && !isZero(v));
        x /= v; y /= v;
        return *this;
    }

    bool operator==(const Point2<T>& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Point2<T>& rhs) const { return x != rhs.x || y != rhs.y; }

    T operator[](int i) const { ASSERT(i == 0 || i == 1); return i ? y : x; }
    T& operator[](int i) { ASSERT(i == 0 || i == 1); return i ? y : x; }

    T x, y;
};


template <typename T>
class Point3 {
  public:
    Point3() : x(0), y(0), z(0) {}
    Point3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { ASSERT(!hasNaNs()); }

    bool hasNaNs() const { return isNaN(x) || isNaN(y); }

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
    explicit operator Vector3<U>() const {
        return Vector3<U>(x, y, z);
    }

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
        return Point3<T>(x / v, y / v, z / v);
    }
    template <typename U>
    Point3<T>& operator/=(U v) {
        ASSERT(!isNaN(v) && !isZero(v));
        x /= v; y /= v; z /= v;
        return *this;
    }

    bool operator==(const Point3<T>& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=(const Point3<T>& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

    T operator[](int i) const {
        ASSERT(i >= 0 && i <= 2);
        return i == 0 ? x : i == 1 ? y : z;
    }
    T& operator[](int i) {
        ASSERT(i >= 0 && i <= 2);
        return i == 0 ? x : i == 1 ? y : z;
    }

    T x, y;
};

// Point template typedefs
typedef Point2<int> Point2i;
typedef Point2<Real> Point2f;
typedef Point3<int> Point3i;
typedef Point3<Real> Point3f;

template <typename T>
inline std::ostream operator<<(std::ostream& os, const Point2<T>& p) {
    return (os << "[" << p.x << ", " << p.y << "]");
}

template <typename T>
inline std::ostream operator<<(std::ostream& os, const Point3<T>& p) {
    return (os << "[" << p.x << ", " << p.y << ", " << p.z << "]");
}


// Geometry inline functions


} // namespace phyr
