#ifndef BVH_HPP
#define BVH_HPP

#include "hitable.hpp"
#include <vector>

class bvh_node: public hitable
{
public:
    bvh_node(): left(0), right(0), bounds() {}

    virtual bool hit(ray const &r, ray::t_type tmin, ray::t_type tmax, hit_record &rec) const;
    virtual bool bounding_box(float time0, float time1, aabb &bounds) const
    {
        bounds = this->bounds;
        return true;
    }
    
    hitable const *left_child() const { return left; }
    hitable const *right_child() const { return right; }

    static bvh_node *build_tree(std::vector<hitable *> hitables, float time0, float time1);

private:
    hitable *left, *right;
    aabb bounds;
};

#endif