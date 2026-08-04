CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

SRC = $(wildcard src/*.c)

OUT = vm

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
~
run: all
	./$(OUT)

debug: all
	./$(OUT) -debug

clean:
	rm -f $(OUT)