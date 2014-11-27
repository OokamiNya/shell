FILES = shell.c shellutil.c prompt.c directorystack.c
OBJECTS = *.o
LIBS = -lreadline

all: clean build run

clean:
	@rm -f shell *.o

build:
	@gcc -Wall -c -g $(FILES)
	@gcc -o shell $(OBJECTS) $(LIBS)

run:
	@rm -f *.o
	@./shell

test:	clean build
	@./shell < test.txt
