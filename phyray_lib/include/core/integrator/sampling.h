#ifndef PHYRAY_CORE_SAMPLING_H
#define PHYRAY_CORE_SAMPLING_H

#include <core/rng.h>
#include <core/phyr.h>
#include <core/geometry/geometry.h>

namespace phyr {

// Sampling a unit disk
Point2f uniformSampleDisk(const Point2f& pt);
Point2f concentricSampleDisk(const Point2f& pt);

template <typename T>
void shuffle(T* sample, int nSamples, int nDimensions, RNG& rng);

// Stratified samples
void stratifiedSample1D(Real* sample, int nSamples, bool jitter, RNG& rng);
void stratifiedSample2D(Point2f* sample, int nxSamples, int nySamples, bool jitter, RNG& rng);

void latinHypercube(Real* sample, int nSamples, int nDimensions, RNG& rng);

}  // namespace phyr

#endif
