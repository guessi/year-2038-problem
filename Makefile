CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -Wno-integer-overflow
TARGET = demo
SOURCES = demo.c

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: all clean
