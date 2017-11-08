#include <core/scene.h>
#include <core/integrator/sampler.h>

namespace phyr {

// Scene Method Definitions
bool Scene::intersect(const Ray& ray, SurfaceInteraction* isect) const {
    ASSERT(ray.d != Vector3f(0,0,0));
    return aggregate->intersectRay(ray, isect);
}

bool Scene::intersectP(const Ray& ray) const {
    ASSERT(ray.d != Vector3f(0,0,0));
    return aggregate->intersectRay(ray);
}

bool Scene::intersectTr(Ray ray, Sampler& sampler, SurfaceInteraction* isect,
                        Spectrum* Tr) const {
    *Tr = Spectrum(1.f);

    while (true) {
        bool hitSurface = intersect(ray, isect);

        // Initialize next ray segment or terminate transmittance computation
        if (!hitSurface) return false;

        if (isect->object->getMaterial() != nullptr) return true;
        ray = isect->emitRay(ray.d);
    }
}

}  // namespace pbrt
