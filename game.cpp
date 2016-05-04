#include "game.h"

Game::Game()
{
    velocityX = velocityY = 0.0f;
    moveX = moveY = 0;
    aiming = 0;
    depth = .15f;
    zoom = 0;
    maxZoom = .55f;
    minZoom = .15f;

    position = Vec(-15,2,0);
    direction = Vec(0.0,0.0,0.0);

    walls.push_back(Wall(Vec(10,0,0),Vec(0,0,10),0.1, 4.0));
    walls.push_back(Wall(Vec(0,0,10),Vec(-10,0,0),0.1, 4.0));
    walls.push_back(Wall(Vec(0,0,-10),Vec(-10,0,0),0.1, 4.0));
    walls.push_back(Wall(Vec(10,0,0),Vec(0,0,-10),0.1, 4.0));
    walls.push_back(Wall(Vec(20,0,20),Vec(20,0,-20),0.1, 4.0));
    walls.push_back(Wall(Vec(20,0,-20),Vec(-20,0,-20),0.1, 4.0));
    walls.push_back(Wall(Vec(-20,0,-20),Vec(-20,0,20),0.1, 4.0));
    walls.push_back(Wall(Vec(-20,0,20),Vec(20,0,20),0.1, 4.0));

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

    for (unsigned int i = 0; i < walls.size(); i++) {
        walls[i].Collide(&position);

    }


}


