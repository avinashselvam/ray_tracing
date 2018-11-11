#ifndef KDTREEH
#define KDTREEH

#include "triangle.h"

struct KDNode{
    public:

    aabb bbox;
    KDNode *left, *right;
    vector<triangle*> triangles;

    KDNode() {
        vec3 zero = vec3(-1000,-1000,-1000); vec3 end = vec3(1000,1000,1000);
        bbox = aabb(zero, end);
        left = NULL; right = NULL;
    }

    void add_triangle(triangle* tri) {
        triangles.push_back(tri);
    }

    void build() {

        int length = triangles.size();
        if(length <= 3) { return; }

        // create children
        left = new KDNode();
        right = new KDNode();

        // find optimal split
        int axis = bbox.longest_axis();

        for(int i = 0; i < length; i++) {
            triangle* tri = triangles[i];
            // float mid;
            // switch(axis) {
            //     case 0:
            //         mid = (bbox.min().x() + bbox.max().x())/2.0f;
            //         if ( tri->v[0].x() > mid || tri->v[1].x() > mid || tri->v[2].x() > mid ){
            //             right->add_triangle(tri);
            //         }
            //         if ( tri->v[0].x() < mid || tri->v[1].x() < mid || tri->v[2].x() < mid ){
            //             left->add_triangle(tri);
            //         }
            //         break;
            //     case 1:
            //         mid = (bbox.min().y() + bbox.max().y())/2.0f;
            //         if ( tri->v[0].y() > mid || tri->v[1].y() > mid || tri->v[2].y() > mid ){
            //             right->add_triangle(tri);
            //         }
            //         if ( tri->v[0].y() < mid || tri->v[1].y() < mid || tri->v[2].y() < mid ){
            //             left->add_triangle(tri);
            //         }
            //         break;
            //     case 2:
            //         mid = (bbox.min().z() + bbox.max().z())/2.0f;
            //         if ( tri->v[0].z() > mid || tri->v[1].z() > mid || tri->v[2].z() > mid ){
            //             right->add_triangle(tri);
            //         }
            //         if ( tri->v[0].z() < mid || tri->v[1].z() < mid || tri->v[2].z() < mid ){
            //             left->add_triangle(tri);
            //         }
            //         break;
            // }
            if(i<length/2) {left->add_triangle(tri);}
            else {right->add_triangle(tri);}
        }

        if(left->triangles.size()) left->build();
        if(right->triangles.size()) right->build();

    }
};

#endif