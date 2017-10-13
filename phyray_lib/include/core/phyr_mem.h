#ifndef PHYR_CORE_MEMORY_H
#define PHYR_CORE_MEMORY_H

#include <core/phyr.h>

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

#define STACK_ALLOC(T, CNT) ((T*) alloca(CNT * sizeof(T)))

void* allocAligned(size_t size);

template <typename T>
T* allocAligned(size_t count);

void freeAligned(void* ptr);

}  // namespace phyr

#endif
