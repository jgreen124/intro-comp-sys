CC = gcc
CFLAGS = -Wall -pthread
OBJ = main.o thread.o utils.o

all: program

program: $(OBJ)
	$(CC) $(CFLAGS) -o program $(OBJ)

main.o: main.c thread.h utils.h
	$(CC) $(CFLAGS) -c main.c

thread.o: thread.c thread.h utils.h
	$(CC) $(CFLAGS) -c thread.c

utils.o: utils.c utils.h thread.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f *.o program
