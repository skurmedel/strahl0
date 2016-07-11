#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "vec3.hpp"

class camera
{
public:
    /*
     Initialize a new camera object with the given vertical field of view,
     in radians.
	*/
    camera(vec3::type vfov, vec3::type aspect=16.0/9.0):
        lower_left_corner(), horizontal(), vertical(), origin(),
        half_height(0), half_width(0)
    {
        half_height = tan(vfov / 2.0);
        half_width  = aspect * half_height;
        lower_left_corner   = vec3(-half_width, -half_height, -1.0);
        horizontal          = vec3(2.0 * half_width, 0, 0);
        vertical            = vec3(0.0, 2.0 * half_height, 0);
    }

    void look_at(vec3 const &from, vec3 const &target, vec3 const &up = vec3(0.0, 1.0, 0.0))
    {
        origin = from;
        vec3 u, v, w;
        w = unit_vector(from-target);
        u = unit_vector(cross(up, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * u - half_height * v - w;
        horizontal = 2.0f * half_width  * u;
        vertical   = 2.0f * half_height * v;
    }

    ray get_ray(float u, float v) const
    {
        return ray(
            origin, 
              lower_left_corner 
            + u * horizontal 
            + v * vertical - origin);
    }

    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    vec3::type half_height;
    vec3::type half_width;
};


#endif