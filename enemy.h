#ifndef ENEMY_H
#define ENEMY_H
#include "charlesE.h"
#include "WorldEngine.h"

class Enemy: public Mob
{
	int pos[3];
	worldEngine obj;
	public:
		unsigned int texture;
		void move();
		virtual void render();
};

class Munition: public Enemy
{

};
#endif
