#include "game.h"
#include "lizandroP.h"

Game::Game()
{
    srand(time(NULL));
    velocityX = velocityY = 0.0f;
    moveX = moveY = 0;
    aiming = 0;
    depth = .15f;
    zoom = 0;
    maxZoom = .55f;
    minZoom = .15f;
    togPortal =0;
    setPortal =0;
    nbullets = 10;
    maxbullets = 10;
    partyMode = 0;
    nkills=0;
    guntype =0;
    hitAnim = 0;
    gundamage= 10;
    mobNum = 0;
    mobDist= 0.0;
    lkey = 0;
    currscore =0;
    temperature = 25.0; // temperature in celsius
    position = Vec(0,2,0);
    direction = Vec(0.0,0.0,0.0);
    respawn_mobs(this, 10);
    ParseLevel("lev1.svg", this, 3.0);

    for (unsigned int i = 0; i < walls.size(); i++) {
        walls[i].game = this;
    }
    //level1.load("Level1.obj");
    floor.load("floor.obj");
}

void Game::Move()
{
    if (hitAnim > 0)
        hitAnim--;

    float ox, oz;
    if (zoom == 1 && depth < maxZoom) {
        depth += (maxZoom - minZoom) / 15;
    }else if (zoom == 0 && depth > minZoom) {
        depth -= (maxZoom - minZoom) / 15;
    }


    if (moveY == 0 || moveX == 0) {
        // moving orthogonally
        velocityX = (float) moveX;
        velocityY = (float) moveY;
    } else {
        // moving diagonally
        velocityX = (float) moveX / 1.414;
        velocityY = (float) moveY / 1.414;
    }

    // slow down when aiming
    float speed = 0.2f - (float) aiming * 0.1f;

    // do not change ox and oz if the player is not moving
    if (nx != position.x || nz != position.z) {
        oz = position.z;
        ox = position.x;
    }

    // assign new position and save that position in nx and nz
    position.z -= (velocityX * cos(direction.x)
            + velocityY * -sin(direction.x)) * speed;
    position.x -= (velocityY * cos(direction.x)
            + velocityX * sin(direction.x)) * speed;
    nz =position.z;
    nx =position.x;

    for (unsigned int i = 0; i < mobs.size(); i++) {
        mobs[i]->Collide(&position);
        for (unsigned int j = 0; j < walls.size(); j++) {
            Vec temp;
            Vec* velocity = mobs[i]->getVel();
            walls[j].Collide(mobs[i]->getLoc(), 2.0, &temp);
            *velocity = Reflect(*velocity, temp);
        }   
        //for (unsigned int j = 0; j < mobs.size(); j++) {
        //    mobs[j]->Collide(mobs[i]->getLoc());
        //}
        //Actually happens in view->render, because it needs the map.
        mobs[i]->move(this);
    }

    for (unsigned int i = 0; i < walls.size(); i++) {
        walls[i].Collide(&position);
    }

    for (unsigned int i = 0; i < bullets.size(); i++) {
        if (bullets[i].age-- < 1) {
            bullets[i] = bullets.back();
            bullets.pop_back();
            i--;
        }
    }

    // handle portals and portal placement
    if (togPortal == 1) {
        defaultPortl.reLocateOBJ(position.x,
                position.y,
                position.z,

                position.x,
                position.y,
                position.z);
        if (setPortal == 1){
            setPortal ^= 1;
            defaultPortl.loc(position.x, position.y, position.z,
                    ox, 2, oz);
        }
    }

}

void Game::Shoot()
{
    float rotx = direction.x;
    float roty = direction.y - PI / 2.0;
    float trailLen = 5.0;
    Vec hitNormal;

    Bullet b;
    Vec origin = position - Vec(0,.1,0);
    Vec direction = Vec(sin(rotx) * sin(roty) * trailLen,
            cos(roty) * trailLen,
            cos(rotx) * sin(roty) * trailLen);

    // -----------Check collision----------------
    float closest = 9e9;
    int wallHit = 0;
    int wallCount = walls.size();
    for (int i = 0; i < wallCount; i++) {
        if (walls[i].Ray(origin, direction, &closest, &hitNormal) == 1)
            wallHit = 1;
    }
    /*
    // &hitNormal is optional, I'm using it for bulletholes
    // Here's how'd you use this.
    // You may not even need "hit" if you are just comparing wall dist to
    // some mob's dist unless that mob was > 9e9 away for some reason.
    if (hit == 1) {
    cout << "hit distant: " << closest << endl;
    } else {
    cout << "no hit" << endl;
    }
    //cout << wallHit << endl;
    if (wallHit != 0)
    return;
     */
    int mobHit = -1; // Using 'hit' to pick which mob we shot now
    int mobCount = mobs.size();
    for (int i = 0; i < mobCount; i++) {
        if (RaySphere(origin, direction,
                    *(mobs[i]->getLoc()), 1.0, &closest) == 1)
            mobHit = i;
    }
    if (mobHit >= 0) {
        wallHit = 0;
        mobNum = mobHit;
        mobDist= closest;
        float distMult = 15.0 / closest;
        if (distMult < 0.0)
            distMult = 0.0;
        if (distMult > 1.0)
            distMult = 1.0;
        // !--- This cout can get removed after actual Mob damage works
        int totDam = (int)((float)gundamage * distMult);
        mobs[mobHit]->damage(totDam, this);
        if (totDam > 0)
            hitAnim = 20;
    }

    if (wallHit == 1) {
        Vec loc = origin + direction.Norm() * (closest - 0.01);
        bulletHoles.push_back(BulletHole(loc,hitNormal));
        while (bulletHoles.size() > MAX_BULLET_HOLES)
            bulletHoles.pop_front();
    }

    b.origin = origin;
    b.direction = direction;
    b.end = b.origin + b.direction;

    b.age = 30;
    bullets.push_back(b);

}








