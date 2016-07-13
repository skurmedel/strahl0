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
    return r0 + (F(1) - r0)* pow((1.0 - cosine), 5);
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

#endif