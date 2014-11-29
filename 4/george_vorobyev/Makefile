all: util.c shell.c
	gcc util.c shell.c -o gshell

shell.c: util.h 
	gcc -c shell.c

util.c: util.h 
	gcc -c util.c

run: all
	./gshell

clean:
	rm *.gch gshell *.o
