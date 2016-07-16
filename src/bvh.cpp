#include "bvh.hpp"
#include "stochastics.hpp"
#include <algorithm>

bool bvh_node::hit(ray const &r, ray::t_type tmin, ray::t_type tmax, hit_record &rec) const
{
    if (bounds.hit(r, tmin, tmax))
    {
        hit_record left_rec, right_rec;
        bool hit_left = left->hit(r, tmin, tmax, left_rec);
        bool hit_right = right->hit(r, tmin, tmax, right_rec);
        if (hit_left && hit_right)
        {
            // Check which hit was closer to ray origin.
            rec = left_rec.t < right_rec.t? left_rec : right_rec;
            return true;
        }
        if (!(hit_left || hit_right))
            return false;
        rec = hit_left? left_rec : right_rec;
        return true;
    }
    return false;
}

bvh_node *bvh_node::build_tree(std::vector<hitable *> hitables, float time0, float time1)
{
    bvh_node *node = new bvh_node();

    // Select a random cardinal axis.
    int axis = int(random1d() * 2);
    bool empty_bounds_detected;
    std::sort(hitables.begin(), hitables.end(), [&](hitable *a, hitable *b){
        aabb box_left, box_right;
        if (!a->bounding_box(0,0, box_left) || !b->bounding_box(0,0, box_right))
            empty_bounds_detected = true;
        if (box_left.min_corner()[axis] - box_right.min_corner()[axis] < 0.0)
            return -1;
        else
            return  1;
        });
    if (hitables.size() == 1)
    {
        node->left = node->right = hitables[0];
    }
    else if (hitables.size() == 2)
    {
        node->left  = hitables[0];
        node->right = hitables[1];
    }
    else
    {
        node->left  = build_tree(std::vector<hitable *>(hitables.begin(), hitables.begin() + hitables.size() / 2), time0, time1);
        node->right = build_tree(std::vector<hitable *>(hitables.begin() + hitables.size() / 2, hitables.end()), time0, time1);
    }
    aabb bounds_left, bounds_right;
    if (!left->bounding_box(time0, time1, bounds_left) || !right->bounding_box(time0, time1, bounds_right))
        empty_bounds_detected = true;
    node->bounds = surrounding_aabb(bounds_left, bounds_right);
    return node;
}

