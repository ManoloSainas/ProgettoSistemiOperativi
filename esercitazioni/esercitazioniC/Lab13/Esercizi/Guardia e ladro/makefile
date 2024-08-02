game: main.o guard.o thief.o parent.o game.o
    gcc main.o guard.o thief.o parent.o game.o -o game -lncurses

main.o: main.c options.h
    gcc -c main.c -o main.o

guard.o: guard.c guard.h options.h
    gcc -c guard.c -o guard.o

thief.o: thief.c thief.h options.h
    gcc -c thief.c -o thief.o

parent.o: parent.c parent.h options.h
    gcc -c parent.c -o parent.o

game.o: game.c options.h guard.h thief.h parent.h
    gcc -c game.c -o game.o

clean:
    rm -f game
    rm -f *.o

	