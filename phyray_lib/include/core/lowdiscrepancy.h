#ifndef PHYRAY_CORE_LOWDISCREPANCY_H
#define PHYRAY_CORE_LOWDISCREPANCY_H

#include <core/phyr.h>
#include <core/rng.h>
#include <core/integrator/sampling.h>

/*
 * Code adapted from pbrt-v3 for use with PhyRay
 */

namespace phyr {

// Low Discrepancy Declarations
Real radicalInverse(int baseIndex, uint64_t a);
std::vector<uint16_t> computeRadicalInversePermutations(RNG& rng);

static constexpr int PrimeTableSize = 1000;
extern const int Primes[PrimeTableSize];

Real scrambledRadicalInverse(int baseIndex, uint64_t a, const uint16_t* perm);
extern const int PrimeSums[PrimeTableSize];

extern uint32_t CMaxMinDist[17][32];

// Low Discrepancy Inline Functions
inline uint32_t reverseBits32(uint32_t n) {
    n = (n << 16) | (n >> 16);
    n = ((n & 0x00ff00ff) << 8) | ((n & 0xff00ff00) >> 8);
    n = ((n & 0x0f0f0f0f) << 4) | ((n & 0xf0f0f0f0) >> 4);
    n = ((n & 0x33333333) << 2) | ((n & 0xcccccccc) >> 2);
    n = ((n & 0x55555555) << 1) | ((n & 0xaaaaaaaa) >> 1);
    return n;
}

inline uint64_t reverseBits64(uint64_t n) {
    uint64_t n0 = reverseBits32((uint32_t)n);
    uint64_t n1 = reverseBits32((uint32_t)(n >> 32));
    return (n0 << 32) | n1;
}

template <int base>
inline uint64_t inverseRadicalInverse(uint64_t inverse, int nDigits) {
    uint64_t index = 0;
    for (int i = 0; i < nDigits; ++i) {
        uint64_t digit = inverse % base;
        inverse /= base;
        index = index * base + digit;
    }
    return index;
}

}  // namespace phyr

#endif
