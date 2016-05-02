#ifndef _GAME_H_
#define _GAME_H_

#include "vec.h"

class Game
{
private:

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
    
    Game();
    void Move();
    
};

#endif
