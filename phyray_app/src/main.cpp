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
    Transform transRight = Transform::translate(Vector3f(3, 0, 10));
    Transform invTransRight = Transform::inverse(transRight);
    Transform transLeft = Transform::translate(Vector3f(-3, 0, 10));
    Transform invTransLeft = Transform::inverse(transLeft);
    Transform lightTrans = Transform::translate(Vector3f(0, 0, 10));
    Transform invLightTrans = Transform::inverse(lightTrans);

    // Create a Sphere
    std::shared_ptr<Shape> sphere0 = createSphereShape(&transRight, &invTransRight, false, 1);
    std::shared_ptr<Shape> sphere1 = createSphereShape(&transLeft, &invTransLeft, false, 1);
    std::shared_ptr<Shape> sphere2 = createSphereShape(&lightTrans, &invLightTrans, false, 0.5);

    // Create an Area light
    // Real white[3] = { 1, 1, 1 };
    // Spectrum lightSpec = Spectrum::getFromRGB(white, SpectrumType::Illuminant);
    // std::shared_ptr<AreaLight> light =
    //        std::make_shared<DiffuseAreaLight>(lightTrans, lightSpec, 5, sphere2);

    // std::shared_ptr<Light> light = createPointLight(Transform(), 30, 5, Point3f(0, 0, -2));
    std::shared_ptr<Light> light = createDistantLight();
    // Construct a list of lights
    std::vector<std::shared_ptr<Light>> sceneLights;
    sceneLights.push_back(light);

    // Create Geometry objects
    std::shared_ptr<Object> sphere0Obj(new GeometricObject(sphere0, metal, nullptr));
    std::shared_ptr<Object> sphere1Obj(new GeometricObject(sphere1, plastic, nullptr));
    // std::shared_ptr<Object> lightObj(new GeometricObject(sphere2, nullptr, light));

    // Create a list of the scene objects
    std::vector<std::shared_ptr<Object>> sceneObjects;
    sceneObjects.push_back(sphere0Obj);
    sceneObjects.push_back(sphere1Obj);
    // sceneObjects.push_back(lightObj);

    // Create the Accel structure
    std::shared_ptr<AccelBVH> accel = std::make_shared<AccelBVH>(sceneObjects, 4);

    // Create the scene
    Scene scene(accel, sceneLights);

    // Create film
    const Real oneOverThree = 1. / 3.;
    std::unique_ptr<Filter> filter(new MitchellFilter(Vector2f(1, 1), Real(1)/3, Real(1)/3));
    Film* film = new Film(Point2i(480, 360),
                          Bounds2f(Point2f(0, 0), Point2f(1, 1)),
                          std::move(filter), 35., "render02", 50.);

    // Create camera
    Transform camLook = Transform::lookAt(Point3f(0, 0, 0), Point3f(0, 0, 1), Vector3f(0, 1, 0));
    std::shared_ptr<const Camera> camera(createPerspectiveCamera(camLook, film, 0, 1e6, 45));

    // Create Sampler and Integrator
    std::shared_ptr<Sampler> sampler(createStratifiedSampler(true, 10, 10));
    Integrator* integrator = createPathIntegrator(sampler, camera, 5);

    LOG_INFO("Done constructing scene.\nRendering...");
    integrator->render(scene);

    LOG_INFO("Done rendering\n");
    parallelCleanup();

    delete film;
    return 0;
}
