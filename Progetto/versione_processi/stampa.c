#include "main.h"

// Dichiarazione degli sprite
char spriteRana[COLONNE_SPRITE_RANA + 1] = "><";
char spriteProiettileRana[COLONNE_SPRITE_PROIETTILE_RANA + 1] = "^";
char spriteCoccodrillo[COLONNE_SPRITE_COCCODRILLO + 1] = "====";
char spriteCoccodrilloImmersione[COLONNE_SPRITE_COCCODRILLO + 1] = "<==>";
char spritePianta[COLONNE_SPRITE_PIANTA + 1] = "@@";
char spriteProiettilePianta[COLONNE_SPRITE_PROIETTILE_PIANTA + 1] = "|";

// Stampa lo sprite dell'oggetto che viene passato durante l'invocazione
void stampaSprite(oggetto sprite)
{
    int i;

    // Evita di stampare oggetti nelle coordinate (0, 0) se non è intenzionale (serve per risolvere alcuni bug visivi)
    if ((sprite.x >= 0 && sprite.y < maxy - 13) &&
            (sprite.tipo == RANA || sprite.tipo == COCCODRILLO_BUONO || sprite.tipo == COCCODRILLO_CATTIVO) ||
        sprite.tipo == COCCODRILLO_IMMERSIONE)
    {
        return;
    }

    // Stampa lo sprite in base al tipo di oggetto
    switch (sprite.tipo)
    {
    case RANA:
        wattron(gioco, COLOR_PAIR(COLORE_RANA));
        for (i = 0; i < COLONNE_SPRITE_RANA; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spriteRana[i]); // Stampa la rana
        }
        wattroff(gioco, COLOR_PAIR(COLORE_RANA));
        break;

    case PROIETTILE_RANA:
        wattron(gioco, COLOR_PAIR(COLORE_ROSSO));
        mvwprintw(gioco, sprite.y, sprite.x, "%s", spriteProiettileRana); // Stampa il proiettile rana
        wattroff(gioco, COLOR_PAIR(COLORE_ROSSO));
        break;

    case PIANTA:
        wattron(gioco, COLOR_PAIR(COLORE_PIANTA));
        for (i = 0; i < COLONNE_SPRITE_PIANTA; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spritePianta[i]); // Stampa la pianta
        }
        wattroff(gioco, COLOR_PAIR(COLORE_PIANTA));
        break;

    case PROIETTILE_PIANTA:
        wattron(gioco, COLOR_PAIR(COLORE_ROSSO));
        mvwprintw(gioco, sprite.y, sprite.x, "%s", spriteProiettilePianta); // Stampa il proiettile pianta
        wattroff(gioco, COLOR_PAIR(COLORE_ROSSO));
        break;

    case COCCODRILLO_BUONO:
        wattron(gioco, COLOR_PAIR(COLORE_COCCODRILLO_BUONO));
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spriteCoccodrillo[i]); // Stampa il coccodrillo buono
        }
        wattroff(gioco, COLOR_PAIR(COLORE_COCCODRILLO_BUONO));
        break;

    case COCCODRILLO_CATTIVO:
        wattron(gioco, COLOR_PAIR(COLORE_COCCODRILLO_CATTIVO));
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spriteCoccodrillo[i]); // Stampa il coccodrillo cattivo
        }
        wattroff(gioco, COLOR_PAIR(COLORE_COCCODRILLO_CATTIVO));
        break;

    case COCCODRILLO_IMMERSIONE:
        wattron(gioco, COLOR_PAIR(COLORE_COCCODRILLO_IMMERSIONE));
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, "%c", spriteCoccodrilloImmersione[i]); // Stampa il coccodrillo che tocca il bordo dello schermo e si immerge
        }
        wattroff(gioco, COLOR_PAIR(COLORE_COCCODRILLO_IMMERSIONE));
        break;
    }
}

// Cancella lo sprite dell'oggetto che viene passato durante l'invocazione
void cancellaSprite(oggetto sprite)
{
    int i;

    switch (sprite.tipo)
    {
    case RANA:
        for (i = 0; i < COLONNE_SPRITE_RANA; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, " "); // Cancella la rana inserendo degli spazi
        }
        break;

    case PROIETTILE_RANA:
        mvwprintw(gioco, sprite.y, sprite.x, " "); // Cancella il proiettile rana
        break;

    case PIANTA:
        for (i = 0; i < COLONNE_SPRITE_PIANTA; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, " "); // Cancella la pianta
        }
        break;

    case PROIETTILE_PIANTA:
        mvwprintw(gioco, sprite.y, sprite.x, " "); // Cancella il proiettile pianta
        break;

    case COCCODRILLO_BUONO:
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, " "); // Cancella il coccodrillo buono
        }
        break;

    case COCCODRILLO_CATTIVO:
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, " "); // Cancella il coccodrillo cattivo
        }
        break;

    case COCCODRILLO_IMMERSIONE:
        for (i = 0; i < COLONNE_SPRITE_COCCODRILLO; i++)
        {
            mvwprintw(gioco, sprite.y, sprite.x + i, " "); // Cancella il coccodrillo che tocca il bordo dello schermo e si immerge
        }
        break;
    }
}

// Funzione per stampare lo sfondo del gioco e le varie informazioni
void graficaGioco(int viteRana, int tempoRimanente)
{
    int numeroTane = NUM_TANE;
    int spazioTraTane = maxx / (numeroTane + 1);

    // Crea lo sfondo dell'argine da cui partirà la rana
    wattron(gioco, COLOR_PAIR(SFONDO_ARGINE));
    for (int i = 1; i < maxx; i++)
    {
        mvwprintw(gioco, maxy - 1, i, " ");
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_ARGINE));

    // Crea lo sfondo dell'acqua
    wattron(gioco, COLOR_PAIR(SFONDO_ACQUA));
    for (int i = 1; i < maxx; i++)
    {
        for (int j = maxy - 9; j < maxy - 1; j++)
        {
            mvwprintw(gioco, j, i, " ");
        }
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_ACQUA));

    // Crea lo sfondo dell'argine superiore, in cui si troveranno le piante
    wattron(gioco, COLOR_PAIR(SFONDO_ARGINE));
    for (int i = 1; i < maxx; i++)
    {
        for (int j = maxy - 11; j < maxy - 9; j++)
        {
            mvwprintw(gioco, j, i, " ");
        }
    }
    wattroff(gioco, COLOR_PAIR(SFONDO_ARGINE));

    // Crea lo sfondo in cui saranno presenti le tane
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

    // Stampa delle informazioni riguardanti il gioco
    wattron(gioco, COLOR_PAIR(COLORE_STANDARD));
    mvwprintw(gioco, maxy - 15, 3, "Vite rana: ");
    mvwprintw(gioco, maxy - 15, maxx / 2 - 7, "Tempo rimasto: ");
    mvwprintw(gioco, maxy - 15, 66, "Punteggio: ");
    wattroff(gioco, COLOR_PAIR(COLORE_STANDARD));

    wattron(gioco, COLOR_PAIR(COLORE_ROSSO));
    mvwprintw(gioco, maxy - 15, 14, "%d", viteRana); // Stampa il numero di vite rimaste

    // Cancella prima il numero di tempo rimasto per evitare sovrapposizioni inserendo degli spazi vuoti
    mvwprintw(gioco, maxy - 15, maxx / 2 + 8, "  ");
    mvwprintw(gioco, maxy - 15, maxx / 2 + 8, "%d", tempoRimanente); // Stampa il nuovo tempo rimanente

    mvwprintw(gioco, maxy - 15, maxx - 6, "%d", 122);
    wattroff(gioco, COLOR_PAIR(COLORE_ROSSO));
}
