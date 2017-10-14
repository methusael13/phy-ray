#include <iomanip>
#include <iostream>

#include <core/phyr.h>
#include <core/phyr_mem.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"

using namespace phyr;

typedef struct _TestUnit TestUnit;
struct _TestUnit {
    size_t id;
    std::string name;
};

int main(int argc, const char* argv[]) {
    MemoryPool pool;

    // Create an array of TestUnits
    TestUnit* units = pool.alloc<TestUnit>(100);
    // Pool size must be a power of 2
    bool valid = (pool.size() & (pool.size() - 1)) == 0;

    if (valid) {
        pool.reset();
        valid = (pool.size() & (pool.size() - 1)) == 0;
    }

    if (valid) {
        size_t poolCapacity = pool.size();
        TestUnit* newUnits = pool.alloc<TestUnit>(50);
        valid = poolCapacity == pool.size();
    }

    pool.reset();
    return valid ? 0 : 1;
}

#pragma GCC diagnostic pop

