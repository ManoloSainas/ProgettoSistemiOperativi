#include "main.h"
#include "thief.h"

void thief_process(int write_pipe) {
    int x = 0, y = 0;
    char buffer[2];
    srand(time(NULL));

    while (1) {
        int direction = rand() % 4;
        switch (direction) {
            case 0: y = (y > 0) ? y - 1 : y; break;
            case 1: y = (y < LINES - 1) ? y + 1 : y; break;
            case 2: x = (x > 0) ? x - 1 : x; break;
            case 3: x = (x < COLS - 1) ? x + 1 : x; break;
        }
        buffer[0] = x;
        buffer[1] = y;
        write(write_pipe, buffer, 2);
        usleep(200000); // Sleep for 200ms to slow down the thief
    }
}