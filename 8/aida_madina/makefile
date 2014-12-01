all: shell.o parse.o pipe.o redirect.o
	gcc -o exec shell.o parse.o pipe.o redirect.o
	./exec

shell.o: shell.c
	gcc -c shell.c

parse.o: parse.c
	gcc -c parse.c

pipe.o: pipe.c
	gcc -c pipe.c

redirect.o: redirect.c
	gcc -c redirect.c

