#ifndef _VIEW_H_
#define _VIEW_H_

#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <iostream>
#include <cstdlib>
#include "game.h"
#include "WorldEngine.h"
#include <string>
#include "charlesE.h"
#include <vector>


#define PI 3.14159

class Mob;
class View

{
private:
    int width;
    int height;
    int is3D;
    Display *dpy;
    Window root;
    Window win;
    GLXContext glc;
    Game *game;
    worldEngine level1;
    vector<Mob*> mobs;
    int wOverride, hOverride;    
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
