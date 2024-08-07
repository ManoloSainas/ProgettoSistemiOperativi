
guardiaLadro: main.o guardia.o ladro.o controllo.o
	gcc main.o guardia.o ladro.o controllo.o -o guardiaLadro -lncurses

main.o: main.c guardia.h ladro.h controllo.h
	gcc -c main.c -o main.o 

guardia.o: guardia.c guardia.h
	gcc -c guardia.c -o guardia.o

ladro.o: ladro.c ladro.h
	gcc -c ladro.c -o ladro.o

controllo.o: controllo.c controllo.h
	gcc -c controllo.c -o controllo.o

clean:
	rm guardiaLadro
	rm *.o