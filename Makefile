CC = g++
CFLAGS = -c -g

INCL = -I.

FILES = \
    board.o

main: main.o $(FILES)
	$(CC) -o main $^

clean:
	rm -f *.o

%.o: %.cpp
	$(CC) $(CFLAGS) $^ $(INCL)
