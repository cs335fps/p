#ifndef _GAME_H_
#define _GAME_H_

#include "nickG.h"
#include "vec.h"
#include "WorldEngine.h"
#include "portal.h"
#include <vector>
#include <deque>
#include "web.h"
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
    int gameRunning;
    unsigned int gameCounter;

    Vec position;
    Vec direction;
    int playerHP;
    int maxHP;
    
    char name[32];
    
    vector<Bullet> bullets;
    vector<Vec> spawnPts;
    deque<BulletHole> bulletHoles;
    void Init();
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
    int dmgAnim; 
    int gundamage;
    float mobDist;
    int mobNum;
    int lkey, displayGameOverOrWon;
    int currscore;
    int setReloadDelay;
    int shots, hits;
    double temperature;

    int partyMode;
    unsigned int skyTex;
    
    float maxZoom;
    float minZoom;
    int zoom, togPortal, setPortal;
    float depth;
    bool togGamOverDisplay;
    void SpawnNewMob();
    vector<Mob*> mobs;
    vector<Wall> walls;
    portal defaultPortl;
    worldEngine floor;
    solidSphere sky;
    
    Game();
    void Move();
    
    void Shoot();
    void renderGameOver(float xres, float yres, unsigned int Tex);
    ~Game();    
};

#endif
