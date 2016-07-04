#ifndef STOCHASTICS_HPP
#define STOCHASTICS_HPP

#include <random>
#include <cfloat>

inline float random1d()
{
    static std::uniform_real_distribution<float> dist(0, 1.0f - FLT_EPSILON);
    static std::ranlux48 gen;
    return dist(gen);
}

inline vec3 sphere_random()
{
    vec3 p;
    do
    {
        p = 2.0f * vec3(random1d(), random1d(), random1d()) - vec3(1,1,1);
    } while (p.len2() >= 1.0);
    return p;
}

#endif