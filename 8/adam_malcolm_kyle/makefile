all: program.o pipe.o
	gcc program.o pipe.o
	./a.out
program.o: program.c program.h
	gcc -c program.c
pipe.o: pipe.c pipe.h
	gcc -c pipe.c
clean:
	rm *.o
