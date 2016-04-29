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
   #include "input.h"
#endif
#ifndef _SOLIDSPHERE_H_
    #include "solidSphere.h"
#endif
class mob
{
private:
    float hp;
    vec loc;
public:
    mob();
    void spawn(Vec location);
    void die();
    void damage(int health);
    void move();
    void render();
};

class enemy: public mob
{

    void move();
};
class munition: public mob
{

};
class npc: public mob
{

};
#endif
