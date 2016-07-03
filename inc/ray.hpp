#ifndef RAY_HPP
#define RAY_HPP

class ray
{
public:
    typedef vec3::type t_type;

    ray(): origin(), direction() { }
    ray(vec3 const &o, vec3 const &d):
        origin(o), direction(d) { }
    
    vec3 operator()(t_type t) const 
    {
        return origin + t * direction;
    }

    vec3 point_at_parameter(t_type t) const { return (*this)(t); }
    
    vec3 origin;
    vec3 direction;
};

#endif