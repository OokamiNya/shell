all: clean compile run

clean:
	rm -f shell *~ *.o

compile: shell.c
	gcc -o shell shell.c

run:
	./shell
