// Charles Enright
// CS 335 Software Engineering
// File includes AI and elements of game mechanics for first person shooter.

#include "solidSphere.h"
#include "charlesE.h"


Mob::Mob(){
    body.redraw(3.0, 10, 10);
}

void Mob::spawn(){
    location.z = 2;
    location.x = 5;
    location.y = 5;
    body.draw(location.x, location.y, location.z);

}

void Mob::death(){


}
void Mob::damage(int health){



}

void Mob::move(){
//Put AI logic here.



}

void Mob::render(){
    this->move();
    body.draw(location.x, location.y, location.z);

}

void Enemy::move(){


}

