#include <core/integrator/sampler.h>

namespace phyr {

// Sampler definitions
#define RESET_OFFSETS (array1DOffset = array2DOffset = 0)

void Sampler::startPixel(const Point2i& pt) {
    currentPixel = pt;
    currentPixelSampleIndex = 0;
    // Reset array offsets for next pixel sample
    RESET_OFFSETS;
}

bool Sampler::startNextSample() {
    RESET_OFFSETS;
    return ++currentPixelSampleIndex < samplesPerPixel;
}

bool Sampler::setSampleIndex(int64_t sampleIdx) {
    RESET_OFFSETS;
    currentPixelSampleIndex = sampleIdx;
    return currentPixelSampleIndex < samplesPerPixel;
}

void Sampler::request1DArray(int n) {
    samples1DArraySizes.push_back(n);
    sampleArray1D.push_back(std::vector<Real>(n * samplesPerPixel));
}

void Sampler::request2DArray(int n) {
    samples2DArraySizes.push_back(n);
    sampleArray2D.push_back(std::vector<Point2f>(n * samplesPerPixel));
}

const Real* Sampler::get1DArray(int n) {
    if (array1DOffset == sampleArray1D.size())
        return nullptr;
    return &sampleArray1D[array1DOffset++][n * currentPixelSampleIndex];
}

const Point2f* Sampler::get2DArray(int n) {
    if (array2DOffset == sampleArray2D.size())
        return nullptr;
    return &sampleArray2D[array2DOffset++][n * currentPixelSampleIndex];
}

CameraSample Sampler::getCameraSample(const Point2i& pRaster) {
    CameraSample cs;

    // Order matters, do not modify
    cs.pFilm = Point2f(pRaster) + getNextSample2D();
    cs.pLens = getNextSample2D();
    return cs;
}

#undef RESET_OFFSETS

#define RESET_OFFSETS (current1DDimension = current2DDimension = 0)

// PixelSampler definitions
PixelSampler::PixelSampler(int64_t samplesPerPixel, int nSampledDimensions)
    : Sampler(samplesPerPixel),
      current1DDimension(0), current2DDimension(0) {
    for (int i = 0; i < nSampledDimensions; i++) {
        samples1D.push_back(std::vector<Real>(samplesPerPixel));
        samples2D.push_back(std::vector<Point2f>(samplesPerPixel));
    }
}

bool PixelSampler::startNextSample() {
    RESET_OFFSETS;
    return Sampler::startNextSample();
}

bool PixelSampler::setSampleIndex(int64_t sampleIdx) {
    RESET_OFFSETS;
    return Sampler::setSampleIndex(sampleIdx);
}

Real PixelSampler::getNextSample1D() {
    if (current1DDimension < samples1D.size())
        return samples1D[current1DDimension++][currentPixelSampleIndex];
    else
        return rng.uniformReal();
}

Point2f PixelSampler::getNextSample2D() {
    if (current2DDimension < samples2D.size())
        return samples2D[current2DDimension++][currentPixelSampleIndex];
    else
        return Point2f(rng.uniformReal(), rng.uniformReal());
}

#undef RESET_OFFSETS

}  // namespace phyr
