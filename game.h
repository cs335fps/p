#ifndef _GAME_H_
#define _GAME_H_

#include "nickG.h"
#include "vec.h"
#include "WorldEngine.h"
#include "portal.h"
#include <vector>
#ifndef _mob_h_
	#include <stdlib.h>
#endif


#define RAND ((float)rand()/RAND_MAX)
#define RCOLOR Vec(RAND,RAND,RAND)
class Mob;
class Wall;
class BulletHole;
class Bullet;
class Game
{
private:
    float tmpPos[3];
public:
    Vec position;
    Vec direction;
    
    vector<Bullet> bullets;
    vector<BulletHole> bulletHoles;
    
    int moveX;
    int moveY;
    float velocityX;
    float velocityY;
    int aiming;
    int nbullets;
    int maxbullets;
    int nkills;
    int guntype;
    int hitAnim;
    
    int partyMode;
    
    float maxZoom;
    float minZoom;
    int zoom, togPortal, setPortal;
    float depth;
    
    vector<Mob*> mobs;
    vector<Wall> walls;

    portal defaultPortl;
    worldEngine floor;
    
    Game();
    void Move();
    
    void Shoot();
    
};

#endif
