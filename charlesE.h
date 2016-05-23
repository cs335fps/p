// Charles Enright
// CS 335 Software Engineering
// File includes header portion of AI and elements of game mechanics.
#ifndef _mob_h_
#define _mob_h_

#ifndef _VEC_H_
    #include "vec.h"
#endif

#ifndef _VIEW_H_
    #include "view.h"
#endif
#ifndef _INPUT_H_
   //#include "input.h"
#endif

#ifndef _SOLIDSPHERE_H_
    #include "solidSphere.h"
#endif
#ifndef _GAME_H_
//	#include <cstdlib>
#endif
#ifndef OPENAL_H
//    #include <unistd>
#endif
#ifndef _MAP_h
    #include "map.h"
#endif

float r(float, float);
Vec MakeVector(float, float, float);
class Game;

void respawn_mobs(Game*, int);
void chadKey(Game* g);
class Mob
{
private:
    int id;
    float hp;
    Vec location;
    Vec velocity;
    Vec maxSpeed;
    solidSphere body;
    int moved;
    unsigned int texture;
public:
    Mob();
    Mob(int, Vec*);
    void spawn(Vec*);
    void death(Game*);
    void damage(int, Game*);
    void move();
    virtual void render();
    void move(Game*);
    virtual int Collide(Vec*); 
    Vec* getLoc() { return &location; }
    void setLocY(float y = 1);
    void setVelY(float y = 0);
    unsigned int getTex() { return texture; }
    void setTex(unsigned int t); 
    bool operator==(int b);
    bool operator==(Mob* b);
    virtual ~Mob();
};
/*
class Enemy: public Mob
{
    void render();
    void move();
};*/
class cWall: public Mob
{
    private:
	Vec start;
	Vec end;
	float height;
	float width;
	Vec corners[8];
	Vec endpoints[2];
	Vec color;

    public:
	cWall();
	cWall(int, Vec*);
	cWall(Vec, Vec, float, float);
	void Set(Vec, Vec, float, float);
	void Draw();
	int Collide(Vec*, float);
};



class Npc: public Mob
{

};
#endif
