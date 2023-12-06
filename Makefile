CC = x86_64-w64-mingw32-g++
CFLAGS = -g -Wall -I ./include
LDFLAGS = -L ./lib -lmingw32 -lSDL2main -lSDL2

SRCDIR = ./src
OBJDIR = ./build
BINDIR = ./bin

SOURCES = $(wildcard $(SRCDIR)/*.cc)
OBJECTS = $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/main.exe

.PHONY: all build clean

all: clean build

build: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJDIR)/* $(TARGET)

