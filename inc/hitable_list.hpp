#ifndef HITABLE_LIST_HPP
#define HITABLE_LIST_HPP

#include <vector>
#include <hitable.hpp>

class hitable_list: public hitable
{
public:
    typedef std::vector<hitable *> list_type;

    hitable_list():list() {}
    hitable_list(list_type const &l): list(l) {}

    virtual bool hit(ray const &, ray::t_type min, ray::t_type max, hit_record &rec) const;

    list_type list;
};

#endif