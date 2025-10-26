# maiku tui makefile
# Author: Francesco Garbin
#
# Preconditions: install package ncurses-devel
#
# Compile with:makefile all

CC=gcc
CFLAGS=-std=c23 -Wall -I./src/
DEPS=ncurses.h
LIBS=-lncurses
SRC_DIR=./src/
OBJ_DIR=./obj/
BIN_DIR=./bin/

all: setup_dirs compile

setup_dirs:
	mkdir -p ./obj
	mkdir -p ./bin

compile: maiku.o
	gcc $(CFLAGS) $(OBJ_DIR)maiku.o -o $(BIN_DIR)maiku $(LIBS)

maiku.o: $(SRC_DIR)maiku.c
	gcc $(CFLAGS) -c $(SRC_DIR)maiku.c -o $(OBJ_DIR)maiku.o

clean:
	rm -rf ./bin
	rm -rf ./obj
