// Charles Enright
// CS 335 Software Engineering
// File includes header portion of AI and elements of game mechanics.


#ifndef _ai_h_
#define _ai_h_

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

class Game;
class Mob
{
private:
    float hp;
    Vec location;
    Vec velocity;
    Vec maxSpeed;
    solidSphere body;
   
public:
    Mob();
    void spawn();
    void death();
    void damage(int health);
    void move();
    virtual void render();
    void move(Game);
    virtual int Collide(Vec*);
};

class Enemy: public Mob
{

    void move();
};
class cWall: public Mob{
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
	cWall(Vec, Vec, float, float);
	void Set(Vec, Vec, float, float);
	void Draw();
	int Collide(Vec*, float);
};

class Wall : public Mob
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
};

class Munition: public Enemy
{

};
class Npc: public Mob
{

};


void drawLightedArm();
void drawFloor();
void box(float, float, float);
Vec MakeVector(float, float, float);
#endif
