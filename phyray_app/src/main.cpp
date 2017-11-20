#include <iostream>

#include <core/phyr_api.h>
#include <core/configparser.h>

int main(int argc, const char* argv[]) {
    // Get filename from command line args
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];

    using namespace phyr;
    LOG_INFO("Initiating Phyray...");

    Spectrum::init();
    parallelInit();

    LOG_INFO("Constructing scene...\n");

    // Create textures and material
    std::shared_ptr<Material> metal(createMetalMaterial(MetalType::Silver, 0.04));

    // Color definitions
    Real yellow[3] = { 0.85, 0.85, 0 };
    Real red[3] = { 1, 0, 0 }, black[3] = { 0, 0, 0 };
    Real white[3] = { 1, 1, 1 };

    std::shared_ptr<Material> whitePlastic(createPlasticMaterial(white, 0.1, 0));
    std::shared_ptr<Material> redPlastic(createPlasticMaterial(red, 0.1, 0));
    std::shared_ptr<Material> blackPlastic(createPlasticMaterial(black, 0.1, 0));
    std::shared_ptr<Material> yellowPlastic(createPlasticMaterial(yellow, 0.1, 0));

    // Matte material
    std::shared_ptr<Material> floorMatte(createMatteMaterial());

    // Glass material
    std::shared_ptr<Material> glass(createGlassMaterial());

    // Create transforms
    Transform transSp0 = Transform::translate(Vector3f(-3, -1, 8));
    Transform invTransSp0 = Transform::inverse(transSp0);
    Transform transSp1 = Transform::translate(Vector3f(3, -1, 8));
    Transform invTransSp1 = Transform::inverse(transSp1);
    Transform transSp2 = Transform::translate(Vector3f(0, -1, 8));
    Transform invTransSp2 = Transform::inverse(transSp2);

    Transform transDiskLight = Transform::translate(Vector3f(0, 3.2, 8)) * Transform::rotateX(90);
    Transform invTransDiskLight = Transform::inverse(transDiskLight);
    Transform transFloor = Transform::translate(Vector3f(0, -2, 8)) * Transform::rotateX(90);
    Transform invTransFloor = Transform::inverse(transFloor);

    Transform identity;

    // Create shapes
    std::shared_ptr<Shape> shapeSphere0 = createSphereShape(&transSp0, &invTransSp0, false, 1);
    std::shared_ptr<Shape> shapeSphere1 = createSphereShape(&transSp1, &invTransSp1, false, 1);
    std::shared_ptr<Shape> shapeSphere2 = createSphereShape(&transSp2, &invTransSp2, false, 1);
    std::shared_ptr<Shape> shapeDisk1 = createDiskShape(&transFloor, &invTransFloor, 0, 15);
    std::shared_ptr<Shape> shapeDisk0 = createDiskShape(&transDiskLight, &invTransDiskLight, 0, 2);

    // The skydome to simulate environment lighting
    std::shared_ptr<Shape> shapeSkyDome = createSphereShape(&identity, &identity, true, 30);

    // Create an Area light
    Spectrum lightSpec = Spectrum::getFromRGB(white, SpectrumType::Illuminant);
    std::shared_ptr<AreaLight> domeLight =
            std::make_shared<DiffuseAreaLight>(identity, Spectrum(0.7) * lightSpec, 20, shapeSkyDome);
    std::shared_ptr<AreaLight> diskLight =
            std::make_shared<DiffuseAreaLight>(transDiskLight, Spectrum(10) * lightSpec, 20, shapeDisk0, true);

    // Construct a list of lights
    std::vector<std::shared_ptr<Light>> sceneLights;
    sceneLights.push_back(domeLight);
    sceneLights.push_back(diskLight);

    // Create Geometry objects
    std::shared_ptr<Object> objSphere0(new GeometricObject(shapeSphere0, blackPlastic, nullptr));
    std::shared_ptr<Object> objSphere1(new GeometricObject(shapeSphere1, blackPlastic, nullptr));
    std::shared_ptr<Object> objSphere2(new GeometricObject(shapeSphere2, redPlastic, nullptr));
    std::shared_ptr<Object> objDisk1(new GeometricObject(shapeDisk1, floorMatte, nullptr));

    std::shared_ptr<Object> objDome(new GeometricObject(shapeSkyDome, nullptr, domeLight));
    std::shared_ptr<Object> objDiskLight(new GeometricObject(shapeDisk0, nullptr, diskLight));

    // Create a list of the scene objects
    std::vector<std::shared_ptr<Object>> sceneObjects;
    sceneObjects.push_back(objSphere0); sceneObjects.push_back(objSphere1);
    sceneObjects.push_back(objDisk1); sceneObjects.push_back(objSphere2);
    sceneObjects.push_back(objDome); sceneObjects.push_back(objDiskLight);

    // Create the Accel structure
    std::shared_ptr<AccelBVH> accel = createBVHAccel(sceneObjects, 2);

    // Create the scene
    Scene scene(accel, sceneLights);

    // Get render settings
    bool useConfig = true;
    RenderConfig config; ConfigArgsList args;
    try {
        useConfig = config.parseConfig("phyray_app/config/render.conf");
    } catch (UnsupportedConfigException& ex1) {
        LOG_ERR_FMT("%s", ex1.what()); parallelCleanup();
        return 1;
    } catch (MalformedConfigException& ex2) {
        LOG_ERR_FMT("%s", ex2.what()); parallelCleanup();
        return 2;
    }

    if (!useConfig) LOG_WARNING("Using default render settings");

    int resx = 640, resy = 400;
    if (useConfig && config.getConfigArgs("resolution", &args)) {
        resx = args.getParam<int>(0).value;
        resy = args.getParam<int>(1).value;
    }

    // Create film
    const Real oneOverThree = 1. / 3.;
    std::unique_ptr<Filter> filter(new MitchellFilter(Vector2f(4, 4), oneOverThree, oneOverThree));
    Film* film = new Film(Point2i(resx, resy), Bounds2f(Point2f(0, 0), Point2f(1, 1)),
                          std::move(filter), 35., filename, 20.);

    // Create camera
    Transform camLook = Transform::lookAt(Point3f(0, 0, 0), Point3f(0, 0, 8), Vector3f(0, 1, 0));
    std::shared_ptr<const Camera> camera(createPerspectiveCamera(camLook, film, 0, 1e6, 45));

    // Create Sampler and Integrator
    int maxBounces = 6, nSamples = 6;
    if (useConfig && config.getConfigArgs("bounces", &args))
        maxBounces = args.getParam<int>(0).value;

    if (useConfig && config.getConfigArgs("samples", &args))
        nSamples = args.getParam<int>(0).value;

    std::shared_ptr<Sampler> sampler(createStratifiedSampler(true, nSamples, nSamples, 10));
    std::unique_ptr<Integrator> integrator(createPathIntegrator(sampler, camera, maxBounces));

    LOG_INFO("Done constructing scene.");

    std::cout << formatString(
                     "\nRendering: \x1b[37;1m[%d samples] [%d max-bounces]\x1b[0m\n",
                     sampler->samplesPerPixel, maxBounces) << std::endl;

    integrator->render(scene);

    std::cout << "Done rendering\n";
    std::cout << "Saved rendered image to file: " << film->filename << ".exr\n" << std::endl;

    parallelCleanup();
    delete film;
    return 0;
}
