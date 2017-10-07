#include <iomanip>
#include <iostream>

#include <core/phyr.h>
#include <core/phyr_math.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int main(int argc, const char* argv[]) {
    phyr::Mat4x4 m(11, 10, 4, 6, 9, 8, 7, 12,
                   13, 14, 15, 16, 17, 3, 5, 18);
    std::cout << "M:\n";
    std::cout << m << std::endl;

    phyr::Mat4x4 tm = phyr::Mat4x4::inverse(m);
    std::cout << "\nInverse M:\n";
    std::cout << tm << std::endl;

    phyr::Mat4x4 prod = m * tm;
    std::cout << "\nProduct:\n";
    std::cout << std::fixed << prod << std::endl;

    return m == phyr::Mat4x4() ? 0 : 1;
}

#pragma GCC diagnostic pop
