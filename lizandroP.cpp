//Lizandro Perez
//Sound Files
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include </usr/include/AL/alut.h>
#include "lizandroP.h"

ALuint alBuffer;
ALuint alSource;
//initalize and add source of sound
int initopenal()
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
	alBuffer = alutCreateBufferFromFile("./ninemm.wav");
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return 0;
	}
	return 0;
}

//will play sound
void openal_sound()
{
		alSourcePlay(alSource);
}

//will clean and delete and sound files
void clean_al()
{
	alDeleteSources(1, &alSource);
	alDeleteBuffers(1, &alBuffer);
	ALCcontext *Context = alcGetCurrentContext();
	ALCdevice *Device = alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
}

