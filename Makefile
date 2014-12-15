CC = g++
CFLAGS = -c -g

SRCDIR = src

INCL = -I$(SRCDIR)

FILES = \
    definitions.o \
    space.o \
    block.o \
    board.o \
    game.o \
    parser.o \
    directoryLoader.o \
    features.o \
    featureFile.o \
    board.features.o \
    rprop.o \
    rprop.training.o

gobot: main.o $(FILES)
	$(CC) -o gobot $^

all: gobot reformat

reformat: reformat.hs
	ghc $^

clean:
	rm -f *.o *.hi

%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $^ $(INCL)
