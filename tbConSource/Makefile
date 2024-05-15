CC=cc
PREFIX=/usr/local/bin
CFLAGS=-static -std=c89 -O3
MACCFLAGS=-O3

DEBUG_FLAGS=-static -std=c89 -O0 -ggdb -Wall
GDB_NAME=gdb

outNameStr=tbCon

source=\
   ../generalLib/samEntryStruct.c\
   tbCon-fun.c\
   tbCon-input.c\
   tbCon.c

all:
	$(CC) $(CFLAGS) -o $(outNameStr) $(source)

mac:
	$(CC) $(MACCFLAGS) -o $(outNameStr) $(source)

debug:
	$(CC) $(DEBUG_FLAGS) -o bugTbCon $(source)
	$(GDB_NAME) -x bug-cmds.txt bugTbCon

# For checking error output
check:
	$(CC) $(DEBUG_FLAGS) -o bugTbCon $(source)

install:
	cp $(outNameStr) $(PREFIX)
	chmod a+xr $(outNameStr) $(PREFIX)/$(outNameStr)
