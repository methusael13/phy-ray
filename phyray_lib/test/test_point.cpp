#include <iostream>

#include <core/phyr.h>
#include <core/phyr_mem.h>
#include <core/geometry/geometry.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

using namespace phyr;

int main(int argc, const char* argv[]) {
    std::cout << "Testing PhyRay Point3f..." << std::endl;

    Point3f* p1 = allocAligned<Point3f>(1);
    Point3f p2(5, 6, 7);
    p1->x = 1; p1->y = 2; p1->z = 3;

    *p1 = p2;

    ASSERT(!p1->hasNaNs());

    bool a = p1->hasNaNs();
    std::cout << a;

    bool valid = *p1 == p2;

    freeAligned(p1);

    return valid ? 0 : 1;
}

#pragma GCC diagnostic pop
