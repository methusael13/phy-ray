#ifndef PHYRAY_CORE_MATERIAL_H
#define PHYRAY_CORE_MATERIAL_H

#include <core/phyr.h>
#include <core/phyr_mem.h>

namespace phyr {

enum class TransportMode { Radiance, Importance };

// Material declarations
class Material {
  public:
    // Interface

    /**
     * Compute the light scattering functions modelled by the object's surface
     */
    virtual void computeScatteringFunctions(SurfaceInteraction* si,
                                            MemoryPool& mem, TransportMode mode,
                                            bool allowMultiLobes) const = 0;
    virtual ~Material();
};

}  // namespace phyr

#endif
