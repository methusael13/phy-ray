#ifndef PHYRAY_MODULES_MIRRORMAT_H
#define PHYRAY_MODULES_MIRRORMAT_H

#include <memory>

#include <core/color/spectrum.h>
#include <core/material/texture.h>
#include <core/material/material.h>

namespace phyr {

// MirrorMaterial Declarations
class MirrorMaterial : public Material {
  public:
    MirrorMaterial(const std::shared_ptr<Texture<Spectrum>>& r) : Kr(r) {}

    // Interface
    void computeScatteringFunctions(SurfaceInteraction* si, MemoryPool& pool,
                                    TransportMode mode,
                                    bool allowMultipleLobes) const override;

  private:
    std::shared_ptr<Texture<Spectrum>> Kr;
};

MirrorMaterial* createMirrorMaterial(Real Kr = 1);

}  // namespace phyr

#endif
