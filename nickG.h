#ifndef _NICKG_H_
#define _NICKG_H_

#include <ctime>
#include <GL/glx.h>
#include "vec.h"
#include <stdlib.h>
#include "charlesE.h"

#define RAND ((float)rand()/RAND_MAX)
int hash(int *, int);
class Game;
class Bullet
{
public:
    Vec start;
    Vec end;
    int age;
    
    void render();
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
        void render();
        int Collide(Vec *);
        void death();
        Game* game;
};

class Seconds
{
    struct timespec startTime;
    struct timespec curTime;

    public:
    Seconds();
    void Start();
    double Get();

};


#endif

