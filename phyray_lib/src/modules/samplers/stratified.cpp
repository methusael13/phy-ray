#include <core/phyr.h>
#include <core/integrator/sampling.h>
#include <modules/samplers/stratified.h>

namespace phyr {

void StratifiedSampler::startPixel(const Point2i& pt) {
    // Generate single stratified samples for the pixel
    for (size_t i = 0; i < samples1D.size(); i++) {
        stratifiedSample1D(&samples1D[i][0], xPixelSamples * yPixelSamples, jitterSamples, rng);
        shuffle(&samples1D[i][0], xPixelSamples * yPixelSamples, 1, rng);
    }

    for (size_t i = 0; i < samples2D.size(); i++) {
        stratifiedSample2D(&samples2D[i][0], xPixelSamples, yPixelSamples, jitterSamples, rng);
        shuffle(&samples2D[i][0], xPixelSamples * yPixelSamples, 1, rng);
    }

    // Generate arrays of stratified samples for the pixel
    for (size_t i = 0; i < samples1DArraySizes.size(); i++) {
        for (int64_t j = 0; j < samplesPerPixel; j++) {
            int nSamples = samples1DArraySizes[i];
            stratifiedSample1D(&sampleArray1D[i][j * nSamples], nSamples, jitterSamples, rng);
            shuffle(&sampleArray1D[i][j * nSamples], nSamples, 1, rng);
        }
    }

    for (size_t i = 0; i < samples2DArraySizes.size(); i++) {
        for (int64_t j = 0; j < samplesPerPixel; j++) {
            int nSamples = samples2DArraySizes[i];
            latinHypercube(&sampleArray2D[i][j * nSamples].x, nSamples, 2, rng);
        }
    }

    // Call base implementation
    PixelSampler::startPixel(pt);
}

}  // namespace phyr
