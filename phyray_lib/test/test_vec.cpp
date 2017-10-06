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
    
    bool valid1 = phyr::epsEqual(dot(v1, v2), 0.);
    bool valid2 = phyr::epsEqual(dot(v2, v3), 0.);
    bool valid3 = phyr::epsEqual(dot(v1, v3), 0.);
    bool valid = (valid1 && valid2 && valid3);

    std::cout << "Result: " << valid << std::endl;
    return valid ? 0 : 1;
}

#pragma GCC diagnostic pop
