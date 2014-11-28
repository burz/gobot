CC = g++
CFLAGS = -c -g

INCL = -I.

FILES = \
    space.o \
    board.o

main: main.o $(FILES)
	$(CC) -o main $^

clean:
	rm -f *.o

%.o: %.cpp
	$(CC) $(CFLAGS) $^ $(INCL)
