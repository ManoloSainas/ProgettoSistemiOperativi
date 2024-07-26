# makefile per l'esercizio 5

program: main.o max.o min.o
	gcc main.o max.o min.o -o program
main.o: main.c max.h min.h
	gcc -c main.c -o main.o
max.o: max.c max.h
	gcc -c max.c -o max.o
min.o: min.c min.h
	gcc -c min.c -o min.o
clean:
	rm -f program *.o
	
