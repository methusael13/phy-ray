#include <core/phyr_mem.h>

#include <fstream>
#include <cstdlib>

namespace phyr {

size_t getL1CacheLineSize() {
    std::fstream fs;
    // Set fstream to throw an exception in case of a failure
    fs.exceptions(fs.exceptions() | std::ios::failbit);

    // Data path for L1 cache (Unix systems)
    const char* cacheDataPath =
            "/sys/devices/system/cpu/cpu0/cache/index1/coherency_line_size";

    size_t l1sz = DEF_PHYR_L1_CACHE_LINESZ;
    try {
        fs.open(cacheDataPath, std::fstream::in);
        fs >> l1sz;
    } catch (std::ios::failure&) {}

    return l1sz;
}

/**
 * Allocate {size} bytes of memory aligned to L1 cache lines.
 * @note Non portable code, use with care
 */
void* allocAligned(size_t size) {
    void* ptr;
    if (posix_memalign(&ptr, PhyRayL1CacheLineSize, size) != 0) ptr = nullptr;
    return ptr;
}

template <typename T>
T* allocAligned(size_t count) {
    return static_cast<T*>(allocAligned(count * sizeof(T)));
}

void freeAligned(void* ptr) { free(ptr); }

}  // namespace phyr
