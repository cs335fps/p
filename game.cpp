#include "game.h"

Game::Game()
{
  velocityX = velocityY = 0.0f;
  moveX = moveY = 0;
  aiming = 0;
  
    for (int i = 0; i < 3; i++) {
        position[i] = 0.0f;
        direction[i] = 0.0f;
    }
}

void Game::Move()
{
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
    
    position.z += (velocityX * cos(direction.x)
    + velocityY * -sin(direction.x)) * speed;
    position.x -= (velocityY * cos(direction.x)
    + velocityX * sin(direction.x)) * speed;

}
