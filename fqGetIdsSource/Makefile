PREFIX=/usr/local/bin
CC=cc
NAME=fqGetIds
CFLAGS=\
  -static \
  --std=c89 \
  -O3

MACCFLAGS=-O3

FILES=\
  vectorWrap.c \
  fqGetIdsFqFun.c \
  fqGetIdsStructs.c \
  fqGetIdsAVLTree.c \
  fqGetIdsHash.c \
  fqGetIdsSearchFq.c \
  fqGetIds.c

# Program to extract fastq entries by a set of read ids
all:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME)

mac:
	$(CC) $(MACCFLAGS) $(FILES) -o $(NAME)

sse:
	$(CC) $(CFLAGS) -DSSE $(FILES) -o $(NAME);

avx2:
	$(CC) $(CFLAGS) -DAVX2 -mavx -mavx2 $(FILES) -o $(NAME);

avx512:
	$(CC) $(CFLAGS) -DAVX512 -mavx512bw $(FILES) -o $(NAME);

neon:
	$(CC) $(CFLAGS) -DNEON -mfpu=neon $(FILES) -o $(NAME);

install:
	mv $(NAME) $(PREFIX)
	chmod a+x $(PREFIX)/$(NAME)

# For cleaning benchmarking programs
clean:
	rm fqGetIdsScalar fqGetIdsSSE fqGetIdsAVX2 fqGetIdsDebug;
