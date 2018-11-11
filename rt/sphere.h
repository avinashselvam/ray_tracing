#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere : public hitable {
    public:

    vec3 center;
    float radius;
    material* mat;

    sphere() {}
    sphere(vec3 cen, float r, material* m){
        center = cen;
        radius = r;
        mat = m;
    }
    
    virtual bool hit(const ray& r, float tmin,
                    float tmax, hit_record& rec) const;

};

void get_sphere_uv(vec3& p, float& u, float& v) {
    float phi = atan2(p.z(), p.x());
    float theta = asin(p.y());
    u = 1 - (phi + M_PI) / (2*M_PI);
    v = (theta + M_PI/2) / M_PI;
}

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    
    vec3 oc = r.origin() - center;

    // calc discriminant to check if ray intersects sphere
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction()); // uses 2b to reduce calc later
    float c = dot(oc, oc) - radius*radius;

    float discriminant = b*b - a*c;

    if(discriminant > 0){
        rec.mat_ptr = mat;
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin){
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            return true;
        }

        temp = (-b + sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin){
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            return true;
        }
    }

    return false;

}

#endif