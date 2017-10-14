#include <core/phyr_mem.h>

#include <fstream>
#include <cstdlib>

namespace phyr {

size_t getL1CacheLineSize() {
    std::fstream fs;
    // Set fstream to throw an exception in case of a failure
    fs.exceptions(fs.exceptions() | std::ios::failbit);

    // Data path for L1 cache (Unix systems)
    const char* cacheDataPath = "/sys/devices/system/cpu/cpu0"
                                "/cache/index1/coherency_line_size";

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

// MemoryPool declarations
void* MemoryPool::alloc(size_t byteCount) {
    // Align {size} with cache line size
    // Optimized alignment calculation, as {MachineAlignment} is a power of 2
    byteCount = (byteCount + MachineAlignment - 1) & ~(MachineAlignment - 1);

    // Check if size requirement is more than what is curently available
    if (curBlockOffset + byteCount > curAllocSize) {
        // Store current block usage data
        stash();

        // Check free blocks for memory
        for (auto it = freeBlocks.begin(); it != freeBlocks.end(); it++) {
            if (it->second >= byteCount) {
                blockPtr = it->first; curAllocSize = it->second;
                // Remove this block from the free set
                freeBlocks.erase(it);
                break;
            }
        }

        // If a block has not yet been found, allocate new
        if (!blockPtr) {
            // Ensure minimum of {blockSize} bytes per block
            curAllocSize = std::max(byteCount, blockSize);
            blockPtr = allocAligned<uint8_t>(curAllocSize);
            // Increment pool size stat
            totalAllocSize += curAllocSize;
        }

        // New memory block allocated, reset block offset
        curBlockOffset = 0;
    }

    void* ptr = blockPtr + curBlockOffset;
    curBlockOffset += byteCount;
    return ptr;
}

}  // namespace phyr
