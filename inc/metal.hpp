#ifndef METAL_HPP
#define METAL_HPP

#include "material.hpp"

class metal: public material
{
public:
    metal(vec3 const &a, vec3::type f = 0.0, vec3::type ref_index = 1.5, vec3 grazing_color=vec3(1,1,1)):
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
        vec3 &out_atten, 
        ray &out_scattered) const
    {
        vec3 dir = unit_vector(r_in.direction);
        vec3::type fresnel = schlick(-dot(dir, rec.N), ref_index);
        vec3 reflected = reflect(dir, rec.N);
        out_scattered = ray(rec.P, reflected + fuzz * sphere_random());
        out_atten = albedo + (grazing_color - albedo) * fresnel;
        return (dot(out_scattered.direction, rec.N) > 0);
    }

private:
    vec3 albedo;
    vec3::type fuzz;
    vec3::type ref_index;
    vec3 grazing_color;
};

#endif