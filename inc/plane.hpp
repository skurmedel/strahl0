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

enum axis: std::size_t
{
    X_AXIS = 0,
    Y_AXIS = 1,
    Z_AXIS = 2
};

struct axes_complement
{
    template <axis A1, axis A2>
    static constexpr axis value() 
    {
        static_assert(A1 != A2, "Must be separate axes.");  
        return (axis[]){X_AXIS, Z_AXIS, Y_AXIS, X_AXIS}[(std::size_t) A1 + (std::size_t) A2];
    }
};

template <axis A1, axis A2>
class aa_rectangle: public hitable
{
public:
    aa_rectangle(vec3::type a0, vec3::type a1, vec3::type b0, vec3::type b1, vec3::type k, material const *mat_ptr):
        a0(a0), a1(a1), b0(b0), b1(b1), k(k), mat_ptr(mat_ptr)
    {

    }

    virtual bool hit(ray const &r, ray::t_type tmin, ray::t_type tmax, hit_record &rec) const {
        axis comp;
        comp = axes_complement::value<A1, A2>();
        float t = (k - r.origin[comp]) / r.direction[comp];

        if (t < tmin || t > tmax)
            return false;

        float c0 = r.origin[A1] + t * r.direction[A1];        
        float c1 = r.origin[A2] + t * r.direction[A2];

        if (c0 < a0 || c0 > a1 || c1 < b0 || c1 > b1)
            return false;

        rec.u = (c0 - a0) / (a1 - a0);        
        rec.v = (c1 - b0) / (b1 - b0);
        rec.t = t;
        rec.mat_ptr = this->mat_ptr;
        rec.P = r(t);
        rec.N[A1] = 0;
        rec.N[A2] = 0;
        rec.N[comp] = 1;
        return true;        
    }    

    virtual bool bounding_box(float time0, float time1, aabb &box) const
    {
        vec3 min(0,0,0);
        vec3 max(0,0,0);
        min[A1] = a0;
        min[A2] = b0;
        min[axes_complement::value<A1, A2>()] = k - 0.00001;
        max[A1] = a1;
        max[A2] = b1;
        max[axes_complement::value<A1, A2>()] = k + 0.00001;
        box = aabb(min, max);
        return true;
    }
private:
    vec3::type a0, a1, b0, b1, k;
    material const *mat_ptr;
};

class flip_normals: public hitable 
{
public:
    flip_normals(hitable *p): ptr(p) 
    {

    }

    virtual bool hit(ray const &r, ray::t_type tmin, ray::t_type tmax, hit_record &rec) const 
    {
            if (ptr->hit(r, tmin, tmax, rec))
            {
                rec.N = -rec.N;
                return true;
            }
            return false;
    }    
    virtual bool bounding_box(float time0, float time1, aabb &box) const
    {
        return ptr->bounding_box(time0, time1, box);
    }    

private:
    hitable *ptr;
};

#endif