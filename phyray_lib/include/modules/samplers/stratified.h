#ifndef PHYRAY_MODULES_STRATIFIED_H
#define PHYRAY_MODULES_STRATIFIED_H

#include <core/phyr.h>
#include <core/integrator/sampler.h>

namespace phyr {

class StratifiedSampler : public PixelSampler {
  public:
    StratifiedSampler(int xPixelSamples, int yPixelSamples,
                      bool jitterSamples, int nSampledDimensions) :
        PixelSampler(xPixelSamples * yPixelSamples, nSampledDimensions),
        xPixelSamples(xPixelSamples), yPixelSamples(yPixelSamples),
        jitterSamples(jitterSamples) {}

    void startPixel(const Point2i& pt) override;

  private:
    const int xPixelSamples, yPixelSamples;
    const bool jitterSamples;
};

}  // namespace phyr

#endif
