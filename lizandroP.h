#ifndef OPENAL_H
#define OPENAL_H

#ifdef GGWINDOWS
#include <al/alut.h>
#endif //GGWINDOWS
#ifdef GGLINUX
#include </usr/include/AL/alut.h>
#endif //GGLINUX

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>
#include </usr/include/AL/alut.h>


class Game;
void emptysound(Game *game);
void reloadAmmo(Game *game);
void reloadMessage(Game *game, int w, int h);
void GameMenu(Game *game, int w, int h);
void openal_sound();
void setGun(Game *game, int n);
void Lizandrokey(Game *game, int w, int h);
void kills(Game *game);
int leaderboard(Game *game);


class Openal{
private:

unsigned int lizandro;

public:

	Openal();
	int initopenal();
	void clean_al();
	~Openal();

};
#endif
