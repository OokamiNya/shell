all: shell.o pipe.o redirect.o
	gcc -o exec shell.o pipe.o redirect.o
	./exec

shell.o: shell.c
	gcc -c shell.c

pipe.o: pipe.c
	gcc -c pipe.c

redirect.o: redirect.c
	gcc -c redirect.c

