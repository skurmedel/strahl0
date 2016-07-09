#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <hitable.hpp>
#include <ray.hpp>
#include <stochastics.hpp>

template <typename F>
inline F schlick(F cosine, F ref_index)
{
    F r0 = (F(1.0) - ref_index) / (F(1.0) + ref_index);
    r0 = r0 * r0;
    return r0 + (F(1) - r0)* pow((1 - cosine), 5);
}

class material
{
public:
    virtual bool scatter(
        ray const &r_in, 
        hit_record const &rec, 
        vec3 &out_atten, 
        ray &out_scattered) const = 0;
};

#define LAMBERTIAN_SCATTER_ALWAYS

class lambertian: public material 
{
public:
    lambertian(vec3 const &a): albedo(a) { }

    virtual bool scatter(
        ray const &r_in, 
        hit_record const &rec, 
        vec3 &out_atten, 
        ray &out_scattered) const
    {
        vec3 target = rec.P + rec.N + sphere_random();
#ifdef LAMBERTIAN_SCATTER_ALWAYS
        // This code path is slower but less noisy for low sample 
        // counts.
        out_scattered = ray(rec.P, target-rec.P);
        out_atten = albedo;
        return true;
#else
        // This code path is quicker, but gives more noise for low 
        // sample counts.
        float p = random1d();
        
        // This code will give strange results if |albedo| > 1.0... 
        // but that is a situation which is not realistic anyway.

        // R is the reflectance AND probability that we will scatter.
        float R = dot(albedo, vec3(0.3333f, 0.3333f, 0.3333f));
        // So if p is larger than R we have (1-R) chance to not 
        // scatter, that is, 1-R chance to absorb the ray.
        if (p > R)
            return false;
        // We will scatter this ray, so we scale its attenuation with
        // the probability (R).
        out_atten = albedo / R;
        // Send it in a random direction on the unit sphere.
        out_scattered = ray(rec.P, target-rec.P);
        return true;
#endif
    }


private:
    vec3 albedo;
};

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