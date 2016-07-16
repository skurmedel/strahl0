#ifndef HITABLE_HPP
#define HITABLE_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "aabb.hpp"

class material;

struct hit_record 
{
    // Position of hit on surface.
    vec3 P;
    // Normal at P on surface.
    vec3 N;
    // Ray parameter t.
    ray::t_type t;
    // Material at hit position.
    material const *mat_ptr;
};

class hitable 
{
public:
    virtual bool hit(ray const &, ray::t_type min, ray::t_type max, hit_record &rec) const = 0;
    virtual bool bounding_box(float time0, float time1, aabb &box) const = 0;
};

#endif