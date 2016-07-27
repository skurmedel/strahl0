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
        ray::t_type N_dot_dir = dot(N, r.direction);
        if (fabs(N_dot_dir) < 0.00000001f)
            return false;
        
        ray::t_type t = dot(N, P - r.origin) / N_dot_dir;
        if (t < 0 || t < min || t > max)
            return false;
        rec.t = t;
        rec.P = r(t);
        rec.N = N;
        // Todo: Compute UVs.
        rec.mat_ptr = this->mat_ptr;
        return true;
    }

    virtual bool bounding_box(float time0, float time1, aabb &box) const
    {
        return false;
    }

protected:
    vec3 P;
    vec3 N;
    material const *mat_ptr;
};

class disc: public plane 
{
public:
    disc(vec3 const &p, vec3 const &n, vec3::type radius, material const *m):
        plane(p, n, m), radius(radius) {}

    virtual bool hit(ray const &r, ray::t_type min, ray::t_type max, hit_record &rec) const
    {
        bool hit_infinite_plane = plane::hit(r, min, max, rec);
        return hit_infinite_plane && (rec.P - P).len() <= radius;
    }    

    virtual bool bounding_box(float time0, float time1, aabb &box) const
    {
        return false;
    }

private:
    vec3::type radius;
};


#endif