#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera{
    public:

    // setting up coordinate system
    vec3 lower_left_corner, horizontal, vertical, origin;

    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vertical_fov, float aspect){
        
        float theta = vertical_fov*(M_PI/180);
        float half_height = tan(theta/2); // because z = -1 is our view plane
        float half_width = aspect*half_height;

        origin = lookfrom;

        vec3 w = unit_vector(lookfrom - lookat);
        vec3 u = unit_vector(cross(vup, w));
        vec3 v = cross(w, u);
        
        lower_left_corner = vec3(-half_width, -half_height, -1.0);
        lower_left_corner = origin - half_width*u - half_height*v - w;
        horizontal = 2*half_width*u;
        vertical = 2*half_height*v;
    }

    ray get_ray(float u, float v) {
         return ray(origin, lower_left_corner + u*horizontal + v*vertical);
    }
    
};

#endif