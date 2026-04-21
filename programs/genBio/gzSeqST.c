/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' seqST SOF: Start Of File
'   - holds gz functions for seqST
'   - the reason this is separate is because my inflate
'     libraries adds around 18kb to all programs, even
'     ones that do not use it
'   o header:
'     - included libraries
'   o fun01: get_gzSeqST
'     - gets a sequence from a fastx/fastx.gz file
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "gzSeqST.h"
#include "seqST.h"
#include "../genLib/ulCp.h"
#include "../genLib/fileFun.h"
#include "../genLib/inflate.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/checkSum.h"
!   - .c  #include "../genLib/genMath.h"
!   - .c  #include "../genLib/endin.h"
!   - .h  #include "../genLib/endLine.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: get_gzSeqST
|   - gets a sequence from a fastx/fastx.gz file
| Input:
|   - fileSTPtr:
|     o file_inflate struct to use in uncompression
|   - typeSCPtr:
|     o is set to or has the correct file type
|   - seqSTPtr:
|     o seqST struct pointer to add new sequence to
|   - inFILE:
|     o FILE pointer to file to add (new file)
|     o 0 if already called get_gzSeqST for this file
|     o must be opened in binary format
|       * fopen(<name>, "rb");
|     o you should set inFILE to 0/null after calling,
|       because freeStack_file_inflate 
|       freeHeap_file_inflate will close inFILE for you
| Output:
|   - Modifies:
|     o if have inFILE, sets fileSTPtr be blanked and then
|       have inFILE
|     o if have inFILE, typeSCPtr to have file type 
|       o def_noType_gzSeqST if end of file
|       o def_faType_gzSeqST if fasta file
|       o def_faType_gzSeqST | def_gzType_gzSeqST if
|         fasta.gz file
|       o def_fqType_gzSeqST if fastq file
|       o def_fqType_gzSeqST | def_gzType_gzSeqST if
|         fastq.gz file
|     o fileSTPtr to be on next read/sequence
|     o seqSTPtr to have next read/sequence
|   - Returns:
|     o 0 for no errors
|     o def_EOF_seqST if at end of file
|     o def_memErr_seqST for memory errors
|     o def_badLine_seqST | def_fileErr_seqST for invalid
|       file entries
|     o def_fileErr_seqST for file errors (including gzip
|       checks)
\-------------------------------------------------------*/
signed char
get_gzSeqST(
   struct file_inflate *fileSTPtr, /*has file to extract*/
   signed char *typeSCPtr,         /*gets/has file type*/
   struct seqST *seqSTPtr,         /*gets sequence*/
   void *inFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - gets a sequence from a fastx/fastx.gz file
   '   o fun01 sec01:
   '     - variable declarations
   '   o fun01 sec02:
   '     - add input file (if one) to file_inflate struct
   '   o fun01 sec03:
   '     - get read id
   '   o fun01 sec04:
   '     - get sequence
   '   o fun01 sec05:
   '     - get q-score entry (fastq only)
   '   o fun01 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define extraBytes_fun01 1024;
   signed char errSC = 0;
   signed char *tmpStr = 0;
   signed char tmpSC = 0;
   signed char tmpArySC[4];

   signed long lenSL = 0;
   signed long bytesSL = 0;
   signed long tmpSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - add input file (if one) to file_inflate struct
   ^   o fun01 sec02 sub01:
   ^     - get memory for id (if no memory)
   ^   o fun01 sec02 sub02:
   ^     - if file input; get file type and id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec02 Sub01:
   *   - get memory for id (if no memory)
   \*****************************************************/

   if(! seqSTPtr->idStr)
   { /*If: need memory for read id*/
      seqSTPtr->idStr = malloc(136 * sizeof(signed char));
      if(! seqSTPtr)
         goto memErr_fun01_sec06;
      seqSTPtr->idSizeSL = 128;
   } /*If: need memory for read id*/

   blank_seqST(seqSTPtr);

   /*****************************************************\
   * Fun01 Sec02 Sub02:
   *   - if file input; get file type and id
   *   o fun01 sec02 sub02 cat01:
   *     - find if .gz file and get first line
   *   o fun01 sec02 sub02 cat02:
   *     - check if .fastq file .fasta
   *   o fun01 sec02 sub02 cat03:
   *     - first line empty, scan for fasta header
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun01 Sec02 Sub02 Cat01:
   +   - find if .gz file and get first line
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(inFILE)
   { /*If: user is starting new file*/
      *typeSCPtr = 0;

      if(
        ! fread(
           (char *) seqSTPtr->idStr,
           sizeof(signed char),
           2,
           inFILE
         )
      ) goto fileErr_fun01_sec06;

      if(seqSTPtr->idStr[0] == 0x1f) /*0x1f = 31*/
      { /*If: gz file or invalid file*/
         *typeSCPtr = def_gzType_gzSeqST;

         errSC =
           add_file_inflate(
              inFILE,
              fileSTPtr,
              seqSTPtr->idStr
           );

         if(errSC == def_memErr_inflate)
            goto memErr_fun01_sec06;
         else if(errSC)
            goto fileErr_fun01_sec06;

         seqSTPtr->idLenSL =
            get_inflate(
               fileSTPtr,
               seqSTPtr->idSizeSL,
               1, /*till line ends or buffer is full*/
               seqSTPtr->idStr,
               &errSC
            ); /*get first line or first 128 characters*/

         if(errSC == def_memErr_inflate)
            goto memErr_fun01_sec06;
         else if(errSC)
            goto fileErr_fun01_sec06;
      } /*If: gz file or invalid file*/

      else
      { /*Else: file is not gz compressed*/
         blank_file_inflate(fileSTPtr, 2);
         fileSTPtr->zipFILE = inFILE;

         seqSTPtr->idLenSL =
            getLine_fileFun(
               fileSTPtr->zipFILE,
               &seqSTPtr->idStr[2],
               seqSTPtr->idSizeSL - 2,
               &tmpSL
            ); /*get first line into buffer*/

         if(! seqSTPtr->idLenSL)
            goto fileErr_fun01_sec06;
         seqSTPtr->idLenSL += 2;
      } /*Else: file is not gz compressed*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec02 Sub02 Cat02:
      +   - check if .fastq file .fasta
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(seqSTPtr->idStr[0] == '@')
         *typeSCPtr |= def_fqType_gzSeqST;
      else if(seqSTPtr->idStr[0] == '>')
         *typeSCPtr |= def_faType_gzSeqST;
      else if(seqSTPtr->idStr[1] == '>')
         *typeSCPtr |= def_faType_gzSeqST;
      else if(seqSTPtr->idStr[2] == '>')
         *typeSCPtr |= def_faType_gzSeqST;
      else
      { /*Else: may need to get past blank lines*/
         if(seqSTPtr->idStr[0] > 32)
            goto fileErr_fun01_sec06; /*unkown file*/
         else if(seqSTPtr->idStr[1] > 32)
            goto fileErr_fun01_sec06; /*unkown file*/
         else if(seqSTPtr->idStr[2] > 32)
            goto fileErr_fun01_sec06; /*unkown file*/

         if(seqSTPtr->idLenSL > 3)
         { /*If: have something to copy*/
            seqSTPtr->idLenSL -= 3;
            cpLen_ulCp(
               seqSTPtr->idStr,
               &seqSTPtr->idStr[3],
               seqSTPtr->idLenSL
            );
         } /*If: have something to copy*/

         else
            seqSTPtr->idLenSL = 0;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun01 Sec02 Sub02 Cat03:
         +   - first line empty, scan for fasta header
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         while(! (*typeSCPtr & def_faType_gzSeqST) )
         { /*Loop: check if fasta or invalid file*/
            tmpStr = seqSTPtr->idStr;
            while(*tmpStr < 33 && *tmpStr)
               ++tmpStr; /*find first non-white space*/
                
            if(*tmpStr == '>')
            { /*If: found file type*/
               *typeSCPtr |= def_faType_gzSeqST;
               break;
            } /*If: found file type*/

            else if(*tmpStr)
               goto fileErr_fun01_sec06; /*not .fasta*/
            else
               seqSTPtr->idLenSL = 0; /*only white space*/


            if(*typeSCPtr & def_gzType_gzSeqST)
            { /*If: reading gz file*/
               seqSTPtr->idLenSL =
                  get_inflate(
                     fileSTPtr,
                     seqSTPtr->idSizeSL,
                     1, /*one line or full buffer*/
                     seqSTPtr->idStr,
                     &errSC
                  );

               if(errSC == def_memErr_inflate)
                  goto memErr_fun01_sec06;
               else if(errSC)
                  goto fileErr_fun01_sec06;
            } /*If: reading gz file*/


            else
            { /*Else: fasta file (uncompressed)*/
               seqSTPtr->idLenSL =
                  getLine_fileFun(
                     fileSTPtr->zipFILE,
                     seqSTPtr->idStr,
                     seqSTPtr->idSizeSL,
                     &tmpSL
                  ); /*get first line into buffer*/

               if(! seqSTPtr->idLenSL)
                  goto fileErr_fun01_sec06;
            } /*Else: fasta file (uncompressed)*/
         } /*Loop: check if fasta or invalid file*/
      } /*Else: may need to get past blank lines*/
   } /*If: user is starting new file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - get read id
   ^   o fun01 sec03 sub01:
   ^     - get first line of read id (if no input file)
   ^   o fun01 sec03 sub02:
   ^     - get read id
   ^   o fun01 sec03 sub03:
   ^     - remove line break and header marker from id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec03 Sub01:
   *   - get first line of read id (if no input file)
   \*****************************************************/

   if(! inFILE)
   { /*If: need to get the first line still*/

      if(*typeSCPtr & def_gzType_gzSeqST)
      { /*If: is gz file*/
         errSC = 0;

         while(! errSC)
         { /*Loop: get next line*/
            seqSTPtr->idLenSL =
               get_inflate(
                  fileSTPtr,
                  seqSTPtr->idSizeSL,
                  1, /*till line ends or buffer is full*/
                  seqSTPtr->idStr,
                  &errSC
               ); /*get first line*/

            if(errSC == def_memErr_inflate)
               goto memErr_fun01_sec06;

            else if(errSC == def_eof_inflate)
            { /*Else If: at end of gz file or entry*/
               errSC = next_file_inflate(fileSTPtr, 0, 0);

               if(errSC == def_eof_inflate)
                  goto eof_fun01_sec06;
               else if(errSC)
                  goto fileErr_fun01_sec06;
            } /*Else If: at end of gz file or entry*/

            else
               break; /*got one line*/
         } /*Loop: get next line*/
      } /*If: is gz file*/

      else
      { /*Else: reading uncompressed file*/
         seqSTPtr->idLenSL =
            getLine_fileFun(
               fileSTPtr->zipFILE,
               seqSTPtr->idStr,
               seqSTPtr->idSizeSL,
               &tmpSL
            ); /*get first line into buffer*/
        if(! seqSTPtr->idLenSL)
           goto eof_fun01_sec06;
      } /*Else: reading uncompressed file*/
   } /*If: need to get the first line still*/

   /*****************************************************\
   * Fun01 Sec03 Sub02:
   *   - get read id
   \*****************************************************/

   while(
         seqSTPtr->idStr[seqSTPtr->idLenSL - 1] != '\n'
      && seqSTPtr->idStr[seqSTPtr->idLenSL - 1] != '\r'
   ){ /*Loop: get read id*/
      tmpStr =
         realloc(
            seqSTPtr->idStr,
            (seqSTPtr->idLenSL + 136)
               * sizeof(signed char)
         );

      if(! tmpStr)
         goto memErr_fun01_sec06;
      seqSTPtr->idStr = tmpStr;
      seqSTPtr->idSizeSL += 128;

      if(*typeSCPtr & def_gzType_gzSeqST)
      { /*If: is gz file*/
         seqSTPtr->idLenSL +=
            get_inflate(
               fileSTPtr,
               128,
               1, /*till line ends or buffer is full*/
               &seqSTPtr->idStr[seqSTPtr->idLenSL],
               &errSC
            );

            if(errSC == def_memErr_inflate)
               goto memErr_fun01_sec06;
            else if(errSC == def_eof_inflate)
               goto badLine_fun01_sec06;
            else if(errSC)
               goto fileErr_fun01_sec06;
      } /*If: is gz file*/

      else
      { /*Else: uncompressed file*/
         seqSTPtr->idLenSL +=
            getLine_fileFun(
               fileSTPtr->zipFILE,
               &seqSTPtr->idStr[seqSTPtr->idLenSL],
               128,
               &tmpSL
            ); /*get first line into buffer*/
        if(! seqSTPtr->idLenSL)
           goto badLine_fun01_sec06;
      } /*Else: uncompressed file*/
   } /*Loop: get read id*/

   /*****************************************************\
   * Fun01 Sec03 Sub03:
   *   - remove line break and header marker from id
   \*****************************************************/

   while(
         seqSTPtr->idStr[seqSTPtr->idLenSL - 1] == '\n'
      || seqSTPtr->idStr[seqSTPtr->idLenSL - 1] == '\r'
   ) --seqSTPtr->idLenSL;

   tmpSL = 0;
   while(
         seqSTPtr->idStr[tmpSL] < 33
      && tmpSL < seqSTPtr->idLenSL
   ) ++tmpSL;

   if(seqSTPtr->idStr[tmpSL] == '@')
      ++tmpSL;
   else if(seqSTPtr->idStr[tmpSL] == '>')
      ++tmpSL;

   cpLen_ulCp(
      seqSTPtr->idStr,
      &seqSTPtr->idStr[tmpSL], /*move past header symbol*/
      seqSTPtr->idLenSL - tmpSL
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - get sequence
   ^   o fun01 sec04 sub01:
   ^     - get first line or part of line in sequence
   ^   o fun01 sec04 sub02:
   ^     - check if reached end of fastq sequence
   ^   o fun01 sec04 sub03:
   ^     - check if reached end of fasta sequence
   ^   o fun01 sec04 sub04:
   ^     - make sure have memory for next line
   ^   o fun01 sec04 sub05:
   ^     - read in next line or part of line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec04 Sub01:
   *   - get first line or part of line in sequence
   \*****************************************************/

   if(! seqSTPtr->seqStr)
   { /*If: need to get memory for sequence*/
      seqSTPtr->seqStr =
         malloc(1032 * sizeof(signed char));
      if(! seqSTPtr->seqStr)
         goto memErr_fun01_sec06;
      seqSTPtr->seqSizeSL = 1024;
   } /*If: need to get memory for sequence*/


   if(*typeSCPtr & def_gzType_gzSeqST)
   { /*If: reading gz file*/
      lenSL =
         get_inflate(
            fileSTPtr,
            seqSTPtr->seqSizeSL,
            1, /*till line ends or buffer is full*/
            seqSTPtr->seqStr,
            &errSC
         ); /*get first sequence line*/

      if(errSC == def_memErr_inflate)
         goto memErr_fun01_sec06;
      else if(errSC == def_eof_inflate)
         goto badLine_fun01_sec06;
      else if(errSC)
         goto fileErr_fun01_sec06;
   } /*If: reading gz file*/

   else
   { /*Else: reading uncompressed file*/
      lenSL =
         getLine_fileFun(
            fileSTPtr->zipFILE,
            seqSTPtr->seqStr,
            seqSTPtr->seqSizeSL,
            &tmpSL
         );

      if(! lenSL)
         goto badLine_fun01_sec06;
   } /*Else: reading uncompressed file*/

   /*****************************************************\
   * Fun01 Sec04 Sub02:
   *   - check if reached end of fastq sequence
   \*****************************************************/

   while(1 == 1)
   { /*Loop: read in sequence*/
      tmpSC = 0;
      tmpArySC[0] = 0;

      if(*typeSCPtr & def_fqType_gzSeqST)
      { /*If: looking for fastq file ending*/
         if(seqSTPtr->seqStr[seqSTPtr->seqLenSL] == '+')
            break;
         seqSTPtr->seqLenSL += lenSL;

         if(*typeSCPtr & def_gzType_gzSeqST)
            bytesSL += lenSL;
         else
            bytesSL += tmpSL;
      } /*If: looking for fastq file ending*/

      /**************************************************\
      * Fun01 Sec04 Sub03:
      *   - check if reached end of fasta sequence
      *   o  fun01 sec04 sub03 cat01:
      *     - next fasta char from .gz file + .fa* check
      *   o fun01 sec04 sub03 cat02:
      *     - get next fasta char from .fa file
      *   o fun01 sec04 sub03 cat03:
      *     - check if have header or sequence
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec04 Sub03 Cat01:
      +   - get next fasta char from .gz file + .fa* check
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(*typeSCPtr & def_faType_gzSeqST)
      { /*Else If: fasta file*/
         seqSTPtr->seqLenSL += lenSL;

         if(*typeSCPtr & def_gzType_gzSeqST)
         { /*If: reading from gz file*/
            while(! errSC)
            { /*Loop: find if next line is header*/
               lenSL =
                  getc_file_inflate(fileSTPtr, tmpArySC);

               if(lenSL < 0)
               { /*If: error*/
                  lenSL *= -1;

                  if(lenSL == def_memErr_inflate)
                     goto memErr_fun01_sec06;
                  else if(lenSL == def_eof_inflate)
                     goto done_fun01_sec06;
                     /*call eof on next sequence read*/
                  else
                     goto fileErr_fun01_sec06;
               } /*If: error*/

               tmpSC = tmpArySC[0];

               if(tmpSC > 32)
                  break;
            } /*Loop: find if next line is header*/
         } /*If: reading from gz file*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun01 Sec04 Sub03 Cat02:
         +   - get next fasta char from .fa file
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else
         { /*Else: reading uncompressed file*/
            tmpSC  = 0;

            while(tmpSC != EOF)
            { /*Loop: find first non-white space*/
               tmpSC = fgetc(fileSTPtr->zipFILE);

               if(tmpSC > 32)
                  break;
               else if(tmpSC == EOF)
                  goto done_fun01_sec06;
            }  /*Loop: find first non-white space*/
         } /*Else: reading uncompressed file*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun01 Sec04 Sub03 Cat03:
         +   - check if have header or sequence
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(tmpSC == '>')
         { /*If: on next header*/
            if(*typeSCPtr & def_gzType_gzSeqST)
               unget_file_inflate(fileSTPtr, tmpSC);
            else
               ungetc(tmpSC, fileSTPtr->zipFILE);
            goto done_fun01_sec06;
         } /*If: on next header*/

         else if( (tmpSC & ~32) > 90 )
            goto fileErr_fun01_sec06; /*non a-z*/
         else if( (tmpSC & ~32) > 64 )
            ; /*a-z*/
         else if(tmpSC == '-')
            ; /*gap*/
         else
            goto badLine_fun01_sec06;
            /*not part of sequence*/
      } /*Else If: fasta file*/

      /**************************************************\
      * Fun01 Sec04 Sub04:
      *   - make sure have memory for next line
      \**************************************************/

      if(
            seqSTPtr->seqLenSL
         >= seqSTPtr->seqSizeSL - 128
      ){ /*If: need more memory*/
         seqSTPtr->seqSizeSL +=
            (seqSTPtr->seqSizeSL >> 1);

         tmpStr =
            realloc(
               seqSTPtr->seqStr,
               (seqSTPtr->seqSizeSL + 8)
                  * sizeof(signed char)
            );
         if(! tmpStr)
            goto memErr_fun01_sec06;
         seqSTPtr->seqStr = tmpStr;
      } /*If: need more memory*/

      /**************************************************\
      * Fun01 Sec04 Sub05:
      *   - read in next line or part of line
      \**************************************************/

      if(tmpSC)
         seqSTPtr->seqStr[seqSTPtr->seqLenSL++] = tmpSC;


      if(*typeSCPtr & def_gzType_gzSeqST)
      { /*If: reading gz file*/
         lenSL =
            get_inflate(
               fileSTPtr,
               seqSTPtr->seqSizeSL - seqSTPtr->seqLenSL,
               1, /*till line ends or buffer is full*/
               &seqSTPtr->seqStr[seqSTPtr->seqLenSL],
               &errSC
            );
         tmpSL = lenSL;

         if(errSC == def_memErr_inflate)
            goto memErr_fun01_sec06;
         else if(errSC == def_eof_inflate)
            goto eof_fun01_sec06;
         else if(errSC)
            goto fileErr_fun01_sec06;
      } /*If: reading gz file*/

      else
      { /*Else: reading uncompressed file*/
         lenSL =
            getLine_fileFun(
               fileSTPtr->zipFILE,
               &seqSTPtr->seqStr[seqSTPtr->seqLenSL],
               seqSTPtr->seqSizeSL - seqSTPtr->seqLenSL,
               &tmpSL
            );
            
         if(! lenSL && *typeSCPtr & def_fqType_gzSeqST)
            goto fileErr_fun01_sec06;
         else if(! lenSL)
            goto eof_fun01_sec06;
      } /*Else: reading uncompressed file*/
   } /*Loop: read in sequence*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec05:
   ^   - get q-score entry (fastq only)
   ^   o fun01 sec05 sub01:
   ^     - find length of q-score entry from sequence
   ^   o fun01 sec05 sub02:
   ^     - make sure have fastq spacer line read in
   ^   o fun01 sec05 sub03:
   ^     - get q-score entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec05 Sub01:
   *   - find length of q-score entry from sequence entry
   \*****************************************************/

   seqSTPtr->seqStr[seqSTPtr->seqLenSL] = 0;

   if(! seqSTPtr->qStr) ;

   else if(seqSTPtr->qSizeSL < bytesSL + 1)
   { /*Else If: q-score buffer is to small*/
      free(seqSTPtr->qStr);
      seqSTPtr->qStr = 0;
   } /*Else If: q-score buffer is to small*/

   if(! seqSTPtr->qStr)
   { /*If: need to get memory for sequence*/
      seqSTPtr->qStr =
         malloc((bytesSL + 8) * sizeof(signed char));
      if(! seqSTPtr->qStr)
         goto memErr_fun01_sec06;
      seqSTPtr->qSizeSL = bytesSL;
   } /*If: need to get memory for sequence*/

   /*****************************************************\
   * Fun01 Sec05 Sub02:
   *   - make sure have fastq spacer line read in
   \*****************************************************/

   if(
         seqSTPtr->seqStr[seqSTPtr->seqLenSL + lenSL - 1]
      == '\n'
   ) ;

   else if(
         seqSTPtr->seqStr[seqSTPtr->seqLenSL + lenSL - 1]
      == '\r'
   ) ;

   else
   { /*Else: need to finish reading in + entry*/
      do{ /*Loop: get full + line*/
         if(*typeSCPtr & def_gzType_gzSeqST)
         { /*If: reading gz file*/
            lenSL =
               get_inflate(
                  fileSTPtr,
                  (signed long) seqSTPtr->qSizeSL,
                  1, /*get line + have extra for break*/
                  seqSTPtr->qStr,
                  &errSC
               ); /*get first line*/

            if(errSC == def_memErr_inflate)
               goto memErr_fun01_sec06;
            if(errSC == def_eof_inflate)
               goto badLine_fun01_sec06;
            else if(errSC)
               goto fileErr_fun01_sec06;
         } /*If: reading gz file*/

         else
         { /*Else: uncompressed file*/
            lenSL =
               getLine_fileFun(
                  fileSTPtr->zipFILE,
                  seqSTPtr->qStr,
                  seqSTPtr->qSizeSL,
                  &tmpSL
            );
            if(! lenSL)
               goto badLine_fun01_sec06;
         } /*Else: uncompressed file*/

         tmpStr = seqSTPtr->qStr + lenSL - 1;
      } while(*tmpStr != '\n' && *tmpStr != '\0');
        /*Loop: get full + line*/
   } /*Else: need to finish reading in + entry*/

   /*****************************************************\
   * Fun01 Sec05 Sub03:
   *   - get q-score entry
   \*****************************************************/

   if(*typeSCPtr & def_gzType_gzSeqST)
   { /*If: reading gz file*/
      seqSTPtr->qLenSL +=
         get_inflate(
            fileSTPtr,
            bytesSL,
            4, /*buffer has extra bytes for line ending*/
            seqSTPtr->qStr,
            &errSC
         ); /*get first sequence line*/

      if(errSC == def_memErr_inflate)
         goto memErr_fun01_sec06;

      else if(errSC == def_eof_inflate)
      { /*Else If: hit the end of the file*/
         if(seqSTPtr->qLenSL == seqSTPtr->seqLenSL)
            ; /*read in full entry*/
         else
            goto eof_fun01_sec06;
      } /*Else If: hit the end of the file*/

      else if(errSC)
         goto fileErr_fun01_sec06;
   } /*If: reading gz file*/

   else
   { /*Else: reading text fastq file*/
      seqSTPtr->qLenSL =
         fread(
            (char *) seqSTPtr->qStr,
            sizeof(signed char),
            bytesSL,
            fileSTPtr->zipFILE
         );
   } /*Else: reading text fastq file*/

   seqSTPtr->qStr[seqSTPtr->seqLenSL] = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun01_sec06:;
      errSC = 0;
      goto noErr_fun01_sec06;

   eof_fun01_sec06:;
      errSC = def_EOF_seqST;
      if(*typeSCPtr & def_gzType_gzSeqST)
      { /*If: gz file, might have more entries*/
         errSC = next_file_inflate(fileSTPtr, 0, 0);
         if(! errSC)
            goto done_fun01_sec06;
         else if(errSC != def_eof_inflate)
            goto fileErr_fun01_sec06;
         else
            errSC = def_EOF_seqST;
      } /*If: gz file, might have more entries*/

      goto noErr_fun01_sec06;

   noErr_fun01_sec06:;
      seqSTPtr->seqLenSL = rmWhite_ulCp(seqSTPtr->seqStr);

      if(*typeSCPtr & def_fqType_gzSeqST)
      { /*If: fastq file*/
         seqSTPtr->qLenSL = rmWhite_ulCp(seqSTPtr->qStr);

         if(seqSTPtr->qLenSL < seqSTPtr->seqLenSL)
            goto badLine_fun01_sec06;
      } /*If: fastq file*/

      goto ret_fun01_sec06;

   memErr_fun01_sec06:;
      errSC = def_memErr_seqST;
      goto ret_fun01_sec06;

   fileErr_fun01_sec06:;
      errSC = def_fileErr_seqST;
      goto ret_fun01_sec06;

   badLine_fun01_sec06:;
      errSC = def_badLine_seqST;
      errSC |= def_fileErr_seqST;
      goto ret_fun01_sec06;

   ret_fun01_sec06:;
      return errSC;
} /*get_gzSeqST*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
