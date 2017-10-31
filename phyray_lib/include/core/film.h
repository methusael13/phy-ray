#ifndef PHYRAY_CORE_FILM_H
#define PHYRAY_CORE_FILM_H

#include <core/phyr.h>
#include <core/concurrency.h>
#include <core/integrator/filter.h>
#include <core/geometry/geometry.h>

#include <memory>

namespace phyr {

class Film {
  public:
    // Film size must be specified in millimetres
    Film(const Point2i& resolution, const Bounds2f& cropWindow,
         std::unique_ptr<Filter> filter, Real filmSize,
         const std::string& filename, Real scale);

    // Data members
    const Point2i resolution;
    // The diagonal length of the film in metres
    const Real filmSize;
    std::unique_ptr<Filter> filter;
    // Filename for the final rendered image
    const std::string filename;
    // Actual image bounds in pixels
    Bounds2i croppedImageBounds;

  private:
    struct Pixel {
        // Represents running weighted sums of spectral pixel contributions
        Real xyz[3] = { 0, 0, 0 };
        // Stores the sum of filter weight values for sample contributions to pixel
        Real filterWeightSum = 0;
        // Stores (unweighted) sum of sample splats
        AtomicReal splatXYZ[3];
        // Extra padding for 32-bit structure
        Real pad;
    };

    const Real scale;
    std::unique_ptr<Pixel[]> pixels;

    // Filter table data
    static constexpr int filterTableSize = 16;
    Real filterTable[filterTableSize * filterTableSize];
};

}  // namespace phyr

#endif
