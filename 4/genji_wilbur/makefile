all: main.o prompt.o parser.o redir.o run.o check.o stringsep.o strip_spaces.o
	gcc -o we.out main.o prompt.o parser.o redir.o run.o check.o strip_spaces.o stringsep.o

main.o: main.c prompt.h parser.h
	gcc -c main.c

prompt.o: prompt.c prompt.h parser.h
	gcc -c prompt.c

parser.o: parser.c parser.h redir.c redir.h strip_spaces.c strip_spaces.h prompt.h
	gcc -c parser.c

redir.o: redir.c redir.h run.c run.h strip_spaces.c strip_spaces.h
	gcc -c redir.c

run.o: run.c run.h check.c check.h strip_spaces.c strip_spaces.h
	gcc -c run.c

check.o: check.c check.h 
	gcc -c check.c

stringsep.o: stringsep.c stringsep.h
	gcc -c stringsep.c

clean:
	rm *.o
	rm *~
