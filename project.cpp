#include "game.h"
#include "view.h"
#include "input.h"
#include "nickG.h" // Seconds, Vec
#include <stdio.h>
#include <string.h>
#include "lizandroP.h"

using namespace std;

int main(int argc, char* argv[])
{
    
    int overrideWidth = 0;
    int overrideHeight = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-w") == 0) {
            overrideWidth = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-h") == 0) {
            overrideHeight = atoi(argv[i+1]);
        }
    }

    Seconds sec; // Start timer
    Game game;
    Openal openal;
    entername(&game);
    View view(&game, overrideWidth, overrideHeight);

    Input input(&game, &view, &openal);
    int done=0;

    const double secPerMove = 1.0/60.0;
    double lastSeconds = sec.Get();

    while (!done) {
        done = input.CheckInput();
        
        double dif = sec.Get() - lastSeconds;
        while (dif > 0.0) {
            // Call Move() until we're caught up
            lastSeconds += secPerMove;
            game.Move();
            
            if (dif > 0.25) {
                // We're really behind, need to catch up
                lastSeconds = sec.Get();
                break;
            }
            dif = sec.Get() - lastSeconds;
        }

        view.Render();
    }
    
    view.cleanupXWindows();
    return 0;
}
