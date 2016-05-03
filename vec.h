#ifndef _VEC_H_
#define _VEC_H_

#include <math.h>

class Vec
{
    private:    

    public:
        float x, y, z;

        // Initialize vector to 0,0,0
        Vec();

        // Initialize vector to a,0,0
        Vec(float a);

        // Initialize vector to a,b,0
        Vec(float a, float b);

        // Initialize vector to a,b,c
        Vec(float a, float b, float c);

        // Initialize vector as a copy of Vec v
        Vec(const Vec &v);

        // Overload subscript. [0] = x, etc.
        float& operator[] (const int index);

        //overload Vec + Vec
        Vec operator+(const Vec &v);

        //overload Vec - Vec
        Vec operator-(const Vec &v);

        //overload -Vec
        Vec operator-();

        // overload Vec * float
        Vec operator*(const float &f);

        // overload Vec / float
        Vec operator/(const float &f);

        // overload Vec = Vec
        Vec& operator=(const Vec &vSource);

        // Overload float * Vec
        friend Vec operator*(float f, Vec v);

        // overload float / Vec
        friend Vec operator/(float f, Vec v);

        // returns magnitude of a vector
        float Magnitude();

        // returns dot product of two vectors Vec.Dot(Vec)
        float Dot(Vec b);

        // return cross product of two vectors Vec.Cross(Vec)
        Vec Cross(Vec b);

        // returns directional unit vector of vector (length 1)
        Vec Norm();

        // normalizes the vector, makes the length 1 but keeps direction
        Vec Normalize();
};

// Returns the normal of the plane made by two vectors
Vec Normal(Vec a, Vec b);

// Returns the normal of the plane made by the endpoints of three vectors
Vec Normal(Vec a, Vec b, Vec c);

// returns the dot product of two vectors similar to Vec.Dot(Vec)
float Dot(Vec a, Vec b);

// returns the cross product of two vectors similar to Vec.Cross(Vec)
Vec Cross(Vec a, Vec b);

#endif
