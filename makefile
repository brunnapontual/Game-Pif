CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 
OBJECTS = main.o keyboard.o screen.o timer.o
TARGET = snake_game

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

main.o: main.c keyboard.h screen.h timer.h
	$(CC) $(CFLAGS) -c main.c

keyboard.o: keyboard.c keyboard.h
	$(CC) $(CFLAGS) -c keyboard.c

screen.o: screen.c screen.h
	$(CC) $(CFLAGS) -c screen.c

timer.o: timer.c timer.h
	$(CC) $(CFLAGS) -c timer.c

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
