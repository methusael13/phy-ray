#ifndef PHYRAY_MODULES_PATHINTEGRATOR_H
#define PHYRAY_MODULES_PATHINTEGRATOR_H

#include <core/phyr.h>
#include <core/integrator/integrator.h>
#include <core/integrator/lightdistrib.h>

namespace phyr {

// PathIntegrator Declarations
class PathIntegrator : public SamplerIntegrator {
  public:
    PathIntegrator(int maxDepth, std::shared_ptr<const Camera> camera,
                   std::shared_ptr<Sampler> sampler,
                   const Bounds2i& pixelBounds, Real rrThreshold = 1,
                   const std::string& lightSampleStrategy = "spatial");

    void preprocess(const Scene& scene, Sampler& sampler) override;

    Spectrum li(const Ray& ray, const Scene& scene,
                Sampler& sampler, MemoryPool& pool, int depth) const override;

  private:
    const int maxDepth;
    const Real rrThreshold;
    const std::string lightSampleStrategy;
    std::unique_ptr<LightDistribution> lightDistribution;
};

PathIntegrator* createPathIntegrator(std::shared_ptr<Sampler> sampler,
                                     std::shared_ptr<const Camera> camera,
                                     int maxDepth = 5);

}  // namespace phyr

#endif
