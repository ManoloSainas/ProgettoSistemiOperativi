frogger: frogger.o schermo.o stampa.o gioco.o rana.o
	gcc frogger.o schermo.o stampa.o gioco.o rana.o -o frogger -lncurses

frogger.o: frogger.h frogger.c
	gcc -c frogger.c

schermo.o: frogger.h schermo.c
	gcc -c schermo.c

stampa.o: frogger.h stampa.c
	gcc -c stampa.c

gioco.o: frogger.h gioco.c
	gcc -c gioco.c

rana.o: frogger.h rana.c
	gcc -c rana.c

clean:
	rm frogger
	rm *.o
