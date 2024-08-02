#include "main.h"

int main() {
    int pipe_guard[2], pipe_thief[2];

    initialize_screen();
    run_game(pipe_guard, pipe_thief);

    return 0;
}