#include <iostream>
#include <strahl0.hpp>
#include <cfloat>

vec3 color(ray const &r, hitable *world)
{
    hit_record rec;
    if (world->hit(r, 0.0f, FLT_MAX, rec))
    {
        return 0.5f * (rec.N + vec3(1,1,1));
    }
    
    vec3 unit = unit_vector(r.direction);
    float t = 0.5f * (unit.y() + 1.0);
    return (1.0f - t)  * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0); 
}

int main(int argc, char *argv[])
{
    int nx = 200;
    int ny = 100;
    int ns = 100;

    hitable_list world;
    world.list.push_back(new sphere(vec3(0,0,-1), 0.5));
    world.list.push_back(new sphere(vec3(0,-100.5,-1), 100.0f));

    camera cam;
    
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
                col += color(r, &world);
            }
            col /= float(ns);                       

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}