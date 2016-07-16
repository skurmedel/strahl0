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

vec3 color(ray const &r, hitable *world, int depth)
{
    hit_record rec = {};
    if (world->hit(r, 0.0001f, FLT_MAX, rec))
    {
        ray scattered;
        vec3 attenuation;
        if (rec.mat_ptr == 0)
            std::cerr << "SCHEISSE!";
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth+1);
        }
        else
            return vec3(0,0,0);
    }
    
    vec3 unit = unit_vector(r.direction);
    float t = 0.5f * (unit.y() + 1.0);
    return (1.0f - t)  * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0); 
}

int main(int argc, char *argv[])
{
    int nx = 600;
    int ny = 300;
    int ns = 50;

    hitable_list world;
    lambertian lambert1(vec3(0.1, 0.2, 0.5));
    lambertian lambert2(vec3(0.8, 0.8, 0.0));
    metal metal1(vec3(0.8, 0.6, 0.2), 0.25);
    metal metal2(vec3(0.3, 0.4, 0.8), 0.5, 1.6);
    dielectric diel(1.5);
    world.add(new sphere(vec3(0,0,-1), 0.5, &lambert1)); 
    //world.add(new sphere(vec3(0,-100.5,-1), 100, &lambert2));
    world.add(new plane(vec3(0,-0.5,0), vec3(0,1,0), &metal2)); 
    world.add(new sphere(vec3( 1,0,-1),  0.5, &metal1)); 
    world.add(new sphere(vec3(-1,0,-1),  0.5,  &diel)); 
    world.add(new sphere(vec3(-1,0,-1), -0.45, &diel));

    camera cam(M_PI/3.5, float(nx)/float(ny));
    cam.look_at(vec3(-2,2,1), vec3(0,0,-1), vec3(0, 1, 0));
    
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
                col += color(r, &world, 0);
            }
            col /= float(ns);
            col = gamma_correct(col);

            int ir = int(254.99 * col[0]);
            int ig = int(254.99 * col[1]);
            int ib = int(254.99 * col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
