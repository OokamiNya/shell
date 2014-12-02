FLAGS = -g -Wall -Wextra -pedantic
CC = gcc $(FLAGS)

.PHONY: all clean jksh

all: build jksh

clean:
	rm -rf build

jksh: build/jksh
	build/jksh

build/jksh: build/jksh.o build/parser.o build/ui.o
	$(CC) -o $@ $^

build/%.o: src/%.c
	$(CC) -o $@ -c $^

build:
	mkdir build