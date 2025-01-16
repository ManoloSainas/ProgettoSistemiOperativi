#include "frogger.h"

// dichiarazione sprite
char spriteRana[COLONNE_SPRITE_RANA + 1] = "><";
char spriteCoccodrilloDestra[COLONNE_SPRITE_COCCODRILLO + 1] = "===<";
char spriteCoccodrilloSinistra[COLONNE_SPRITE_COCCODRILLO + 1] = ">===";
char spriteProiettileCocco[COLONNE_SPRITE_PROIETTILE + 1] = "@";
char spriteProiettileRana[COLONNE_SPRITE_PROIETTILE + 1] = "-";

void graficaGioco(bool tana_status[])
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

    tana_status[0] == true ? mvwprintw(gioco, 6, 11, "  ") : mvwprintw(gioco, 6, 11, "XX");
    tana_status[1] == true ? mvwprintw(gioco, 6, 23, "  ") : mvwprintw(gioco, 6, 23, "XX");
    tana_status[2] == true ? mvwprintw(gioco, 6, 35, "  ") : mvwprintw(gioco, 6, 35, "XX");
    tana_status[3] == true ? mvwprintw(gioco, 6, 47, "  ") : mvwprintw(gioco, 6, 47, "XX");
    tana_status[4] == true ? mvwprintw(gioco, 6, 59, "  ") : mvwprintw(gioco, 6, 59, "XX");

    wattroff(gioco, COLOR_PAIR(COLORE_TANE));

    // refresh della finestra
    wrefresh(gioco);
}

void stampaSprite(elementoGioco elemento)
{
    int lunghezza, start_visibile, fine_visibile;

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
        break;

    case COCCODRILLO:
        lunghezza = COLONNE_SPRITE_COCCODRILLO;

        wattron(gioco, COLOR_PAIR(COLORE_COCCODRILLO));
        if (elemento.direzione == SINISTRA)
        {

            // Calcola l'inizio e la fine visibili
            start_visibile = (elemento.x < minx + 1) ? (minx)-elemento.x : 0;
            fine_visibile = (elemento.x + lunghezza > maxx - 1) ? maxx - 1 - elemento.x : lunghezza;

            if (start_visibile < fine_visibile)
            {
                mvwprintw(gioco, elemento.y, elemento.x + start_visibile, "%.*s", fine_visibile - start_visibile, spriteCoccodrilloSinistra + start_visibile);
            }
        }

        if (elemento.direzione == DESTRA)
        {
            int start_visibile, fine_visibile;

            // Calcola l'inizio e la fine visibile dello sprite, correggendo per i limiti dello schermo
            start_visibile = (elemento.x < minx) ? minx - elemento.x : 0;
            fine_visibile = (elemento.x + lunghezza > maxx - 1) ? maxx + 2 - elemento.x : lunghezza;

            // Verifica che lo sprite non vada oltre i limiti orizzontali
            if (elemento.x > 4)
            {
                if (fine_visibile > start_visibile)
                {
                    if (elemento.x + lunghezza >= minx && elemento.x < maxx)
                    { // Assicura che sia nella finestra visibile
                        mvwprintw(gioco, elemento.y, elemento.x + start_visibile - 3, "%.*s", fine_visibile - start_visibile, spriteCoccodrilloDestra + start_visibile);
                    }
                }
            }
            else
            {

                if (elemento.x == 1)
                    mvwprintw(gioco, elemento.y, 1, "%s", "<");
                if (elemento.x == 2)
                    mvwprintw(gioco, elemento.y, 1, "%s", "=<");
                if (elemento.x == 3)
                    mvwprintw(gioco, elemento.y, 1, "%s", "==<");
                if (elemento.x == 4)
                    mvwprintw(gioco, elemento.y, 1, "%s", "===<");
            }
        }
        wattroff(gioco, COLOR_PAIR(COLORE_COCCODRILLO));
        break;
    case PROIETTILE_COCCODRILLO:
        wattron(gioco, COLOR_PAIR(COLORE_PROIETTILE_COCCODRILLO));
        mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteProiettileCocco);
        wattroff(gioco, COLOR_PAIR(COLORE_PROIETTILE_COCCODRILLO));

        break;

    case GRANATA:
        switch (elemento.y)
        {
        case (7):
            wattron(gioco, COLOR_PAIR(COLORE_GRANATA_ARGINE));
            mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteProiettileRana);
            wattroff(gioco, COLOR_PAIR(COLORE_GRANATA_ARGINE));
            break;
        case (16):
            wattron(gioco, COLOR_PAIR(COLORE_GRANATA_ARGINE));
            mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteProiettileRana);
            wattroff(gioco, COLOR_PAIR(COLORE_GRANATA_ARGINE));
            break;
        default:
            wattron(gioco, COLOR_PAIR(COLORE_GRANATA_ACQUA));
            mvwprintw(gioco, elemento.y, elemento.x, "%s", spriteProiettileRana);
            wattroff(gioco, COLOR_PAIR(COLORE_GRANATA_ACQUA));
            break;
        }

    default:
        break;
    }
    box(gioco, ACS_VLINE, ACS_HLINE); // Disegna il contorno
}

void cancellaSprite(elementoGioco elemento)
{
    switch (elemento.tipo)
    {
    case RANA:
        switch (elemento.y)
        {
        case (6):
            wattron(gioco, COLOR_PAIR(COLORE_RANA_TANA));
            mvwprintw(gioco, elemento.y, elemento.x, "  ");
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_TANA));
            break;
        case (7):
            wattron(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            mvwprintw(gioco, elemento.y, elemento.x, "  ");
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            break;
        case (16):
            wattron(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            mvwprintw(gioco, elemento.y, elemento.x, "  ");
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            break;
        default:
            wattron(gioco, COLOR_PAIR(COLORE_RANA_COCCODRILLO));
            mvwprintw(gioco, elemento.y, elemento.x, "  ");
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_COCCODRILLO));
            break;
        }

        break;
    case COCCODRILLO:
        wattron(gioco, COLOR_PAIR(SFONDO_ACQUA));
        if (elemento.direzione == SINISTRA && elemento.x < maxx - 5)
        {
            mvwprintw(gioco, elemento.y, elemento.x + 4, "%c", ' ');
        }
        if (elemento.direzione == DESTRA && elemento.x > 4)
        {
            mvwprintw(gioco, elemento.y, elemento.x - 4, "%c", ' ');
        }
        wattroff(gioco, COLOR_PAIR(SFONDO_ACQUA));
        break;
    case PROIETTILE_COCCODRILLO:
        wattron(gioco, COLOR_PAIR(SFONDO_ACQUA));
        if (elemento.direzione == SINISTRA)
        {
            mvwprintw(gioco, elemento.y, elemento.x + 1, "%c", ' ');
        }
        if (elemento.direzione == DESTRA)
        {
            mvwprintw(gioco, elemento.y, elemento.x - 1, "%c", ' ');
        }
        wattroff(gioco, COLOR_PAIR(SFONDO_ACQUA));
        break;
    case GRANATA:
        switch (elemento.y)
        {
        case (6):
            wattron(gioco, COLOR_PAIR(COLORE_RANA_TANA));
            if (elemento.direzione == DESTRA)
            {
                mvwprintw(gioco, elemento.y, elemento.x - 1, " ");
            }

            if (elemento.direzione == SINISTRA)
            {
                mvwprintw(gioco, elemento.y, elemento.x + 1, " ");
            }

            wattroff(gioco, COLOR_PAIR(COLORE_RANA_TANA));
            break;
        case (7):
            wattron(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            if (elemento.direzione == DESTRA)
                mvwprintw(gioco, elemento.y, elemento.x - 1, " ");
            if (elemento.direzione == SINISTRA)
                mvwprintw(gioco, elemento.y, elemento.x + 1, " ");
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            break;
        case (16):
            wattron(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            if (elemento.direzione == DESTRA)
                mvwprintw(gioco, elemento.y, elemento.x - 1, " ");
            if (elemento.direzione == SINISTRA)
                mvwprintw(gioco, elemento.y, elemento.x + 1, " ");
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_ARGINE));
            break;
        default:
            wattron(gioco, COLOR_PAIR(COLORE_RANA_COCCODRILLO));
            if (elemento.direzione == DESTRA)
                mvwprintw(gioco, elemento.y, elemento.x - 1, " ");
            if (elemento.direzione == SINISTRA)
                mvwprintw(gioco, elemento.y, elemento.x + 1, " ");
            wattroff(gioco, COLOR_PAIR(COLORE_RANA_COCCODRILLO));
            break;
        }

    default:
        break;
    }
}
