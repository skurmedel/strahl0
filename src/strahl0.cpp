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
    int nx = 500;
    int ny = 250;
    int ns = 15;

    hitable_list world;
    lambertian lambert1(vec3(0.0, 0.0, 1.0));
    lambertian lambert2(vec3(1.0, 0.0, 0.0));
    float R = 0.707;
    world.list.push_back(new sphere(vec3(-R, 0, -1), R, &lambert1));
    world.list.push_back(new sphere(vec3( R, 0, -1), R, &lambert2));

    camera cam(M_PI/2.0, float(nx)/float(ny));
    
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
