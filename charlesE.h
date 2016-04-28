#ifndef _ai_h_
#define _ai_h_

class mob
{
    float hp;

    void __mob();
    void spawn(vec location);
    void die();
    void damage(int health);
    void move();
}

class enemy: public mob
{

    void move;
}
class munition: public mob
{

}
class npc: public mob
{

}
#endif
