#include <iomanip>
#include <iostream>

#include <core/phyr.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int main(int argc, const char* argv[]) {
    Real fp = 0.5;
    for (int i = 0; i < 40; i++) {
        std::cout << std::fixed << std::setprecision(54) << fp << std::endl;
        fp = phyr::nextFloatDown(fp);
    }

    return phyr::epsEqual(fp, 0.5f) ? 1 : 0;
}

#pragma GCC diagnostic pop
