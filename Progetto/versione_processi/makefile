frogger: main.o rana.o gioco.o grafica.o
	gcc -g main.o rana.o gioco.o grafica.o -o frogger -lncurses

main.o: main.c main.h 
	gcc -g -c main.c -o main.o 

rana.o: rana.c main.h
	gcc -g -c rana.c -o rana.o 

gioco.o: gioco.c main.h
	gcc -g -c gioco.c -o gioco.o

grafica.o: grafica.c main.h
	gcc -g -c grafica.c -o grafica.o

clean:
	rm frogger
	rm *.o