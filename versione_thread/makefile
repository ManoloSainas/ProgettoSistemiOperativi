frogger: frogger.o schermo.o stampa.o gioco.o gestione.o rana.o coccodrillo.o proiettile.o
	gcc frogger.o schermo.o stampa.o gioco.o gestione.o rana.o coccodrillo.o proiettile.o -g -o frogger -lncurses -lpthread

frogger.o: frogger.h frogger.c
	gcc -c frogger.c

schermo.o: frogger.h schermo.c
	gcc -c schermo.c

stampa.o: frogger.h stampa.c
	gcc -c stampa.c

gioco.o: frogger.h gioco.c
	gcc -c gioco.c

gestione.o: frogger.h gestione.c
	gcc -c gestione.c

rana.o: frogger.h rana.c
	gcc -c rana.c

coccodrillo.o: frogger.h coccodrillo.c
	gcc -c coccodrillo.c

proiettile.o: frogger.h proiettile.c
	gcc -c proiettile.c

clean:
	rm frogger
	rm *.o



