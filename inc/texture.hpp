#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cmath>

#include "color.hpp"
#include "vec3.hpp"

class texture
{
public:
    virtual color value(float u, float v, vec3 const &P) const = 0;
};

class constant_texture: public texture
{
public:
    constant_texture(): val(0,0,0) { }
    constant_texture(color c): val(c) { }

    virtual color value(float u, float v, vec3 const &P) const 
    {
        return val;
    }

    color val;
};

class checker_texture: public texture
{
public:
    checker_texture(texture const *even, texture const *odd, vec3::type scale = 10):
        even(even), odd(odd), scale(scale) 
    {
        
    }

    virtual color value(float u, float v, vec3 const &P) const 
    {
        vec3::type d = sin(scale * P.x()) * cos(scale * P.y()) * sin(scale * P.z());
        if (d < 0)
            return odd->value(u, v, P);
        return even->value(u, v, P); 
    }

    texture const *even;
    texture const *odd;
    vec3::type scale;
};

#endif