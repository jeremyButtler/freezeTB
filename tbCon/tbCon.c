/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Indcluded libraries
'   o main:
'     - Driver function to build the consensus with
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Indcluded libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../generalLib/dataTypeShortHand.h" /*no .c*/
#include "../generalLib/ulCpStr.h"           /*no .c*/
#include "../generalLib/base10StrToNum.h"    /*no .c*/
#include "../generalLib/samEntryStruct.h"

#include "tbCon-fun.h"
#include "tbCon-input.h"

/*Hidden libraries
`   - ../generalLib/numToStr.h       (no .c)
`   - ../generalLib/genMath.h        (no .c)
*/

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

   uint lenRefUI = defRefLen;  /*Length of the reference*/
   char refIdStr[256]; /*Name of reference*/
      /*I would be supprised if a reference id was longer
      `   then 64 characters, so 256 should be safe
      */

   struct tbConSet settings;

   /*Iterators for loops*/
   int iIter = 0;
   short errS = 0;
   char shiftC = 0;
   char helpBl = 0;

   /*Files for input*/
   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*Non-user input*/
   /*For the sam file*/
   char *buffStr = 0;
   ulong lenBuffUL = 0;
   struct samEntry samST;
 
   /*Grabbing reference id from sam file*/
   char *tmpStr = 0;
   ulong tabUL = ulCpMakeDelim('\t');
   char multiRefBl = 0; /*1: means multiple refs/exit*/
   
   /*For building the consensus*/
   struct conBase *conBaseAryST;

   /*For collapsing the consensus*/
   char errC = 0;
   int numFragmentsI = 0;
   struct samEntry *samSTAry = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^   - Get user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Default reference id*/
   refIdStr[0] = 'N';
   refIdStr[2] = 'A';
   refIdStr[3] = '\0';

   initTbConSet(&settings);
   iIter = 1;

   while(iIter < lenArgsI)
   { /*Loop: through all user input*/
      errS =
         checkTbConArg(
            argsAryStr[iIter],
            argsAryStr[iIter + 1],
            &samFileStr,
            &outFileStr,
            &tsvFileStr,
            &lenRefUI,
            &settings,
            &shiftC 
         );

      if(errS)
      { /*If: there was an error*/
         if(errS & def_tbCon_input_pHelp)
         { /*If: I printing the help message*/
            pTbConHelp(
               errS & def_tbCon_input_pFullHelp,
               stdout
            );

            exit(0);
         } /*If: I printing the help message*/

         else if(errS & def_tbCon_input_pVersion)
         { /*If: I printing the help message*/
            pTbConVersion(stdout);
            exit(0);
         } /*If: I printing the help message*/

         else if(errS & def_tbCon_input_nonNum_Err)
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

         else if(errS & def_tbCon_input_unkown_Err)
         { /*Else if: invalid input*/
            if(helpBl)
            { /*If: this is the first error*/
               pTbConHelp(errS, stderr);
               helpBl = 0;
            } /*If: this is the first error*/

            fprintf(
               stderr, 
               "%s is not recongnized\n",
               argsAryStr[iIter]
            );
         } /*Else if: invalid input*/
      } /*If: there was an error*/

      tbCon_checkInput_falseErr:;

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
       { /*If: this is an header entry*/

          /*This is not perfect because it assumes the
          `  sam file @SQ entry is in the order of
          `  @SQ\tSN:reference-name\tLN:reference-length
          */

          if(! cStrEql("@SQ\t", samST.extraStr, '\t'))
          { /*If: this has length information*/
             if(multiRefBl)
             { /*If: sam file has multiple references*/
                fprintf(
                   stderr,
                   "-sam %s has multiple references\n",
                   samFileStr
                ); /*Let user know the problem*/

                if(samFILE != stdin) fclose(samFILE);
                if(outFILE != stdout) fclose(samFILE);

                freeSamEntryStack(&samST);
                free(buffStr);
 
                /*This is very unlikely*/
                if(conBaseAryST)
                   freeConBaseAry(&conBaseAryST,lenRefUI);

                exit(-1);
             } /*If: sam file has multiple references*/

             multiRefBl = 1;

             /*Get past "@SQ\t"*/
             tmpStr = samST.extraStr + 4; 

             while(*tmpStr++ !=':') if(*tmpStr <31) break;

             /*Check if I have a reference id*/
             if(*(tmpStr - 1) != ':')
                goto tbCon_main_pSamHead;

             tmpStr +=
                ulCpStrDelim(refIdStr,tmpStr,tabUL,'\t');

             ++tmpStr;
            
             if(*tmpStr < 31 ) goto tbCon_main_pSamHead;

             /*Move past the LN: flag*/
             while(*tmpStr++ !=':') if(*tmpStr <31) break;

             /*Check if I have a read length*/
             if(*(tmpStr - 1) != ':')
                goto tbCon_main_pSamHead;

             /*Get the reference length*/
             tmpStr = base10StrToUI(tmpStr, lenRefUI);

             /*Check if I had a conversion error*/
             if(*tmpStr > 31) lenRefUI = defRefLen;
          } /*If: this has length information*/

          tbCon_main_pSamHead:;

          pSamEntry(&samST,&buffStr,&lenBuffUL,outFILE);
          goto nextLine;
       } /*If: this is an header entry*/

       /*Check if the read is an umapped read (4), an
       `   secondary alignemnt (256), or an supplementary
       `   alignment (2048)
       */
       if(samST.flagUS & (4 | 256 | 2048)) goto nextLine;

       if(samST.mapqUC < settings.minMapqUC)
          goto nextLine;

       errS =
          addReadToConBaseArray(
             &samST,
             &conBaseAryST,
             &lenRefUI, 
             &settings
          ); /*Build the consensus; read by read*/

       if(errS & def_tbCon_Memory_Err)
       { /*If: I had a memory error*/
          free(buffStr);
          fclose(samFILE);
          fclose(outFILE);
          freeSamEntryStack(&samST);
          freeConBaseAry(&conBaseAryST, lenRefUI);

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

   freeSamEntryStack(&samST);
   fclose(samFILE);

   fprintf(
      outFILE,
      "@tbCom -sam %s\n",
      samFileStr
   ); /*Add all settings in*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^   - Print and collapse the consensus
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(tsvFileStr)
   { /*If: the user wanted the variants*/
      errC =
         pConBaseArray(
            conBaseAryST,
            lenRefUI,
            refIdStr,
            &settings,
            tsvFileStr
         ); /*Print out the variants (not a vcf)*/

      if(errC)
      { /*If: I had a file error*/
         fprintf(
            stderr,
            "Could not open -out-tsv %s; skipping diff",
            tsvFileStr
         );

         fprintf(
            stderr,
            " table print\n"
         );
      } /*If: I had a file error*/
   } /*If: the user wanted the variants*/

   /*Check if the ouput and tsv file are the same*/
   if(outFILE == stdout)
   { /*If: the ouput file was stdout*/
     if((tsvFileStr && *tsvFileStr == '-'))
        fprintf(outFILE, "\n");
   } /*If: the ouput file was stdout*/

   else if(! cStrEql(outFileStr, tsvFileStr, '\0'))
      fprintf(outFILE, "\n");

   /*Print the consensuses*/
   samSTAry =
      (struct samEntry *)
      collapseConBaseArray(
         conBaseAryST,
         lenRefUI,
         &numFragmentsI,
         refIdStr,
         &settings,
         &errC
      ); /*Collapse the consensus*/

   freeConBaseAry(&conBaseAryST, lenRefUI);

   /*Print the consensuses*/
   for(iIter = 0; iIter < numFragmentsI; ++iIter)
   { /*Loop: Print and free each sam entry*/
      /*Check if ths was a false entry
      `   For some odd reason my counting system is
      `   off here (problem is in collapseConBaseArray)
      */
      if(samSTAry[iIter].qryIdStr[0] != '\0')
          pSamEntry(
             &samSTAry[iIter],
             &buffStr,
             &lenBuffUL,
             outFILE
          ); /*Print out the consensuses as a sam file*/

      freeSamEntryStack(&samSTAry[iIter]);
   } /*Loop: Print and free each sam entry*/

   free(buffStr);
   buffStr = 0;
   lenBuffUL = 0;

   free(samSTAry);
   fclose(outFILE);

    exit(0);
} /*main*/
