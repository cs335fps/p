#include "game.h"

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

    position = Vec(0,2,0);
    direction = Vec(0.0,0.0,0.0);
    mobs.push_back(new Mob());
    
    // Yeah, I'll find a different way of doing this.
    mobs.push_back(new Wall(Vec(-5,0,-10),Vec(-5,0,0),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-5,0,5),Vec(10,0,5),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(10,0,-5),Vec(10,0,5),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-5,0,-10),Vec(55,0,-10),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(45,0,-40),Vec(45,0,-20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(65,0,-50),Vec(45,0,-50),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(65,0,-20),Vec(65,0,-50),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(45,0,-20),Vec(55,0,-20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(55,0,-10),Vec(55,0,-20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(10,0,-5),Vec(25,0,-5),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(30,0,-5),Vec(60,0,-5),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(60,0,-20),Vec(60,0,-5),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(65,0,-20),Vec(60,0,-20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(45,0,-50),Vec(45,0,-45),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-25,0,-75),Vec(10,0,-75),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(10,0,-55),Vec(10,0,-75),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(10,0,-50),Vec(10,0,-35),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(10,0,-35),Vec(-10,0,-35),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-15,0,-35),Vec(-30,0,-35),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-25,0,-75),Vec(-25,0,-40),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(25,0,-5),Vec(25,0,15),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(30,0,-5),Vec(30,0,5),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(45,0,-45),Vec(30,0,-45),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(10,0,-55),Vec(30,0,-55),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(25,0,-40),Vec(45,0,-40),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(10,0,-50),Vec(25,0,-50),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(25,0,-40),Vec(25,0,-50),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(30,0,-45),Vec(30,0,-55),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-10,0,-35),Vec(-10,0,0),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-5,0,0),Vec(-10,0,0),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-15,0,-35),Vec(-15,0,0),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-105,0,-55),Vec(-25,0,-55),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-105,0,-15),Vec(-70,0,-15),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-45,0,-45),Vec(-45,0,-15),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-45,0,-15),Vec(-60,0,-15),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-105,0,-55),Vec(-105,0,-15),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-30,0,-35),Vec(-30,0,-50),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-45,0,-55),Vec(-45,0,-50),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-45,0,-45),Vec(-40,0,-45),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-40,0,-50),Vec(-30,0,-50),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-40,0,-45),Vec(-40,0,-50),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-15,0,0),Vec(-45,0,0),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-45,0,-15),Vec(-45,0,0),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-70,0,-15),Vec(-70,0,0),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-55,0,0),Vec(-70,0,0),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-50,0,-10),Vec(-50,0,20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-60,0,-15),Vec(-60,0,-5),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-75,0,40),Vec(-45,0,40),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-75,0,40),Vec(-75,0,70),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-75,0,70),Vec(-25,0,70),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-20,0,70),Vec(50,0,70),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-40,0,40),Vec(0,0,40),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(0,0,45),Vec(0,0,70),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-10,0,5),Vec(-10,0,40),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(0,0,45),Vec(5,0,45),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-5,0,35),Vec(5,0,35),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-5,0,5),Vec(-5,0,35),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-50,0,20),Vec(-40,0,20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-40,0,40),Vec(-40,0,20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-45,0,40),Vec(-45,0,30),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-55,0,30),Vec(-45,0,30),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-55,0,0),Vec(-55,0,30),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(5,0,15),Vec(50,0,15),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(5,0,60),Vec(5,0,15),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(50,0,60),Vec(5,0,60),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-25,0,75),Vec(55,0,75),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-25,0,70),Vec(-25,0,75),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(50,0,60),Vec(50,0,70),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(55,0,75),Vec(55,0,5),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(30,0,5),Vec(55,0,5),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-50,0,5),Vec(-10,0,5),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(25,0,25),Vec(15,0,35),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(45,0,25),Vec(15,0,55),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(15,0,25),Vec(15,0,35),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(25,0,25),Vec(15,0,25),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-60,0,-50),Vec(-60,0,-20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-60,0,-20),Vec(-90,0,-20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-95,0,-50),Vec(-95,0,-20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-95,0,-50),Vec(-65,0,-50),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-65,0,-50),Vec(-65,0,-25),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-70,0,-45),Vec(-70,0,-20),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-90,0,-20),Vec(-90,0,-45),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-85,0,-50),Vec(-85,0,-25),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-55,0,45),Vec(-30,0,45),0.1, 4.0,RCOLOR));
    mobs.push_back(new Wall(Vec(-10,0,40),Vec(-10,0,55),0.1, 4.0,RCOLOR));

    //level1.load("Level1.obj");
    floor.load("floor.obj");
}

void Game::Move()
{
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

    position.z -= (velocityX * cos(direction.x)
            + velocityY * -sin(direction.x)) * speed;
    position.x -= (velocityY * cos(direction.x)
            + velocityX * sin(direction.x)) * speed;

    for (unsigned int i = 0; i < mobs.size(); i++) {
        mobs[i]->Collide(&position);
	for(unsigned int j = 0; j < mobs.size(); j++){
	    mobs[j]->Collide(mobs[i]->getLoc());
	}
        mobs[i]->move(this);
    }
    
    if (togPortal == 1){
        defaultPortl.reLocateOBJ(position.x,
                position.y,
                position.z,

                position.x,
                position.y,
                position.z);
    }
    /*
       if (level1.isTouching(position.x, position.y, position.z,1, tmpPos)){
       position.x = tmpPos[0];
       position.z = tmpPos[2];
       } else{
       tmpPos[0] = position.x;
       tmpPos[2] = position.z;
       }
     */

}



