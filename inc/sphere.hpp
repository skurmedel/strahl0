#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <hitable.hpp>

class sphere: public hitable 
{
public:
    sphere(): radius(0) {}
    sphere(vec3 cen, vec3::type radius): center(cen), radius(radius) {}
    
    virtual bool hit(ray const &, ray::t_type min, ray::t_type max, hit_record &rec) const;

    vec3 normal_for_p(vec3 const &p) const
    {
        return unit_vector(p - center);
    }

    vec3 center;
    vec3::type radius;
};

#endif