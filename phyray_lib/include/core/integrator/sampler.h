#ifndef PHYRAY_CORE_SAMPLER_H
#define PHYRAY_CORE_SAMPLER_H

#include <core/rng.h>
#include <core/phyr.h>
#include <core/camera/camera.h>
#include <core/geometry/geometry.h>

#include <memory>

namespace phyr {

// Sampler declarations
class Sampler {
  public:
    Sampler(int64_t samplesPerPixel) : samplesPerPixel(samplesPerPixel) {}

    virtual ~Sampler();

    // Interface declarations
    // Starts sampling
    virtual void startPixel(const Point2i& pt);
    // Prepare for next sample. Reset internal data
    virtual bool startNextSample();

    // Acquires one upcoming sample
    virtual Real getNextSample1D() = 0;
    // Acquires two upcoming samples
    virtual Point2f getNextSample2D() = 0;

    void request1DArray(int n);
    void request2DArray(int n);

    const Real* get1DArray(int n);
    const Point2f* get2DArray(int n);

    virtual int refineRequestCount(int n) const { return n; }
    // This is to be called for use with multiple threads
    virtual std::unique_ptr<Sampler> clone(int seed) = 0;
    // Manually set the index of the sample to access
    virtual bool setSampleIndex(int64_t sampleIdx);

    // Initializes CameraSample for a given pixel
    CameraSample getCameraSample(const Point2i& pRaster);

    int64_t currentSampleIndex() const { return currentPixelSampleIndex; }

    const int64_t samplesPerPixel;

  protected:
    Point2i currentPixel;
    int64_t currentPixelSampleIndex;

    std::vector<int> samples1DArraySizes, samples2DArraySizes;
    std::vector<std::vector<Real>> sampleArray1D;
    std::vector<std::vector<Point2f>> sampleArray2D;

  private:
    size_t array1DOffset, array2DOffset;
};


// PixelSampler declarations
class PixelSampler : public Sampler {
  public:
    PixelSampler(int64_t samplesPerPixel, int nSampledDimensions);

    // Interface
    virtual bool startNextSample() override;
    virtual bool setSampleIndex(int64_t sampleIdx) override;

    virtual Real getNextSample1D() override;
    virtual Point2f getNextSample2D() override;

  protected:
    std::vector<std::vector<Real>> samples1D;
    std::vector<std::vector<Point2f>> samples2D;

    size_t current1DDimension, current2DDimension;
    RNG rng;
};


// GlobalSampler declarations
class GlobalSampler : public Sampler {
  public:
    GlobalSampler(int64_t samplesPerPixel) : Sampler(samplesPerPixel) {}

    // Interface
    virtual int64_t getIndexForSample(int64_t sampleIdx) const = 0;
    /**
     * Returns the sample offset within the current pixel
     */
    virtual Real sampleDimension(int64_t index, int dimension) const = 0;

    void startPixel(const Point2i& pt) override;
    bool startNextSample() override;
    bool setSampleIndex(int64_t sampleIdx) override;

    Real getNextSample1D() override;
    Point2f getNextSample2D() override;

  private:
    int dimension;
    int64_t intervalSampleIndex;

    // Dimension intervals
    static const int arrayStartDim = 5;
    int arrayEndDim;
};

}  // namespace phyr

#endif
