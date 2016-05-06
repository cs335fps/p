#ifndef _SCENE_H_
#define _SCENE_H_

#include <limits>
#include <cstdlib>
#include <vector>
#include "object.h"
#include "vector.h"
#include "fimage.h"
#include <time.h>

#define RAND (float)rand()/RAND_MAX
#define INF std::numeric_limits<float>::infinity()
#define PI 3.14159265359;


using namespace std;

class Object;
class Light;
class hit;

class Scene {
public:
    
    vector<Object*> Objects;
    vector<Light*> Lights;
    vector<fimage*> Textures;
    
    fimage image;
    
    char output[64];

    float windowScale;
    float scale; 
    float superSample;
    float shadowSample;
    int recursion;
    int sampleMethod;
    int softShadows;
    int occlusion;
    Vec win; // window reletive to pov
    Vec pov; // pov
    Vec p;   // aboslute center point of window
    Vec u, v;
    
    Vec ambient;    
    
    Scene();
    
    fimage *AddTexture(const char *c);
    
    void setRes(int w, int h, int d);
    
    void setCamera(Vec origin, Vec focus, float s);
    
    Scene(int w, int h, int d);
    
    void windowVect();
    
    Vec getPixelCoordinate(float x, float y);
    
    void getPixelVector(float x, float y, Vec *Rp, Vec *RA);
    
    Light* newLight();
    
    template<typename T>
    T *newObject() {
        T *p = new T;
        Objects.push_back(p);
        return p; 
    }
    
    void drawScene();
    float SampleLight(hit*, Vec*, Light*, int);
    float Occlusion(hit h, Vec d);
    Vec TraceLight(hit, Vec);
    hit Closest(Vec, Vec);
    Vec Cast(Vec o, Vec d, int depth);
    
};



#endif
