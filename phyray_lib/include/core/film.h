#ifndef PHYRAY_CORE_FILM_H
#define PHYRAY_CORE_FILM_H

#include <core/phyr.h>
#include <core/concurrency.h>
#include <core/color/spectrum.h>
#include <core/integrator/filter.h>
#include <core/geometry/geometry.h>

#include <memory>

namespace phyr {

struct FilmTilePixel {
    Spectrum contributionSum = Real(0);
    Real filterWeightSum = Real(0);
};

// FilmTile declarations
class FilmTile {
  public:
    FilmTile(const Bounds2i& pixelBounds, const Vector2f& filterRadius,
             const Real* filterTable, int filterTableSize) :
        pixelBounds(pixelBounds), filterRadius(filterRadius),
        invFilterRadius(Real(1) / filterRadius.x, Real(1) / filterRadius.y),
        filterTable(filterTable), filterTableSize(filterTableSize) {
        // Allocate pixels
        pixels = std::vector<FilmTilePixel>(std::max(0, pixelBounds.area()));
    }

  private:
    const Bounds2i pixelBounds;
    const Vector2f filterRadius, invFilterRadius;
    const Real* filterTable;
    const int filterTableSize;

    std::vector<FilmTilePixel> pixels;
};

// Film declarations
class Film {
  public:
    // Film size must be specified in millimetres
    Film(const Point2i& resolution, const Bounds2f& cropWindow,
         std::unique_ptr<Filter> filter, Real filmSize,
         const std::string& filename, Real scale);

    // Interface
    Bounds2i getSampleBounds() const;
    // Returns the actual extent of the film in the scene
    Bounds2f getPhysicalExtent() const;

    // Returns a pointer to a FilmTile given the sample bounds
    std::unique_ptr<FilmTile> getFilmTile(const Bounds2i& sampleBounds);

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
        // Represents running weighted
        // sums of spectral pixel contributions
        Real xyz[3] = { 0, 0, 0 };
        // Stores the sum of filter weight
        // values for sample contributions to pixel
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
