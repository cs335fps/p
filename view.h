#ifndef _VIEW_H_
#define _VIEW_H_

#include <cmath>
#include <vector>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <iostream>
#include <cstdlib>
#include "game.h"
#include <string>
#include <string.h>
#include <unistd.h>
#include "charlesE.h"
#include "nickG.h"
#include "lizandroP.h"
#include "map.h"

#define PI 3.14159

using namespace std;

class Game;
class Mob;

class View
{
private:
    int width;
    int height;
    int is3D;
    GLdouble depth;
    Display *dpy;
    Window root;
    Window win;
    GLXContext glc;
    Game *game;
    float ox,oy,oz;
    int wOverride, hOverride;    
    unsigned int mobTex, skyTex, Win, Lose, keys, sheet, wallTex, floorTex;

public:
    View(Game *g, int w = 0, int h = 0);
    void InitWindow();
    void cleanupXWindows(void);
    void set_title(void);
    void ShowCursor(const int onoff);
    void CenterCursor();
    Display *GetDisplay();
    int GetWidth();
    int GetHeight();
    void Render();
    void SwitchTo3D();
    void SwitchTo2D();
    void Lighting();
    void HUD();
    ~View();
};

#endif
