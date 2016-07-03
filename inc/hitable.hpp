#ifndef HITABLE_HPP
#define HITABLE_HPP

#include <vec3.hpp>
#include <ray.hpp>

struct hit_record 
{
    // Position of hit on surface.
    vec3 P;
    // Normal at P on surface.
    vec3 N;
    // Ray parameter t.
    ray::t_type t;
};

class hitable 
{
public:
    virtual bool hit(ray const &, ray::t_type min, ray::t_type max, hit_record &rec) const = 0;
};

#endif