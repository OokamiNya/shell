FILES = main.c newshell.c

all: clean build run

clean:
	@rm -f shell

build:
	@gcc -Wall -o shell -lcurses $(FILES)

run:
	@./shell

test:
	@./shell < test.txt
