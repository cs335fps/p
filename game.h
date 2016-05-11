#ifndef _GAME_H_
#define _GAME_H_

#include "vec.h"
#include "nickG.h"
#include "WorldEngine.h"
#include "portal.h"
#include <vector>

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
    
    float maxZoom;
    float minZoom;
    int zoom, togPortal;
    float depth;
    
    std::vector<Wall> walls;
    portal defaultPortl;
    worldEngine floor;
    
    Game();
    void Move();
    
};

#endif
