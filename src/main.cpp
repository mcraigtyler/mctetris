#include <curses.h>

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);

    mvprintw(0, 0, "MCTetris bootstrapped. Press any key to exit.");
    refresh();
    getch();

    endwin();
    return 0;
}
