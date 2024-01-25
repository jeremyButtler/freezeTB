CC = cc

CFLAGS += -static -O3 -Wno-unused-function

# For MACS (do not lie -Wno-unused-function
MACFLAGS += -static -O3

# Debuggin
BUGFLAGS += -ggdb -Wall -O0 -static -o debugAmpDepth

all:
	$(CC) $(CFLAGS) -o ampDepth ampDepth.c

mac:
	$(CC) $(MACFLAGS) -o ampDepth ampDepth.c

debug:
	$(CC) $(BUGFLAGS) -o debugAmpDepth ampDepth.c
	#gdb -ex "b ampDepth.c:68" -ex "r -paf ../old.paf -sam ../test.sam" debugAmpDepth
