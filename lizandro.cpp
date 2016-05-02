#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include </usr/include/AL/alut.h>
#include <iostream>
#define numOfSounds 3
using namespace std;

int initopenal()
{
	//Get started right here.
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	ALuint alBuffer[numOfSounds];
	alBuffer[0] = alutCreateBufferFromFile("./sounds/ninemm.wav");
	alBuffer[1] = alutCreateBufferFromFile("./sounds/shot.wav");
	alBuffer[2] = alutCreateBufferFromFile("./sounds/ninemm.wav");
	//
	//Source refers to the sound.
	ALuint alSource[numOfSounds];
	//Generate a source, and store it in a buffer.
	for(int size = 0; size < numOfSounds; size++){ 
		alGenSources(1, &alSource[size]);
		alSourcei(alSource[size], AL_BUFFER, alBuffer[size]);
	
		//Set volume and pitch to normal, no looping of sound.
		alSourcef(alSource[size], AL_GAIN, 1.0f);
		alSourcef(alSource[size], AL_PITCH, 1.0f);
		alSourcei(alSource[size], AL_LOOPING, AL_FALSE);
		if (alGetError() != AL_NO_ERROR) {
			printf("ERROR: setting source\n");
			return 0;
		}
	}
	return 0;
}

void openalmain(int alSource[])
{
	char a;
	while(a != 'o'){
	    cout <<"enter a char\n";
	    cin >> a;

	    if(a == 'b'){
		alSourcePlay(alSource[1]);
		usleep(255000);
	    }	
	}
}

void cleanopenal(ALuint alSource[], ALuint alBuffer[]){
	//Cleanup.
	
	//First delete the source.
	for(int size = 0; size < numOfSounds; size++){
		alDeleteSources(1, &alSource[size]);
		//Delete the buffer.
		alDeleteBuffers(1, &alBuffer[size]);
	}
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
}

