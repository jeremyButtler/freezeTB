
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../../generalLib/ulCpStr.h"
#include "mkMiruTbl-verison.h"

/*Maximum score for an base*/
#define def_matchScore_mkMiruTbl 5

/*-------------------------------------------------------\
| Fun-01: getInput_mkMiruTbl
|   - Gets user input they supplied with mkMiruTbl
| Input:
|   - numArgsSI:
|     o Number of arguments the user input
|   - argAryStr:
|     o Arguments the user input
|   - refFileStr:
|     o Will hold name/path of fasta file with reference
|   - primFileStr:
|     o Will hold name/path of fasta file with primers
|   - tblFileStr:
|     o Will hold name/path of tsv with MIRU table
|   - minPercScoreFL:
|     o Minimum percent score to keep an primer mapping
| Output:
|   - Modifies:
|     o all input variables to hold user input
|   - Returns:
|     o 0 for no errors
|     o (256 | argument #) for an non-numeric number error
|     o (502 | argument) for an invalid entry
|     o 1024 for the help message
|     o 2048 for the version number
\-------------------------------------------------------*/
int
getInput_mkMiruTbl(
   int numArgsSI,
   char *argAryStr[],
   char **refFileStr,
   char **primFileStr,
   char **tblFileStr,
   float *minPercScoreFL
){
   char *parmStr = 0;
   int siArg = 1;

   while(siArg < numArgsSI)
   { /*Loop: Get the user input*/
      if(! cStrEql("-ref", argAryStr[siArg], '\0'))
         *refFileStr = argAryStr[siArg + 1]; 

      else if(! cStrEql("-prim", argAryStr[siArg], '\0'))
         *primFileStr = argAryStr[siArg + 1]; 

      else if(!cStrEql("-miru-tbl",argAryStr[siArg],'\0'))
         *tblFileStr = argAryStr[siArg + 1]; 

      else if(
         ! cStrEql(
              "-min-perc-score",
              argAryStr[siArg],
              '\0'
           )
      ){ /*Else If: this is min percent score*/
         minPercScoreFL = atof(argAryStr[siArg + 1]);

         if(! minPercScoreFL)
            return 256 | siArg;
      } /*Else If: this is min percent score*/

      else if(!cStrEql("-h",argAryStr[siArg],'\0'))
         return 1024;

      else if(!cStrEql("--h",argAryStr[siArg],'\0'))
         return 1024;

      else if(!cStrEql("help",argAryStr[siArg],'\0'))
         return 1024;

      else if(!cStrEql("-help",argAryStr[siArg],'\0'))
         return 1024;

      else if(!cStrEql("--help",argAryStr[siArg],'\0'))
         return 1024;

      else if(!cStrEql("-v",argAryStr[siArg],'\0'))
         return 2048;

      else if(!cStrEql("--v",argAryStr[siArg],'\0'))
         return 2048;

      else if(!cStrEql("version",argAryStr[siArg],'\0'))
         return 2048;

      else if(!cStrEql("-version",argAryStr[siArg],'\0'))
         return 2048;

      else if(!cStrEql("--version",argAryStr[siArg],'\0'))
         return 2048;

      else
         return 512 | siArg;

      siArg += 2;
   } /*Loop: Get the user input*/
} /*getInput_mkMiruTbl*/

/*-------------------------------------------------------\
| Fun-02: pHelp_mkMiruTbl
|   - Prints out the verision number
| Input:
|   - outFILE:
|     o File to print the version number to
| Output:
|   - Prints:
|     o The version number to outFILE
\-------------------------------------------------------*/
void
pVersion_mkMiruTbl(
   FILE *outFILE
){
   fprintf(
      outFILE,
      "%i-%02i-%02i\n",
      def_year_mkMiruTb_version,
      def_month_mkMiruTb_version,
      def_day_mkMiruTb_version
   );
} /*Print out the version number*/

/*-------------------------------------------------------\
| Fun-03: pVersion
|   - Prints out the verision number
| Input:
|   - outFILE:
|     o File to print the version number to
| Output:
|   - Prints:
|     o The version number to outFILE
\-------------------------------------------------------*/
void
pVersion_mkMiruTbl(
   FILE *outFILE
){
   fprintf(
      outFILE,
      "%i-%02i-%02i\n",
      def_year_mkMiruTb_version,
      def_month_mkMiruTb_version,
      def_day_mkMiruTb_version
   );
} /*Print out the version number*/

/*-------------------------------------------------------\
| Main:
|   - Adds the primer coordinates to the MIRU table
| Input:
| Output:
|   - Prints the MIRU table with the primer coordiantes
|     added in
\-------------------------------------------------------*/
#ifdef PLAN9
void
#else
int
#endif
main(
   int numArgsSI,
   char *argAryStr[]
){
   char *refFileStr = 0;  /*Path to reference file*/
   char *primFileStr = 0; /*Path to primers*/
   char *tblFileStr = 0;  /*Path to miru table*/
   float minPercScoreFL = 0.88;

   struct seqStruct *refSTHeap = 0;
   uchar errUC = 0;

   struct seqStruct *seqSTHeapAry = 0;
   int numPrimSI = 0;
   int siPrim = 0;

   long scoreL = 0;
   float percScoreFL = 0;
   ulong startAlnUL = 0;
   ulong endAlnUL = 0;
   ulong discardUL = 0;
   ulong ignoreUL = 0;

   struct alnSet alnSetSTStack;
   char *buffHeapStr = 0;/*Buffer for reading files*/
   char *posBuffStr = 0; /*Position in buffHeapStr*/
   char *pBuffStr = 0;   /*Position to print in buffHeap*/
   int lenBuffSI = 0;
   ulong oldByteCountUL = 0;
   ulong numBytesUL = 0;

   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^   o main sec-02 sub-02:
   ^     - Check reference file exits
   ^   o main sec-02 sub-03:
   ^     - Check primer file exits
   ^   o main sec-02 sub-04:
   ^     - Check MIRU table file exits
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siPrim =
      getInput_mkMiruTbl(
         numArgsSI,
         argAryStr,
         &refFileStr,
         &primFileStr,
         &tblFileStr,
         &minPercScoreFL
      );

   if(siPrim)
   { /*If: I have an error*/
      if(siPrim & 1024)
      { /*If: I need to print the version number*/
         pHelp_mkMiruTbl(stdout);
         exit(0);
      } /*If: I need to print the version number*/

      if(siPrim & 2048)
      { /*If: I need to print the version number*/
         pVersion_mkMiruTbl(stdout);
         exit(0);
      } /*If: I need to print the version number*/

      if(siPrim & 256)
      { /*If: there was an non-numeric entry error*/
         pHelp_mkMiruTbl(stderr);

         siPrim &= (~256);

         fprintf(
            stderr,
            "%s (from %s) is not numeric\n",
            argAryStr[siPrim + 1],
            argAryStr[siPrim]
         ); /*Let the user know the problem*/

         exit(-1);
      } /*If: there was an non-numeric entry error*/

      pHelp_mkMiruTbl(stderr);
      siPrim &= (~512);

      fprintf(
         stderr,
         "%s is not recongonized\n",
         argAryStr[siPrim]
      ); /*Let the user know the problem*/

      exit(-1);
   } /*If: I had an error*/

   initAlnSet(&alnSetSTStack);

   /*****************************************************\
   * Main Sec-02 Sub-02:
   *   - Check reference file exits
   \*****************************************************/

   inFILE = fopen(refFileStr);

   if(! inFILE)
   { /*If: I could not open the primer file*/
      fprintf(
         stderr,
         "Unable to open -ref %s\n",
         refFileStr
      );

      goto errCleanUp_main;
   } /*If: I could not open the primer file*/

   fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec-02 Sub-03:
   *   - Check primer file exits
   \*****************************************************/

   inFILE = fopen(primFileStr);

   if(! inFILE)
   { /*If: I could not open the primer file*/
      fprintf(
         stderr,
         "Unable to open -prim %s\n",
         primFileStr
      );

      goto errCleanUp_main;
   } /*If: I could not open the primer file*/

   fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec-02 Sub-04:
   *   - Check MIRU table file exits
   \*****************************************************/

   inFILE = fopen(tblFileStr);

   if(! inFILE)
   { /*If: I could not open the primer file*/
      fprintf(
         stderr,
         "Unable to open -miru-tbl %s\n",
         tblFileStr
      );

      goto errCleanUp_main;
   } /*If: I could not open the primer file*/

   fclose(inFILE);
   inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^   - Read in the sequences
   ^   o main sec-03 sub-01:
   ^     - Read in the reference sequence
   ^   o main sec-03 sub-02:
   ^     - Read in primer sequences
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-03 Sub-01:
   *   - Read in the reference sequence
   \*****************************************************/

   inFILE = fopen(refFileStr, "r");

   errUC = 
     readFaSeq(
        inFILE,
        refSTHeap
     );

   if(errUC > 1)
   { /*If: I had an error*/
      if(errUC & 2)
      { /*If: I had an invalid fasta file*/
         fprintf(
            stderr,
            "-ref %s has an non-fasta entry\n",
            refFileStr
         );
      } /*If: I had an invalid fasta file*/

      if(errUC & 64)
      { /*If: I had an memory error*/
         fprintf(
            stderr,
            "Memory error while reading -ref %s\n",
            refFileStr
         );
      } /*If: I had an memory error*/

      goto errCleanUp_main;
   } /*If: I had an error*/


   fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec-03 Sub-02:
   *   - Read in primer sequences
   *   o main sec-03 sub-02 cat-01:
   *     - Find the number of primers in the file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec-03 Sub-02 Cat-01:
   +   - Find the number of primers in the file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   inFILE = fopen(primFileStr, "r");

   seqSTHeapAry = malloc(1 * sizeof(seqStruct));
   
   errUC = 
   inFILE = fopen(primFileStr, "r");

   seqSTHeapAry = malloc(1 * sizeof(seqStruct));
   
   errUC = 
     readFaSeq(
        inFILE,
        seqSTHeapAry
     );

   while(errUC & 1)
   { /*Loop: Find the number of primers in the file*/
      ++numPrimSI;

      errUC = 
        readFaSeq(
           inFILE,
           seqSTHeapAry
        );
   } /*Loop: Find the number of primers in the file*/

   if(errUC > 1)
   { /*If: I had an error*/
      if(errUC & 2)
      { /*If: I had an invalid fasta file*/
         fprintf(
           stderr,
           "Entry %i in -prim %s is not an fasta entry\n",
           numPrimSI,
           primFileStr
         );
      } /*If: I had an invalid fasta file*/

      if(errUC & 64)
      { /*If: I had an memory error*/
         fprintf(
            stderr,
            "Memory error on entry %i in -prim %s\n",
            numPrimSI,
            primFileStr
         );
      } /*If: I had an memory error*/

      goto errCleanUp_main;
   } /*If: I had an error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec-03 Sub-02 Cat-02:
   +   - Read in the primers
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fseek(inFILE, 0, SEEK_SET);

   freeSeqST(seqSTHeapAry);
   seqSTHeapAry = 0;

   seqSTHeapAry =
      malloc(numPrimSI * sizeof(struct seqStruct));

   if(! seqSTHeapAry)
   { /*If: I had an memory error*/
      fprintf(
         stderr,
         "Memory error\n"
      );
   } /*If: I had an memory error*/

   errUC = 
     readFaSeq(
        inFILE,
        seqSTHeapAry
     );

   for(
      siPrim = 0;
      siPrim < numPrimSI;
      ++siPrim
   ){ /*Loop: Get the primer sequences*/
      errUC = 
        readFaSeq(
           inFILE,
           seqSTHeapAry
        );

      if(errUC > 1)
      { /*If: I had an memory error*/
         fprintf(
            stderr,
            "Memory error on entry %i in -prim %s\n",
            siPrim,
            primFileStr
         );

         goto errCleanUp_main;
      } /*If: I had an memory error*/
   } /*Loop: Get the primer sequences*/

   fclose(inFILE);
   inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^   - Find the primer coordinates
   ^   o main sec-04 sub-01:
   ^     - Start loop and align the foward primer
   ^   o main sec-04 sub-02:
   ^     - Align the reverse primer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-04 Sub-01:
   *   - Start loop and align the foward primer
   \*****************************************************/

   for(
      siPrim = 0;
      siPrim + 1 < numPrimSI;
      siPrim += 2
   ){ /*Loop: Align the primer sequences*/
      scoreL =
         memWater(
            refSTHeap,            /*Reference sequence*/
            seqSTHeapAry[siPrim], /*Primer sequence*/
            &discardUL,      /*Position of ref on primer*/
            &ignoreUL,       /*Position of ref on primer*/
            &startAlnUL,
            &endAlnUL,
            &alnSetSTStack
         ); /*Aling the forward primer*/

      if(scoreL == 0)
      { /*If: I had an memory error*/
         fprintf(
            stderr,
            "Memory error aligning primer %i: -prim %s\n",
            siPrim,
            primFileStr
         );

         goto errCleanUp_main;
      } /*If: I had an memory error*/

      percScoreFL = (float) seqSTHeapAry[siPrim].lenSeqUL;
      percScoreFL *= def_matchScore_mkMiruTbl;
      percScoreFL = ((float) scoreL) / percScoreFL;

      if(percScoreFL < minPercScoreFL)
      { /*If: I could not map this primer*/
         fprintf(
            stderr,
            "Unable to align primer %i, foward: %s\n",
            siPrim,
            seqSTHeapAry[siPrim].idCStr
         );

         fprintf(
            stderr,
            " in -prim %s\n",
            primFileStr
         );

         goto errCleanUp_main;
      } /*If: I could not map this primer*/

      /*Add in the primer coordinates*/
      seqSTHeapAry[siPrim].offsetUL = startAlnUL;
      seqSTHeapAry[siPrim].endUL = endAlnUL;

      /**************************************************\
      * Main Sec-04 Sub-02:
      *   - Align the reverse primer
      \**************************************************/

      reverseComplementSeq(seqSTHeapSry[siPrim + 1]); 

      scoreL =
         memWater(
            refSTHeap,              /*Reference sequence*/
            seqSTHeapAry[siPrim + 1], /*Primer sequence*/
            &discardUL,      /*Position of ref on primer*/
            &ignoreUL,       /*Position of ref on primer*/
            &startAlnUL,
            &endAlnUL,
            &alnSetSTStack
         ); /*Aling the forward primer*/

      if(scoreL == 0)
      { /*If: I had an memory error*/
         fprintf(
            stderr,
            "Memory error aligning primer %i: -prim %s\n",
            siPrim + 1,
            primFileStr
         );

         goto errCleanUp_main;
      } /*If: I had an memory error*/

      percScoreFL = (float) seqSTHeapAry[siPrim].lenSeqUL;
      percScoreFL *= def_matchScore_mkMiruTbl;
      percScoreFL = ((float) scoreL) / percScoreFL;

      if(percScoreFL < minPercScoreFL)
      { /*If: I could not map this primer*/
         fprintf(
            stderr,
            "Unable to align primer %i, reverse: %s\n",
            siPrim + 1,
            seqSTHeapAry[siPrim + 1].idCStr
         );

         fprintf(
            stderr,
            " in -prim %s\n",
            primFileStr
         );

         goto errCleanUp_main;
      } /*If: I could not map this primer*/

      /*Primer coordinates*/
      seqSTHeapAry[siPrim + 1].offsetUL = startAlnUL;
      seqSTHeapAry[siPrim + 1].endUL = endAlnUL;
   } /*Loop: Align the primer sequences*/

   freeSeqST(refSTHeap);
   refSTHeap = 0;

   freeAlnSetStack(&alnSetSTStack);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^   - Read in table and add the primer coordinates
   ^   o main sec-05 sub-01:
   ^     - Set up the buffer memory & open the miru table
   ^   o main sec-05 sub-02:
   ^     - Read in at least the first line of MIRU table
   ^   o main sec-05 sub-03:
   ^     - Add the primer coordinates to the primer names
   ^   o main sec-05 sub-04:
   ^     - Print out the rest of the file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-05 Sub-01:
   *   - Set up the buffer memory and open the miru table
   \*****************************************************/

   lenBuffSI = 1 << 10; /*~ 2000 characters*/

   buffHeapStr = malloc((lenBuffSI + 1) * sizeof(char));

   if(! buffHeapStr)
   { /*If: I had an memory error*/
      fprintf(
         stderr,
         "Memory error after primer step\n"
       );

      goto errCleanUp_main;
   } /*If: I had an memory error*/

   inFILE = fopen(tblFileStr, "r");

   /*****************************************************\
   * Main Sec-05 Sub-02:
   *   - Read in at least the first line of MIRU table
   \*****************************************************/

   numBytesUL =
      fread(
         buffHeapStr,
         sizeof(char),
         lenBuffSI,
         inFILE
      ); /*Read in the frist line*/

   if(! numBytesUL)
   { /*If: File does not have an new line*/
      fprintf(
         stderr,
         "-miru-tbl %s\n has no lines\n",
         tblFileStr
      );

      goto errCleanUp_main;
   } /*If: File does not have an new line*/

   buffHeapStr[lenBuffSI] = '\0';
   posBuffStr = buffHeapStr;
   posBuffStr += ulEndStrLine(buffHeapStr);

   while(*posBuffStr != '\n')
   { /*Loop: Read in the first line*/
      lenBuffSI <<= 1;

      posBuffStr =
         realloc(
            buffHeapStr,
            (lenBuffSI + 1) * sizeof(char)
         ); /*Resize the buffer*/

      if(! posBuffStr)
      { /*If: I had an memory error*/
         fprintf(
            stderr,
            "Memory error reading -miru-tbl %s\n",
            tblFileStr
         );

         goto errCleanUp_main;
      } /*If: I had an memory error*/

      buffHeapStr = posBuffStr;
      posBuffStr += numBytesUL;

      oldByteCountUL = numBytesUL;

      numBytesUL +=
         fread(
            posBuffStr,
            sizeof(char),
            (lenBuffSI >> 1),
            inFILE
         ); /*Read in the frist line*/

      buffHeapStr[numBytesUL] = '\0';

      if(oldByteCountUL == numBytesUL)
      { /*If: File does not have an new line*/
         fprintf(
            stderr,
            "-miru-tbl %s\n has no newline\n",
            tblFileStr
         );

         goto errCleanUp_main;
      } /*If: File does not have an new line*/

      posBuffStr += ulEndStrLine(posBuffStr);
   } /*Loop: Read in the first line*/

   /*****************************************************\
   * Main Sec-05 Sub-03:
   *   - Add the primer coordinates to the primer names
   *   o main sec-05 sub-02 Cat-01:
   *     - Get past and print out the first column
   *   o main sec-05 sub-02 Cat-02:
   *     - Print out the header for each primer
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec-05 Sub-02 Cat-01:
   +   - Get past and print out the first column
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   pBuffStr = buffHeapStr;
   posBuffStr = buffHeapStr;

   while(*posBuffStr++ > 32) ;

   --posBuffStr;

   if(*posBuffStr == '\t') ;
   else if(*posBuffStr == ' ') ;
   else
   { /*Else: File has only 1 column*/
      fprintf(
         stderr,
         "-miru-tbl %s\n has only one column\n",
         tblFileStr
      );

      goto errCleanUp_main;
   } /*Else: File does not have an new line*/

   *(posBuffStr - 1) = '\0';

   fprintf(
      stdout,
      "%s\t",
      pBuffStr
   ); /*Print out the frist columns header*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec-05 Sub-02 Cat-02:
   +   - Print out the header for each primer
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   /*I am going to assume the user input the primers in
   `   order. It is a bit dumb, but it works
   */
   siPrim = 0;

   while(*posBuffStr != '\n')
   { /*Loop: Print out the header*/
      pBuffStr = posBuffStr;

      while(*posBuffStr++ > 32) ;

      if(*(posBuffStr - 1) == '\n')
         break;

      *(posBuffStr - 1) = '\0';

      fprintf(      
         stdout,
         "%s.%lu.%lu\t",
         seqSTHeapAry[siPrim].offsetUL,
         seqSTHeapAry[siPrim + 1].offsetUL
      );
   } /*Loop: Print out the header*/

   *(posBuffStr - 1) = '\0';

   fprintf(      
      stdout,
      "%s.%lu.%lu\n%s",
      seqSTHeapAry[siPrim].offsetUL,
      seqSTHeapAry[siPrim + 1].offsetUL,
      posBuffStr
   );

   /*****************************************************\
   * Main Sec-05 Sub-04:
   *   - Print out the rest of the file
   \*****************************************************/

   while(
      fread(
         buffHeapStr,
         sizeof(char),
         lenBuffSI,
         inFILE
      ) /*Read in the rest of the file*/
   ) fprintf(stdout, "%s", posBuffStr);

   free(buffHeapStr);
   buffHeapStr = 0;

   fclose(inFILE);
   inFILE = 0;

   freeSeqAryST(
      seqSTHeapAry,
      numPrimSI
   );

   seqSTHeapAry = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-0x:
   ^   - Clean up
   ^   o main sec-0x sub-01:
   ^     - Clean up after success
   ^   o main sec-0x sub-02:
   ^     - Clean up after an error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-0x Sub-01:
   *   - Clean up after success
   \*****************************************************/

   exit(0);

   /*****************************************************\
   * Main Sec-0x Sub-02:
   *   - Clean up after an error
   \*****************************************************/

   errCleanUp_main:;

   free(buffHeapStr);
   buffHeapStr = 0;

   freeSeqST(refSTHeap);
   refSTHeap = 0;

   freeSeqAryST(
      seqSTHeapAry,
      numPrimSI
   );

   seqSTHeapAry = 0;

   freeAlnSetStack(&alnSetSTStack);

   if(inFILE)
      fclose(inFILE);

   inFILE = 0;

   exit(-1);
} /*main*/
