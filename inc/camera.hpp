#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <random>
#include <cfloat>

inline float random1d()
{
    static std::uniform_real_distribution<float> dist(0, 1.0f - FLT_EPSILON);
    static std::ranlux48 gen;
    return dist(gen);
}

class camera
{
public:
    camera():
        lower_left_corner(-2.0, -1.0, -1.0),
        horizontal(4.0, 0.0, 0.0),
        vertical(0.0, 2.0, 0.0),
        origin(0.0, 0.0, 0.0)
    {
        
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