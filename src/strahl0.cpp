#include <iostream>
#include <cfloat>
#include "strahl0.hpp"

vec3 gamma_correct(vec3 const &v)
{
    vec3::type p = 1.0f/2.2f;
    return vec3(
        powf(v.x(), p),
        powf(v.y(), p),
        powf(v.z(), p)
    );
}

color compute_color(ray const &r, hitable *world, int depth)
{
    hit_record rec = {};
    if (world->hit(r, 0.0001f, FLT_MAX, rec))
    {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr == 0)
            std::cerr << "SCHEISSE!";
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * compute_color(scattered, world, depth+1);
        }
        else
            return color(0,0,0);
    }
    
    vec3 unit = unit_vector(r.direction);
    float t = 0.5f * (unit.y() + 1.0);
    return (1.0f - t)  * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); 
}

int main(int argc, char *argv[])
{
    int nx = 600;
    int ny = 300;
    int ns = 50; 

    constant_texture blue_texture(color(0.1, 0.2, 0.5));
    constant_texture red_texture(color(1.0, 0.0, 0.0));
    constant_texture gold_texture(color(0.8, 0.6, 0.2));
    checker_texture checkers(&blue_texture, &red_texture);
    image_texture img_texture(image::load("textures/earth.jpg"));

    hitable_list objects;
    lambertian lambert1(&img_texture);
    metal metal1(&gold_texture, 0.25);
    metal metal2(&checkers, 0.02);
    dielectric diel(1.5);
    objects.add(new sphere(vec3(0,0,-1), 0.5, &lambert1)); 
    //objects.add(new sphere(vec3(0,-100.5,-1), 100, &lambert2));
    objects.add(new plane(vec3(0,-1.5,0), vec3(0,1,0), &metal2)); 
    objects.add(new sphere(vec3( 1,0,-1),  0.5, &metal1)); 
    for (int i = 0; i < 4; i++)
    {
        objects.add(new sphere(vec3(-1,0,-1-i),  0.5,  &diel)); 
        objects.add(new sphere(vec3(-1,0,-1-i), -0.45, &diel));
    }

    bvh_node *node = bvh_node::build_tree(objects.get_list(), 0, 1);

    camera cam(M_PI/3.7, float(nx)/float(ny));
    cam.look_at(vec3(-2,3,1), vec3(0,0,-1), vec3(0, 1, 0));
    
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0,0,0);
            for (int s=0; s < ns; s++)
            {
                float u = float(i + random1d()) / float(nx);
                float v = float(j + random1d()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += compute_color(r, &objects, 0);
            }
            col /= float(ns);
            col = gamma_correct(col);
            if (col.x() > 1 || col.y() > 1 || col.z() > 1)
                std::cout << "# channel greater than 1 at " << i << "," << j << "\n";

            int ir = int(254.99 * col[0]);
            int ig = int(254.99 * col[1]);
            int ib = int(254.99 * col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
