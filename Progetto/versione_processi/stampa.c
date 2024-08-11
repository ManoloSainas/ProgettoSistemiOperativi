#include "main.h"

// Dichiarazione degli sprite
char spriteRana[COLONNE_SPRITE_RANA + 1] = "><";
char spriteProiettileRana[COLONNE_SPRITE_PROIETTILE_RANA + 1] = "^";

// stampa lo sprite dell'oggetto che viene passato durante l'invocazione
void stampaSprite(oggetto sprite, int viteRana)
{
    int i;

    switch (sprite.tipo)
    {
    case RANA:

        wattron(gioco, COLOR_PAIR(COLORE_ROSSO));

        for (i = 0; i < COLONNE_SPRITE_RANA; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spriteRana[i]);
        }
        break;
    case PROIETTILE_RANA:

        wattron(gioco, COLOR_PAIR(COLORE_ROSSO));
        mvwprintw(gioco, sprite.y, sprite.x, "%s", spriteProiettileRana);
        break;
    }
}

void cancellaSprite(oggetto sprite)
{
    int i;

    wattron(gioco, COLOR_PAIR(COLORE_CANCELLAZIONE));

    switch (sprite.tipo)
    {
    case RANA:
        for (i = 0; i < COLONNE_SPRITE_RANA; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spriteRana[i]);
        }
        break;
    case PROIETTILE_RANA:
        mvwprintw(gioco, sprite.y, sprite.x, "%s", spriteProiettileRana);
        break;
    }
}