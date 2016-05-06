#include "vector.h"

float Vec::AngleX() {
    float a = atan2(x[1],x[0]) / 3.141593;
    if (a < 0.0)
        a += 2.0;
    return a;
}

float Vec::AngleY() {
    Vec z(0.0,0.0,1.0);
    float a = asin(fabs(dot(z))/(magnitude())) / 3.141593 + 0.5;
    if (x[2] < 0.0)
        a = 1.0 - a;
    return a;
}

float& Vec::operator[] (int i) { return x[i]; }
Vec::Vec() {
    x[0] = x[1] = x[2] = 0.0;
}

Vec::Vec(float a, float b, float c) {
    x[0] = a;
    x[1] = b;
    x[2] = c;
}

void Vec::set(float a, float b, float c) {
    x[0] = a;
    x[1] = b;
    x[2] = c;
}

float Vec::magnitude() {
    return sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
}

void Vec::normalize() {
    float m = magnitude();
    x[0] /= m;
    x[1] /= m;
    x[2] /= m;
}

Vec Vec::GetNormalized() {
    Vec r = *this;
    float m = r.magnitude();
    r[0] /= m;
    r[1] /= m;
    r[2] /= m;
    return r;
}

Vec Vec::cross(Vec v) {
    Vec c;
    c.x[0] = x[1] * v.x[2] - x[2] * v.x[1];
    c.x[1] = x[2] * v.x[0] - x[0] * v.x[2];
    c.x[2] = x[0] * v.x[1] - x[1] * v.x[0];
    return c;
}

Vec Vec::blend(Vec v, float f) {
    Vec c;
    for (int i = 0; i < 3; i++) {
        c.x[i] = pow(pow(x[i],f)+pow(v.x[i],f),(1.0/f));
    }
    return c;// / 2.0;
}

float Vec::dot(Vec v) {
    float prod = x[0] * v.x[0] + x[1] * v.x[1] + x[2] * v.x[2];
    return prod;
}

Vec::Vec(const Vec &v) {
    x[0] = v.x[0];
    x[1] = v.x[1];
    x[2] = v.x[2];
}

Vec& Vec::operator=(const Vec &v) {
    x[0] = v.x[0];
    x[1] = v.x[1];
    x[2] = v.x[2]; 
    return *this;
}

Vec Vec::operator+(const Vec &v) {
    Vec c;
    c.x[0] = x[0] + v.x[0];
    c.x[1] = x[1] + v.x[1];
    c.x[2] = x[2] + v.x[2];
    return c;
}

Vec Vec::operator-(const Vec &v) {
    Vec c;
    c.x[0] = x[0] - v.x[0];
    c.x[1] = x[1] - v.x[1];
    c.x[2] = x[2] - v.x[2];
    return c;
}
Vec Vec::operator*(const float &f) {
    Vec c;
    c.x[0] = x[0] * f;
    c.x[1] = x[1] * f;
    c.x[2] = x[2] * f;
    return c;
}
Vec Vec::operator/(const float &f) {
    Vec c;
    c.x[0] = x[0] / f;
    c.x[1] = x[1] / f;
    c.x[2] = x[2] / f;
    return c;
}
Vec Vec::operator*=(const float &f) {
    x[0] = x[0] * f;
    x[1] = x[1] * f;
    x[2] = x[2] * f;
    return *this;
}

void Vec::print() {
    printf("{%f,%f,%f}\n",x[0],x[1],x[2]);
}
