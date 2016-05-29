#include "game.h"
#include "lizandroP.h"

Game::Game()
{
   
    noScoreReport = 0;
    ParseLevel("lev1.svg", this, 3.0);
    for (unsigned int i = 0; i < walls.size(); i++) {
        walls[i].game = this;
    }
    
    Init();

    //level1.load("Level1.obj");
    floor.load("floor.obj");
}

void Game::Init()
{
    // RESET
    int m = this->mobs.size();
    for (int i = 0; i < m; i++) {
        this->mobs[i]->death(this);
    }
    bullets.clear();
    bulletHoles.clear();



    //SET
    temperature = 25.0; // temperature in celsius
    displayGameOverOrWon = -5;
    togGamOverDisplay = false;
    gameCounter = 0;
    gameRunning = 0;
    srand(time(NULL));
    velocityX = velocityY = 0.0f;
    killStreak = 0;
    maxKillStreak = 0;
    moveX = moveY = 0;
    aiming = 0;
    depth = .15f;
    zoom = 0;
    maxZoom = .2f;
    minZoom = .15f;
    togPortal =0;
    setPortal =0;
    nbullets = 10;
    maxbullets = 10;
    partyMode = 0;
    nkills=0;
    guntype =0;
    hitAnim = 0;
    dmgAnim = 0;
    gundamage= 10;
    mobNum = 0;
    mobDist= 0.0;
    lkey = 0;
    shots = 0;
    hits = 0;
    currscore =0;
    servMessage.clear();
    playerHP = 60;
    maxHP = 60;
    setReloadDelay =0;
    position = Vec(0,2,0);
    direction = Vec(0.0,0.0,0.0);
    respawn_mobs(this, 10);
    for (unsigned int i = 0; i < mobs.size(); i++) {
        Vec* spt = &spawnPts[i % spawnPts.size()];
        mobs[i]->spawn(spt);
        mobs[i]->setTick();
    }


}

void Game::Move()
{
    if (gameRunning == 0)
        return;

    if (hitAnim > 0)
        hitAnim--;
    if(setReloadDelay > 0)
        setReloadDelay -= 1;

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
    int nMobs = mobs.size();
    while (nMobs < 5) {
        SpawnNewMob();
        nMobs = mobs.size();
    }
    for (int i = 0; i < nMobs; i++) {
        for (unsigned int j = 0; j < walls.size(); j++) {
            Vec temp;
            Vec* velocity = mobs[i]->getVel();
            walls[j].Collide(mobs[i]->getLoc(), 2.0, &temp);
            *velocity = Reflect(*velocity, temp);
        }
        if (mobs[i]->Collide(&position) == 1) {
            this->playerHP -= 5;
            mobs[i]->damage(10, this);
            this->dmgAnim = 20;
            killStreak = 0;
            cout << "Raptor " << i << " damaged player 5 points. " << endl;
            cout << "Player has " << playerHP << " health remaining." << endl;
        }	   
        //for (unsigned int j = 0; j < mobs.size(); j++) {
        //    mobs[j]->Collide(mobs[i]->getLoc());
        //}
        //Actually happens in view->render, because it needs the map.
        mobs[i]->move(this);

        float ang = atan2(mobs[i]->getVel()->x,
                mobs[i]->getVel()->z);
        ang = ang / 3.14159 * 180.0 + 90.0;
        float lastAng = mobs[i]->lastFace.x;
        float diff = ang - lastAng;
        if (diff > 180.0 || diff < -180.0)
            diff *= -1.0;
        float delta = 5.0;
        if (diff > 0.1) {
            if (diff < delta)
                lastAng = ang;
            else
                lastAng += delta;
        } else if (diff < -0.1) {
            if (diff > delta)
                lastAng = ang;
            else
                lastAng -= delta;
        } else {
            lastAng = ang;
        }
        lastAng = fmod(lastAng,360.0);
        if (lastAng < 0.0)
            lastAng+=360.0;

        mobs[i]->body.rot(0,0,lastAng,0);
        mobs[i]->lastFace.x = lastAng;
        if (mobs[i]->getTick() > gameCounter || mobs[i]->dino == 0)
            continue;

        mobs[i]->setTick(gameCounter + (15 * RAND) + 30);

        Vec sightDir = -1.0 * (*mobs[i]->getLoc() - position);
        float closestSight = sightDir.Magnitude();
        int wallHit = 0;
        int wallCount = walls.size();
        for (int j = 0; j < wallCount; j++) {
            if (walls[j].Ray(*mobs[i]->getLoc(), sightDir, &closestSight) == 1)
                wallHit = 1;
        }
        if (wallHit == 0) {
            Bullet b;
            float mobErr = 0.2;
            Vec err = Vec((2.0 * RAND - 0.1) * mobErr,
                    (2.0 * RAND - 0.1) * mobErr,
                    (2.0 * RAND - 0.1) * mobErr);
            sightDir.Normalize();
            sightDir = sightDir + err;
            sightDir = sightDir * closestSight;
            b.origin = *mobs[i]->getLoc() + Vec(0,-1,0);
            b.direction = sightDir;
            b.end = b.origin + b.direction;
            b.age = 30;
            bullets.push_back(b);
            float tmp = 9e9;
            if (RaySphere(b.origin, b.direction, position, 1, &tmp)) {
                playerHP -= 5;
                dmgAnim = 20;
                killStreak = 0;
            }
            if (playerHP <= 0) {
                playerHP = 0;
                
                // set displayGameOverOrWon only once
                if (togGamOverDisplay == false) {
                    togGamOverDisplay = true;
                    displayGameOverOrWon = 30;
                    
                }
                
                // use to display the mesage on screen that the player 
                // wone or lost the game
                if(displayGameOverOrWon == 0) {
                    Init();
                } else if (displayGameOverOrWon == 29 && !noScoreReport) {
                    // Make sure the game over screen comes up before
                    // we block with this web request.
                    Web w;
                    string s = w.Score(name, 
                        nkills, 
                        shots, 
                        hits, 
                        gameCounter, 
                        maxKillStreak);
                    vector<string> svec = Split(s,",");
                    if (svec.size() > 1) {
                        servMessage = svec;
                    }                        
                }
                displayGameOverOrWon -= 1;
            }
        }
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
    if (dmgAnim > 0)
        dmgAnim--;
    gameCounter++;
}

void Game::SpawnNewMob()
{
    // Spawn mob at a random point away from player and other mobs
    int rnd;
    int counter = 20;
    int prox = 1;
    double thresh = 20.0;
    while (prox == 1 && counter-- > 0) {
        prox = 0;
        rnd = RAND * ((float)spawnPts.size() + .9999);
        if ((spawnPts[rnd] - position).Magnitude() < thresh) {
            prox = 1;
            continue;
        }
        for (unsigned int i = 0; i < mobs.size(); i++) {
            if ((spawnPts[rnd] - *mobs[i]->getLoc()).Magnitude() < thresh) {
                prox = 1;
                break;
            }
        }
    }

    respawn_mobs(this, 1);
    mobs[mobs.size()-1]->spawn(&spawnPts[rnd]);
}

void Game::Shoot()
{
    shots++;
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
                    *(mobs[i]->getLoc()), 1.0, &closest) == 1
           )
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
        hits++;
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
void Game::renderGameOver(float xres, float yres, unsigned int Tex)
{
    glBindTexture(GL_TEXTURE_2D, Tex);

    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(xres, yres);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f( xres, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f( 0.0f, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f( 0.0f, yres);

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnd();
}













