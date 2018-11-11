#ifndef RAYH
#define RAYH
#include "vec.h"

/*
a ray can be defined by a starting point (origin) and a direction
any point on the ray can be addressed by a parameter t
*/

class ray{
    public:

    vec3 A, B;
    
    ray() {}
    ray(const vec3& a, const vec3& b) {A = a; B = b;}

    vec3 origin() const { return A; }
    vec3 direction() const { return B; }
    vec3 point_at_parameter(float t) const { return A + t*B; }
};

#endif