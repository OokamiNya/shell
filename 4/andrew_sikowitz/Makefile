all: shell.o
	gcc -o shell shell.c

shell.o: shell.c shell.h
	gcc -c shell.c

run: all
	./shell
