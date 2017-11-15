#include <iostream>

#include <core/phyr_api.h>
#include <modules/materials/matte.h>

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

    // Color definitions
    Real yellow[3] = { 0.85, 0.85, 0 };
    Real red[3] = { 1, 0, 0 }, black[3] = { 0, 0, 0 };
    Real white[3] = { 1, 1, 1 }, green[3] = { 0, 1, 0 };
    Real blue[3] = { 0, 0, 1 };

    // ------------------------ Material definitions ----------------------------

    // Plastic materials
    std::shared_ptr<Material> whitePlastic(createPlasticMaterial(white, 0.25, 0));
    std::shared_ptr<Material> redPlastic(createPlasticMaterial(red, 0.25, 0));
    std::shared_ptr<Material> blackPlastic(createPlasticMaterial(black, 0.5, 0));
    std::shared_ptr<Material> yellowPlastic(createPlasticMaterial(yellow, 0.5, 0));
    std::shared_ptr<Material> bluePlastic(createPlasticMaterial(blue, 0.5, 0));

    // Matte materials
    std::shared_ptr<Material> whiteMatte(createMatteMaterial(white));
    std::shared_ptr<Material> redMatte(createMatteMaterial(red));
    std::shared_ptr<Material> yellowMatte(createMatteMaterial(yellow));
    std::shared_ptr<Material> greenMatte(createMatteMaterial(green));

    // Glass / Mirror material
    std::shared_ptr<Material> glass(createGlassMaterial());
    std::shared_ptr<Material> mirror(createMirrorMaterial());

    // Metal materials
    std::shared_ptr<Material> metal(createMetalMaterial(MetalType::Silver, 0.2));

    // ----------------------- Transform definitions -----------------------------

    // An arbitrarily large radius to simulate flat planes
    Real rad = 1e5;
    Real xext = 3, yext = 4, zext = 10;

    // Object transforms
    Transform cx0 = Transform::translate(Vector3f(-rad - xext, 0, 0));
    Transform icx0 = Transform::inverse(cx0);
    Transform cx1 = Transform::translate(Vector3f(rad + xext, 0, 0));
    Transform icx1 = Transform::inverse(cx1);
    Transform cy0 = Transform::translate(Vector3f(0, rad + yext, 0));
    Transform icy0 = Transform::inverse(cy0);
    Transform cy1 = Transform::translate(Vector3f(0, -rad, 0));
    Transform icy1 = Transform::inverse(cy1);
    Transform cz0 = Transform::translate(Vector3f(0, 0, rad + zext));
    Transform icz0 = Transform::inverse(cz0);
    Transform cz1 = Transform::translate(Vector3f(0, 0, -rad - zext));
    Transform icz1 = Transform::inverse(cz1);
    
    // Room objects
    Transform sp0 = Transform::translate(Vector3f(xext - 1, 1, zext - 1));
    Transform isp0 = Transform::inverse(sp0);
    Transform sp1 = Transform::translate(Vector3f(-xext * 0.5, 0.5, zext - 2));
    Transform isp1 = Transform::inverse(sp1);
    Transform sp2 = Transform::translate(Vector3f(0, 0.75, zext - 3));
    Transform isp2 = Transform::inverse(sp2);
    Transform sp3 = Transform::translate(Vector3f(-xext + 0.8, 2, zext - 4));
    Transform isp3 = Transform::inverse(sp3);
    Transform sp4 = Transform::translate(Vector3f(1, 0.6, zext - 4));
    Transform isp4 = Transform::inverse(sp4);

    // Light transforms (5 disk lights)
    Real dy = yext - 0.02;
    Transform dfl = Transform::translate(Vector3f(-xext * 0.5, dy, zext * 0.9)) *
                    Transform::rotateX(90);
    Transform idfl = Transform::inverse(dfl);
    Transform dfr = Transform::translate(Vector3f(xext * 0.5, dy, zext * 0.9)) *
                    Transform::rotateX(90);
    Transform idfr = Transform::inverse(dfr);
    Transform dbl = Transform::translate(Vector3f(-xext * 0.5, dy, zext * 0.1)) *
                    Transform::rotateX(90);
    Transform idbl = Transform::inverse(dbl);
    Transform dbr = Transform::translate(Vector3f(xext * 0.5, dy, zext * 0.1)) *
                    Transform::rotateX(90);
    Transform idbr = Transform::inverse(dbr);
    Transform dcc = Transform::translate(Vector3f(0, dy, zext * 0.5)) *
                    Transform::rotateX(90);
    Transform idcc = Transform::inverse(dcc);

    // Create shapes
    std::shared_ptr<Shape> wallL = createSphereShape(&cx0, &icx0, false, rad);
    std::shared_ptr<Shape> wallR = createSphereShape(&cx1, &icx1, false, rad);
    std::shared_ptr<Shape> wallU = createSphereShape(&cy0, &icy0, false, rad);
    std::shared_ptr<Shape> wallD = createSphereShape(&cy1, &icy1, false, rad);
    std::shared_ptr<Shape> wallF = createSphereShape(&cz0, &icz0, false, rad);
    std::shared_ptr<Shape> wallB = createSphereShape(&cz1, &icz1, false, rad);
    
    std::shared_ptr<Shape> ball0 = createSphereShape(&sp0, &isp0, false, 1);
    std::shared_ptr<Shape> ball1 = createSphereShape(&sp1, &isp1, false, 0.5);
    std::shared_ptr<Shape> ball2 = createSphereShape(&sp2, &isp2, false, 0.75);
    std::shared_ptr<Shape> ball3 = createSphereShape(&sp3, &isp3, false, 0.75);
    std::shared_ptr<Shape> ball4 = createSphereShape(&sp4, &isp4, false, 0.6);

    // The disk light for the room
    std::shared_ptr<Shape> lfl = createDiskShape(&dfl, &idfl, 0.1, 0.5);
    std::shared_ptr<Shape> lfr = createDiskShape(&dfr, &idfr, 0.1, 0.5);
    std::shared_ptr<Shape> lbl = createDiskShape(&dbl, &idbl, 0.1, 0.5);
    std::shared_ptr<Shape> lbr = createDiskShape(&dbr, &idbr, 0.1, 0.5);
    std::shared_ptr<Shape> lcc = createDiskShape(&dcc, &idcc, 0.1, 1.5);

    // Create an Area light
    Real intensity = 10;
    Spectrum lightSpec = Spectrum::getFromRGB(white, SpectrumType::Illuminant);
    std::shared_ptr<AreaLight> lightFL =
            std::make_shared<DiffuseAreaLight>(dfl, Spectrum(intensity) * lightSpec, 20, lfl, true);
    std::shared_ptr<AreaLight> lightFR =
            std::make_shared<DiffuseAreaLight>(dfr, Spectrum(intensity) * lightSpec, 20, lfr, true);
    std::shared_ptr<AreaLight> lightBL =
            std::make_shared<DiffuseAreaLight>(dbl, Spectrum(intensity) * lightSpec, 20, lbl, true);
    std::shared_ptr<AreaLight> lightBR =
            std::make_shared<DiffuseAreaLight>(dbr, Spectrum(intensity) * lightSpec, 20, lbr, true);
    std::shared_ptr<AreaLight> lightCC =
            std::make_shared<DiffuseAreaLight>(dcc, Spectrum(intensity - 5) * lightSpec, 20, lcc, true);
            
    // Construct a list of lights
    std::vector<std::shared_ptr<Light>> sceneLights;
    sceneLights.push_back(lightFL); sceneLights.push_back(lightFR);
    sceneLights.push_back(lightBL); sceneLights.push_back(lightBR);
    sceneLights.push_back(lightCC);

    // Geometry for scene components
    std::shared_ptr<Object> objWallL(new GeometricObject(wallL, redMatte, nullptr));
    std::shared_ptr<Object> objWallR(new GeometricObject(wallR, greenMatte, nullptr));
    std::shared_ptr<Object> objWallU(new GeometricObject(wallU, whiteMatte, nullptr));
    std::shared_ptr<Object> objWallD(new GeometricObject(wallD, whiteMatte, nullptr));
    std::shared_ptr<Object> objWallF(new GeometricObject(wallF, whiteMatte, nullptr));
    std::shared_ptr<Object> objWallB(new GeometricObject(wallB, whiteMatte, nullptr));
    
    // Geometry for room components
    std::shared_ptr<Object> objSphere0(new GeometricObject(ball0, whiteMatte, nullptr));
    std::shared_ptr<Object> objSphere1(new GeometricObject(ball1, metal, nullptr));
    std::shared_ptr<Object> objSphere2(new GeometricObject(ball2, bluePlastic, nullptr));
    std::shared_ptr<Object> objSphere3(new GeometricObject(ball3, glass, nullptr));
    std::shared_ptr<Object> objSphere4(new GeometricObject(ball4, mirror, nullptr));

    // Geometry for disk area lights
    std::shared_ptr<Object> objLightFL(new GeometricObject(lfl, nullptr, lightFL));
    std::shared_ptr<Object> objLightFR(new GeometricObject(lfr, nullptr, lightFR));
    std::shared_ptr<Object> objLightBL(new GeometricObject(lbl, nullptr, lightBL));
    std::shared_ptr<Object> objLightBR(new GeometricObject(lbr, nullptr, lightBR));
    std::shared_ptr<Object> objLightCC(new GeometricObject(lcc, nullptr, lightCC));

    // Create a list of the scene objects
    std::vector<std::shared_ptr<Object>> sceneObjects;
    sceneObjects.push_back(objWallL); sceneObjects.push_back(objWallR);
    sceneObjects.push_back(objWallU); sceneObjects.push_back(objWallD);
    sceneObjects.push_back(objWallF); sceneObjects.push_back(objWallB);
    sceneObjects.push_back(objSphere0); sceneObjects.push_back(objSphere1);
    sceneObjects.push_back(objSphere2); sceneObjects.push_back(objSphere3);
    sceneObjects.push_back(objSphere4);

    sceneObjects.push_back(objLightFL); sceneObjects.push_back(objLightFR);
    sceneObjects.push_back(objLightBL); sceneObjects.push_back(objLightBR);
    sceneObjects.push_back(objLightCC);

    // Create the Accel structure
    std::shared_ptr<AccelBVH> accel = createBVHAccel(sceneObjects, 2);

    // Create the scene
    Scene scene(accel, sceneLights);

    // Create film
    const Real oneOverThree = 1. / 3.;
    std::unique_ptr<Filter> filter(new MitchellFilter(Vector2f(3, 3), oneOverThree, oneOverThree));
    Film* film = new Film(Point2i(640, 400), Bounds2f(Point2f(0, 0), Point2f(1, 1)),
                          std::move(filter), 35., filename, 20.);

    // Create camera
    Transform camLook = Transform::lookAt(Point3f(0, yext * 0.5, 0),
                                          Point3f(0, yext * 0.5, 1), Vector3f(0, 1, 0));
    std::shared_ptr<const Camera> camera(createPerspectiveCamera(camLook, film, 0, 1e6, 45));

    // Create Sampler and Integrator
    int maxBounces = 10;
    std::shared_ptr<Sampler> sampler(createStratifiedSampler(true, 50, 50, 20));
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
