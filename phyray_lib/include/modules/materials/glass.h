#ifndef PHYRAY_MODULES_GLASSMAT_H
#define PHYRAY_MODULES_GLASSMAT_H

#include <memory>

#include <core/color/spectrum.h>
#include <core/material/texture.h>
#include <core/material/material.h>

namespace phyr {

// GlassMaterial Declarations
class GlassMaterial : public Material {
  public:
    GlassMaterial(const std::shared_ptr<Texture<Spectrum>>& Kr,
                  const std::shared_ptr<Texture<Spectrum>>& Kt,
                  const std::shared_ptr<Texture<Real>>& uRoughness,
                  const std::shared_ptr<Texture<Real>>& vRoughness,
                  const std::shared_ptr<Texture<Real>>& index,
                  bool remapRoughness) :
        Kr(Kr), Kt(Kt),
        uRoughness(uRoughness), vRoughness(vRoughness),
        index(index), remapRoughness(remapRoughness) {}

    // Interface
    void computeScatteringFunctions(SurfaceInteraction* si, MemoryPool& pool,
                                    TransportMode mode,
                                    bool allowMultipleLobes) const override;

  private:
    std::shared_ptr<Texture<Spectrum>> Kr, Kt;
    std::shared_ptr<Texture<Real>> uRoughness, vRoughness;
    std::shared_ptr<Texture<Real>> index;
    bool remapRoughness;
};

GlassMaterial* createGlassMaterial(Real Kr = 1, Real Kt = 1, Real eta = 1.5);

}  // namespace phyr

#endif
