#include <core/phyr_mem.h>
#include <core/material/reflectance.h>

#include <modules/materials/matte.h>
#include <modules/textures/consttex.h>

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

MatteMaterial* createMatteMaterial(Real Kd, Real sigma) {
    std::shared_ptr<Texture<Spectrum>> kd =
            std::make_shared<ConstantTexture<Spectrum>>(Spectrum(Kd));
    std::shared_ptr<Texture<Real>> _sigma =
            std::make_shared<ConstantTexture<Real>>(sigma);

    return new MatteMaterial(kd, _sigma);
}

MatteMaterial* createMatteMaterial(Real rgb[3], Real sigma) {
    Spectrum matteRGB = Spectrum::getFromRGB(rgb);
    std::shared_ptr<Texture<Spectrum>> kd =
            std::make_shared<ConstantTexture<Spectrum>>(matteRGB);
    std::shared_ptr<Texture<Real>> _sigma =
            std::make_shared<ConstantTexture<Real>>(sigma);

    return new MatteMaterial(kd, _sigma);
}

}  // namespace phyr
