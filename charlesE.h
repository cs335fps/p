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
class Mob
{
private:
    float hp;
    Vec location;
    solidSphere body;
public:
    Mob();
    void spawn();
    void death();
    void damage(int health);
    void move();
    void render();
};

class Enemy: public Mob
{

    void move();
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
vec MakeVector(float, float, float);
#endif
