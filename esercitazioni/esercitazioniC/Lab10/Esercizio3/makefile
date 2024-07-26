esercizio3: main.o somma.o prodotto.o
	gcc main.o somma.o prodotto.o -o esercizio
main.o: main.c main.h
	gcc -c main.c -o main.o
somma.o: somma.c somma.h
	gcc -c somma.c -o somma.o
prodotto.o: prodotto.c prodotto.h
	gcc -c prodotto.c -o prodotto.o
clean:
	rm esercizio3
	rm *.o