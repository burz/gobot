CC = g++
CFLAGS = -c -g

INCL = -I.

FILES = \
    space.o \
    block.o \
    board.o \
    game.o \
    parser.o

main: main.o $(FILES)
	$(CC) -o main $^

reformat: reformat.hs
	ghc $^

clean:
	rm -f *.o *.hi

%.o: %.cpp
	$(CC) $(CFLAGS) $^ $(INCL)
