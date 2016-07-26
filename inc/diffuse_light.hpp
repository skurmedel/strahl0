#ifndef DIFFUSE_LIGHT_HPP
#define DIFFUSE_LIGHT_HPP

#include "material.hpp"
#include "texture.hpp"

class diffuse_light: public material
{
public:
    diffuse_light(texture *e): emission(e) {}

    virtual bool scatter(
        ray const &r_in, 
        hit_record const &rec, 
        color &out_atten, 
        ray &out_scattered) const 
    { 
        return false; 
    }
    
    virtual color emitted(vec3::type u, vec3::type v, vec3 const &P) const
    {
        return emission->value(u, v, P);
    }

private:
    texture *emission;
};

#endif