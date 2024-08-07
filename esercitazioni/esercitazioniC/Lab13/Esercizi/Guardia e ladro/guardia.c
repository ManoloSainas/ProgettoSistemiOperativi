#include "guardia.h"
#include "main.h"

void guardia(int pipeout) {
  struct pos pos_guardia;
  pos_guardia.c = '#';
  pos_guardia.x = MAXX - 1;
  pos_guardia.y = MAXY - 1;

  write(pipeout, &pos_guardia, sizeof(pos_guardia));

  while (1) {
    char c;
    switch (c = getch()) {

    case SU:
      if (pos_guardia.y > 0)
        pos_guardia.y -= 1;
      break;

    case GIU:
      if (pos_guardia.y < MAXY - 1)
        pos_guardia.y += 1;
      break;

    case SINISTRA:
      if (pos_guardia.x > 0)
        pos_guardia.x -= 1;
      break;

    case DESTRA:
      if (pos_guardia.x < MAXX - 1)
        pos_guardia.x += 1;
      break;
    }

    write(pipeout, &pos_guardia, sizeof(pos_guardia));
  }
}
