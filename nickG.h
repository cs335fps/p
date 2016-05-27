#ifndef _NICKG_H_
#define _NICKG_H_

#include <ctime>
#include <GL/glx.h>
#include "vec.h"
#include <stdlib.h>
#include "charlesE.h"
#include <fstream>
#include <string>
#include <vector>

#define RAND ((float)rand()/RAND_MAX)
int hash(int *, int);

int RaySphere(Vec rayOrigin, Vec rayDirection, 
        Vec sphereOrigin, float sphereRadius, float *closest);
class Game;
void DrawCrosshairs(Game *game, int w, int h);
void DrawHealth(Game* game, int w, int h);
void DrawAmmo(Game* game, int w, int h);
void ParseLevel(const char* fileName, Game* game, float);
vector<string> Split(string, string);

Vec Reflect(Vec dir, Vec norm);

class BulletHole
{
    public:
        Vec origin;
        Vec normal;
        void render();
        void render2();
        BulletHole(Vec, Vec);
};

class Bullet
{
    public:
        Vec origin;
        Vec direction;
        Vec end;
        int age;

        void render();
};

class Wall
{
    private:
        Vec start;
        Vec end;
        float width;
        float length;
        Vec c[8]; // corners
        Vec v[2]; // endpoints
        Vec color;
    public:
        float height;
        Wall();
        Wall(Vec, Vec, float, float, Vec col = Vec(1,1,1));
        void Set(Vec, Vec, float, float, Vec);
        void render();
        int Collide(Vec *);
        int Collide(Vec *, float, Vec*);
        void death();
        Game* game;
        vector<Vec> GetPoints(double units = 1.0);
        int Ray(Vec origin, Vec direction, 
                float * closest, Vec * normal = NULL);
        void SetHeight(float);
        ~Wall();
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



