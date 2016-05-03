#include "game.h"
#include "view.h"
#include "input.h"
#include "nickG.h" // Seconds, Vec

int main()
{
    Seconds sec; // Start timer
    Game game;
    View view(&game);
    Input input(&game, &view);
    int done=0;

    const double secPerMove = 1.0/60.0;
    double lastSeconds = sec.Get();

    while (!done) {
        done = input.CheckInput();

        while (sec.Get() > lastSeconds) {
            // Call Move() until we're caught up
            lastSeconds += secPerMove;
            game.Move();
        }

        view.Render();
    }

    view.cleanupXWindows();

    return 0;
}
