#ifndef PHYRAY_MODULES_CONSTTEX_H
#define PHYRAY_MODULES_CONSTTEX_H

#include <core/material/texture.h>

namespace phyr {

template <typename T>
class ConstantTexture : public Texture<T> {
  public:
    // Constructor
    ConstantTexture(const T& value) : value(value) {}

    // Interface
    T evaluate(const SurfaceInteraction&) const override { return value; }

  private:
    T value;
};

}  // namespace phyr

#endif
