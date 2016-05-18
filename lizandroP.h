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
#include </usr/include/AL/alut.h>
class Openal{
private:
	//sounds buffers
ALuint alBuffer0;
ALuint alBuffer1;
ALuint alBuffer2;

//sound source 
ALuint alSource[3];

public:

	Openal();
	int initopenal();
	void openal_sound();
	void clean_al();

};
#endif
