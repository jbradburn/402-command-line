main: create.o filelib.o
	gcc -o main create.o filelib.o

create.o: create.c filelib.h globals.h
	gcc -c create.c

filelib.o: filelib.c filelib.h globals.h
	gcc -c filelib.c

clean:
	rm -f *.o core