all: shell.o redirect.o
	gcc -o exec shell.o redirect.o
	./exec

shell.o: shell.c
	gcc -c shell.c

redirect.o: redirect.c
	gcc -c redirect.c
