#ifndef METAL_HPP
#define METAL_HPP

#include "material.hpp"
#include "texture.hpp"

class metal: public material
{
public:
    metal(texture const *a, vec3::type f = 0.0, vec3::type ref_index = 1.5, color grazing_color=color(1,1,1)):
        albedo(a), fuzz(f), ref_index(ref_index), grazing_color(grazing_color) 
    {
        if (fuzz < 0.0)
            fuzz = 0.0;
        if (fuzz > 1.0)
            fuzz = 1.0;
    }
    
    virtual bool scatter(
        ray const &r_in, 
        hit_record const &rec, 
        color &out_atten, 
        ray &out_scattered) const
    {
        color albedo_val = albedo->value(rec.u, rec.v, rec.P);
        vec3 dir = unit_vector(r_in.direction);
        vec3::type fresnel = schlick(-dot(dir, rec.N), ref_index);
        vec3 reflected = reflect(dir, rec.N);
        out_scattered = ray(rec.P, reflected + fuzz * sphere_random());
        out_atten = albedo_val + (grazing_color - albedo_val) * fresnel;
        return (dot(out_scattered.direction, rec.N) > 0);
    }

private:
    texture const *albedo;
    vec3::type fuzz;
    vec3::type ref_index;
    color grazing_color;
};

#endif