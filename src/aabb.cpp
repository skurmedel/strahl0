#include "aabb.hpp"
#include <algorithm>

bool aabb::hit(ray const &r, ray::t_type tmin, ray::t_type tmax) const
{
    // Uses a variant of Amy William's method:
    // http://people.csail.mit.edu/amy/papers/box-jgt.pdf
    for (int e = 0; e < 3; ++e) 
    {
        ray::t_type inv_d = 1.0f / r.direction[e];
        ray::t_type t0 = (min[e] - r.origin[e]) * inv_d;
        ray::t_type t1 = (max[e] - r.origin[e]) * inv_d;
        if (inv_d < ray::t_type(0.0))
            std::swap(t0, t1);
        tmin = t0 > tmin? t0 : tmin;
        tmax = t1 < tmax? t1 : tmax;
        if (tmax <= tmin)
            return false;
    }
    return true;
}

#define ffmin(a, b) ((a) < (b)? (a) : (b))
#define ffmax(a, b) ((a) > (b)? (a) : (b))

aabb surrounding_aabb(aabb const &b0, aabb const &b1)
{
    vec3 min(
        ffmin(b0.min.x(), b1.min.x()),
        ffmin(b0.min.y(), b1.min.y()),
        ffmin(b0.min.z(), b1.min.z())
    );
    vec3 max(
        ffmax(b0.max.x(), b1.max.x()),
        ffmax(b0.max.y(), b1.max.y()),
        ffmax(b0.max.z(), b1.max.z())
    );
    return aabb(min, max);
}
