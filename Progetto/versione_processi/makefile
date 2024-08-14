frogger: main.o schermo.o gioco.o stampa.o gestione.o rana.o pianta.o
	gcc main.o schermo.o gioco.o stampa.o gestione.o rana.o pianta.o -o frogger -lncurses

main.o: main.h  main.c 
	gcc -c main.c 

schermo.o: main.h schermo.c 
	gcc -c schermo.c

gioco.o: main.h gioco.c
	gcc -c gioco.c

stampa.o: main.h stampa.c
	gcc -c stampa.c

gestione.o: main.h gestione.c
	gcc -c gestione.c

rana.o: main.h rana.c
	gcc -c rana.c

pianta.o: main.h pianta.c
	gcc -c pianta.c

clean:
	rm frogger
	rm *.o