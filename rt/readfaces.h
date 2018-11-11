#ifndef READFACESH
#define READFACESH

#include <fstream>
#include <sstream>

void readfile(){

    ifstream objfile;
    objfile.open ("./models/cube.txt");
    string line;
    vec3 v1, v2, v3;
    float s = 1;
    vec3 offset = vec3(0,0,1);
    int READ_LENGTH = 12; // 720
    hitable *mesh[READ_LENGTH];
    if (!objfile){
        cerr<<"file not found";
        exit(0);
    }
    int i = 0;
    while(i<READ_LENGTH && getline(objfile, line)){
        stringstream ss(line);
        ss>>v1>>v2>>v3;
        triangle* temp = new triangle(s*v1-offset, s*v2-offset, s*v3-offset, new lambertian( new constant_texture(vec3(0.4, 0.1, 0.3))));
        root.add_triangle(temp);
        mesh[i++] = temp;
        // cout<<i<<"th triangle\n";
    }
    objfile.close();
}

#endif