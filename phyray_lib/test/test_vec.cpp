#include <iostream>

#include <core/phyr.h>
#include <core/geometry/geometry.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int main(int argc, const char* argv[]) {
    std::cout << "Testing PhyRay Vector3f..." << std::endl;

    phyr::Vector3f v1(1, 2, 3);
    std::cout << "Type: " << sizeof(v1.x) << std::endl;
    std::cout << "v1: " << v1 << std::endl;
    v1 = phyr::normalize(v1);
    std::cout << "v1 (normalized): " << v1 << std::endl;

    phyr::Vector3f v2, v3;
    phyr::coordinateSystem(v1, &v2, &v3);
    std::cout << "v2: " << v2 << std::endl;
    std::cout << "v3: " << v3 << std::endl;
    
    bool valid1 = dot(v1, v2) < 1e-10;
    bool valid2 = dot(v2, v3) < 1e-10;
    bool valid3 = dot(v1, v3) < 1e-10;
    bool valid = (valid1 && valid2 && valid3);

    std::cout << "\nValid1: " << std::round(dot(v1, v2)) 
              << "\nValid2: " << std::round(dot(v2, v3)) 
              << "\nValid3: " << std::round(dot(v1, v3)) << std::endl;  

    std::cout << "Result: " << valid << std::endl;

    phyr::Point3f p1(1, 2, 3);
    phyr::Point3f p2(1, 2, 3);
    phyr::Vector3f v = p1 - p2;
    bool pass = v.x == 0 && v.y == 0 && v.z == 0;

    return valid ? 0 : 1;
}

#pragma GCC diagnostic pop
