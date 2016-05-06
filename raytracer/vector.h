#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cstdio>
#include <cmath>

class Vec {
private:
    float x[3];
public:
    float AngleX();
    float AngleY();
    float& operator[] (int i);
    Vec();
    
    Vec(float a, float b, float c);
    
    void set(float a, float b, float c);
    
    float magnitude();
    
    void normalize();
    
    Vec GetNormalized();
    
    Vec cross(Vec v);
    Vec blend(Vec, float);
    
    float dot(Vec v);
    
    Vec(const Vec &v);
    
    Vec& operator=(const Vec &v);
    
    Vec operator+(const Vec &v);
    
    Vec operator-(const Vec &v);
    Vec operator*(const float &f);
    Vec operator/(const float &f);
    Vec operator*=(const float &f);
    
    void print();
};

#endif
