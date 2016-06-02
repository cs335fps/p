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

unsigned int reload=0;
loadBMP rload;

unsigned int scope=0;
loadBMP scoe;

int nsound =2;

//sounds buffers
ALuint alBuffer0;
ALuint alBuffer1;
ALuint alBuffer2;
ALuint alBuffer3;
ALuint alBuffer4;
ALuint alBuffer5;

ALuint dinoBuff[5];
ALuint dinoSour[5];

//sound source 
ALuint alSource[5];
extern "C" {
#include "fonts.h"
}


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
	alBuffer0 = alutCreateBufferFromFile("./sounds/sblast.wav");
	alBuffer1 = alutCreateBufferFromFile("./sounds/50.wav");
	alBuffer2 = alutCreateBufferFromFile("./sounds/gunshot.wav");
	alBuffer3 = alutCreateBufferFromFile("./sounds/test.wav");
	alBuffer4 = alutCreateBufferFromFile("./sounds/raptor.wav");
	alBuffer5 = alutCreateBufferFromFile("./sounds/bite.wav");

	alGenSources(1, &alSource[0]);
	alGenSources(1, &alSource[1]);
	alGenSources(1, &alSource[2]);
	alGenSources(1, &alSource[3]);
	alGenSources(1, &alSource[4]);
	alGenSources(1, &alSource[5]);
	
	for (int i = 0; i < 5; i++) {
		dinoBuff[i] = alutCreateBufferFromFile("./sounds/gunshot.wav");
		alGenSources(1, &dinoSour[i]);
		alSourcei(dinoSour[i], AL_BUFFER, dinoBuff[i]);
		alSourcef(dinoSour[i], AL_GAIN, 0.75f);
		alSourcef(dinoSour[i], AL_PITCH, 1.0f);
		alSourcei(dinoSour[i], AL_LOOPING, AL_FALSE);
	}
	
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

	//setting for fourth sound
	alSourcei(alSource[3], AL_BUFFER, alBuffer3);
	alSourcef(alSource[3], AL_GAIN, 1.0f);
	alSourcef(alSource[3], AL_PITCH, 1.0f);
	alSourcei(alSource[3], AL_LOOPING, AL_FALSE);

	//setting for fourth sound
	alSourcei(alSource[4], AL_BUFFER, alBuffer4);
	alSourcef(alSource[4], AL_GAIN, 1.0f);
	alSourcef(alSource[4], AL_PITCH, 1.0f);
	alSourcei(alSource[4], AL_LOOPING, AL_FALSE);

	//setting for fourth sound
	alSourcei(alSource[5], AL_BUFFER, alBuffer5);
	alSourcef(alSource[5], AL_GAIN, 1.0f);
	alSourcef(alSource[5], AL_PITCH, 1.0f);
	alSourcei(alSource[5], AL_LOOPING, AL_FALSE);

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

void dino_sound()
{
	static int n = -1;
	n = (n + 1) % 5;
	alSourcePlay(dinoSour[n]);
}


//Function to clean and delete sound files
void Openal::clean_al()
{
	//clear source 
	alDeleteSources(1, &alSource[0]);
	alDeleteSources(1, &alSource[1]);
	alDeleteSources(1, &alSource[2]);
	alDeleteSources(1, &alSource[3]);
	alDeleteSources(1, &alSource[4]);
	alDeleteSources(1, &alSource[5]);


	//clear buffer
	alDeleteBuffers(1, &alBuffer0);
	alDeleteBuffers(1, &alBuffer1);
	alDeleteBuffers(1, &alBuffer2);
	alDeleteBuffers(1, &alBuffer3);
	alDeleteBuffers(1, &alBuffer4);
	alDeleteBuffers(1, &alBuffer5);
	
	for (int i = 0; i < 5; i++) {
		alDeleteSources(1, &dinoSour[i]);
		alDeleteBuffers(1, &dinoBuff[i]);
	}

	ALCcontext *Context = alcGetCurrentContext();
	ALCdevice *Device = alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
}

void setGun(Game *game, int n)
{

	//sets settings for Handgun
	if (n == 0) {
	    game->guntype =0;
            game->nbullets =10;
            game->maxbullets =10;
            game->gundamage = 10;
            nsound = 2;
	}

	//sets settings for Snipper
	if (n == 1) {
	    game->guntype = 1;
            game->nbullets = 5;
            game->maxbullets = 5;
            game->gundamage = 30;
            nsound = 1;
	}

	//sets settings for shotgun
	if (n == 2) {
	    game->guntype=2;
	    game->nbullets=12;
	    game->maxbullets=12;
	    game->gundamage = 15;
	    nsound = 0;
	}
}

void reloadAmmo(Game *game)
{       
	if (game->guntype == 2) {
		game->setReloadDelay = 60;
	}
	else if (game->guntype == 1) {
		game->setReloadDelay = 160;
	}

	game->nbullets = game->maxbullets;
}

void emptysound(Game *game)
{	
	alSourcePlay(alSource[3]);
}

void raptorsound()
{   
    alSourcePlay(alSource[4]);
}

void bitesound()
{   
    alSourcePlay(alSource[5]);
}

void reloadMessage(Game *game, int w, int h)
{
    if (reload == 0)
	reload = rload.getBMP("reload.bmp");

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
    ggprint8b(&r, 16, 0, "1 - 9mm");
    ggprint8b(&r, 16, 0, "2 - Sniper");
    ggprint8b(&r, 16, 0, "3 - ShotGun");
    ggprint8b(&r, 16, 0, "Nround: %i / %i", game->nbullets, game->maxbullets);
    ggprint8b(&r, 16, 0, "Kills: %i", game->nkills);
    ggprint8b(&r, 16, 0, "Player: %s", game->name);
    // ggprint8b(&r, 16, 0, "Score: Sounds");
    glEnd();
}

//updates kills by one
//gets called after mobs death;
void kills(Game *game)
{
  game->nkills = game->nkills+1;
  
}


//destructor
Openal::~Openal()
{
  
}  

//mykey
void Lizandrokey(Game *game, int w, int h)
{
    int l = h / 1*5;

    glPushMatrix();
    glTranslatef(w/2,h/2,0);
    glColor4f(0,0,0,0.9);
    glBegin(GL_LINE);
    GLUquadric* qobj = gluNewQuadric();
    gluQuadricOrientation(qobj,GLU_INSIDE);
    gluDisk(qobj, (float)l/5.0*.5, l/5, 62, 1);
    glEnd(); 
    glPopMatrix();
    
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2d(w / 2 - l / 20, h / 2);
    glVertex2d(w / 2 + l / 20, h / 2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2d(w / 2, h / 2 - l / 20);
    glVertex2d(w / 2, h / 2 + l / 20);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2d(w / 2 - l / 20, h / 2+1);
    glVertex2d(w / 2 + l / 20, h / 2+1);
    glEnd();
    glBegin(GL_LINES);
    glVertex2d(w / 2+1, h / 2 - l / 20);
    glVertex2d(w / 2+1, h / 2 + l / 20);
    glEnd();
    
    int hitAnim = game->hitAnim;
    if (hitAnim > 0) {
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(1.0f,0.0f,0.0f);
    glVertex2d(w / 2 - l / 20, h / 2);
    glVertex2d(w / 2 + l / 20, h / 2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2d(w / 2, h / 2 - l / 20);
    glVertex2d(w / 2, h / 2 + l / 20);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2d(w / 2 - l / 20, h / 2+1);
    glVertex2d(w / 2 + l / 20, h / 2+1);
    glEnd();
    glBegin(GL_LINES);
    glVertex2d(w / 2+1, h / 2 - l / 20);
    glVertex2d(w / 2+1, h / 2 + l / 20);
    glEnd();
    
    }
    
    glBegin(GL_LINES);
    Rect r;
    r.bot = h-100;
    r.left = w/2;
    r.center = 0;
    glColor3f(1.0f,0.0f,0.0f);
    ggprint13(&r, 16, 0, "");
    ggprint13(&r, 16, 0, "Mob: %i", game->mobNum);
    ggprint13(&r, 16, 0, "Distance: %f", game->mobDist);
    glEnd();
     

}

#include <fstream>
#include <string>

int leaderboard(Game *game)
{
        game->currscore = game->nkills;
        cout<<"----------------Leaderboard---------------\n";
        cout<<"__________________________________________\n";


    string line;
    string content[10];
    ifstream scoretxt ("score.txt");
    if (scoretxt.is_open()){
        
    for (int i=0;i<10;i++){
      scoretxt >> content[i];
    }
      stringstream n;
      cout << "your kills: " << game->currscore <<"   "<< endl;
      if(game->currscore >= atoi(content[1].c_str())){
          cout<<"congrats you are ranked #1 in the leaderboard!\n";
          content[0] = game->name;
          n << game->nkills;
          content[1] = n.str();
      }
      else if(game->currscore >= atoi(content[3].c_str())){
          cout<<"congrats you are ranked #2 in the leaderboard!\n";
          content[2] = game->name;
           n << game->nkills;
          content[3] = n.str();
      }
      else if(game->currscore >= atoi(content[5].c_str())){
          cout<<"congrats you are ranked #3 in the leaderboard!\n";
          content[4] = game->name;
           n << game->nkills;
          content[5] = n.str();
      }
      else if(game->currscore >= atoi(content[7].c_str())){
          cout<<"congrats you are ranked #4 in the leaderboard!\n";
          content[6] = game->name;
           n << game->nkills;
          content[7] = n.str();
      }
      else if(game->currscore >= atoi(content[9].c_str())){
          cout<<"congrats you are ranked #5 in the leaderboard!\n";
          content[8] = game->name;
           n << game->nkills;
          content[9] = n.str();
          cout << content[8]<<"test output\n";
      }
      else
          cout<<"sorry your score was to low to place on the board\n";
    
        for (int i=0, cnt=0;i<10;i++){
      cnt++;
      if(cnt%2 ==0)
        printf("%10s %7s\n", content[i-1].data(), content[i].data());
    }
    

    }
    scoretxt.close();

    ofstream scoretxtout ("score.txt");
    if (scoretxtout.is_open()) {
        for (int i=0, cnt=0;i<10;i++) {
            cnt++;

            if (cnt % 2 == 0)
                scoretxtout << content[i-1].data()<<" " << content[i].data() << endl; 
        }
        
    }

    scoretxtout.close();    
    return 0;

}

void entername(Game* game)
{
  char name[32];
  cout <<"Enter your name\n";
  //cin >> name;
  cin.getline(name,sizeof(name));
 
  if (name==NULL) {
    cout <<"Invalid input, Please enter name again.\n";
    //cin >> name;
    cin.getline(name,sizeof(name));
  }
  strcpy(game->name, name);
}
