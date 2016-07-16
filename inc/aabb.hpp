#ifndef AABB_HPP
#define AABB_HPP

#include "vec3.hpp"
#include "ray.hpp"

class aabb 
{
public:
    aabb(): min(), max() {}
    aabb(vec3 const &min, vec3 const &max): min(min), max(max)
    {}

    bool hit(ray const &r, ray::t_type tmin, ray::t_type tmax) const;

    friend aabb surrounding_aabb(aabb const &b0, aabb const &b1); 

private:
    vec3 min;
    vec3 max;
};


#endif