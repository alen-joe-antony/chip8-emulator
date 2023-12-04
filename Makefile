CC = x86_64-w64-mingw32-gcc
CFLAGS = -g -Wall -I ./include/
LDFLAGS = -L ./lib/ -lmingw32 -lSDL2main -lSDL2

SRCDIR = ./src
OBJDIR = ./build
BINDIR = ./bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/main.exe

.PHONY: all build clean

all: clean build

build: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJDIR)/* $(TARGET)

