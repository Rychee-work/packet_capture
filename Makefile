CC=gcc
CFLAGS=-Wall -O2
OBJ=capture.o socket_init.o parser.o

all: capture

capture: $(OBJ)
	$(CC) $(CFLAGS) -o capture $(OBJ)

clean:
	rm -f *.o capture
