#include "frogger.h"

// dichiarazione sprite
char spriteRana[COLONNE_SPRITE_RANA + 1] = "><";
char spriteCoccodrilloDestra[COLONNE_SPRITE_COCCODRILLO + 1] = "===<";
char spriteCoccodrilloSinistra[COLONNE_SPRITE_COCCODRILLO + 1] = ">===";
char spriteProiettileCocco[COLONNE_SPRITE_PROIETTILE + 1] = "@";
char spriteProiettileRana[COLONNE_SPRITE_PROIETTILE + 1] = "-";

void graficaGioco()
{
    // Pulizia dello sfondo
    wattron(gioco, COLOR_PAIR(SFONDO_MARCIAPIEDE));
    for (int i = 1; i < maxx - 1; i++)
    {
        mvwprintw(gioco, maxy - 2, i, " ");
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_MARCIAPIEDE));

    wattron(gioco, COLOR_PAIR(SFONDO_ACQUA));
    for (int i = 1; i < maxx - 1; i++)
    {
        for (int j = maxy - 10; j < maxy - 2; j++)
        {
            mvwprintw(gioco, j, i, " ");
        }
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_ACQUA));

    wattron(gioco, COLOR_PAIR(SFONDO_ERBA));
    for (int i = 1; i < maxx - 1; i++)
    {
        for (int j = maxy - 11; j < maxy - 10; j++)
        {
            mvwprintw(gioco, j, i, " ");
        }
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_ERBA));

    wattron(gioco, COLOR_PAIR(SFONDO_TANE));
    for (int i = 1; i < maxx - 1; i++)
    {
        for (int j = maxy - 13; j < maxy - 11; j++)
        {
            mvwprintw(gioco, j, i, " ");
        }
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_TANE));

    // Stampa delle tane, equidistanti tra loro, nella riga di coordinate y=maxy-12
    wattron(gioco, COLOR_PAIR(COLORE_TANE));

    mvwprintw(gioco, 6, 11, "  ");
    mvwprintw(gioco, 6, 23, "  ");
    mvwprintw(gioco, 6, 35, "  ");
    mvwprintw(gioco, 6, 47, "  ");
    mvwprintw(gioco, 6, 59, "  ");

    wattroff(gioco, COLOR_PAIR(COLORE_TANE));
}

void stampaSprite(elementoGioco elemento)
{
    switch (elemento.tipo)
    {
    case RANA:
        mvwprintw(gioco, elemento.y, elemento.x, spriteRana);
        break;

    case COCCODRILLO:
        if (true)
            mvwprintw(gioco, elemento.y, elemento.x, spriteCoccodrilloSinistra);
        break;

    default:
        break;
    }
}