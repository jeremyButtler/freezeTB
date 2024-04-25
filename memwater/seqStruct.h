/*#########################################################
# Name: seqStruct
#   - Holds functions for reading in or manipulating
#     sequences
#########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o header:
'    - header guards
'  o .h st-01: seqStruct
'    - Holds an single sequence (fasta/fastq)
'  o .c fun-01 addLineToBuffSeqFun:
'    - Add characters from file to buffer, if needed 
'      resize. This will only read in till the end of the
'      line
'  o fun-02 readFqSeq:
'    - Reads a fastq sequence from a fastq file
'  o fun-03 readFaSeq:
'    - Grabs the next read in the fasta file
'  o .c fun-04 complementBase:
'    - Returns the complement of a base
'  o fun-05 reverseComplementSeq:
'    - Reverse complement a sequence
'  o .h fun-06 blankSeqST:
'    - Sets values in seqST to zero
'  o .h fun-07 initSeqST:
'     - Sets values in seqST to blank values
'  o fun-08 freeSeqSTStack:
'    - Frees variables in an seqStruct (calls blankSeqST)
'  o fun-09 freeSeqST:
'    - Frees an seqST strucuter (calls fredSeqSTStack)
'  o fun-10: freeSeqAryST
'    - Frees an array of seqStruct's
'  o fun-11 cpReadIdRPad:
'     - Copies read id to a buffer and adds in endIdC to
'       the end. If needed, this function will add right
'       padding of spaces to the end.
'  o fun-12: cpSeqST
'    - Copies an seqStruct structure
'  o license:
'    - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Header:
|  - header guards
\--------------------------------------------------------*/

#ifndef SEQUENCE_STRUCTURE_H
#define SEQUENCE_STRUCTURE_H

/*--------------------------------------------------------\
| ST-01: seqStruct
|  - Holds an single sequence (fasta/fastq)
\--------------------------------------------------------*/
typedef struct seqStruct
{ /*seqStruct*/
  char *idStr;               /*Id of th sequence*/
  unsigned long  lenIdUL;     /*Length of sequence id*/
  unsigned long  lenIdBuffUL; /*Lenght of Id buffer*/

  char *seqStr;              /*Sequence*/
  unsigned long  lenSeqUL;    /*Length of the sequence*/
  unsigned long  lenSeqBuffUL;/*Length sequence buffer*/

  char *qStr;                /*q-score entry*/
  unsigned long  lenQUL;      /*Length of the Q-score*/
  unsigned long  lenQBuffUL;  /*Length of Q-score buffer*/

  unsigned long  offsetUL;    /*Offset for an alignment*/
  unsigned long  endAlnUL;    /*Marks end of alignment*/
}seqStruct;


/*-------------------------------------------------------\
| Fun-02: readRefFqSeq
|  - Grabs the next read in the fastq file
| Input:
|  - fqFILE:
|    o Pointer to FILE handle to a fastq file to get the
|      next read from
|  - seqST:
|    o Pointer to seqStruct structure to hold next read
| Output:
|  - Modifies:
|    o refStruct to hold the read in fastq entry & sets
|      its pointers
|  - Returns:
|     o 0: if EOF
|     o 1: if succeded
|     o 2: If file was not a fastq file
|     o 130: If file had an invalide entry
|       - This error uses to flags, first it uses 2 to
|         specify that it is not a fastq file
|       - 2nd it uses 128 to specifty that it is not an
|         blank file
|     o 64: If malloc failed to find memory
\-------------------------------------------------------*/
unsigned char
readFqSeq(
  void *fqFILE,           /*fastq file with sequence*/
  struct seqStruct *seqST /*Will hold one fastq entry*/
);

/*-------------------------------------------------------\
| Fun-03 TOC: readFaSeq
|  -  Grabs the next read in the fasta file
| Input:
|  - faFILE:
|    o Pointer to FILE handle to a fasta file to get the
|      next read from
|  - seqST:
|    o Pointer to seqStruct structure to hold next read
| Output:
|  - Modifies:
|    o seqST to hold one fasta entry
|  - Returns:
|     o 0: if EOF
|     o 1: if succeded
|     o 2: for an invalid fasta entry
|     o 64: If malloc failed to find memory
| Note:
|   - This will remove new lines from the sequence.
|   - This will only remove spaces or tabs at the end of
|     each sequence entry, so "atg atc \n" will got to
|     "atc atc".
\-------------------------------------------------------*/
unsigned char
readFaSeq(
  void *faFILE,           /*Fasta file with sequence*/
  struct seqStruct *seqST /*Will hold one fastq entry*/
);

/*-------------------------------------------------------\
| Fun-05: reverseComplementSeq
|  - Reverse complement a sequence
| Input:
|  - seqST:
|    o Pointer to seqStruct structure with a sequence to
|      reverse complement
| Output:
|  - Modfies
|    o Reverse complements the sequence entry and if
|      reverses the Q-score entry
\-------------------------------------------------------*/
void
reverseComplementSeq(
  struct seqStruct *seqST  /*Sequence to reverse comp*/
);


/*-------------------------------------------------------\
| Fun-06: blankSeqST
|  - Sets values in seqST to blank values
| Input:
|  - seqSTPtr:
|    o Pointer to an seqStruct structuer to blank
| Output:
|  - Modifies
|    o Sets id, sequence, and Q-score entreis to start
|      with '\0' and the id, sequence, and Q-score lengths
|      to 0. This does not change the buffer lengths.
\-------------------------------------------------------*/
#define \
blankSeqST(seqSTPtr){\
   if((seqSTPtr)->idStr)\
      *(seqSTPtr)->idStr = '\0';\
   \
   (seqSTPtr)->lenIdUL = 0;\
   \
   if((seqSTPtr)->seqStr)\
      *(seqSTPtr)->seqStr='\0';\
   \
   (seqSTPtr)->lenSeqUL = 0;\
   \
   if((seqSTPtr)->qStr)\
      *(seqSTPtr)->qStr = '\0';\
   \
   (seqSTPtr)->lenQUL = 0;\
   \
   (seqSTPtr)->offsetUL = 0;\
   (seqSTPtr)->endAlnUL = 0;\
} /*blankSeqST*/

/*-------------------------------------------------------\
| Fun-07: initSeqST
|  - Sets vlues in seqST to zero
| Input:
|  - seqSTPtr:
|    o Pointer to an seqStruct structuer to initialize
| Output:
|  - Modifies
|    o all values in seqST to be 0
\-------------------------------------------------------*/
#define \
initSeqST(seqSTPtr){\
   (seqSTPtr)->idStr = 0;\
   (seqSTPtr)->lenIdBuffUL = 0;\
   \
   (seqSTPtr)->seqStr = 0;\
   (seqSTPtr)->lenSeqBuffUL = 0;\
   \
   (seqSTPtr)->qStr = 0;\
   (seqSTPtr)->lenQBuffUL = 0;\
   \
   blankSeqST((seqSTPtr));\
} /*initSeqST*/

/*-------------------------------------------------------\
| Fun-08: freeSeqSTStack
|  - Frees the variables in an seqStruct strucuter
| Input:
|  - seqSTPtr:
|    o Pointer to an seqStruct structuer that has variables
|      to free
| Output:
|  - Frees
|    o seqSTPtr-idStr and sets to 0
|    o seqSTPtr-seqStr and sets to 0
|    o seqSTPtr-qStr and sets to 0
\-------------------------------------------------------*/
void
freeSeqSTStack(
   struct seqStruct *seqSTPtr
);

/*-------------------------------------------------------\
| Fun-09: freeSeqST
|  - Frees the seqST strucuter
| Input:
|  - seqSTPtr:
|    o Pointer to an seqStruct structuer to free
| Output:
|  - Frees
|    o seqST from memory and sets to 0
\-------------------------------------------------------*/
void
freeSeqST(
   struct seqStruct *seqSTPtr
);

/*-------------------------------------------------------\
| Fun-10: freeSeqAryST
|  - Frees an array of seqStruct's
| Input:
|  - seqSTAryPtr:
|    o Pointer to an arrya of seqStruct structuer to free
|  - numSeqSI:
|    o Number of seqStructs in the seqStruct array
|     (seqSTAryPtr)
| Output:
|  - Frees
|    o seqST from memory
\-------------------------------------------------------*/
void
freeSeqAryST(
   struct seqStruct *seqSTAryPtr,
   int  numSeqSI
);

/*-------------------------------------------------------\
| Fun-11: cpReadIdRPad
|  - Copies read id to a buffer and adds in endIdC to
|    the end. If needed, this function will add right
|    padding of spaces to the end.
| Input:
|  - seqST:
|    o Pointer to seqStruct with read id to copy
|  - buffStr:
|    o c-string to copy read id to
|  - endIdC:
|    o character to add in to mark the end of the read id
|  - padRI:
|    o Amount of padding to add to the right of the read
|      id
| Output:
|  - Modifies:
|    o buffStr to hold the sequence id + endIdC. If the
|      id + endIdC is shorter than padRI, the copied id is
|      padded with spaces on the right till it is the same
|      size as padRI.
|  - Returns:
|    o Pointer to end of copied id or padding if padding
|      was applied.
\-------------------------------------------------------*/
char *
cpReadIdRPad(
   struct seqStruct *seqST, /*Has read id to copy*/
   char *buffStr,           /*Buffer to add read id to*/
   char endIdC,      /*padding for end of id (0 to skip)*/
   int padRI         /*Padding to add to right of id*/
);

/*-------------------------------------------------------\
| Fun-12: cpSeqST
|   - Copies an seqStruct structure
| Input:
|   - dupSeqST:
|     o Pointer to an seqStruct to copy to (duplicate)
|   - cpSeqST:
|     o Pointer to an seqStruct to copy
| Output:
|   - Modifies:
|     o dupSeqST to be an copy of cpSeqST
|   - returns:
|     o 0 for no errors
|     o 64 for an memory error
\-------------------------------------------------------*/
signed char
cpSeqST(
   struct seqStruct *dupSeqST, /*copy to (duplicate)*/
   struct seqStruct *cpSeqST   /*seqStruct to copy*/
);

#endif
