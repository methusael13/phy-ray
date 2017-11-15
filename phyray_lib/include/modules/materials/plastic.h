#ifndef PHYRAY_MODULES_PLASTICMAT_H
#define PHYRAY_MODULES_PLASTICMAT_H

#include <memory>

#include <core/color/spectrum.h>
#include <core/material/texture.h>
#include <core/material/material.h>

namespace phyr {

// PlasticMaterial Declarations
class PlasticMaterial : public Material {
  public:
    // PlasticMaterial Public Methods
    PlasticMaterial(const std::shared_ptr<Texture<Spectrum>>& Kd,
                    const std::shared_ptr<Texture<Spectrum>>& Ks,
                    const std::shared_ptr<Texture<Real>>& roughness,
                    bool remapRoughness)
        : Kd(Kd), Ks(Ks), roughness(roughness), remapRoughness(remapRoughness) {}

    // Interface
    void computeScatteringFunctions(SurfaceInteraction* si, MemoryPool& pool,
                                    TransportMode mode,
                                    bool allowMultipleLobes) const override;

  private:
    std::shared_ptr<Texture<Spectrum>> Kd, Ks;
    std::shared_ptr<Texture<Real>> roughness;
    const bool remapRoughness;
};

PlasticMaterial* createPlasticMaterial(Real Kd = 0.25, Real Ks = 0.25,
                                       Real roughness = 0.1,
                                       bool remapRoughness = true);

PlasticMaterial* createPlasticMaterial(Real rgb[3], Real Ks = 0.25,
                                       Real roughness = 0.1,
                                       bool remapRoughness = true);

}  // namespace phyr

#endif
