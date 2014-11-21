
all: execute.o shell.o parse.o
	gcc -o  shell execute.o shell.o parse.o
shell.o: shell.c
	gcc -c  shell.c
execute.o: execute.c
	gcc -c  execute.c
parse.o: parse.c
	gcc -c parse.c
clean:
	rm *.o
	rm *~

debug:
	gcc -g shell.c execute.c parse.c
