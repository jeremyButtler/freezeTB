/*########################################################
# Name: tbCon-inputFus.h
#  - Holds functions for getting input, printing version
#    numbers, and printing help messages
# Libraries
#   - "tmpCon.h"                          (No .c file)
#   o "../generaLib/samEntryStruct.h"     (No .c file)
#   o "../generaLib/base10StrToNum.h"     (No .c file)
#   o "../generaLib/dataTypeShortHand.h"  (No .c file)
#   o "../generaLib/ulCpStr.h"            (No .c file)
#   o "../generaLib/numToStr.h"           (No .c file)
# C Standard Libraries:
#  - <string.h>
#  o <stdlib.h>
#  o <stdio.h>
#  o <limits.h>
########################################################*/

#ifndef TB_CONSENSUS_INPUT_FUNCITONS_H
#define TB_CONSENSUS_INPUT_FUNCITONS_H

#include "tbCon.h"
#include <string.h>

#define def_tbCon_year 2024
#define def_tbCon_month 02
#define def_tbCon_day 20

/*Error message flags for user input*/
#define def_tbCon_nonNum_Err 1
#define def_tbCon_unkown_Err 2
#define def_tbCon_pHelp 4
#define def_tbCon_pFullHelp 8 
#define def_tbCon_pVersion 16

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
|   - refIdStr:
|     o c-string to hold the reference name
|   - refLenUI:
|     o Pointer to unsigned int to hold the reference
|       length
|   - settings:
|     o Pointer to tbConSet structure with other settings
|   - shiftC:
|     o Pointer to character to hold 1, argument read,
|       or 0; parmater was a boolean
| Output:
|    - Modifies:
|      o All variables input to their specified values
|    - Returns:
|      o 0 for no errors
|      o def_tbCon_pHelp for the help message
|      o def_tbCon_pFullHelp | def_tbCon_pHelp for the
|        full help message
|      o def_tbCon_pVersion for the version number
|      o def_tbCon_nonNum_Err for a non-numeric number
|      o def_tbCon_unkown_Err for an unkown entry
\-------------------------------------------------------*/
static short checkTbConArg(
   char *parmStr,/*Parameter for argument (flag)*/
   char *argStr, /*Argument (user input)*/
   char **samFileStr, /*Input sam file to use*/
   char **outFileStr, /*File to output to*/
   char **tsvFileStr, /*Path to tsv file to pring*/
   char **refIdStr,   /*Name of reference (to print out)*/
   uint *refLenUI,    /*Length of reference*/
   struct tbConSet *settings,
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

   *shiftC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-02:
   ^   - Output and input files
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(!strcmp("-sam", parmStr))
   { /*If: The user input a sam file*/
      *samFileStr = argStr;
      *shiftC = 1;
   } /*If: The user input a sam file*/

   else if(!strcmp("-", parmStr))
      *samFileStr = parmStr;

   else if(!strcmp("-out", parmStr))
   { /*Else If: The user provided an output file*/
      *outFileStr = argStr;
      *shiftC = 1;
   } /*Else If: The user provided an output file*/

   else if(!strcmp("-out-tsv", parmStr))
   { /*Else If: The user provided an output file*/
      *tsvFileStr = argStr;
      *shiftC = 1;
   } /*Else If: The user provided an output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-03:
   ^   - Reference data
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(!strcmp("-ref-name", parmStr))
   { /*If: The user provided an reference name/id*/
      *refIdStr = argStr;
      *shiftC = 1;
   } /*If: The user provided an reference name/id*/

   else if(!strcmp("-ref-len", parmStr))
   { /*If: The user provided the reference length*/
      macTmpStr = base10StrToUI(argStr, *refLenUI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided reference length*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-04:
   ^   - Min quality scores, length, and depth (non-print)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(!strcmp("-min-q", parmStr))
   { /*If: The user provided the minimum Q-score*/
      macTmpStr = base10StrToUI(argStr, settings->minQI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum Q-score*/


   else if(!strcmp("-min-q-ins", parmStr))
   { /*If: user provided the minimum inserion Q-score*/
      macTmpStr= base10StrToUI(argStr,settings->minInsQI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: user provided the minimum inserion Q-score*/

   else if(!strcmp("-min-len", parmStr))
   { /*If: The user provided the minimum fragment length*/
      macTmpStr= base10StrToUI(argStr,settings->minLenI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum fragment length*/

   else if(!strcmp("-min-depth", parmStr))
   { /*If: The user provided the minimum depth*/
      macTmpStr=base10StrToUI(argStr,settings->minDepthI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum depth*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-05:
   ^   - Min percent support for snp/match, ins, & del
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*atof reurns 0 for failure*/
   else if(!strcmp("-perc-snp-sup", parmStr))
   { /*If: The user provided the minimum % snp support*/
      settings->minPercSnpF = atof(argStr);

      if(settings->minPercSnpF == 0)
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % snp support*/

   else if(!strcmp("-perc-ins-sup", parmStr))
   { /*If: The user provided the minimum % ins support*/
      settings->minPercInsF = atof(argStr);

      if(settings->minPercInsF == 0)
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % ins support*/

   else if(!strcmp("-perc-del-sup", parmStr))
   { /*If: The user provided the minimum % del support*/
      settings->minPercDelF = atof(argStr);

      if(settings->minPercDelF == 0)
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % del support*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-06:
   ^   - Masking
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(!strcmp("-mask", parmStr))
   { /*If: The user provided the base to mask with*/
      settings->maskC = *argStr;
      *shiftC = 1;
   } /*If: The user provided the base to mask with*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-07:
   ^   - Printing mininum settings
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Settings for printing out variations*/
   else if(!strcmp("-p-min-depth", parmStr))
   { /*If: The user provided the minimum depth*/
      macTmpStr =
        base10StrToUI(argStr,settings->printMinDepthI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum depth*/

   /*The print percentage depths*/
   else if(!strcmp("-p-perc-snp-sup", parmStr))
   { /*If: The user provided the minimum % snp support*/
      settings->printMinSupSnpF = atof(argStr);

      if(settings->printMinSupSnpF == 0)
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % snp support*/

   else if(!strcmp("-p-perc-ins-sup", parmStr))
   { /*If: The user provided the minimum % ins support*/
      settings->printMinSupInsF = atof(argStr);

      if(settings->printMinSupInsF == 0)
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % ins support*/

   else if(!strcmp("-p-perc-del-sup", parmStr))
   { /*If: The user provided the minimum % snp support*/
      settings->printMinSupDelF = atof(argStr);

      if(settings->printMinSupDelF == 0)
         macRetS |= def_tbCon_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum % snp support*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-08:
   ^   - Unkown input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(
         !strcmp("-h", parmStr)
      || !strcmp("--h", parmStr)
      || !strcmp("-help", parmStr)
      || !strcmp("--help", parmStr)
      || !strcmp("help", parmStr)
   ) macRetS = def_tbCon_pHelp;

   else if(
         !strcmp("-h-all", parmStr)
      || !strcmp("--h-all", parmStr)
      || !strcmp("-help-all", parmStr)
      || !strcmp("--help-all", parmStr)
      || !strcmp("help-all", parmStr)
   ) macRetS = def_tbCon_pFullHelp | def_tbCon_pHelp;

   else if(
         !strcmp("-v", parmStr)
      || !strcmp("--v", parmStr)
      || !strcmp("-version", parmStr)
      || !strcmp("--version", parmStr)
      || !strcmp("version", parmStr)
   ) macRetS = def_tbCon_pVersion;

   else macRetS = def_tbCon_unkown_Err;

   return macRetS;
} /*checkTbConArg*/

static void pTbConHelp(
   char pFullC,
   FILE *outFILE
){
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

static void pTbConVersion(
   FILE *outFILE
){ /*pTbConVersion*/
   fprintf(
       outFILE,
       "tbCon Version: %i-%i-%i\n",
       def_tbCon_year,
       def_tbCon_month,
       def_tbCon_day
   ); /*Version of tbCon*/
} /*pTbConVersion*/
#endif
