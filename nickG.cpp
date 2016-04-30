// Nicholas Gardner
#include "vec.h"

float& Vec::operator[] (const int index)
{
    if (index == 0) {
        return x;
    
    } else if (index == 1) {
        return y;
    
    } else if (index == 2) {
        return z;
    
    } else {
        int i = index % 3;
        return (*this)[i];
    }
}

Vec::Vec()
{
    x = y = z = 0.0f;
}

Vec::Vec(float a, float b)
{
    Vec(a, b, 0.0f);
}

Vec::Vec(float a, float b, float c)
{
    x = a;
    y = b;
    z = c;
}

Vec::Vec(const Vec &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

Vec Vec::operator+(const Vec &v)
{
    Vec c;
    c.x = x + v.x;
    c.y = y + v.y;
    c.z = z + v.z;
    return c;
}

Vec Vec::operator-(const Vec &v)
{
    Vec c;
    c.x = x - v.x;
    c.y = y - v.y;
    c.z = z - v.z;
    return c;
}

Vec Vec::operator-()
{
    Vec c;
    c.x = -x;
    c.y = -y;
    c.z = -z;
    return c;
}

Vec Vec::operator*(const float &f)
{
    Vec c;
    c.x = x * f;
    c.y = y * f;
    c.z = z * f;
    return c;
}

Vec Vec::operator/(const float &f)
{
    Vec c;
    c.x = x / f;
    c.y = y / f;
    c.z = z / f;
    return c;
}

Vec& Vec::operator=(const Vec &vSource)
{
    x = vSource.x;
    y = vSource.y;
    z = vSource.z;
    return *this;
}

Vec operator*(float f, Vec v)
{
    Vec c;
    c.x = v.x * f;
    c.y = v.y * f;
    c.z = v.z * f;
    return c;
}

Vec operator/(float f, Vec v)
{
    Vec c;
    c.x = f / v.x;
    c.y = f / v.y;
    c.z = f / v.z;
    return c;
}

float Vec::Magnitude(){
    return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}

float Vec::Dot(Vec b){
    return x * b.x + y * b.y + z * b.z;
}

Vec Vec::Cross(Vec b){
    Vec c;
    c.x = y * b.z - z * b.y;
    c.y = z * b.x - x * b.z;
    c.z = x * b.y - y * b.x;
    return c;
}

Vec Vec::Norm(){
    float m = Magnitude();
    Vec c(x/m, y/m, z/m);
    return c;
}

Vec Vec::Normalize(){
    float m = Magnitude();
    x = x/m;
    y = y/m;
    z = z/m;
    return *this;
}

Vec Normal(Vec a, Vec b)
{
    return a.Cross(b).Norm();
}

Vec Normal(Vec a, Vec b, Vec c)
{
    Vec u(a-b);
    Vec v(a-c);
    return Normal(u, v);
}

float Dot(Vec a, Vec b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec Cross(Vec a, Vec b)
{
    Vec c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
}
