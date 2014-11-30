all: program.o
	gcc program.o
	./a.out
program.o: program.c program.h
	gcc -c program.c
clean:
	rm *.o
