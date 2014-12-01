all: shellstuff.o
	gcc shellstuff.o
	make run
shellstuff.o: shellstuff.c shellstuff.h
	gcc -c shellstuff.c
clean:
	rm *.o
run:
	./a.out
