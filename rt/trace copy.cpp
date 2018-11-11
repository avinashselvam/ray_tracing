#include <iostream>
#include <fstream> // to write an .ppm image file
#include "float.h" // to use MAXFLOAT
#include <random>
#include <sstream>


#include "camera.h"
#include "sphere.h"
#include "triangle.h"
#include "texture.h"
#include "material.h"
#include "hitablelist.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


using namespace std;

#define NUM_BOUNCES 3
#define NUM_SAMPLES 10

/*
to get color from cast ray
if -> recursively send diffuse reflections
else -> weird way to make background gradient as fn of direction of cast ray
*/
vec3 get_color(const ray& r, hitable *world, int j){
    hit_record rec;
    if(world->hit(r, 0, MAXFLOAT, rec)){
        return vec3(0.5, 0.5, 0.5);
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered) && j < NUM_BOUNCES){
            return emitted + attenuation*get_color(scattered, world, ++j);
        }
        else{
            return emitted;
        }
    }
    else{
        // vec3 unit_direction = unit_vector(r.direction());
        // float t = 0.5*(unit_direction.y() + 1.0); // y lies b/w -1 and 1
        // return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.4, 0.7, 1.0);
        return vec3(0,0,0);    
    }
    
}

int main()
{
    
    // file reading
    ifstream objfile;
    objfile.open ("./models/tree.txt");
    string line;
    vec3 v1, v2, v3;
    vec3 offset = vec3(0,0,3);
    int READ_LENGTH = 130; // 130
    hitable *mesh[READ_LENGTH+1];
    if (!objfile){
        cerr<<"file not found";
        exit(0);
    }
    int i = 0;
    while(i<READ_LENGTH && getline(objfile, line)){
        stringstream ss(line);
        ss>>v1>>v2>>v3;
        mesh[i++] = new triangle(v1-offset, v2-offset, v3-offset, new lambertian( new constant_texture(vec3(0.4, 0.1, 0.3))));
        cout<<i<<"th triangle\n";
    }
    objfile.close();
    // end of file reading

    std::string inputfile = "./models/tree.obj";
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
    
    if (!err.empty()) { // `err` may contain warning message.
    std::cerr << err << std::endl;
    }

    if (!ret) {
    exit(1);
    }

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
            // access to vertex
            tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
            tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
            tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
            tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
            tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
            tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
            tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
            tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
            tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];

            cout<<vx<<vy<<vx;
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }


    // dimensions of image
    int nx = 200;
    int ny = 200;

    camera cam(vec3(0,0,0), vec3(0,0,-1), vec3(0,1,0), 90, float(nx)/float(ny));

    ofstream image;
    image.open("output.ppm");
    image<<"P3\n"<<nx<<" "<<ny<<"\n255\n";
    
    // setting up world
    hitable *list[6];
    list[0] = new sphere(vec3(0,1,-3), 0.2, new diffuse_light( new constant_texture(vec3(16, 16, 16))));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian( new constant_texture(vec3(0.4, 0.1, 0.3))));
    list[4] = new triangle(vec3(0,1,-3), vec3(1,1,-3), vec3(1,1,-4), new diffuse_light( new constant_texture(vec3(4, 4, 4))));
    list[5] = new triangle(vec3(0,1,-1), vec3(0,1,-2), vec3(1,1,-2), new diffuse_light( new constant_texture(vec3(4, 4, 4))));    
    list[3] = new sphere(vec3(0,0,-1), 0.3, new metal(vec3(0.4, 0.4, 0.4)));    
    list[2] = new sphere(vec3(-1,0,-1), 0.3, new lambertian( new constant_texture(vec3(0.4, 0.1, 0.3))));
    // list[0] = new triangle(vec3(0,0,-1), vec3(0,1,-1), vec3(1,0,-1));
    mesh[READ_LENGTH] = list[4];
    // hitable *world = new hitable_list(mesh, READ_LENGTH+1);
    hitable *world = new hitable_list(mesh, READ_LENGTH);

    
    // ray casting
    for(int i=ny-1; i>=0; i--){
        
        cout<<i<<"\n";
        
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
                color += get_color(r, world, 0);
            }

            color/=NUM_SAMPLES;
            color = vec3(sqrt(color[0]),sqrt(color[1]),sqrt(color[2])); // gamma 2 correction

            int ir = int(255.99*color[0]);
            int ig = int(255.99*color[1]);
            int ib = int(255.99*color[2]);

            ir = ir > 255 ? 255 : ir;
            ig = ig > 255 ? 255 : ig;
            ib = ib > 255 ? 255 : ib;

            image<<ir<<" "<<ig<<" "<<ib<<"\n";
        }
    }

    image.close();
    return 0;
}
