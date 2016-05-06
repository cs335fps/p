#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <cmath>
#include "vector.h"
#include "fimage.h"
#include "scene.h"

#define RAND (float)rand()/RAND_MAX

//class Scene;
struct hit;

class Texture {
public:
    Texture();
    enum Pattern{NONE,CHECKER,IMAGE};
    Vec color;
    Vec color2;
    Vec color3;
    float reflection;
    float opacity;
    float refraction;
    Pattern pattern;
    float scale;
    fimage* image;
    float imageVert[3][2];
    float intensity;
};

class Object {
public:
    enum Geometry{NONE,PLANE,TRIANGLE,POLY,SPHERE};
    Texture texture;  
    Vec origin; 
    virtual void Speak();
    virtual hit Trace(Vec, Vec);
    virtual void SetNormal(Vec);
    virtual Vec GetNormal();
    virtual void SetNormal(float, float, float);
    virtual void Setu(Vec);
    virtual void SetRadius(float);
    virtual void SetIntensity(float);
    virtual void AddVert(float, float, float);
    virtual void AddVert(Vec);
    virtual void SetVert(int, float, float, float);
    virtual void SetVert(Vec, Vec, Vec);
    virtual Vec Color(hit);
};

class Plane : public Object {
    
    Vec normal;
    Vec u; // rotation
public:
    hit Trace(Vec o, Vec d);
    
    void SetNormal(Vec v);
    void SetNormal(float x, float y, float z);
    Vec GetNormal();
    void Setu(Vec v);
    Vec Getu();
    Vec Color(hit);
    void Speak();
    void SetIntensity(float);
};

class Triangle : public Object {
public:
    Vec vert[3];
    Vec normal;
    Geometry type();
    void Speak();
    void AddVert(float, float, float);
    void AddVert(Vec);
    void SetVert(int, float, float, float);
    void SetVert(Vec x, Vec y, Vec z);
    void CalcNorm();
    
    hit Trace(Vec o, Vec d);
    Vec Color(hit);
    
    bool Inside(Vec v);
    Vec Barycentric(Vec);
    void SetIntensity(float);
};

class Poly : public Object {
public:
    Vec *vertices;
    float nVertices;
    Vec normal;
    void Speak();
    void SetIntensity(float);
};

class Sphere : public Object {
public:
    float radius;
    Vec u; //rotation
    Vec v; //rotation
    
    void SetRadius(float r);
    
    hit Trace(Vec o, Vec d);
    Vec Color(hit);
    void Speak();
    void SetIntensity(float);
};

class Light : public Object {
public:
    float intensity;
    void SetIntensity(float);
    void SetRadius(float r);
    Vec RandomSpot();
    float radius;
    Light();
};

struct hit {
    Vec location;
    Vec normal;
    float distance;
    bool contact;
    Object *object;
    Vec barycentric;
    hit(Object * o, Vec l, float d, Vec n) {
        object = o;
        location = l;
        normal = n;
        distance = d;
        contact = 1;
    }
    hit(Object * o, Vec l, float d, Vec n, Vec b) {
        barycentric = b;
        object = o;
        location = l;
        normal = n;
        distance = d;
        contact = 1;
    }
    hit(bool c) {
        contact = c;
        object = NULL;
    }
};

#endif
