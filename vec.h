#ifndef _VEC_H_
#define _VEC_H_

#include <math.h>
#include <iostream>

class Vec
{
    private:    

    public:
        float x, y, z;

        Vec();

        Vec(float a);

        Vec(float a, float b);

        Vec(float a, float b, float c);

        Vec(const Vec &v);

        float& operator[] (const int index);

        Vec operator+(const Vec &v);

        Vec operator-(const Vec &v);

        Vec operator-();

        Vec operator*(const float &f);

        Vec operator/(const float &f);

        Vec& operator=(const Vec &vSource);

        friend Vec operator*(float f, Vec v);

        friend Vec operator/(float f, Vec v);

        float Magnitude();

        float Dot(Vec b);

        Vec Cross(Vec b);

        Vec Norm();

        Vec Normalize();
        
        void Print();

};

Vec Normal(Vec a, Vec b);

Vec Normal(Vec a, Vec b, Vec c);

float Dot(Vec a, Vec b);

Vec Cross(Vec a, Vec b);

#endif
