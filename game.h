#ifndef _GAME_H_
#define _GAME_H_

#include "vec.h"
#include "nickG.h"
#include "WorldEngine.h"
#include "portal.h"
#include <vector>
#include <stdlib.h>
#define RAND ((float)rand()/RAND_MAX)
#define RCOLOR Vec(RAND,RAND,RAND)
class Mob;
class Wall;
class Game
{
private:
    float tmpPos[3];
public:
    Vec position;
    Vec direction;
    
    int moveX;
    int moveY;
    float velocityX;
    float velocityY;
    int aiming;
    int nbullets;
    int maxbullets;
    
    float maxZoom;
    float minZoom;
    int zoom, togPortal;
    float depth;
    
    vector<Mob*> mobs;
    
    std::vector<Wall*> walls;
    portal defaultPortl;
    worldEngine floor;
    
    Game();
    void Move();
    
};

#endif
