EXEC_NAME=mess

all: clean build run

clean:
	rm -f *~ *.o ${EXEC_NAME}

build: execute.o main.o
	gcc execute.o main.o -o ${EXEC_NAME}

execute.o: execute.c execute.h
	gcc -c execute.c

main.o: main.c
	gcc -c main.c

run:
	./${EXEC_NAME}
