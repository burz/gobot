CC = clang++
CPPFLAGS = -std=c++11 -stdlib=libc++
CFLAGS = $(CPPFLAGS) -c -g -Wall

SRCDIR = src

INCL = -I$(SRCDIR)

FILES = \
    definitions.o \
    space.o \
    block.o \
    board.o \
    game.o \
    directoryLoader.o \
    blockFeatures.o \
    board.features.o \
    lifeFile.o \
    rprop.weights.o \
    rprop.layers.o \
    rprop.o \
    bootstrap.o

gobot: main.o $(FILES)
	$(CC) $(CPPFLAGS) -o gobot $^

all: gobot reformat

reformat: reformat.hs
	ghc $^

clean:
	rm -f *.o *.hi

%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $^ $(INCL)
