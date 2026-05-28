#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
    // world
    hittable_list world;

    for (int a = -3; a < 3; a++) {
        for (int b = -3; b < 3; b++) {
            if (random_double() > 0.68)
                continue;

            auto choose_mat = random_double();
            point3 center(a + 0.6 * random_double(), 0.2, b + 0.6 * random_double());

            if ((center - point3(0, 0.8, 0)).length() > 1) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.68) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.9) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // auto material_center = make_shared<dielectric>(1.5);
    // auto material_left   = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    // auto material_right  = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);

    world.add(make_shared<sphere>(point3(0, -100.5, 0), 100.5, material_ground));
    // world.add(make_shared<sphere>(point3(0.0, 0.25, -1.0), 0.5, material_center));
    // world.add(make_shared<sphere>(point3(-2.0, 0.25, -1.0), 0.5, material_left));
    // world.add(make_shared<sphere>(point3(2.0, 0.25, -1.0), 0.5, material_right));

    auto difflight = make_shared<diffuse_light>(color(4, 4, 0.5));
    world.add(make_shared<sphere>(point3(0, 0.5, 0), 0.5, difflight));

    // camera
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 1;
    cam.max_depth         = 50;
    cam.background        = color(0.9, 0.9, 0.9);

    cam.vfov              = 20;
    cam.lookfrom          = point3(10, 2, 3);
    cam.lookat            = point3(0, 1, -0.2);
    cam.vup               = vec3(0, 5, 0);

    cam.defocus_angle     = 0.6;
    cam.focus_dist        = 10.0;

    cam.render(world);
}