#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "hitable.hpp"
#include "ray.hpp"
#include "stochastics.hpp"
#include "color.hpp"

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
        color &out_atten, 
        ray &out_scattered) const = 0;
    
    virtual color emitted(vec3::type u, vec3::type v, vec3 const &P)
    {
        return color(0,0,0);
    }
};

#endif