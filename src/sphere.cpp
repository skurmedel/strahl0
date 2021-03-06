#include <sphere.hpp>
#include <cmath>

void set_uvs(vec3 const &unit_p, hit_record &rec)
{
    vec3::type phi = std::atan2(unit_p.z(), unit_p.x());
    vec3::type theta = std::asin(unit_p.y());
    rec.u = (phi + M_PI) / (2.0f * M_PI);
    rec.v = 1.0 - (theta + M_PI / 2.0f) / M_PI;
}

bool sphere::hit(
    ray const &r, ray::t_type min, ray::t_type max, hit_record &rec) const
{
    typedef ray::t_type F;

    F dot_dir = dot(r.direction, r.direction);
    vec3 disp = r.origin - center;
    F dot_dir_disp = dot(r.direction, disp);
    F dot_disp = dot(disp, disp);
    // We divide with dot_dir, and so we can use the pq-formula instead
    // because it is much more beautiful :D
    // F p = (2.0 * dot_dir_disp) / dot_dir;
    F p = (dot_dir_disp) / dot_dir;
    F q = (dot_disp - radius * radius) / dot_dir;
    // p has generally a 1 / 2^2-term in the full formula, but it is 
    // eliminated by one of the sphere intersection terms in p.
    // F discr = (p * p / 4.0f - q);
    F discr = (p * p - q);
    if (discr > 0.0f) 
    {
        // Same as above, the 1 / 2 term is cancelled by a term in p.
        F t1 = -p - sqrt(discr);
        F t2 = -p + sqrt(discr);
        if (t1 > t2)
        {
            F tmp = t2;
            t2 = t1;
            t1 = tmp;
        }
        F t = 0;
        if (min < t1 && t1 < max)
        { 
            t = t1;
        }
        else if (min < t2 && t2 < max)
        {
            t = t2;
        }
        else
        {
            return false;
        }

        rec.t = t;
        rec.P = r(t);
        
        rec.N = (rec.P - center) / radius;
        set_uvs(rec.N, rec);
        rec.mat_ptr = this->mat_ptr;
        return true;
    }
    return false;
}

bool sphere::bounding_box(float time0, float time1, aabb &box) const
{
    vec3 offset(radius, radius, radius);
    box = aabb(
        center - offset,
        center + offset);
    return true;
}
