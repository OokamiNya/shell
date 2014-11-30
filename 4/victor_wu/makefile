all: run.o shell.o
	gcc -o Terminal run.o shell.o

run.o: run.c
	gcc -c run.c

shell.o: shell.c shell.h
	gcc -c shell.c

clean:
	rm *.o
	rm Terminal
	clear

run:
	./Terminal
