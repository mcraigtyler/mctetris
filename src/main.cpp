#include <curses.h>

#include "model/game_model.h"

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);

    mctetris::model::GameModel model;
    if (!model.Spawn(mctetris::model::TetrominoType::T)) {
        mvprintw(0, 0, "Failed to spawn initial piece. Press any key to exit.");
        refresh();
        getch();
        endwin();
        return 1;
    }

    mvprintw(0, 0, "MCTetris bootstrapped. Press any key to exit.");
    refresh();
    getch();

    endwin();
    return 0;
}
