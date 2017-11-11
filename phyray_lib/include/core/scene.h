#ifndef PHYRAY_CORE_SCENE_H
#define PHYRAY_CORE_SCENE_H

#include <core/phyr.h>
#include <core/geometry/geometry.h>
#include <core/object/object.h>
#include <core/light/light.h>
#include <core/color/spectrum.h>

#include <memory>

namespace phyr {

// Scene Declarations
class Scene {
  public:
    Scene(std::shared_ptr<Object> aggregate,
          const std::vector<std::shared_ptr<Light>>& lights) :
        lights(lights), aggregate(aggregate) {
        worldBounds = aggregate->worldBounds();

        // Preprocess lights
        for (const auto& light : lights) {
            light->preprocess(*this);
            if (light->flags & (int)LightFlags::INFINITE)
                infiniteLights.push_back(light);
        }
    }

    // Interface
    const Bounds3f& getWorldBounds() const { return worldBounds; }
    bool intersect(const Ray& ray, SurfaceInteraction* isect) const;
    bool intersectP(const Ray& ray) const;
    bool intersectTr(Ray ray, Sampler& sampler, SurfaceInteraction* isect,
                     Spectrum* transmittance) const;

    std::vector<std::shared_ptr<Light>> lights;
    // Store infinite light sources separately for cases where we only want
    // to loop over them.
    std::vector<std::shared_ptr<Light>> infiniteLights;

  private:
    std::shared_ptr<Object> aggregate;
    Bounds3f worldBounds;
};

}  // namespace phyr

#endif
