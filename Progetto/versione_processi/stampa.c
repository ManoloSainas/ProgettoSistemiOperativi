#include "main.h"

// Dichiarazione degli sprite
char spriteRana[COLONNE_SPRITE_RANA + 1] = "><";
char spriteProiettileRana[COLONNE_SPRITE_PROIETTILE_RANA + 1] = "^";
char spriteCoccodrillo[COLONNE_SPRITE_COCCODRILLO + 1] = "====";
char spriteCoccodrilloImmersione[COLONNE_SPRITE_COCCODRILLO + 1] = "<==>";
char spritePianta[COLONNE_SPRITE_PIANTA + 1] = "@@";
char spriteProiettilePianta[COLONNE_SPRITE_PROIETTILE_PIANTA + 1] = "|";

// stampa lo sprite dell'oggetto che viene passato durante l'invocazione
void stampaSprite(oggetto sprite)
{
    int i;

    // Evita di stampare la rana o il coccodrillo nelle coordinate (0, 0) se non è intenzionale
    if ((sprite.x >= 0 && sprite.y < maxy - 13) &&
        (sprite.tipo == RANA || sprite.tipo == COCCODRILLO_BUONO || sprite.tipo == COCCODRILLO_CATTIVO))
    {
        return;
    }

    switch (sprite.tipo)
    {
    case RANA:
        wattron(gioco, COLOR_PAIR(COLORE_RANA)); // Colore della rana
        for (i = 0; i < COLONNE_SPRITE_RANA; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spriteRana[i]); // Stampa la rana
        }
        wattroff(gioco, COLOR_PAIR(COLORE_RANA)); // Disattiva il colore della rana
        break;

    case PROIETTILE_RANA:
        wattron(gioco, COLOR_PAIR(COLORE_ROSSO));                         // Colore del proiettile
        mvwprintw(gioco, sprite.y, sprite.x, "%s", spriteProiettileRana); // Stampa il proiettile
        wattroff(gioco, COLOR_PAIR(COLORE_ROSSO));                        // Disattiva il colore del proiettile
        break;

    case PIANTA:
        wattron(gioco, COLOR_PAIR(COLORE_PIANTA)); // Colore della pianta
        for (i = 0; i < COLONNE_SPRITE_PIANTA; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spritePianta[i]); // Stampa la pianta
        }
        wattroff(gioco, COLOR_PAIR(COLORE_PIANTA)); // Disattiva il colore della pianta
        break;

    case PROIETTILE_PIANTA:
        wattron(gioco, COLOR_PAIR(COLORE_ROSSO));                           // Colore del proiettile
        mvwprintw(gioco, sprite.y, sprite.x, "%s", spriteProiettilePianta); // Stampa il proiettile
        wattroff(gioco, COLOR_PAIR(COLORE_ROSSO));                          // Disattiva il colore del proiettile
        break;

    case COCCODRILLO_BUONO:
        wattron(gioco, COLOR_PAIR(COLORE_COCCODRILLO_BUONO)); // Colore del coccodrillo
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spriteCoccodrillo[i]); // Stampa il coccodrillo
        }
        wattroff(gioco, COLOR_PAIR(COLORE_COCCODRILLO_BUONO)); // Disattiva il colore del coccodrillo
        break;

    case COCCODRILLO_CATTIVO:
        wattron(gioco, COLOR_PAIR(COLORE_COCCODRILLO_CATTIVO)); // Colore del coccodrillo
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spriteCoccodrillo[i]); // Stampa il coccodrillo
        }
        wattroff(gioco, COLOR_PAIR(COLORE_COCCODRILLO_CATTIVO)); // Disattiva il colore del coccodrillo
        break;

    case COCCODRILLO_IMMERSIONE:
        wattron(gioco, COLOR_PAIR(COLORE_COCCODRILLO_IMMERSIONE)); // Colore del coccodrillo
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spriteCoccodrilloImmersione[i]); // Stampa il coccodrillo
        }
        wattroff(gioco, COLOR_PAIR(COLORE_COCCODRILLO_IMMERSIONE)); // Disattiva il colore del coccodrillo
        break;
    }
}

void cancellaSprite(oggetto sprite)
{
    int i;

    switch (sprite.tipo)
    {
    case RANA:
        // Sovrascrivi l'area della rana con spazi
        for (i = 0; i < COLONNE_SPRITE_RANA; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, " "); // Cancella con spazi
        }
        break;

    case PROIETTILE_RANA:
        mvwprintw(gioco, sprite.y, sprite.x, " "); // Cancella il proiettile
        break;

    case PIANTA:
        // Sovrascrivi l'area della pianta con spazi
        for (i = 0; i < COLONNE_SPRITE_PIANTA; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, " "); // Cancella con spazi
        }
        break;

    case PROIETTILE_PIANTA:
        mvwprintw(gioco, sprite.y, sprite.x, " "); // Cancella il proiettile
        break;

    case COCCODRILLO_BUONO:
        // Sovrascrivi l'area del coccodrillo con spazi
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, " "); // Cancella con spazi
        }
        break;

    case COCCODRILLO_CATTIVO:
        // Sovrascrivi l'area del coccodrillo con spazi
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, " "); // Cancella con spazi
        }
        break;

    case COCCODRILLO_IMMERSIONE:
        // Sovrascrivi l'area del coccodrillo con spazi
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, " "); // Cancella con spazi
        }
        break;
    }
}
// Funzione per stampare lo sfondo del gioco e le varie informazioni
void graficaGioco(int viteRana, int tempoRimanente)
{
    int numeroTane = NUM_TANE;
    int spazioTraTane = maxx / (numeroTane + 1);

    // Pulizia dello sfondo
    wattron(gioco, COLOR_PAIR(SFONDO_ARGINE));
    for (int i = 1; i < maxx; i++)
    {
        mvwprintw(gioco, maxy - 1, i, " ");
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_ARGINE));

    wattron(gioco, COLOR_PAIR(SFONDO_ACQUA));
    for (int i = 1; i < maxx; i++)
    {
        for (int j = maxy - 9; j < maxy - 1; j++)
        {
            mvwprintw(gioco, j, i, " ");
        }
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_ACQUA));

    wattron(gioco, COLOR_PAIR(SFONDO_ARGINE));
    for (int i = 1; i < maxx; i++)
    {
        for (int j = maxy - 11; j < maxy - 9; j++)
        {
            mvwprintw(gioco, j, i, " ");
        }
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_ARGINE));

    wattron(gioco, COLOR_PAIR(SFONDO_TANE));
    for (int i = 1; i < maxx; i++)
    {
        for (int j = maxy - 13; j < maxy - 11; j++)
        {
            mvwprintw(gioco, j, i, " ");
        }
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_TANE));

    // Stampa delle tane, equidistanti tra loro, nella riga di coordinate y=maxy-12
    wattron(gioco, COLOR_PAIR(COLORE_TANE));
    for (int i = 1; i <= numeroTane; i++)
    {
        mvwprintw(gioco, maxy - 12, i * spazioTraTane + 2, "  ");
    }
    wattroff(gioco, COLOR_PAIR(COLORE_TANE));

    // Stampa delle informazioni
    wattron(gioco, COLOR_PAIR(COLORE_STANDARD));
    mvwprintw(gioco, maxy - 15, 3, "Vite rana: ");
    mvwprintw(gioco, maxy - 15, maxx / 2 - 7, "Tempo rimasto: ");
    mvwprintw(gioco, maxy - 15, 66, "Punteggio: ");
    wattroff(gioco, COLOR_PAIR(COLORE_STANDARD));

    wattron(gioco, COLOR_PAIR(COLORE_ROSSO));
    mvwprintw(gioco, maxy - 15, 14, "%d", viteRana);

    // Cancella prima il numero di tempo rimasto per evitare sovrapposizioni
    mvwprintw(gioco, maxy - 15, maxx / 2 + 8, "  ");                 // Spazi per cancellare il numero precedente
    mvwprintw(gioco, maxy - 15, maxx / 2 + 8, "%d", tempoRimanente); // Stampa il nuovo tempo rimanente

    mvwprintw(gioco, maxy - 15, maxx - 6, "%d", 122);
    wattroff(gioco, COLOR_PAIR(COLORE_ROSSO));
}
