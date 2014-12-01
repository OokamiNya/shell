all: main.o shell.o	talkback.o
	gcc -o a.out main.o shell.o
	gcc -o talkback.out talkback.o
main.o: main.c shell.h
	gcc -c main.c
shell.o: shell.c shell.h	
	gcc -c shell.c
talkback.o: talkback.c
	gcc -c talkback.c
