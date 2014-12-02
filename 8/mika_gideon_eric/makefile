FILES = s4.c executor.c
GCC = gcc -g -Wall

all: clean build test

compile: clean build

clean:
	rm -rf build/

build:
	mkdir build/
	$(GCC) $(FILES) -o build/test

test:
	build/test
