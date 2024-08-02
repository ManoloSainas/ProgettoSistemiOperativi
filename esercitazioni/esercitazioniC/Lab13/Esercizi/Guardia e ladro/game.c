#include "main.h"
#include "guard.h"
#include "thief.h"
#include "parent.h"

void initialize_screen() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(30); // Reduce timeout to 30ms for more responsive input
    curs_set(0); // Hide the cursor
}

void run_game(int pipe_guard[2], int pipe_thief[2]) {
    pid_t pid_guard, pid_thief;

    // Create pipes
    pipe(pipe_guard);
    pipe(pipe_thief);

    // Fork guard process
    if ((pid_guard = fork()) == 0) {
        close(pipe_guard[READ_END]);
        guard_process(pipe_guard[WRITE_END]);
        exit(0);
    }

    // Fork thief process
    if ((pid_thief = fork()) == 0) {
        close(pipe_thief[READ_END]);
        thief_process(pipe_thief[WRITE_END]);
        exit(0);
    }

    // Parent process
    close(pipe_guard[WRITE_END]);
    close(pipe_thief[WRITE_END]);
    parent_process(pipe_guard[READ_END], pipe_thief[READ_END]);

    // Wait for child processes to finish
    wait(NULL);
    wait(NULL);

    // End ncurses
    curs_set(1); // Show the cursor again
    endwin();
}