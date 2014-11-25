all: JAVOshell.o executor.o
	gcc JAVOshell.o executor.o
JAVOshell.o: executor.c JAVOshell.c
	gcc -c JAVOshell.c
executor.o: executor.c
	gcc -c executor.c
signal.o: signal.c signal.h
	gcc -c signal.c signal.h
clean:
	rm *.o
	rm a.out
