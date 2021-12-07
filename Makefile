CC=gcc
BIN=/usr/local/bin
CFLAGS=-Wall -I. -std=c99 -g
.PHONY: install clean purge

cfetch: cfetch.c
	@$(CC) -o $@ $^ $(CFLAGS)

install:
	@sudo cp cfetch $(BIN)

clean:
	@rm cfetch

purge: clean
	@sudo rm $(BIN)/cfetch
