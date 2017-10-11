#ifndef PHYRAY_CORE_TRANSFORM_H
#define PHYRAY_CORE_TRANSFORM_H

#include <core/phyr.h>
#include <core/phyr_math.h>
#include <core/geometry/geometry.h>

namespace phyr {

// Transform class declarations
class Transform {
  public:
    // Constructors
    Transform() {}
    Transform(const Real tMat[4][4]) {
        mat = Mat4x4(tMat[0][0], tMat[0][1], tMat[0][2], tMat[0][3],
                     tMat[1][0], tMat[1][1], tMat[1][2], tMat[1][3],
                     tMat[2][0], tMat[2][1], tMat[2][2], tMat[2][3],
                     tMat[3][0], tMat[3][1], tMat[3][2], tMat[3][3]);
        invMat = Mat4x4::inverse(mat);
    }
    Transform(const Mat4x4& mat) : mat(mat), invMat(Mat4x4::inverse(mat)) {}
    Transform(const Mat4x4& mat, const Mat4x4& invMat) : mat(mat), invMat(invMat) {}

    // Getters
    const Mat4x4& getMatrix() const { return mat; }
    const Mat4x4& getInverseMatrix() const { return invMat; }

    // Other public functions
    inline bool operator==(const Transform& t) const {
        return mat == t.mat && invMat == t.invMat;
    }
    inline bool operator!=(const Transform& t) const {
        return mat != t.mat && invMat != t.invMat;
    }
    bool isIdentity() const { return mat == Mat4x4(); }

    /**
     * Returns true if this transform changes the
     * destination coordinate system.
     *
     * This is only true if the determinant of the upper left
     * 3x3 matrix is negative
     */
    bool changesCoordinateSystem() const {
        Real det = mat.d[0][0] * (mat.d[1][1] * mat.d[2][2] - mat.d[1][2] * mat.d[2][1]) -
                   mat.d[0][1] * (mat.d[1][0] * mat.d[2][2] - mat.d[1][2] * mat.d[2][0]) +
                   mat.d[0][2] * (mat.d[1][0] * mat.d[2][1] - mat.d[1][1] * mat.d[2][0]);
        return det < 0;
    }
    inline Transform operator*(const Transform& t) const;

    // Transforms are applied on geometric primitives through
    // the overloaded function operator for each primitive type

    /**
     * @todo Reimplement with proper error bound checks
     */
    template <typename T>
    inline Point3<T> operator()(const Point3<T>& p) const {
        Real npx = mat.d[0][0] * p.x + mat.d[0][1] * p.y + mat.d[0][2] * p.z + mat.d[0][3];
        Real npy = mat.d[1][0] * p.x + mat.d[1][1] * p.y + mat.d[1][2] * p.z + mat.d[1][3];
        Real npz = mat.d[2][0] * p.x + mat.d[2][1] * p.y + mat.d[2][2] * p.z + mat.d[2][3];
        Real npw = mat.d[3][0] * p.x + mat.d[3][1] * p.y + mat.d[3][2] * p.z + mat.d[3][3];

        if (npw == 1) return Point3<T>(npx, npy, npz);
        else return Point3<T>(npx, npy, npz) / npw;
    }

    /**
     * @todo Reimplement with proper error bound checks
     */
    template <typename T>
    inline Vector3<T> operator()(const Vector3<T>& v) const {
        return Vector3<T>(
            mat.d[0][0] * v.x + mat.d[0][1] * v.y + mat.d[0][2] * v.z,
            mat.d[1][0] * v.x + mat.d[1][1] * v.y + mat.d[1][2] * v.z,
            mat.d[2][0] * v.x + mat.d[2][1] * v.y + mat.d[2][2] * v.z
        );
    }

    /**
     * Applying transforms on normals require special treatment.
     * Normals should maintain the invariant that they're always perpendicular
     * to the shape surface. This invariant can be defined by dot(N,T) = 0,
     * where N is the normal and T the tangent at a point on the surface.
     *
     * A surface point transformed by M also transforms T by M.
     * So, we have T' = MT. Let N be transformed by an arbitrary transformation, say S.
     * Then, N' = SN and dot(N',T') = 0; dot(SN, MT) = 0;
     * transpose(SN) * MT = 0; transpose(N) * transpose(S) * MT = 0;
     * Since, transpose(N) * T = 0, it follows that, transpose(S) * M = 1;
     * transpose(S) = inverse(M); S = transpose(inverse(M))
     *
     * Thus a normal N should be multiplied by the transpose of the inverse of the
     * transformation matrix.
     */
    template <typename T>
    inline Normal3<T> operator()(const Normal3<T>& n) const {
        // Inverse is already maintained by the Transform class
        return Normal3<T>(invMat.d[0][0] * n.x + invMat.d[1][0] * n.y + invMat.d[2][0] * n.z,
                          invMat.d[0][1] * n.x + invMat.d[1][1] * n.y + invMat.d[2][1] * n.z,
                          invMat.d[0][2] * n.x + invMat.d[1][2] * n.y + invMat.d[2][2] * n.z);
    }

    /**
     * @todo Reimplement with proper error bound checks
     */
    inline Ray operator()(const Ray& r) const {
        Point3f o = (*this)(r.o);  // Transform origin
        Vector3f d = (*this)(r.d); // Transform direction
        return Ray(o, d, r.tMax, r.time);
    }

    inline Bounds3f operator()(const Bounds3f& b) const {
        // Transform the bounds points
        Point3f p1 = (*this)(b.pMin), p2 = (*this)(b.pMax);
        // The Bounds constructor will create the necessary limits
        return Bounds3f(p1, p2);
    }

    SurfaceInteraction operator()(const SurfaceInteraction& si) const;

    // Static functions
    static Transform inverse(const Transform& t) {
        return Transform(t.invMat, t.mat);
    }
    static Transform transpose(const Transform& t) {
        return Transform(Mat4x4::transpose(t.mat), Mat4x4::transpose(t.invMat));
    }

    // Factory methods
    static Transform translate(const Vector3f& v);
    static Transform scale(Real sx, Real sy, Real sz);

    // Axis aligned rotation
    /**
     * Rotate by {theta} degrees around the x-axis
     */
    static Transform rotateX(Real theta);

    /**
     * Rotate by {theta} degrees around the y-axis
     */
    static Transform rotateY(Real theta);

    /**
     * Rotate by {theta} degrees around the z-axis
     */
    static Transform rotateZ(Real theta);

    /**
     * Rotation by theta angles around the axis given by {axis}.
     * {axis} is assumed to be normalized
     */
    static Transform rotate(const Vector3f& axis, Real theta);

    /**
     * Calculates a transformation from LHS world space to camera space.
     * The calculated camera space is such that the camera position is at the origin,
     * and it's looking forward in the +Z direction, and the +Y axis is along
     * the up direction.
     *
     * @param pos       The camera origin in world space
     * @param lookPos   The world space position being looked at from the camera
     * @param up        The world space up direction of the camera
     * @returns         The Transform object that maps LHS world space coordinates to
     *                  camera space coordinates
     */
    static Transform lookAt(const Point3f& loc, const Point3f& targetLoc, const Vector3f& up);

  private:
    Mat4x4 mat, invMat;
};

} // namespace phyr

#endif
