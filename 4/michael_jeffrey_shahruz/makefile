all: main.o
	gcc main.o
	make clean
	make run
main.o: main.c
	gcc -c main.c
clean:
	rm *.o
run:
	./a.out
