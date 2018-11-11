#ifndef TRIANGLEH
#define TRIANGLEH

#include "hitable.h"
#include "aabb.h"
#include<math.h>

#define EPSILON 0.001f

class triangle : public hitable {
    public:

    vec3 v[3], normal, mid_pt;
    float d;
    material* mat;
    aabb bbox;

    triangle() {}
    triangle(vec3 v1, vec3 v2, vec3 v3, material* m){
        v[0] = v1; v[1] = v2; v[2] = v3;
        normal = cross(v2-v1, v3-v1); normal.normalise();
        d = dot(v1, normal);
        mat = m;
        
        vec3 min_, max_;
        min_ = vec3(min3(v1[0], v2[0], v3[0]), min3(v1[1], v2[1], v3[1]), min3(v1[2], v2[2], v3[2]));
        max_ = vec3(max3(v1[0], v2[0], v3[0]), max3(v1[1], v2[1], v3[1]), max3(v1[2], v2[2], v3[2]));
        bbox = aabb(min_, max_);
        
        mid_pt = (v1 + v2 + v3)/3;
    }

    // barycentric algo
    bool is_inside(vec3& p, float& s, float& t) const {    
        vec3 s0 = v[2] - v[0];
        vec3 s1 = v[1] - v[0];
        vec3 s2 = p - v[0];

        float dot00 = dot(s0, s0);
        float dot01 = dot(s0, s1);
        float dot02 = dot(s0, s2);
        float dot11 = dot(s1, s1);
        float dot12 = dot(s1, s2);

        float denom = dot00 * dot11 - dot01 * dot01;
        float u = (dot11 * dot02 - dot01 * dot12) / denom;
        float v = (dot00 * dot12 - dot01 * dot02) / denom;

        s = u; t = v;

        return (u >= 0) && (v >= 0) && (u + v < 1);
    }
    
    virtual bool hit(const ray& r, float tmin,
                    float tmax, hit_record& rec) const;
    
};

bool triangle::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    
    float temp = abs(dot(normal, r.origin()) + d)/(dot(normal, r.direction())+EPSILON);
   
    if (temp < tmax && temp > tmin){
        vec3 p = r.point_at_parameter(temp);
        if(is_inside(p, rec.u, rec.v)){
            rec.mat_ptr = mat;
            rec.t = temp;
            rec.p = p;
            rec.normal = -normal;
            return true;
        }
    }

    return false;

}

#endif
