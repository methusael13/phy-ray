#include <core/film.h>
#include <core/phyr_mem.h>

#include <core/debug.h>

namespace phyr {

// FilmTile definitions
void FilmTile::addSample(const Point2f& pFilm, const Spectrum& spec, Real sampleWeight) {
    // Compute sample's raster bounds
    // Convert continuous pixel coordinates to discrete
    Point2f pFilmDiscrete = pFilm - Vector2f(0.5, 0.5);
    // Calculate pixel bounds
    Point2i p0 = Point2i(ceil(pFilmDiscrete - filterRadius));
    Point2i p1 = Point2i(floor(pFilmDiscrete + filterRadius)) + Point2i(1, 1);
    p0 = max(p0, pixelBounds.pMin);
    p1 = min(p1, pixelBounds.pMax);

    // Loop over filter support and add sample to pixel arrays
    // Precompute x and y filter table offsets
    int* ftx = STACK_ALLOC(int, p1.x - p0.x);
    for (int x = p0.x; x < p1.x; x++) {
        Real fx = std::abs((x - pFilmDiscrete.x) * invFilterRadius.x * filterTableSize);
        ftx[x - p0.x] = std::min(int(std::floor(fx)), filterTableSize - 1);
    }

    int* fty = STACK_ALLOC(int, p1.y - p0.y);
    for (int y = p0.y; y < p1.y; y++) {
        Real fy = std::abs((y - pFilmDiscrete.y) * invFilterRadius.y * filterTableSize);
        fty[y - p0.y] = std::min(int(std::floor(fy)), filterTableSize - 1);
    }

    for (int y = p0.y; y < p1.y; y++) {
        for (int x = p0.x; x < p1.x; x++) {
            // Evaluate filter value at (x, y) pixel
            int idx = fty[y - p0.y] * filterTableSize + ftx[x - p0.x];
            Real filterWeight = filterTable[idx];

            // Update filter values with filtered sample contribution
            FilmTilePixel& pixel = getPixel(Point2i(x, y));
            pixel.contributionSum += spec * sampleWeight * filterWeight;
            pixel.filterWeightSum += filterWeight;
        }
    }
}


// Film definitions
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
    // Convert from continuous to discrete pixel coordinates
    Point2i p0 = Point2i(ceil(bounds.pMin - halfPixel - filter->radius));
    Point2i p1 = Point2i(floor(bounds.pMax - halfPixel + filter->radius)) + Point2i(1, 1);

    // Tile bounds
    Bounds2i tilePixelBounds(intersect(Bounds2i(p0, p1), croppedImageBounds));
    // Return pointer to generated FilmTile
    return std::unique_ptr<FilmTile>(new FilmTile(tilePixelBounds, filter->radius,
                                                  filterTable, filterTableSize));
}

void Film::mergeFilmTile(std::unique_ptr<FilmTile> tile) {
    // Acquire lock
    std::lock_guard<std::mutex> lock(mutex);
    // Iterate through all pixels within pixel bounds
    for (Point2i pixel : tile->getPixelBounds()) {
        // Merge pixel into {Film::pixels}
        FilmTilePixel& tilePixel = tile->getPixel(pixel);
        Pixel& filmPixel = getPixel(pixel);

        // Get xyz spectrum data
        Real xyz[3];
        tilePixel.contributionSum.toXYZConstants(xyz);
        // Add spectrum data to {filmPixel}
        filmPixel.xyz[0] += xyz[0]; filmPixel.xyz[1] += xyz[1];
        filmPixel.xyz[2] += xyz[2];
        filmPixel.filterWeightSum += tilePixel.filterWeightSum;
    }
}

void Film::setImage(const Spectrum* img) {
    int nPixels = croppedImageBounds.area();
    for (int i = 0; i < nPixels; i++) {
        Pixel& pixel = pixels[i];
        img[i].toXYZConstants(pixel.xyz);
        pixel.filterWeightSum = 1;
        pixel.splatXYZ[0] = pixel.splatXYZ[1] = pixel.splatXYZ[2] = 0;
    }
}

void Film::addSplat(const Point2f& pt, const Spectrum& spec) {
#ifndef PHYRAY_OPTIMIZE
    // Ignore invalid spectrum data
    if (spec.hasNaNs()) {
        LOG_WARNING_FMT("Ignoring splat at (%f, %f) due to NaN value", pt.x, pt.y);
        return;
    }

    if (spec.getYConstant() < 0) {
        LOG_WARNING_FMT("Ignoring splat at (%f, %f) with negative luminance", pt.x, pt.y);
        return;
    }

    if (std::isinf(spec.getYConstant())) {
        LOG_WARNING_FMT("Ignoring splat at (%f, %f) with infinite luminance", pt.x, pt.y);
        return;
    }
#endif

    Point2i p(pt);
    if (!insideExclusive(p, croppedImageBounds)) return;

    // Get xyz contributions
    Real xyz[3];
    spec.toXYZConstants(xyz);
    // Add as splat to pixel
    Pixel& pixel = getPixel(p);
    pixel.splatXYZ[0].add(xyz[0]); pixel.splatXYZ[1].add(xyz[1]);
    pixel.splatXYZ[2].add(xyz[2]);
}

void Film::writeImage(Real splatScale) {
    // Allocate space for RGB image data
    std::unique_ptr<Real[]> rgb(new Real[3 * croppedImageBounds.area()]);
    int idx = 0;

    // Convert image to RGB and calculate final pixel values
    for (Point2i p : croppedImageBounds) {
        // Convert XYZ pixel data to RGB
        Pixel& pixel = getPixel(p);
        convertXYZToRGB(pixel.xyz, &rgb[3 * idx]);

        // Normalize pixel with filter weight sum
        Real filterWeightSum = pixel.filterWeightSum;
        if (filterWeightSum != 0) {
            Real invFilterWeightSum = Real(1) / filterWeightSum;
            // Clamp minimum rgb value to 0
            rgb[3 * idx    ] = std::max(Real(0), rgb[3 * idx    ] * invFilterWeightSum);
            rgb[3 * idx + 1] = std::max(Real(0), rgb[3 * idx + 1] * invFilterWeightSum);
            rgb[3 * idx + 2] = std::max(Real(0), rgb[3 * idx + 2] * invFilterWeightSum);
        }

        // Add splat value to pixel
        Real splatRGB[3];
        Real splatXYZ[3] = { pixel.splatXYZ[0], pixel.splatXYZ[1], pixel.splatXYZ[2] };
        convertXYZToRGB(splatXYZ, splatRGB);

        rgb[3 * idx    ] = scale * (rgb[3 * idx    ] + splatScale * splatRGB[0]);
        rgb[3 * idx + 1] = scale * (rgb[3 * idx + 1] + splatScale * splatRGB[1]);
        rgb[3 * idx + 2] = scale * (rgb[3 * idx + 2] + splatScale * splatRGB[2]);

        // Increment to next pixel index
        idx++;
    }

    // TODO:
    // Write rgb image data to file. Apply gamma correction according
    // to the sRGB standard if writing to an 8-bit integer format.
}

}  // namespace phyr
