#include <iomanip>
#include <iostream>

#include <core/phyr.h>
#include <core/geometry/transform.h>
#include <core/geometry/interaction.h>

#include <modules/shapes/sphere.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

using namespace phyr;

int main(int argc, const char* argv[]) {
    // Translate transform
    Transform lw = Transform::translate(Vector3f(10, 10, 10));
    std::cout << "Translate transform:\n" << lw.getMatrix() << std::endl;
    Transform wl = Transform::inverse(lw);
    std::cout << "Inverse translate transform:\n" << lw.getInverseMatrix() << std::endl;

    // Sphere
    Sphere sp = Sphere(&lw, &wl, 2);

    // Test ray
    Ray ray = Ray(Point3f(0, 0, 0), Vector3f(2, 2, 2));
    std::cout << "Ray: " << ray << std::endl;
    Ray tRay = wl(ray);
    std::cout << "Transformed ray: " << tRay << std::endl;

    // Intersection parameters
    Real t0; SurfaceInteraction si;

    // Test intersection
    bool isec = sp.intersectRay(ray, &t0, &si);
    std::cout << "Intersection " << (isec ? "successful" : "failed") << std::endl;
    if (isec) {
        std::cout << "Ray t0: " << t0 << "\n";
        std::cout << "Intersection point: " << si.p << std::endl;
        std::cout << "Ray extension: " << ray(t0) << std::endl;
    }

    return isec ? 0 : 1;
}

#pragma GCC diagnostic pop
