FILES = shell.c
LIBS = -lreadline

all: clean build run

clean:
	@rm -f shell
	@rm -f *~

build:
	@gcc -Wall -c -g $(FILES)
	@gcc -o shell shell.o $(LIBS)

run:
	@rm -f shell.o
	@./shell

test:
	@./shell < test.txt
