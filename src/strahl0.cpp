#include <iostream>
#include <cfloat>
#include "strahl0.hpp"
#include "external.hpp"

#ifdef _WIN32
#include <fcntl.h>
#endif

vec3 gamma_correct(vec3 const &v)
{
    vec3::type p = 1.0f/2.2f;
    return vec3(
        powf(v.x(), p),
        powf(v.y(), p),
        powf(v.z(), p)
    );
}

vec3 gamma_corrected_clamp(color const &c)
{
    color gc = gamma_correct(c);
    return color(
        gc.x() > 1.0? 1.0 : gc.x(),
        gc.y() > 1.0? 1.0 : gc.y(),
        gc.z() > 1.0? 1.0 : gc.z()
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
        color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.P); 
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return emitted + attenuation * compute_color(scattered, world, depth+1);
        }
        else
            return emitted;
    }
    
    return color(0,0,0); 
}

void stbi_write_to_stdout(void *context, void *data, int size)
{
    fwrite(data, sizeof(char), size, stdout);
}

int main(int argc, char *argv[])
{
    int nx = 600;
    int ny = 300;
    int ns = 50; 

    constant_texture light_color(70.0f*color(0.8, 1.0, 1.0));

    constant_texture blue_texture(color(0.1, 0.2, 0.5));
    constant_texture red_texture(color(1.0, 0.0, 0.0));
    constant_texture gold_texture(color(0.8, 0.6, 0.2));
    checker_texture checkers(&blue_texture, &red_texture);
    image_texture img_texture(image::load("textures/earth.jpg"));

    hitable_list objects;
    lambertian lambert1(&img_texture);
    metal metal1(&gold_texture, 0.25);
    lambertian floor(&checkers);
    diffuse_light light(&light_color);
    dielectric diel(1.5);
    objects.add(new sphere(vec3(0,0,-1), 0.5, &lambert1)); 
    //objects.add(new sphere(vec3(0,-100.5,-1), 100, &lambert2));
    objects.add(new plane(vec3(0,-1.5,0), vec3(0,1,0), &floor)); 
    objects.add(new plane(vec3(2.5,-1.5,0), vec3(-1,0,0), &floor)); 
    objects.add(new plane(vec3(2.5,-1.5,-4), vec3(0,0,1), &floor)); 
    objects.add(new sphere(vec3( 1,0,-1),  0.5, &metal1));
    objects.add(new sphere(vec3( 0.5, 0.3, 0.5), 0.25, &light));
    for (int i = 0; i < 2; i++)
    {
        objects.add(new sphere(vec3(-1,0,-1-i),  0.5,  &diel)); 
        objects.add(new sphere(vec3(-1,0,-1-i), -0.45, &diel));
    }

    bvh_node *node = bvh_node::build_tree(objects.get_list(), 0, 1);

    camera cam(M_PI/3.7, float(nx)/float(ny));
    cam.look_at(vec3(-2,3,1), vec3(0,0,-1), vec3(0, 1, 0));
    
#ifdef _WIN32
    setmode(fileno(stdout), O_BINARY);
#endif

    // Don't stack allocate this, it is usually way larger than the compiler's set
    // stack size. 
    unsigned char *img = new unsigned char[nx * ny * 3];

    //std::cout << "P3\n" << nx << " " << ny << "\n255\n";
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
            col = gamma_corrected_clamp(col);
            /*if (col.x() > 1 || col.y() > 1 || col.z() > 1)
                std::cout << "# channel greater than 1 at " << i << "," << j << "\n";
            */
            img[((ny - 1 - j) * nx + i) * 3 + 0] = (unsigned char)(254.99 * col[0]);
            img[((ny - 1 - j) * nx + i) * 3 + 1] = (unsigned char)(254.99 * col[1]);
            img[((ny - 1 - j) * nx + i) * 3 + 2] = (unsigned char)(254.99 * col[2]);
            //std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    if (!stbi_write_png_to_func(stbi_write_to_stdout, 0, nx, ny, 3, img, 3 * sizeof(char) * nx))
        std::cerr << "Failed to write anything at all.\n";

    delete[] img;
}
