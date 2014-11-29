FILES = shell.c shellutil.c prompt.c directorystack.c
OBJECTS = *.o
LIBS = -lreadline

all: clean compile build run

clean:
	@rm -f shell *.o

compile:
	@gcc -Wall -c -g $(FILES)

compiledebug:
	@gcc -Wall -c -g $(FILES) -D DEBUG

build:
	@gcc -o shell $(OBJECTS) $(LIBS)
	@rm -f *.o

run:
	@./shell

test:	clean compile build
	@./shell < test.txt

debug:	clean compiledebug build run
