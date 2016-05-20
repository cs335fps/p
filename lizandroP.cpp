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
#include "view.h"
#include "game.h"
unsigned int reload;

int nsound =2;

	//sounds buffers
ALuint alBuffer0;
ALuint alBuffer1;
ALuint alBuffer2;

//sound source 
ALuint alSource[3];
extern "C" {
#include "fonts.h"
}


Openal::Openal()
{

	loadBMP rload;
	reload = rload.getBMP("reload.bmp");


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
	alBuffer0 = alutCreateBufferFromFile("./sblast.wav");
	alBuffer1 = alutCreateBufferFromFile("./50.wav");
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
void openal_sound()
{

	alSourcePlay(alSource[nsound]);
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
void setGun(Game *game, int n)
{

	//sets settings for Handgun
	if(n == 0){
		game->guntype =0;
        game->nbullets =10;
        game->maxbullets =10;
        nsound = 2;
	}

	//sets settings for Snipper
	if(n == 1){
		game->guntype = 1;
        game->nbullets = 5;
        game->maxbullets = 5;
        nsound = 1;
	}

	//sets settings for shotgun
	if(n == 2){
		game->guntype=2;
	    game->nbullets=12;
	    game->maxbullets=12;
	    nsound = 0;
	}
}

void reloadMessage(Game *game, int w, int h){
    glColor3f(1.0f,1.0f,1.0f);
    glBindTexture(GL_TEXTURE_2D, reload);
    if (game->nbullets<1){
        glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex2i(w/3, (h/3)*2);

        glTexCoord2f(1,1);
        glVertex2i((w/3)*2, (h/3)*2);

        glTexCoord2f(1,0);
        glVertex2i((w/3)*2, (h/3));

        glTexCoord2f(0,0);
        glVertex2i(w/3, h/3);
        glEnd();
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GameMenu(Game *game, int w, int h)
{
    static int si = 0;
    if (si == 0) {
        initialize_fonts();
        si++;
    }
    //Game Menu
    glBegin(GL_LINES);
    Rect r;
    r.bot = h - 10;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0, "");
    ggprint8b(&r, 16, 0, "Toggles - N: Nick, L: Lizandro, C: Charles, R: Roy");
    ggprint8b(&r, 16, 0, "Spacebar - Reload");
    ggprint8b(&r, 16, 0, "Left click - Shoot");
    ggprint8b(&r, 16, 0, "Y - Sniper");
    ggprint8b(&r, 16, 0, "H - 9mm");
    ggprint8b(&r, 16, 0, "U - ShotGun");
    ggprint8b(&r, 16, 0, "Nround: %i / %i", game->nbullets, game->maxbullets);
    ggprint8b(&r, 16, 0, "Kills: %i", game->nkills);
    // ggprint8b(&r, 16, 0, "Score: Sounds");
    glEnd();
}

