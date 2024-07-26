program: main.o max.o
	gcc main.o max.o -o program
main.o: main.c max.h
	gcc -c main.c -o main.o
max.o: max.c max.h
	gcc -c max.c -o max.o
clean:
	rm -f program *.o