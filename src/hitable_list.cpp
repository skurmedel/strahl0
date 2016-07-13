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

void hitable_list::add(hitable *h)
{
    list.push_back(h);
}