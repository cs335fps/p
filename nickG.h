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
        void death();
        Game* game;
		vector<Vec> GetPoints(double units = 1.0);
		int Ray(Vec origin, Vec direction, float * closest);
        void SetHeight(float);
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

