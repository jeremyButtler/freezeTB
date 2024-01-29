CC = cc
CFLAGS += -std=c99 -O3 -static -Wall -Wno-unused-function
DEBUG = -std=c99 -O0 -ggdb -static -Wall -Wno-unused-function

all:
	$(CC) $(CFLAGS) ampMapCounts.c -o ampMapCounts

debug:
	$(CC) $(DEBUG) ampMapCounts.c -o ampMapCountsDeb.o
	gdb -x CMDS.txt ampMapCountsDeb.o
	rm ampAmpCountsDeb.o