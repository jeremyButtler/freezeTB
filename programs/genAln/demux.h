/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' demux SOF: Start Of File
'   - has functions to demultiplexes reads by barcode
'   o header:
'     - guards and forward declarations
'   o fun01: sortBarcodes_demux
'     - sorts a the mapped barcodes by their barcode and
'       then start coordiante
'   o .c fun01: sortBarcodes_demux
'     - sorts the mapped barcodes by their barcode and
'       then by start coordiante
'   o .c fun02: sortStart_demux
'     - sorts the mapped barcodes by starting coordinates
'   o fun03: barcodeCoords_demux
'     - demux a read
'   o fun04: read_demux
'     - convert barcode coordinates to demuxed reads
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and forward declarations
\-------------------------------------------------------*/

#ifndef DE_MULTIPLEX_READS_H
#define DE_MULTIPLEX_READS_H

struct seqST;
struct tblST_kmerFind;
struct refST_kmerFind;

/*-------------------------------------------------------\
| Fun03: barcodeCoords_demux
|   - find the coordinates of barcodes in a read
| Input:
|   - lenSIPtr:
|     o signed int pointer to get length of returned array
|   - seqSTPtr:
|     o seqSTPtr struct pointer with read to demux
|   - barAryST:
|     o refST_kmerFind struct array with barcodes to demux
|   - barLenSI:
|     o number of barcodes in demux
|   - tblSTPtr:
|     o tblST_kmerFind struct pointer to use
|   - minPercScoreF:
|     o float with minimum percent score to keep a primer
|       mapping or discard
|     o recomend 0.9 (90%) to 0.95 (95%)
|     o do not go lower then 80% (otherwise to many
|       mappings kept)
|   - minScoreSL:
|     o signed long with minimum score to keep a primer
|       mapping or discard
|     o disable by using 0
|   - alnSetPtr:
|     o alnSet struct pointer with alignment settings
| Output:
|   - Modifies:
|     o lenSIPtr
|       - length of the returned array
|       - -1 if had two different barcodes overlapping
|       - -2 for memory errors
|   - Returns:
|     o array with the barcode index, start, end, and
|       score of each barcode
|       * index (n % 4 = 0) is barcode index
|       * index (n % 4 = 1) is the barcode start
|       * index (n % 4 = 2) is the barcode end
|       * index (n % 4 = 3) is the score
|       * ex: [index_1, start_1, end_1, score_1,
|              index_2, start_2 end_2, score_2,
|              index_3, start_3, end_3, score_3,
|              ...
|             ]
|     o for memory errors or no sequences (check lenSI)
\-------------------------------------------------------*/
signed int *
barcodeCoords_demux(
   signed int *lenSIPtr,    /*gets returned array length*/
   struct seqST *seqSTPtr,          /*has read to demux*/
   struct refST_kmerFind *barSTPtr, /*has barcodes*/
   signed int barLenSI,             /*number barcodes*/
   struct tblST_kmerFind *tblSTPtr, /*for searching*/
   float minPercScoreF,             /*min % score*/
   signed long minScoreSL,          /*min score to keep*/
   struct alnSet *alnSetPtr         /*settings*/
);

/*-------------------------------------------------------\
| Fun04: read_demux
|   - convert barcode coordinates to demuxed reads
| Input:
|   - seqSTPtr:
|     o seqSTPtr struct pointer with read to demux
|   - maxSplitSI:
|     o maximum number of splits before tossing read
|   - minDistSI:
|     o minimum distance between barcodes
|   - trimBl:
|     o 1: trim barcodes
|     o 0: do not trim barcodes
|   - coordsArySI:
|     o signed int array retured from barcodeCoords_demux
|       * index (n % 4 = 0) is barcode index
|       * index (n % 4 = 1) is the barcode start
|       * index (n % 4 = 2) is the barcode end
|       * index (n % 4 = 3) is the score
|   - coordLenSI:
|     o length of coordsArySI (number barcodes << 2)
|   - barFileStrAry:
|     o c-string array with output file names
|     o you need to check and make sure these files are
|       writeable before hand (I assume here)
| Output:
|   - Prints:
|     o demuxed and if requested split sequence to the
|       correct file in barFileStrAry
|   - Returns:
|     o 0 for no errors
|     o 1 if read had no barcodes
|     o 2 if read had to many barcodes
|     o 3 if read had barcodes at both ends; no easy split
|     o 4 if read had to barcodes right next to each other
\-------------------------------------------------------*/
signed int
read_demux(
   struct seqST *seqSTPtr,   /*read to demux*/
   signed int maxSplitSI,    /*max splits before discard*/
   signed int minDistSI, /*min distance between barcodes*/
   signed char trimBl,       /*1: trim barcodes*/
   signed int coordArySI[],  /*has barcode mappings*/
   signed int coordLenSI,    /*length of coordLenSI*/
   signed char *barFileStrAry[] /*output file names*/
);

#endif
