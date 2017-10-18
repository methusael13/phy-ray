#include <core/geometry/geometry.h>

namespace phyr {

// Miscellaneous operations
template <typename T>
T minComponent(const Vector3<T>& v) { return std::min(v.x, std::min(v.y, v.z)); }
template <typename T>
T maxComponent(const Vector3<T>& v) { return std::max(v.x, std::max(v.y, v.z)); }

template <typename T>
int maxDimension(const Vector3<T>& v) {
    return (v.x > v.y) ? (v.x > v.z ? 0 : 2) : (v.y > v.z ? 1 : 2);
}

template <typename T>
Vector3<T> permute(const Vector3<T>& v, int i, int j, int k) {
    return Vector3<T>(v[i], v[j], v[k]);
}
template <typename T>
Point3<T> permute(const Point3<T>& v, int i, int j, int k) {
    return Point3<T>(v[i], v[j], v[k]);
}

template <typename T>
Point2<T> lerp(Real f, const Point2<T>& p1, const Point2<T>& p2) {
    return (1 - f) * p1 + f * p2;
}
template <typename T>
Point3<T> lerp(Real f, const Point3<T>& p1, const Point3<T>& p2) {
    return (1 - f) * p1 + f * p2;
}

template <typename T>
Point2<T> floor(const Point2<T>& p) {
    return Point2<T>(std::floor(p.x), std::floor(p.y));
}
template <typename T>
Point3<T> floor(const Point3<T>& p) {
    return Point3<T>(std::floor(p.x), std::floor(p.y), std::floor(p.z));
}

template <typename T>
Point2<T> ceil(const Point2<T>& p) {
    return Point2<T>(std::ceil(p.x), std::ceil(p.y));
}
template <typename T>
Point3<T> ceil(const Point3<T>& p) {
    return Point3<T>(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z));
}

// Min, Max
template <typename T>
Vector3<T> min(const Vector3<T>& v1, const Vector3<T>& v2) {
    return Vector3<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y),
                      std::min(v1.z, v2.z));
}
template <typename T>
Point2<T> min(const Point2<T>& p1, const Point2<T>& p2) {
    return Point2<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
}
template <typename T>
Point3<T> min(const Point3<T>& p1, const Point3<T>& p2) {
    return Point3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
                     std::min(p1.z, p2.z));
}
template <typename T>
Vector3<T> max(const Vector3<T>& v1, const Vector3<T>& v2) {
    return Vector3<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y),
                      std::max(v1.z, v2.z));
}
template <typename T>
Point2<T> max(const Point2<T>& p1, const Point2<T>& p2) {
    return Point2<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
}
template <typename T>
Point3<T> max(const Point3<T>& p1, const Point3<T>& p2) {
    return Point3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
                     std::max(p1.z, p2.z));
}


// Bounds union operations
template <typename T>
Bounds2<T> unionBounds(const Bounds2<T>& b, const Point2<T>& p) {
    return Bounds2<T>(
                Point2<T>(std::min(b.pMin.x, p.x), std::min(b.pMin.y, p.y)),
                Point2<T>(std::max(b.pMax.x, p.x), std::max(b.pMax.y, p.y)));
}
template <typename T>
Bounds2<T> unionBounds(const Bounds2<T>& b1, const Bounds2<T>& b2) {
    return Bounds2<T>(
                Point2<T>(std::min(b1.pMin.x, b2.pMin.x), std::min(b1.pMin.y, b2.pMin.y)),
                Point2<T>(std::max(b1.pMax.x, b2.pMax.x), std::max(b1.pMax.y, b2.pMax.y)));
}
template <typename T>
Bounds3<T> unionBounds(const Bounds3<T>& b, const Point3<T>& p) {
    return Bounds3<T>(
                Point3<T>(std::min(b.pMin.x, p.x), std::min(b.pMin.y, p.y),
                          std::min(b.pMin.z, p.z)),
                Point3<T>(std::max(b.pMax.x, p.x), std::max(b.pMax.y, p.y),
                          std::max(b.pMax.z, p.z)));
}
template <typename T>
Bounds3<T> unionBounds(const Bounds3<T>& b1, const Bounds3<T>& b2) {
    return Bounds3<T>(
                Point3<T>(std::min(b1.pMin.x, b2.pMin.x), std::min(b1.pMin.y, b2.pMin.y),
                          std::min(b1.pMin.z, b2.pMin.z)),
                Point3<T>(std::max(b1.pMax.x, b2.pMax.x), std::max(b1.pMax.y, b2.pMax.y),
                          std::max(b1.pMax.z, b2.pMax.z)));
}

} // namespace phyr
