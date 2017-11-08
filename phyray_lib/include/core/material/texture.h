#ifndef PHYRAY_CORE_TEXTURE_H
#define PHYRAY_CORE_TEXTURE_H

#include <core/phyr.h>
#include <core/geometry/interaction.h>

namespace phyr {

template <typename T>
class Texture {
  public:
    // Interface
    virtual T evaluate(const SurfaceInteraction& si) const = 0;

    virtual ~Texture() {}
};

}  // namespace phyr

#endif
