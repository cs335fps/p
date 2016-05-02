#include <iostream>
#include "game.h"
#include "view.h"
#include "input.h"
#include "vec.h"
#include "nickG.h"

using namespace std;

int main()
{
    Seconds sec;
    Game game;
    View view(&game);
    Input input(&game, &view);
    int done=0;

    const double secPerMove = 1.0/60.0;
    double lastSeconds = sec.Get();

    while (!done) {
        done = input.CheckInput();

        while (sec.Get() > lastSeconds) {
            game.Move();

            // This will make sure that Move() fires as many
            // times as necessary to catch up.
            lastSeconds += secPerMove;
        }

        view.Render();
    }
    view.cleanupXWindows();

    return 0;
}


