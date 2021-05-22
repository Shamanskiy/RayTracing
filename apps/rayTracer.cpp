#include <iostream>

#include "Objects/Scene.h"
#include "Objects/Sphere.h"
#include "Core/Camera.h"
#include "Core/Image.h"
#include "Utils/Timer.h"

using namespace std;

Scene makeScene()
{
    Scene scene;
    scene.addObject(make_unique<Sphere>(Vec3(0.0, 0.0, -1.0), 0.5f));
    scene.addObject(make_unique<Sphere>(Vec3(0.0, -100.5, -1.0), 100.0f));
    return scene;
}

ImageSettings getImageSettings()
{
    ImageSettings settings;
    settings.width = 400;
    settings.height = 200;
    settings.antialiasing = 100;
    return settings;
}

Camera makeCamera()
{
    return Camera();
}

Image renderScene(const Camera& camera, const ImageSettings& settings, const Scene& scene)
{
    cout << "Rendering...\n";
    Timer timer;

    Image image = camera.render(scene, settings, Verbosity::all);

    cout << timer.reportElapsedTime();
    return image;
}

void saveImage(const Image& image)
{
    string outputFileName("diffuse_material.ppm");
    cout << "Saving...\n";
    Timer timer;

    image.save(outputFileName, Verbosity::all);

    cout << timer.reportElapsedTime();
    cout << "Image saved to " << outputFileName << "\n";
}

int main() {
    auto scene = makeScene();
    auto settings = getImageSettings();
    auto camera = makeCamera();

    auto image = renderScene(camera, settings, scene);

    saveImage(image);
}

