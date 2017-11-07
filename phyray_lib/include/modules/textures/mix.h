#ifndef PHYRAY_MODULES_MIXTEX_H
#define PHYRAY_MODULES_MIXTEX_H

#include <memory>

#include <core/material/texture.h>
#include <core/geometry/interaction.h>

namespace phyr {

template <typename T>
class MixTexture : public Texture<T> {
  public:
    // Constructor
    MixTexture(const std::shared_ptr<Texture<T>>& tex1,
               const std::shared_ptr<Texture<T>>& tex2,
               const std::shared_ptr<Texture<Real>>& factor) :
        tex1(tex1), tex2(tex2), factor(factor) {}

    // Interface

    /**
     * Linearly interpolates between the two given textures {tex1}
     * and {tex2} using {factor} as the blend factor
     */
    T evaluate(const SurfaceInteraction& si) const override {
        T fact = factor->evaluate(si);
        return (1 - fact) * tex1->evaluate(si) + fact * tex2->evaluate(si);
    }

  private:
    std::shared_ptr<Texture<T>> tex1, tex2;
    std::shared_ptr<Texture<Real>> factor;
};

}  // namespace phyr

#endif
