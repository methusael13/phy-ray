#ifndef PHYR_CORE_MEMORY_H
#define PHYR_CORE_MEMORY_H

#include <core/phyr.h>

#include <list>
#include <alloca.h>

namespace phyr {

size_t getL1CacheLineSize();

// Use a default cache line size of 64 bytes
#define DEF_PHYR_L1_CACHE_LINESZ 64

#if defined(PHYR_L1_CACHE_LINESZ) && PHYR_L1_CACHE_LINESZ > 0
static constexpr int PhyRayL1CacheLineSize = PHYR_L1_CACHE_LINESZ;
#else
static const int PhyRayL1CacheLineSize = getL1CacheLineSize();
#endif

#define STACK_ALLOC(T, CNT) ((T*)(alloca(CNT * sizeof(T))))

// Macro for memory allocation with the new placement op and MemoryPool
#define POOL_ALLOC(pool, Type) new ((pool).alloc(sizeof(Type))) Type

void* allocAligned(size_t size);

template <typename T>
T* allocAligned(size_t count);

void freeAligned(void* ptr);

// Default block size of 256 KB
#define DEF_MEMPOOL_BLK_SZ (256 * 1024)

/**
 * L1 cache line aligned memory pool class.
 * This class is likely to be used for several memory allocation tasks
 * by multiple entities before being destroyed. The pool reuses any memory
 * block that has been once allocated by it for future allocation requests.
 */
class alignas(DEF_PHYR_L1_CACHE_LINESZ) MemoryPool {
    public:
    MemoryPool(const size_t blockSize = DEF_MEMPOOL_BLK_SZ) :
        blockSize(blockSize), totalAllocSize(0), curBlockOffset(0), curAllocSize(0) {}

    ~MemoryPool() {
        if (blockPtr) freeAligned(blockPtr);
        for (auto it : freeBlocks) freeAligned(it.first);
        for (auto it : usedBlocks) freeAligned(it.first);
    }

    /**
     * Returns total current allocated size of the pool in bytes
     */
    size_t size() const { return totalAllocSize; }

    void* alloc(size_t byteCount);

    // Templated alloc version for array allocations
    template <typename T>
    T* alloc(size_t n = 1, bool constructItems = false) {
        T* ptr =  static_cast<T*>(alloc(n * sizeof(T)));
        if (constructItems) {
            for (size_t i = 0; i < n; i++) {
                // Use placement new operator for using {ptr} as the storage buffer
                new (&ptr[i]) T();
            }
        }
        return ptr;
    }

    void reset() {
        // Don't stash, current block can be reused
        curBlockOffset = 0;
        // Move all used block data to free blocks set
        freeBlocks.splice(freeBlocks.begin(), usedBlocks);
    }

  private:
    // Prevent class copy
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    /**
     * Store current block usage data in the used block set and reset
     */
    void stash() {
        if (blockPtr) {
            usedBlocks.push_back(std::make_pair(blockPtr, curAllocSize));
            blockPtr = nullptr; curAllocSize = 0;
        }
    }

    // Minimum default size of memory blocks
    const size_t blockSize;
    // Total allocation size in bytes for the memory pool
    size_t totalAllocSize;
    // Current block offset and block allocation size
    size_t curBlockOffset, curAllocSize;
    // Pointer to the current block in memory
    uint8_t* blockPtr = nullptr;

    std::list<std::pair<uint8_t*, size_t>> freeBlocks, usedBlocks;
};

#undef DEF_MEMPOOL_BLK_SZ

}  // namespace phyr

#endif
