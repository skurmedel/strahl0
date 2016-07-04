#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>

template <typename F>
class vector3
{
public:
    typedef F type;

    vector3(): e{0,0,0} {}
    vector3(F x, F y, F z): e{x,y,z} {}

    const vector3<F> &operator+() const { return *this; }

    vector3<F> operator-() const { return vector3<F>(-e[0], -e[1], -e[2]); }
    F operator[](int i) const { return e[i]; }
    F &operator[](int i) { return e[i]; }

#define MAKE_ASS_OPER(op)\
    vector3<F> &operator op ## = (vector3<F> const &v2) {\
        e[0] op ## = v2.e[0];\
        e[1] op ## = v2.e[1];\
        e[2] op ## = v2.e[2];\
        return *this;\
    }

    MAKE_ASS_OPER(+);
    MAKE_ASS_OPER(-);
    MAKE_ASS_OPER(*);
    MAKE_ASS_OPER(/);
#undef MAKE_ASS_OPER

    vector3<F> &operator*=(F const t) 
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vector3<F> &operator/=(F const t) 
    {
        e[0] /= t;
        e[1] /= t;
        e[2] /= t;
        return *this;
    }

    F len2() const 
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    F len() const 
    {
        return sqrt(len2());
    }

    inline void normalize();    

    inline F x() const { return e[0]; }
    inline F y() const { return e[1]; }
    inline F z() const { return e[2]; }

    F e[3];
};

#define MAKE_OPER(op)\
    template <typename F> \
    inline vector3<F> operator op (vector3<F> const &v1, vector3<F> const &v2) {\
        auto v = v1;\
        v op ## = v2;\
        return v;\
    }

MAKE_OPER(+);
MAKE_OPER(-);
MAKE_OPER(*);
MAKE_OPER(/);

#undef MAKE_OPER

typedef vector3<float> vec3;
typedef vec3 rgb;

template <typename F>
inline vector3<F> operator*(vector3<F> const &v1, F const f) 
{
    vector3<F> v = v1;
    v *= f;
    return v;
}

template <typename F>
inline vector3<F> operator*(F const f, vector3<F> const &v1) 
{
    vector3<F> v = v1;
    v *= f;
    return v;
}

template <typename F>
inline vector3<F> operator/(vector3<F> const &v1, F const f) 
{
    vector3<F> v = v1;
    v /= f;
    return v;
}

template <typename F>
inline F dot(vector3<F> const &v1, vector3<F> const &v2) 
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2]; 
}

template <typename F>
inline vector3<F> unit_vector(vector3<F> const &v)
{
    return v / v.len();
}

template <typename F>
inline void vector3<F>::normalize() 
{
    auto repr = 1.0f / (len() + F(0.000000001));
    e[0] *= repr;
    e[1] *= repr;
    e[2] *= repr;
}

template <typename F>
inline vector3<F> reflect(vector3<F> const &v, vector3<F> const &N)
{
    return v - 2.0f * dot(v, N) * N;
}

#endif