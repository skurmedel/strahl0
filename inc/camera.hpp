#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"

class camera
{
public:
    /*
     Initialize a new camera object with the given vertical field of view,
     in radians.
	*/
    camera(vec3::type vfov, vec3::type aspect=16.0/9.0):
        lower_left_corner(), horizontal(), vertical(), origin()
    {
        vec3::type half_height = tan(vfov / 2.0);
        vec3::type half_width = aspect * half_height;
        lower_left_corner   = vec3(-half_width, -half_height, -1.0);
        horizontal          = vec3(2.0 * half_width, 0, 0);
        vertical            = vec3(0.0, 2.0 * half_height, 0);
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
};


#endif