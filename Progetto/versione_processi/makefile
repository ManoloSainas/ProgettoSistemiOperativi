frogger: main.o grafica.o
	gcc main.o grafica.o -o frogger -lncurses

main.o: main.c main.h 
	gcc -c main.c -o main.o 

grafica.o: grafica.c main.h
	gcc -c grafica.c -o grafica.o 

clean:
	rm frogger
	rm *.o