#ifndef _INPUT_H_
#define _INPUT_H_

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "game.h"
#include "view.h"

#define PI 3.14159

class Input
{
private:
View *view;;
Game *game;
Display *dpy;
    
public:
    Input(Game *g, View *v);
    int CheckInput();
    int CheckKeys(XEvent *e);
    void CheckMouse(XEvent *e);
    
};

#endif