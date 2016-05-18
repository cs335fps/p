/*

Lizandro Perez
cs335
software engineering 
Prof. Gordon


created: 4/30/16
modified: 05/17/16


task:
Create Sound Files
Game Menu
Toggle from lab
adjust bullets, max bullets, reload, 
zoom according to weapon selected.


*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include </usr/include/AL/alut.h>
#include "lizandroP.h"


Openal::Openal()
{

}

//initalize and add source of sound
int Openal::initopenal()
{
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return 0;
	}

	alGetError();

	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);

	//load sound file into buffer 
	alBuffer0 = alutCreateBufferFromFile("./test.wav");
	alBuffer1 = alutCreateBufferFromFile("./shot.wav");
	alBuffer2 = alutCreateBufferFromFile("./ninemm.wav");
	alGenSources(1, &alSource[0]);
	alGenSources(1, &alSource[1]);
	alGenSources(1, &alSource[2]);
	
	//setting for first sound 
	alSourcei(alSource[0], AL_BUFFER, alBuffer0);
	alSourcef(alSource[0], AL_GAIN, 1.0f);
	alSourcef(alSource[0], AL_PITCH, 1.0f);
	alSourcei(alSource[0], AL_LOOPING, AL_FALSE);
	
	//setting for second sound 
	alSourcei(alSource[1], AL_BUFFER, alBuffer1);
	alSourcef(alSource[1], AL_GAIN, 2.0f);
	alSourcef(alSource[1], AL_PITCH, 1.0f);
	alSourcei(alSource[1], AL_LOOPING, AL_FALSE);

	//setting for third sound
	alSourcei(alSource[2], AL_BUFFER, alBuffer2);
	alSourcef(alSource[2], AL_GAIN, 1.0f);
	alSourcef(alSource[2], AL_PITCH, 1.0f);
	alSourcei(alSource[2], AL_LOOPING, AL_FALSE);

	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return 0;
	}
	return 0;
}

//function to play sound
void Openal::openal_sound(int num)
{
	//play selected sound
	alSourcePlay(alSource[num]);
}

//Function to clean and delete sound files
void Openal::clean_al()
{
	//clear source 
	alDeleteSources(1, &alSource[0]);
	alDeleteSources(1, &alSource[1]);
	alDeleteSources(1, &alSource[2]);

	//clear buffer
	alDeleteBuffers(1, &alBuffer0);
	alDeleteBuffers(1, &alBuffer1);
	alDeleteBuffers(1, &alBuffer2);

	ALCcontext *Context = alcGetCurrentContext();
	ALCdevice *Device = alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
}

