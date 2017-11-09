#include <iostream>

#include <core/phyr_api.h>
#include <modules/materials/matte.h>

int main(int argc, const char* argv[]) {
    using namespace phyr;
    LOG_INFO("Initiating Phyray...");

    Spectrum::init();
    parallelInit();

    LOG_INFO("Constructing scene...\n");
    // Create textures and material
    std::shared_ptr<Material> metal(createMetalMaterial());
    std::shared_ptr<Material> plastic(createPlasticMaterial());

    // Create transforms for two spheres
    Transform transRight = Transform::translate(Vector3f(0, 0, 10));
    Transform invTransRight = Transform::inverse(transRight);
    Transform transLeft = Transform::translate(Vector3f(-5, 0, 10));
    Transform invTransLeft = Transform::inverse(transLeft);

    // Create a Sphere
    std::shared_ptr<Shape> sphere0 = createSphereShape(&transRight, &invTransRight, false, 2);
    std::shared_ptr<Shape> sphere1 = createSphereShape(&transLeft, &invTransLeft, false, 1);

    // Create an Area light
    // std::shared_ptr<AreaLight> light = createDiffuseAreaLight(transLeft, sphere1, 1, 1, 4);

    // std::shared_ptr<Light> light = createPointLight(Transform(), 30, 5, Point3f(0, 0, -2));
    std::shared_ptr<Light> light = createDistantLight(Transform());
    // Construct a list of lights
    std::vector<std::shared_ptr<Light>> sceneLights;
    sceneLights.push_back(light);

    // Create Geometry objects
    std::shared_ptr<Object> sphere0Obj(new GeometricObject(sphere0, metal, nullptr));
    std::shared_ptr<Object> sphere1Obj(new GeometricObject(sphere1, plastic, nullptr));

    // Create a list of the scene objects
    std::vector<std::shared_ptr<Object>> sceneObjects;
    sceneObjects.push_back(sphere0Obj);
    // sceneObjects.push_back(sphere1Obj);

    // Create the Accel structure
    std::shared_ptr<AccelBVH> accel = std::make_shared<AccelBVH>(sceneObjects, 4);

    // Create the scene
    Scene scene(accel, sceneLights);

    // Create film
    const Real oneOverThree = 1. / 3.;
    std::unique_ptr<Filter> filter(new LanczosSincFilter(Vector2f(4, 4), 3));
    Film* film = new Film(Point2i(640, 480),
                          Bounds2f(Point2f(0, 0), Point2f(1, 1)),
                          std::move(filter), 35., "render01", 1);

    // Create camera
    Transform camLook = Transform::lookAt(Point3f(0, 0, -5),
                                          Point3f(0, 0, 1), Vector3f(0, 1, 0));
    Bounds2f screen;
    Real aspect = Real(film->resolution.x) / film->resolution.y;
    if (aspect > 1) {
        // Landscape
        screen.pMin.x = -aspect; screen.pMax.x = aspect;
        screen.pMin.y = -1.0f; screen.pMax.y = 1.0f;
    } else {
        screen.pMin.x = -1.0f; screen.pMax.x = 1.0f;
        screen.pMin.y = -1.0f / aspect; screen.pMax.y = 1.0f / aspect;
    }

    std::shared_ptr<const Camera> camera(
                new PerspectiveCamera(camLook, screen, 2.f, 1e6, 60., film));

    // Create Sampler and Integrator
    std::shared_ptr<Sampler> sampler(createStratifiedSampler(true, 20, 20));
    Integrator* integrator = createPathIntegrator(sampler, camera);

    LOG_INFO("Done constructing scene.\nRendering...");
    integrator->render(scene);

    LOG_INFO("Done rendering\n");
    parallelCleanup();

    delete film;
    return 0;
}
