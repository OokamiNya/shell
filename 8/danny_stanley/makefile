FILES = shell.c link_list.c
LIBS = -lreadline

all: clean build run

clean:
	@rm -f shell
	@rm -f *~

build:
	@gcc -Wall -c -g $(FILES)
	@gcc -o shell shell.o link_list.o $(LIBS)

run:
	@rm -f shell.o
	@rm -f link_list.o
	@./shell

test:
	@./shell < test.txt
