frogger: main.o rana.o
	gcc main.o rana.o -o frogger -lncurses

main.o: main.c main.h 
	gcc -c main.c -o main.o 

rana.o: rana.c main.h
	gcc -c rana.c -o rana.o 

clean:
	rm frogger
	rm *.o