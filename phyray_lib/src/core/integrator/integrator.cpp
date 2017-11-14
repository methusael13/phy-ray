#include <core/scene.h>
#include <core/phyr_reporter.h>
#include <core/integrator/integrator.h>

namespace phyr {

// Integrator Method Definitions
Integrator::~Integrator() {}

// Integrator Utility Functions
Spectrum uniformSampleAllLights(const Interaction& it, const Scene& scene,
                                MemoryPool& pool, Sampler& sampler,
                                const std::vector<int>& nLightSamples,
                                bool handleMedia) {
    Spectrum L(0.f);
    for (size_t j = 0; j < scene.lights.size(); ++j) {
        // Accumulate contribution of _j_th light to _L_
        const std::shared_ptr<Light> &light = scene.lights[j];
        int nSamples = nLightSamples[j];
        const Point2f *uLightArray = sampler.get2DArray(nSamples);
        const Point2f *uScatteringArray = sampler.get2DArray(nSamples);
        if (!uLightArray || !uScatteringArray) {
            // Use a single sample for illumination from _light_
            Point2f uLight = sampler.getNextSample2D();
            Point2f uScattering = sampler.getNextSample2D();
            L += estimateDirect(it, uScattering, *light, uLight, scene, sampler,
                                pool, handleMedia);
        } else {
            // Estimate direct lighting using sample arrays
            Spectrum Ld(0.f);
            for (int k = 0; k < nSamples; ++k)
                Ld += estimateDirect(it, uScatteringArray[k], *light,
                                     uLightArray[k], scene, sampler, pool,
                                     handleMedia);
            L += Ld / nSamples;
        }
    }

    return L;
}

Spectrum uniformSampleOneLight(const Interaction& it, const Scene& scene,
                               MemoryPool& arena, Sampler& sampler,
                               bool handleMedia, const Distribution1D* lightDistrib) {
    // Randomly choose a single light to sample, _light_
    int nLights = int(scene.lights.size());
    if (nLights == 0) return Spectrum(0.f);
    int lightNum; Real lightPdf;

    if (lightDistrib) {
        lightNum = lightDistrib->sampleDiscrete(sampler.getNextSample1D(), &lightPdf);
        if (lightPdf == 0) return Spectrum(0.f);
    } else {
        lightNum = std::min((int)(sampler.getNextSample1D() * nLights), nLights - 1);
        lightPdf = Real(1) / nLights;
    }

    const std::shared_ptr<Light>& light = scene.lights[lightNum];
    Point2f uLight = sampler.getNextSample2D();
    Point2f uScattering = sampler.getNextSample2D();

    return estimateDirect(it, uScattering, *light, uLight,
                          scene, sampler, arena, handleMedia) / lightPdf;
}

Spectrum estimateDirect(const Interaction& it, const Point2f& uScattering,
                        const Light& light, const Point2f& uLight,
                        const Scene& scene, Sampler& sampler,
                        MemoryPool& pool, bool handleMedia, bool specular) {
    BxDFType bsdfFlags =
        specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
    Spectrum Ld(0.f);

    // Sample light source with multiple importance sampling
    Vector3f wi;
    Real lightPdf = 0, scatteringPdf = 0;
    VisibilityTester visibility;
    Spectrum Li = light.sample_li(it, uLight, &wi, &lightPdf, &visibility);

    if (lightPdf > 0 && !Li.isBlack()) {
        // Compute BSDF or phase function's value for light sample
        Spectrum f;
        if (it.isSurfaceInteraction()) {
            // Evaluate BSDF for light sampling strategy
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
            f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
                absDot(wi, isect.shadingGeom.n);
            scatteringPdf = isect.bsdf->pdf(isect.wo, wi, bsdfFlags);
        }
//        else {
//            // Evaluate phase function for light sampling strategy
//            const MediumInteraction &mi = (const MediumInteraction &)it;
//            Float p = mi.phase->p(mi.wo, wi);
//            f = Spectrum(p);
//            scatteringPdf = p;
//            VLOG(2) << "  medium p: " << p;
//        }

        if (!f.isBlack()) {
            // Compute effect of visibility for light source sample
            if (handleMedia) {
                Li *= visibility.tr(scene, sampler);
            } else {
                if (!visibility.unoccluded(scene)) {
                    Li = Spectrum(0.f);
                }
            }

            // Add light's contribution to reflected radiance
            if (!Li.isBlack()) {
                if (isDeltaLight(light.flags))
                    Ld += f * Li / lightPdf;
                else {
                    Real weight = powerHeuristic(1, lightPdf, 1, scatteringPdf);
                    Ld += f * Li * weight / lightPdf;
                }
            }
        }
    }

    // Sample BSDF with multiple importance sampling
    if (!isDeltaLight(light.flags)) {
        Spectrum f;
        bool sampledSpecular = false;
        if (it.isSurfaceInteraction()) {
            // Sample scattered direction for surface interactions
            BxDFType sampledType;
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
            f = isect.bsdf->sample_f(isect.wo, &wi, uScattering, &scatteringPdf,
                                     bsdfFlags, &sampledType);
            f *= absDot(wi, isect.shadingGeom.n);
            sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
        }
//        else {
//            // Sample scattered direction for medium interactions
//            const MediumInteraction &mi = (const MediumInteraction &)it;
//            Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//            f = Spectrum(p);
//            scatteringPdf = p;
//        }

        if (!f.isBlack() && scatteringPdf > 0) {
            // Account for light contributions along sampled direction _wi_
            Real weight = 1;
            if (!sampledSpecular) {
                lightPdf = light.pdf_li(it, wi);
                if (lightPdf == 0) return Ld;
                weight = powerHeuristic(1, scatteringPdf, 1, lightPdf);
            }

            // Find intersection and compute transmittance
            SurfaceInteraction lightIsect;
            Ray ray = it.emitRay(wi);
            Spectrum Tr(1.f);
            bool foundSurfaceInteraction =
                handleMedia ? scene.intersectTr(ray, sampler, &lightIsect, &Tr)
                            : scene.intersect(ray, &lightIsect);

            // Add light contribution from material sampling
            Spectrum Li(0.f);
            if (foundSurfaceInteraction) {
                if (lightIsect.object->getAreaLight() == &light)
                    Li = lightIsect.le(-wi);
            } else {Li = light.le(ray); }
            if (!Li.isBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
        }
    }

    return Ld;
}

std::unique_ptr<Distribution1D> computeLightPowerDistribution(const Scene& scene) {
    if (scene.lights.empty()) return nullptr;
    std::vector<Real> lightPower;
    for (const auto &light : scene.lights)
        lightPower.push_back(light->power().getYConstant());

    return std::unique_ptr<Distribution1D>(
        new Distribution1D(&lightPower[0], lightPower.size()));
}

// SamplerIntegrator Method Definitions
void SamplerIntegrator::render(const Scene& scene) {
    preprocess(scene, *sampler);
    // Render image tiles in parallel

    // Compute number of tiles, _nTiles_, to use for parallel rendering
    Bounds2i sampleBounds = camera->film->getSampleBounds();
    Vector2i sampleExtent = sampleBounds.diagonal();

    const int tileSize = 16;
    Point2i nTiles((sampleExtent.x + tileSize - 1) / tileSize,
                   (sampleExtent.y + tileSize - 1) / tileSize);

    // Get access to the Progress reporter
    ProgressReporter* reporter = ProgressReporter::getInstance();
    const std::string token = reporter->startReport(nTiles.x * nTiles.y);

    {
        ParallelFor2D([&](Point2i tile) {
            // Render section of image corresponding to {tile}

            // Allocate {MemoryArena} for tile
            MemoryPool pool;

            // Get sampler instance for tile
            int seed = tile.y * nTiles.x + tile.x;
            std::unique_ptr<Sampler> tileSampler = sampler->clone(seed);

            // Compute sample bounds for tile
            int x0 = sampleBounds.pMin.x + tile.x * tileSize;
            int x1 = std::min(x0 + tileSize, sampleBounds.pMax.x);
            int y0 = sampleBounds.pMin.y + tile.y * tileSize;
            int y1 = std::min(y0 + tileSize, sampleBounds.pMax.y);
            Bounds2i tileBounds(Point2i(x0, y0), Point2i(x1, y1));

            // Get {FilmTile} for tile
            std::unique_ptr<FilmTile> filmTile = camera->film->getFilmTile(tileBounds);

            // Loop over pixels in tile to render them
            for (Point2i pixel : tileBounds) {
                {
                    tileSampler->startPixel(pixel);
                }

                // Do this check after the StartPixel() call; this keeps
                // the usage of RNG values from (most) Samplers that use
                // RNGs consistent, which improves reproducability /
                // debugging.
                if (!insideExclusive(pixel, pixelBounds))
                    continue;

                do {
                    // Initialize _CameraSample_ for current sample
                    CameraSample cameraSample = tileSampler->getCameraSample(pixel);

                    // Generate camera ray for current sample
                    Ray ray;
                    Real rayWeight = camera->generateRay(cameraSample, &ray);

                    // Evaluate radiance along camera ray
                    Spectrum L(0.f);
                    if (rayWeight > 0) L = li(ray, scene, *tileSampler, pool);

                    // Issue warning if unexpected radiance value returned
                    if (L.hasNaNs()) {
                        LOG_ERR_FMT(
                            "Not-a-number radiance value returned "
                            "for pixel (%d, %d), sample %d. Setting to black.",
                            pixel.x, pixel.y,
                            (int)tileSampler->currentSampleIndex());
                        L = Spectrum(0.f);
                    } else if (L.getYConstant() < -1e-5) {
                        LOG_ERR_FMT(
                            "Negative luminance value, %f, returned "
                            "for pixel (%d, %d), sample %d. Setting to black.",
                            L.getYConstant(), pixel.x, pixel.y,
                            (int)tileSampler->currentSampleIndex());
                        L = Spectrum(0.f);
                    } else if (std::isinf(L.getYConstant())) {
                          LOG_ERR_FMT(
                            "Infinite luminance value returned "
                            "for pixel (%d, %d), sample %d. Setting to black.",
                            pixel.x, pixel.y,
                            (int)tileSampler->currentSampleIndex());
                        L = Spectrum(0.f);
                    }

                    // Add camera ray's contribution to image
                    filmTile->addSample(cameraSample.pFilm, L, rayWeight);

                    // Free _MemoryArena_ memory from computing image sample
                    // value
                    pool.reset();
                } while (tileSampler->startNextSample());
            }

            // Merge image tile into _Film_
            camera->film->mergeFilmTile(std::move(filmTile));
            // Report update
            reporter->updateProgress(token);
        }, nTiles);
        reporter->endReport(token);
    }

    // Save final image after rendering
    camera->film->writeImage();
}

Spectrum SamplerIntegrator::specularReflect(const Ray& ray,
                                            const SurfaceInteraction& isect,
                                            const Scene& scene, Sampler& sampler,
                                            MemoryPool& pool, int depth) const {
    // Compute specular reflection direction _wi_ and BSDF value
    Vector3f wo = isect.wo, wi;
    Real pdf;
    BxDFType type = BxDFType(BSDF_REFLECTION | BSDF_SPECULAR);
    Spectrum f = isect.bsdf->sample_f(wo, &wi, sampler.getNextSample2D(), &pdf, type);

    // Return contribution of specular reflection
    const Normal3f &ns = isect.shadingGeom.n;
    if (pdf > 0.f && !f.isBlack() && absDot(wi, ns) != 0.f) {
        // Compute ray differential _rd_ for specular reflection
        Ray rd = isect.emitRay(wi);

// @todo: Uncomment once RayDifferentials are implemented
#if 0
        if (ray.hasDifferentials) {
            rd.hasDifferentials = true;
            rd.rxOrigin = isect.p + isect.dpdx;
            rd.ryOrigin = isect.p + isect.dpdy;
            // Compute differential reflected directions
            Normal3f dndx = isect.shading.dndu * isect.dudx +
                            isect.shading.dndv * isect.dvdx;
            Normal3f dndy = isect.shading.dndu * isect.dudy +
                            isect.shading.dndv * isect.dvdy;
            Vector3f dwodx = -ray.rxDirection - wo,
                     dwody = -ray.ryDirection - wo;
            Float dDNdx = Dot(dwodx, ns) + Dot(wo, dndx);
            Float dDNdy = Dot(dwody, ns) + Dot(wo, dndy);
            rd.rxDirection =
                wi - dwodx + 2.f * Vector3f(Dot(wo, ns) * dndx + dDNdx * ns);
            rd.ryDirection =
                wi - dwody + 2.f * Vector3f(Dot(wo, ns) * dndy + dDNdy * ns);
        }
#endif
        return f * li(rd, scene, sampler, pool, depth + 1) * absDot(wi, ns) / pdf;
    } else { return Spectrum(0.f); }
}

Spectrum SamplerIntegrator::specularTransmit(
    const Ray& ray, const SurfaceInteraction& isect,
    const Scene& scene, Sampler& sampler, MemoryPool& pool, int depth) const {
    Vector3f wo = isect.wo, wi;
    Real pdf;

    const Point3f& p = isect.p;
    const Normal3f& ns = isect.shadingGeom.n;
    const BSDF& bsdf = *isect.bsdf;
    Spectrum f = bsdf.sample_f(wo, &wi, sampler.getNextSample2D(), &pdf,
                               BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR));
    Spectrum L = Spectrum(0.f);
    if (pdf > 0.f && !f.isBlack() && absDot(wi, ns) != 0.f) {
        // Compute ray differential _rd_ for specular transmission
        Ray rd = isect.emitRay(wi);

// Uncomment once RayDifferential is implemented
#if 0
        if (ray.hasDifferentials) {
            rd.hasDifferentials = true;
            rd.rxOrigin = p + isect.dpdx;
            rd.ryOrigin = p + isect.dpdy;

            Float eta = bsdf.eta;
            Vector3f w = -wo;
            if (Dot(wo, ns) < 0) eta = 1.f / eta;

            Normal3f dndx = isect.shading.dndu * isect.dudx +
                            isect.shading.dndv * isect.dvdx;
            Normal3f dndy = isect.shading.dndu * isect.dudy +
                            isect.shading.dndv * isect.dvdy;

            Vector3f dwodx = -ray.rxDirection - wo,
                     dwody = -ray.ryDirection - wo;
            Float dDNdx = Dot(dwodx, ns) + Dot(wo, dndx);
            Float dDNdy = Dot(dwody, ns) + Dot(wo, dndy);

            Float mu = eta * Dot(w, ns) - Dot(wi, ns);
            Float dmudx =
                (eta - (eta * eta * Dot(w, ns)) / Dot(wi, ns)) * dDNdx;
            Float dmudy =
                (eta - (eta * eta * Dot(w, ns)) / Dot(wi, ns)) * dDNdy;

            rd.rxDirection =
                wi + eta * dwodx - Vector3f(mu * dndx + dmudx * ns);
            rd.ryDirection =
                wi + eta * dwody - Vector3f(mu * dndy + dmudy * ns);
        }
#endif
        L = f * li(rd, scene, sampler, pool, depth + 1) * absDot(wi, ns) / pdf;
    }

    return L;
}

}  // namespace phyr
