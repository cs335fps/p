
#include "object.h"

Texture::Texture() {
    pattern = Texture::NONE;
    intensity = 0.0;
    reflection = 0.0;
    
}

void Object::Speak(){printf("Object\n");}
hit Object::Trace(Vec, Vec) {
    return hit(0);
}
void Object::SetNormal(Vec) {}
Vec Object::GetNormal(){ return Vec(0.0,0.0,0.0); }
void Object::SetNormal(float, float, float) {}
void Object::Setu(Vec) {}
void Object::SetRadius(float) {}
void Object::SetIntensity(float) {}
void Object::AddVert(float, float, float) {}
void Object::AddVert(Vec) {}
void Object::SetVert(int, float, float, float) {}
void Object::SetVert(Vec, Vec, Vec) {}
Vec Object::Color(hit) { return Vec(0.0,0.0,0.0); }

void Light::SetIntensity(float i) {
    intensity = i;
}

void Sphere::SetIntensity(float i) {
    texture.intensity = i;
}

void Plane::SetIntensity(float i) {
    texture.intensity = i;
}

void Triangle::SetIntensity(float i) {
    texture.intensity = i;
}

void Poly::SetIntensity(float i) {
    texture.intensity = i;
}

void Light::SetRadius(float r) { radius = r; }

Light::Light() {
    radius = 0.0;
}

Vec Light::RandomSpot() {
    Vec v = origin;
    v[0] += (RAND * radius * 2.0) - (radius);
    v[1] += (RAND * radius * 2.0) - (radius);
    v[2] += (RAND * radius * 2.0) - (radius);
    return v;
}

void Plane::Speak(){printf("Plane\n");}

hit Plane::Trace(Vec o, Vec d) {
    Vec location;
    d.normalize();
    Vec n = normal.GetNormalized();
    float dDotNormal = d.dot(n);
    if (dDotNormal == 0.0)
        return hit(0);
    float t = -1.0 * (o - origin).dot(n) / dDotNormal;
    if (t < 0.0) {
        return hit(0);
    }
    
    location = o + (d * t);
    
    return hit(this, location, t, n);
}

Vec Plane::Color(hit t) {
    Vec location = t.location;
    Vec color = texture.color;
    if (texture.pattern == Texture::CHECKER) {
        int col = 0;
        for (int i = 0; i <3; i++){
            if ((int)((location[i] + 0.001) / texture.scale) % 2)
                col ^= 1;
            if (location[i] < 0.001)
                col ^= 1;
        }
        if (!col)
            color = texture.color2;
    }
    return color;
}

void Plane::SetNormal(Vec v) { normal = v; }
void Plane::SetNormal(float x, float y, float z) { normal.set(x,y,z); }
Vec Plane::GetNormal() { return normal; }
void Plane::Setu(Vec v) { u = v; }
Vec Plane::Getu() { return u; }


Object::Geometry Triangle::type(){return TRIANGLE;}
void Triangle::Speak(){printf("Triangle\n");}

void Triangle::AddVert(float x, float y, float z) {
    vert[2] = vert[1];
    vert[1] = vert[0];
    vert[0] = Vec(x,y,z);
    CalcNorm();
}

void Triangle::SetVert(int i, float x, float y, float z) {
    vert[i] = Vec(x,y,z);
    CalcNorm();
}

void Triangle::SetVert(Vec x, Vec y, Vec z) {
    vert[0] = x;
    vert[1] = y;
    vert[2] = z;
    CalcNorm();
}


void Triangle::AddVert(Vec v) {
    vert[2] = vert[1];
    vert[1] = vert[0];
    vert[0] = v;
    CalcNorm();
}

void Triangle::CalcNorm() {
    normal = (vert[0] - vert[1]).cross(vert[0] - vert[2]);
    origin = vert[0];
}

hit Triangle::Trace(Vec o, Vec d) {
    Vec location;
    d.normalize();
    Vec n = normal.GetNormalized();
    float dDotNormal = d.dot(n);
    if (dDotNormal == 0.0)
        return hit(0);
    //if (dDotNormal < 0.0)
    //    n = n * -1.0;
    float t = -1.0 * (o - origin).dot(n) / dDotNormal;
    if (t < 0.0) {
        return hit(0);
    }
    location = o + (d * t);
    Vec b = Barycentric(location);
    if (Inside(b))
        return hit(this, location, t, n, b);
    else
        return hit(0);
}

bool Triangle::Inside(Vec v) {
    return (v[0] >= 0.0 && v[1] >= 0.0 && v[2] >= 0);
}

Vec Triangle::Barycentric(Vec location) {
    float ABC = normal.dot((vert[1] - vert[0]).cross(vert[2] - vert[0]));
    float PBC = normal.dot((vert[1] - location).cross(vert[2] - location));
    float PCA = normal.dot((vert[2] - location).cross(vert[0] - location));
    float u = PBC / ABC;
    float v = PCA / ABC;
    return Vec(u, v, 1.0 - u - v);
}

Vec Triangle::Color(hit t) {
    if (texture.pattern == Texture::IMAGE) {
        float u = t.barycentric[0];
        float v = t.barycentric[1];
        float w = t.barycentric[2];
        float x = u * texture.imageVert[0][0] + v * texture.imageVert[1][0] + w * texture.imageVert[2][0];
        float y = u * texture.imageVert[0][1] + v * texture.imageVert[1][1] + w * texture.imageVert[2][1];
        x = x * (float)texture.image->width;
        y = y * (float)texture.image->height;
        return texture.image->getPixel(x,y);
    } else {
        return texture.color;
    }
}

void Poly::Speak(){printf("Poly\n");}


void Sphere::Speak(){printf("Sphere\n");}

void Sphere::SetRadius(float r) { radius = r; }

hit Sphere::Trace(Vec o, Vec d) {
    d.normalize();
    float a = d.dot(d);
    float b = (d.dot(o - origin));
    float c = (o - origin).dot(o - origin) - radius * radius;
    float s = b*b - a * c;
    if (s < 0.0)
        return hit(0);
    float d1 = (-1.0 * b + sqrt(s)) / a;
    float d2 = (-1.0 * b - sqrt(s)) / a;
    
    if (d2 < d1 && d2 > 0.0)
        d1 = d2;
    if (d1 < 0.0)
        return hit(0);
    Vec location = (d * d1) + o;
    
    Vec n = location - origin;
    n.normalize();
    
    
    return hit(this, location, d1, n);
}

Vec Sphere::Color(hit t) {
    Vec color = texture.color;
    Vec n = t.normal;
    
    if (texture.pattern == Texture::CHECKER) {
        int col = 0;
        float x = n.AngleX();
        float y = n.AngleY();
        
        if ((int)((x + 0.001) / texture.scale) % 2)
            col ^= 1;
        if ((int)((y + 0.001) / texture.scale) % 2)
            col ^= 1;
        
        
        if (!col)
            color = texture.color2;
    } else if (texture.pattern == Texture::IMAGE) {
        
        float x = n.AngleX();
        float y = n.AngleY();
        
        if(texture.image != NULL) {
            x = x * (float)texture.image->width / 2.0;
            y = (float)texture.image->height - y * (float)texture.image->height;
            return texture.image->getPixel(x,y);
        }
    }
    return color;
}
