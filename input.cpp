#include "input.h"
void chadKey(Game*);
void startAstar(Game*);
Input::Input(Game *g, View *v, Openal *o)
{
    view = v;
    game = g;
    openal = o;
    dpy = view->GetDisplay();
    view->CenterCursor();
    openal->initopenal();
}

int Input::CheckInput()
{
    while (XPending(dpy)) {
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
        if (game->gameRunning == 0) {
            game->gameRunning = 1;
        }

        if (game->displayGameOverOrWon == 1)
            game->displayGameOverOrWon = 0;

        if (key == XK_Escape) {
            openal->clean_al();
            leaderboard(game);
            return 1;
        }
        if (game->playerHP >0){
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
            if (key == XK_r) {
                //Fire Roy's key
                game->togPortal ^=1;
            }
            if (key == XK_f) {
                //Fire Roy's key
                if (game->togPortal == 1)
                    game->setPortal ^=1;
            }
            if (key == XK_n) {
                //Fire Nick's key
                game->partyMode ^= 1;
            }
            if (key == XK_l) {
                game->lkey^=1;
                raptorsound();
            }
            if (key == XK_m) {
                //Fire A* algorithm.
                startAstar(this->game);
            }
            if (key == XK_c) {
                //Fire Chad's key
                //cout << "c key pressed.";
                game->noScoreReport = 1;
                chadKey(this->game, this->view);
                //respawn_mobs(this->game, 10);
            }
            if (key == XK_1) {
                setGun(game,0);
                game->maxZoom = .2f;
            }
            if (key == XK_2) {
                setGun(game,1);
                game->maxZoom = .55f;
            }
            if (key == XK_3) {
                setGun(game,2);
                game->maxZoom = .25f;
            }
            if (key == XK_space 
                && game->setReloadDelay == 0
                && game->nbullets != game->maxbullets) {
                reloadAmmo(game);
            }
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
        if (e->xbutton.button==3 && game->playerHP > 0) {
            //Right button was released
            game->zoom = 0;
            game->aiming = 0;
            return;
        }
        return;
    }
    if (e->type == ButtonPress) {
        if (game->gameRunning == 0) {
            game->gameRunning = 1;
            return;
        }
        if (game->displayGameOverOrWon == 1) {
            game->displayGameOverOrWon = 0;
            return;
        }
        if (e->xbutton.button==1 
                && game->playerHP > 0  
                && game->setReloadDelay == 0) {
            //Left button was pressed
            if (game->nbullets <1) {
                emptysound(game);
                return;
            }
            openal_sound();
            game->Shoot();
            game->nbullets -= 1;
            return;
        }
        if (e->xbutton.button==3 && game->playerHP > 0) {
            //Right button was pressed
            if (game->guntype==1)
                game->zoom =1.5;
            else
                game->zoom = 1;

            game->aiming = 1;
            return;
        }
    }
    if (game->playerHP > 0){
        // Ignore first 5 move inputs
        // First few are garbage.
        static int start = 0;
        if (start < 5 || game->gameRunning == 0) {
            start++;
            view->CenterCursor();
            return;
        }

        int dx = e->xbutton.x - (view->GetWidth() / 2);
        int dy = e->xbutton.y - (view->GetHeight() / 2);
        game->direction.x -=(float) dx / 2000.0 / (game->depth / game->minZoom);
        game->direction.y -=(float) dy / 2000.0 / (game->depth / game->minZoom);
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
}








