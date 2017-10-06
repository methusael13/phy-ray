#include <iomanip>
#include <iostream>

#include <core/phyr.h>
#include <core/phyr_math.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int main(int argc, const char* argv[]) {
    phyr::Mat4x4 m = phyr::Mat4x4::zeroes();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m(i, j) = 4 * i + (j + 1);

    std::cout << "M:\n";
    std::cout << m << std::endl;

    phyr::Mat4x4 tm = phyr::Mat4x4::transpose(m);
    std::cout << "\nTransposed M:\n";
    std::cout << tm << std::endl;

    phyr::Mat4x4 prod = m * tm;
    std::cout << "\nProduct:\n";
    std::cout << prod << std::endl;

    return 0;
}

#pragma GCC diagnostic pop
