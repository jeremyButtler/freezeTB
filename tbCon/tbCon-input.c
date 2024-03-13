/*########################################################
# Name: tbCon-inputFus.h
#  - Holds functions for getting input, printing version
#    numbers, and printing help messages
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
'   o header:
'     - Included libraries
'   o fun-01: checkTbConArg
'     - Checks user input to see if was a valid argument,
'       if so, this then sets the argument
'   o fun-02: pTbConHelp
'     - Prints out the tbCon help message
'   o fun-03: pTbConVersion
'     - Prints out the version number for tbCon
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries
\-------------------------------------------------------*/

#include "tbCon-input.h"

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/base10StrToNum.h"
#include "../generalLib/ulCpStr.h"
#include "tbCon-fun.h"

/*-------------------------------------------------------\
| Fun-01: checkTbConArg
|   - Checks user input to see if was a valid argument,
|     if so, this then sets the argument
| Input:
|   - parmStr:
|     o c-string with the paramter user input (flag)
|   - argStr
|     o c-string with the argument user input
|   - samFileStr:
|     o Pointer to c-string to hold the path to the sam
|       file
|   - outFileStr:
|     o Pointer to the file to output the sam entries for
|       the consensus to
|   - tsvFileStr:
|     o Pointer to the tsv file to output the variants
|       to
|   - refLenUI:
|     o Pointer to unsigned int to hold the reference
|       length
|   - tbConhSetSTPtr:
|     o Pointer to tbConSet structure with other settings
|   - shiftC:
|     o Pointer to character to hold 1, argument read,
|       or 0; parmater was a boolean
| Output:
|    - Modifies:
|      o All variables input to their specified values
|    - Returns:
|      o 0 for no errors
|      o def_tbCon_input_pHelp for the help message
|      o   def_tbCon_input_pFullHelp
|        | def_tbCon_input_pHelp
|        for the full help message
|      o def_tbCon_input_pVersion for the version number
|      o def_tbCon_input_nonNum_Err for non-numeric number
|      o def_tbCon_input_unkown_Err for an unkown entry
\-------------------------------------------------------*/
short
checkTbConArg(
   char *parmStr,/*Parameter for argument (flag)*/
   char *argStr, /*Argument (user input)*/
   char **samFileStr, /*Input sam file to use*/
   char **outFileStr, /*File to output to*/
   char **tsvFileStr, /*Path to tsv file to pring*/
   unsigned int *refLenUI,    /*Length of reference*/
   void *tbConSetSTPtr,
   char *shiftC        /*How much to shift argument by*/
){ /*checkTbConArg*/
   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-01 TOC: checkTbConArg
   '   - Check a single user input argument
   '   o fun-01 sec-01:
   '      - Variable declerations
   '   o fun-01 sec-02:
   '      - Output and input files
   '   o fun-01 sec-03:
   '      - Reference data
   '   o fun-01 sec-04:
   '      - Min quality scores, length, and depth
   '        (non-print)
   '   o fun-01 sec-05:
   '      - Min percent support for snp/match, ins, & del
   '   o fun-01 sec-06:
   '      - Masking
   '   o fun-01 sec-07:
   '      - Printing mininum settings
   '   o fun-01 sec-08:
   '      - Unkown input
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char *macTmpStr = 0;
   short macRetS = 0;

   struct tbConSet *settings =
     (struct tbConSet *) tbConSetSTPtr;

   *shiftC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-02:
   ^   - Output and input files
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! cStrEql("-sam", parmStr, '\0'))
   { /*If: The user input a sam file*/
      *samFileStr = argStr;
      *shiftC = 1;
   } /*If: The user input a sam file*/

   else if(! cStrEql("-", parmStr, '\0'))
      *samFileStr = parmStr;

   else if(! cStrEql("-out", parmStr, '\0'))
   { /*Else If: The user provided an output file*/
      *outFileStr = argStr;
      *shiftC = 1;
   } /*Else If: The user provided an output file*/

   else if(! cStrEql("-out-tsv", parmStr, '\0'))
   { /*Else If: The user provided an output file*/
      *tsvFileStr = argStr;
      *shiftC = 1;
   } /*Else If: The user provided an output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-03:
   ^   - Reference data
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(! cStrEql("-ref-len", parmStr, '\0'))
   { /*If: The user provided the reference length*/
      macTmpStr = base10StrToUI(argStr, *refLenUI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided reference length*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-04:
   ^   - Min quality scores, length, and depth (non-print)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(! cStrEql("-min-q", parmStr, '\0'))
   { /*If: The user provided the minimum Q-score*/
      macTmpStr = base10StrToUI(argStr, settings->minQI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum Q-score*/


   else if(! cStrEql("-min-q-ins", parmStr, '\0'))
   { /*If: user provided the minimum inserion Q-score*/
      macTmpStr= base10StrToUI(argStr,settings->minInsQI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: user provided the minimum inserion Q-score*/

   else if(! cStrEql("-min-mapq", parmStr, '\0'))
   { /*If: user provided the minimum mapping quality*/
      macTmpStr=base10StrToUC(argStr,settings->minMapqUC);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: user provided the minimum mapping quality*/

   else if(! cStrEql("-min-len", parmStr, '\0'))
   { /*If: The user provided the minimum fragment length*/
      macTmpStr= base10StrToUI(argStr,settings->minLenI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum fragment length*/

   else if(! cStrEql("-min-depth", parmStr, '\0'))
   { /*If: The user provided the minimum depth*/
      macTmpStr=base10StrToUI(argStr,settings->minDepthI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum depth*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-05:
   ^   - Min percent support for snp/match, ins, & del
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*atof reurns 0 for failure*/
   else if(! cStrEql("-perc-snp-sup", parmStr, '\0'))
   { /*If: The user provided the minimum % snp support*/
      settings->minPercSnpF = atof(argStr);

      if(settings->minPercSnpF == 0)
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % snp support*/

   else if(! cStrEql("-perc-ins-sup", parmStr, '\0'))
   { /*If: The user provided the minimum % ins support*/
      settings->minPercInsF = atof(argStr);

      if(settings->minPercInsF == 0)
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % ins support*/

   else if(! cStrEql("-perc-del-sup", parmStr, '\0'))
   { /*If: The user provided the minimum % del support*/
      settings->minPercDelF = atof(argStr);

      if(settings->minPercDelF == 0)
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % del support*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-06:
   ^   - Masking
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(! cStrEql("-mask", parmStr, '\0'))
   { /*If: The user provided the base to mask with*/
      settings->maskC = *argStr;
      *shiftC = 1;
   } /*If: The user provided the base to mask with*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-07:
   ^   - Printing mininum settings
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Settings for printing out variations*/
   else if(! cStrEql("-p-min-depth", parmStr, '\0'))
   { /*If: The user provided the minimum depth*/
      macTmpStr =
        base10StrToUI(argStr,settings->printMinDepthI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum depth*/

   /*The print percentage depths*/
   else if(! cStrEql("-p-perc-snp-sup", parmStr, '\0'))
   { /*If: The user provided the minimum % snp support*/
      settings->printMinSupSnpF = atof(argStr);

      if(settings->printMinSupSnpF == 0)
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % snp support*/

   else if(! cStrEql("-p-perc-ins-sup", parmStr, '\0'))
   { /*If: The user provided the minimum % ins support*/
      settings->printMinSupInsF = atof(argStr);

      if(settings->printMinSupInsF == 0)
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % ins support*/

   else if(! cStrEql("-p-perc-del-sup", parmStr, '\0'))
   { /*If: The user provided the minimum % snp support*/
      settings->printMinSupDelF = atof(argStr);

      if(settings->printMinSupDelF == 0)
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % snp support*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-08:
   ^   - Unkown input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(
         ! cStrEql("-h", parmStr, '\0')
      || ! cStrEql("--h", parmStr, '\0')
      || ! cStrEql("-help", parmStr, '\0')
      || ! cStrEql("--help", parmStr, '\0')
      || ! cStrEql("help", parmStr, '\0')
   ) macRetS = def_tbCon_input_pHelp;

   else if(
         ! cStrEql("-h-all", parmStr, '\0')
      || ! cStrEql("--h-all", parmStr, '\0')
      || ! cStrEql("-help-all", parmStr, '\0')
      || ! cStrEql("--help-all", parmStr, '\0')
      || ! cStrEql("help-all", parmStr, '\0')
   ) macRetS =
        def_tbCon_input_pFullHelp | def_tbCon_input_pHelp;

   else if(
         ! cStrEql("-v", parmStr, '\0')
      || ! cStrEql("--v", parmStr, '\0')
      || ! cStrEql("-version", parmStr, '\0')
      || ! cStrEql("--version", parmStr, '\0')
      || ! cStrEql("version", parmStr, '\0')
   ) macRetS = def_tbCon_input_pVersion;

   else macRetS = def_tbCon_input_unkown_Err;

   return macRetS;
} /*checkTbConArg*/

/*-------------------------------------------------------\
| Fun-02: pTbConHelp
|   - Prints out the tbCon help message
| Input:
|   - pFullC:
|     o 1: Print out the full help message (TODO: add in)
|     o 0: Print out the short help message
|   - outVoidFILE:
|     o FILE pointer (FILE *outFILE) to file to print to
| Output:
|   - Prints:
|     o tbCon help message to outFILE
\-------------------------------------------------------*/
void
pTbConHelp(
   char pFullC,
   void *outVoidFILE
){
   /*This is for my sanity*/
   FILE *outFILE = (FILE *) outVoidFILE;

   fprintf(outFILE, "Usage: tbCon -sam file.sam\n");

   fprintf(
      outFILE,
      "  - Makes an consensus genome from input sam file"
    );

    fprintf(outFILE, "\n");
    fprintf(outFILE, "Input:\n");

   /*TODO: Make full help message*/

   fprintf(outFILE, "Output:\n");

   fprintf(
      outFILE,
      "  - Prints the consensus to -out as a sam file\n"
    );

    fprintf(
       outFILE,
      "  - Prints the variants to the tsv file (-tsv)\n"
    );
} /*pTbConHelp*/

/*-------------------------------------------------------\
| Fun-03: pTbConVersion
|   - Prints out the version number for tbCon
| Input:
|   - outFILE:
|     - FILE pointer (FILE *outFILE) to file to print to
| Output:
|   - Prints:
|     o version number of tbCon to outFILE
\-------------------------------------------------------*/
void
pTbConVersion(
   void *outFILE
){ /*pTbConVersion*/
   fprintf(
       (FILE *) outFILE,
       "tbCon Version: %i-%i-%i\n",
       def_tbCon_input_year,
       def_tbCon_input_month,
       def_tbCon_input_day
   ); /*Version of tbCon*/
} /*pTbConVersion*/
