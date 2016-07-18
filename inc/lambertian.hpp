#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "material.hpp"
#include "texture.hpp"

#define LAMBERTIAN_SCATTER_ALWAYS

class lambertian: public material 
{
public:
    lambertian(texture const *a): albedo(a) { }

    virtual bool scatter(
        ray const &r_in, 
        hit_record const &rec, 
        color &out_atten, 
        ray &out_scattered) const
    {
        vec3 target = rec.P + rec.N + sphere_random();
        color albedo_val = albedo->value(0, 0, rec.P);
#ifdef LAMBERTIAN_SCATTER_ALWAYS
        // This code path is slower but less noisy for low sample 
        // counts.
        out_scattered = ray(rec.P, target-rec.P);
        out_atten = albedo_val;
        return true;
#else
        // This code path is quicker, but gives more noise for low 
        // sample counts.
        float p = random1d();
        
        // This code will give strange results if |albedo_val| > 1.0... 
        // but that is a situation which is not realistic anyway.

        // R is the reflectance AND probability that we will scatter.
        float R = dot(albedo_val, vec3(0.3333f, 0.3333f, 0.3333f));
        // So if p is larger than R we have (1-R) chance to not 
        // scatter, that is, 1-R chance to absorb the ray.
        if (p > R)
            return false;
        // We will scatter this ray, so we scale its attenuation with
        // the probability (R).
        out_atten = albedo_val / R;
        // Send it in a random direction on the unit sphere.
        out_scattered = ray(rec.P, target-rec.P);
        return true;
#endif
    }


private:
    texture const *albedo;
};

#endif