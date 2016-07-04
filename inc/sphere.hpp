#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <hitable.hpp>
#include <material.hpp>

class sphere: public hitable 
{
public:
    sphere(material const *m): center(), radius(0), mat_ptr(m) {}
    sphere(vec3 cen, vec3::type radius, material const *m): 
        center(cen), radius(radius), mat_ptr(m) 
    {}
    
    virtual bool hit(ray const &, ray::t_type min, ray::t_type max, hit_record &rec) const;

    vec3 normal_for_p(vec3 const &p) const
    {
        return unit_vector(p - center);
    }

    vec3 center;
    vec3::type radius;
    material const *mat_ptr;
};

#endif