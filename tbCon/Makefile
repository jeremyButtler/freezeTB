CC=cc
PREFIX=/usr/local/bin
CFLAGS=-static -std=c89 -O3 -Wno-unused-function
MAC_CFLAGS=-O3

DEBUG_FLAGS=-static -std=89 -O0 -ggdb -Wall
GDB_NAME=gdb

outNameStr=tbCon

all:
	$(CC) $(CFLAGS) -o $(outNameStr) tbCon.c

mac:
	$(CC) $(MAC_CFLAGS) -o $(outNameStr) tbCon.c

debug:
	$(CC) $(DEBUG_FLAGS) -o bugTbCon tbCon.h
	$(GDB_NAME) -x bug-cmds.txt bugTbCon

install:
	cp $(outNameStr) $(PREFIX)
	chmod a+x $(outNameStr) $(PREFIX)/$(outNameStr)
