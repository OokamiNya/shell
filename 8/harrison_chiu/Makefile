EXEC_NAME=mess

all: clean build run

clean: 
	for dir in src/*/; do make -C $$dir clean; done;
	rm -f ${EXEC_NAME}

build:
	for dir in src/*/; do make -C $$dir build; done;
	gcc -c "src/main.c" -o "obj/main.o"
	gcc obj/* -o ${EXEC_NAME}

run:
	./${EXEC_NAME}
