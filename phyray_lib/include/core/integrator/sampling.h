#ifndef PHYRAY_CORE_SAMPLING_H
#define PHYRAY_CORE_SAMPLING_H

#include <core/phyr.h>
#include <core/geometry/geometry.h>

namespace phyr {

// Sampling a unit disk
Point2f uniformSampleDisk(const Point2f& pt);
Point2f concentricSampleDisk(const Point2f& pt);

}  // namespace phyr

#endif
