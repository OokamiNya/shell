all: shell.o redirect.o
	gcc -o shell shell.o redirect.o
	./shell	

shell.o: shell.c shell.h redirect.h
	gcc -c shell.c

redirect.o: redirect.c redirect.h
	gcc -c redirect.c

clean:
	rm *~
	rm *.o	
	rm ./shell
