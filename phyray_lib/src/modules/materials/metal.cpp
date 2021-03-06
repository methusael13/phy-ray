#include <core/material/reflectance.h>

#include <modules/materials/metal.h>
#include <modules/textures/consttex.h>

namespace phyr {

// MetalMaterial Method Definitions
void MetalMaterial::computeScatteringFunctions(SurfaceInteraction* si, MemoryPool& pool,
                                               TransportMode mode,
                                               bool allowMultipleLobes) const {
    si->bsdf = POOL_ALLOC(pool, BSDF)(*si);

    Real uRough =
        uRoughness ? uRoughness->evaluate(*si) : roughness->evaluate(*si);
    Real vRough =
        vRoughness ? vRoughness->evaluate(*si) : roughness->evaluate(*si);
    if (remapRoughness) {
        uRough = TrowbridgeReitzDistribution::roughnessToAlpha(uRough);
        vRough = TrowbridgeReitzDistribution::roughnessToAlpha(vRough);
    }

    Fresnel* frMf =
            POOL_ALLOC(pool, FresnelConductor)(1., eta->evaluate(*si), k->evaluate(*si));
    MicrofacetDistribution* distrib =
        POOL_ALLOC(pool, TrowbridgeReitzDistribution)(uRough, vRough);
    si->bsdf->add(POOL_ALLOC(pool, MicrofacetReflection)(1., distrib, frMf));
}

const int WavelengthSampleCount = 56;
const Real MetalWavelengths[WavelengthSampleCount] = {
    298.7570554, 302.4004341, 306.1337728, 309.960445,  313.8839949,
    317.9081487, 322.036826,  326.2741526, 330.6244747, 335.092373,
    339.6826795, 344.4004944, 349.2512056, 354.2405086, 359.374429,
    364.6593471, 370.1020239, 375.7096303, 381.4897785, 387.4505563,
    393.6005651, 399.9489613, 406.5055016, 413.2805933, 420.2853492,
    427.5316483, 435.0322035, 442.8006357, 450.8515564, 459.2006593,
    467.8648226, 476.8622231, 486.2124627, 495.936712,  506.0578694,
    516.6007417, 527.5922468, 539.0616435, 551.0407911, 563.5644455,
    576.6705953, 590.4008476, 604.8008683, 619.92089,   635.8162974,
    652.5483053, 670.1847459, 688.8009889, 708.4810171, 729.3186941,
    751.4192606, 774.9011125, 799.8979226, 826.5611867, 855.0632966,
    885.6012714
};

// Indices of refraction {N} and Absorption coefficient {K} for Cu
const Real CuN[WavelengthSampleCount] = {
    1.400313, 1.380000, 1.358438, 1.340000, 1.329063, 1.325000, 1.332500, 1.340000,
    1.334375, 1.325000, 1.317812, 1.310000, 1.300313, 1.290000, 1.281563, 1.270000,
    1.249062, 1.225000, 1.200000, 1.180000, 1.174375, 1.175000, 1.177500, 1.180000,
    1.178125, 1.175000, 1.172812, 1.170000, 1.165312, 1.160000, 1.155312, 1.150000,
    1.142812, 1.135000, 1.131562, 1.120000, 1.092437, 1.040000, 0.950375, 0.826000,
    0.645875, 0.468000, 0.351250, 0.272000, 0.230813, 0.214000, 0.209250, 0.213000,
    0.216250, 0.223000, 0.236500, 0.250000, 0.254188, 0.260000, 0.280000, 0.300000
};

const Real CuK[WavelengthSampleCount] = {
    1.662125, 1.687000, 1.703313, 1.720000, 1.744563, 1.770000, 1.791625, 1.810000,
    1.822125, 1.834000, 1.851750, 1.872000, 1.894250, 1.916000, 1.931688, 1.950000,
    1.972438, 2.015000, 2.121562, 2.210000, 2.177188, 2.130000, 2.160063, 2.210000,
    2.249938, 2.289000, 2.326000, 2.362000, 2.397625, 2.433000, 2.469187, 2.504000,
    2.535875, 2.564000, 2.589625, 2.605000, 2.595562, 2.583000, 2.576500, 2.599000,
    2.678062, 2.809000, 3.010750, 3.240000, 3.458187, 3.670000, 3.863125, 4.050000,
    4.239563, 4.430000, 4.619563, 4.817000, 5.034125, 5.260000, 5.485625, 5.717000
};

// Indices of refraction {N} and Absorption coefficient {K} for Au
const Real AuN[WavelengthSampleCount] = {
    1.795000, 1.812000, 1.822625, 1.830000, 1.837125, 1.840000, 1.834250, 1.824000,
    1.812000, 1.798000, 1.782000, 1.766000, 1.752500, 1.740000, 1.727625, 1.716000,
    1.705875, 1.696000, 1.684750, 1.674000, 1.666000, 1.658000, 1.647250, 1.636000,
    1.628000, 1.616000, 1.596250, 1.562000, 1.502125, 1.426000, 1.345875, 1.242000,
    1.086750, 0.916000, 0.754500, 0.608000, 0.491750, 0.402000, 0.345500, 0.306000,
    0.267625, 0.236000, 0.212375, 0.194000, 0.177750, 0.166000, 0.161000, 0.160000,
    0.160875, 0.164000, 0.169500, 0.176000, 0.181375, 0.188000, 0.198125, 0.210000
};

const Real AuK[WavelengthSampleCount] = {
    1.920375, 1.920000, 1.918875, 1.916000, 1.911375, 1.904000, 1.891375, 1.878000,
    1.868250, 1.860000, 1.851750, 1.846000, 1.845250, 1.848000, 1.852375, 1.862000,
    1.883000, 1.906000, 1.922500, 1.936000, 1.947750, 1.956000, 1.959375, 1.958000,
    1.951375, 1.940000, 1.924500, 1.904000, 1.875875, 1.846000, 1.814625, 1.796000,
    1.797375, 1.840000, 1.956500, 2.120000, 2.326250, 2.540000, 2.730625, 2.880000,
    2.940625, 2.970000, 3.015000, 3.060000, 3.070000, 3.150000, 3.445812, 3.800000,
    4.087687, 4.357000, 4.610188, 4.860000, 5.125813, 5.390000, 5.631250, 5.880000
};

// Indices of refraction {N} and Absorption coefficient {K} for Ag
const Real AgN[WavelengthSampleCount] = {
    1.519000, 1.496000, 1.432500, 1.323000, 1.142062, 0.932000, 0.719062, 0.526000,
    0.388125, 0.294000, 0.253313, 0.238000, 0.221438, 0.209000, 0.194813, 0.186000,
    0.192063, 0.200000, 0.198063, 0.192000, 0.182000, 0.173000, 0.172625, 0.173000,
    0.166688, 0.160000, 0.158500, 0.157000, 0.151063, 0.144000, 0.137313, 0.132000,
    0.130250, 0.130000, 0.129938, 0.130000, 0.130063, 0.129000, 0.124375, 0.120000,
    0.119313, 0.121000, 0.125500, 0.131000, 0.136125, 0.140000, 0.140063, 0.140000,
    0.144313, 0.148000, 0.145875, 0.143000, 0.142563, 0.145000, 0.151938, 0.163000
};

const Real AgK[WavelengthSampleCount] = {
    1.080000, 0.882000, 0.761063, 0.647000, 0.550875, 0.504000, 0.554375, 0.663000,
    0.818563, 0.986000, 1.120687, 1.240000, 1.345250, 1.440000, 1.533750, 1.610000,
    1.641875, 1.670000, 1.735000, 1.810000, 1.878750, 1.950000, 2.029375, 2.110000,
    2.186250, 2.260000, 2.329375, 2.400000, 2.478750, 2.560000, 2.640000, 2.720000,
    2.798125, 2.880000, 2.973750, 3.070000, 3.159375, 3.250000, 3.348125, 3.450000,
    3.553750, 3.660000, 3.766250, 3.880000, 4.010625, 4.150000, 4.293125, 4.440000,
    4.586250, 4.740000, 4.908125, 5.090000, 5.288750, 5.500000, 5.720624, 5.950000
};

// Indices of refraction {N} and Absorption coefficient {K} for Al
const Real AlN[WavelengthSampleCount] = {
    0.273375, 0.280000, 0.286813, 0.294000, 0.301875, 0.310000, 0.317875, 0.326000,
    0.334750, 0.344000, 0.353813, 0.364000, 0.374375, 0.385000, 0.395750, 0.407000,
    0.419125, 0.432000, 0.445688, 0.460000, 0.474688, 0.490000, 0.506188, 0.523000,
    0.540063, 0.558000, 0.577313, 0.598000, 0.620313, 0.644000, 0.668625, 0.695000,
    0.723750, 0.755000, 0.789000, 0.826000, 0.867000, 0.912000, 0.963000, 1.020000,
    1.080000, 1.150000, 1.220000, 1.300000, 1.390000, 1.490000, 1.600000, 1.740000,
    1.910000, 2.140000, 2.410000, 2.630000, 2.800000, 2.740000, 2.580000, 2.240000
};

const Real AlK[WavelengthSampleCount] = {
    3.593750, 3.640000, 3.689375, 3.740000, 3.789375, 3.840000, 3.894375, 3.950000,
    4.005000, 4.060000, 4.113750, 4.170000, 4.233750, 4.300000, 4.365000, 4.430000,
    4.493750, 4.560000, 4.633750, 4.710000, 4.784375, 4.860000, 4.938125, 5.020000,
    5.108750, 5.200000, 5.290000, 5.380000, 5.480000, 5.580000, 5.690000, 5.800000,
    5.915000, 6.030000, 6.150000, 6.280000, 6.420000, 6.550000, 6.700000, 6.850000,
    7.000000, 7.150000, 7.310000, 7.480000, 7.650000, 7.820000, 8.010000, 8.210000,
    8.390000, 8.570000, 8.620000, 8.600000, 8.450000, 8.310000, 8.210000, 8.210000
};

MetalMaterial* createMetalMaterial(MetalType type, Real rough) {
    const Real *nData, *kData;

    switch (type) {
        case Copper: nData = CuN; kData = CuK; break;
        case Gold: nData = AuN; kData = AuK; break;
        case Silver: nData = AgN; kData = AgK; break;
        case Aluminium: nData = AlN; kData = kData; break;
    }

    // Spectrum data from metal {eta} and {k} values
    std::shared_ptr<Texture<Spectrum>> metalN =
        std::make_shared<ConstantTexture<Spectrum>>(
            Spectrum::getFromSample(MetalWavelengths, nData, WavelengthSampleCount));
    std::shared_ptr<Texture<Spectrum>> metalK =
        std::make_shared<ConstantTexture<Spectrum>>(
            Spectrum::getFromSample(MetalWavelengths, kData, WavelengthSampleCount));

    std::shared_ptr<Texture<Real>> roughness =
        std::make_shared<ConstantTexture<Real>>(rough);

    return new MetalMaterial(metalN, metalK, roughness, nullptr, nullptr, false);
}

}  // namespace phyr
