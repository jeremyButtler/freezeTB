/*########################################################
# Name: tbCon
#  - Builds a consensus using the non-subsampling quick
#    tbCon method
# Libraries
#   - "tbCon-inputFuns.h"                 (No .c file)
#   o "tbCon.h"                           (No .c file)
#   o "../generaLib/samEntryStruct.h"     (No .c file)
#   o "../generaLib/base10StrToNum.h"     (No .c file)
#   o "../generaLib/dataTypeShortHand.h"  (No .c file)
#   o "../generaLib/ulCpStr.h"            (No .c file)
#   o "../generaLib/numToStr.h"           (No .c file)
# C Standard Libraries:
#  o <stdlib.h>
#  o <stdio.h>
#  o <limits.h>
#  o <string.h>
########################################################*/

#include "tbCon-inputFuns.h"

int main(
   int lenArgsI,
   char *argsAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main:
   '   - Builds a consensus from the input sam file
   '   o main sec-01:
   '     - variable declerations
   '   o main sec-02:
   '     - Get and check user input
   '   o main sec-03:
   '     - Build consensus
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Files to output to*/
   char *samFileStr = 0;
   char *outFileStr = 0;
   char *tsvFileStr = 0; /*File to print results to*/

   char *refIdStr = defRefNameStr; /*Name of reference*/
   uint lenRefUI = defRefLen;  /*Length of the reference*/

   struct tbConSet settings;

   /*Iterators for loops*/
   int iIter = 0;
   short errS = 0;
   char shiftC = 0;
   char helpBl = 0;

   /*Files for input*/
   FILE *samFILE = 0;
   FILE *outFILE = 0;
   FILE *tsvFILE = 0;

   /*Non-user input*/
   /*For the sam file*/
   char *buffStr = 0;
   ulong lenBuffUL = 0;
   struct samEntry samST;
   
   /*For building the consensus*/
   struct conBase *conBaseAryST;

   /*For collapsing the consensus*/
   char errC = 0;
   int numFragmentsI = 0;
   struct samEntry *samAryST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^   - Get user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   initTbConSet(&settings);
   iIter = 0;

   while(iIter < lenArgsI)
   { /*Loop: through all user input*/
      errS =
         checkTbConAry(
            argsAryStr[iIter],
            argsAryStr[iIter + 1],
            &samFileStr,
            &outFileStr,
            &tsvFileStr,
            &refIdStr,
            &lenRefUI,
            &settings,
            &shiftC 
         );

      if(errS & def_tbCon_pHelp)
      { /*If: I printing the help message*/
         pTbConHelp(errS & def_tbCon_pFullHelp, stdout);
         exit(0);
      } /*If: I printing the help message*/

      else if(errS & def_tbCon_pVersion)
      { /*If: I printing the help message*/
         pTbConVersion(stdout);
         exit(0);
      } /*If: I printing the help message*/

      else if(errS & def_tbCon_nonNum_Err)
      { /*If: I had non-numeric input*/
         if(helpBl)
         { /*If: this is the first error*/
            pTbConHelp(errS, stderr);
            helpBl = 0;
         } /*If: this is the first error*/

         fprintf(
            stderr,
            "%s %s is non-numeric\n",
             argsAryStr[iIter],
             argsAryStr[iIter + 1]
         );
      } /*If: I had non-numeric input*/

      else if(errS & def_tbCon_unkown_Err)
      { /*Else if: invalid input*/
         if(helpBl)
         { /*If: this is the first error*/
            pTbConHelp(errS, stderr);
            helpBl = 0;
         } /*If: this is the first error*/

         fprintf(stderr, "%s is not recongnized\n");
      } /*Else if: invalid input*/

      iIter += 1 + shiftC;
   } /*Loop: through all user input*/

   if(errS) exit(-1); /*Error with input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^   - Open files
   ^   o main sec-03 sub-01:
   ^     - Check if I can open the sam file
   ^   o main sec-03 sub-02:
   ^     - Check if I can open the output file
   ^   o main sec-03 sub-03:
   ^     - Check if I can open the output tsv file for the
   ^       variant reporting
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Main Sec-03 Sub-01:
    *   - Check if I can open the sam file
    \****************************************************/

    if(samFileStr == 0 || samFileStr[0] == '-')
       samFILE = stdin;

    else
    { /*Else: The user provided a sam file*/
       samFILE = fopen(samFileStr, "r");

       if(!samFILE)
       { /*If: I could not open the sam file*/
          fprintf(
              stderr,
              "Could not open -sam %s\n",
              samFileStr
          );

          exit(-1);
       } /*If: I could not open the sam file*/
    } /*Else: The user provided a sam file*/

    /****************************************************\
    * Main Sec-03 Sub-02:
    *   - Check if I can open the output file
    \****************************************************/

    if(outFileStr == 0 || outFileStr[0] == '-')
       outFILE = stdout;

    else
    { /*Else: The user provided a out file*/
       outFILE = fopen(outFileStr, "r");

       if(!outFILE)
       { /*If: I could not open the out file*/
          fprintf(
              stderr,
              "Could not open -out %s\n",
              outFileStr
          );

          fclose(samFILE);
          exit(-1);
       } /*If: I could not open the out file*/
    } /*Else: The user provided a out file*/

    /****************************************************\
    * Main Sec-03 Sub-02:
    *   - Check if I can open the output tsv file for the
    *     variant reporting
    \****************************************************/

    if(tsvFileStr[0] == '-')
       tsvFILE = stdout;

    else
    { /*Else: The user provided a tsv file*/
       tsvFILE = fopen(tsvFileStr, "r");

       if(!tsvFILE)
       { /*If: I could not open the tsv file*/
          fprintf(
              stderr,
              "Could not open -tsv %s\n",
              tsvFileStr
          );

          fclose(samFILE);
          fclose(outFILE);
          exit(-1);
       } /*If: I could not open the tsv file*/
    } /*Else: The user provided a tsv file*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^   - Build the consensus
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   initSamEntry(&samST);
   errS =
      readSamLine(&samST, &buffStr, &lenBuffUL, samFILE);

   while(!errS)
   { /*Loop: Read in the sam file/build consensus*/
       /*Check if this line is part of the header*/
       if(samST.extraStr[0] == '@') 
          pSamEntry(&samST, buffStr, lenBuffUL, outFILE);

       /*Check if the read is an umapped read (4), or is
       ` a secondary alignemnt (256)
       */
       if(samST.mapqUC & (4 | 256)) goto nextLine;

       errS =
          addReadToConBaseArray(
             &samST,
             &conBaseAryST,
             &lenRefUI, 
             &settings
          ); /*Build the consensus; read by read*/

      if(errS & def_tbCon_Memory_Err)
      { /*If: I had a memory error*/
         fclose(samFILE);
         fclose(outFILE);
         fclose(tsvFILE);
         freeSamEntryStack(&samST);
         freeConBaseAry(conBaseAryST, lenRefUI);

         fprintf(
            stderr,
            "Memory error while building consensus\n"
         );

         exit(-1);
      } /*If: I had a memory error*/

      nextLine:;

      errS =
         readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);
   } /*Loop: Read in the sam file/build consensus*/
      nextLine:;

      errS =
         readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);
   } /*Loop: Read in the sam file/build consensus*/

   fclose(samFILE);
   free(buffStr);
   buffStr = 0;
   lenBuffUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^   - Print and collapse the consensus
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(tsvFILE)
   { /*If: the user wanted the variants*/
      pConBaseArray(
         conBaseAryST,
         lenRefUI,
         refIdStr,
         settings,
         tsvFILE
      ); /*Print out the variants (not a vcf)*/
   } /*If: the user wanted the variants*/

   if(*tsvFILE == *outFILE) fprintf(tsvFILE, "\n");
   if(tsvFILE){fclose(tsvFILE); tsvFILE = 0;}

   samAryST =
      collapseCOnBaseArray(
         conBaseAryST,
         lenRefUI,
         &numFragmentsI,
         refIdStr,
         settings,
         errC
      ); /*Collapse the consensus*/

   freeConBaseAry(conBaseAryST, lenRefUI);

   /*Print the consensus*/
   for(iIter = 0; iIter < numFragmentsI; ++iIter)
   { /*Loop: Print and free each sam entry*/
      pSamEntryAsFasta(&samST, outFILE);
      freeSamEntryStack(&samST[iIter]);
   } /*Loop: Print and free each sam entry*/

   free(samST);
   fclose(outFILE);

    exit(0);
} /*main*/
