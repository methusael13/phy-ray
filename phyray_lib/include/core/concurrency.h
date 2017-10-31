#ifndef PHYRAY_CORE_CONCURRENCY_H
#define PHYRAY_CORE_CONCURRENCY_H

#include <core/phyr.h>

#include <atomic>

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

}  // namespace phyr

#endif
