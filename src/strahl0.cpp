#include <iostream>
#include <strahl0.hpp>
#include <cfloat>

vec3 gamma_correct(vec3 const &v)
{
    vec3::type p = 1.0f/2.2f;
    return vec3(
        powf(v.x(), p),
        powf(v.y(), p),
        powf(v.z(), p)
    );
}

vec3 color(ray const &r, hitable *world)
{
    hit_record rec;
    if (world->hit(r, 0.0001f, FLT_MAX, rec))
    {
        vec3 target = rec.P + rec.N + sphere_random();
        return 0.5f * color(ray(rec.P, target - rec.P), world);
    }
    
    vec3 unit = unit_vector(r.direction);
    float t = 0.5f * (unit.y() + 1.0);
    return (1.0f - t)  * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0); 
}

int main(int argc, char *argv[])
{
    int nx = 300;
    int ny = 150;
    int ns = 30;

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
            col = gamma_correct(col);                       

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}