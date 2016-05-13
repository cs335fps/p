#ifndef _NICKG_H_
#define _NICKG_H_

#include <ctime>
#include <GL/glx.h>
#include "vec.h"
#include <stdlib.h>

#define RAND ((float)rand()/RAND_MAX)

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
        float length;
        Vec c[8]; // corners
        Vec v[2]; // endpoints


        Vec color;
    public:
        Wall();
        Wall(Vec, Vec, float, float, Vec col = Vec(1,1,1));
        void Set(Vec, Vec, float, float, Vec);
        void Draw();
        int Collide(Vec *);

};

#endif
