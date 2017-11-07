#include <core/material/reflectance.h>

#include <modules/materials/glass.h>
#include <modules/textures/consttex.h>

namespace phyr {

// GlassMaterial Method Definitions
void GlassMaterial::computeScatteringFunctions(SurfaceInteraction* si,
                                               MemoryPool& pool,
                                               TransportMode mode,
                                               bool allowMultipleLobes) const {
    Real eta = index->evaluate(*si);
    Real urough = uRoughness->evaluate(*si);
    Real vrough = vRoughness->evaluate(*si);

    Spectrum R = Kr->evaluate(*si).clamp();
    Spectrum T = Kt->evaluate(*si).clamp();

    // Initialize {bsdf} for smooth or rough dielectric
    si->bsdf = POOL_ALLOC(pool, BSDF)(*si, eta);

    if (R.isBlack() && T.isBlack()) return;

    bool isSpecular = urough == 0 && vrough == 0;
    if (isSpecular && allowMultipleLobes) {
        si->bsdf->add(POOL_ALLOC(pool, FresnelSpecular)(R, T, 1.f, eta, mode));
    } else {
        if (remapRoughness) {
            urough = TrowbridgeReitzDistribution::roughnessToAlpha(urough);
            vrough = TrowbridgeReitzDistribution::roughnessToAlpha(vrough);
        }
        MicrofacetDistribution* distrib =
            isSpecular ? nullptr : POOL_ALLOC(pool, TrowbridgeReitzDistribution)(urough, vrough);

        if (!R.isBlack()) {
            Fresnel *fresnel = POOL_ALLOC(pool, FresnelDielectric)(1.f, eta);
            if (isSpecular)
                si->bsdf->add(POOL_ALLOC(pool, SpecularReflection)(R, fresnel));
            else
                si->bsdf->add(POOL_ALLOC(pool, MicrofacetReflection)(R, distrib, fresnel));
        }

        if (!T.isBlack()) {
            if (isSpecular)
                si->bsdf->add(POOL_ALLOC(pool, SpecularTransmission)(T, 1.f, eta, mode));
            else
                si->bsdf->add(POOL_ALLOC(pool, MicrofacetTransmission)(T, distrib, 1.f, eta, mode));
        }
    }
}

GlassMaterial* createGlassMaterial(Real Kr, Real Kt, Real eta) {
    std::shared_ptr<Texture<Spectrum>> kr =
            std::make_shared<Texture<Spectrum>>(Kr);
    std::shared_ptr<Texture<Spectrum>> kt =
            std::make_shared<Texture<Spectrum>>(Kt);

    std::shared_ptr<Texture<Real>> _eta =
            std::make_shared<Texture<Real>>(eta);

    std::shared_ptr<Texture<Real>> roughu =
            std::make_shared<Texture<Real>>(0);
    std::shared_ptr<Texture<Real>> roughv =
            std::make_shared<Texture<Real>>(0);

    return new GlassMaterial(kr, kt, roughu, roughv, _eta, true);
}

}  // namespace pbrt
