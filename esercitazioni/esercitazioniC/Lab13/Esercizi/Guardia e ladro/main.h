#ifndef OPTIONS_H
#define OPTIONS_H

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

// Define characters for guard and thief
#define GUARD '#'
#define THIEF '$'

// Define pipe indices
#define READ_END 0
#define WRITE_END 1

void initialize_screen();
void run_game(int pipe_guard[2], int pipe_thief[2]);

#endif // OPTIONS_H