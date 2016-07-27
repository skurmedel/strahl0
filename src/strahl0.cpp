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

void mk_cornell_box(hitable_list &list)
{
    material *r = new lambertian(new constant_texture(color(0.65, 0.05, 0.05)));
    material *white = new lambertian(new constant_texture(color(0.73, 0.73, 0.73)));
    material *g = new lambertian(new constant_texture(color(0.12, 0.45, 0.15)));

    material *glass = new dielectric(1.56);
    material *light = new diffuse_light(new constant_texture(color(15,15,15)));

    list.add(new flip_normals(new aa_rectangle<Y_AXIS, Z_AXIS>(0, 555, 0, 555, 555, g))); 
    list.add(new aa_rectangle<Y_AXIS, Z_AXIS>(0, 555, 0, 555, 0, r)); 
    list.add(new aa_rectangle<X_AXIS, Z_AXIS>(213, 343, 227, 332, 554, light)); 
    list.add(new flip_normals(new aa_rectangle<X_AXIS, Z_AXIS>(0, 555, 0, 555, 555, white))); 
    list.add(new aa_rectangle<X_AXIS, Z_AXIS>(0, 555, 0, 555, 0, white)); 
    list.add(new flip_normals(new aa_rectangle<X_AXIS, Y_AXIS>(0, 555, 0, 555, 555, white)));
    list.add(new sphere(vec3(277.5,277.5,277.5), 200.0f, glass)); 
}

void set_cornell_view(camera &cam, int nx, int ny)
{
    cam = camera(0.6981, float(nx)/float(ny));
    cam.look_at(vec3(278, 278, -800), vec3(278,278,0));
}

int main(int argc, char *argv[])
{
    int nx = 450;
    int ny = 450;
    int ns = 300; 

    camera cam(M_PI/3.7, float(nx)/float(ny));
    hitable_list list;
    mk_cornell_box(list);
    set_cornell_view(cam, nx, ny);
    
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
                col += compute_color(r, &list, 0);
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
