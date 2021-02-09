CC=gcc
TUNE= -c

all:	app-hw1

app-hw1:	main.o initMatrix.o displayMatrix.o ThreadCalcul.o calcul.o
	$(CC) main.o initMatrix.o displayMatrix.o ThreadCalcul.o calcul.o -o mmm -lm -lpthread

main.o:	main.c
	$(CC) $(TUNE) main.c -o main.o 

initMatrix.o:	initMatrix.c
	$(CC) $(TUNE) initMatrix.c -o initMatrix.o
	
displayMatrix.o:	displayMatrix.c
	$(CC) $(TUNE) displayMatrix.c -o displayMatrix.o	

ThreadCalcul.o:	ThreadCalcul.c
	$(CC) $(TUNE) ThreadCalcul.c -o ThreadCalcul.o
	
calcul.o: calcul.c
	$(CC) $(TUNE) calcul.c -o calcul.o

clean:
	rm -fr app-hw1 *.o
