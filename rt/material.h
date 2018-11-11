#ifndef MATERIALH
#define MATERIALH

vec3 reflect(const vec3& v, const vec3& n){
        return v - 2*dot(v, n)*n;
}

// to get vector inside a unit sphere centered at (0,0,0)
vec3 random_point_on_sphere(){
    vec3 p;
    do{
        p = 2.0*vec3(drand48(), drand48(), drand48()) -  vec3(1,1,1);
    }while(p.squared_length() >= 1.0);
    return p;
}

class texture;

class material{
    public:

    virtual bool scatter(const ray& r_in, const hit_record& rec,
                        vec3& attenuation, ray& scattered) const = 0;
    virtual vec3 emitted(float u, float v, vec3& p) const {
        return vec3(0,0,0);
    }
};

class lambertian : public material {
    
    public:
    texture* albedo;

    lambertian(texture* a){
        albedo = a;
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 target = rec.p + rec.normal + random_point_on_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo->value(0, 0, rec.p);
        return true;
    }
};

class metal : public material {
    
    public:
    texture* albedo;

    metal(texture* a){
        albedo = a;
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo->value(0, 0, rec.p);
        return true;
        return (dot(scattered.direction(), rec.normal) > 0); // if hit the exterior
    }
};

class diffuse_light : public material {
    public:
    texture* emit;

    diffuse_light(texture* a){
        emit = a;
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec,
    vec3& attenuation, ray& scattered) const { return false; }
    virtual vec3 emitted(float u, float v, vec3& p) const {
        return emit->value(u, v, p);
    }

};

#endif