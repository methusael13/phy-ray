#ifndef PHYRAY_CORE_CONCURRENCY_H
#define PHYRAY_CORE_CONCURRENCY_H

#include <core/phyr.h>
#include <core/geometry/geometry.h>

#include <mutex>
#include <atomic>
#include <functional>
#include <condition_variable>

namespace phyr {

class AtomicReal {
  public:
    explicit AtomicReal(Real value = 0) : bits(floatToBits(value)) {}

    // Interface
    operator Real() const { return bitsToFloat(bits); }
    Real operator=(Real value) { bits = floatToBits(value); return value; }

    void add(Real value) {
#ifdef PHYRAY_USE_LONG_P
        uint64_t expected = bits, desired;
#else
        uint32_t expected = bits, desired;
#endif
        do {
            desired = floatToBits(bitsToFloat(bits) + value);
        } while (!bits.compare_exchange_weak(expected, desired));
    }

  private:
#ifdef PHYRAY_USE_LONG_P
    std::atomic<uint64_t> bits;
#else
    std::atomic<uint32_t> bits;
#endif
};

// Simple one-use barrier; ensures that multiple threads all reach a
// particular point of execution before allowing any of them to proceed
// past it.
//
// Note: this should be heap allocated and managed with a shared_ptr, where
// all threads that use it are passed the shared_ptr. This ensures that
// memory for the Barrier won't be freed until all threads have
// successfully cleared it.
class Barrier {
  public:
    Barrier(int count) : count(count) { ASSERT(count > 0); }
    ~Barrier() { ASSERT(count == 0); }

    // Interface
    void wait();

  private:
    std::mutex mutex;
    std::condition_variable cv;
    int count;
};

void ParallelFor(std::function<void(int64_t)> func, int64_t count,
                 int chunkSize = 1);
extern thread_local int ThreadIndex;

void ParallelFor2D(std::function<void(Point2i)> func, const Point2i &count);

int maxThreadIndex();
int numSystemCores();

void parallelInit();
void parallelCleanup();
void mergeWorkerThreadStats();

}  // namespace phyr

#endif
