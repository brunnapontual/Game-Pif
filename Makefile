TARGET = SnackSnake

CC = gcc

CFLAGS = -Wall -Wextra -pedantic -std=c11

LIBS = -lm

SOURCES = main.c timer.c screen.c keyboard.c mylib.c

all: $(TARGET)

$(TARGET): $(SOURCES)
		$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
		rm -f $(TARGET)

run: $(TARGET)
		./$(TARGET)

play: all run

.PHONY: clean run play
