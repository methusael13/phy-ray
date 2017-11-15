#include <core/phyr_mem.h>
#include <modules/materials/plastic.h>
#include <core/material/reflectance.h>

#include <modules/textures/consttex.h>

namespace phyr {

// PlasticMaterial Method Definitions
void PlasticMaterial::computeScatteringFunctions(SurfaceInteraction* si,
                                                 MemoryPool& pool, TransportMode mode,
                                                 bool allowMultipleLobes) const {
    si->bsdf = POOL_ALLOC(pool, BSDF)(*si);
    // Initialize diffuse component of plastic material
    Spectrum kd = Kd->evaluate(*si).clamp();
    if (!kd.isBlack())
        si->bsdf->add(POOL_ALLOC(pool, LambertianReflection)(kd));

    // Initialize specular component of plastic material
    Spectrum ks = Ks->evaluate(*si).clamp();
    if (!ks.isBlack()) {
        Fresnel* fresnel = POOL_ALLOC(pool, FresnelDielectric)(1.5f, 1.f);
        // Create microfacet distribution {distrib} for plastic material
        Real rough = roughness->evaluate(*si);
        if (remapRoughness)
            rough = TrowbridgeReitzDistribution::roughnessToAlpha(rough);
        MicrofacetDistribution *distrib =
            POOL_ALLOC(pool, TrowbridgeReitzDistribution)(rough, rough);
        BxDF* spec = POOL_ALLOC(pool, MicrofacetReflection)(ks, distrib, fresnel);
        si->bsdf->add(spec);
    }
}

PlasticMaterial* createPlasticMaterial(Real Kd, Real Ks, Real roughness,
                                       bool remapRoughness) {
    std::shared_ptr<Texture<Spectrum>> kd =
            std::make_shared<ConstantTexture<Spectrum>>(Kd);
    std::shared_ptr<Texture<Spectrum>> ks =
            std::make_shared<ConstantTexture<Spectrum>>(Ks);
    std::shared_ptr<Texture<Real>> _roughness =
            std::make_shared<ConstantTexture<Real>>(roughness);

    return new PlasticMaterial(kd, ks, _roughness, remapRoughness);
}

PlasticMaterial* createPlasticMaterial(Real rgb[3], Real Ks, Real roughness,
                                       bool remapRoughness) {
    Spectrum kdRGB = Spectrum::getFromRGB(rgb, SpectrumType::Reflectance);

    std::shared_ptr<Texture<Spectrum>> kd =
            std::make_shared<ConstantTexture<Spectrum>>(kdRGB);
    std::shared_ptr<Texture<Spectrum>> ks =
            std::make_shared<ConstantTexture<Spectrum>>(Ks);
    std::shared_ptr<Texture<Real>> _roughness =
            std::make_shared<ConstantTexture<Real>>(roughness);

    return new PlasticMaterial(kd, ks, _roughness, remapRoughness);
}

}  // namespace phyr
