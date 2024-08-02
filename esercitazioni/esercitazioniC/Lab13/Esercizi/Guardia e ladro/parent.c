#include "main.h"
#include "parent.h"

void parent_process(int read_pipe_guard, int read_pipe_thief) {
    int guard_x = COLS - 1, guard_y = LINES - 1;
    int thief_x = 0, thief_y = 0;
    char buffer[2];

    while (1) {
        // Read guard position
        if (read(read_pipe_guard, buffer, 2) > 0) {
            guard_x = buffer[0];
            guard_y = buffer[1];
        }

        // Read thief position
        if (read(read_pipe_thief, buffer, 2) > 0) {
            thief_x = buffer[0];
            thief_y = buffer[1];
        }

        // Clear screen and draw objects
        clear();
        mvaddch(guard_y, guard_x, GUARD);
        mvaddch(thief_y, thief_x, THIEF);
        refresh();

        // Check for game termination
        if (guard_x == thief_x && guard_y == thief_y) {
            mvprintw(LINES / 2, COLS / 2 - 5, "You caught the thief!");
            refresh();
            sleep(2);
            break;
        }
    }
}