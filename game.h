#ifndef _GAME_H_
#define _GAME_H_

#include "vec.h"
#include "nickG.h"
#include "WorldEngine.h"
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
    int zoom;
    float depth;
    
    std::vector<Wall> walls;
    
    Game();
    void Move();
    
};

#endif
