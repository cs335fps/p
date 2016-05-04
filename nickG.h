#ifndef _NICKG_H_
#define _NICKG_H_

#include <ctime>
#include <GL/glx.h>
#include "vec.h"

class Seconds
{
    struct timespec startTime;
    struct timespec curTime;

    public:
    Seconds();
    void Start();
    double Get();

};

class Wall
{
    private:
        Vec start;
        Vec end;
        float height;
        float width;
        Vec c[8]; // corners
        Vec v[2]; // endpoints


        Vec color;
    public:
        Wall();
        Wall(Vec, Vec, float, float);
        void Set(Vec, Vec, float, float);
        void Draw();
        int Collide(Vec *);

};

#endif
