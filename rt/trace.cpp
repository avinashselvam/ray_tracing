#include <iostream>
#include <fstream> // to write an .ppm image file
#include "float.h" // to use MAXFLOAT
#include <random>
#include <sstream>
#include <vector>


#include "camera.h"
#include "sphere.h"
#include "kdtree.h"
#include "texture.h"
#include "material.h"
#include "hitablelist.h"
#include "treeutils.h"


using namespace std;

#define NUM_BOUNCES 3
#define NUM_SAMPLES 10

/*
to get color from cast ray
if -> recursively send diffuse/specular reflections
else -> return background color
*/
vec3 get_color(const ray& r, hitable *world, int j){
    hit_record rec;
    if(world->hit(r, 0.001, MAXFLOAT, rec)){
        return rec.normal;
        return vec3(1,1,1);
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered) && (j < NUM_BOUNCES)){
            return emitted + attenuation*get_color(scattered, world, ++j);
        }
        else{
            return emitted;
        }
    }
    else{
        return vec3(0,0,0);
    }
}

vector<KDNode*> hitted;

void traverse_kd(KDNode* current, ray r){
    if(current->left == NULL && current->right == NULL){
        hitted.push_back(current);
        return;
    }
    else{
        if(current->left->bbox.hit(r)) traverse_kd(current->left, r);
        if(current->right->bbox.hit(r)) traverse_kd(current->right, r);
    }
}
  
// void traverse_kd(KDNode* current){
//     // cout<<current->list_of_triangles.size()<<"\n";
//     for(int i= 0; i<current->list_of_triangles.size(); i++){
//         cout<<current->list_of_triangles[i]->mid_pt<<"\n";
//     }
//     if(current->left != NULL) traverse_kd(current->left);
//     if(current->right != NULL) traverse_kd(current->right);
// }

// void hit_kd(const ray& r, KDNode* current, vector<triangle*>& tri3){
//     while(current->bbox.hit(r)){
//         tri3 = current->list_of_triangles;
//         if(current->left != NULL && current->left->bbox.hit(r)) current = current->left;
//         else if(current->right != NULL && current->right->bbox.hit(r)) current = current->right;
//         else return;
//     }
// }

// void test_hit(){
//     vec3 l, r;
//     l = vec3(0,0,0);
//     r = vec3(1,1,1);
//     aabb box = aabb(l, r);
//     cout<<box.hit(ray(vec3(0,0,-10), vec3(0,0,1)));
// }

int main()
{
    // KDTree
    KDNode root;

    // file reading
    ifstream objfile; string line; vec3 v1, v2, v3;
    float s = 1; vec3 offset = vec3(0,0,1); int i = 0;
    int READ_LENGTH = 12; // 720
    hitable *mesh[READ_LENGTH];
    objfile.open ("./models/cube.txt");
    if (!objfile){ cerr<<"file not found"; exit(0); }
    while(i < READ_LENGTH && getline(objfile, line)){
        stringstream ss(line);
        ss>>v1>>v2>>v3;
        triangle* temp = new triangle(s*v1-offset, s*v2-offset, s*v3-offset, new lambertian( new constant_texture(vec3(0.4, 0.1, 0.3))));
        mesh[i++] = temp; root.add_triangle(temp);
    }
    cout<<"read "<<i<<" triangles\n";
    objfile.close();
    // end of file reading

    root.build();
    print2D(&root);
    cout<<"here";

    

    // dimensions of image
    int nx = 200;
    int ny = 200;

    camera cam(vec3(0,0,0), vec3(0,0,-1), vec3(0,1,0), 90, float(nx)/float(ny));
    
    ofstream image;
    image.open("output.ppm");
    image<<"P3\n"<<nx<<" "<<ny<<"\n255\n";
    
    // setting up world
    hitable *list[7], *cornell_box[16];

    // set up cornell box for showcase
    // front
    cornell_box[0] = new triangle(vec3(-1,1,-1), vec3(1,1,-2), vec3(1,1,-1), new lambertian( new constant_texture(vec3(1, 1, 1))));
    cornell_box[1] = new triangle(vec3(-1,1,-1), vec3(-1,1,-2), vec3(1,1,-2), new lambertian( new constant_texture(vec3(1, 1, 1))));
    
    // green
    cornell_box[2] = new triangle(vec3(-1,-1,-1), vec3(1,-1,-1), vec3(-1,-1,-2), new lambertian( new constant_texture(vec3(0.1, 0.4, 0.1))));
    cornell_box[3] = new triangle(vec3(1,-1,-1), vec3(1,-1,-2), vec3(-1,-1,-2), new lambertian( new constant_texture(vec3(0.1, 0.4, 0.1))));
    
    // blue
    cornell_box[4] = new triangle(vec3(1,1,-1), vec3(1,1,-2), vec3(1,-1,-1), new lambertian( new constant_texture(vec3(0.1, 0.1, 0.4))));
    cornell_box[5] = new triangle(vec3(1,-1,-1), vec3(1,1,-2), vec3(1,-1,-2), new lambertian( new constant_texture(vec3(0.1, 0.1, 0.4))));
    
    // red
    cornell_box[6] = new triangle(vec3(-1,-1,-1), vec3(-1,-1,-2), vec3(-1,1,-1), new lambertian( new constant_texture(vec3(0.4, 0.1, 0.1))));
    cornell_box[7] = new triangle(vec3(-1,1,-1), vec3(-1,-1,-2), vec3(-1,1,-2), new lambertian( new constant_texture(vec3(0.4, 0.1, 0.1))));

    cornell_box[8] = new triangle(vec3(-1,-1,-2), vec3(-1,1,-2), vec3(1,1,-2), new metal( new constant_texture(vec3(0.4, 0.4, 0.4))));
    cornell_box[9] = new triangle(vec3(-1,-1,-2), vec3(1,1,-2), vec3(1,-1,-2), new metal( new constant_texture(vec3(0.4, 0.4, 0.4))));

    cornell_box[12] = new triangle(vec3(-1,-1,2), vec3(-1,1,2), vec3(1,1,2), new metal( new constant_texture(vec3(0.4, 0.4, 0.4))));
    cornell_box[13] = new triangle(vec3(-1,-1,2), vec3(1,1,2), vec3(1,-1,2), new metal( new constant_texture(vec3(0.4, 0.4, 0.4))));

    // light
    cornell_box[10] = new sphere(vec3(0,1.2,-1.1), 0.3, new diffuse_light( new constant_texture(vec3(34, 34, 34))));
    
    // sample spheres
    cornell_box[11] = new sphere(vec3(0,-0.9,-1.5), 0.3, new metal(new constant_texture(vec3(0.4, 0.4, 0.4))));
    cornell_box[14] = new sphere(vec3(0.2,-0.5,-1.7), 0.4, new metal(new constant_texture(vec3(0.9, 0.2, 0.5))));
    cornell_box[15] = new sphere(vec3(-0.4,-0.1,-1.1), 0.25, new lambertian(new constant_texture(vec3(0.4, 0.4, 0.4))));

    // hitable *world = new hitable_list(mesh, READ_LENGTH+2);
    hitable *world = new hitable_list(cornell_box, 16);
    
    // ray casting
    for(int i=ny-1; i>=0; i--){
        
        // cout<<i<<"\n";
        
        for(int j=0; j<nx; j++){
            
            /*
            send few rays through same pixel and average
            anti aliasing by super sampling
            */
            vec3 color = vec3(0,0,0);
            
            for(int s=0; s<NUM_SAMPLES; s++){
                float v = float(i + drand48()) / float(ny);
                float u = float(j + drand48()) / float(nx);
                ray r = cam.get_ray(u, v);
                
                traverse_kd(&root, r);
                
                for(int m=0; m<hitted.size(); m++){
                    vector<triangle*> tri3 = hitted[m]->triangles;
                    hitable *local_world[3];
                    
                    for(int n=0; n<tri3.size(); n++){
                        local_world[n] = tri3[n];
                    }
                    
                    color += get_color(r, new hitable_list(local_world, 3), 0);
                }
                hitted.clear();
                // color += get_color(r, world, 0);
            }

            color /= NUM_SAMPLES;
            color = vec3(sqrt(color[0]),sqrt(color[1]),sqrt(color[2])); // gamma 2 correction

            int ir = int(255.99*color[0]);
            int ig = int(255.99*color[1]);
            int ib = int(255.99*color[2]);

            // clamp
            ir = ir > 255 ? 255 : ir;
            ig = ig > 255 ? 255 : ig;
            ib = ib > 255 ? 255 : ib;

            image<<ir<<" "<<ig<<" "<<ib<<"\n";
        }
    }

    image.close();
    return 0;
}
