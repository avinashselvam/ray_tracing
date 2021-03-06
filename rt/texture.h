#ifndef TEXTUREH
#define TEXTUREH

class texture {
    public:
    virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture : public texture {
    public:
    vec3 color;

    constant_texture(){}
    constant_texture(vec3 c){
        color = c;
    }
    virtual vec3 value(float u, float v, const vec3& p) const {
        return color;
    }
};

#endif