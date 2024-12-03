frogger: frogger.o schermo.o
	gcc frogger.o schermo.o -o frogger -lncurses

frogger.o: frogger.h frogger.c
	gcc -c frogger.c

schermo.o: frogger.h schermo.c
	gcc -c schermo.c

clean:
	rm frogger
	rm *.o
