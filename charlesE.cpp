// Charles Enright
// CS 335 Software Engineering
// File includes AI and elements of game mechanics for first person shooter.

#include "solidSphere.h"
#include "charlesE.h"


Mob::Mob(){
    body.redraw(2.0, 10, 10);
    this->spawn();
}

void Mob::spawn(){
    location.z = 2;
    location.x = 2;
    location.y = 2; // y is up and down.
    body.draw(location.x, location.y, location.z);
    //velocity.z = -0.05;
    //velocity.x = .15;
    //velocity.y = 0.05;
}

void Mob::death(){
    delete this;	

}
void Mob::damage(int health){

    this->hp -= health;
    if(this->hp < 0)
	this->death();
}

void Mob::move(){

}

void Mob::move(Game g){
//Put AI logic here.
//
//1) Collision detection: gather local objects
//2) Collision detection: for each local object, is touching object?
//3) collision detection: if touching object, bounce
    location.z += velocity.z;
    location.y += velocity.y;
    location.x += velocity.x;
}

void Mob::render(){
    this->move();
    body.draw(location.x, location.y, location.z);

}

void Enemy::move(){


}

