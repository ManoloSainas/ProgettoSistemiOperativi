#include "main.h"
#include "guard.h"

void guard_process(int write_pipe) {
    int x = COLS - 1, y = LINES - 1;
    char buffer[2];

    while (1) {
        int ch = getch();
        switch (ch) {
            case KEY_UP:    y = (y > 0) ? y - 1 : y; break;
            case KEY_DOWN:  y = (y < LINES - 1) ? y + 1 : y; break;
            case KEY_LEFT:  x = (x > 0) ? x - 1 : x; break;
            case KEY_RIGHT: x = (x < COLS - 1) ? x + 1 : x; break;
        }
        buffer[0] = x;
        buffer[1] = y;
        write(write_pipe, buffer, 2);
    }
}