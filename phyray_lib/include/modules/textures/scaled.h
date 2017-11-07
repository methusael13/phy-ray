#ifndef PHYRAY_MODULES_SCALEDTEX_H
#define PHYRAY_MODULES_SCALEDTEX_H

#include <memory>

#include <core/material/texture.h>
#include <core/geometry/interaction.h>

namespace phyr {

template <typename T1, typename T2>
class ScaleTexture : public Texture<T2> {
  public:
    // Constructor
    ScaleTexture(const std::shared_ptr<Texture<T1>>& tex1,
                 const std::shared_ptr<Texture<T2>>& tex2) :
        tex1(tex1), tex2(tex2) {}

    // Interface
    T2 evaluate(const SurfaceInteraction& si) const override {
        return tex1->evaluate(si) * tex2->evaluate(si);
    }

  private:
    std::shared_ptr<Texture<T1>> tex1;
    std::shared_ptr<Texture<T2>> tex2;
};

}  // namespace phyr

#endif
