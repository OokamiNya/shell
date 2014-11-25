FILES = shell.c link_list.c
OBJECTS = shell.o link_list.o
LIBS = -lreadline

all: clean build run

clean:
	@rm -f shell

build:
	@gcc -Wall -c -g $(FILES)
	@gcc -o shell $(OBJECTS) $(LIBS)

run:
	@rm -f *.o
	@./shell

test:
	@./shell < test.txt
