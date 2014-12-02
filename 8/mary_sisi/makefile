all: shell.o
	gcc -o turtle shell.o

shell.o: shell.c shell.h
	gcc -c shell.c

clean:
	rm *.o
	rm *.gch
	rm *~
