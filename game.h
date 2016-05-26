#ifndef _GAME_H_
#define _GAME_H_

#include "nickG.h"
#include "vec.h"
#include "WorldEngine.h"
#include "portal.h"
#include <vector>
#include <deque>
#ifndef _mob_h_
	#include <stdlib.h>
#endif


#define RAND ((float)rand()/RAND_MAX)
#define RCOLOR Vec(RAND,RAND,RAND)
#define MAX_BULLET_HOLES 50
class Mob;
class Wall;
class BulletHole;
class Bullet;

class Game
{
private:
    float tmpPos[3];
    float ox, oz, nx, nz;
public:
    Vec position;
    Vec direction;
    
    vector<Bullet> bullets;
    vector<Vec> spawnPts;
    deque<BulletHole> bulletHoles;
    
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
    int gundamage;
    float mobDist;
    int mobNum;
    int lkey;
    int currscore;
    int setReloadDelay;
    double temperature;
    int health;

    int partyMode;
    unsigned int skyTex;
    
    float maxZoom;
    float minZoom;
    int zoom, togPortal, setPortal;
    float depth;
    
    vector<Mob*> mobs;
    vector<Wall> walls;
    portal defaultPortl;
    worldEngine floor;
    solidSphere sky;
    
    Game();
    void Move();
    
    void Shoot();
    ~Game();    
};

#endif
