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

# -------- Q3 Build Target --------
q3: program_q3

program_q3: main_q3.o thread_q3.o utils.o
	$(CC) $(CFLAGS) -o program_q3 main_q3.o thread_q3.o utils.o

main_q3.o: main_q3.c thread.h utils.h
	$(CC) $(CFLAGS) -c main_q3.c -o main_q3.o

thread_q3.o: thread_q3.c thread.h utils.h
	$(CC) $(CFLAGS) -c thread_q3.c -o thread_q3.o

# --------------------------------

clean:
	rm -f *.o program program_q3
