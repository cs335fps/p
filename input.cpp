#include "input.h"

Input::Input(Game *g, View *v)
{
    view = v;
    game = g;
    dpy = view->GetDisplay();
}

int Input::CheckInput()
{
    while(XPending(dpy)) {
        XEvent e;
        XNextEvent(dpy, &e);
        CheckMouse(&e);
        int done = CheckKeys(&e);
        if (done == 1)
            return 1;
    }
    return 0;
}

int Input::CheckKeys(XEvent *e)
{
    if (e->type == KeyPress) {
        int key = XLookupKeysym(&e->xkey, 0);
        if (key == XK_Escape) {
            return 1;
        }
        if (key == XK_w) {
            game->moveX = 1;
        }
        if (key == XK_s) {
            game->moveX = -1;
        }
        if (key == XK_a) {
            game->moveY = 1;
        }
        if (key == XK_d) {
            game->moveY = -1;
        }
        
    }else if (e->type == KeyRelease) {
        int key = XLookupKeysym(&e->xkey, 0);
        if (key == XK_w) {
            game->moveX = 0;
        }
        if (key == XK_s) {
            game->moveX = 0;
        }
        if (key == XK_a) {
            game->moveY = 0;
        }
        if (key == XK_d) {
            game->moveY = 0;
        }        
    }
    return 0;
}

void Input::CheckMouse(XEvent *e)
{
    if (e->type == ButtonRelease) {
        if (e->xbutton.button==3) {
            //Right button was released
            view->zoom = 0;
            game->aiming = 0;
            return;
        }
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button was pressed
            return;
        }
        if (e->xbutton.button==3) {
            //Right button was pressed
            view->zoom = 1;
            game->aiming = 1;
            return;
        }
    }
    
    int dx = e->xbutton.x - (view->GetWidth() / 2);
    int dy = e->xbutton.y - (view->GetHeight() / 2);
    game->direction.x -=(float) dx / 2000.0 / (view->depth / view->minZoom);
    game->direction.y -=(float) dy / 2000.0 / (view->depth / view->minZoom);
    game->direction.x = fmod(game->direction.x,2.0*PI);
    if (game->direction.x < 0.0)
        game->direction.x += PI * 2.0;
    if (game->direction.y > PI / 2.0 - 0.01)
        game->direction.y = PI / 2.0 - 0.01;
    if (game->direction.y < -PI / 2.0 + 0.01)
        game->direction.y = -PI / 2.0 + 0.01;
    if (dx != 0 || dy != 0)
        view->CenterCursor();
}
