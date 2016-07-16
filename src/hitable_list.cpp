#include <hitable_list.hpp>

bool hitable_list::hit(
    ray const &r, ray::t_type min, ray::t_type max, hit_record &rec) const
{
    hit_record temp;
    bool hit_anything = false;
    ray::t_type closest = max;
    for (list_type::const_iterator i = list.begin(); i!=list.end(); i++)
    {
        hitable *item = *i;
        if (item->hit(r, min, closest, temp))
        {
            hit_anything = true;
            closest = temp.t;
            rec = temp;
        }
    }
    return hit_anything;
}


bool hitable_list::bounding_box(float time0, float time1, aabb &box) const
{
    if (list.size() < 1)
        return false;
    
    aabb temp_box;
    bool first_true = list[0]->bounding_box(time0, time1, temp_box);
    if (!first_true)
        return false;
    else
        box = temp_box;
    for (std::size_t i = 0; i < list.size(); ++i)
    {
        if (list[i]->bounding_box(time0, time1, temp_box))
        {
            box = surrounding_aabb(box, temp_box);
        } 
        else 
            return false;
    }
    return true;
}

void hitable_list::add(hitable *h)
{
    list.push_back(h);
}