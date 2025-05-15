/*########################################################
# Name: whoToTbAmr
#   - holds functions to convert 2021/2023 WHO database to
#     tbAmr format.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o .c fun01: amrIsRes_whoToAmr
'     - dectects if an amr is classified as resistant
'   o .c fun02: addOneAa_whoToAmr
'     - add single amino acid variant to an amrST struct
'   o fun03: WHO2021Catalog_whoToAmr
'     - gets amrs from the genome indice tab (as csv) of
'       the WHO TB antibiotic resistance 2021 catalog
'   o c. fun04: crossRes_whoToAmr
'     - check if have cross resitance (2023 catalog)
'   o fun05: WHO2023Catalog_whoToAmr
'     - reads in both tabs of the 2023 WHO TB catalog (as
'       separate tsv's) and extracts AMRs
'   o fun06: parseVarId_whoToAmr
'     - parse variant idea from the WHO 2023 TB catalog
'   o .c fun07: sortCoords_whoToAmr
'     - sorts the gene start, end, and direction arrays by
'       the starting coordiantes
'   o fun08: addCodonPos_whoToAmr
'       - Adds the amino acid sequences for deletions and
'         large duplications, reading frame orientation
'         (forward/reverse) to the, and the first
'         reference base in the codon to an amrST that
'         has been processed with parseVarId_whoToAmr.
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
   /*not sure on strstr library*/
#else
   #include <stdlib.h>
   #include <string.h>
#endif

#include "whoToAmr.h"

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"
#include "../genLib/genMath.h"
#include "../genLib/fileFun.h"
#include "../genBio/codonFun.h"

#include "../genFreezeTB/drugAry.h"
#include "../genFreezeTB/amrST.h"
#include "genIndice.h"

/*Are just .h files*/
#include "../genLib/endLine.h"
#include "../genBio/ntTo2Bit.h"
#include "../genBio/revNtTo2Bit.h"
#include "../genBio/codonTbl.h"

/*-------------------------------------------------------\
| Fun01: amrIsRes_whoToAmr
|   - dectects if an amr is classified as resistant
| Input:
|   - amrSC:
|     - character with grade assigned by the WHO
| Output:
|   - Returns:
|     o 1 if was a grade 1/2 (resistance)
|     o 0 if was not a grade 1/2
\-------------------------------------------------------*/
#define amrIsRes_whoToAmr(amrSC) ( ( (amrSC) > 48 ) & ( (amrSC) < 51 ) )
  /*Logic:
  `  - lt3: amrSC < 51:
  `    o 1 if is a number < 3 or is a special symbol, or
  `      an invisible character
  '    o limits to special symbols and 0, 1, 2
  '      This should remove NA's, and grade 3, 4, and 5
  '  - gt0: amrSC > 48:
  `    o 1 if is a number > 0 or a character + extra
  `    o 0 if not 1
  `  -  gt0 & lt3:
  `    o 1 if number is 1 or 2
  `    o 0 if character is not 1 or 2
  */     

/*-------------------------------------------------------\
| Fun02: addOneAa_whoToAmr
|   - add single amino acid variant to an amrST struct
| Input:
|   - amrSTPtr:
|     - pointer to amrST structure to add variant to
|   - refAaSC:
|     - reference amino acid
|   - amrAaSC:
|     - mutant (amr) amino acid
| Output:
|   - Modifies:
|     o refAaStr in amrSTPtr to pointer to buffer with
|       the refAaSC amino acid
|     o lenRefAaUI to be 2 (aa and null)
|     o amrAaStr in amrSTPtr to pointer to buffer with
|       amrAaSC amino acid
|     o lenAmrAaUI to be 2 (aa and null)
|   - Returns
|     o 0 for success
|     o def_memErr_amrST for a memory error
\-------------------------------------------------------*/
signed char
addOneAa_whoToAmr(
   struct amrST *amrSTPtr,
   signed char refAaSC,
   signed char amrAaSC
){
   signed char errSC = 0;

   /*save reference amino acid*/
   if(amrSTPtr->refAaStr)
      free(amrSTPtr->refAaStr);/*make sure no old memory*/

   amrSTPtr->refAaStr = 0;

   amrSTPtr->lenRefAaUI = 1;
   amrSTPtr->refAaStr = malloc(2 * sizeof(signed char));

   if(! amrSTPtr->refAaStr)
      goto err_fun02;

   amrSTPtr->refAaStr[0] = refAaSC;
   amrSTPtr->refAaStr[1] = '\0';

   /*save amr amino acid*/
   amrSTPtr->lenAmrAaUI = 1;
   amrSTPtr->amrAaStr = malloc(2 * sizeof(signed char));

   if(! amrSTPtr->amrAaStr)
      goto err_fun02;

   amrSTPtr->amrAaStr[0] = amrAaSC;
   amrSTPtr->amrAaStr[1] = '\0';

   errSC = 0;
   goto ret_fun02;

   err_fun02:;
   errSC = def_memErr_amrST;
   goto ret_fun02;

   ret_fun02:;
   return errSC;
} /*addOneAa_whoToAmr*/

/*-------------------------------------------------------\
| Fun03: WHO2021Catalog_whoToAmr
|   - gets amrs from the genome indice tab (as csv) of the
|     WHO TB antibiotic resistance 2021 catalog
| Input:
|   - whoCsvStr:
|     o c-string with the path to the csv file for the
|       WHO's genome indience tab of the TB antibiotic
|       catalog spread sheet
|   - numArmUL:
|     o modified to hold the number of catalog entriex
|   - drugAryStr:
|     o c-string that will hold the name of each drug
|   - numDrugsSI:
|     o will have the number of druges in drugAryStr
|   - keepNonResBl:
|     o 1: keep grade 3, 4, and 5 entries (not an AMR)
|     o 0: only keep mutations that cause an AMR
|   - errSC:
|     o pointer to signed char to hold error message
| Output:
|   - Modifies:
|     o numArmUL to hold the number of extracted amr
|       mutations.
|     o drugAryStr to hold the name of each antibiotic
|     o numDrugsSI to hold the number of drugs in
|       drugAryStr
|     o errSC to hold error message
|       - 0 if no error
|       - def_memErr_amrST for memory errors
|       - def_fileErr_amrST for file errors
|   - Returns:
|     o array of amrST's (free with freeHeapAry_amrST)
\-------------------------------------------------------*/
struct amrST *
WHO2021Catalog_whoToAmr(
   signed char *whoCsvStr,
   unsigned long *numAmrUL,
   signed char **drugAryStr,
   signed int *numDrugsSI,
   signed char keepNonResBl,   /*1: to keep everything*/
   signed char *errSC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec01:
   '     - gets amrs from genome indice tab (as csv) of
   '       the WHO TB antibiotic resistance 2021 catalog
   '   o fun03 sec02:
   '     - find the number of lines in the who file
   '   o fun03 sec03:
   '     - set up the antibiotic table
   '   o fun03 sec04:
   '     - extract AMR mutations
   '   o fun03 sec05:
   '     - return and clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declartions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define lenBuffUS 2048 << 2
   signed char buffStr[lenBuffUS]; /*buffer*/
   signed char *lineHeapStr = buffStr;
      /*holds a single line*/
   signed char *tmpStr = 0;

   signed long  maxLineSL = 0; /*max line length*/
   signed long numLinesSL = 0;

   unsigned int uiIter = 0;    /*iterator for loops*/
   unsigned int uiColIter = 0; /*looping through columns*/
   unsigned int uiAmr = 0;

   unsigned int lenSeqUI = 0;
   unsigned int lenIdUI = 0;

   struct amrST *amrHeapST = 0;
   FILE *whoFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - find the number of lines in the who file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   whoFILE = fopen((char *) whoCsvStr, "r");
   if(! whoFILE)
      goto fileErr_fun03_sec05_sub03;
   numLinesSL = lineCnt_fileFun(whoFILE, &maxLineSL);
   
   if(numLinesSL < 2)
      goto fileErr_fun03_sec05_sub03;
   --numLinesSL; /*account for header*/
   maxLineSL += 3; /*account for line breaks and null*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - set up the antibiotic table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *drugAryStr = alloc_drugAry(14);

   if(*drugAryStr == 0)
      goto memErr_fun03_sec05_sub02;

   *numDrugsSI = 14;

   /*WHO has changed format, so safe to hardcode*/
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "RIF",
      0,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "INH",
      1,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "EMB",
      2,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "PZA",
      3,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "LEV",
      4,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "MXF",
      5,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "BDQ",
      6,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "LZD",
      7,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "CFZ",
      8,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "AMI",
      9,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "STM",
      10,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "ETH",
      11,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "KAN",
      12,
      '\0'
   );
   cpDrug_drugAry(
      *drugAryStr,
      (signed char *) "CAP",
      13,
      '\0'
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - extract AMR mutations
   ^   o fun03 sec04 sub01:
   ^     - set up memory and move past catalog header
   ^   o fun03 sec04 sub02:
   ^     - find length of gene name
   ^   o fun03 sec04 sub03:
   ^     - check if it is reverse complement
   ^   o fun03 sec04 sub04:
   ^     - move past codon position 
   ^   o fun03 sec04 sub05:
   ^     - get reference coordinate of mutation
   ^   o fun03 sec04 sub06:
   ^     - copy reference sequence
   ^   o fun03 sec04 sub07:
   ^     - copy mutant sequence
   ^   o fun03 sec04 sub08:
   ^     - ignore amino acid entry (get with sam file)
   ^   o fun03 sec04 sub09:
   ^     - move to first antibiotic column
   ^   o fun03 sec04 sub10:
   ^     - get antibiotics
   ^   o fun03 sec04 sub11:
   ^     - get final decided position
   ^   o fun03 sec04 sub12:
   ^     - get final decided reference sequence
   ^   o fun03 sec04 sub13:
   ^     - get final decided mutant sequence
   ^   o fun03 sec04 sub14:
   ^     - get variant id
   ^   o fun03 sec04 sub15:
   ^     - set variant type
   ^   o fun03 sec04 sub16:
   ^     - move to next amr
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec04 Sub01:
   *   - set up memory and move past catalog header
   \*****************************************************/

   lineHeapStr =
      malloc(maxLineSL * sizeof(signed char));

   if(lineHeapStr == 0)
      goto memErr_fun03_sec05_sub02;

   amrHeapST = calloc(numLinesSL, sizeof(struct amrST));
     /*some waste in memory usage, but easier to do*/

   if(amrHeapST == 0)
      goto memErr_fun03_sec05_sub02;

   *numAmrUL = 0;

   if(! fgets((char *) lineHeapStr, maxLineSL, whoFILE) )
      goto fileErr_fun03_sec05_sub03;
      /*nothing in file, never will happen*/

   while(fgets((char *) lineHeapStr, maxLineSL, whoFILE))
   { /*Loop: get AMRs from WHO catalog*/

     /***************************************************\
     * Fun03 Sec04 Sub02:
     *   - find length of gene name
     \***************************************************/

     for(
        uiIter = 0;
        lineHeapStr[uiIter] != ',';
        ++uiIter
     ){ /*Loop: move past first entry*/
         if(lineHeapStr[uiIter] == '\0')
            goto fileErr_fun03_sec05_sub03;
     } /*Loop: move past first entry*/
     
     lenIdUI = uiIter; /*+1 for '\0'*/

     amrHeapST[*numAmrUL].geneIdStr =
        calloc((lenIdUI + 1), sizeof(signed char));

     if(! amrHeapST[*numAmrUL].geneIdStr)
        goto memErr_fun03_sec05_sub02;
     
     for(
        uiIter = 0;
        lineHeapStr[uiIter] != ',';
        ++uiIter
     ) amrHeapST[*numAmrUL].geneIdStr[uiIter] =
          lineHeapStr[uiIter];

     amrHeapST[*numAmrUL].geneIdStr[uiIter] = '\0';
     amrHeapST[*numAmrUL].lenGeneIdUI = uiIter;

     ++uiIter;

     /***************************************************\
     * Fun03 Sec04 Sub03:
     *   - check if it is reverse complement
     \***************************************************/

     while(lineHeapStr[uiIter] != ',')
     { /*Loop: move to direction entry*/
         if(lineHeapStr[uiIter] == '\0')
            goto fileErr_fun03_sec05_sub03;

        ++uiIter;
     } /*Loop: move to direction entry*/
    
     amrHeapST[*numAmrUL].dirFlagSC =
         -(lineHeapStr[uiIter -1] == 'c')
       & def_revCompDir_amrST;
       /*not sure if this really works, but old fuction*/

     ++uiIter;
     
     /***************************************************\
     * Fun03 Sec04 Sub04:
     *   - move past codon position 
     \***************************************************/
     
     /*get off variant column*/
     while(lineHeapStr[uiIter] != ',')
     { /*Loop: move past codon position*/
        if(lineHeapStr[uiIter] == '\0')
           goto fileErr_fun03_sec05_sub03;

        ++uiIter;
     } /*Loop: move past codon position*/

     while(lineHeapStr[uiIter] != ',')
     { /*Loop: move past codon number*/
         if(lineHeapStr[uiIter] == '\0')
            goto fileErr_fun03_sec05_sub03;

         ++uiIter;
     } /*Loop: move past codon number*/

     ++uiIter; /*get off ,*/

     /*not reliable for finding first base in codon,
     `  so, i leverage the sam file for this information
     */

     /***************************************************\
     * Fun03 Sec04 Sub05:
     *   - get reference coordinate of mutation
     *   o fun03 sec04 sub05 cat01:
     *     - get reference coordiante
     *   o fun03 sec04 sub05 cat02:
     *     - move past reference coordiante
     \***************************************************/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun03 Sec04 Sub05 Cat01:
     +   - get reference coordiante
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     tmpStr = &lineHeapStr[uiIter];

     /*get around multi entries; they used commas*/
     while(*tmpStr < 48)
     { /*Loop: move past first entry*/
         if(*tmpStr == '\0')
            goto fileErr_fun03_sec05_sub03;
         ++tmpStr;
     } /*Loop: move past first entry*/

     tmpStr +=
        strToUI_base10str(
           tmpStr,
           &amrHeapST[*numAmrUL].refPosUI
        );

     /*convert index 1 to index 0*/
     --(amrHeapST[*numAmrUL].refPosUI);

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun03 Sec04 Sub05 Cat02:
     +   - move past reference coordiante
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     /*the next entry is the sequence, so no numbers*/
     if(lineHeapStr[uiIter] == '"')
     { /*If: had multiple positions*/
        for(
           uiIter = 1 + tmpStr - lineHeapStr;
           lineHeapStr[uiIter] != '"';
           ++uiIter
        ){ /*Loop: get past multiple positions*/
           if(lineHeapStr[uiIter] == '\0')
              goto fileErr_fun03_sec05_sub03;
        } /*Loop: get past multiple positions*/

        uiIter += 2;
     } /*If: had multiple positions*/

     else
     { /*Else: this entry only had a single position*/
        while(*tmpStr != ',')
        { /*Loop: find next entry*/
          if(*tmpStr == '\0')
             goto fileErr_fun03_sec05_sub03;

           ++tmpStr;
        } /*Loop: find next entry*/

        ++tmpStr; /*get off ,*/
        uiIter = tmpStr - lineHeapStr;
     } /*Else: This entry only had a single position*/

     /***************************************************\
     * Fun03 Sec04 Sub06:
     *   - copy reference sequence
     \***************************************************/

     /*get the length of the reference sequence*/

     for(
        lenSeqUI = uiIter;
        lineHeapStr[lenSeqUI] != ',';
        ++lenSeqUI
     ){ /*Loop: get length of reference sequence*/
        if(lineHeapStr[lenSeqUI] == '\0')
           goto fileErr_fun03_sec05_sub03;
     } /*Loop: get length of reference sequence*/

     amrHeapST[*numAmrUL].refSeqStr =
        calloc((lenSeqUI + 1), sizeof(signed char));

     if(! amrHeapST[*numAmrUL].refSeqStr)
        goto memErr_fun03_sec05_sub02;

     while(uiIter < lenSeqUI)
     { /*Loop: copy reference sequence*/
       amrHeapST[*numAmrUL].refSeqStr[uiIter] =
          lineHeapStr[uiIter];

       ++uiIter;
     } /*Loop: copy reference sequence*/

     amrHeapST[*numAmrUL].refSeqStr[uiIter] = '\0';
     amrHeapST[*numAmrUL].lenRefSeqUI = uiIter;

     ++uiIter; /*get off ','*/

     /***************************************************\
     * Fun03 Sec04 Sub07:
     *   - copy mutant sequence
     \***************************************************/

     for(
        lenSeqUI = uiIter;
        lineHeapStr[lenSeqUI] != ',';
        ++lenSeqUI
     ){ /*Loop: get length of mutant sequence*/
        if(lineHeapStr[lenSeqUI] == '\0')
           goto fileErr_fun03_sec05_sub03;
     } /*Loop: get length of mutant sequence*/

     amrHeapST[*numAmrUL].amrSeqStr =
        calloc((lenSeqUI + 1), sizeof(signed char));

     if(!amrHeapST[*numAmrUL].amrSeqStr)
        goto memErr_fun03_sec05_sub02;

     while(uiIter < lenSeqUI)
     { /*Loop: copy amr sequence*/
       amrHeapST[*numAmrUL].amrSeqStr[uiIter] =
          lineHeapStr[uiIter];

       ++uiIter;
     } /*Loop: copy amr sequence*/

     amrHeapST[*numAmrUL].amrSeqStr[uiIter] = '\0';
     amrHeapST[*numAmrUL].lenAmrSeqUI = uiIter;

     ++uiIter; /*get off ','*/

     /***************************************************\
     * Fun03 Sec04 Sub08:
     *   - ignore amino acid entry (get with sam file)
     \***************************************************/

     /*get positioning errors with this, so it is easier
     `  to add this in with a sam file
     */

     while(lineHeapStr[uiIter] !=',')
     { /*Loop: past amino acid entry*/
        if(lineHeapStr[uiIter] == '\0')
           goto fileErr_fun03_sec05_sub03;

        ++uiIter;
     } /*Loop: past amino acid entry*/

     ++uiIter;

     /***************************************************\
     * Fun03 Sec04 Sub09:
     *   - move to first antibiotic column
     \***************************************************/

     while(lineHeapStr[uiIter] !=',')
     { /*Loop: past extra entry*/
        if(lineHeapStr[uiIter] == '\0')
           goto fileErr_fun03_sec05_sub03;

        ++uiIter;
     } /*Loop: past extra entry*/

     ++uiIter;

     while(lineHeapStr[uiIter] !=',')
     { /*Loop: past extra entry*/
        if(lineHeapStr[uiIter] == '\0')
           goto fileErr_fun03_sec05_sub03;

        ++uiIter;
     } /*Loop: past extra entry*/

     ++uiIter;

     /***************************************************\
     * Fun03 Sec04 Sub10:
     *   - get antibiotics
     \***************************************************/

     for(
        uiAmr = 0;
        uiAmr < 15;
        ++uiAmr
     ){ /*Loop: get rating of all AMRs*/
        /*move to next AMR confidence column*/

        for(uiColIter = 0; uiColIter < 3; ++uiColIter)
        { /*Loop: move to genome_index column*/
           while(lineHeapStr[uiIter] !=',')
           { /*Loop: past column*/
              if(lineHeapStr[uiIter] == '\0')
                 goto fileErr_fun03_sec05_sub03;

              ++uiIter;
           } /*Loop: past column*/

           ++uiIter;
        } /*Loop: move to genome_index column*/

        amrHeapST[*numAmrUL].amrFlagsUL[0] |=
           (
                 amrIsRes_whoToAmr(lineHeapStr[uiIter])
              << uiAmr
           ); /*get if resistance is present*/
     } /*Loop: get rating of all AMRs*/

     if(
           ! keepNonResBl
        && amrHeapST[*numAmrUL].amrFlagsUL[0] == 0
     ){ /*If: discarding non-AMR mutants*/
        freeStack_amrST(&amrHeapST[*numAmrUL]);
        continue;
     } /*If: discarding non-AMR mutants*/

     /***************************************************\
     * Fun03 Sec04 Sub11:
     *   - get final decided position
     \***************************************************/

     while(lineHeapStr[uiIter] !=',')
     { /*Loop: get past last AMR entry*/
        if(lineHeapStr[uiIter] == '\0')
           goto fileErr_fun03_sec05_sub03;

        ++uiIter;
     } /*Loop: get past last AMR entry*/

     ++uiIter;

     while(lineHeapStr[uiIter] !=',')
     { /*Loop: get past reference entry*/
        if(lineHeapStr[uiIter] == '\0')
           goto fileErr_fun03_sec05_sub03;

        ++uiIter;
     } /*Loop: get past reference entry*/

     ++uiIter;

     uiIter +=
        strToUI_base10str(
           &lineHeapStr[uiIter],
           &amrHeapST[*numAmrUL].refPosUI
        );

     /*convert index 1 to index 0*/
     --(amrHeapST[*numAmrUL].refPosUI);

     while(lineHeapStr[uiIter] !=',')
     { /*Loop: move to next entry*/
        if(lineHeapStr[uiIter] == '\0')
           goto fileErr_fun03_sec05_sub03;

        ++uiIter;
     } /*Loop: move to next entry*/

     ++uiIter;

     /***************************************************\
     * Fun03 Sec04 Sub12:
     *   - get final decided reference sequence
     \***************************************************/

     lenSeqUI = 0;

     while(lineHeapStr[uiIter] !=',')
     { /*Loop: copy final reference sequence*/
        if(lineHeapStr[uiIter] == '\0')
           goto fileErr_fun03_sec05_sub03;

        amrHeapST[*numAmrUL].refSeqStr[lenSeqUI] =
            lineHeapStr[uiIter];

        ++lenSeqUI;
        ++uiIter;
     } /*Loop: copy final reference sequence*/
      
     amrHeapST[*numAmrUL].refSeqStr[lenSeqUI] = '\0';
     amrHeapST[*numAmrUL].lenRefSeqUI = lenSeqUI;
     ++uiIter; /*get off ','*/

     /***************************************************\
     * Fun03 Sec04 Sub13:
     *   - get final decided mutant sequence
     \***************************************************/

     lenSeqUI = 0;

     while(lineHeapStr[uiIter] !=',')
     { /*Loop: copy final AMR sequence*/
        if(lineHeapStr[uiIter] == '\0')
           goto fileErr_fun03_sec05_sub03;

        amrHeapST[*numAmrUL].amrSeqStr[lenSeqUI] =
            lineHeapStr[uiIter];

        ++lenSeqUI;
        ++uiIter;
     } /*Loop: copy final AMR sequence*/

     amrHeapST[*numAmrUL].amrSeqStr[lenSeqUI] = '\0';
     amrHeapST[*numAmrUL].lenAmrSeqUI = lenSeqUI;

     /***************************************************\
     * Fun03 Sec04 Sub14:
     *   - get variant id
     \***************************************************/

     for(
        uiColIter = 0;
        uiColIter < 6;
        ++uiColIter
     ){ /*Loop: to variant column*/
        while(lineHeapStr[uiIter] != ',')
        { /*Loop: to next column*/
           if(lineHeapStr[uiIter] == '\0')
              goto fileErr_fun03_sec05_sub03;

           ++uiIter;
        } /*Loop: to next column*/

        ++uiIter;
     } /*Loop: to variant column*/

     if(
           lineHeapStr[uiIter] == ','
        || lineHeapStr[uiIter] < 33
     ){ /*If: blank cell*/
       amrHeapST[*numAmrUL].lenVarIdUI = 2;

       amrHeapST[*numAmrUL].varIdStr =
          malloc(3 * sizeof(signed char));

       if(! amrHeapST[*numAmrUL].varIdStr)
          goto memErr_fun03_sec05_sub02;

       amrHeapST[*numAmrUL].varIdStr[0] = 'N';
       amrHeapST[*numAmrUL].varIdStr[1] = 'A';
       amrHeapST[*numAmrUL].varIdStr[2] = '\0';
     } /*If: blank cell*/

     for(
        uiColIter = uiIter;
        lineHeapStr[uiColIter] != ',';
        ++uiColIter
     ){ /*Loop: till next entry*/
        if(
              lineHeapStr[uiColIter] == '\n'
           || lineHeapStr[uiColIter] == '\r'
           || lineHeapStr[uiColIter] == '\0'
        ) break; /*no more entries*/
     } /*Loop: till next entry*/

     uiColIter -= uiIter;
     amrHeapST[*numAmrUL].lenVarIdUI = uiColIter;
     ++uiColIter;

     amrHeapST[*numAmrUL].varIdStr =
        calloc(uiColIter, sizeof(signed char));

     for(
        uiColIter = 0;
        uiColIter < amrHeapST[*numAmrUL].lenVarIdUI;
        ++uiColIter
     ) { /*Loop: copy variant id*/
        if(
              lineHeapStr[uiColIter] == '\n'
           || lineHeapStr[uiColIter] == '\r'
           || lineHeapStr[uiColIter] == '\0'
        ) break; /*no more entries*/

        amrHeapST[*numAmrUL].varIdStr[uiColIter] =
           lineHeapStr[uiIter];

        ++uiIter;
     } /*Loop: copy variant id*/

     amrHeapST[*numAmrUL].varIdStr[uiColIter] = '\0';

     /***************************************************\
     * Fun03 Sec04 Sub15:
     *   - set variant type
     \***************************************************/

     if(  amrHeapST[*numAmrUL].lenAmrSeqUI
        < amrHeapST[*numAmrUL].lenRefSeqUI
     ){ /*If: deletion*/
        amrHeapST[*numAmrUL].mutTypeStr[0] = 'd';
        amrHeapST[*numAmrUL].mutTypeStr[1] = 'e';
        amrHeapST[*numAmrUL].mutTypeStr[2] = 'l';
     } /*If: deletion*/

     else if(
          amrHeapST[*numAmrUL].lenAmrSeqUI
        > amrHeapST[*numAmrUL].lenRefSeqUI
     ){ /*Else If: insertion*/
        amrHeapST[*numAmrUL].mutTypeStr[0] = 'i';
        amrHeapST[*numAmrUL].mutTypeStr[1] = 'n';
        amrHeapST[*numAmrUL].mutTypeStr[2] = 's';
     } /*Else If: insertion*/

     else
     { /*Else: assuming snp*/
        amrHeapST[*numAmrUL].mutTypeStr[0] = 's';
        amrHeapST[*numAmrUL].mutTypeStr[1] = 'n';
        amrHeapST[*numAmrUL].mutTypeStr[2] = 'p';
     } /*Else: assuming snp*/

     amrHeapST[*numAmrUL].mutTypeStr[3] = '\0';

     /***************************************************\
     * Fun03 Sec04 Sub16:
     *   - move to next amr
     \***************************************************/

     ++(*numAmrUL);
     init_amrST(&amrHeapST[*numAmrUL]);
   } /*Loop: get AMRs from WHO catalog*/
  
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec05:
   ^   - return and clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sortPos_amrST(amrHeapST, 0, *numAmrUL - 1);
   *errSC = 0;
   goto cleanUp_fun03_sec05_sub05;

   memErr_fun03_sec05_sub02:;
      *errSC = def_memErr_amrST;
      goto errCleanUp_fun03_sec05_sub04;

   fileErr_fun03_sec05_sub03:;
      *errSC = def_fileErr_amrST;
      goto errCleanUp_fun03_sec05_sub04;

   errCleanUp_fun03_sec05_sub04:;
      if(amrHeapST)
      { /*If: need to free the AMR array*/
         freeHeapAry_amrST(
           amrHeapST,
           *numAmrUL
         );
         amrHeapST = 0;
      } /*If: need to free the AMR array*/

      amrHeapST = 0;
      goto cleanUp_fun03_sec05_sub05;

   cleanUp_fun03_sec05_sub05:;
      if(! whoFILE) ;
      else if(whoFILE == stdin) ;
      else if(whoFILE != stdout) ;
      else 
         fclose(whoFILE);
      whoFILE = 0;

      if(! lineHeapStr) ;
      else if(lineHeapStr != buffStr)
         free(lineHeapStr); /*buffStr on stack*/
      lineHeapStr = 0;

      return amrHeapST;
} /*WHO2021Catalog_whoToAmr*/

/*-------------------------------------------------------\
| Fun04: crossRes_whoToAmr
|   - check if have cross resitance (2023 catalog)
| Input:
|   - crossStr:
|     o c-string with cross resitance to check
|   - amrSTPtr:
|     o pointer to amrST structure to add cross resitance
|   - dugAryStr:
|     o drugAry (drugAry.h, really c-string) to hold
|       new antbiotic names
|   - numDrugsSI:
|     o pointer to signed int with number of antibiotics
|       in drugAryStr
|   - maxDrugsSI:
|     o pointer to signed int with maximum number of
|       antibiotics that can be stored in the current
|       drugAryStr
| Output:
|   - Modifies:
|     o crosRefFlagsUL in amrSTPtr to hold cross resitance
|       results
|     o drugAryStr if needs to be resized
|       - 0/free if had a memory error
|       - not 0/resized if everything went ok
|     o numDrugsSI if added a new antibtiotic
|     o maxDrugsSI if had to resize drugAryStr
|   - Returns:
|     o 0 for success
|     o def_noRes_whoToAmr for not an amr
|     o def_memErr_amrST for memory errors
\-------------------------------------------------------*/
signed char
crossRes_whoToAmr(
   signed char *crossStr,
   struct amrST *amrSTPtr,
   signed char **drugAryStr,
   signed int *numDrugsSI,
   signed int *maxDrugsSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - check if have cross resitance (2023 catalog)
   '   o fun04 sec01:
   '     - variable declerations
   '   o fun04 sec02:
   '     - resize drug array if full
   '   o fun04 sec03:
   '     - check no cross resistance and fluoroquine
   '   o fun04 sec04:
   '     - check bedaquiline/clorfazimine cross resistance
   '   o fun04 sec05:
   '     - check isonizaid & ethionamide cross resistance
   '   o fun04 sec06:
   '     - check delamanid and penicillin myceial dreg res
   '   o fun04 sec07:
   '     - return if had error
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siShift = 0;
   signed char errSC = 0;
   signed char *tmpStr = 0; /*for reallocs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - resize drug array if full
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*numDrugsSI >= *maxDrugsSI)
   { /*If: need to resize array*/
      *maxDrugsSI <<= 1;

      tmpStr =
         realloc_drugAry(
            *drugAryStr,
            *maxDrugsSI
         );

      if(! tmpStr)
         goto memErr_fun04_sec07;

      *drugAryStr = tmpStr;
   } /*If: need to resize array*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - check no cross resistance and fluoroquine
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*crossStr < 32)
      goto noRes_fun04_sec07;

   else if(
      ! eql_charCp(
         (signed char *) "FQ X-R\t",
         crossStr,
         (signed char) '\t'
       )
   ){ /*Else If: fluoroquine cross resitance*/
      siShift =
        find_drugAry(
           (signed char *) "fluoroquine",
           *drugAryStr,
           *numDrugsSI + 1,
           '\0'
        );

      if(siShift < 0)
      { /*If: no fluoroquine in drug array*/
         ++(*numDrugsSI);
         siShift = *numDrugsSI;

         cpDrug_drugAry(
            *drugAryStr,
            (signed char *) "fluoroquine",
            *numDrugsSI,
            '\0'
         ); /*add fluoroquine to drug array*/
      } /*If: no fluoroquine in drug array*/

      (amrSTPtr)->crossResFlagsUL[0] |=
         (1UL << (siShift)); /*mark as cross resistant*/
   } /*Else If: fluoroquine cross resitance*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - check bedaquiline/clorfazimine cross resistance
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(
      ! eql_charCp(
         (signed char *) "BDQ-CFZ X-R\t",
         crossStr,
         '\t'
      )
   ){ /*Else If: bedaquiline/clofazimine*/
      siShift =
        find_drugAry(
           (signed char *) "bedaquiline",
           *drugAryStr,
           *numDrugsSI + 1,
           '\0'
        );

      if(siShift < 0)
      { /*If: no bedaquiline in drug array*/
         ++(*numDrugsSI);
         siShift = *numDrugsSI;

         cpDrug_drugAry(
            *drugAryStr,
            (signed char *) "bedaquiline",
            *numDrugsSI,
            '\0'
         ); /*add bedaquiline to drug array*/
      } /*If: no bedaquiline in drug array*/

      (amrSTPtr)->crossResFlagsUL[0] |=
         (1UL << (siShift)); /*flag bedaquiline res*/

      siShift =
        find_drugAry(
           (signed char *) "clofazimine",
           *drugAryStr,
           *numDrugsSI + 1,
           '\0'
        );

      if(siShift < 0)
      { /*If: no clorfazimine in drug array*/
         ++(*numDrugsSI);
         siShift = *numDrugsSI;

         cpDrug_drugAry(
            *drugAryStr,
            (signed char *) "clofazimine",
            *numDrugsSI,
            '\0'
         ); /*add clorfazimin to drug array*/
      } /*If: no clorfazimine in drug array*/

      (amrSTPtr)->crossResFlagsUL[0] |=
         (1UL << (siShift)); /*flag clorfazimin res*/
   } /*Else If: bedaquiline/clofazimine*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec05:
   ^   - check isonizaid and ethionamide cross resistance
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(
      ! eql_charCp(
         (signed char *) "INH-ETO X-R\t",
         crossStr,
         '\t'
      )
   ){ /*Else If: isoniazid/ethionamide resistance*/
      siShift =\
        find_drugAry(\
           (signed char *) "isoniazid",
           *drugAryStr,
           *numDrugsSI + 1,
            '\0'
        );

      if(siShift < 0)
      { /*If: no isoniazid in drug array*/
         ++(*numDrugsSI);
         siShift = *numDrugsSI;

         cpDrug_drugAry(
            *drugAryStr,
            (signed char *) "isoniazid",
            *numDrugsSI,
            '\0'
         ); /*add isoniazid to drug array*/
      } /*If: no isoniazid in drug array*/

      (amrSTPtr)->crossResFlagsUL[0] |=
         (1UL << (siShift)); /*flag isoniazid resistance*/

      siShift =
        find_drugAry(\
           (signed char *) "ethionamide",
           *drugAryStr,
           *numDrugsSI + 1,
           '\0'
        );

      if(siShift < 0)
      { /*If: no ethiondamide in drug array*/
         ++(*numDrugsSI);
         siShift = *numDrugsSI;

         cpDrug_drugAry(
            *drugAryStr,
            (signed char * ) "ethionamide",
            *numDrugsSI,
            '\0'
         ); /*add ethiondamide to drug array*/
      } /*If: no ethiondamide in drug array*/
      \
      (amrSTPtr)->crossResFlagsUL[0] |=
         (1UL << (siShift)); /*flag ethionmaide res*/
   } /*Else If: isoniazid/ethionamide resistance*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec06:
   ^   - check delamanid and penicillin myceial dreg res
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(
      ! eql_charCp(
         (signed char *)
            "Confers DLM-PMD cross-resistance\t",
         crossStr,
         '\t'
      )
   ){ /*Else If: is delamanid/(penicillin myceial dreg)*/
      siShift =
        find_drugAry(\
           (signed char *) "delamanid",
           *drugAryStr,
           *numDrugsSI + 1,
           '\0'
        );

      if(siShift < 0)
      { /*If: no delamind in drug array*/
         ++(*numDrugsSI);
         siShift = *numDrugsSI;

         cpDrug_drugAry(
            *drugAryStr,
            (signed char *) "delamanid",
            *numDrugsSI,
            '\0'
         ); /*add delamanind to drug array*/
      } /*If: no delamind in drug array*/

      (amrSTPtr)->crossResFlagsUL[0] |=
         (1UL << (siShift)); /*flag delamind resistance*/

      siShift =
        find_drugAry(
           (signed char *) "penicillin-myceial-dreg",
           *drugAryStr,
           *numDrugsSI + 1,
            '\0'
        );

      if(siShift < 0)
      { /*If: no penicillin myceial dreg in drug array*/
         ++(*numDrugsSI);
         siShift = *numDrugsSI;

         cpDrug_drugAry(
            *drugAryStr,
            (signed char *) "penicillin-myceial-dreg",
           *numDrugsSI,
            '\0'
         ); /*add penicillin mycial dreg to drug array*/
      } /*If: no penicillin myceial dreg in drug array*/

      (amrSTPtr)->crossResFlagsUL[0] |=
         (1UL << (siShift)); /*flag penicillin mycial*/
   } /*Else If: is delamanid/(penicillin myceial dreg)*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec07:
   ^   - return if had error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun04_sec07;

   noRes_fun04_sec07:;
   errSC = def_noRes_whoToAmr;
   goto ret_fun04_sec07;

   memErr_fun04_sec07:;
   errSC = def_memErr_amrST;
   goto ret_fun04_sec07;

   ret_fun04_sec07:;
   return errSC;
} /*crossRes_whoToAmr*/

/*-------------------------------------------------------\
| Fun05: WHO2023Catalog_whoToAmr
|   - reads in both tabs of the 2023 WHO TB catalog (as
|     separate tsv's) and extracts AMRs
| Input:
|   - whoMasterStr:
|     o c-string with the path to the tsv file with 
|       tab one (master) of the 2023 who TB catalog
|   - whoIndiceFILE:
|     o c-string with the path to the tsv with tab two
|       (genome indices) of the 2023 who catalog
|   - numAmrUL:
|     o modifed to hold the number of kept amr genes
|   - drugStr:
|     o modified to hold the antibiotic names. Each name
|       is at most 31 characters long. To get to the next
|       name add 32.
|   - numDrugsSI:
|     o modfied to hold the number of drugs in drugStr
|   - keepNonResBl:
|     o 1: keep grade 3, 4, and 5 entries (not an AMR)
|     o 0: only keep mutations that cause an AMR
|   - rmFullGeneVarBl, 
|     o 1: ignore all variants that apply to the full gene
|     o 0: keep all variants
|   - errSC:
|     o holds error type
| Output:
|   - Modifies:
|     o numAmrUL to hold the number of used amrSTs in
|       the returned amrST structure array
|     o drugAryStr to hold the name of each antibiotic
|     o numDrugsSI to hold the number of drugs in
|       drugAryStr
|     o errSC:
|       - 0 for no errors
|       - def_memErr_amrST for memory errors
|       - def_fileErr_amrST for file errors
|   - Returns:
|     o array of amrST structures with catalog AMRs
\-------------------------------------------------------*/
struct amrST *
WHO2023Catalog_whoToAmr(
   signed char *whoMasterStr,  /*master tab tsv path*/
   signed char *whoIndiceStr,  /*genome indice tab path*/
   unsigned long *numAmrUL,    /*number of amrs kept*/
   signed char **drugAryStr,   /*holds antibiotic names*/
   signed int *numDrugsSI,     /*number drugs in drugAry*/
   signed char keepNonResBl,   /*1: keep all AMR grades*/
   signed char rmFullGeneVarBl,/*1:no entire gene events*/
   signed char *errSC          /*reports errors*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC: WHO2023Catalog_whoToAmr
    '   o fun05 sec01:
    '     - variable declerations
    '   o fun05 sec02:
    '     - get genome coordinates
    '   o fun05 sec03:
    '     - get master file length and setup buffer
    '   o fun05 sec04:
    '     - set up buffers
    '   o fun05 sec05:
    '     - read in file
    '   o fun05 sec06:
    '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun05 Sec01:
    ^   - Variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    signed char *buffHeapStr = 0;
    unsigned int lenBuffUI = 4096;

    signed int siTab = 0;
    unsigned int uiPos = 0;

    signed long lenFileSL = 0;
    signed long maxLineSL = 0;
    unsigned int lenPosAryUI = 0;
    signed char *tmpStr = 0;

    signed int siDrug = 0;     /*antbiotic on*/
    signed int maxDrugsSI = 0;
       /*number antibiotics in drugAry*/

    signed char cpAmrBl = 0;
      /*1: more than 1 indice for amr*/
    signed int indexSI = 0;
    struct genIndice *indiceHeapAryST = 0;

    struct amrST *amrHeapST = 0;

    FILE *whoFILE = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun05 Sec02:
    ^   - Get the genome coordinates
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    *errSC = 0;

    indiceHeapAryST =
       read2023Db_genIndice(
          whoIndiceStr,
          &lenPosAryUI,
          errSC
       ); /*get genome coordiantes*/

    if(indiceHeapAryST == 0)
    { /*If: error*/
       if(*errSC == def_memErr_genIndice)
          goto memErr_fun05_sec06_sub02;
       else
          goto fileErr_fun05_sec06_sub03;
    } /*If: error*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun05 Sec03:
    ^   - get master file length and setup buffer
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    whoFILE = fopen((char *) whoMasterStr, "r");
    if(whoFILE == 0)
       goto  fileErr_fun05_sec06_sub03;

    lenFileSL = lineCnt_fileFun(whoFILE, &maxLineSL);
    if(lenFileSL < 3)
       goto fileErr_fun05_sec06_sub03; /*file to short*/
    maxLineSL += 3; /*add room for \r, \n, and \0*/

    buffHeapStr =
       malloc((maxLineSL + 8) * sizeof(signed char));
    if(! buffHeapStr)
       goto memErr_fun05_sec06_sub02;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun05 Sec04:
    ^   - set up buffers
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    *numDrugsSI = 0;
    *drugAryStr = alloc_drugAry(16);

    if(*drugAryStr == 0)
      goto memErr_fun05_sec06_sub02;

    maxDrugsSI = 16;

    /*this will be massively oversized for just resitant
    `   genes, but it works.
    ` for all AMRs I need to add in lenFileSL becuase
    `   there are more AMRs the in lenPosAryUI or
    `   lenFileSL alone. this is quite annoying. i found
    `   that even just adding lenPosAryUI + lenFileSL was
    `   not enough. so, I needed to multiply lenFileSL by
    `   2. There are around 203910 entries total.
    */
    amrHeapST =
       calloc(
          lenPosAryUI + (lenFileSL << 1),
          sizeof(struct amrST)
       );

    if(amrHeapST == 0)
       goto memErr_fun05_sec06_sub02;

    *numAmrUL = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun05 Sec05:
    ^   - read in file
    ^   o fun05 sec05 sub01:
    ^     - get past header and start loop
    ^   o fun05 sec05 sub02:
    ^     - get primary antibiotic
    ^   o fun05 sec05 sub03:
    ^     - get gene name
    ^   o fun05 sec05 sub04:
    ^     - move past mutation column
    ^   o fun05 sec05 sub05:
    ^     - read in variant column
    ^   o fun05 sec05 sub06:
    ^     - read past teir column
    ^   o fun05 sec05 sub07:
    ^     - get effect column
    ^   o fun05 sec05 sub08:
    ^     - Get genomic coordinate if present.
    ^       this will be updated twice if there is an
    ^       genome corrdiante in the indicies tab, but
    ^       also ensures all positions with coordinates
    ^       are found
    ^   o fun05 sec05 sub09:
    ^     - move to next set of targets to extract
    ^   o fun05 sec05 sub10:
    ^     - check if resitance
    ^   o fun05 sec05 sub11:
    ^     - get comment entry
    ^   o fun05 sec05 sub12:
    ^     - check for cross resistance (additional grade)
    ^   o fun05 sec05 sub13:
    ^     - deal with genomic coordiantes
    ^   o fun05 sec05 sub14:
    ^     - move to next amr entry
    ^   o fun05 sec05 sub15:
    ^     - sort AMRs by position (after read in loop)
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun05 Sec05 Sub01:
    *   - get past header and start loop
    \****************************************************/

    /*move past the headers*/
    tmpStr =
       (signed char *)
       fgets(
          (char *) buffHeapStr,
          lenBuffUI,
          whoFILE
    ); /*first header*/

    tmpStr =
       (signed char *) 
       fgets(
          (char *) buffHeapStr,
          lenBuffUI,
          whoFILE
    ); /*second header*/

    *numDrugsSI = 0;
    uiPos = 0;

    goto firstAMR_fun05_sec05_sub02;

    while(
       fgets(
          (char *) buffHeapStr,
          lenBuffUI,
          whoFILE
       )
    ){ /*Loop: get AMRs from catalog*/

       /*************************************************\
       * Fun05 Sec05 Sub02:
       *   - get primary antibiotic
       \*************************************************/

       uiPos = 0;

       siDrug = 
          find_drugAry(
             buffHeapStr,
             *drugAryStr,
             *numDrugsSI + 1,
             '\t'
          );

       if(siDrug < 0)
       { /*If: on new antibiotic*/ 
         ++(*numDrugsSI);

         firstAMR_fun05_sec05_sub02:;

         if(*numDrugsSI >= maxDrugsSI - 1)
         { /*If: need to resize the drug list*/
            tmpStr =
               realloc_drugAry(
                  *drugAryStr,
                  *numDrugsSI + 10
               );

            if(! tmpStr)
               goto memErr_fun05_sec06_sub02;

            *drugAryStr = tmpStr;
            maxDrugsSI += 10;
         } /*If: need to resize the drug list*/

         cpDrug_drugAry(
             *drugAryStr,
              buffHeapStr,
              *numDrugsSI,
              '\t'
         ); /*add new drug to drug array*/
       } /*If: on new antibiotic*/ 

       amrHeapST[*numAmrUL].amrFlagsUL[0] =
          ((unsigned long) 1 << (siDrug));

       while(buffHeapStr[uiPos] > 31)
          ++uiPos;

       if(buffHeapStr[uiPos] != '\t')
          goto fileErr_fun05_sec06_sub03;

       ++uiPos; /*get off tab*/

       /*************************************************\
       * Fun05 Sec05 Sub03:
       *   - get gene name
       \*************************************************/

       amrHeapST[*numAmrUL].lenGeneIdUI = uiPos;

       while(
             buffHeapStr[amrHeapST[*numAmrUL].lenGeneIdUI]
          != '\t'
       ){ /*Loop: get gene id length*/
          if(
             buffHeapStr[amrHeapST[*numAmrUL].lenGeneIdUI]
             == '\0'
          ) goto fileErr_fun05_sec06_sub03;

          ++amrHeapST[*numAmrUL].lenGeneIdUI;
       } /*Loop: get gene id length*/
       
       amrHeapST[*numAmrUL].lenGeneIdUI -= uiPos;

       amrHeapST[*numAmrUL].geneIdStr =
          calloc(
             (amrHeapST[*numAmrUL].lenGeneIdUI + 1),
             sizeof(signed char)
          );

       if(! amrHeapST[*numAmrUL].geneIdStr)
          goto memErr_fun05_sec06_sub02;
       
       cpLen_charCp(
          amrHeapST[*numAmrUL].geneIdStr,
          &buffHeapStr[uiPos],
          amrHeapST[*numAmrUL].lenGeneIdUI
       );

       uiPos += amrHeapST[*numAmrUL].lenGeneIdUI + 1;

       /*************************************************\
       * Fun05 Sec05 Sub04:
       *   - move past mutation column
       \*************************************************/

       while(buffHeapStr[uiPos] != '\t')
       { /*Loop: to end of column*/
          if(buffHeapStr[uiPos] == '\0')
             goto fileErr_fun05_sec06_sub03;

          ++uiPos;
       } /*Loop: to end of column*/

       ++uiPos; /*get off tab*/

       /*************************************************\
       * Fun05 Sec05 Sub05:
       *   - read in variant column
       \*************************************************/

       amrHeapST[*numAmrUL].lenVarIdUI = uiPos;

       while(
             buffHeapStr[amrHeapST[*numAmrUL].lenVarIdUI]
          != '\t'
       ){ /*Loop: get variant id length*/
          if(
             buffHeapStr[amrHeapST[*numAmrUL].lenVarIdUI]
             == '\0'
          ) goto fileErr_fun05_sec06_sub03;

          ++amrHeapST[*numAmrUL].lenVarIdUI;
       } /*Loop: get variant id length*/
       
       amrHeapST[*numAmrUL].lenVarIdUI -= uiPos;

       amrHeapST[*numAmrUL].varIdStr =
          calloc(
             (amrHeapST[*numAmrUL].lenVarIdUI + 1),
             sizeof(signed char)
          );

       if(! amrHeapST[*numAmrUL].varIdStr)
          goto memErr_fun05_sec06_sub02;

       cpLen_charCp(
          amrHeapST[*numAmrUL].varIdStr,
          &buffHeapStr[uiPos],
          amrHeapST[*numAmrUL].lenVarIdUI
       );

       uiPos += amrHeapST[*numAmrUL].lenVarIdUI + 1;

       /*************************************************\
       * Fun05 Sec05 Sub06:
       *   - read past teir column
       \*************************************************/

       while(buffHeapStr[uiPos] != '\t')
       { /*Loop: to end of column*/
          if(buffHeapStr[uiPos] == '\0')
             goto fileErr_fun05_sec06_sub03;

          ++uiPos;
       } /*Loop: to end of column*/

       ++uiPos; /*get off tab*/

       /*************************************************\
       * Fun05 Sec05 Sub07:
       *   - get effect column
       *   o fun05 sec05 sub07 cat01:
       *     - find effect length and allocate memory
       *   o fun05 sec05 sub07 cat02:
       *     - copy effect entry and remove spaces
       \*************************************************/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun05 Sec05 Sub07 Cat01:
       +   - find effect length and allocate memory
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       amrHeapST[*numAmrUL].lenEffectUI = uiPos;

       while(
             buffHeapStr[amrHeapST[*numAmrUL].lenEffectUI]
          != '\t'
       ){ /*Loop: get effect length*/
          if(
             buffHeapStr[amrHeapST[*numAmrUL].lenEffectUI]
             == '\0'
          ) goto fileErr_fun05_sec06_sub03;

          ++amrHeapST[*numAmrUL].lenEffectUI;
       } /*Loop: get effect length*/

       amrHeapST[*numAmrUL].lenEffectUI -= uiPos;

       if(amrHeapST[*numAmrUL].lenEffectUI == 0)
          goto skipEffect_fun05_sec05_sub07_cat02;

       amrHeapST[*numAmrUL].effectStr =
          calloc(
              (amrHeapST[*numAmrUL].lenEffectUI + 1),
              sizeof(signed char)
          );

       if(! amrHeapST[*numAmrUL].effectStr)
          goto memErr_fun05_sec06_sub02;
       
       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun05 Sec05 Sub07 Cat02:
       +   - copy effect entry and remove spaces
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       cpLen_charCp(
          amrHeapST[*numAmrUL].effectStr,
          &buffHeapStr[uiPos],
          amrHeapST[*numAmrUL].lenEffectUI
       );

       /*convert spaces to dashes*/
       tmpStr = amrHeapST[*numAmrUL].effectStr;

       while(buffHeapStr[uiPos] > 31)
       { /*Loop: convert spaces to dashes*/
          *tmpStr += (-(*tmpStr == ' ') & 13);
          ++tmpStr;
          ++uiPos;
       } /*Loop: convert spaces to dashes*/

       ++uiPos; /*get off tab*/

       skipEffect_fun05_sec05_sub07_cat02:;

       /*************************************************\
       * Fun05 Sec05 Sub08:
       *   - get genomic coordinate if present.
       *     this will be updated twice if there is an
       *     genome corrdiante in the indicies tab, but
       *     also ensures all positions with coordinates
       *     are found
       \*************************************************/

       if(
             buffHeapStr[uiPos] > 47
          && buffHeapStr[uiPos] < 58
       ){ /*If: have genomic coordinate*/
          uiPos +=
             strToUI_base10str(
                &buffHeapStr[uiPos],
                &amrHeapST[*numAmrUL].refPosUI
             );

          /*convert coordinate to index 0*/
          --amrHeapST[*numAmrUL].refPosUI;
       } /*If: have genomic coordinate*/

       /*Else: is see genomic coordinates entry*/

       while(buffHeapStr[uiPos] != '\t')
       { /*Loop: move to next column*/
          if(buffHeapStr[uiPos] == '\0')
             break;

          ++uiPos;
       } /*Loop: move to next column*/

       ++uiPos; /*get off tab*/

       /*************************************************\
       * Fun05 Sec05 Sub09:
       *   - move to next set of targets to extract
       \*************************************************/

       for(
          siTab = 7;
          siTab < 105;
          ++siTab
       ){ /*Loop: get past columns I am ignoring*/
          while(buffHeapStr[uiPos] != '\t')
          { /*Loop: move to next column*/
             if(buffHeapStr[uiPos] == '\0')
                goto fileErr_fun05_sec06_sub03;

             ++uiPos;
          } /*Loop: move to next column*/

          ++uiPos; /*get off tab*/
       } /*Loop: get past columns I am ignoring*/

       /*************************************************\
       * Fun05 Sec05 Sub10:
       *   - check if AMR provides resitance
       \*************************************************/

       if(
             ! keepNonResBl
          && buffHeapStr[uiPos] != '1'
          && buffHeapStr[uiPos] != '2'
       ){ /*If: to low of grade to keep*/
          freeStack_amrST(&amrHeapST[*numAmrUL]);
          continue;
       } /*If: to low of grade to keep*/

       /*store grade*/
       amrHeapST[*numAmrUL].gradeSC =
          buffHeapStr[uiPos] - 48;

       while(buffHeapStr[uiPos] != '\t')
       { /*Loop: move to next column*/
          if(buffHeapStr[uiPos] == '\0')
             goto fileErr_fun05_sec06_sub03;

          ++uiPos;
       } /*Loop: move to next column*/

       ++uiPos; /*get off tab*/

       /*************************************************\
       * Fun05 Sec05 Sub11:
       *   - get comment entry
       *   o fun05 sec05 sub11 cat01:
       *     - allocate memory for comment entry
       *   o fun05 sec05 sub11 cat02:
       *     - check for delamind/penicillan myceil dreg
       *   o fun05 sec05 sub11 cat03:
       *     - copy comment and remove spaces
       *   o fun05 sec05 sub11 cat04:
       *     - check for high/low/additive resistance
       *   o fun05 sec05 sub11 cat05:
       *     - check for gene dependencies
       *   o fun05 sec05 sub11 cat06:
       *     - move off tab/to next entry
       \*************************************************/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun05 Sec05 Sub11 Cat01:
       +   - allocate memory for comment entry
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       amrHeapST[*numAmrUL].lenCommentUI = uiPos;

       while(
            buffHeapStr[amrHeapST[*numAmrUL].lenCommentUI]
          != '\t'
       ){ /*Loop: get comment length*/
          if(
            buffHeapStr[amrHeapST[*numAmrUL].lenCommentUI]
            == '\0'
          ) goto fileErr_fun05_sec06_sub03;

          ++amrHeapST[*numAmrUL].lenCommentUI;
       } /*Loop: get comment length*/

       amrHeapST[*numAmrUL].lenCommentUI -= uiPos;
       
       if(amrHeapST[*numAmrUL].lenCommentUI < 1)
          goto skipComment_fun05_sec05_sub11_cat06;

       amrHeapST[*numAmrUL].commentStr =
          calloc(
             (amrHeapST[*numAmrUL].lenCommentUI + 3),
             sizeof(signed char)
          );

       if(! amrHeapST[*numAmrUL].commentStr)
          goto memErr_fun05_sec06_sub02;

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun05 Sec05 Sub11 Cat02:
       +   - check for delamind/penicillan myceil dreg res
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       /*this is rare, so more effecient to do 2 checks*/
       if(
         ! eql_charCp(
           (signed char *)
              "Confers DLM-PMD cross-resistance\t",
           &buffHeapStr[uiPos],
           '\t'
         )
       ){ /*If: is an antibiotic note*/
          crossRes_whoToAmr(
             &buffHeapStr[uiPos],
             &amrHeapST[*numAmrUL],
             drugAryStr,
             numDrugsSI,
             &maxDrugsSI
          );

          if(! *drugAryStr)
             goto memErr_fun05_sec06_sub02;
       }/*If: this is an antibiotic note*/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun05 Sec05 Sub11 Cat03:
       +   - copy comment and remove spaces
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       cpLen_ulCp(
          amrHeapST[*numAmrUL].commentStr,
          &buffHeapStr[uiPos],
          amrHeapST[*numAmrUL].lenCommentUI
       );

       /*Convert spaces to dashes*/
       tmpStr = amrHeapST[*numAmrUL].commentStr;

       while(buffHeapStr[uiPos] > 31)
       { /*Loop: Convert spaces (32) to dashes*/
          *tmpStr += (-(*tmpStr == 32) & 13);
          ++tmpStr;
          ++uiPos;
       } /*Loop: Convert spaces (32) to dashes*/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun05 Sec05 Sub11 Cat04:
       +   - check for high/low/additive resistance
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       tmpStr = amrHeapST[*numAmrUL].commentStr;

       /*process the possible elements in the comment*/
       if(
          strstr((char *) tmpStr, "High-level-resistance")
       ) amrHeapST[*numAmrUL].highResBl = 1;

       if(strstr((char *) tmpStr, "Low-level-resistance"))
          amrHeapST[*numAmrUL].lowResBl = 1;

       if(
          strstr(
            (char *) tmpStr,
            "multiple,-genetically-linked-low-level-resistance-mutations-are-additive-and-confer-high-level-resistance"
          )
       ) amrHeapST[*numAmrUL].additiveResBl = 1;

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun05 Sec05 Sub11 Cat05:
       +   - check for gene dependencies
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       tmpStr =
          (signed char *)
          strstr(
             (char *) tmpStr,
             "Can-only-confer-resistance-if-genetically-linked-to-a-functional"
       ); /*in 2023 catalog, only 1 gene*/

       if(tmpStr)
       { /*If: gene dependency*/
          tmpStr += 65;

          if(*tmpStr < 32)
             goto noNeededGene_fun05_sec05_sub11_cat05;

          amrHeapST[*numAmrUL].lenNeedsGeneUI =
             lenStr_charCp(
                tmpStr,
                '\0'
             );

          amrHeapST[*numAmrUL].needsGeneStr =
             calloc(
               (amrHeapST[*numAmrUL].lenNeedsGeneUI + 1),
               sizeof(signed char)
             );

          if(! amrHeapST[*numAmrUL].needsGeneStr)
             goto memErr_fun05_sec06_sub02;

          cpLen_charCp(
             amrHeapST[*numAmrUL].needsGeneStr,
             tmpStr,
             amrHeapST[*numAmrUL].lenNeedsGeneUI
          );
       } /*If: gene dependency*/

       else
       { /*Else: there is no gene entry*/
          noNeededGene_fun05_sec05_sub11_cat05:;

          amrHeapST[*numAmrUL].lenNeedsGeneUI = 2;

          amrHeapST[*numAmrUL].needsGeneStr =
             malloc(3 * sizeof(signed char));

          if(! amrHeapST[*numAmrUL].needsGeneStr)
             goto memErr_fun05_sec06_sub02;
         
          amrHeapST[*numAmrUL].needsGeneStr[0] = 'N';
          amrHeapST[*numAmrUL].needsGeneStr[1] = 'A';
          amrHeapST[*numAmrUL].needsGeneStr[2] = '\0';
       } /*Else: there is no gene entry*/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun05 Sec05 Sub11 Cat06:
       +   - move off tab/to next entry
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       if(buffHeapStr[uiPos] != '\t')
          goto fileErr_fun05_sec06_sub03;

       ++uiPos; /*get off tab*/

       skipComment_fun05_sec05_sub11_cat06:;

       /*************************************************\
       * Fun05 Sec05 Sub12:
       *   - check for cross resistance (additional grade)
       \*************************************************/

       for(
          siTab = 0;
          siTab < 4;
          ++siTab
       ){ /*Loop: get past columns I am ignoring*/
          while(buffHeapStr[uiPos] != '\t')
          { /*Loop: move to next column*/
             if(buffHeapStr[uiPos] == '\0')
                goto fileErr_fun05_sec06_sub03;

             ++uiPos;
          } /*Loop: move to next column*/

          ++uiPos; /*get off tab*/
       } /*Loop: get past columns I am ignoring*/

       crossRes_whoToAmr(
          &buffHeapStr[uiPos],
          &amrHeapST[*numAmrUL],
          drugAryStr,
          numDrugsSI,
          &maxDrugsSI
       );

       if(! *drugAryStr)
          goto  memErr_fun05_sec06_sub02;

       /*************************************************\
       * Fun05 Sec05 Sub13:
       *   - deal with genomic coordiantes
       *   o fun05 sec05 sub13 cat01:
       *     - find matching gene indice(s)
       *   o fun05 sec05 sub13 cat02:
       *     - check if have another amr/start loop
       *   o fun05 sec05 sub13 cat03:
       *     - in next amr, copy gene id
       *   o fun05 sec05 sub13 cat04:
       *     - in next amr, copy variant id
       *   o fun05 sec05 sub13 cat05:
       *     - in next amr, copy effect entry
       *   o fun05 sec05 sub13 cat06:
       *     - in next amr, copy the comment entry
       *   o fun05 sec05 sub13 cat07:
       *     - get reference postion
       *   o fun05 sec05 sub13 cat08:
       *     - copy reference sequence
       *   o fun05 sec05 sub13 cat09:
       *     - copy amr sequence
       *   o fun05 sec05 sub13 cat10:
       *     - check amr mutation type
       *   o fun05 sec05 sub13 cat11:
       *     - move to next gene indice/dup amr
       \*************************************************/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun05 Sec05 Sub13 Cat01:
       +   - find matching gene indice(s)
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       indexSI =
          findVariant_genIndice(
             indiceHeapAryST,
             amrHeapST[*numAmrUL].varIdStr,
             (signed int) lenPosAryUI
          ); /*find the variant's gene indice*/

       if(indexSI < 0)
       { /*If: no genome coordinates*/
          if(rmFullGeneVarBl & (indexSI < 0))
          { /*If: discarding no genome coord entries*/
             fprintf(
                 stderr,
                 "%s not found in indice sheet%s",
                 amrHeapST[*numAmrUL].varIdStr,
                 str_endLine
             ); /*let user know it is missing*/

             freeStack_amrST(&amrHeapST[*numAmrUL]);
          } /*If: discarding no genome coord entries*/

          /*assign genome coordanites at end of fun08*/
          else
          { /*Else: keeping these entries*/
             uiPos = amrHeapST[*numAmrUL].lenVarIdUI;
             tmpStr = amrHeapST[*numAmrUL].varIdStr;

             if(
                   (tmpStr[uiPos - 3] & (~32)) == 'L'
                && (tmpStr[uiPos - 2] & (~32)) == 'O'
                && (tmpStr[uiPos - 1] & (~32)) == 'F'
             ){ /*If: loss of function*/
                amrHeapST[*numAmrUL].mutTypeStr[0] = 'l';
                amrHeapST[*numAmrUL].mutTypeStr[1] = 'o';
                amrHeapST[*numAmrUL].mutTypeStr[2] = 'f';

                amrHeapST[*numAmrUL].wholeGeneFlagSC =
                   def_geneFrameshift_amrST;

                amrHeapST[*numAmrUL].frameshiftBl = 1;
             } /*If: loss of function*/

             else if(
                   (tmpStr[uiPos - 8] & (~32)) == 'D'
                && (tmpStr[uiPos - 7] & (~32)) == 'E'
                && (tmpStr[uiPos - 6] & (~32)) == 'L'
                && (tmpStr[uiPos - 5] & (~32)) == 'E'
                && (tmpStr[uiPos - 4] & (~32)) == 'T'
                && (tmpStr[uiPos - 3] & (~32)) == 'I'
                && (tmpStr[uiPos - 2] & (~32)) == 'O'
                && (tmpStr[uiPos - 1] & (~32)) == 'N'
             ){ /*Else If: deletion*/
                amrHeapST[*numAmrUL].mutTypeStr[0] = 'd';
                amrHeapST[*numAmrUL].mutTypeStr[1] = 'e';
                amrHeapST[*numAmrUL].mutTypeStr[2] = 'l';

                amrHeapST[*numAmrUL].wholeGeneFlagSC =
                   def_geneDel_amrST;
             } /*Else If: deletion*/

             /*no idea what type of event it is*/
             else
                amrHeapST[*numAmrUL].unknownBl = 1;

             /*need to mark the sequence as blank*/
             amrHeapST[*numAmrUL].refSeqStr =
                malloc(2 * sizeof(signed char));
             
             if(! amrHeapST[*numAmrUL].refSeqStr)
                goto memErr_fun05_sec06_sub02;

             amrHeapST[*numAmrUL].refSeqStr[0] = '0';
             amrHeapST[*numAmrUL].refSeqStr[1] = '\0';

             amrHeapST[*numAmrUL].amrSeqStr =
                malloc(2 * sizeof(signed char));
             
             if(! amrHeapST[*numAmrUL].amrSeqStr)
                goto memErr_fun05_sec06_sub02;

             amrHeapST[*numAmrUL].amrSeqStr[0] = '0';
             amrHeapST[*numAmrUL].amrSeqStr[1] = '\0';

             ++(*numAmrUL);
          } /*Else: keeping these entries*/

          continue;
       } /*If: no genome coordinates*/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun05 Sec05 Sub13 Cat02:
       +   - Check if I have another amr/start loop
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       cpAmrBl = 1;

       while(cpAmrBl)
       { /*Loop: copy needed coordiantes*/

          /*no other gene Index entries*/
          if(indexSI + 1 >= (signed int) lenPosAryUI)
          { /*If: at the end of the genome indicies*/
             cpAmrBl = 0;
             goto cpRef_fun05_sec05_sub13_cat07;
          } /*If: at the end of the genome indicies*/

          cpAmrBl =
             ! eql_charCp(
                indiceHeapAryST[indexSI + 1].varIdStr,
                amrHeapST[*numAmrUL].varIdStr,
                '\0'
             ); /*Compare strings*/

          if(cpAmrBl)
          { /*If: have multiple amrs for this entry*/
             init_amrST(&amrHeapST[*numAmrUL + 1]);

             amrHeapST[*numAmrUL + 1].crossResFlagsUL[0] = 
                amrHeapST[*numAmrUL].crossResFlagsUL[0];

             amrHeapST[*numAmrUL + 1].amrFlagsUL[0] = 
                amrHeapST[*numAmrUL].amrFlagsUL[0];

             amrHeapST[*numAmrUL + 1].gradeSC =
                amrHeapST[*numAmrUL].gradeSC;

             /*++++++++++++++++++++++++++++++++++++++++++\
             + Fun05 Sec05 Sub13 Cat03:
             +   - in next amr, copy the gene id
             \++++++++++++++++++++++++++++++++++++++++++*/

             amrHeapST[*numAmrUL + 1].geneIdStr = 
               calloc(
                 (amrHeapST[*numAmrUL].lenGeneIdUI + 1),
                 sizeof(signed char)
               );

             if(! amrHeapST[*numAmrUL + 1].geneIdStr)
                goto memErr_fun05_sec06_sub02;

             amrHeapST[*numAmrUL + 1].lenGeneIdUI = 
                amrHeapST[*numAmrUL].lenGeneIdUI;

             cpLen_charCp(
                amrHeapST[*numAmrUL + 1].geneIdStr,
                amrHeapST[*numAmrUL].geneIdStr,
                amrHeapST[*numAmrUL].lenGeneIdUI
             );

             /*++++++++++++++++++++++++++++++++++++++++++\
             + Fun05 Sec05 Sub13 Cat04:
             +   - in next amr, copy variant id
             \++++++++++++++++++++++++++++++++++++++++++*/

             amrHeapST[*numAmrUL + 1].varIdStr = 
               calloc(
                 (amrHeapST[*numAmrUL].lenVarIdUI + 1),
                 sizeof(signed char)
               );

             if(! amrHeapST[*numAmrUL + 1].varIdStr)
                goto memErr_fun05_sec06_sub02;

             amrHeapST[*numAmrUL + 1].lenVarIdUI = 
                amrHeapST[*numAmrUL].lenVarIdUI;

             cpLen_charCp(
                amrHeapST[*numAmrUL + 1].varIdStr,
                amrHeapST[*numAmrUL].varIdStr,
                amrHeapST[*numAmrUL].lenVarIdUI
             );

             /*++++++++++++++++++++++++++++++++++++++++++\
             + Fun05 Sec05 Sub13 Cat05:
             +   - in next amr, copy effect entry
             \++++++++++++++++++++++++++++++++++++++++++*/

             amrHeapST[*numAmrUL + 1].effectStr = 
               calloc(
                  (amrHeapST[*numAmrUL].lenEffectUI + 1),
                  sizeof(signed char)
               );

             if(! amrHeapST[*numAmrUL + 1].effectStr)
                goto memErr_fun05_sec06_sub02;

             amrHeapST[*numAmrUL + 1].lenEffectUI = 
                amrHeapST[*numAmrUL].lenEffectUI;

             cpLen_charCp(
                amrHeapST[*numAmrUL + 1].effectStr,
                amrHeapST[*numAmrUL].effectStr,
                amrHeapST[*numAmrUL].lenEffectUI
             );

             /*++++++++++++++++++++++++++++++++++++++++++\
             + Fun05 Sec05 Sub13 Cat06:
             +   - in next amr, copy the comment entry
             \++++++++++++++++++++++++++++++++++++++++++*/

             amrHeapST[*numAmrUL + 1].lenCommentUI = 
                amrHeapST[*numAmrUL].lenCommentUI;

             amrHeapST[*numAmrUL + 1].commentStr = 
               calloc(
                  (amrHeapST[*numAmrUL].lenCommentUI + 1),
                  sizeof(signed char)
               );

             if(! amrHeapST[*numAmrUL + 1].commentStr)
                goto memErr_fun05_sec06_sub02;

             cpLen_charCp(
                amrHeapST[*numAmrUL + 1].commentStr,
                amrHeapST[*numAmrUL].commentStr,
                amrHeapST[*numAmrUL].lenCommentUI
             );

             /*copy the booleans for the comment*/
             amrHeapST[*numAmrUL + 1].highResBl = 
                amrHeapST[*numAmrUL].highResBl;

             amrHeapST[*numAmrUL + 1].lowResBl = 
                amrHeapST[*numAmrUL].lowResBl;

             amrHeapST[*numAmrUL + 1].additiveResBl = 
                amrHeapST[*numAmrUL].additiveResBl;

             /*deal with needed gene entry*/
             amrHeapST[*numAmrUL + 1].lenNeedsGeneUI = 
                amrHeapST[*numAmrUL].lenNeedsGeneUI;

             amrHeapST[*numAmrUL + 1].needsGeneStr = 
               calloc(
                 (amrHeapST[*numAmrUL].lenNeedsGeneUI+1),
                 sizeof(signed char)
               );

             if(! amrHeapST[*numAmrUL + 1].needsGeneStr)
                goto memErr_fun05_sec06_sub02;

             cpLen_charCp(
                amrHeapST[*numAmrUL + 1].needsGeneStr,
                amrHeapST[*numAmrUL].needsGeneStr,
                amrHeapST[*numAmrUL].lenNeedsGeneUI
             );
          } /*If: have multiple amrs for this entry*/

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun05 Sec05 Sub13 Cat07:
          +   - copy reference position
          \+++++++++++++++++++++++++++++++++++++++++++++*/

           cpRef_fun05_sec05_sub13_cat07:;

           amrHeapST[*numAmrUL].refPosUI =
             indiceHeapAryST[indexSI].posUI;

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun05 Sec05 Sub13 Cat08:
          +   - copy reference sequence
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          amrHeapST[*numAmrUL].refSeqStr = 
            calloc(
              (indiceHeapAryST[indexSI].lenRefSeqUI + 1),
              sizeof(signed char)
            );

          if(! amrHeapST[*numAmrUL].refSeqStr)
             goto memErr_fun05_sec06_sub02;

          amrHeapST[*numAmrUL].lenRefSeqUI = 
             indiceHeapAryST[indexSI].lenRefSeqUI;

          cpLen_charCp(
             amrHeapST[*numAmrUL].refSeqStr,
             indiceHeapAryST[indexSI].refSeqStr,
             amrHeapST[*numAmrUL].lenRefSeqUI
          );

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun05 Sec05 Sub13 Cat09:
          +   - copy amr sequence
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          amrHeapST[*numAmrUL].amrSeqStr = 
            calloc(
              (indiceHeapAryST[indexSI].lenAmrSeqUI + 1),
              sizeof(signed char)
            );

          if(! amrHeapST[*numAmrUL].amrSeqStr)
             goto memErr_fun05_sec06_sub02;

          amrHeapST[*numAmrUL].lenAmrSeqUI = 
             indiceHeapAryST[indexSI].lenAmrSeqUI;

          cpLen_charCp(
             amrHeapST[*numAmrUL].amrSeqStr,
             indiceHeapAryST[indexSI].amrSeqStr,
             amrHeapST[*numAmrUL].lenAmrSeqUI
          ); /*Copy the amrerence sequence*/

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun05 Sec05 Sub13 Cat10:
          +   - check amr mutation type
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          uiPos = amrHeapST[*numAmrUL].lenVarIdUI;
          tmpStr = amrHeapST[*numAmrUL].varIdStr;

          /*removing, since varaint ID has information*/
          /*if(
                 (tmpStr[uiPos - 3] & (~32)) == 'L'
             && (tmpStr[uiPos - 2] & (~32)) == 'O'
             && (tmpStr[uiPos - 1] & (~32)) == 'F'
          ){*/ /*If: loss of function mutation*/
          /*
             amrHeapST[*numAmrUL].mutTypeStr[0] = 'l';
             amrHeapST[*numAmrUL].mutTypeStr[1] = 'o';
             amrHeapST[*numAmrUL].mutTypeStr[2] = 'f';
          }*/ /*If: loss of function mutation*/

          if(
               amrHeapST[*numAmrUL].lenRefSeqUI
             > amrHeapST[*numAmrUL].lenAmrSeqUI
          ){ /*Else If: deletion*/
             amrHeapST[*numAmrUL].mutTypeStr[0] = 'd';
             amrHeapST[*numAmrUL].mutTypeStr[1] = 'e';
             amrHeapST[*numAmrUL].mutTypeStr[2] = 'l';
          } /*Else If: deletion*/

          else if(
               amrHeapST[*numAmrUL].lenRefSeqUI
             < amrHeapST[*numAmrUL].lenAmrSeqUI
          ){ /*Else If: insertion*/
             amrHeapST[*numAmrUL].mutTypeStr[0] = 'i';
             amrHeapST[*numAmrUL].mutTypeStr[1] = 'n';
             amrHeapST[*numAmrUL].mutTypeStr[2] = 's';
          } /*Else If: insertion*/

          else
          { /*Else: snp*/
             amrHeapST[*numAmrUL].mutTypeStr[0] = 's';
             amrHeapST[*numAmrUL].mutTypeStr[1] = 'n';
             amrHeapST[*numAmrUL].mutTypeStr[2] = 'p';
          } /*Else: snp*/

          amrHeapST[*numAmrUL].mutTypeStr[3] = '\0';

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun05 Sec05 Sub13 Cat11:
          +   - move to next gene indice/dup amr
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          *numAmrUL += cpAmrBl;
          ++indexSI;
       } /*Loop: copy needed coordiantes*/

       /*************************************************\
       * Fun05 Sec05 Sub14:
       *   - move to next amr entry
       \*************************************************/

       ++(*numAmrUL);
    } /*Loop: get AMRs from catalog*/

    /****************************************************\
    * Fun05 Sec05 Sub15:
    *   - sort AMRs by position (after read in loop)
    \****************************************************/

    sortPos_amrST(amrHeapST, 0, *numAmrUL - 1);
    ++(*numDrugsSI); /*Convert to index 1*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun05 Sec06:
    ^   - Clean up
    ^   o fun05 sec06 sub01:
    ^     - no error clean up
    ^   o fun05 sec06 sub02:
    ^     - memory error clean up
    ^   o fun05 sec06 sub03:
    ^     - file error clean up
    ^   o fun05 sec06 sub04:
    ^     - error clean up (all errors call)
    ^   o fun05 sec06 sub05:
    ^     - general clean up (everything calls)
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun05 Sec06 Sub01:
    *   - no error clean up
    \****************************************************/
 
    *errSC = 0;
    goto cleanUp_fun05_Sec06_sub05;

    /****************************************************\
    * Fun05 Sec06 Sub02:
    *   - memory error clean up
    \****************************************************/

    memErr_fun05_sec06_sub02:;
    *errSC = def_memErr_amrST;
    goto errCleanUp_fun05_sec06_sub04;

    /****************************************************\
    * Fun05 Sec06 Sub03:
    *   - file error clean up
    \****************************************************/

    fileErr_fun05_sec06_sub03:;
    *errSC = def_fileErr_amrST;
    goto errCleanUp_fun05_sec06_sub04;

    /****************************************************\
    * Fun05 Sec06 Sub03:
    *   - error clean up (all errors call)
    \****************************************************/

    errCleanUp_fun05_sec06_sub04:;

    if(*drugAryStr)
       free(*drugAryStr);

    *drugAryStr = 0;

    if(amrHeapST)
       freeHeapAry_amrST(
          amrHeapST,
          *numAmrUL
       );

    amrHeapST = 0;

    goto cleanUp_fun05_Sec06_sub05;

    /****************************************************\
    * Fun05 Sec06 Sub05:
    *   - general clean up (everything calls)
    \****************************************************/

    cleanUp_fun05_Sec06_sub05:;

    if(buffHeapStr)
       free(buffHeapStr);

    buffHeapStr = 0;

    if(
          whoFILE
       && whoFILE != stdin
       && whoFILE != stdout
    ) fclose(whoFILE);

    whoFILE = 0;

    if(indiceHeapAryST)
       freeHeapAry_genIndice(
          indiceHeapAryST,
          lenPosAryUI
       );

    indiceHeapAryST = 0;

    return amrHeapST;
} /*WHO2023Catalog_whoToAmr*/

/*-------------------------------------------------------\
| Fun06: parseVarId_whoToAmr
|   - parse variant idea from the WHO 2023 TB catalog
| Input:
|   - amrST:
|     - pointer to array of amrST structures to update and
|       process variants for
|   - numAmrSI:
|     - number of amrSTs in amrST
| Output:
|   - Modifies:
|     o each structure in amrST with amino acid mutations
|       to have the amino acid sequence whe possible. for
|       large deletions/duplications you will need to call
|       addCodonPos_whoToAmr
|   - Returns
|     o 0 for success
|     o def_memErr_amrST for a memory error
\-------------------------------------------------------*/
signed char
parseVarId_whoToAmr(
   struct amrST *amrSTPtr, /*has amr variants to update*/
   signed int numAmrSI     /*number of amrs*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '   - parse variant idea from the WHO 2023 TB catalog
   '   o fun06 sec01:
   '     - variable declerations
   '   o fun06 sec02:
   '     - process variant id for amino acid entries
   '   o fun06 sec03:
   '     - clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;    /*for reporting errors*/

   signed char firstAaSC = 0;
   signed char secAaSC = 0;
   signed char *varStr = 0;
   signed char *cpStr = 0;
   signed int siIndex = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - process variant id for amino acid entries
   ^   o fun06 sec02 sub01:
   ^     - start loop (all AMR(s)) and check if I have an
   ^       amino acid variant to process
   ^   o fun06 sec02 sub02:
   ^     - get first amino acid from variant id
   ^   o fun06 sec02 sub03:
   ^     - get codon number of first amio acid
   ^   o fun06 sec02 sub04:
   ^     - get second amino acid/mutation type for
   ^       variants that are not deletions/insertions
   ^   o fun06 sec02 sub05:
   ^     - handle deletions/insertions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec02 Sub01: who2023ParsVar
   *   - start loop (all AMR(s)) and check if I have an
   *     amino acid variant to process
   \*****************************************************/

   for(
      siIndex = 0;
      siIndex < numAmrSI;
      ++siIndex
   ){ /*Loop: process each variant*/
     varStr = amrSTPtr[siIndex].varIdStr;

     if(varStr[1] != '.')
     { /*If: includes gene name (2023 catalog)*/
        while(
              *varStr !='.'
           && *varStr > 32
        ) ++varStr;

        --varStr;
     } /*If: includes gene name (2023 catalog)*/

     if(
           varStr[0] != '.'
        || (varStr[1] & ~32) != 'P'
     ){ /*If: not a 2023 catalog entry*/

        if((varStr[0] & ~32) != 'P' || varStr[1] != '.')
        { /*If: not aa mutation*/

           if(
                 (*(varStr - 0) & ~32) == 'F'
              && (*(varStr - 1) & ~32) == 'O'
              && (*(varStr - 2) & ~32) == 'L'
              && (*(varStr - 3)) == '_'
           ){ /*If: gene loss of function entry*/
               amrSTPtr[siIndex].wholeGeneFlagSC |=
                  def_geneFrameshift_amrST;
           } /*If: gene loss of function entry*/

           continue; /*This not an aa mutation*/
        } /*If: not an aa mutation*/
     } /*If: not a 2023 catalog entry*/

     varStr += 2; /*+2 is to get off '.p'*/
     
     /***************************************************\
     * Fun06 Sec02 Sub02:
     *   - get first amino acid from variant id
     \***************************************************/

     if(*varStr == '*')
     { /*If: stop codon*/
        firstAaSC = '*';
        ++varStr;
     } /*If: stop codon*/

     else if(
           (varStr[0] & ~32) == 'T'
        && (varStr[1] & ~32) == 'E'
        && (varStr[2] & ~32) == 'R'
     ){ /*If: stop codon*/
        firstAaSC = '*';
        varStr += 3;
     } /*If stop codn*/

     else 
     { /*Else: not a stop codon*/
        firstAaSC = aaTripToChar_codonFun(varStr);
        varStr += 3;
     } /*Else: not a stop codon*/

     /***************************************************\
     * Fun06 Sec02 Sub03: who2023ParsVar
     *   - get codon number of first amio acid
     \***************************************************/

     varStr +=
        strToUI_base10str(
           varStr,
           &amrSTPtr[siIndex].codonNumUI
        );

     /***************************************************\
     * Fun06 Sec02 Sub04: who2023ParsVar
     *   - get second amino acid/mutation type for
     *     variants that are not deletions/insertions
     *   o fun06 sec02 sub04 cat01:
     *     - any amino acid (first aa)
     *   o fun06 sec02 sub04 cat02:
     *     - stop codon (first aa)
     *   o fun06 sec02 sub04 cat03:
     *     - frame shift (frist aa)
     *   o fun06 sec02 sub04 cat04:
     *     - deletion (frist aa)
     *   o fun06 sec02 sub04 cat05:
     *     - singe amino acid duplication (first aa)
     *   o fun06 sec02 sub04 cat06:
     *     - single amino acid mutation (first/second aa)
     \***************************************************/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub04 Cat01:
     +   - any amino acid (first aa)
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     if(varStr[0] == '?')
     { /*If: could be any aa change*/
        secAaSC = '?';

        amrSTPtr[siIndex].endCodonNumUI =
           amrSTPtr[siIndex].codonNumUI;

        if(
            addOneAa_whoToAmr(
               &amrSTPtr[siIndex],
               firstAaSC,
               '?'
        )) goto memErr_fun06_sec03;

        continue;
     } /*If: could be any aa change*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub04 Cat02:
     +   - stop codon (first aa)
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/
     
     else if(varStr[0] == '*')
     { /*If: stop*/
        secAaSC = '*';

        amrSTPtr[siIndex].endCodonNumUI =
           amrSTPtr[siIndex].codonNumUI;

        if(
           addOneAa_whoToAmr(
              &amrSTPtr[siIndex],
              firstAaSC,
              '*'
        )) goto memErr_fun06_sec03;

        continue;
     } /*If: stop*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub04 Cat03:
     +   - frame shift (frist aa)
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     else if(
           (varStr[0] & ~32) == 'F'
        && (varStr[1] & ~32) == 'S' 
     ){ /*Else If: this was a frame shift*/
        amrSTPtr[siIndex].frameshiftBl = 1;
        amrSTPtr[siIndex].lenRefAaUI = 1;

        amrSTPtr[siIndex].refAaStr =
           malloc(2 * sizeof(signed char));

        if(! amrSTPtr[siIndex].refAaStr)
           goto memErr_fun06_sec03;

        amrSTPtr[siIndex].refAaStr[0] = firstAaSC;
        amrSTPtr[siIndex].refAaStr[1] = '\0';

        /*no amr amino acid, because it is a frame shift*/

        amrSTPtr[siIndex].endCodonNumUI =
           amrSTPtr[siIndex].codonNumUI;

        continue;
     } /*Else If: this was a frame shift*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub04 Cat04:
     +   - deletion (frist aa)
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     else if(
           (varStr[0] & ~32) == 'D'
        && (varStr[1] & ~32) == 'E'
        && (varStr[2] & ~32) == 'L'
     ) goto parseDel_fun06_sec02_sub05_cat02;

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub04 Cat05:
     +   - singe amino acid duplication (first aa)
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     else if(
           (varStr[0] & ~32) == 'D'
        && (varStr[1] & ~32) == 'U'
        && (varStr[2] & ~32) == 'P'
     ){ /*Else If: one aa duplication*/
        amrSTPtr[siIndex].lenRefAaUI = 1;

        amrSTPtr[siIndex].refAaStr =
           malloc(2 * sizeof(signed char));

        if(! amrSTPtr[siIndex].refAaStr)
           return def_memErr_amrST;

        amrSTPtr[siIndex].refAaStr[0] = firstAaSC;
        amrSTPtr[siIndex].refAaStr[1] = '\0';

        amrSTPtr[siIndex].lenAmrAaUI = 2;

        amrSTPtr[siIndex].amrAaStr =
           malloc(3 * sizeof(signed char));

        if(!amrSTPtr[siIndex].amrAaStr)
           goto memErr_fun06_sec03;

        amrSTPtr[siIndex].amrAaStr[0] = firstAaSC;
        amrSTPtr[siIndex].amrAaStr[1] = firstAaSC;
        amrSTPtr[siIndex].amrAaStr[2] = '\0';

        amrSTPtr[siIndex].endCodonNumUI =
           amrSTPtr[siIndex].codonNumUI;

        continue;
     } /*Else If: one aa duplication*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub04 Cat06:
     +   - single amino acid mutation (first/second aa)
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     else if(varStr[0] != '_')
     { /*Else If: for 2023 this is a single change*/

        /*check if is a lost stop codon*/
        if(
           ! eql_charCp(
              (signed char *) "ext*?\0",
              varStr,
              '\0'
        )) secAaSC = '?';

        else if(varStr[1] == '\0')
           secAaSC = varStr[0]; /*one letter*/

        /*three letter amino acid code*/
        else
           secAaSC = aaTripToChar_codonFun(varStr);

        if(
           addOneAa_whoToAmr(
              &amrSTPtr[siIndex],
              firstAaSC,
              secAaSC
        )) goto memErr_fun06_sec03;
 
        amrSTPtr[siIndex].endCodonNumUI =
           amrSTPtr[siIndex].codonNumUI;

        continue;
     } /*Else If: for 2023 this is a single change*/

     /***************************************************\
     * Fun06 Sec02 Sub05:
     *   - handle deletions/insertions
     *   o fun06 sec02 sub05 cat01:
     *     - find amino acid at end of eletion/insertion
     *   o fun06 sec02 sub05 cat02:
     *     - find position of the last amino acid
     *   o fun06 sec02 sub05 cat03:
     *     - insertion only case; add start/end aa to ref
     *   o fun06 sec02 sub05 cat04:
     *     - ins case; add start/end aa to amrSeqStr
     *   o fun06 sec02 sub05 cat05:
     *     - ins case; add in the inserted aa sequence
     *   o fun06 sec02 sub05 cat06:
     *     - handle large duplications (> 1 amino acid)
     *   o fun06 sec02 sub05 cat07:
     *     - deletion; add first and last aa to refAaStr
     *   o fun06 sec02 sub05 cat08:
     *     - del only; add mutate aa sequence to amrAaStr
     *   o fun06 sec02 sub05 cat09:
     *     - del; update amino acid sequence in amrAaStr
     \***************************************************/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub05 Cat01:
     +   - find amino acid at end of deletion/insertion
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     ++varStr; /*get off '_'*/
     
     /*not likely to happen, but want to make sure*/
     if(
           varStr[0] == '*'
        || varStr[0] == '?'
     ){ /*If: could be any aa change*/
        secAaSC = varStr[0];
        ++varStr;
     } /*If: could be any aa change*/

     else
     { /*Else: has three letter code*/
        secAaSC = aaTripToChar_codonFun(varStr);
        varStr += 3;
     } /*Else: has three letter code*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub05 Cat02:
     +   - find position of last amino acid
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     /*this adds some extra checks, but also makes sure
     `   the end codon number is set
     */
     parseDel_fun06_sec02_sub05_cat02:;

     /*last codon in deletion/insertion*/
     cpStr = varStr;

     cpStr +=
       strToUI_base10str(
          varStr,
          &amrSTPtr[siIndex].endCodonNumUI
       );

     if(cpStr == varStr)
     { /*If: no end codon number*/
        amrSTPtr[siIndex].endCodonNumUI =
           amrSTPtr[siIndex].codonNumUI;
     } /*If: no end codon number*/

     varStr = cpStr;

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub05 Cat03:
     +   - insertion only case; add start/end aa to ref
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     if(
           (varStr[0] & ~32) == 'I'
        && (varStr[1] & ~32) == 'N'
        && (varStr[2] & ~32) == 'S'
     ){ /*If: insertion*/
        varStr += 3; /*get off ins*/

        /*copy reference amino acids*/
        amrSTPtr[siIndex].lenRefAaUI = 2;

        amrSTPtr[siIndex].refAaStr =
           malloc(3 * sizeof(signed char));

        if(! amrSTPtr[siIndex].refAaStr)
           goto memErr_fun06_sec03;

        amrSTPtr[siIndex].refAaStr[0] = firstAaSC;
        amrSTPtr[siIndex].refAaStr[1] = secAaSC;
        amrSTPtr[siIndex].refAaStr[2] = '\0';

        /*+++++++++++++++++++++++++++++++++++++++++++++++\
        + Fun06 Sec02 Sub05 Cat04:
        +   - ins case; add start/end aa to amrAaStr
        \+++++++++++++++++++++++++++++++++++++++++++++++*/

        /*copy amr amino acid sequence. will be an over
        ` estimate, but it works
        */

        cpStr = varStr;

        while(*cpStr != '\0')
        { /*Loop: get string length*/
           if(
                 *cpStr == '*'
              || *cpStr == '?'
           ) ++cpStr;     /*single character*/

           else
              cpStr += 3; /*multipe characters*/

           ++amrSTPtr[siIndex].lenAmrAaUI;
        } /*Loop: get string length*/

        amrSTPtr[siIndex].lenAmrAaUI += 2;

        amrSTPtr[siIndex].amrAaStr =
           calloc(
              (amrSTPtr[siIndex].lenAmrAaUI + 1),
              sizeof(signed char)
           );

        if(! amrSTPtr[siIndex].amrAaStr)
           goto memErr_fun06_sec03;

        /*add start/end amino acids to amr aa sequence*/
        amrSTPtr[siIndex].amrAaStr[0] = firstAaSC;

        amrSTPtr[siIndex].amrAaStr[
           amrSTPtr[siIndex].lenAmrAaUI - 1
        ] = secAaSC;

        amrSTPtr[siIndex].amrAaStr[
           amrSTPtr[siIndex].lenAmrAaUI
        ] = '\0';

        /*+++++++++++++++++++++++++++++++++++++++++++++++\
        + Fun06 Sec02 Sub05 Cat05:
        +   - ins case; Add in the inserted aa sequence
        \+++++++++++++++++++++++++++++++++++++++++++++++*/

        cpStr = &amrSTPtr[siIndex].amrAaStr[1];

        while(*varStr != '\0')
        { /*Loop: copy aa from varStr*/
           if(
                 *varStr == '*'
              || *varStr == '?'
           ) *cpStr++ = *varStr++;

           else
           { /*Else: three letter codon*/
              *cpStr++ = aaTripToChar_codonFun(varStr);
              varStr += 3;
           } /*Else: three letter codon*/
        } /*Loop: copy aa from varStr*/

        continue;
     } /*If: insertion*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub05 Cat06:
     +   - handle large duplications (> 1 amino acid)
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     if(   (varStr[0] & ~32) == 'D'
        && (varStr[1] & ~32) == 'U'
        && (varStr[2] & ~32) == 'P'
     ){ /*If: large duplication*/
        amrSTPtr[siIndex].aaMultiDupBl = 1;

        amrSTPtr[siIndex].lenRefAaUI = 
             amrSTPtr[siIndex].endCodonNumUI
           - amrSTPtr[siIndex].codonNumUI
           + 1; /*+1 to account for first aa*/

        amrSTPtr[siIndex].refAaStr =
           calloc(
              (amrSTPtr[siIndex].lenRefAaUI  + 1),
              sizeof(signed char)
           );

        if(! amrSTPtr[siIndex].refAaStr)
           goto memErr_fun06_sec03;

        amrSTPtr[siIndex].refAaStr[0] = firstAaSC;

        amrSTPtr[siIndex].refAaStr[
           amrSTPtr[siIndex].lenRefAaUI - 1
        ] = secAaSC;

        amrSTPtr[siIndex].refAaStr[
           amrSTPtr[siIndex].lenRefAaUI
        ] = '\0';

        /*set up the amr buffer*/

        amrSTPtr[siIndex].lenAmrAaUI = 
           amrSTPtr[siIndex].lenRefAaUI << 1;

        amrSTPtr[siIndex].amrAaStr =
           calloc(
              (amrSTPtr[siIndex].lenAmrAaUI  + 1),
              sizeof(signed char)
           );

        if(! amrSTPtr[siIndex].amrAaStr)
           goto memErr_fun06_sec03;

        continue; /*will handle aa in sam file step*/
     } /*If: large duplication*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub05 Cat07:
     +   - deletion; add first and last aa to refAaStr
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     varStr += 3; /*get off "del"*/

     /*find number of amino acids in reference*/
     /*reference length needs to be found*/
     amrSTPtr[siIndex].lenRefAaUI =
          1
        + ab_genMath(
               amrSTPtr[siIndex].endCodonNumUI
             - amrSTPtr[siIndex].codonNumUI
          ); /*get length of deletion*/

     amrSTPtr[siIndex].refAaStr =
        calloc(
           (amrSTPtr[siIndex].lenRefAaUI + 1),
           sizeof(signed char)
        );

     if(! amrSTPtr[siIndex].refAaStr)
        goto memErr_fun06_sec03;

     amrSTPtr[siIndex].refAaStr[0] = firstAaSC;

     if(secAaSC != 0)
     { /*If: range of deletions*/
        amrSTPtr[siIndex].refAaStr[
            amrSTPtr[siIndex].lenRefAaUI - 1
        ] = secAaSC;

        amrSTPtr[siIndex].refAaStr[
            amrSTPtr[siIndex].lenRefAaUI
        ] = '\0';
     } /*If: range of deletions*/

     else
        amrSTPtr[siIndex].refAaStr[2] = '\0';

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub05 Cat08:
     +   - del only; add mutate aa sequence to amrAaStr
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     /*get the amino acid sequece for the amr*/
     if(*varStr == '\0')
     { /*If: just a deletion*/
        /*copy amr amino acid sequence*/
        amrSTPtr[siIndex].lenAmrAaUI = 1;

        amrSTPtr[siIndex].amrAaStr =
           malloc(2 * sizeof(signed char));

        if(! amrSTPtr[siIndex].amrAaStr)
           goto memErr_fun06_sec03;

        amrSTPtr[siIndex].amrAaStr[0] = '0';
        amrSTPtr[siIndex].amrAaStr[1] = '\0';

        continue;
     } /*If: just a deletion*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun06 Sec02 Sub05 Cat09:
     +   - del; update amino acid sequence in amrAaStr
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     /*Get off the ins*/
     varStr += 3;
     cpStr = varStr;

     while(*cpStr != '\0')
     { /*Loop: get string length*/
        if(
              *cpStr == '*'
           || *cpStr == '?'
        ) ++cpStr;

        else
           cpStr += 3;

        ++amrSTPtr[siIndex].lenAmrAaUI;
     } /*Loop: get string length*/

     amrSTPtr[siIndex].amrAaStr =
        calloc(
           (amrSTPtr[siIndex].lenAmrAaUI + 1),
           sizeof(signed char)
        );

     if(! amrSTPtr[siIndex].amrAaStr)
        goto memErr_fun06_sec03;

     amrSTPtr[siIndex].amrAaStr[
        amrSTPtr[siIndex].lenAmrAaUI
     ] = '\0';

     /*add new insertion in*/
     cpStr = &amrSTPtr[siIndex].amrAaStr[0];

     while(*varStr != '\0')
     { /*Loop: copy aa from varStr*/
        if(*
              varStr == '*'
           || *varStr == '?'
        ) *cpStr++ = *varStr++;

        else
        { /*Else: three letter codon*/
           *cpStr++ = aaTripToChar_codonFun(varStr);
           varStr += 3;
        } /*Else: three letter codon*/
     } /*Loop: copy aa from varStr*/
   } /*Loop: process each variant*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun06_sec03;

   memErr_fun06_sec03:;
   errSC = def_memErr_amrST;
   goto ret_fun06_sec03;

   ret_fun06_sec03:;

   return errSC;
} /*parseVarId_whoToAmr*/

/*-------------------------------------------------------\
| Fun07: sortCoords_whoToAmr
|   - sorts the gene start, end, and direction arrays by
|     the starting coordiantes
| Input:
|   - startArySI:
|     o signed int array of gene starting coordinates
|   - endArySI:
|     o signed int array of gene ending coordinates
|   - idHeapAryStr:
|     o drugAry (c-string) array of gene ids to keep
|       sorted with coordiantes
|   - revAryBl:
|     o signed char array of gene 1 (reverse gene) and 0
|   - startSI:
|     o first index to sort
|   - endSI:
|     o last index to sort (index 0)
| Output:
|   - Modifies:
|     o startArySI to be sorted from least to greatest
|     o endArySI to be sorted by startArySI
|     o revAryBl to be sorted by startArySI
\-------------------------------------------------------*/
void
sortCoords_whoToAmr(
   signed int *startArySI,
   signed int *endArySI,
   signed char *idHeapAryStr,
   signed char *revAryBl,
   signed int startSI,
   signed int endSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - sorts signed char array from least to greatest
   '   o fun07 sec01:
   '     - variable declerations
   '   o fun07 sec02:
   '     - find the number of rounds to sort for
   '   o fun07 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*number of elements to sort*/
   signed int numElmSI = endSI - startSI;

   /*number of sorting rounds*/
   signed int subSI = 0;
   signed int nextSI = 0;
   signed int lastSI = 0;
   signed int onSI = 0;

   /*variables to incurment loops*/
   signed int siIndex = 0;
   signed int siElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Recursion formsia: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < numElmSI - 1)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSI > 0)
   { /*Loop: all rounds*/
      for(
         siIndex = 0;
         siIndex <= endSI;
         ++siIndex
      ){ /*Loop: though sub array*/
         siElm = siIndex;

         for(
            siElm = siIndex;
            siElm + subSI <= endSI;
            siElm += subSI
         ){ /*Loop: swap elements in subarray*/
            nextSI = siElm + subSI;

            if(startArySI[siElm] > startArySI[nextSI])
            { /*If I need to swap an element*/
               startArySI[siElm] ^= startArySI[nextSI];
               startArySI[nextSI] ^= startArySI[siElm];
               startArySI[siElm] ^= startArySI[nextSI];

               endArySI[siElm] ^= endArySI[nextSI];
               endArySI[nextSI] ^= endArySI[siElm];
               endArySI[siElm] ^= endArySI[nextSI];

               revAryBl[siElm] ^= revAryBl[nextSI];
               revAryBl[nextSI] ^= revAryBl[siElm];
               revAryBl[siElm] ^= revAryBl[nextSI];

               swap_drugAry(
                  idHeapAryStr,
                  siElm,
                  nextSI
               );

               lastSI = siElm;
               onSI = siElm;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;

                  if(startArySI[onSI] > startArySI[lastSI])
                     break; /*Positioned the element*/
   
                  startArySI[onSI] ^= startArySI[lastSI];
                  startArySI[lastSI] ^= startArySI[onSI];
                  startArySI[onSI] ^= startArySI[lastSI];

                  endArySI[onSI] ^= endArySI[lastSI];
                  endArySI[lastSI] ^= endArySI[onSI];
                  endArySI[onSI] ^= endArySI[lastSI];

                  revAryBl[onSI] ^= revAryBl[lastSI];
                  revAryBl[lastSI] ^= revAryBl[onSI];
                  revAryBl[onSI] ^= revAryBl[lastSI];

                  swap_drugAry(
                     idHeapAryStr,
                     onSI,
                     lastSI
                  );

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortCoords_whoToAmr*/

/*-------------------------------------------------------\
| Fun08: addCodonPos_whoToAmr
|   - adds amino acid sequences for deletions and
|     large duplications, reading frame orientation
|     (forward/reverse), and the first reference
|     base in the codon to an amrST that has been
|     processed with parseVarId_whoToAmr.
| Input:
|   - amrST:
|     o pointer to an array of amrST structs to update
|   - numAmrSI:
|     o number of amrSTs in amrST
|   - geneTblFileStr:
|     o c-string with the path to the gene coordinates
|       file with gene mappings 
|       - column 1: gene name (max of 64 characters)
|       - column 1: ignored (suggest reference id)
|       - column 3: + for foward gene; - for reverse gene
|       - column 4: frist mapped base in reference
|       - column 5: last mapped base in reference
|   - refFileStr:
|     o c-string with path to fasta file with reference
|       sequence (should only have one sequence)
| Output:
|   - Modifies:
|     o each structure in amrST with amino acid mutations
|       to have the amino acid sequence and starting
|       position of codon on the reference
|     o buffStr to be larger if needed
|     o lenBuffUL if buffStr is resized
|     o samFILE to point to the end of the file
|   - Returns
|     o 0 for success
|     o def_fileErr_amrST for an file error
|     o def_memErr_amrST for a memory error
\-------------------------------------------------------*/
signed char
addCodonPos_whoToAmr(
   struct amrST *amrSTPtr,     /*amr variants to update*/
   signed int numAmrSI,        /*number of amrs*/
   signed char *geneTblFileStr,/*gene coordiantes path*/
   signed char *refFileStr     /*reference file path*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC: addCodonPos_whoToAmrt
   '   - adds amino acid sequences for deletions and
   '     large duplications, reading frame orientation
   '     (forward/reverse), and the first reference
   '     base in the codon to an amrST that has been
   '     processed with parseVarId_whoToAmr.
   '   o fun08 sec01:
   '     - variable declerations
   '   o fun08 sec02:
   '     - get gene coordinates
   '   o fun08 sec03:
   '     - read in reference sequence
   '   o fun08 sec04:
   '     - add in gene coordinates and ammino acid
   '   o fun08 sec05:
   '     - add in whole gene target data
   '   o fun08 sec06:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01: addCodonPos_whoToAmr
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *buffHeapStr = 0;
   signed long maxLineSL = 0;
   signed long numGenesSL = 0;
   signed int posSI = 0;
   unsigned int tmpUI = 0; /*For temporary stuff*/

   signed char *refSeqHeapStr = 0;
   unsigned long lenRefUL = 0;

   signed char *revGeneHeapAryBl = 0;
   signed int *startGeneHeapArySI = 0;
   signed int *endGeneHeapArySI = 0;

   signed char *idHeapAryStr = 0;    /*holds gene ids*/

   signed char *cpStr = 0;
   signed char *dupStr = 0;
   signed int siIndex = 0;
   /*int iSwap = 0;*/    /*off gene correction attempt*/

   /*for dealing with aa deletions*/
   signed int siCodon = 0;
      /*codon on for copying deletions*/
   unsigned char oneNtUC = 0;
   unsigned char twoNtUC = 0;
   unsigned char threeNtUC = 0;

   signed char retErrSC = 0; /*error message to return*/

   FILE *tmpFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - get gene coordinates
   ^   o fun08 sec02 sub01:
   ^     - open gene coordinates table
   ^   o fun08 sec02 sub02:
   ^     - find number of genes in table
   ^   o fun08 sec02 sub03:
   ^     - allocate memory for gene coordinates
   ^   o fun08 sec02 sub04:
   ^     - get gene coordinates from table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec02 Sub01:
   *   - open coordinates table
   \*****************************************************/

   tmpFILE = fopen((char *) geneTblFileStr, "r");
   if(! tmpFILE)
      goto fileErr_fun08_sec06_sub03;

   numGenesSL = lineCnt_fileFun(tmpFILE, &maxLineSL);
   if(numGenesSL < 2)
      goto fileErr_fun08_sec06_sub03;
   --numGenesSL; /*account for header*/
   maxLineSL += 3;

   buffHeapStr =
      malloc((maxLineSL + 8) * sizeof(signed char));
   if(! buffHeapStr)
      goto memErr_fun08_sec06_sub02;

   /*****************************************************\
   * Fun08 Sec02 Sub03:
   *   - allocate memory for gene coordinates
   \*****************************************************/

   revGeneHeapAryBl =
      malloc((numGenesSL + 1) * sizeof(signed char));
   if(! revGeneHeapAryBl)
      goto memErr_fun08_sec06_sub02;

   startGeneHeapArySI =
      malloc((numGenesSL + 1) * sizeof(signed int));
   if(! startGeneHeapArySI)
      goto memErr_fun08_sec06_sub02;

   endGeneHeapArySI =
      malloc((numGenesSL + 1) * sizeof(signed int));
   if(! endGeneHeapArySI)
      goto memErr_fun08_sec06_sub02;
   
   idHeapAryStr =
      alloc_drugAry((unsigned int) numGenesSL);
   if(! idHeapAryStr)
      goto memErr_fun08_sec06_sub02;
   
   /*****************************************************\
   * Fun08 Sec02 Sub04:
   *   - get gene coordinates from table
   *   o fun08 sec02 sub04 cat01:
   *     - get first line and start loop
   *   o fun08 sec02 sub04 cat02:
   *     - copy gene id
   *   o fun08 sec02 sub04 cat03:
   *     - move past reference id
   *   o fun08 sec02 sub04 cat04:
   *     - get gene coordiantes
   *   o fun08 sec02 sub04 cat05:
   *     - close file and sort gene coordiantes
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun08 Sec02 Sub04 Cat01:
   +   - get first line and start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if( ! fgets((char *) buffHeapStr, maxLineSL, tmpFILE) )
      goto fileErr_fun08_sec06_sub03; /*get past header*/

   posSI = 0;

   while(fgets((char *) buffHeapStr, maxLineSL, tmpFILE))
   { /*Loop: read in file*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec02 Sub04 Cat02:
      +   - copy gene id
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      cpStr = buffHeapStr;

      cpStr +=
         cpDrugCase_drugAry(
            idHeapAryStr,
            cpStr,
            posSI,
            '\t'
         ); /*copy gene id over*/

      ++cpStr;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec02 Sub04 Cat03:
      +   - move past reference id
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(*cpStr != '\t')
      { /*Loop: get past reference name*/
         if(*cpStr == '\0')
            goto fileErr_fun08_sec06_sub03;

         ++cpStr;
      } /*Loop: get past reference name*/

      ++cpStr;

      /*sets to 1 if I have an '-', else it is 0*/
      revGeneHeapAryBl[posSI] = (*cpStr == '-');

      while(*cpStr != '\t')
      { /*Loop: get past reference name*/
         if(*cpStr == '\0')
            goto fileErr_fun08_sec06_sub03;

         ++cpStr;
      } /*Loop: get past reference name*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec02 Sub04 Cat04:
      +   - get gene coordiantes
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      ++cpStr;

      cpStr +=
         strToSI_base10str(
            cpStr,
            &startGeneHeapArySI[posSI]
         );

      --startGeneHeapArySI[posSI]; /*convert to index 0*/

      if(*cpStr == '\0')
         goto fileErr_fun08_sec06_sub03;

      ++cpStr; /*get off tab*/

      cpStr +=
         strToSI_base10str(
           cpStr,
           &endGeneHeapArySI[posSI]
         );

      --endGeneHeapArySI[posSI]; /*convert to index 0*/

      ++posSI;
   } /*Loop: read in file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun08 Sec02 Sub04 Cat05:
   +   - close file and sort gene coordiantes
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fclose(tmpFILE);
   tmpFILE = 0;

   sortCoords_whoToAmr(
      startGeneHeapArySI,
      endGeneHeapArySI,
      idHeapAryStr,
      revGeneHeapAryBl,
      0,
      posSI - 1
   ); /*sort the genes by starting position*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - read in reference sequence
   ^   o fun08 sec02 sub01:
   ^     - open reference sequence file
   ^   o fun08 sec02 sub02:
   ^     - find reference sequence length
   ^   o fun08 sec02 sub03:
   ^     - allocate memory for reference sequence
   ^   o fun08 sec02 sub04:
   ^     - get and clean up reference sequence
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec02 Sub01:
   *   - open the reference file
   \*****************************************************/

   tmpFILE =
      fopen((char *) refFileStr, "r");

   if(! tmpFILE)
      goto fileErr_fun08_sec06_sub03;

   /*****************************************************\
   * Fun08 Sec02 Sub02:
   *   - find length of reference sequence
   \*****************************************************/

   if(! fgets((char *) buffHeapStr, maxLineSL, tmpFILE) )
      goto fileErr_fun08_sec06_sub03; /*nothing in file*/

   tmpUI = ftell(tmpFILE); /*record header*/
   fseek(tmpFILE, 0, SEEK_END);
   lenRefUL = ftell(tmpFILE);

   lenRefUL -= tmpUI;/*rough length of reference*/
   /*this assumes that there is only one sequence in the
   `   file. Memory usage will be bad for multiple.
   */
   
   /*****************************************************\
   * Fun08 Sec02 Sub03:
   *   - allocate memory for reference sequence
   \*****************************************************/

   refSeqHeapStr =
      malloc((lenRefUL + 1) * sizeof(signed char));

   if(! refSeqHeapStr)
      goto memErr_fun08_sec06_sub02;

   /*****************************************************\
   * Fun08 Sec02 Sub04:
   *   - get and clean up reference sequence
   \*****************************************************/

   /*go back to the start of the sequence, but skip the
   `   header
   */
   fseek(tmpFILE, tmpUI, SEEK_SET);

   tmpUI =
      fread(
         (char *) refSeqHeapStr,
         sizeof(signed char),
         lenRefUL,
         tmpFILE
      );

   refSeqHeapStr[tmpUI] = '\0';

   lenRefUL = 0;
   cpStr = refSeqHeapStr;

   while(*cpStr != '\0')
   { /*Loop: remove white space*/
      if(*cpStr == '>')
         break; /*finished first sequence*/

      refSeqHeapStr[lenRefUL] = *cpStr;
      lenRefUL += (*cpStr > 32);
      ++cpStr;
   } /*Loop: remove white space*/

   refSeqHeapStr[lenRefUL] = '\0';

   fclose(tmpFILE);
   tmpFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec04: addCodonPos_whoToAmr
   ^   - add in gene coordinates and ammino acid sequences 
   ^   o fun08 sec04 sub01:
   ^     - move past entire gene effects
   ^   o fun08 sec04 sub02:
   ^     - find gene the AMR is on
   ^   o fun08 sec04 sub03:
   ^     - check if AMR is on multiple genes/find primary
   ^   o fun08 sec04 sub04:
   ^     - add in gene starting positions
   ^   o fun08 sec04 sub05:
   ^     - processing for reverse complement AMR genes
   ^   o fun08 sec04 sub06:
   ^     - add in large duplication events
   ^   o fun08 sec04 sub07:
   ^     - clean up allocated memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec04 Sub01:
   *   - Move past the entire gene effects
   \*****************************************************/

    /*check if this has had a reference position
    `   assigned. 0 likely means it has not had an
    `   reference position assigned
    */
    for(
       siIndex = 0;
       siIndex < numAmrSI;
       ++siIndex
    ){ /*Loop: find reference position*/
       if(amrSTPtr[siIndex].refPosUI)
          break;
    } /*Loop: find reference position*/

   /*****************************************************\
   * Fun08 Sec04 Sub02:
   *   - find gene the AMR is on
   \*****************************************************/

    posSI = 0;

    while(siIndex < numAmrSI)
    { /*Loop: assign aa sequences to each AMR*/

       while(
            endGeneHeapArySI[posSI++]
          < (signed int) amrSTPtr[siIndex].refPosUI
       ){ /*Loop: find AMR position on gene*/
         if(posSI > numGenesSL)
            break; /*AMR not on gene*/
       } /*Loop: find AMR position on gene*/

       --posSI; /*always one gene off*/

       if(posSI >= numGenesSL)
          break; /*finished (no more genes)*/

      /**************************************************\
      * Fun08 Sec04 Sub03:
      *   - check if AMR is on multiple genes/find primary
      \**************************************************/

       /*check I the sequence is inside gene bounds*/
       tmpUI =
            amrSTPtr[siIndex].refPosUI
          + amrSTPtr[siIndex].lenRefSeqUI
          - 1; /*-1 to convert index 1 to index 0*/

       if(
            tmpUI > (unsigned int) endGeneHeapArySI[posSI]
          ||   amrSTPtr[siIndex].refPosUI
             < (unsigned int) startGeneHeapArySI[posSI]
       ){ /*If: sequence outside of selected gene bounds*/
          /*see if this AMR is really on the next gene
          `   by comparing it to the last/next AMR
          */
          tmpUI =
             eql_charCp(
                amrSTPtr[siIndex].geneIdStr,
                get_drugAry(
                   idHeapAryStr,
                   (unsigned int) posSI
                ),
                '\0'
             ); /*check if I have the same ids*/

          if(! tmpUI) ; /*correct gene; do nothing*/

          else if(posSI > 0)
          { /*Else If: wrong gene; check previous gene*/
             tmpUI =
                eql_charCp(
                   amrSTPtr[siIndex].geneIdStr,
                   get_drugAry(
                      idHeapAryStr,
                      (unsigned int) posSI - 1
                   ),
                   '\0'
                ); /*check if last gene was correct*/

                if(! tmpUI)
                   --posSI; /*found gene*/
                else if(posSI < numGenesSL - 1)
                   goto checkNextGene_fun08_sec04_sub03;
                else if(siIndex > 0)
                   goto checkLastAmr_fun08_sec04_sub03;

             /*else: no idea which gene it is*/
          } /*If: wrong gene; check previous gene*/

          else if(posSI < numGenesSL - 1)
          { /*Else If: wrong gene: check next gene*/
              checkNextGene_fun08_sec04_sub03:;

              tmpUI =
                 eql_charCp(
                    amrSTPtr[siIndex].geneIdStr,
                    get_drugAry(
                       idHeapAryStr,
                       (unsigned int) posSI + 1
                    ),
                    '\0'
                 ); /*check if next gene was correct*/

             if(! tmpUI)
                ++posSI;
             else if(siIndex > 0)
                goto checkLastAmr_fun08_sec04_sub03;
             /*else: no idea which gene it is*/
          } /*Else If: wrong gene: check next gene*/

          else if(siIndex > 0)
          { /*Else If: have a last amr to check*/
             checkLastAmr_fun08_sec04_sub03:;

              tmpUI =
                 eql_charCp(
                    amrSTPtr[siIndex].geneIdStr,
                    amrSTPtr[siIndex - 1].geneIdStr,
                    '\0'
                 ); /*check if I have the same ids*/

             if(! tmpUI)
                --posSI;
             /*else: no idea which gene it belongs to*/
          } /*Else If: have a last amr to check*/

          /*else: no idea what gene belongs to*/
       } /*If: sequence outside of selected gene bounds*/

      /**************************************************\
      * Fun08 Sec04 Sub04:
      *   - add in gene starting positions
      \**************************************************/

       /*make sure the start is first mapped base*/
       if(
            startGeneHeapArySI[posSI]
          < endGeneHeapArySI[posSI]
       ){ /*If: start comes first*/
          amrSTPtr[siIndex].geneFirstRefUI =
             startGeneHeapArySI[posSI];

          amrSTPtr[siIndex].geneLastRefUI=
             endGeneHeapArySI[posSI];
       } /*If: start comes first*/

       else
       { /*Else: start comes last*/
         amrSTPtr[siIndex].geneLastRefUI =
            startGeneHeapArySI[posSI];

         amrSTPtr[siIndex].geneFirstRefUI =
            endGeneHeapArySI[posSI];
       } /*Else: start comes last*/

      /**************************************************\
      * Fun08 Sec04 Sub05:
      *   - processing for reverse complement AMR genes
      *   o fun08 sec04 sub05 cat01:
      *     - add in direction & check if aa sequence
      *   o fun08 sec04 sub05 cat02:
      *     - find codon position & check aa seq length
      *   o fun08 sec04 sub05 cat03:
      *     - get reference amino acid sequence for
      *       AMR(s) in reverse complement genes
      \**************************************************/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun08 Sec04 Sub05 Cat01:
       +   - add in direction and check if aa sequence
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       if(revGeneHeapAryBl[posSI])
       { /*If: reverse complement gene*/
          amrSTPtr[siIndex].dirFlagSC =
             def_revCompDir_amrST;

          if(amrSTPtr[siIndex].codonNumUI == 0)
          { /*If: not amino acid mutation*/
             ++siIndex;
             continue;
          } /*If: not amino acid mutation*/

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun08 Sec04 Sub05 Cat02:
          +   - find codon position & check aa seq length
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          /*Update the starting position*/
          /*this assumes the codon number is correct. i
          `   had one case (pncA_p.Ter187Argext*?) were I
          `   am not sure if this was true. Ref: 2288680,
          `   codon: 187. The output was 2288675, which
          `   is off
          */
          amrSTPtr[siIndex].codonPosUI =
               endGeneHeapArySI[posSI]
             - ((amrSTPtr[siIndex].codonNumUI - 1) * 3);
          
          if(amrSTPtr[siIndex].endCodonNumUI == 0)
          { /*If: single aa change*/
             amrSTPtr[siIndex].endCodonNumUI =
                amrSTPtr[siIndex].codonNumUI;

             ++siIndex;
             continue;
          } /*If: single aa change*/

          /*check to see if more than 1 or 2 aa long*/
          tmpUI =
               amrSTPtr[siIndex].endCodonNumUI
             - amrSTPtr[siIndex].codonNumUI;

          if(tmpUI < 2)
          { /*If: single amino acid (already set up)*/
             ++siIndex;
             continue;
          } /*If: single amino acid (already set up)*/

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun08 Sec04 Sub05 Cat03:
          +   - Get reference amino acid sequence for
          +     AMR(s) in reverse complement genes
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          /*set up for getting the reference amino
          `   acids; the +1 is to ignore the first amino
          `   acid in the deletion (I already have it)
          */
          dupStr = amrSTPtr[siIndex].refAaStr + 1;

          /*-3 to get off the first codon*/
          cpStr =
               refSeqHeapStr
             + amrSTPtr[siIndex].codonPosUI - 3;

          for(
             siCodon = amrSTPtr[siIndex].codonNumUI + 1;
             siCodon
                < (signed int)
                   amrSTPtr[siIndex].endCodonNumUI;
             ++siCodon
          ){ /*loop: copy deletion*/
             oneNtUC =
                revNtTo2Bit[(unsigned char) *cpStr--];
             twoNtUC =
                revNtTo2Bit[(unsigned char) *cpStr--];
             threeNtUC =
                revNtTo2Bit[(unsigned char) *cpStr--];

             *dupStr++ =
               codonTbl[oneNtUC][twoNtUC][threeNtUC];
          } /*loop: copy deletion*/
       } /*If: reverse complement gene*/

      /**************************************************\
      * Fun08 Sec04 Sub06:
      *   - processing for forward AMR genes
      *   o fun08 sec04 sub06 cat01:
      *     - add in direction & check if aa sequence
      *   o fun08 sec04 sub06 cat02:
      *     - find codon position & check aa seq length
      *   o fun08 sec04 sub06 cat03:
      *     - get reference amino acid sequence for
      *       AMR(s) in reverse complement genes
      \**************************************************/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun08 Sec04 Sub06 Cat01:
       +   - add in direction and check if aa sequence
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       else
       { /*Else: foward gene*/
          amrSTPtr[siIndex].dirFlagSC =
             def_forwardDir_amrST;

          if(amrSTPtr[siIndex].codonNumUI == 0)
          { /*If: no amino acid seqence*/
             ++siIndex;
             continue;
          } /*If: no amino acid seqence*/

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun08 Sec04 Sub06 Cat02:
          +   - Find codon position & check aa seq length
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          /*update the starting position*/
          /*this assumes the codon number is correct. I
          `   had one case (pncA_p.Ter187Argext*?) were I
          `   am not sure if this was true. Ref: 2288680,
          `   codon: 187. The output was 2288675, which
          `   is off. this was an reverse case
          */
          amrSTPtr[siIndex].codonPosUI =
               startGeneHeapArySI[posSI]
             + ((amrSTPtr[siIndex].codonNumUI - 1) * 3);

          if(amrSTPtr[siIndex].endCodonNumUI == 0)
          { /*If: single aa change*/
             amrSTPtr[siIndex].endCodonNumUI =
                amrSTPtr[siIndex].codonNumUI;

             ++siIndex;
             continue;
          } /*If: single aa change*/

          /*Check to see if more than 1 or 2 aa long*/
          tmpUI =
               amrSTPtr[siIndex].endCodonNumUI
             - amrSTPtr[siIndex].codonNumUI;

          if(tmpUI < 2)
          { /*If: single amino acid (already setup)*/
             ++siIndex;
             continue;
          } /*If: single amino acid (already setup)*/

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun08 Sec04 Sub06 Cat03:
          +   - get reference amino acid sequence for
          +     AMR(s) in reverse complement genes
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          /*set up for getting the reference amino
          `   acids; the +1 is to ignore the first amino
          `   acid in the deletion (I already have it)
          */
          dupStr = amrSTPtr[siIndex].refAaStr + 1;

          /*+3 to get off the first codon*/
          cpStr =
               refSeqHeapStr
             + amrSTPtr[siIndex].codonPosUI
             + 3;

          for(
             siCodon = amrSTPtr[siIndex].codonNumUI + 1;
             siCodon
                < (signed int)
                  amrSTPtr[siIndex].endCodonNumUI;
             ++siCodon
          ){ /*Loop: copy deletion*/
             oneNtUC =
                ntTo2Bit[(unsigned char) *cpStr++];
             twoNtUC =
                ntTo2Bit[(unsigned char) *cpStr++];
             threeNtUC =
                ntTo2Bit[(unsigned char) *cpStr++];

             *dupStr++ =
               codonTbl[oneNtUC][twoNtUC][threeNtUC];
          } /*Loop: copy deletion*/
       } /*Else: foward gene*/

      /**************************************************\
      * Fun08 Sec04 Sub07:
      *   - Add in large duplication events
      \**************************************************/

       if(amrSTPtr[siIndex].aaMultiDupBl)
       { /*If: large duplicate*/
          dupStr = amrSTPtr[siIndex].amrAaStr;
          cpStr = amrSTPtr[siIndex].refAaStr;

          while(*cpStr != '\0')
             *dupStr++ = *cpStr++;

          cpStr = amrSTPtr[siIndex].refAaStr;

          while(*cpStr != '\0')
             *dupStr++ = *cpStr++;

          *dupStr = '\0';
       } /*If: large duplicate*/

       ++siIndex;
    } /*Loop: assign aa sequences to each AMR*/

   /*****************************************************\
   * Fun08 Sec04 Sub08:
   *   - clean up allocated memory
   \*****************************************************/

   free(revGeneHeapAryBl);
   free(startGeneHeapArySI);
   free(endGeneHeapArySI);

   revGeneHeapAryBl = 0;
   startGeneHeapArySI = 0;
   endGeneHeapArySI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec05:
   ^   - add in the whole gene target data
   ^   o fun08 sec05 sub01:
   ^     - set up gene coordinates for deletions
   ^   o fun08 sec05 sub02:
   ^     - Add in the genes sequecne
   ^   o fun08 sec05 sub03:
   ^     - resort by reference position (+ clean up)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec05 Sub01:
   *   - set up gene coordinates for deletions
   *   o fun08 sec05 sub01 cat01:
   *     - sort genes by id and start loop
   *   o fun08 sec05 sub01 cat02:
   *     - need to get coordinates from previous gene
   *       (whole gene events)
   *   o fun08 sec05 sub01 cat03:
   *     - need to get coordinates from next gene
   *   o fun08 sec05 sub01 cat04:
   *     - handle non-deletion events (move on)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun08 Sec05 Sub01 Cat01:
   +   - sort genes by id and start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   sortGeneId_amrST(
      amrSTPtr,
      0,
      numAmrSI - 1
   );

   for(
      siIndex = 0;
      siIndex < numAmrSI;
      ++siIndex
   ){ /*Loop: add gene start & end to entire gene events*/

      if(amrSTPtr[siIndex].wholeGeneFlagSC)
      { /*If: an entire gene event*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun08 Sec05 Sub01 Cat02:
         +   - need to get coordinates from previous gene
         +     (whole gene events)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(   siIndex > 1
            &&
               ! eql_charCp(
                 amrSTPtr[siIndex].geneIdStr,
                 amrSTPtr[siIndex - 1].geneIdStr,
                 '\0'
               )
         ){ /*If: last amr is the same gene*/
            amrSTPtr[siIndex].geneFirstRefUI =
               amrSTPtr[siIndex - 1].geneFirstRefUI;

            amrSTPtr[siIndex].geneLastRefUI =
               amrSTPtr[siIndex - 1].geneLastRefUI;

            amrSTPtr[siIndex].dirFlagSC =
               amrSTPtr[siIndex - 1].dirFlagSC;

            if(
                 amrSTPtr[siIndex].wholeGeneFlagSC
               & def_geneDel_amrST
            ){ /*If: add in coordinate for gene deletion*/

               if(amrSTPtr[siIndex].refPosUI <= 0)
                  amrSTPtr[siIndex].refPosUI =
                     amrSTPtr[siIndex-1].geneFirstRefUI-1;
                  /*if no AMR position assigned yet*/

                  /*want to grab base before the gene*/
            } /*If: add in coordinate for gene deletion*/
         } /*If: last amr is the same gene*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun08 Sec05 Sub01 Cat03:
         +   - need to get coordinates from next gene
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else
         { /*Else: The AMR is same as the next gene*/
            amrSTPtr[siIndex].geneFirstRefUI =
               amrSTPtr[siIndex + 1].geneFirstRefUI - 1;

            amrSTPtr[siIndex].geneLastRefUI =
               amrSTPtr[siIndex + 1].geneLastRefUI;

            amrSTPtr[siIndex].dirFlagSC =
               amrSTPtr[siIndex + 1].dirFlagSC;

            if(
                 amrSTPtr[siIndex].wholeGeneFlagSC
               & def_geneDel_amrST
            ){ /*If: Add in coordinate for gene deletion*/
               /*I want to grab the base before the gene*/

               if(amrSTPtr[siIndex].refPosUI <= 0)
                  amrSTPtr[siIndex].refPosUI =
                     amrSTPtr[siIndex+1].geneFirstRefUI-1;
                  /*if no AMR position assigned yet*/

            } /*If: Add in coordinate for gene deletion*/
         } /*Else: The AMR is same as the next gene*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun08 Sec05 Sub01 Cat04:
         +   - handle non-deletion events (move on)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(!
            (    amrSTPtr[siIndex].wholeGeneFlagSC
               & def_geneDel_amrST
            )
         ) continue;

         if(
               amrSTPtr[siIndex].amrSeqStr[0] != '0'
            && amrSTPtr[siIndex].refSeqStr[0] != '0'
         ) continue;

         /***********************************************\
         * Fun08 Sec05 Sub02:
         *   - add in the genes sequence
         *   o fun08 sec05 sub02 cat01:
         *     - copy gene + base before gene to reference
         *   o fun08 sec05 sub02 cat02:
         *     - copy the base before the gene to the AMR
         \***********************************************/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun08 Sec05 Sub02 Cat01:
         +   - copy gene + base before gene into reference
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         /*get length of the deletion*/
         tmpUI =
              amrSTPtr[siIndex].geneLastRefUI
            - amrSTPtr[siIndex].geneFirstRefUI;

         if(amrSTPtr[siIndex].refSeqStr)
            free(amrSTPtr[siIndex].refSeqStr);

         amrSTPtr[siIndex].refSeqStr = 0;

         ++tmpUI; /*account for base before gene*/

         amrSTPtr[siIndex].refSeqStr =
            malloc((tmpUI + 1) * sizeof(signed char));

         if(! amrSTPtr[siIndex].refSeqStr)
            goto memErr_fun08_sec06_sub02;

         amrSTPtr[siIndex].lenRefSeqUI = tmpUI;

         cpLen_ulCp(
            amrSTPtr[siIndex].refSeqStr,
            &refSeqHeapStr[ amrSTPtr[siIndex].refPosUI ],
            amrSTPtr[siIndex].lenRefSeqUI
         ); /*copy genes sequence + 1 base*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun08 Sec05 Sub02 Cat02:
         +   - copy base before gene to AMR
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(amrSTPtr[siIndex].amrSeqStr)
            free(amrSTPtr[siIndex].amrSeqStr);

         amrSTPtr[siIndex].amrSeqStr = 0;

         amrSTPtr[siIndex].amrSeqStr =
            malloc(2 * sizeof(char));

         if(! amrSTPtr[siIndex].amrSeqStr)
            goto memErr_fun08_sec06_sub02;

         amrSTPtr[siIndex].amrSeqStr[0] =
            refSeqHeapStr[amrSTPtr[siIndex].refPosUI];

         amrSTPtr[siIndex].amrSeqStr[1] = '\0';
      } /*If: This is an entire gene event*/
   } /*Loop: add gene start & end to entire gene events*/

   /*****************************************************\
   * Fun08 Sec05 Sub03:
   *   - resort by reference position (+ clean up)
   \*****************************************************/

   free(refSeqHeapStr);
   refSeqHeapStr = 0;

   sortPos_amrST(
      amrSTPtr,
      0,
      numAmrSI - 1
   ); 

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec06:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   retErrSC = 0;
   goto cleanUp_fun08_sec06_sub04;

   memErr_fun08_sec06_sub02:;
      retErrSC = def_memErr_amrST;
      goto cleanUp_fun08_sec06_sub04;

   fileErr_fun08_sec06_sub03:;
      retErrSC = def_fileErr_amrST;
      goto cleanUp_fun08_sec06_sub04;

   cleanUp_fun08_sec06_sub04:;
      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;

      if(idHeapAryStr)
         free(idHeapAryStr);
      idHeapAryStr = 0;

      if(refSeqHeapStr)
         free(refSeqHeapStr);
      refSeqHeapStr = 0;

      if(revGeneHeapAryBl)
         free(revGeneHeapAryBl);
      revGeneHeapAryBl = 0;

      if(startGeneHeapArySI)
         free(startGeneHeapArySI);
      startGeneHeapArySI = 0;

      if(endGeneHeapArySI)
         free(endGeneHeapArySI);
      endGeneHeapArySI = 0;

      if(tmpFILE)
         fclose(tmpFILE);
      tmpFILE = 0;

      return retErrSC;
} /*addCodonPos_whoToAmr*/

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
