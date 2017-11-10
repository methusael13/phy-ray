#ifndef PHYRAY_CORE_INTEGRATOR_H
#define PHYRAY_CORE_INTEGRATOR_H

#include <core/phyr.h>
#include <core/object/object.h>
#include <core/color/spectrum.h>
#include <core/light/light.h>
#include <core/material/reflectance.h>
#include <core/integrator/sampler.h>
#include <core/material/material.h>
#include <core/integrator/sampling.h>

namespace phyr {

// Integrator Declarations
class Integrator {
  public:
    virtual ~Integrator();
    virtual void render(const Scene& scene) = 0;
};

Spectrum uniformSampleAllLights(const Interaction& it, const Scene& scene,
                                MemoryPool& pool, Sampler& sampler,
                                const std::vector<int>& nLightSamples,
                                bool handleMedia = false);
Spectrum uniformSampleOneLight(const Interaction& it, const Scene& scene,
                               MemoryPool& pool, Sampler& sampler,
                               bool handleMedia = false,
                               const Distribution1D *lightDistrib = nullptr);
Spectrum estimateDirect(const Interaction& it, const Point2f& uShading,
                        const Light& light, const Point2f& uLight,
                        const Scene& scene, Sampler& sampler,
                        MemoryPool& pool, bool handleMedia = false,
                        bool specular = false);

std::unique_ptr<Distribution1D> computeLightPowerDistribution(const Scene& scene);

// SamplerIntegrator Declarations
class SamplerIntegrator : public Integrator {
  public:
    SamplerIntegrator(std::shared_ptr<const Camera> camera,
                      std::shared_ptr<Sampler> sampler,
                      const Bounds2i& pixelBounds)
        : camera(camera), sampler(sampler), pixelBounds(pixelBounds) {}

    // Interface
    virtual void preprocess(const Scene& scene, Sampler& sampler) {}
    void render(const Scene& scene);

    /**
     * Evaluates the radiance along a given camera ray {ray}
     */
    virtual Spectrum li(const Ray& ray, const Scene& scene,
                        Sampler& sampler, MemoryPool& arena,
                        int depth = 0) const = 0;

    Spectrum specularReflect(const Ray& ray,
                             const SurfaceInteraction& isect,
                             const Scene& scene, Sampler& sampler,
                             MemoryPool& pool, int depth) const;
    Spectrum specularTransmit(const Ray& ray,
                              const SurfaceInteraction& isect,
                              const Scene& scene, Sampler& sampler,
                              MemoryPool& pool, int depth) const;

  protected:
    std::shared_ptr<const Camera> camera;

  private:
    std::shared_ptr<Sampler> sampler;
    const Bounds2i pixelBounds;
};

}  // namespace phyr

#endif
