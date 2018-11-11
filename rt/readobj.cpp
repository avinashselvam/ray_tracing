#include <iostream>
#include <fstream> // to write an .ppm image file
#include <string>
#include "float.h" // to use MAXFLOAT
#include <random>
#include "camera.h"
#include "triangle.h"
#include "hitablelist.h"
#include <sstream>

using namespace std;

int main(){

    ifstream objfile;
    objfile.open ("car.txt");
    string line;

    vec3 v1, v2, v3;

    hitable *mesh[2718];
    
    if (!objfile){
        cerr<<"file not found";
        exit(0);
    }

    int i = 0;

    while(i<2718 && getline(objfile, line)){
        cout<<"hi";
        stringstream ss(line);
        ss>>v1>>v2>>v3;
        mesh[i] = new triangle(v1, v2, v3);
        cout<<mesh[i++];
    }

    cout<<sizeof(mesh);

    objfile.close();

    return 0;
}

 // file reading
    ifstream objfile;
    objfile.open ("cube.txt");
    string line;
    vec3 v1, v2, v3;
    vec3 offset = vec3(0,0,300);
    int READ_LENGTH = 12;
    hitable *mesh[READ_LENGTH];
    if (!objfile){
        cerr<<"file not found";
        exit(0);
    }
    int i = 0;
    while(i<READ_LENGTH && getline(objfile, line)){
        stringstream ss(line);
        ss>>v1>>v2>>v3;
        mesh[i++] = new triangle(v1-offset, v2-offset, v3-offset);
        cout<<i<<"th triangle\n";
    }
    objfile.close();
// end of file reading