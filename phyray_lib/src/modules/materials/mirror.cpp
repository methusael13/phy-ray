#include <core/material/reflectance.h>

#include <modules/materials/mirror.h>

namespace phyr {

// MirrorMaterial Method Definitions
void MirrorMaterial::computeScatteringFunctions(SurfaceInteraction *si,
                                                MemoryPool& pool,
                                                TransportMode mode,
                                                bool allowMultipleLobes) const {
    si->bsdf = POOL_ALLOC(pool, BSDF)(*si);
    Spectrum reflectSpec = Kr->evaluate(*si).clamp();

    if (!reflectSpec.isBlack()) {
        si->bsdf->add(POOL_ALLOC(pool, SpecularReflection)(
            reflectSpec, POOL_ALLOC(pool, FresnelPureReflect)()));
    }
}

MirrorMaterial* createMirrorMaterial(Real Kr) {
    std::shared_ptr<Texture<Spectrum>> kr =
            std::make_shared<Texture<Spectrum>>(Kr);

    return new MirrorMaterial(kr);
}

}  // namespace phyr
