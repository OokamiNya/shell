
all: execute.o shell.o parse.o redirect.o
	gcc -o  shell execute.o shell.o parse.o redirect.o
shell.o: shell.c
	gcc -c  shell.c
execute.o: execute.c
	gcc -c  execute.c
parse.o: parse.c
	gcc -c parse.c
redirect.o: redirect.c
	gcc -c redirect.c
clean:
	rm *.o
	rm *~

debug:
	gcc -g shell.c execute.c parse.c redirect.c
