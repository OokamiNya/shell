FILES = shell.c
LIBS = -lreadline

all: clean build run

clean:
	@rm -f shell

build:
	@gcc -Wall -o shell $(LIBS) $(FILES)

run:
	@./shell

test:
	@./shell < test.txt
