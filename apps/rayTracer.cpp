#include <fstream>
#include <iostream>
#include <chrono>
#include <random>

#include <time.h>
#include <cstdlib>

#include "Core/Camera.h"
#include "Core/Image.h"
#include "Objects/Scene.h"
#include "Objects/Sphere.h"
#include "Utils/ProgressBar.h"

using namespace std;

float map_y_to_zero_one(const Vec3& vec)
{
    Vec3 unitDirection = vec.normalize();
    return 0.5 * (unitDirection.y() + 1.0);
}

Vec3 randomUnitSphere() {
    Vec3 p;
    do {
        p = 2.0 * Vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX) - Vec3(1.0, 1.0, 1.0);
    } while (p.length_sq() >= 1.0);
    return p;
}

Vec3 color(const Ray& ray, const Scene& scene)
{
    auto [t, point, normal] = scene.testRay(ray, Interval(0.001, Interval::limit_max()));
    if (t > 0.0)
        return 0.5 * color(Ray(point, normal + randomUnitSphere()), scene);
        
    t = map_y_to_zero_one(ray.direction());
    Vec3 white(1.0, 1.0, 1.0);
    Vec3 lightBlue(0.5, 0.7, 1.0);
    return lerp(white, lightBlue, t);
}

void reportElapsedTime(ostream& output, chrono::steady_clock::time_point begin)
{
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    output.precision(3);
    output << "Complete in " << elapsed.count() * 1e-9 << " seconds\n";
}

int main() {
    srand((unsigned)time(0));
    int width = 200;
    int height = 100;
    int superSampling = 100;

    Camera camera;
    Image image(height, width);
    Scene scene;
    scene.addObject(make_unique<Sphere>(Vec3(0.0, 0.0, -1.0), 0.5f));
    scene.addObject(make_unique<Sphere>(Vec3(0.0, -100.5, -1.0), 100.0f));

    std::string outputFileName("diffuse_material.ppm");
    ofstream imageFile(outputFileName);
    imageFile << "P3\n" << width << " " << height << "\n255\n";

    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << "Rendering...\n";
    ProgressBar bar(height, 35);
    for (int i = 0; i < height; i++)
    {
        bar.displayNext(std::cout);

        for (int j = 0; j < width; j++)
        {
            Vec3 col;
            for (int s = 0; s < superSampling; s++)
            {
                float u = float(j + (float)rand() / RAND_MAX) / width;
                float v = float(i + (float)rand() / RAND_MAX) / height;
                col += color(camera.getRay(u, v), scene);
            }

            image(i, j) = col / superSampling;
        }
    }
    reportElapsedTime(cout, begin);

    std::cout << "Saving...\n";
    begin = std::chrono::high_resolution_clock::now();
    bar.reset();
    for (int i = 0; i < height; i++)
    {
        bar.displayNext(std::cout);

        for (int j = 0; j < width; j++)
        {
            auto col = image(i, j);
            int ir = int(255.99 * sqrt(col.r()));
            int ig = int(255.99 * sqrt(col.g()));
            int ib = int(255.99 * sqrt(col.b()));

            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    imageFile.close();

    reportElapsedTime(cout, begin);
    std::cout << "Image saved to " << outputFileName << "\n";
}
