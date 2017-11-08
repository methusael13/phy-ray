#include <core/scene.h>
#include <core/light/light.h>
#include <core/color/spectrum.h>

namespace phyr {

// Light definitions
Light::~Light() {}

Spectrum Light::le(const Ray& ray) const { return Spectrum(0); }

// VisibilityTester definitions
bool VisibilityTester::unoccluded(const Scene& scene) const {
    return !scene.intersectP(p0.emitRay(p1));
}

Spectrum VisibilityTester::tr(const Scene& scene, Sampler& sampler) const {
    Ray ray(p0.emitRay(p1));
    // Assume that medium doesn't exist
    Spectrum Tr(1.f);

    while (true) {
        SurfaceInteraction isect;
        bool hitSurface = scene.intersect(ray, &isect);
        // Handle opaque surface along ray's path
        if (hitSurface && isect.object->getMaterial() != nullptr)
            return Spectrum(0.0f);

        // @todo: Support medium
        // Update transmittance for current ray segment
        // if (ray.medium) Tr *= ray.medium->Tr(ray, sampler);

        // Generate next ray segment or return final transmittance
        if (!hitSurface) break;
        ray = isect.emitRay(p1);
    }

    return Tr;
}

}  // namespace phyr
