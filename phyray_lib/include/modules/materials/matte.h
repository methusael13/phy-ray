#ifndef PHYRAY_MODULES_MATTEMAT_H
#define PHYRAY_MODULES_MATTEMAT_H

#include <memory>

#include <core/color/spectrum.h>
#include <core/material/texture.h>
#include <core/material/material.h>

namespace phyr {

class MatteMaterial : public Material {
  public:
    MatteMaterial(const std::shared_ptr<Texture<Spectrum>>& Kd,
                  const std::shared_ptr<Texture<Real>>& sigma) :
        Kd(Kd), sigma(sigma) {}

    // Interface
    void computeScatteringFunctions(SurfaceInteraction* si,
                                    MemoryPool& mem, TransportMode mode,
                                    bool allowMultiLobes) const override;

  private:
    /**
     * @brief Kd The diffuse component
     */
    std::shared_ptr<Texture<Spectrum>> Kd;

    /**
     * @brief {sigma} defines the surface roughness factor.
     * If {sigma} has a value of 0, {MatteMaterial} creates a
     * {LambertianReflection} BRDF, otherwise the {OrenNayar} BRDF.
     */
    std::shared_ptr<Texture<Real>> sigma;
};

}  // namespace phyr

#endif
