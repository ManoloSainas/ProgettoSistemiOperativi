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
        switch (elemento.y)
        {
        case (6):
            wattron(gioco, COLOR_PAIR(COLORE_RANA_TANA));
            mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteRana);
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_TANA));
            break;
        case (7):
            wattron(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteRana);
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            break;
        case (16):
            wattron(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteRana);
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            break;
        default:
            wattron(gioco, COLOR_PAIR(COLORE_RANA_COCCODRILLO));
            mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteRana);
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_COCCODRILLO));
            break;
        }

    case COCCODRILLO:
        wattron(gioco, COLOR_PAIR(COLORE_COCCODRILLO));
        if (elemento.direzione == SINISTRA)
        {
            mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteCoccodrilloSinistra);
        }
        if (elemento.direzione == DESTRA)
        {
            mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteCoccodrilloDestra);
        }
        wattroff(gioco, COLOR_PAIR(COLORE_COCCODRILLO));
        break;
    case PROIETTILE_COCCODRILLO:
        wattron(gioco, COLOR_PAIR(COLORE_COCCODRILLO_SPARO));
        mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteProiettileCocco);
        wattroff(gioco, COLOR_PAIR(COLORE_COCCODRILLO_SPARO));

        break;
    case GRANATA_RANA:
        wattron(gioco, COLOR_PAIR(COLORE_ROSSO));
        mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteProiettileRana);
        wattroff(gioco, COLOR_PAIR(COLORE_ROSSO));
        break;
    default:
        break;
    }
}

void cancellaSprite(elementoGioco elemento)
{
    switch (elemento.tipo)
    {
    case RANA:
        wattron(gioco, COLOR_PAIR(COLORE_BLU));
        mvwprintw(gioco, elemento.y, elemento.x, "  ");
        wattroff(gioco, COLOR_PAIR(COLORE_BLU));

        break;
    case COCCODRILLO:
        wattron(gioco, COLOR_PAIR(COLORE_BLU));
        if (elemento.direzione == SINISTRA)
        {
            mvwprintw(gioco, elemento.y, elemento.x + 4, "%c", ' ');
        }
        if (elemento.direzione == DESTRA)
        {
            mvwprintw(gioco, elemento.y, elemento.x - 1, "%c", ' ');
        }
        wattroff(gioco, COLOR_PAIR(COLORE_BLU));
        break;
    case PROIETTILE_COCCODRILLO:
        wattron(gioco, COLOR_PAIR(COLORE_BLU));
        if (elemento.direzione == SINISTRA)
        {
            mvwprintw(gioco, elemento.y, elemento.x + 1, "%c", ' ');
        }
        if (elemento.direzione == DESTRA)
        {
            mvwprintw(gioco, elemento.y, elemento.x - 1, "%c", ' ');
        }
        wattroff(gioco, COLOR_PAIR(COLORE_BLU));
        break;
    case GRANATA_RANA:
        if (elemento.y >= maxy - 1 || elemento.y < miny + 7)
            wattron(gioco, COLOR_PAIR(SFONDO_MARCIAPIEDE));
        if (elemento.y < maxy - 1 || elemento.y > miny + 8)
            wattron(gioco, COLOR_PAIR(SFONDO_ACQUA));
        if (elemento.y < miny + 6)
            wattron(gioco, COLOR_PAIR(SFONDO_ERBA));
        if (elemento.direzione == SINISTRA)
        {
            mvwprintw(gioco, elemento.y, elemento.x + 1, "%c", ' ');
        }
        if (elemento.direzione == DESTRA)
        {
            mvwprintw(gioco, elemento.y, elemento.x - 1, "%c", ' ');
        }
        if (elemento.y >= maxy - 2)
            wattroff(gioco, COLOR_PAIR(SFONDO_MARCIAPIEDE));
        if (elemento.y < maxy - 2 || elemento.y > miny + 2)
            wattroff(gioco, COLOR_PAIR(SFONDO_ACQUA));
        if (elemento.y < miny + 2)
            wattroff(gioco, COLOR_PAIR(SFONDO_ERBA));
        break;
    default:
        break;
    }
}

//
//
//
//
//
//
//
//