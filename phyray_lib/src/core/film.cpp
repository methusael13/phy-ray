#include <core/film.h>

namespace phyr {

Film::Film(const Point2i& resolution, const Bounds2f& cropWindow,
           std::unique_ptr<Filter> filter, Real filmSize,
           const std::string& filename, Real scale) :
    resolution(resolution), filmSize(filmSize * .001), filter(std::move(filter)),
    filename(filename), scale(scale) {
    // Compute film image bounds
    croppedImageBounds = Bounds2i(Point2i(std::ceil(resolution.x * cropWindow.pMin.x),
                                          std::ceil(resolution.y * cropWindow.pMin.y)),
                                  Point2i(std::ceil(resolution.x * cropWindow.pMax.x),
                                          std::ceil(resolution.y * cropWindow.pMax.y)));

    // Allocate memory for image pixels
    pixels = std::unique_ptr<Pixel[]>(new Pixel[croppedImageBounds.area()]);

    // Precompute filter weight table
    Real invFilterTableSize = Real(1) / filterTableSize;
    int offset = 0;

    for (int y = 0; y < filterTableSize; y++) {
        for (int x = 0; x < filterTableSize; x++) {
            Point2f p;
            p.x = (x + 0.5) * filter->radius.x * invFilterTableSize;
            p.y = (y + 0.5) * filter->radius.y * invFilterTableSize;
            filterTable[offset++] = filter->evaluate(p);
        }
    }
}

Bounds2i Film::getSampleBounds() const {
    // Convert from discrete to continuous pixel coordinates
    // accounting for half-pixel offsets, expanding by the filter radius
    // and then rounding outward.
    const Vector2f halfPixel(0.5, 0.5);
    Bounds2f bounds(
        floor(Point2f(croppedImageBounds.pMin) + halfPixel - filter->radius),
        ceil(Point2f(croppedImageBounds.pMax) - halfPixel + filter->radius));
    return Bounds2i(bounds);
}

Bounds2f Film::getPhysicalExtent() const {
    // Physcial dimensions of the film is calculated in metres from
    // given film diagonal size and aspect ratio of the final output image.
    Real aspectRatio = Real(resolution.x) / Real(resolution.y);
    Real y = std::sqrt((filmSize * filmSize) / (1 + aspectRatio * aspectRatio));
    Real x = y * aspectRatio;
    return Bounds2f(Point2f(-x * 0.5, -y * 0.5), Point2f(x * 0.5, y * 0.5));
}

std::unique_ptr<FilmTile> Film::getFilmTile(const Bounds2i& sampleBounds) {
    // Bound image pixels that samples in sampleBounds contribute to
    const Vector2f halfPixel(0.5, 0.5);
    Bounds2f bounds = Bounds2f(sampleBounds);
    Point2i p0 = Point2i(ceil(bounds.pMin - halfPixel - filter->radius));
    Point2i p1 = Point2i(floor(bounds.pMax - halfPixel + filter->radius)) + Point2i(1, 1);

    // Tile bounds
    Bounds2i tilePixelBounds(intersect(Bounds2i(p0, p1), croppedImageBounds));
    // Return pointer to generated FilmTile
    return std::unique_ptr<FilmTile>(new FilmTile(tilePixelBounds, filter->radius,
                                                  filterTable, filterTableSize));
}

}  // namespace phyr
