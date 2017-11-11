#include <core/scene.h>
#include <modules/integrators/path.h>

/*
 * Code adapted from pbrt-v3 for use with PhyRay
 */

namespace phyr {

// PathIntegrator Method Definitions
PathIntegrator::PathIntegrator(int maxDepth, std::shared_ptr<const Camera> camera,
                               std::shared_ptr<Sampler> sampler,
                               const Bounds2i& pixelBounds, Real rrThreshold,
                               const std::string& lightSampleStrategy)
    : SamplerIntegrator(camera, sampler, pixelBounds),
      maxDepth(maxDepth), rrThreshold(rrThreshold),
      lightSampleStrategy(lightSampleStrategy) {}

void PathIntegrator::preprocess(const Scene& scene, Sampler& sampler) {
    lightDistribution = createLightSampleDistribution(lightSampleStrategy, scene);
}

Spectrum PathIntegrator::li(const Ray& r, const Scene& scene,
                            Sampler& sampler, MemoryPool& pool, int depth) const {
    Spectrum L(0); Spectrum beta(1.f);
    Ray ray(r);

    bool specularBounce = false;
    int bounces;

    // {etaScale} tracks the accumulated effect
    // of radiance scaling due to rays passing through refractive
    // boundaries. This value is tracked in order to remove it from beta
    // when Russian roulette is applied; this is worthwhile, since it helps
    // avoiding terminating refracted rays that are about to be refracted back
    // out of a medium and thus have their beta value increased.
    Real etaScale = 1;

    for (bounces = 0; ; ++bounces) {
        // Find next path vertex and accumulate contribution

        // Intersect _ray_ with scene and store intersection in _isect_
        SurfaceInteraction isect;
        bool foundIntersection = scene.intersect(ray, &isect);

        // Possibly add emitted light at intersection
        if (bounces == 0 || specularBounce) {
            // Add emitted light at path vertex or from the environment
            if (foundIntersection) {
                L += beta * isect.le(-ray.d);
            } else {
                for (const auto& light : scene.infiniteLights)
                    L += beta * light->le(ray);
            }
        }

        // Terminate path if ray escaped or _maxDepth_ was reached
        if (!foundIntersection || bounces >= maxDepth) break;

        // Compute scattering functions and skip over medium boundaries
        isect.computeScatteringFunctions(ray, pool, true);
        if (!isect.bsdf) {
            ray = isect.emitRay(ray.d);
            bounces--;
            continue;
        }

        const Distribution1D* distrib = lightDistribution->lookup(isect.p);

        // Sample illumination from lights to find path contribution.
        // (But skip this for perfectly specular BSDFs.)
        if (isect.bsdf->numComponents(BxDFType(BSDF_ALL & ~BSDF_SPECULAR)) > 0) {
            Spectrum Ld = beta * uniformSampleOneLight(isect, scene, pool,
                                                       sampler, false, distrib);
            ASSERT(Ld.getYConstant() >= 0.f);
            L += Ld;
        }

        // Sample BSDF to get new path direction
        Vector3f wo = -ray.d, wi; Real pdf;
        BxDFType flags;
        Spectrum f = isect.bsdf->sample_f(wo, &wi, sampler.getNextSample2D(), &pdf, BSDF_ALL, &flags);

        if (f.isBlack() || pdf == 0.f) break;
        beta *= f * absDot(wi, isect.shadingGeom.n) / pdf;

        ASSERT(beta.getYConstant() >= 0.f);
        ASSERT(!std::isinf(beta.getYConstant()));

        specularBounce = (flags & BSDF_SPECULAR) != 0;
        if ((flags & BSDF_SPECULAR) && (flags & BSDF_TRANSMISSION)) {
            Real eta = isect.bsdf->eta;
            // Update the term that tracks radiance scaling for refraction
            // depending on whether the ray is entering or leaving the
            // medium.
            etaScale *= (dot(wo, isect.n) > 0) ? (eta * eta) : 1 / (eta * eta);
        }
        ray = isect.emitRay(wi);

        // @todo: Account for subsurface scattering, if applicable

        // Possibly terminate the path with Russian roulette.
        // Factor out radiance scaling due to refraction in rrBeta.
        Spectrum rrBeta = beta * etaScale;
        if (rrBeta.maxComponentValue() < rrThreshold && bounces > 3) {
            Real q = std::max((Real).05, 1 - rrBeta.maxComponentValue());
            if (sampler.getNextSample1D() < q) break;
            beta /= 1 - q;
            ASSERT(!std::isinf(beta.getYConstant()));
        }
    }

    return L;
}

PathIntegrator* createPathIntegrator(std::shared_ptr<Sampler> sampler,
                                     std::shared_ptr<const Camera> camera,
                                     int maxDepth) {
    Bounds2i _pixelBounds = camera->film->getSampleBounds();

    if (_pixelBounds.area() == 0) {
        LOG_ERR("Degenerate pixel bounds specified");
        return nullptr;
    }

    return new PathIntegrator(maxDepth, camera, sampler, _pixelBounds, 1, "spatial");
}

}  // namespace phyr
