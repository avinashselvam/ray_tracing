#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class material;

struct hit_record{
    float u, v; // for texture mapping
    float t; // time of hit
    vec3 p; // point at which ray hit the surface
    vec3 normal; // normal to the surface at this point
    material *mat_ptr; // mat of the surface hit
};

class hitable{
    public:
    virtual bool hit(const ray& r, float t_min,
                    float t_max, hit_record& rec) const = 0;
};

#endif