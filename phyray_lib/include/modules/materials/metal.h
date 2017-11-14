#ifndef PHYRAY_MODULES_METALMAT_H
#define PHYRAY_MODULES_METALMAT_H

#include <memory>

#include <core/color/spectrum.h>
#include <core/material/texture.h>
#include <core/material/material.h>

namespace phyr {

// Available ready-made metal materials
enum MetalType { Copper, Gold, Silver, Aluminium };

// MetalMaterial Declarations
class MetalMaterial : public Material {
  public:
    // Constructor
    MetalMaterial(const std::shared_ptr<Texture<Spectrum>>& eta,
                  const std::shared_ptr<Texture<Spectrum>>& k,
                  const std::shared_ptr<Texture<Real>>& rough,
                  const std::shared_ptr<Texture<Real>>& urough,
                  const std::shared_ptr<Texture<Real>>& vrough,
                  bool remapRoughness) :
        eta(eta), k(k), roughness(rough), uRoughness(urough),
        vRoughness(vrough), remapRoughness(remapRoughness) {}

    // Interface
    void computeScatteringFunctions(SurfaceInteraction* si, MemoryPool& pool,
                                    TransportMode mode,
                                    bool allowMultipleLobes) const override;

  private:
    std::shared_ptr<Texture<Spectrum>> eta, k;
    std::shared_ptr<Texture<Real>> roughness, uRoughness, vRoughness;
    bool remapRoughness;
};

MetalMaterial* createMetalMaterial(MetalType type = MetalType::Copper,
                                   Real rough = .01);

}  // namespace phyr

#endif
