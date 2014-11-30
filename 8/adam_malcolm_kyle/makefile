all: program.o
	gcc program.o
	./a.out
program.o: program.c program.h
	gcc -c shell.c
clean:
	rm *.o
