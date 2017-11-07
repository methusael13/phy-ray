#include <core/phyr_mem.h>
#include <modules/materials/matte.h>
#include <core/material/reflectance.h>

namespace phyr {

void MatteMaterial::computeScatteringFunctions(SurfaceInteraction* si,
                                               MemoryPool& pool, TransportMode mode,
                                               bool allowMultiLobes) const {
    // Evaluate textures for {MatteMaterial} material and allocate BRDF
    si->bsdf = POOL_ALLOC(pool, BSDF)(*si);
    Spectrum r = Kd->evaluate(*si).clamp();
    Real sig = clamp(sigma->evaluate(*si), 0, 90);

    if (!r.isBlack()) {
        if (sig == 0)
            si->bsdf->add(POOL_ALLOC(pool, LambertianReflection)(r));
        else
            si->bsdf->add(POOL_ALLOC(pool, OrenNayar)(r, sig));
    }
}

}  // namespace phyr
