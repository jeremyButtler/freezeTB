/*########################################################
# Name: tbCon-inputFus.h
#  - Holds functions for getting input, printing version
#    numbers, and printing help messages
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries
'   o fun-01: checkTbConArg
'     - Checks user input to see if was a valid argument,
'       if so, this then sets the argument
'   o fun-02: pTbConHelp
'     - Prints out the tbCon help message
'   o fun-03: pTbConVersion
'     - Prints out the version number for tbCon
'   o license:
'     - Licensing for this code (public domain / mit)
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

#include "tbCon-version.h"

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
      macTmpStr = base10StrToSI(argStr, settings->minQI);

      /*Check for errors*/
      if(macTmpStr[0] != '\0')
         macRetS |= def_tbCon_input_nonNum_Err;

      *shiftC = 1;
   } /*If: The user provided the minimum Q-score*/


   else if(! cStrEql("-min-q-ins", parmStr, '\0'))
   { /*If: user provided the minimum inserion Q-score*/
      macTmpStr= base10StrToSI(argStr,settings->minInsQI);

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
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02 TOC:
   '   - Print the help message for tbCon
   '   o fun-02 sec-01:
   '     - Print out the usage and description entry
   '   o fun-02 sec-02:
   '     - Print out the input paramaters
   '   o fun-02 sec-03:
   '     - Print output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-01:
   ^   - Print out the usage and description entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Usage: tbCon -sam file.sam\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Makes an consensus genome from input sam file"
    );

    fprintf((FILE *) outFILE, "\n");

    fprintf((FILE *) outFILE, "Input:\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^   - Print out the input paramaters
   ^   o fun-02 sec-02 sub-02:
   ^     - Print out user input/output options
   ^   o fun-02 sec-02 sub-03:
   ^     - Print cosnesus settings
   ^   o fun-02 sec-02 sub-04:
   ^     - Print variant file settings
   ^   o fun-02 sec-02 sub-05:
   ^     - Help message and version numbers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun-02 Sec-02 Sub-02:
    *   - Print out user input/output options
    *   o fun-02 sec-02 sub-02 cat-01:
    *     - File IO header
    *   o fun-02 sec-02 sub-02 cat-02:
    *     - sam file input parameter
    *   o fun-02 sec-02 sub-02 cat-03:
    *     - output file parameter
    *   o fun-02 sec-02 sub-02 cat-04:
    *     - output variant tsv parameter
    \****************************************************/

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun-02 Sec-02 Sub-02 Cat-01:
    +   - File IO header
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf((FILE *) outFILE, "  Input and Output:\n");

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun-02 Sec-02 Sub-02 Cat-02:
    +   - sam file input parameter
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "    -sam: [Required; stdin]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Sam file with mapped reads\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Use '-' for stdin input\n"
    );

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun-02 Sec-02 Sub-02 Cat-03:
    +   - output file parameter
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "    -out: [stdout]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Sam file to save output cosnesuses in\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Use '-' for stdout output\n"
    );

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun-02 Sec-02 Sub-02 Cat-04:
    +   - output variant tsv parameter
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *)outFILE,
       "    -out-tsv: [Not used]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Name of tsv to save filtered variants to"
    );

    fprintf((FILE *) outFILE, "\n");

   if(! pFullC)
      goto skipExtaSettings_fun02_sec02_sub05;

   /****************************************************\
   * Fun-02 Sec-02 Sub-03:
   *   - Print cosnesus settings
   *   o fun-02 sec-02 sub-03 cat-01:
   *     - Consensus settings header
   *   o fun-02 sec-02 sub-03 cat-02:
   *     - Minimum mapping quality parameter
   *   o fun-02 sec-02 sub-03 cat-03:
   *     - Minimum Base Q-score value parameter
   *   o fun-02 sec-02 sub-03 cat-04:
   *     - Minimum insertion Q-score value parameter
   *   o fun-02 sec-02 sub-03 cat-05:
   *     - Minimum depth setting
   *   o fun-02 sec-02 sub-03 cat-06:
   *     - Minimum length setting
   *   o fun-02 sec-02 sub-03 cat-07:
   *     - Mask parameter
   *   o fun-02 sec-02 sub-03 cat-08:
   *     - Minimum percent SNP support setting
   *   o fun-02 sec-02 sub-03 cat-09:
   *     - Minimum percent insertion support setting
   *   o fun-02 sec-02 sub-03 cat-10:
   *     - Minimum percent deletion support setting
   \****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-01:
   +   - Consensus settings header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf((FILE *) outFILE, "  Cosensus settings:\n");

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-02:
   +   - Minimum mapping quality parameter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-mapq: [%i]\n",
      defMinMapQ
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum mapping quailty to keep an read\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-03:
   +   - Minimum Base Q-score value parameter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   
   fprintf(
      (FILE *) outFILE,
      "    -min-q: [%i]\n",
      defMinBaseQ
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum Q-score to keep an base\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-04:
   +   - Minimum insertion Q-score value parameter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   
   fprintf(
      (FILE *) outFILE,
      "    -min-q-ins: [%i]\n",
      defMinInsQ
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum Q-score to keep an insertion\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-05:
   +   - Minimum depth setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-depth: [%i]\n",
      defMinDepth
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum read depth to not break consensus"
    );

    fprintf(
       (FILE *) outFILE,
       "\n        into fragments\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-06:
   +   - Minimum length setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-len: [%i]\n",
      defMinLen
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum fragment length (-min-depth)"
    );

    fprintf(
       (FILE *) outFILE,
       " needed\n        to keep an fragment\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-06:
   +   - Mask parameter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -mask: [%c]\n",
      defMaskBase
   );

    fprintf(
       (FILE *) outFILE,
       "      o Base to mask unsupported positions with\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-07:
   +   - Minimum percent SNP support setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-snp-sup: [%.2f]\n",
      defMinPerSnp
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to not"
    );

    fprintf(
       (FILE *) outFILE,
       "\n        mask an SNP/match (range: 0 to 1)\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-08:
   +   - Minimum percent insertion support setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-ins-sup: [%.2f]\n",
      defMinPerIns
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to keep an"
    );

    fprintf(
       (FILE *) outFILE,
       "\n        insertion (range: 0 to 1)\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Unsupported positions are removed\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-09:
   +   - Minimum percent deltion support setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-del-sup: [%.2f]\n",
      defMinPerDel
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to keep an"
    );

    fprintf(
       (FILE *) outFILE,
       "\n        deletion (range: 0 to 1)\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Unsupported positions are replaced with"
    );

   fprintf((FILE *) outFILE, " -mask\n");

   /****************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - Print variant file settings
   *   o fun-02 sec-02 sub-04 cat-01:
   *     - Variant file header
   *   o fun-02 sec-02 sub-04 cat-02:
   *     - Print Minimum depth setting
   *   o fun-02 sec-02 sub-04 cat-03:
   *     - Print Minimum percent SNP support setting
   *   o fun-02 sec-02 sub-04 cat-04:
   *     - Print Minimum percent insertion support setting
   *   o fun-02 sec-02 sub-04 cat-05:
   *     - Minimum percent deltion support setting
   \****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-04 Cat-01:
   +   - Consensus settings header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf((FILE *) outFILE, "  Print variants:\n");

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-04 Cat-02:
   +   - Print Minimum depth setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-min-depth: [%i]\n",
      defMinPrintDepth
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum read depth print out an variant\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-04 Cat-03:
   +   - Print Minimum percent SNP support setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-snp-sup: [%.2f]\n",
      defMinPrintPerSnp
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to print"
    );

    fprintf(
       (FILE *) outFILE,
       "\n        an SNP/match variant (range: 0 to 1)\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-04 Cat-04:
   +   - Print Minimum percent insertion support setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-ins-sup: [%.2f]\n",
      defMinPrintPerIns
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to print"
    );

    fprintf(
       (FILE *) outFILE,
       " an\n        insertion variant (range: 0 to 1)\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-04 Cat-05:
   +   - Minimum percent deltion support setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-del-sup: [%.2f]\n",
      defMinPrintPerDel
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to print"
    );

    fprintf(
       (FILE *) outFILE,
       " an\n        deletion variant (range: 0 to 1)\n"
    );

   /****************************************************\
   * Fun-02 Sec-02 Sub-05:
   *   - Help message and version numbers
   \****************************************************/

   skipExtaSettings_fun02_sec02_sub05:;

   fprintf(
      (FILE *) outFILE,
      "  -h: Print the shortened help message\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -h-all: Print the entire help message\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: Print the version number\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-03:
   ^   - Print output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:\n");

   fprintf(
      outFILE,
      "  - Prints the consensus to -out as a sam file\n"
    );

    fprintf(
       outFILE,
      "  - Prints variants to -out-tsv (if provided)\n"
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
       "tbCon Version: %i-%02i-%02i\n",
       def_tbCon_input_year,
       def_tbCon_input_month,
       def_tbCon_input_day
   ); /*Version of tbCon*/
} /*pTbConVersion*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
