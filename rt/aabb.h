#ifndef AABBH
#define AABBH

float min3(float a, float b, float c){
    float min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

float max3(float a, float b, float c){
    float max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

class aabb{
    public:

    vec3 min_, max_;

    aabb(){}
    aabb(vec3& a, vec3& b){
        min_ = a;
        max_ = b;
    }

    vec3 min() const { return min_; }
    vec3 max() const { return max_; }

    bool hit(const ray& r) const {

        float t0[3], t1[3];
        
        for(int a=0; a<3; a++){
            float inv_denom = 1.0f/r.direction()[a];
            float temp0 = (min()[a] - r.origin()[a]) * inv_denom;
            float temp1 = (max()[a] - r.origin()[a]) * inv_denom;

            t0[a] = fmin(temp0, temp1);
            t1[a] = fmax(temp0, temp1);
        }

        float tmin = max3(t0[0], t0[1], t0[2]);
        float tmax = min3(t1[0], t1[1], t1[2]);

        return tmin < tmax;
    }

    void extend(aabb box){
        vec3 small(fmin(min_.x(), box.min_.x()),
        fmin(min_.y(), box.min_.y()),
        fmin(min_.z(), box.min_.z())
        );

        vec3 big(fmax(max_.x(), box.max_.x()),
        fmax(max_.y(), box.max_.y()),
        fmax(max_.z(), box.max_.z())
        );
        
        min_ = small;
        max_ =  big;
    }

    void split(int axis, aabb& left, aabb& right){
        vec3 rmax = max();
        vec3 lmin = min();
        vec3 rmin, lmax;
        float mid;
        switch(axis) {
                case 0:
                    mid = (min().x() + max().x())/2.0f;
                    rmin = vec3(mid, min().y(), min().z());
                    lmax = vec3(mid, max().y(), max().z());
                    break;
                case 1:
                    mid = (min().y() + max().y())/2.0f;
                    rmin = vec3(min().x(), mid, min().z());
                    lmax = vec3(max().x(), mid, max().z());
                    break;
                case 2:
                    mid = (min().z() + max().z())/2.0f;
                    rmin = vec3(min().x(), min().y(), mid);
                    lmax = vec3(min().x(), max().y(), mid);
                    break;
            }
        left = aabb(lmin, lmax);
        right = aabb(rmin, rmax);
    }

    int longest_axis(){
        vec3 diag = max_ - min_;
        return diag.argmax();
    }
};

#endif