// Charles Enright
// CS 335 Software Engineering
// File includes AI and elements of game mechanics for first person shooter.

#include "solidSphere.h"
#include "charlesE.h"


Mob::Mob(){
    body.redraw(3.0, 10, 10);
}

void Mob::spawn(){
    location.z = 39;
    location.x = 11;
    location.y = 2;
    body.draw(location.x, location.y, location.z);

}

void Mob::death(){


}
void Mob::damage(int health){



}

void Mob::move(){




}

void Mob::render(){
	body.draw(location.x, location.y, location.z);

}

void Enemy::move(){


}

