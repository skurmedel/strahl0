#ifndef PLANE_HPP
#define PLANE_HPP

#include "hitable.hpp"

class plane: public hitable
{
public:
    plane(vec3 const &p, vec3 const &n, material const *m):
        P(p), N(unit_vector(n)), mat_ptr(m)
    {

    }

    virtual bool hit(ray const &r, ray::t_type min, ray::t_type max, hit_record &rec) const
    {
        // Since our plane is infinite, there's only one way the ray can miss,
        // and that is for our plane and ray to be parallel. If our ray is  
        // contained in the plane, we see that as a miss too.
        ray::t_type N_dot_dir = dot(N, unit_vector(r.direction));
        if (fabs(N_dot_dir) < 0.0000001f)
            return false;
        
        ray::t_type t = dot(N, P - r.origin) / N_dot_dir;
        if (t < min || t > max)
            return false;
        rec.t = t;
        rec.P = r(t);
        rec.N = N;
        rec.mat_ptr = this->mat_ptr;
        return true;
    }

private:
    vec3 P;
    vec3 N;
    material const *mat_ptr;
};

#endif