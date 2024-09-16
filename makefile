# Compiler to use
CC = gcc

CFLAGS = -Wall -Wextra -pedantic -std=c11
TARGET = cshell
SOURCES = main.c commands.c

OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
