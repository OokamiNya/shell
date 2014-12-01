# Objects
objects = \
	build/main.o \
	build/prompt.o \
	build/cd.o \
	build/error.o \
	build/parse.o \
	build/execute.o \
	build/list.o \
	build/control.o \
	build/input.o

# Output File
output = normandy

# Default Target
all: $(objects)
	gcc $(objects) -o $(output)

# Object Files
build/main.o: src/main.c
	gcc -c src/main.c -o build/main.o

build/prompt.o: src/prompt.c include/prompt.h
	gcc -c src/prompt.c -o build/prompt.o

build/cd.o: src/cd.c include/cd.h
	gcc -c src/cd.c -o build/cd.o

build/error.o: src/error.c include/error.h
	gcc -c src/error.c -o build/error.o

build/parse.o: src/parse.c include/parse.h
	gcc -c src/parse.c -o build/parse.o

build/execute.o: src/execute.c include/execute.h
	gcc -c src/execute.c -o build/execute.o

build/list.o: src/list.c include/list.h
	gcc -c src/list.c -o build/list.o

build/control.o: src/control.c include/control.h
	gcc -c src/control.c -o build/control.o

build/input.o: src/input.c include/input.h
	gcc -c src/input.c -o build/input.o

# Clean Target
clean:
	rm build/*o
	rm $(output)
	rm /tmp/normandy_*

# Run Target
run:
	./$(output)
