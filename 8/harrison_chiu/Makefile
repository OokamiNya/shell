all: clean build run

clean:
	rm -f *~ *.o xq

build: execute.o main.o
	gcc execute.o main.o -o xq

execute.o: execute.c execute.h
	gcc -c execute.c

main.o: main.c
	gcc -c main.c

run:
	./xq
