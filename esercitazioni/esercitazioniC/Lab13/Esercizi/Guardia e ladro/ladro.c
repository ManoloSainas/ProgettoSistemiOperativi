#include "ladro.h"
#include "main.h"

void ladro(int pipeout) {
  struct pos pos_ladro;
  long int r;
  int dx, dy;
  pos_ladro.x = 1;
  pos_ladro.y = 1;
  pos_ladro.c = '$';

  write(pipeout, &pos_ladro, sizeof(pos_ladro));

  while (1) {
    r = random();

    if (r < RAND_MAX / 2)
      dx = PASSO;
    else
      dx = -PASSO;

    if (pos_ladro.x + dx < 1 || pos_ladro.x + dx >= MAXX)
      dx = -dx;

    pos_ladro.x += dx;

    r = random();
    if (r < RAND_MAX / 2)
      dy = PASSO;
    else
      dy = -PASSO;

    if (pos_ladro.y + dy < 1 || pos_ladro.y + dy >= MAXY)
      dy = dy;

    pos_ladro.y += dy;

    write(pipeout, &pos_ladro, sizeof(pos_ladro));
    usleep(100000);
  }
}
