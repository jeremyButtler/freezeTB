/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' addAmr SOF: Start Of File
'   - has functions to get tbAmr database information from
'     variant ids
'   o header;
'     - included libraries
'   o .c fun01: aaToSeq_addAmr
'     - change amino acids to a valid nucleotide sequence
'   o .c fun02: revCmp_addAmr
'     - reverse complement a sequence
'   o .c fun03: addOneAa_addAmr
'     - add single amino acid variant to an amrST struct
'   o .c fun04: parseVarId_addAmr
'     - parse variant id to its correct format
'   o .c fun05: addNtSeq_addAmr
'     - adds nucleotide sequence to AMR
'   o .c fun06: addCodonPos_addAmr
'     - adds amino acid sequences for deletions and
'       large duplications, reading frame orientation
'       (forward/reverse), and the first reference
'       base in the codon to an amrST that has been
'       processed with addNtSeq_addAmr
'   o fun07: var_addAmr
'     - adds a variant to an AMR struct, but does not
'       complete (call addCodonPos_addAmr)
'   o fun08: addDrugs_addAmr:
'     - add anti-microbials (antibiotics) to an AMR
'   o fun09: lineToVar_addAmr
'     - convert line from variant file to tbAmr entry
'   o fun10: getVar_addAmr
'     - get a variant id from a file
'   o license:
'     - licensing for this code (public dofun09 / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header;
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "addAmr.h"

#include "../genLib/ulCp.h"
#include "../genLib/base10str.h"
#include "../genLib/genMath.h"
#include "../genLib/fileFun.h"
#include "../genBio/codonFun.h"
#include "../genBio/seqST.h"
#include "../genBio/geneCoord.h"

#include "drugAry.h"
#include "amrST.h"

/*.h files only*/
#include "../genBio/ntTo2Bit.h"
#include "../genBio/revNtTo2Bit.h"
#include "../genBio/codonTbl.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   -
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: aaToSeq_addAmr
|   - converts amino acids to a valid nucleotide sequence
| Input:
|   - aaStr:
|     o c-string with single letter amino acids to convert
|       * can include * (for stop) or ? for any
|   - seqStr:
|     o c-string to hold sequence (must be large enough)
|   - refStr:
|     o c-string with reference aa sequence; single letter
|     o used for '?' mark entry, use 0 to ignore
| Output:
|   - Modifies:
|     o seqStr to have converted aaStr
|   - Returns:
|     o number bases added to seqStr
\-------------------------------------------------------*/
unsigned int
aaToSeq_addAmr(
   signed char *aaStr,  /*aminio acid sequence*/
   signed char *seqStr, /*gets converted Nt sequence*/
   signed char *refStr  /*ref aa sequence, for ? case*/
){
   unsigned int uiVar = 0;

   while(*aaStr)
   { /*Loop: translate aa seqeunce*/
      switch(*aaStr++)
      { /*Switch: find amino acid sequence*/
         case '*':
         /*Case: stop codon*/
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'A';
            break;
         /*Case: stop codon*/

         case '?':
         /*Case: anything but reference*/
            if(! refStr)
            { /*If: no reference input*/
               /*no idea what to put, so mask*/
               seqStr[uiVar++] = 'N';
               seqStr[uiVar++] = 'N';
               seqStr[uiVar++] = 'N';
            } /*If: no reference input*/

            else
            { /*Else: can check reference*/
               if(*refStr == '*')
               { /*If: can not assign stop*/
                  seqStr[uiVar++] = 'A';
                  seqStr[uiVar++] = 'T';
                  seqStr[uiVar++] = 'G';
               } /*If: can not assign stop*/
 
               else
               { /*Else: can asign stop*/
                  seqStr[uiVar++] = 'T';
                  seqStr[uiVar++] = 'A';
                  seqStr[uiVar++] = 'A';
               } /*Else: can asign stop*/
            } /*Else: can check reference*/

            break;
         /*Case: anything but reference*/

         case 'a':
         case 'A':
         /*Case: alanine*/
            seqStr[uiVar++] = 'G';
            seqStr[uiVar++] = 'C';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: alanine*/

         case 'r':
         case 'R':
         /*Case: arginine*/
            seqStr[uiVar++] = 'C';
            seqStr[uiVar++] = 'G';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: arginine*/

         case 'n':
         case 'N':
         /*Case: asparagine*/
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: asparagine*/

         case 'd':
         case 'D':
         /*Case: aspartic acid*/
            seqStr[uiVar++] = 'G';
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: aspartic acid*/

         case 'c':
         case 'C':
         /*Case: cysteine*/
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'G';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: cysteine*/

         case 'q':
         case 'Q':
         /*Case: glutamine*/
            seqStr[uiVar++] = 'C';
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'A';
            break;
         /*Case: glutamine*/

         case 'e':
         case 'E':
         /*Case: glutamic acid*/
            seqStr[uiVar++] = 'G';
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'A';
            break;
         /*Case: glutamic acid*/

         case 'g':
         case 'G':
         /*Case: glycine*/
            seqStr[uiVar++] = 'G';
            seqStr[uiVar++] = 'G';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: glycine*/

         case 'h':
         case 'H':
         /*Case: histidine*/
            seqStr[uiVar++] = 'C';
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: histidine*/

         case 'i':
         case 'I':
         /*Case: isoleucine*/
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: isoleucine*/

         case 'k':
         case 'K':
         /*Case: lysine*/
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'A';
            break;
         /*Case: lysine*/

         case 'l':
         case 'L':
         /*Case: luecine*/
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'A';
            break;
         /*Case: luecine*/

         case 'm':
         case 'M':
         /*Case: methionine (start)*/
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'G';
            break;
         /*Case: methionine (start)*/

         case 'f':
         case 'F':
         /*Case: phenylalanine*/
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: phenylalanine*/

         case 'p':
         case 'P':
         /*Case: proline*/
            seqStr[uiVar++] = 'C';
            seqStr[uiVar++] = 'C';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: proline*/

         case 's':
         case 'S':
         /*Case: serine*/
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'C';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: serine*/

         case 't':
         case 'T':
         /*Case: threonine*/
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'C';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: threonine*/

         case 'w':
         case 'W':
         /*Case: tryptophan*/
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'G';
            seqStr[uiVar++] = 'G';
            break;
         /*Case: tryptophan*/

         case 'y':
         case 'Y':
         /*Case: tryosine*/
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'A';
            seqStr[uiVar++] = 'T';
            break;
         /*Case: tryosine*/

         case 'v':
         case 'V':
         /*Case: valine*/
            seqStr[uiVar++] = 'G';
            seqStr[uiVar++] = 'T';
            seqStr[uiVar++] = 'G';
            break;
         /*Case: valine*/
      } /*Switch: find amino acid sequence*/
   } /*Loop: translate aa seqeunce*/

   seqStr[uiVar] = 0;
   return uiVar;
} /*aaToSeq_addAmr*/

/*-------------------------------------------------------\
| Fun02: revCmp_addAmr
|   - reverse complement a sequence
| Input:
|   - seqStr:
|     o c-string with sequence to reverse complement
| Output:
|   - Modifies:
|     o seqStr to have reverse complement sequence
\-------------------------------------------------------*/
void
revCmp_addAmr(
   signed char *seqStr /*sequence to reverse complement*/
){
    signed char *endStr = seqStr;
    signed char swapSC = 0;

    endStr += endStr_ulCp(endStr);
    --endStr;
    if(endStr < seqStr)
       return; /*nothing to do*/

    while(endStr >= seqStr)
    { /*Loop: reverse complement*/
       switch(*seqStr & (~32)) /*Make sure upper case*/
       { /*Switch: complement starting base*/
           case 'A': *seqStr = 'T'; break;
           case 'C': *seqStr = 'G'; break;
           case 'G': *seqStr = 'C'; break;
           case 'T': *seqStr = 'A'; break;
           case 'U': *seqStr = 'A'; break;
           case 'W': *seqStr = 'W'; break;
           case 'S': *seqStr = 'S'; break;
           case 'M': *seqStr = 'K'; break;  /*A/C->T/G*/
           case 'K': *seqStr = 'M'; break;  /*T/G->A/C*/
           case 'R': *seqStr = 'Y'; break;  /*A/G->T/C*/
           case 'Y': *seqStr = 'R'; break;  /*T/C->A/G*/
           case 'B': *seqStr = 'V'; break;/*C/G/T->G/C/A*/
           case 'D': *seqStr = 'H'; break;/*G/T/A->C/A/T*/
           case 'H': *seqStr = 'D'; break;/*C/A/T->G/T/A*/
           case 'V': *seqStr = 'B'; break;/*A/C/G->T/G/C*/
           case 'N': *seqStr = 'N'; break;/*A/C/G/T*/
           default: *seqStr = 0;
       } /*Switch: complement starting base*/

       if(seqStr == endStr)
           break;

       switch(*endStr & (~32)) /*Make sure upper case*/
       { /*Switch: complement ending base*/
           case 'A': *endStr = 'T'; break;
           case 'C': *endStr = 'G'; break;
           case 'G': *endStr = 'C'; break;
           case 'T': *endStr = 'A'; break;
           case 'U': *endStr = 'A'; break;
           case 'W': *endStr = 'W'; break;
           case 'S': *endStr = 'S'; break;
           case 'M': *endStr = 'K'; break;  /*A/C->T/G*/
           case 'K': *endStr = 'M'; break;  /*T/G->A/C*/
           case 'R': *endStr = 'Y'; break;  /*A/G->T/C*/
           case 'Y': *endStr = 'R'; break;  /*T/C->A/G*/
           case 'B': *endStr = 'V'; break;/*C/G/T->G/C/A*/
           case 'D': *endStr = 'H'; break;/*G/T/A->C/A/T*/
           case 'H': *endStr = 'D'; break;/*C/A/T->G/T/A*/
           case 'V': *endStr = 'B'; break;/*A/C/G->T/G/C*/
           case 'N': *endStr = 'N'; break;/*A/C/G/T*/
           default: *endStr = 0;
       } /*Switch: complement ending base*/

       /*reverse (swap) bases*/
       swapSC = *seqStr;
       *seqStr++ = *endStr;
       *endStr-- = swapSC;
    } /*Loop: reverse complement*/
} /*revCmp_addAmr*/

/*-------------------------------------------------------\
| Fun03: addOneAa_addAmr
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
|     o def_memErr_addAmr for a memory error
\-------------------------------------------------------*/
signed char
addOneAa_addAmr(
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
      errSC = def_memErr_addAmr;
      goto ret_fun02;

   ret_fun02:;
      return errSC;
} /*addOneAa_addAmr*/

/*-------------------------------------------------------\
| Fun04: parseVarId_addAmr
|   - parse variant id to its correct format
| Input:
|   - amrSTPtr:
|     - pointer to amrST struct to update and process
|       variants for
| Output:
|   - Modifies:
|     o amrSTPtr with amino acid mutations to have the
|       amino acid sequence whe possible. For large
|       deletions/duplications you will need to call
|       addCodonPos_addAmr
|   - Returns
|     o 0 for success
|     o def_memErr_addAmr for a memory error
\-------------------------------------------------------*/
signed char
parseVarId_addAmr(
   struct amrST *amrSTPtr /*amr variant to update*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - parse variant idea from the WHO 2023 TB catalog
   '   o fun04 sec01:
   '     - variable declerations
   '   o fun04 sec02:
   '     - process variant id for amino acid entries
   '   o fun04 sec03:
   '     - clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;    /*for reporting errors*/

   signed char firstAaSC = 0;
   signed char secAaSC = 0;
   signed char *varStr = 0;
   signed char *cpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - process variant id for amino acid entries
   ^   o fun04 sec02 sub01:
   ^     - start loop (all AMR(s)) and check if I have an
   ^       amino acid variant to process
   ^   o fun04 sec02 sub02:
   ^     - get first amino acid from variant id
   ^   o fun04 sec02 sub03:
   ^     - get codon number of first amio acid
   ^   o fun04 sec02 sub04:
   ^     - get second amino acid/mutation type for
   ^       variants that are not deletions/insertions
   ^   o fun04 sec02 sub05:
   ^     - handle deletions/insertions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec02 Sub01: who2023ParsVar
   *   - start loop (all AMR(s)) and check if I have an
   *     amino acid variant to process
   \*****************************************************/

   varStr = amrSTPtr->varIdStr;

   if(varStr[1] != '.')
   { /*If: includes gene name*/
      while(*varStr !='.' && *varStr > 32)
         ++varStr;
      --varStr;
   } /*If: includes gene name*/

   if(varStr[0] != '.' || (varStr[1] & ~32) != 'P')
   { /*If: missing gene name*/
      if((varStr[0] & ~32) != 'P' || varStr[1] != '.')
      { /*If: not aa mutation*/
         if(
               (*(varStr - 0) & ~32) == 'F'
            && (*(varStr - 1) & ~32) == 'O'
            && (*(varStr - 2) & ~32) == 'L'
            && (*(varStr - 3)) == '_'
         ){ /*If: gene loss of function entry*/
             amrSTPtr->wholeGeneFlagSC |=
                def_geneFrameshift_amrST;
         } /*If: gene loss of function entry*/

         goto done_fun04_sec03; /*not aa mutation*/
      } /*If: not an aa mutation*/
   } /*If: missing gene name*/

   varStr += 2; /*+2 is to get off '.p'*/
     
   /***************************************************\
   * Fun04 Sec02 Sub02:
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
   * Fun04 Sec02 Sub03: who2023ParsVar
   *   - get codon number of first amio acid
   \***************************************************/

   varStr +=
      strToUI_base10str(varStr, &amrSTPtr->codonNumUI);

   /***************************************************\
   * Fun04 Sec02 Sub04: who2023ParsVar
   *   - get second amino acid/mutation type for
   *     variants that are not deletions/insertions
   *   o fun04 sec02 sub04 cat01:
   *     - any amino acid (first aa)
   *   o fun04 sec02 sub04 cat02:
   *     - stop codon (first aa)
   *   o fun04 sec02 sub04 cat03:
   *     - frame shift (frist aa)
   *   o fun04 sec02 sub04 cat04:
   *     - deletion (frist aa)
   *   o fun04 sec02 sub04 cat05:
   *     - singe amino acid duplication (first aa)
   *   o fun04 sec02 sub04 cat06:
   *     - single amino acid mutation (first/second aa)
   \***************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub04 Cat01:
   +   - any amino acid (first aa)
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(varStr[0] == '?')
   { /*If: could be any aa change*/
      secAaSC = '?';
      amrSTPtr->endCodonNumUI = amrSTPtr->codonNumUI;

      if( addOneAa_addAmr(amrSTPtr, firstAaSC, '?') )
         goto memErr_fun04_sec03;
      goto done_fun04_sec03;
   } /*If: could be any aa change*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub04 Cat02:
   +   - stop codon (first aa)
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/
   
   else if(varStr[0] == '*')
   { /*If: stop*/
      secAaSC = '*';
      amrSTPtr->endCodonNumUI = amrSTPtr->codonNumUI;

      if( addOneAa_addAmr(amrSTPtr, firstAaSC, '*') )
         goto memErr_fun04_sec03;
      goto done_fun04_sec03;
   } /*If: stop*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub04 Cat03:
   +   - frame shift (frist aa)
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
         (varStr[0] & ~32) == 'F'
      && (varStr[1] & ~32) == 'S' 
   ){ /*Else If: this was a frame shift*/
      amrSTPtr->frameshiftBl = 1;
      amrSTPtr->lenRefAaUI = 1;

      amrSTPtr->refAaStr =
         malloc(2 * sizeof(signed char));
      if(! amrSTPtr->refAaStr)
         goto memErr_fun04_sec03;

      amrSTPtr->refAaStr[0] = firstAaSC;
      amrSTPtr->refAaStr[1] = '\0';

      amrSTPtr->mutTypeStr[0] = 'd';
      amrSTPtr->mutTypeStr[1] = 'e';
      amrSTPtr->mutTypeStr[2] = 'l';
      amrSTPtr->mutTypeStr[3] = 0;

      /*no amr amino acid, because it is a frame shift*/
      amrSTPtr->endCodonNumUI = amrSTPtr->codonNumUI;
      goto done_fun04_sec03;
   } /*Else If: this was a frame shift*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub04 Cat04:
   +   - deletion (frist aa)
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
         (varStr[0] & ~32) == 'D'
      && (varStr[1] & ~32) == 'E'
      && (varStr[2] & ~32) == 'L'
   ) goto parseDel_fun04_sec02_sub05_cat02;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub04 Cat05:
   +   - singe amino acid duplication (first aa)
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
         (varStr[0] & ~32) == 'D'
      && (varStr[1] & ~32) == 'U'
      && (varStr[2] & ~32) == 'P'
   ){ /*Else If: one aa duplication*/
      amrSTPtr->lenRefAaUI = 1;

      amrSTPtr->refAaStr =
         malloc(2 * sizeof(signed char));

      if(! amrSTPtr->refAaStr)
         goto memErr_fun04_sec03;

      amrSTPtr->refAaStr[0] = firstAaSC;
      amrSTPtr->refAaStr[1] = '\0';

      amrSTPtr->lenAmrAaUI = 2;

      amrSTPtr->amrAaStr =
         malloc(3 * sizeof(signed char));

      if(!amrSTPtr->amrAaStr)
         goto memErr_fun04_sec03;

      amrSTPtr->amrAaStr[0] = firstAaSC;
      amrSTPtr->amrAaStr[1] = firstAaSC;
      amrSTPtr->amrAaStr[2] = '\0';
      amrSTPtr->endCodonNumUI = amrSTPtr->codonNumUI;
      goto done_fun04_sec03;
   } /*Else If: one aa duplication*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub04 Cat06:
   +   - single amino acid mutation (first/second aa)
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(varStr[0] != '_')
   { /*Else If: for 2023 this is a single change*/

      /*check if is a lost stop codon*/
      if(varStr[0] != 'e') ;
      else if(varStr[1] != 'x') ;
      else if(varStr[2] != 't') ;
      else if(varStr[3] != '*') ;
      else if(varStr[4] != '?') ;
      else if(varStr[5]) ;
      else
      { /*Else: loss of stop codon*/
         secAaSC = '?';
         goto addAa_fun04_sec02_sub04_cat06;
      } /*Else: loss of stop codon*/

      if(! varStr[1])
         secAaSC = varStr[0]; /*one letter*/
      else
         secAaSC = aaTripToChar_codonFun(varStr);
         /*three letter amino acid code*/

      addAa_fun04_sec02_sub04_cat06:;
         if(addOneAa_addAmr(amrSTPtr,firstAaSC,secAaSC))
            goto memErr_fun04_sec03;
         amrSTPtr->endCodonNumUI = amrSTPtr->codonNumUI;
         goto done_fun04_sec03;
   } /*Else If: for 2023 this is a single change*/

   /***************************************************\
   * Fun04 Sec02 Sub05:
   *   - handle deletions/insertions
   *   o fun04 sec02 sub05 cat01:
   *     - find amino acid at end of eletion/insertion
   *   o fun04 sec02 sub05 cat02:
   *     - find position of the last amino acid
   *   o fun04 sec02 sub05 cat03:
   *     - insertion only case; add start/end aa to ref
   *   o fun04 sec02 sub05 cat04:
   *     - ins case; add start/end aa to amrSeqStr
   *   o fun04 sec02 sub05 cat05:
   *     - ins case; add in the inserted aa sequence
   *   o fun04 sec02 sub05 cat06:
   *     - handle large duplications (> 1 amino acid)
   *   o fun04 sec02 sub05 cat07:
   *     - deletion; add first and last aa to refAaStr
   *   o fun04 sec02 sub05 cat08:
   *     - del only; add mutate aa sequence to amrAaStr
   *   o fun04 sec02 sub05 cat09:
   *     - del; update amino acid sequence in amrAaStr
   \***************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub05 Cat01:
   +   - find amino acid at end of deletion/insertion
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   ++varStr; /*get off '_'*/
   
   /*not likely to happen, but want to make sure*/
   if(varStr[0] == '*' || varStr[0] == '?')
   { /*If: could be any aa change*/
      secAaSC = varStr[0];
      ++varStr;
   } /*If: could be any aa change*/

   else
   { /*Else: has three letter code*/
      secAaSC = aaTripToChar_codonFun(varStr);
      varStr += 3;
   } /*Else: has three letter code*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub05 Cat02:
   +   - find position of last amino acid
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   /*this adds some extra checks, but also makes sure
   `   the end codon number is set
   */
   parseDel_fun04_sec02_sub05_cat02:;

   /*last codon in deletion/insertion*/
   cpStr = varStr;

   cpStr +=
     strToUI_base10str(varStr,&amrSTPtr->endCodonNumUI);
   if(cpStr == varStr)
      amrSTPtr->endCodonNumUI = amrSTPtr->codonNumUI;
   varStr = cpStr;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub05 Cat03:
   +   - insertion only case; add start/end aa to ref
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
         (varStr[0] & ~32) == 'I'
      && (varStr[1] & ~32) == 'N'
      && (varStr[2] & ~32) == 'S'
   ){ /*If: insertion*/
      varStr += 3; /*get off ins*/

      /*copy reference amino acids*/
      amrSTPtr->lenRefAaUI = 2;

      amrSTPtr->refAaStr =
         malloc(3 * sizeof(signed char));

      if(! amrSTPtr->refAaStr)
         goto memErr_fun04_sec03;

      amrSTPtr->refAaStr[0] = firstAaSC;
      amrSTPtr->refAaStr[1] = secAaSC;
      amrSTPtr->refAaStr[2] = '\0';

      /*+++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun04 Sec02 Sub05 Cat04:
      +   - ins case; add start/end aa to amrAaStr
      \+++++++++++++++++++++++++++++++++++++++++++++++*/

      /*copy amr amino acid sequence. will be an over
      ` estimate, but it works
      */

      cpStr = varStr;

      while(*cpStr != '\0')
      { /*Loop: get string length*/
         if(*cpStr == '*' || *cpStr == '?')
            ++cpStr;     /*single character*/
         else
            cpStr += 3; /*multipe characters*/

         ++amrSTPtr->lenAmrAaUI;
      } /*Loop: get string length*/

      amrSTPtr->lenAmrAaUI += 2;

      amrSTPtr->amrAaStr =
         calloc(
            amrSTPtr->lenAmrAaUI + 1,
            sizeof(signed char)
         );

      if(! amrSTPtr->amrAaStr)
         goto memErr_fun04_sec03;

      /*add start/end amino acids to amr aa sequence*/
      amrSTPtr->amrAaStr[0] = firstAaSC;
      amrSTPtr->amrAaStr[amrSTPtr->lenAmrAaUI - 1] =
         secAaSC;
      amrSTPtr->amrAaStr[amrSTPtr->lenAmrAaUI] = '\0';

      /*+++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun04 Sec02 Sub05 Cat05:
      +   - ins case; Add in the inserted aa sequence
      \+++++++++++++++++++++++++++++++++++++++++++++++*/

      cpStr = &amrSTPtr->amrAaStr[1];

      while(*varStr != '\0')
      { /*Loop: copy aa from varStr*/
         if(*varStr == '*' || *varStr == '?')
            *cpStr++ = *varStr++;
         else
         { /*Else: three letter codon*/
            *cpStr++ = aaTripToChar_codonFun(varStr);
            varStr += 3;
         } /*Else: three letter codon*/
      } /*Loop: copy aa from varStr*/

      goto done_fun04_sec03;
   } /*If: insertion*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub05 Cat06:
   +   - handle large duplications (> 1 amino acid)
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(   (varStr[0] & ~32) == 'D'
      && (varStr[1] & ~32) == 'U'
      && (varStr[2] & ~32) == 'P'
   ){ /*If: large duplication*/
      amrSTPtr->aaMultiDupBl = 1;

      amrSTPtr->lenRefAaUI = amrSTPtr->endCodonNumUI;
      amrSTPtr->lenRefAaUI -= amrSTPtr->codonNumUI;
      ++amrSTPtr->lenRefAaUI; /*to account for 1st aa*/

      amrSTPtr->refAaStr =
         calloc(
            amrSTPtr->lenRefAaUI + 1,
            sizeof(signed char)
         );
      if(! amrSTPtr->refAaStr)
         goto memErr_fun04_sec03;

      amrSTPtr->refAaStr[0] = firstAaSC;
      amrSTPtr->refAaStr[amrSTPtr->lenRefAaUI - 1] =
         secAaSC;
      amrSTPtr->refAaStr[amrSTPtr->lenRefAaUI] = '\0';

      /*set up the amr buffer*/
      amrSTPtr->lenAmrAaUI = amrSTPtr->lenRefAaUI << 1;

      amrSTPtr->amrAaStr =
         calloc(
            amrSTPtr->lenAmrAaUI + 1,
            sizeof(signed char)
         );
      if(! amrSTPtr->amrAaStr)
         goto memErr_fun04_sec03;
      goto done_fun04_sec03;
        /*will handle aa in reference file step*/
   } /*If: large duplication*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub05 Cat07:
   +   - deletion; add first and last aa to refAaStr
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   varStr += 3; /*get off "del"*/

   /*find number of amino acids in reference*/
   /*reference length needs to be found*/
   amrSTPtr->lenRefAaUI =
        1
      + ab_genMath(
             amrSTPtr->endCodonNumUI
           - amrSTPtr->codonNumUI
        ); /*get length of deletion*/

   amrSTPtr->refAaStr =
      calloc(
         amrSTPtr->lenRefAaUI + 1,
         sizeof(signed char)
      );
   if(! amrSTPtr->refAaStr)
      goto memErr_fun04_sec03;

   amrSTPtr->refAaStr[0] = firstAaSC;

   if(secAaSC != 0)
   { /*If: range of deletions*/
      amrSTPtr->refAaStr[amrSTPtr->lenRefAaUI - 1] =
         secAaSC;
      amrSTPtr->refAaStr[amrSTPtr->lenRefAaUI] = '\0';
   } /*If: range of deletions*/

   else
      amrSTPtr->refAaStr[2] = '\0';

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub05 Cat08:
   +   - del only; add mutate aa sequence to amrAaStr
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   /*get the amino acid sequece for the amr*/
   if(*varStr == '\0')
   { /*If: just a deletion*/
      /*copy amr amino acid sequence*/
      amrSTPtr->lenAmrAaUI = 1;

      amrSTPtr->amrAaStr =
         malloc(2 * sizeof(signed char));
      if(! amrSTPtr->amrAaStr)
         goto memErr_fun04_sec03;

      amrSTPtr->amrAaStr[0] = '0';
      amrSTPtr->amrAaStr[1] = '\0';
      goto done_fun04_sec03;
   } /*If: just a deletion*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec02 Sub05 Cat09:
   +   - del; update amino acid sequence in amrAaStr
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   /*Get off the ins*/
   varStr += 3;
   cpStr = varStr;

   while(*cpStr != '\0')
   { /*Loop: get string length*/
      if(*cpStr == '*' || *cpStr == '?')
         ++cpStr;
      else
         cpStr += 3;
      ++amrSTPtr->lenAmrAaUI;
   } /*Loop: get string length*/

   amrSTPtr->amrAaStr =
      calloc(
         amrSTPtr->lenAmrAaUI + 1,
         sizeof(signed char)
      );
   if(! amrSTPtr->amrAaStr)
      goto memErr_fun04_sec03;

   amrSTPtr->amrAaStr[amrSTPtr->lenAmrAaUI] = '\0';

   /*add new insertion in*/
   cpStr = &amrSTPtr->amrAaStr[0];

   while(*varStr != '\0')
   { /*Loop: copy aa from varStr*/
      if(*varStr == '*' || *varStr == '?')
         *cpStr++ = *varStr++;

      else
      { /*Else: three letter codon*/
         *cpStr++ = aaTripToChar_codonFun(varStr);
         varStr += 3;
      } /*Else: three letter codon*/
   } /*Loop: copy aa from varStr*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun04_sec03:;
      errSC = 0;
      goto ret_fun04_sec03;

   memErr_fun04_sec03:;
      errSC = def_memErr_addAmr;
      goto ret_fun04_sec03;

   ret_fun04_sec03:;
      return errSC;
} /*parseVarId_addAmr*/

/*-------------------------------------------------------/
| Fun05: addNtSeq_addAmr
|   - adds nucleotide sequence to AMR
| Input:
|   - varStr:
|     o c-string with variant to convert
|   - refStr:
|     o reference sequence
|   - startUI:
|     o first base in gene
|   - endUI:
|     o last base in gene
|   - revBl:
|     o 1: is reverse gene
|     o 0: is forward gene
|   - amrSTPtr:
|     o pointer to amrST struct to add variant to
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_memErr_addAmr for memory errors
|     o def_varErr_addAmr for invalid variant ids
|     o def_noVar_addAmr if no variants
|     o def_lof_addAmr if loss of function; no coordinates
|     o def_del_addAmr if deletion with no coordinates
| Note:
|   - this does not always fill in the reference or amr
|     amino acid sequence, always call addCodonPos_addAmr
|     to make sure
\-------------------------------------------------------*/
signed char
addNtSeq_addAmr(
   signed char *varStr,   /*variant to add*/
   signed char *refStr,   /*reference sequence*/
   unsigned int startUI,  /*first base in gene*/
   unsigned int endUI,    /*last base in gene*/
   signed char revBl,     /*1: for reverse gene*/
   struct amrST *amrSTPtr /*add varaint to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - adds nucleotide sequence to AMR
   '   o fun05 sec01:
   '     - variable declarations
   '   o fun05 sec02:
   '     - move to variant id type
   '   o fun05 sec03:
   '     - nucleotide variants
   '   o fun05 sec04:
   '     - amino acid variants
   '   o fun05 sec05:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siVar = 0;
   unsigned int uiRef = 0;
   signed int varStartSI = 0;
   signed int varEndSI = 0;
   unsigned int lenUI = 0;
   signed char addNtBl = 0;/*marks if could add pre base*/
   signed char afterStopBl = 0;
   signed char codeNtBl = 0; /*1: is coding nucleotide,
                             `  for reverse genes I need
                             `  to reverse complement this
                             `  since it is in gene order
                             */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - move to variant id type
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(varStr[siVar] && varStr[siVar] != '.')
      ++siVar;

   if(! varStr[siVar])
   { /*If: at end of string*/
      if(! siVar)
         goto noVar_fun05_sec05; /*empty string*/
      else if( (varStr[siVar - 1] & ~32) == 'F' )
         goto lof_fun05_sec05; /*no coordinates*/
      else if( (varStr[siVar - 1] & ~32) == 'N' )
         goto del_fun05_sec05; /*deletion varaint*/
      else
         goto varErr_fun05_sec05; /*no idea what is*/
   } /*If: at end of string*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - nucleotide variants
   ^   o fun05 sec03 sub01:
   ^     - get coordiantes of variant
   ^   o fun05 sec03 sub02:
   ^     - copy duplication
   ^   o fun05 sec03 sub03:
   ^     - copy deletion
   ^   o fun05 sec03 sub04:
   ^     - copy inversion
   ^   o fun05 sec03 sub05:
   ^     - copy insertion
   ^   o fun05 sec03 sub04:
   ^     - copy snp
   ^   o fun05 sec03 sub05:
   ^     - .c (coding?) varaints, loop back to nucleotide
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec03 Sub01:
   *   - get coordiantes of variant
   \*****************************************************/

   --siVar; /*move to variant type*/
   if( (varStr[siVar] & ~32) == 'N' )
   { /*If: nucleotide variant*/
      addNtVar_fun05_sec05:;

      siVar += 2; /*move to first base*/

      if(varStr[siVar] == '*')
         afterStopBl = 1; /*comes after stop codon*/

      siVar +=
        strToSI_base10str(&varStr[siVar], &varStartSI);
      if(varStartSI > 0)
         --varStartSI; /*convert to index 0*/

      /*find start position, I am not covering all
      `  hgvs symbols (leaving out exons (bacteria do not
      `  have)
      */
      if(afterStopBl)
      { /*If: comes after stop codon*/
         if(revBl)
            varStartSI -= startUI;
         else
            varStartSI += endUI;
      } /*If: comes after stop codon*/

      else if(revBl)
         varStartSI = endUI - varStartSI;
      else
         varStartSI += startUI;

      amrSTPtr->refPosUI = varStartSI;

      if((varStr[siVar] & ~32) == 'A')
         goto cpSnp_fun05_sec03_sub04;
      else if((varStr[siVar] & ~32) == 'T')
         goto cpSnp_fun05_sec03_sub04;
      else if((varStr[siVar] & ~32) == 'G')
         goto cpSnp_fun05_sec03_sub04;
      else if((varStr[siVar] & ~32) == 'C')
         goto cpSnp_fun05_sec03_sub04;

      else if(varStr[siVar] == '_')
      { /*Else If: have end coordinate*/
         ++siVar;
         siVar +=
            strToSI_base10str(&varStr[siVar], &varEndSI);

         if(varEndSI > 0)
            --varEndSI; /*convert to index 0*/
         if(afterStopBl)
         { /*If: comes after stop codon*/
            if(revBl)
               varEndSI -= startUI;
            else
               varEndSI += endUI;
         } /*If: comes after stop codon*/

         else if(revBl)
            varEndSI = endUI - varEndSI;
         else
            varEndSI += startUI;
      } /*Else If: have end coordinate*/

      if(! varStr[siVar])
         goto varErr_fun05_sec05; /*no idea what is*/

      /**************************************************\
      * Fun05 Sec03 Sub02:
      *   - copy duplication
      \**************************************************/

      else if(
            (varStr[siVar] & ~32) == 'D'
         && (varStr[siVar + 1] & ~32) == 'U'
      ){ /*Else If: duplication*/
         amrSTPtr->mutTypeStr[0] = 'i';
         amrSTPtr->mutTypeStr[1] = 'n';
         amrSTPtr->mutTypeStr[2] = 's';
         amrSTPtr->mutTypeStr[3] = 0;

         if(varEndSI)
            lenUI = ab_genMath(varEndSI - varStartSI);
         else
            lenUI = 1;

         if(revBl)
         { /*If: reverse complement*/
            varStartSI -= lenUI; /*get to first base*/
            ++varStartSI; /*account for index 1 lenUI*/
         } /*If: reverse complement*/


         amrSTPtr->refSeqStr =
            malloc( (lenUI + 8) * sizeof(signed char));
         if(! amrSTPtr->refSeqStr)
            goto memErr_fun05_sec05;

         amrSTPtr->amrSeqStr =
           malloc(((lenUI <<1) +8) * sizeof(signed char));
         if(! amrSTPtr->amrSeqStr)
            goto memErr_fun05_sec05;

         amrSTPtr->lenRefSeqUI = lenUI;
         amrSTPtr->lenAmrSeqUI = lenUI << 1;

         for(uiRef = 0; uiRef < lenUI; ++uiRef)
         { /*Loop: copy sequence*/
            amrSTPtr->refSeqStr[uiRef] =
               refStr[varStartSI];
            amrSTPtr->amrSeqStr[uiRef] =
               refStr[varStartSI++];
         } /*Loop: copy sequence*/

         amrSTPtr->refSeqStr[uiRef] = 0;
         cpLen_ulCp(
            &amrSTPtr->amrSeqStr[uiRef],
            amrSTPtr->refSeqStr,
            uiRef
         );
      }  /*Else If: duplication*/

      /**************************************************\
      * Fun05 Sec03 Sub03:
      *   - copy deletion
      \**************************************************/

      else if( (varStr[siVar] & ~32) == 'D')
      { /*Else If: deletion*/
         amrSTPtr->mutTypeStr[0] = 'd';
         amrSTPtr->mutTypeStr[1] = 'e';
         amrSTPtr->mutTypeStr[2] = 'l';
         amrSTPtr->mutTypeStr[3] = 0;

         siVar += 3; /*get of 'del' entry*/

         if(varEndSI)
            lenUI = ab_genMath(varEndSI - varStartSI);
         else
            lenUI = 1;

         if(revBl)
         { /*If: reverese complement sequence*/
            varStartSI -= lenUI; /*get to first base*/
            ++varStartSI; /*account for index 1 lenUI*/
         } /*If: reverese complement sequence*/


         amrSTPtr->refSeqStr =
            malloc( (lenUI + 9) * sizeof(signed char));
         if(! amrSTPtr->refSeqStr)
            goto memErr_fun05_sec05;

         amrSTPtr->lenRefSeqUI = lenUI;

         if(amrSTPtr->refPosUI > 0)
         { /*If: can add in a first base*/
            --amrSTPtr->refPosUI;
            amrSTPtr->refSeqStr[0] = refStr[varStartSI-1];
            ++lenUI;
            uiRef = 1;
            addNtBl = 1;
         } /*If: can add in a first base*/

         while(uiRef < lenUI)
            amrSTPtr->refSeqStr[uiRef++] =
               refStr[varStartSI++];
         amrSTPtr->refSeqStr[uiRef] = 0;

         while(
               varStr[siVar] > 32
            && (varStr[siVar] & ~32) != 'I'
         ) ++varStr;

         /*set up no amr sequence flag*/

         if((varStr[siVar] & ~32) == 'I')
         { /*If: deletion insertion*/
            siVar += 3; /*get of ins entry*/
            lenUI = endStr_ulCp(&varStr[siVar]);
            amrSTPtr->amrSeqStr =
               malloc((1 + 9) * sizeof(signed char));
            if(! amrSTPtr->amrSeqStr)
               goto memErr_fun05_sec05;

            if(addNtBl)
            { /*If: can add in a first base*/
               amrSTPtr->amrSeqStr[0] =
                  refStr[varStartSI - 1];
               uiRef = 1;
            } /*If: can add in a first base*/

            else
               uiRef = 0;

            cpLen_ulCp(
               &amrSTPtr->amrSeqStr[uiRef],
               &varStr[siVar],
               lenUI
            );

            amrSTPtr->lenAmrSeqUI = lenUI;

            if(lenUI == amrSTPtr->lenRefSeqUI)
            { /*If: is really a snp (del & ins hide)*/
               amrSTPtr->mutTypeStr[0] = 's';
               amrSTPtr->mutTypeStr[1] = 'n';
               amrSTPtr->mutTypeStr[2] = 'p';
               amrSTPtr->mutTypeStr[3] = 0;
            } /*If: is really a snp (del & ins hide)*/

            /*very unlikey*/
            else if(lenUI > amrSTPtr->lenRefSeqUI)
            { /*Else If: really ins hidden by deletion*/
               amrSTPtr->mutTypeStr[0] = 'i';
               amrSTPtr->mutTypeStr[1] = 'n';
               amrSTPtr->mutTypeStr[2] = 's';
               amrSTPtr->mutTypeStr[3] = 0;
            } /*Else If: really ins hidden by deletion*/
         } /*If: deletion insertion*/

         else
         { /*Else: no other bases*/
            amrSTPtr->amrSeqStr =
               malloc((1 + 8) * sizeof(signed char));
            if(! amrSTPtr->amrSeqStr)
               goto memErr_fun05_sec05;

            if(addNtBl)
               amrSTPtr->amrSeqStr[0] =
                  refStr[varStartSI - 1];
            else
               amrSTPtr->amrSeqStr[0] = '0';
            amrSTPtr->amrSeqStr[1] = 0;
            amrSTPtr->lenAmrSeqUI = 1;
         } /*Else: no other bases*/
      } /*Else If: deletion*/

      /**************************************************\
      * Fun05 Sec03 Sub04:
      *   - copy inversion
      \**************************************************/

      else if(
            (varStr[siVar] & ~32) == 'I'
         && (varStr[siVar + 2] & ~32) == 'V'
      ){ /*Else If: inversion*/
         amrSTPtr->mutTypeStr[0] = 's';
         amrSTPtr->mutTypeStr[1] = 'n';
         amrSTPtr->mutTypeStr[2] = 'p';
         amrSTPtr->mutTypeStr[3] = 0;

         if(varEndSI)
            lenUI = ab_genMath(varEndSI - varStartSI);
         else
            lenUI = 1;

         amrSTPtr->refSeqStr =
            malloc( (lenUI + 8) * sizeof(signed char));
         if(! amrSTPtr->refSeqStr)
            goto memErr_fun05_sec05;

         amrSTPtr->amrSeqStr =
            malloc((lenUI + 8) * sizeof(signed char));
         if(! amrSTPtr->amrSeqStr)
            goto memErr_fun05_sec05;

         amrSTPtr->lenRefSeqUI = lenUI;
         amrSTPtr->lenAmrSeqUI = lenUI;

         for(uiRef = 0; uiRef < lenUI; ++uiRef)
         { /*Loop: copy sequence*/
            amrSTPtr->refSeqStr[uiRef] =
               refStr[varStartSI];
            amrSTPtr->amrSeqStr[uiRef] =
               refStr[varStartSI++];
         } /*Loop: copy sequence*/

         amrSTPtr->refSeqStr[uiRef] = 0;
         amrSTPtr->amrSeqStr[uiRef] = 0;

         revCmp_addAmr(amrSTPtr->amrSeqStr);
      }  /*Else If: inversion*/

      /**************************************************\
      * Fun05 Sec03 Sub05:
      *   - copy insertion
      \**************************************************/

      else if( (varStr[siVar] & ~32) == 'I')
      { /*Else If: insertion*/
         amrSTPtr->mutTypeStr[0] = 'i';
         amrSTPtr->mutTypeStr[1] = 'n';
         amrSTPtr->mutTypeStr[2] = 's';
         amrSTPtr->mutTypeStr[3] = 0;

         siVar += 3; /*get of 'ins' entry*/
         lenUI = endStr_ulCp(&varStr[siVar]);

         if(revBl)
         { /*If: is a reverse complement sequence*/
            varStartSI -= lenUI; /*get to first base*/
            ++varStartSI; /*account for index 1 lenUI*/
         } /*If: is a reverse complement sequence*/

         amrSTPtr->amrSeqStr =
            malloc((lenUI + 9) * sizeof(signed char));
         if(! amrSTPtr->amrSeqStr)
            goto memErr_fun05_sec05;

         amrSTPtr->refSeqStr =
            malloc((1 + 8) * sizeof(signed char));
         if(! amrSTPtr->refSeqStr)
            goto memErr_fun05_sec05;
         amrSTPtr->refSeqStr[0] = refStr[varStartSI];
         amrSTPtr->refSeqStr[1] = 0;
         amrSTPtr->lenRefSeqUI = 1;

         uiRef = 0;
         amrSTPtr->lenAmrSeqUI = lenUI;
         amrSTPtr->amrSeqStr[uiRef++]= refStr[varStartSI];
         while(varStr[siVar])
           amrSTPtr->amrSeqStr[uiRef++] = varStr[siVar++];
         amrSTPtr->amrSeqStr[uiRef] = 0;
         amrSTPtr->lenAmrSeqUI = uiRef;
      } /*Else If: insertion*/

      /**************************************************\
      * Fun05 Sec03 Sub04:
      *   - copy snp
      \**************************************************/

      else
      { /*Else: bases*/
         cpSnp_fun05_sec03_sub04:;

         amrSTPtr->mutTypeStr[0] = 's';
         amrSTPtr->mutTypeStr[1] = 'n';
         amrSTPtr->mutTypeStr[2] = 'p';
         amrSTPtr->mutTypeStr[3] = 0;

         lenUI = 0;
         while(varStr[siVar] != '>')
           ++siVar; /*move to '>' symbol*/

         siVar += 1; /*get of '>' entry*/
         lenUI = endStr_ulCp(&varStr[siVar]);

         if(revBl)
         { /*If: reverse complement*/
            varStartSI -= lenUI; /*get to first base*/
            ++varStartSI; /*account for length 1 always*/
         } /*If: reverse complement*/

         amrSTPtr->refSeqStr =
            malloc((lenUI + 8) * sizeof(signed char));
         if(! amrSTPtr->refSeqStr)
            goto memErr_fun05_sec05;

         amrSTPtr->amrSeqStr =
            malloc((lenUI + 8) * sizeof(signed char));
         if(! amrSTPtr->amrSeqStr)
            goto memErr_fun05_sec05;

         amrSTPtr->lenRefSeqUI = lenUI;
         amrSTPtr->lenAmrSeqUI = lenUI;
         uiRef = 0;
         while(varStr[siVar])
         { /*Loop: copy mutation*/
           amrSTPtr->refSeqStr[uiRef] =
              refStr[varStartSI++];
           amrSTPtr->amrSeqStr[uiRef++] = varStr[siVar++];
         } /*Loop: copy mutation*/

         amrSTPtr->refSeqStr[uiRef] = 0;
         amrSTPtr->amrSeqStr[uiRef] = 0;
      } /*Else: bases*/

      /**************************************************\
      * Fun05 Sec03 Sub05:
      *   - reverse complement if needed
      \**************************************************/

      if(codeNtBl)
         revCmp_addAmr(amrSTPtr->amrSeqStr);
         /*need to reverse complement AMR sequence*/
   } /*If: nucleotide variant*/

   /*****************************************************\
   * Fun05 Sec03 Sub05:
   *   - .c (coding?) varaints, loop back into nucleotide
   \*****************************************************/

   else if( (varStr[siVar] & ~32) == 'C' )
   { /*Else If: coding variant*/
      codeNtBl = revBl;
      goto addNtVar_fun05_sec05;
   } /*Else If: coding variant*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - amino acid variants
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if( (varStr[siVar] & ~32) == 'P' )
   { /*Else If: amino acid variant*/
      if(parseVarId_addAmr(amrSTPtr))
         goto memErr_fun05_sec05;
         /*gives me the aa sequence*/
      
      if(revBl)
      { /*If: reverse complement gene*/
         amrSTPtr->refPosUI = endUI;
         amrSTPtr->refPosUI -=
            ( (amrSTPtr->endCodonNumUI - 1) * 3 );
      } /*If: reverse complement gene*/

      else
      { /*Else: forward gene*/
         amrSTPtr->refPosUI = startUI;
         amrSTPtr->refPosUI +=
            ( (amrSTPtr->endCodonNumUI - 1) * 3 );
      } /*Else: forward gene*/

      if(
            ! amrSTPtr->amrAaStr
         || amrSTPtr->amrAaStr[0] == '0'
      ) goto aaDel_fun05_sec04;

      else if(
            ! amrSTPtr->refAaStr
         || amrSTPtr->refAaStr[0] == '0'
      ) goto aaIns_fun05_sec04;

      else if(
         amrSTPtr->lenAmrAaUI == amrSTPtr->lenRefAaUI
      ){ /*Else If: snp mutation type*/
         amrSTPtr->mutTypeStr[0] = 's';
         amrSTPtr->mutTypeStr[1] = 'n';
         amrSTPtr->mutTypeStr[2] = 'p';
         amrSTPtr->mutTypeStr[3] = 0;
      } /*Else If: snp mutation type*/

      else if(amrSTPtr->lenAmrAaUI < amrSTPtr->lenRefAaUI)
      { /*Else If: deletion mutation type*/
         aaDel_fun05_sec04:;

         amrSTPtr->mutTypeStr[0] = 'd';
         amrSTPtr->mutTypeStr[1] = 'e';
         amrSTPtr->mutTypeStr[2] = 'l';
         amrSTPtr->mutTypeStr[3] = 0;

         if(
               ! amrSTPtr->amrAaStr
            || amrSTPtr->amrAaStr[0] == '0'
         ){ /*If: no AMR sequence assigned*/
            amrSTPtr->amrSeqStr =
               malloc(8 * sizeof(signed char));
            if(! amrSTPtr->amrSeqStr)
               goto memErr_fun05_sec05;
            amrSTPtr->amrSeqStr[0] = '0';
            amrSTPtr->amrSeqStr[1] = 0;
            amrSTPtr->lenAmrSeqUI = 1;
         } /*If: no AMR sequence assigned*/
      } /*Else If: deletion mutation type*/

      else
      { /*Else: insertion mutation type*/
         aaIns_fun05_sec04:;

         amrSTPtr->mutTypeStr[0] = 'i';
         amrSTPtr->mutTypeStr[1] = 'n';
         amrSTPtr->mutTypeStr[2] = 's';
         amrSTPtr->mutTypeStr[3] = 0;

         if(
               ! amrSTPtr->refAaStr
            || amrSTPtr->refAaStr[0] == '0'
         ){ /*If: no reference sequence assigned*/
            amrSTPtr->refSeqStr =
               malloc(8 * sizeof(signed char));
            if(! amrSTPtr->refSeqStr)
               goto memErr_fun05_sec05;
            amrSTPtr->refSeqStr[0] = '0';
            amrSTPtr->refSeqStr[1] = 0;
            amrSTPtr->lenRefSeqUI = 1;
         } /*If: no reference sequence assigned*/
      } /*Else: insertion mutation type*/
   } /*Else If: amino acid variant*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec05:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siVar = 0;
   goto done_fun05_sec05;

   memErr_fun05_sec05:;
      siVar = def_memErr_addAmr;
      goto done_fun05_sec05;

   varErr_fun05_sec05:;
      siVar = def_varErr_addAmr;
      goto done_fun05_sec05;

   noVar_fun05_sec05:;
      siVar = def_noVar_addAmr;
      goto done_fun05_sec05;

   lof_fun05_sec05:;
      siVar = def_lof_addAmr;
      goto done_fun05_sec05;

   del_fun05_sec05:;
      siVar = def_del_addAmr;
      goto done_fun05_sec05;

   done_fun05_sec05:;
      return (signed char) siVar;
} /*addNtSeq_addAmr*/

/*-------------------------------------------------------\
| Fun06: addCodonPos_addAmr
|   - adds amino acid sequences for deletions and
|     large duplications, reading frame orientation
|     (forward/reverse), and the first reference
|     base in the codon to an amrST that has been
|     processed with addNtSeq_addAmr
| Input:
|   - amrST:
|     o pointer to an array of amrST structs to update
|   - indexSI:
|     o >= 0; index of gene AMR is in
|     o < 0; this function will find the index of AMR gene
|   - coordsSTPtr:
|     o geneCoord struc with coordinates
|     o must be sorted by start (this is done when using
|       getCoords_geneCoord)
|   - refStr:
|     o c-string with reference seqeunce
|   - aaToSeqBl:
|     o 1: add in sequence by back convering amino acid
|          sequence
|     o 0: do not add sequence (user has or will add)
| Output:
|   - Modifies:
|     o amrST to have the amino acid sequence and starting
|       position of codon on the reference
|   - Returns:
|     o 0 for no errors
|     o 1 if could not find gene
|     o 2 for memory errors
\-------------------------------------------------------*/
signed char
addCodonPos_addAmr(
   struct amrST *amrSTPtr,   /*amr variant to update*/
   struct geneCoord *coordsSTPtr,/*has gene coordinates*/
   signed int indexSI,      /*index of gene or -1*/
   signed char *refStr,     /*reference seqeunce*/
   signed char aaToSeqBl    /*1: add sequence from aa*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC: addCodonPos_addAmr
   '   - adds amino acid sequences for deletions and
   '     large duplications, reading frame orientation
   '     (forward/reverse), and the first reference
   '     base in the codon to an amrST that has been
   '     processed with addNtSeq_addAmr
   '   o fun06 sec01:
   '     - variable declerations
   '   o fun06 sec02:
   '     - add gene coordinates and ammino acid sequences 
   '   o fun06 sec03:
   '     - finish reverese complement AA AMR sequences
   '   o fun06 sec04:
   '     - finish forward AA AMR sequences
   '   o fun06 sec05:
   '     - add in large duplication events
   '   o fun06 sec06:
   '     - get nucleotide sequence from amino acids
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *cpStr = 0;
   signed char *dupStr = 0;
   unsigned int tmpUI = 0;

   /*for dealing with aa deletions*/
   signed int siCodon = 0;
   unsigned char oneNtUC = 0;
   unsigned char twoNtUC = 0;
   unsigned char threeNtUC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - add in gene coordinates and ammino acid sequences 
   ^   o fun06 sec02 sub01:
   ^     - find gene the AMR is on
   ^   o fun06 sec02 sub02:
   ^     - check if AMR is on multiple genes/find primary
   ^   o fun06 sec02 sub04:
   ^     - add in gene starting positions
   ^   o fun06 sec02 sub05:
   ^     - processing for reverse complement AMR genes
   ^   o fun06 sec02 sub06:
   ^     - add in large duplication events
   ^   o fun06 sec02 sub07:
   ^     - clean up allocated memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec02 Sub01:
   *   - find gene the AMR is on
   \*****************************************************/

   if(indexSI >= 0)
      goto addGeneStart_fun06_sec02_sub03;
      /*can skip finding the gene*/

   if(indexSI < 0)
      indexSI =
         findStart_geneCoord(
            coordsSTPtr,
            amrSTPtr->refPosUI,
            refStr
         );

   if(indexSI < 0)
      goto missingGene_fun06_sec07; /*nothing ot do*/

   /*****************************************************\
   * Fun06 Sec02 Sub02:
   *   - check if AMR is on multiple genes/find primary
   \*****************************************************/

    /*check I the sequence is inside gene bounds*/
    tmpUI = amrSTPtr->refPosUI + amrSTPtr->lenRefSeqUI -1;
        /*-1 to convert index 1 to index 0*/

    if(tmpUI > coordsSTPtr->endAryUI[indexSI])
       goto findTrueGene_fun06_sec02_sub03;
    else if(
          amrSTPtr->refPosUI
       < coordsSTPtr->startAryUI[indexSI]
    ){ /*Else If: am outside of selected gene bounds*/
       findTrueGene_fun06_sec02_sub03:;

       /*see if this AMR is really on the next gene
       `   by comparing it to the last/next AMR
       */
       tmpUI =
          eqlNull_ulCp(
             amrSTPtr->geneIdStr,
             coordsSTPtr->idStrAry[indexSI]
          ); /*check if I have the same ids*/

       if(! tmpUI)
          ; /*correct gene; do nothing*/

       else if(indexSI > 0)
       { /*Else If: wrong gene; check previous gene*/
          tmpUI =
             eqlNull_ulCp(
                amrSTPtr->geneIdStr,
                coordsSTPtr->idStrAry[indexSI - 1]
             ); /*check if last gene was correct*/
          tmpUI |=
             eqlNull_ulCp(
                refStr,
                coordsSTPtr->refAryStr[indexSI - 1]
             ); /*check if last gene was correct*/

          if(! tmpUI)
             --indexSI; /*found gene*/
          else if(indexSI < coordsSTPtr->lenSI - 1)
             goto checkNextGene_fun06_sec02_sub03;
       } /*If: wrong gene; check previous gene*/

       else if(indexSI < coordsSTPtr->lenSI -  1)
       { /*Else If: wrong gene: check next gene*/
           checkNextGene_fun06_sec02_sub03:;

           tmpUI =
              eqlNull_ulCp(
                 amrSTPtr->geneIdStr,
                 coordsSTPtr->idStrAry[indexSI + 1]
              ); /*check if next gene was correct*/

          if(! tmpUI)
             ++indexSI;
          /*else: no idea which gene it is*/
       } /*Else If: wrong gene: check next gene*/

       /*else: no idea what gene belongs to*/
    } /*Else If: am outside of selected gene bounds*/

    /*************************************************\
    * Fun06 Sec02 Sub03:
    *   - add in gene starting positions
    \*************************************************/

    addGeneStart_fun06_sec02_sub03:;

    /*make sure the start is first mapped base*/
    if(
         coordsSTPtr->startAryUI[indexSI]
       < coordsSTPtr->endAryUI[indexSI]
    ){ /*If: start comes first*/
       amrSTPtr->geneFirstRefUI =
          coordsSTPtr->startAryUI[indexSI];

       amrSTPtr->geneLastRefUI =
          coordsSTPtr->endAryUI[indexSI];
    } /*If: start comes first*/

    else
    { /*Else: start comes last*/
       amrSTPtr->geneLastRefUI =
          coordsSTPtr->startAryUI[indexSI];

       amrSTPtr->geneFirstRefUI =
          coordsSTPtr->endAryUI[indexSI];
    } /*Else: start comes last*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun06 Sec03:
    ^   - finish reverese complement AA AMR sequences
    ^   o fun06 sec03 sub01:
    ^     - add direction, check if aa seq, find position
    ^   o fun06 sec03 sub02:
    ^     - get reference amino acid sequence for
    ^       AMR(s) in reverse complement genes
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun06 Sec03 Sub01:
    *   - add direction, check if aa seq, find position
    \****************************************************/

    if(coordsSTPtr->dirAryUC[indexSI])
    { /*If: reverse complement gene*/
       amrSTPtr->dirFlagSC = def_revCompDir_amrST;

       if(amrSTPtr->codonNumUI == 0)
          goto done_fun06_sec07;
          /*not amino acid mutation*/


       /*Update the starting position*/
       /*this assumes the codon number is correct. i
       `   had one case (pncA_p.Ter187Argext*?) were I
       `   am not sure if this was true. Ref: 2288680,
       `   codon: 187. The output was 2288675, which
       `   is off
       */
       amrSTPtr->codonPosUI =
            coordsSTPtr->endAryUI[indexSI]
          - ( (amrSTPtr->codonNumUI - 1) * 3 );
       
       if(amrSTPtr->endCodonNumUI == 0)
       { /*If: single aa change*/
          amrSTPtr->endCodonNumUI = amrSTPtr->codonNumUI;
          goto addSeq_fun06_sec06;
       } /*If: single aa change*/

       /*check to see if more than 1 or 2 aa long*/
       tmpUI =
           amrSTPtr->endCodonNumUI - amrSTPtr->codonNumUI;

       if(tmpUI < 2)
          goto addSeq_fun06_sec06;
          /*single amino acid (already set up)*/

       /*************************************************\
       * Fun06 Sec03 Sub02:
       *   - Get reference amino acid sequence for
       *     AMR(s) in reverse complement genes
       \*************************************************/

       /*set up for getting the reference amino
       `   acids; the +1 is to ignore the first amino
       `   acid in the deletion (I already have it)
       */
       dupStr = amrSTPtr->refAaStr + 1;

       /*-3 to get off the first codon*/
       cpStr = refStr + amrSTPtr->codonPosUI - 3;

       for(
          siCodon = amrSTPtr->codonNumUI + 1;
          siCodon < (signed int) amrSTPtr->endCodonNumUI;
          ++siCodon
       ){ /*loop: copy deletion*/
          oneNtUC = revNtTo2Bit[(unsigned char) *cpStr--];
          twoNtUC = revNtTo2Bit[(unsigned char) *cpStr--];
          threeNtUC=revNtTo2Bit[(unsigned char) *cpStr--];
          *dupStr++=codonTbl[oneNtUC][twoNtUC][threeNtUC];
       } /*loop: copy deletion*/
    } /*If: reverse complement gene*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun06 Sec04:
    ^   - finish forward AA AMR sequences
    ^   o fun06 sec04 sub01:
    ^     - add direction, check if aa seq, find position
    ^   o fun06 sec04 sub02:
    ^     - get reference amino acid sequence for
    ^       AMR(s) in reverse complement genes
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun06 Sec02 Sub01:
    *   - add direction, check if aa seq, find position
    \****************************************************/

    else
    { /*Else: foward gene*/
       amrSTPtr->dirFlagSC = def_forwardDir_amrST;

       if(amrSTPtr->codonNumUI == 0)
          goto done_fun06_sec07;
          /*no amino acid seqence*/


       /*update the starting position*/
       /*this assumes the codon number is correct. I
       `   had one case (pncA_p.Ter187Argext*?) were I
       `   am not sure if this was true. Ref: 2288680,
       `   codon: 187. The output was 2288675, which
       `   is off. this was an reverse case
       */
       amrSTPtr->codonPosUI =
            coordsSTPtr->startAryUI[indexSI]
          + ( (amrSTPtr->codonNumUI - 1) * 3 );

       if(amrSTPtr->endCodonNumUI == 0)
       { /*If: single aa change*/
          amrSTPtr->endCodonNumUI = amrSTPtr->codonNumUI;
          goto addSeq_fun06_sec06;
       } /*If: single aa change*/

       /*Check to see if more than 1 or 2 aa long*/
       tmpUI =
           amrSTPtr->endCodonNumUI - amrSTPtr->codonNumUI;

       if(tmpUI < 2)
          goto addSeq_fun06_sec06;
          /*single amino acid (already setup)*/

       /*************************************************\
       * Fun06 Sec04 Sub02:
       *   - get reference amino acid sequence for
       *     AMR(s) in reverse complement genes
       \*************************************************/

       /*set up for getting the reference amino
       `   acids; the +1 is to ignore the first amino
       `   acid in the deletion (I already have it)
       */
       dupStr = amrSTPtr->refAaStr + 1;

       /*+3 to get off the first codon*/
       cpStr = refStr + amrSTPtr->codonPosUI + 3;

       for(
          siCodon = amrSTPtr->codonNumUI + 1;
          siCodon < (signed int) amrSTPtr->endCodonNumUI;
          ++siCodon
       ){ /*Loop: copy deletion*/
          oneNtUC = ntTo2Bit[(unsigned char) *cpStr++];
          twoNtUC = ntTo2Bit[(unsigned char) *cpStr++];
          threeNtUC = ntTo2Bit[(unsigned char) *cpStr++];
          *dupStr++=codonTbl[oneNtUC][twoNtUC][threeNtUC];
       } /*Loop: copy deletion*/
    } /*Else: foward gene*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun06 Sec05:
    ^   - add in large duplication events
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(amrSTPtr->aaMultiDupBl)
    { /*If: large duplicate*/
       dupStr = amrSTPtr->amrAaStr;

       cpStr = amrSTPtr->refAaStr;
       while(*cpStr != '\0')
          *dupStr++ = *cpStr++;

       cpStr = amrSTPtr->refAaStr;
       while(*cpStr != '\0')
          *dupStr++ = *cpStr++;
       *dupStr = '\0';
    } /*If: large duplicate*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun06 Sec06:
    ^   - get nucleotide sequence from amino acids
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    addSeq_fun06_sec06:;

    if(aaToSeqBl)
    { /*If: adding sequences to amr*/
       if(amrSTPtr->refAaStr && *amrSTPtr->refAaStr !='0')
       { /*If: have reference amino acid sequence*/
          tmpUI = amrSTPtr->lenRefAaUI * 3;

          if(! amrSTPtr->refSeqStr) ;
          else if(tmpUI >= amrSTPtr->lenRefSeqUI)
          { /*If: need more memory*/
             free(amrSTPtr->refSeqStr);
             amrSTPtr->refSeqStr = 0;
          } /*If: need more memory*/

          if(! amrSTPtr->refSeqStr)
          { /*If need to assign memory*/
             amrSTPtr->refSeqStr =
                malloc((tmpUI + 8) * sizeof(signed char));
             if(! amrSTPtr->refSeqStr)
                goto memErr_fun06_sec07;
          } /*If need to assign memory*/

          dupStr = amrSTPtr->refSeqStr;
          cpStr = &refStr[amrSTPtr->codonPosUI];

          if(amrSTPtr->dirFlagSC & def_revCompDir_amrST)
             cpStr -= tmpUI - 1;

          tmpUI = amrSTPtr->codonPosUI;
          siCodon = 0;

          while(amrSTPtr->refAaStr[siCodon])
          { /*Loop: add reference bases in*/
            *dupStr++ = *cpStr++;
            ++amrSTPtr->lenRefSeqUI;

            if(! (amrSTPtr->lenRefSeqUI % 3))
               ++siCodon;
          } /*Loop: add reference bases in*/

	       *dupStr = 0;
       } /*If: have reference amino acid sequence*/


       if(amrSTPtr->amrAaStr && *amrSTPtr->amrAaStr !='0')
       { /*If: have AMR amino acid sequence*/
          tmpUI = amrSTPtr->lenAmrAaUI * 3;

          if(! amrSTPtr->amrSeqStr) ;
          else if(tmpUI >= amrSTPtr->lenAmrSeqUI)
          { /*If: need more memory*/
             free(amrSTPtr->amrSeqStr);
             amrSTPtr->amrSeqStr = 0;
          } /*If: need more memory*/

          if(! amrSTPtr->amrSeqStr)
          { /*If need to assign memory*/
             amrSTPtr->amrSeqStr =
                malloc((tmpUI + 8) * sizeof(signed char));
             if(! amrSTPtr->amrSeqStr)
                goto memErr_fun06_sec07;
          } /*If need to assign memory*/

          amrSTPtr->lenAmrSeqUI =
             aaToSeq_addAmr(
                amrSTPtr->amrAaStr,
                amrSTPtr->amrSeqStr,
                amrSTPtr->refAaStr
             ); /*convert amino acids to a valid
                ` nucleotide sequence
                */
          if(coordsSTPtr->dirAryUC[indexSI])
             revCmp_addAmr(amrSTPtr->amrSeqStr);
       } /*If: have AMR amino acid sequence*/
    } /*If: adding sequences to amr*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec07:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun06_sec07:;
      indexSI = 0;
      goto ret_fun06_sec07;

   missingGene_fun06_sec07:;
      indexSI = 1;
      goto ret_fun06_sec07;

   memErr_fun06_sec07:;
      indexSI = 2;
      goto ret_fun06_sec07;

   ret_fun06_sec07:;
     return (signed char) indexSI;
} /*addCodonPos_addAmr*/

/*-------------------------------------------------------\
| Fun07: var_addAmr
|   - adds a variant to an AMR struct, but does not
|     complete (call addCodonPos_addAmr)
| Input:
|   - varStr:
|     o variant id to process, should be in form of
|       * "gene_p.refAAnumberAMRAA" (ex gyrB_p.Ser447Phe)
|       * "gene_LoF"
|       * "gene_deletion"
|   - geneStr:
|     o c-string with name of gene to add
|   - coordsSTPtr:
|     o geneCoord struct pointer with gene coordinates
|     o needs to be sorted by gene name
|       * use sortName_geneCoord()
|   - refIdStr:
|     o c-string with reference id
|   - refSeqStr:
|     o c-string with the reference sequence
|   - errSCPtr:
|     o pointer to signed char to get returned error type
| Output:
|   - Modifies:
|     o errSCPtr to have error types:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_noVar_addAmr if variant id is empty
|   - Returns:
|     o pointer to amrST struct with added variant
|     o 0 for memory errors
\-------------------------------------------------------*/
struct amrST *
var_addAmr(
   signed char *varStr,      /*variant to add*/
   signed char *geneStr,     /*name of gene to add*/
   struct geneCoord *coordsSTPtr,/*has gene coordaintes*/
   signed char *refIdStr,      /*reference id*/
   signed char *refSeqStr,     /*reference sequence*/
   signed char *errSCPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - adds a variant to an AMR struct, but does not
   '     complete (call addCodonPos_addAmr)
   '   o fun07 sec01:
   '     - variable declarations
   '   o fun07 sec02:
   '     - add variant id and gene name to struct
   '   o fun07 sec03:
   '     - find gene and add AMR entries
   '   o fun07 sec04:
   '     - finish filling lof and gene deletion entries
   '   o fun07 sec05:
   '     - return and clean up if needed
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siPos = 0;
   struct amrST *amrHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - add variant id and gene name to struct
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   amrHeapST = malloc(sizeof(struct amrST));
   if(! amrHeapST)
      goto memErr_fun07_sec05;
   init_amrST(amrHeapST);

   amrHeapST->lenGeneIdUI = endStr_ulCp(geneStr);
   amrHeapST->geneIdStr =
      malloc(
        (amrHeapST->lenGeneIdUI + 8) * sizeof(signed char)
      );
   if(! amrHeapST->geneIdStr)
      goto memErr_fun07_sec05;
   cpLen_ulCp(
      amrHeapST->geneIdStr,
      geneStr,
      amrHeapST->lenGeneIdUI
   );

   amrHeapST->lenVarIdUI = endStr_ulCp(varStr);
   if(! amrHeapST->lenVarIdUI)
      goto noVar_fun07_sec05;
   amrHeapST->varIdStr =
      malloc(
         (amrHeapST->lenVarIdUI + 8) * sizeof(signed char)
      );
   if(! amrHeapST->varIdStr)
      goto noVar_fun07_sec05;
   cpLen_ulCp(
      amrHeapST->varIdStr,
      varStr,
      amrHeapST->lenVarIdUI
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - find gene and add AMR entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siPos =
      findName_geneCoord(coordsSTPtr, geneStr, refIdStr);

   if(siPos >= coordsSTPtr->lenSI)
      goto noGeneErr_fun07_sec05;
      /*could not find gene in list*/
   else if(siPos < 0)
      goto noGeneErr_fun07_sec05;
      /*could not find gene in list*/
   
   *errSCPtr =
      addNtSeq_addAmr(
         varStr,
         refSeqStr,
         coordsSTPtr->startAryUI[siPos],
         coordsSTPtr->endAryUI[siPos],
         coordsSTPtr->dirAryUC[siPos],
         amrHeapST
      ); /*get nucleotide sequence (if can)*/
         
   if(! *errSCPtr)
      ; /*no errors*/
   else if(*errSCPtr == def_memErr_addAmr)
      goto memErr_fun07_sec05;
   else if(*errSCPtr == def_del_addAmr)
      goto lofOrDel_fun07_sec04;
      /*gene deletion, add in at end*/
   else if(*errSCPtr == def_lof_addAmr)
      goto lofOrDel_fun07_sec04;
      /*gene loss of function, add in at end*/
   else
      goto varErr_fun07_sec05;

   *errSCPtr =
      addCodonPos_addAmr(
            amrHeapST,
            coordsSTPtr,/*gene coordinate list*/
            siPos,      /*coordinates of gene*/
            refSeqStr,  /*reference sequence*/
            1           /*want to get sequences*/
      ); /*finish the amino acid entries*/

   if(! *errSCPtr)
      ;
   else if(*errSCPtr == 1)
      goto noGeneErr_fun07_sec05;
   else
      goto memErr_fun07_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - finish filling lof and gene deletion entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*errSCPtr)
   { /*If: had lof or gene deletion*/
      lofOrDel_fun07_sec04:;

      amrHeapST->refPosUI=coordsSTPtr->startAryUI[siPos];
      amrHeapST->dirFlagSC =
         (signed char) coordsSTPtr->dirAryUC[siPos];

      amrHeapST->geneFirstRefUI =
         coordsSTPtr->startAryUI[siPos];

      amrHeapST->geneLastRefUI =
         coordsSTPtr->endAryUI[siPos];

      if(*errSCPtr == def_lof_addAmr)
      { /*If: had loss of function*/
         amrHeapST->frameshiftBl = 1;
         amrHeapST->mutTypeStr[0] = 'l';
         amrHeapST->mutTypeStr[1] = 'o';
         amrHeapST->mutTypeStr[2] = 'f';
         amrHeapST->mutTypeStr[3] = 0;
      } /*If: had loss of function*/

      else
      { /*Else: is gene deletion*/
         amrHeapST->mutTypeStr[0] = 'd';
         amrHeapST->mutTypeStr[1] = 'e';
         amrHeapST->mutTypeStr[2] = 'l';
         amrHeapST->mutTypeStr[3] = 0;
      } /*Else: is gene deletion*/

      amrHeapST->wholeGeneFlagSC = 1;

      amrHeapST->lenRefSeqUI =
           coordsSTPtr->endAryUI[siPos]
         - coordsSTPtr->startAryUI[siPos];
      amrHeapST->refSeqStr =
         malloc(
            (amrHeapST->lenRefSeqUI + 8)
               * sizeof(signed char)
         );
      if(! amrHeapST->refSeqStr)
         goto memErr_fun07_sec05;
      cpLen_ulCp(
         amrHeapST->refSeqStr,
         &refSeqStr[ coordsSTPtr->startAryUI[siPos] ],
         amrHeapST->lenRefSeqUI
      );

      /*copy the entire gene for the event*/
      amrHeapST->amrSeqStr =
         malloc(9 * sizeof(signed char));
      if(! amrHeapST->amrSeqStr)
         goto memErr_fun07_sec05;
      amrHeapST->amrSeqStr[0] = '0';
      amrHeapST->amrSeqStr[1] = 0;
      amrHeapST->lenAmrSeqUI = 1;
      amrHeapST->lenAmrAaUI = 0;
   } /*If: had lof or gene deletion*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec05:
   ^   - return and clean up if needed
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun07_sec05;
 
   memErr_fun07_sec05:;
      *errSCPtr = def_memErr_addAmr;
      goto errClean_fun07_sec05;

   noVar_fun07_sec05:;
      *errSCPtr = def_noVar_addAmr;
      goto errClean_fun07_sec05;

   noGeneErr_fun07_sec05:;
      *errSCPtr = def_noGene_addAmr;
      goto errClean_fun07_sec05;

   /*general error for failed to complete variant*/
   varErr_fun07_sec05:;
      *errSCPtr = def_varErr_addAmr;
      goto errClean_fun07_sec05;

   errClean_fun07_sec05:;
      if(amrHeapST)
         freeHeap_amrST(amrHeapST);
      amrHeapST = 0;
      goto ret_fun07_sec05;

   ret_fun07_sec05:;
      return amrHeapST;
} /*var_addAmr*/

/*-------------------------------------------------------\
| Fun08: addDrugs_addAmr:
|   - add anti-microbials (antibiotics) to an AMR
| Input:
|   - amrSTPtr:
|     o amrST struct pointer to add drugs to
|   - newDrugStr:
|     o c-string with new drug to add to amrSTPtr
|   - crossResBl:
|     o 1: set drug as cross resistant in amrSTPtr
|     o 0: set drug as primrary resistant in amrSTPtr
|   - drugAryStr:
|     o c-string pointer with antiobitics names
|     o this is a an array, so only update with functions
|       in drugAry.c/h
|     o you can set drugAryStr to 0 to have this function
|       allocate memory for you
|       * signed char *drugAryStr = 0;
|       * signed int drugLenSI = 0;
|       * signed int drugSizeSI = 0;
|       * sigend char errorSC =
|            addDrugs_addAmr(
|              amrSTPtr,
|              (signed char *) "PMD",
|              0,
|              &drugAryStr,
|              &drugLenSI,
|              &drugSizeSI
|            );
|   - drugLenSIPtr:
|     o signed int pointer with number drugs in drugAryStr
|   - drugSizeSIPtr:
|     o signed int pointer with maximum number of drugs
|       that can be in drugAryStr before resizing
| Output:
|   - Modifies:
|     o amrSTPtr to have the new drug
|     o drugAryStr to have new drug if not already in it
|     o drugLenSIPtr to have new drugAryStr length if a
|       new drug was added
|     o drugSizeSIPtr to have new drugAryStr size if
|       drugAryStr is resized
|   - Returns:
|     o 0 for no errors
|     o def_memErr_addAmr for memory errors
|     o def_overflow_addAmr if went past the maximum drug
|       limit
\-------------------------------------------------------*/
signed char
addDrugs_addAmr(
   struct amrST *amrSTPtr,  /*add drug to*/
   signed char *newDrugStr, /*drug to add*/
   signed char crossResBl,  /*1: is cross resistance*/
   signed char **drugAryStr,/*has drugs already added*/
   signed int *drugLenSIPtr,/*number drugs in drugAryStr*/
   signed int *drugSizeSIPtr/*max drugs in drugAryStr*/
){
   signed int indexSI = 0;
   signed int flagIndexSI = 0; /*index of flag array*/
   signed char *tmpStr = 0;

   if(! *drugAryStr)
   { /*If: need memory for array*/
      *drugAryStr = alloc_drugAry(16);
      if(! *drugAryStr)
         goto memErr_fun07_sec0x;
      *drugLenSIPtr = 0;
      *drugSizeSIPtr = 16;
   } /*If: need memory for array*/

   indexSI =
      find_drugAry(
         newDrugStr,
         *drugAryStr,
         *drugLenSIPtr,
         0
      ); /*see if drug is new or old*/

   if(indexSI < 0)
   { /*If: the drug is new; add to drug array*/
      if(*drugLenSIPtr >= *drugSizeSIPtr)
      { /*If: need more memory*/
         *drugSizeSIPtr += (*drugSizeSIPtr >> 1);
         tmpStr =
            realloc_drugAry(*drugAryStr, *drugSizeSIPtr);
         if(! tmpStr)
            goto memErr_fun07_sec0x;
         *drugAryStr = tmpStr;
      } /*If: need more memory*/

      indexSI = *drugLenSIPtr;
      cpDrug_drugAry(*drugAryStr, newDrugStr, indexSI, 0);
      ++*drugLenSIPtr;
   }  /*If: the drug is new; add to drug array*/

   if( indexSI > (signed int) (sizeof(unsigned long) << 3) )
      flagIndexSI = indexSI % (sizeof(unsigned long) <<3);
      /*find correct array index for this drug*/
   else
      flagIndexSI = 0;

   if(flagIndexSI > def_maxDrugs_amrST)
      goto maxDrugErr_fun07_sec0x;

   if(crossResBl)
      amrSTPtr->crossResFlagsUL[flagIndexSI] =
         ((unsigned long) 1 << indexSI);
   else
      amrSTPtr->amrFlagsUL[flagIndexSI] =
         ((unsigned long) 1 << indexSI);

   return 0;

   memErr_fun07_sec0x:;
      return def_memErr_addAmr;
   maxDrugErr_fun07_sec0x:;
      return def_overflow_addAmr;
} /*addDrugs_addAmr*/

/*-------------------------------------------------------\
| Fun09: lineToVar_addAmr
|   - convert line from variant file to tbAmr entry
| Input:
|   - lineStr:
|     o c-string with line to convert
|   - refIndexSIPtr:
|     o signed int pointer to get the index of the
|       reference used
|   - coordsSTPtr:
|     o geneCoord struct pointer with genes to map to
|   - refSeqArySTPtr:
|     o seqST struct array with the reference sequences
|       sorted by reference name (use `sort_seqST()`)
|   - refSeqLenSL:
|     o number of reference sequences
|   - drugAryStr:
|     o pointer to c-string (techincally array) with drugs
|       to add to
|   - drugLenSIPtr:
|     o signed int pointer with number drugs in drugAryStr
|   - drugSizeSIPtr:
|     o signed int pointer with maximum number of drugs
|       that can be in drugAryStr before resizing
|   - errSCPtr:
|     o signed char to get errors
|   - errPosSLPtr:
|     o signed long pointer to get positon of error in
|       line
| Output:
|   - Modifies:
|     o varFILE to be on next line after variant
|     o refIndexSIPtr to have -1 or the index of the
|       reference used
|     o drugAryStr to have new drugs
|       * realloced or malloced if need to get more memory
|     o drugLenSIPtr to have new drugAryStr length if a
|       new drug was added
|     o drugSizeSIPtr to have new drugAryStr size if
|       drugAryStr is resized
|     o errSCPtr:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_fileErr_addAmr for file errors
|       * def_noVar_addAmr if variant id is empty
|       * def_varErr_addAmr if something is wrong with the
|         varaint entry
|       * def_overflow_addAmr if went past the maximum
|         drug limit (for an amrST struct)
|       * def_eof_addAmr if end of file (no variant)
|       * def_noRef_addAmr if no reference found
|     o errPosSLPtr to get position of error in line
|   - Returns:
|     o 0 for errors
|     o amrST struct with new AMR
\-------------------------------------------------------*/
struct amrST *
lineToVar_addAmr(
   signed char *lineStr,         /*has variant id*/
   signed int *refIndexSIPtr,    /*gets reference used*/
   struct geneCoord *coordsSTPtr,/*has gene coordaintes*/
   struct seqST *refSeqArySTPtr, /*reference sequences*/
   signed long refSeqLenSL,  /*number of references*/
   signed char **drugAryStr, /*has drugs in database*/
   signed int *drugLenSIPtr,/*number drugs in drugAryStr*/
   signed int *drugSizeSIPtr,/*max drugs in drugAryStr*/
   signed char *errSCPtr,    /*gets errors*/
   signed long *errPosSLPtr  /*positon in line error at*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - convert line from variant file to tbAmr entry
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - get line with variant id
   '   o fun09 sec03:
   '     - get enough information to process the variant
   '   o fun09 sec04:
   '     - process variant
   '   o fun09 sec05:
   '     - add drugs and other flags to AMR
   '   o fun09 sec06:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tmpStr = 0;
   signed char *varStr = 0;      /*to variant id*/
   signed char *afterVarStr = 0; /*entry after variant*/

   signed char refStr[128]; /*has reference id*/

   #define def_maxGeneId_fun09 128
   signed char geneStr[def_maxGeneId_fun09 + 8];

   signed char oldCharSC = 0;
   signed char *oldPosStr = 0;

   signed long lenSL = 0;

   struct amrST *amrHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - get line with variant id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *refIndexSIPtr = -1;

   for(lenSL = 0; lenSL < def_maxGeneId_fun09; ++lenSL)
      geneStr[lenSL] = 0;
   lenSL = 0;

   /*check if line is empty*/
   tmpStr = lineStr;

   while(*tmpStr < 33)
   { /*Loop: find first item in line*/
      if(*lineStr == '\0')
         goto noVar_fun09_sec06;
      ++tmpStr;
   } /*Loop: find first item in line*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - get enough information to process the variant
   ^   o fun09 sec03 sub01:
   ^     - get variant id and if possible extract gene id
   ^   o fun09 sec03 sub02:
   ^     - get reference assigned to
   ^   o fun09 sec03 sub03:
   ^     - get gene name (if missing)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec03 Sub01:
   *   - get variant id and if possible extract gene id
   \*****************************************************/

   varStr = lineStr;

   while(*tmpStr > 32)
      ++tmpStr;

   if(*tmpStr == '\0')
      goto noVar_fun09_sec06;
      /*nothing in variant line to add to database*/

   oldPosStr = tmpStr;
   oldCharSC = *tmpStr;
   *tmpStr++ = 0;

   while(*tmpStr < 33)
   { /*Loop: find next entry*/
      if(*tmpStr == '\0')
         goto noVar_fun09_sec06;
         /*at best only variant id given*/
      ++tmpStr;
   } /*Loop: find next entry*/

   lenSL = 0;
   while(varStr[lenSL] && varStr[lenSL] != '_')
      ++lenSL;

   if(! varStr[lenSL])
      geneStr[0] = '\0';
   else if(varStr[lenSL + 2] == '.')
   { /*If: have gene name*/
      getGeneId_fun09_sec03_sub01:;

      lenSL = 0;
      while(varStr[lenSL] != '_' && varStr[lenSL])
      { /*Loop: copy the gene name*/
         geneStr[lenSL] = varStr[lenSL];
         ++lenSL;
      } /*Loop: copy the gene name*/

      geneStr[lenSL] = 0;
      lenSL = 0;
   } /*If: have gene name*/

   else if(
         (varStr[lenSL + 1] & ~32) == 'L'
      && (varStr[lenSL + 2] & ~32) == 'O'
      && (varStr[lenSL + 3] & ~32) == 'F'
   ) goto getGeneId_fun09_sec03_sub01;

   else if(
         (varStr[lenSL + 1] & ~32) == 'D'
      && (varStr[lenSL + 2] & ~32) == 'E'
      && (varStr[lenSL + 3] & ~32) == 'L'
      && (varStr[lenSL + 4] & ~32) == 'E'
      && (varStr[lenSL + 5] & ~32) == 'T'
      && (varStr[lenSL + 6] & ~32) == 'I'
      && (varStr[lenSL + 7] & ~32) == 'O'
      && (varStr[lenSL + 8] & ~32) == 'N'
   ) goto getGeneId_fun09_sec03_sub01;

   afterVarStr = tmpStr;

   /*****************************************************\
   * Fun09 Sec03 Sub02:
   *   - get reference assigned to
   \*****************************************************/

   refStr[0] = 0;
   while(*tmpStr)
   { /*Loop: get flags in file*/
      if(tmpStr[0] != '-')
         ;
      else if(tmpStr[1] != 'r')
         ;
      else if(tmpStr[2] != 'e')
         ;
      else if(tmpStr[3] != 'f')
         ;
      else if(tmpStr[4] > 32)
         ;
      else
      { /*Else: found the reference flag*/
         tmpStr += 4;
         while(*tmpStr < 33)
         { /*Loop: find start of gene name*/
            if(! *tmpStr)
               goto varErr_fun09_sec06;
            ++tmpStr;
         } /*Loop: find start of gene name*/

         lenSL = 0;
         while(*tmpStr > 32)
            refStr[lenSL++] = *tmpStr++;
         refStr[lenSL] = 0;
         break;
      } /*Else: found the reference flag*/

      ++tmpStr;
   } /*Loop: get flags in file*/

   if(! refStr[0])
      goto noRef_fun09_sec06;

   *refIndexSIPtr =
      search_seqST(refSeqArySTPtr, refStr, refSeqLenSL);
   if(*refIndexSIPtr < 0)
      goto noRef_fun09_sec06;

   while(*tmpStr && *tmpStr < 33)
      ++tmpStr;
   if(! *tmpStr)
      goto varErr_fun09_sec06;
   afterVarStr = tmpStr;

   /*****************************************************\
   * Fun09 Sec03 Sub03:
   *   - get gene name (if missing)
   \*****************************************************/

   if(! *geneStr)
   { /*If: need to grab the gene name from user input*/
      while(*tmpStr)
      { /*Loop: get flags in file*/
         if(tmpStr[0] != '-')
            ;
         else if(tmpStr[1] != 'g')
            ;
         else if(tmpStr[2] != 'e')
            ;
         else if(tmpStr[3] != 'n')
            ;
         else if(tmpStr[4] != 'e')
            ;
         else if(tmpStr[5] > 32)
            ;
         else
         { /*Else: found the gene flag*/
            tmpStr += 5;
            while(*tmpStr < 33)
            { /*Loop: find start of gene name*/
               if(! *tmpStr)
                  goto varErr_fun09_sec06;
               ++tmpStr;
            } /*Loop: find start of gene name*/

            lenSL = 0;
            while(*tmpStr > 32)
               geneStr[lenSL++] = *tmpStr++;
            geneStr[lenSL] = 0;
         } /*Else: found the gene flag*/
      } /*Loop: get flags in file*/

      while(*tmpStr && *tmpStr < 33)
         ++tmpStr;
      if(! *tmpStr)
         goto varErr_fun09_sec06;
      afterVarStr = tmpStr;
   } /*If: need to grab the gene name from user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - process variant
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   amrHeapST =
      var_addAmr(
         varStr,
         geneStr,
         coordsSTPtr,
         refStr, /*reference id*/
         refSeqArySTPtr[*refIndexSIPtr].seqStr,
         errSCPtr
       );

   *oldPosStr = oldCharSC;
   oldPosStr = 0;

   if(! *errSCPtr)
      ;
   else if(*errSCPtr == def_memErr_addAmr)
      goto memErr_fun09_sec06;
   else
      goto noVar_fun09_sec06; /*missing gene or no var*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec05:
   ^   - add drugs and other flags to AMR
   ^   o fun09 sec05 sub01:
   ^     - check if have mutataion grade, also start loop
   ^   o fun09 sec05 sub02:
   ^     - check resitance types
   ^   o fun09 sec05 sub03:
   ^     - check if mutation needs a gene
   ^   o fun09 sec05 sub04:
   ^     - check if user provided a comment
   ^   o fun09 sec05 sub05:
   ^     - check if effect entry
   ^   o fun09 sec05 sub06:
   ^     - add resitant drugs
   ^   o fun09 sec05 sub07:
   ^     - deal with white space or invalid entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec05 Sub01:
   *   - check if have a mutataion grade, also start loop
   \*****************************************************/

   amrHeapST->highResBl = def_highRes_addAmr;
   amrHeapST->lowResBl = def_lowRes_addAmr;
   amrHeapST->additiveResBl = def_addRes_addAmr;
   amrHeapST->gradeSC = def_grade_addAmr;

   tmpStr = afterVarStr;
   while(*tmpStr)
   { /*Loop: get and add remaing flags*/
      if(
        ! eqlWhite_ulCp((signed char *) "-grade-1",tmpStr)
      ){ /*If: grade 1 mutation*/
         amrHeapST->gradeSC = 1;
         tmpStr += 8;
      }  /*If: grade 1 mutation*/

      else if(
        ! eqlWhite_ulCp((signed char *) "-grade-2",tmpStr)
      ){ /*Else If: grade 2 mutation*/
         amrHeapST->gradeSC = 2;
         tmpStr += 8;
      }  /*Else If: grade 2 mutation*/

      else if(
        ! eqlWhite_ulCp((signed char *) "-grade-3",tmpStr)
      ){ /*Else If: grade 3 mutation*/
         amrHeapST->gradeSC = 3;
         tmpStr += 8;
      }  /*Else If: grade 3 mutation*/

      else if(
        ! eqlWhite_ulCp((signed char *) "-grade-4",tmpStr)
      ){ /*Else If: grade 4 mutation*/
         amrHeapST->gradeSC = 4;
         tmpStr += 8;
      }  /*Else If: grade 4 mutation*/

      else if(
        ! eqlWhite_ulCp((signed char *) "-grade-5",tmpStr)
      ){ /*Else If: grade 5 mutation*/
         amrHeapST->gradeSC = 5;
         tmpStr += 8;
      }  /*Else If: grade 5 mutation*/

      /**************************************************\
      * Fun09 Sec05 Sub02:
      *   - check resitance types
      \**************************************************/

      else if(
        ! eqlWhite_ulCp((signed char *)"-high-res",tmpStr)
      ){ /*Else If: high resistant mutation*/
         amrHeapST->highResBl = 1;
         amrHeapST->lowResBl = 0;
         tmpStr += 9;
      }  /*Else If: high resistant mutation*/

      else if(
         ! eqlWhite_ulCp(
            (signed char *) "-no-high-res",
            tmpStr
         )
      ){ /*Else If: mutation is not high resistance*/
         amrHeapST->highResBl = 0;
         tmpStr += 12;
      }  /*Else If: mutation is not high resistance*/

      else if(
        ! eqlWhite_ulCp((signed char *) "-low-res",tmpStr)
      ){ /*Else If: low resistant mutation*/
         amrHeapST->lowResBl = 1;
         amrHeapST->highResBl = 0;
         tmpStr += 8;
      }  /*Else If: low resistant mutation*/

      else if(
         ! eqlWhite_ulCp(
            (signed char *) "-no-low-res",
            tmpStr
         )
      ){ /*Else If: mutation is not low resistance*/
         amrHeapST->lowResBl = 0;
         tmpStr += 11;
      }  /*Else If: mutation is not low resistance*/

      else if(
         ! eqlWhite_ulCp(
             (signed char *) "-add-res",
             tmpStr
         )
      ){ /*Else If: additive resistant mutation*/
         amrHeapST->additiveResBl = 1;
         tmpStr += 8;
      }  /*Else If: is additive resistant mutation*/

      else if(
         ! eqlWhite_ulCp(
            (signed char *) "-no-add-res",
            tmpStr
         )
      ){ /*Else If: mutation is not additive resistance*/
         amrHeapST->additiveResBl = 0;
         tmpStr += 11;
      }  /*Else If: mutation is not additive resistance*/

      /**************************************************\
      * Fun09 Sec05 Sub03:
      *   - check if mutation needs a gene
      \**************************************************/

      else if(
         ! eqlWhite_ulCp((signed char *) "-needs", tmpStr)
      ){ /*Else If: mutation needs gene for resistance*/
         if(amrHeapST->needsGeneStr)
            free(amrHeapST->needsGeneStr);
         amrHeapST->needsGeneStr = 0;
            /*in case user called multiple times*/

         tmpStr += 7;
         while(*tmpStr && *tmpStr < 33)
            ++tmpStr;
         if(! *tmpStr)
            goto varErr_fun09_sec06;

         lenSL = endWhite_ulCp(tmpStr);


         amrHeapST->needsGeneStr =
            malloc((lenSL + 8) * sizeof(signed char));
         if(! amrHeapST->needsGeneStr)
            goto memErr_fun09_sec06;
         amrHeapST->lenNeedsGeneUI = lenSL;

         cpLen_ulCp(amrHeapST->needsGeneStr,tmpStr,lenSL);
         tmpStr += lenSL;
      }  /*Else If: mutation needs gene for resistance*/

      /**************************************************\
      * Fun09 Sec05 Sub04:
      *   - check if user provided a comment
      \**************************************************/

      else if(
         ! eqlWhite_ulCp((signed char *) "-note", tmpStr)
      ){ /*Else If: mutation needs gene for resistance*/
         if(amrHeapST->commentStr)
            free(amrHeapST->commentStr);
         amrHeapST->commentStr = 0;
            /*in case user called multiple times*/

         tmpStr += 5;
         while(*tmpStr && *tmpStr < 33)
            ++tmpStr;
         if(! *tmpStr)
            goto varErr_fun09_sec06;

         lenSL = endWhite_ulCp(tmpStr);


         amrHeapST->commentStr =
            malloc((lenSL + 8) * sizeof(signed char));
         if(! amrHeapST->commentStr)
            goto memErr_fun09_sec06;
         amrHeapST->lenCommentUI = lenSL;

         cpLen_ulCp(amrHeapST->commentStr, tmpStr, lenSL);
         tmpStr += lenSL;
      }  /*Else If: mutation needs gene for resistance*/

      /**************************************************\
      * Fun09 Sec05 Sub05:
      *   - check if effect entry
      \**************************************************/

      else if(
         ! eqlWhite_ulCp((signed char *) "-effect",tmpStr)
      ){ /*Else If: effect of the mutation*/
         if(amrHeapST->effectStr)
            free(amrHeapST->effectStr);
         amrHeapST->effectStr = 0;
            /*in case user called multiple times*/

         tmpStr += 8;
         while(*tmpStr && *tmpStr < 33)
            ++tmpStr;
         if(! *tmpStr)
            goto varErr_fun09_sec06;

         lenSL = endWhite_ulCp(tmpStr);


         amrHeapST->effectStr =
            malloc((lenSL + 8) * sizeof(signed char));
         if(! amrHeapST->effectStr)
            goto memErr_fun09_sec06;
         amrHeapST->lenEffectUI = lenSL;

         cpLen_ulCp(amrHeapST->effectStr, tmpStr, lenSL);
      }  /*Else If: effect of the mutation*/

      /**************************************************\
      * Fun09 Sec05 Sub06:
      *   - add resitant drugs
      *   o fun09 sec05 sub06 cat01:
      *     - amikacin
      *   o fun09 sec05 sub06 cat02:
      *     - bedaquiline
      *   o fun09 sec05 sub06 cat03:
      *     - capreomycin
      *   o fun09 sec05 sub06 cat04:
      *     - clofazimine
      *   o fun09 sec05 sub06 cat05:
      *     - delamanid
      *   o fun09 sec05 sub06 cat06:
      *     - ethambutol
      *   o fun09 sec05 sub06 cat07:
      *     - ethionamide
      *   o fun09 sec05 sub06 cat08:
      *     - fluoroquine
      *   o fun09 sec05 sub06 cat09:
      *     - isoniazid
      *   o fun09 sec05 sub06 cat10:
      *     - kanamycin
      *   o fun09 sec05 sub06 cat11:
      *     - levofloxacin
      *   o fun09 sec05 sub06 cat12:
      *     - linezolid
      *   o fun09 sec05 sub06 cat13:
      *     - moxifloxacin
      *   o fun09 sec05 sub06 cat14:
      *     - protonimid
      *   o fun09 sec05 sub06 cat15:
      *     - pyrazinamide
      *   o fun09 sec05 sub06 cat16:
      *     - rifampicin
      *   o fun09 sec05 sub06 cat17:
      *     - streptomycin
      *   o fun09 sec05 sub06 cat18:
      *     - other
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat01:
      +   - amikacin
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-amk", tmpStr)
      ){ /*Else If: have amikacin resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "amikacin",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      } /*Else If: have amikacin resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat02:
      +   - bedaquiline
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-bdq", tmpStr)
      ){ /*Else If: have bedaquiline resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "bedaquiline",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have bedaquiline resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat03:
      +   - capreomycin
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-cap", tmpStr)
      ){ /*Else If: have capreomycin resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "capreomycin",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have capreomycin resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat04:
      +   - clofazimine
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-cfz", tmpStr)
      ){ /*Else If: have clofazimine resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "clofazimine",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have clofazimine resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat05:
      +   - delamanid
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-dlm", tmpStr)
      ){ /*Else If: have delamind resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "delamanid",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have delamind resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat06:
      +   - ethambutol
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-emb", tmpStr)
      ){ /*Else If: have ethambutol resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "ethambutol",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have ethambutol resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat07:
      +   - ethionamide
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-eto", tmpStr)
      ){ /*Else If: have ethionamide resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "ethionamide",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have ethionamide resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat08:
      +   - fluoroquine
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-flq", tmpStr)
      ){ /*Else If: have fluoroquine resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "fluoroquine",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have fluoroquine resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat09:
      +   - isoniazid
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-inh", tmpStr)
      ){ /*Else If: have isoniazid resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "isoniazid",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have isoniazid resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat10:
      +   - kanamycin
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-kan", tmpStr)
      ){ /*Else If: have kanamycin resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "kanamycin",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have kanamycin resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat11:
      +   - levofloxacin
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-lfx", tmpStr)
      ){ /*Else If: have levofloxacin resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "levofloxacin",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have levofloxacin resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat12:
      +   - linezolid
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-lzd", tmpStr)
      ){ /*Else If: have linezolid resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "linezolid",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have linezolid resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat13:
      +   - moxifloxacin
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-mfx", tmpStr)
      ){ /*Else If: have moxifloxacin resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "moxifloxacin",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have moxifloxacin resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat14:
      +   - protonimid
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-pmd", tmpStr)
      ){ /*Else If: have protonimid resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "protonimid",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have protonimid resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat15:
      +   - pyrazinamide
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-pza", tmpStr)
      ){ /*Else If: have pyrazinamide resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "pyrazinamide",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have pyrazinamide resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat16:
      +   - rifampicin
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-rif", tmpStr)
      ){ /*Else If: have rifampicin resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "rifampicin",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have rifampicin resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat17:
      +   - streptomycin
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-stm", tmpStr)
      ){ /*Else If: have streptomycin resistance*/
         tmpStr += 4;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               (signed char *) "streptomycin",
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have streptomycin resistance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec05 Sub06 Cat18:
      +   - other drugs
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlWhite_ulCp((signed char *) "-other", tmpStr)
      ){ /*Else If: have streptomycin resistance*/
         tmpStr += 6;
         while(*tmpStr && *tmpStr < 33)
            ++tmpStr;
         if(! *tmpStr)
            goto varErr_fun09_sec06;

         /*convert drug name into a c-string*/
         oldPosStr = tmpStr;
         oldPosStr += endWhite_ulCp(oldPosStr);
         oldCharSC = *oldPosStr;
         *oldPosStr = 0;

         *errSCPtr =
            addDrugs_addAmr(
               amrHeapST,
               tmpStr,
               0, /*not using cross resistance*/
               drugAryStr,
               drugLenSIPtr,
               drugSizeSIPtr
            );

         *oldPosStr = oldCharSC;
         tmpStr = oldPosStr;
         oldPosStr = 0;

         if(! *errSCPtr)
            ;
         else if(*errSCPtr == def_memErr_addAmr)
            goto memErr_fun09_sec06;
         else if(*errSCPtr == def_overflow_addAmr)
            goto overflow_fun09_sec06;
      }  /*Else If: have streptomycin resistance*/

      /**************************************************\
      * Fun09 Sec05 Sub07:
      *   - deal with white space or invalid entries
      \**************************************************/

      else if(*tmpStr < 33)
      { /*Else If: white space*/
         while(*tmpStr && *tmpStr < 33)
            ++tmpStr;
      } /*Else If: white space*/

      else
         goto varErr_fun09_sec06;
         /*invalid entry*/
   } /*Loop: get and add remaing flags*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec06:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun09_sec06;

   memErr_fun09_sec06:;
      *errSCPtr = def_memErr_addAmr;
      goto errClean_fun09_sec06;

   varErr_fun09_sec06:;
      *errSCPtr = def_varErr_addAmr;
      goto errClean_fun09_sec06;

   overflow_fun09_sec06:;
      *errSCPtr = def_overflow_addAmr;
      goto errClean_fun09_sec06;

   noVar_fun09_sec06:;
      *errSCPtr = def_noVar_addAmr;
      goto errClean_fun09_sec06;

   noRef_fun09_sec06:;
      *errSCPtr = def_noRef_addAmr;
      goto errClean_fun09_sec06;

   errClean_fun09_sec06:;
      *errPosSLPtr = tmpStr - lineStr;

      if(amrHeapST)
         freeHeap_amrST(amrHeapST);
      amrHeapST = 0;

      if(oldPosStr)
         *oldPosStr = oldCharSC;
      goto ret_fun09_sec06;

   ret_fun09_sec06:;
      return amrHeapST;
} /*lineToVar_addAmr*/

/*-------------------------------------------------------\
| Fun10: getVar_addAmr
|   - get a variant id from a file
| Input:
|   - varFILE:
|     o FILE pointer with variant line to get
|   - refIndexSIPtr:
|     o signed int pointer to get the index of the
|       reference used
|   - coordsSTPtr:
|     o geneCoord struct pointer with genes to map to
|   - refSeqArySTPtr:
|     o seqST struct array with the reference sequences
|       sorted by reference name (use `sort_seqST()`)
|   - refSeqLenSL:
|     o number of reference sequences
|   - drugAryStr:
|     o pointer to c-string (techincally array) with drugs
|       to add to
|   - drugLenSIPtr:
|     o signed int pointer with number drugs in drugAryStr
|   - drugSizeSIPtr:
|     o signed int pointer with maximum number of drugs
|       that can be in drugAryStr before resizing
|   - errSCPtr:
|     o signed char to get errors
|   - errPosSLPtr:
|     o signed long pointer to get positon of error in
|       line
| Output:
|   - Modifies:
|     o varFILE to be on next line after variant
|     o refIndexSIPtr to have -1 or the index of the
|       reference used
|     o drugAryStr to have new drugs
|       * realloced or malloced if need to get more memory
|     o drugLenSIPtr to have new drugAryStr length if a
|       new drug was added
|     o drugSizeSIPtr to have new drugAryStr size if
|       drugAryStr is resized
|     o errSCPtr:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_fileErr_addAmr for file errors
|       * def_noVar_addAmr if blank line or variant id is
|         empty
|       * def_varErr_addAmr if something is wrong with the
|         varaint entry
|       * def_overflow_addAmr if went past the maximum
|         drug limit (for an amrST struct)
|       * def_eof_addAmr if end of file (no variant)
|     o errPosSLPtr to get position of error in line
|   - Returns:
|     o 0 for errors
|     o amrST struct with new AMR
\-------------------------------------------------------*/
struct amrST *
getVar_addAmr(
   void *varFILE,            /*file with variant ids*/
   signed int *refIndexSIPtr,    /*gets reference used*/
   struct geneCoord *coordsSTPtr,/*has gene coordaintes*/
   struct seqST *refSeqArySTPtr, /*reference sequences*/
   signed long refSeqLenSL,  /*number of references*/
   signed char **drugAryStr, /*has drugs in database*/
   signed int *drugLenSIPtr,/*number drugs in drugAryStr*/
   signed int *drugSizeSIPtr, /*max drugs in drugAryStr*/
   signed char *errSCPtr,    /*gets errors*/
   signed long *errPosSLPtr  /*positon in line error at*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10 TOC:
   '   - get a variant id from a file
   '   o fun10 sec01:
   '     - variable declarations
   '   o fun10 sec02:
   '     - get line with variant id
   '   o fun10 sec03:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define def_maxLine_fun10 4096 - 8
   signed char lineStr[def_maxLine_fun10 + 8];
   signed char *tmpStr = 0;
   signed long lenSL = 0;
   struct amrST *amrHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
   ^   - get line with variant id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lenSL =
      getLine_fileFun(
         (FILE *) varFILE,
         lineStr,
         def_maxLine_fun10,
         &lenSL
      ); /*get line with variant id*/

   if(! lenSL)
      goto eof_fun10_sec03;


   /*check if line is empty*/
   tmpStr = lineStr;

   while(*tmpStr < 33)
   { /*Loop: find first item in line*/
      if(*tmpStr == '\0')
         goto noVar_fun10_sec03;
      ++tmpStr;
   } /*Loop: find first item in line*/

   amrHeapST =
      lineToVar_addAmr(
         lineStr,
         refIndexSIPtr,
         coordsSTPtr,   /*has gene coordaintes*/
         refSeqArySTPtr,/*reference sequences*/
         refSeqLenSL,   /*number of references*/
         drugAryStr,    /*has drugs in database*/
         drugLenSIPtr,  /*number drugs in drugAryStr*/
         drugSizeSIPtr, /*max drugs in drugAryStr*/
         errSCPtr,      /*gets errors*/
         errPosSLPtr    /*gets position of error*/
      ); /*convert line to tbAmr database entry*/
   
   if(! *errSCPtr)
      ; /*no errors*/
   else if(*errSCPtr == def_memErr_addAmr)
      goto memErr_fun10_sec03;
   else if(*errSCPtr == def_varErr_addAmr)
      goto varErr_fun10_sec03;
   else if(*errSCPtr == def_overflow_addAmr)
      goto overlfow_fun10_sec03;
   else if(*errSCPtr == def_noRef_addAmr)
      goto noRef_fun10_sec03;
   else
      goto noVar_fun10_sec03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec03:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun10_sec03;

   memErr_fun10_sec03:;
      *errSCPtr = def_memErr_addAmr;
      goto errClean_fun10_sec03;

   varErr_fun10_sec03:;
      *errSCPtr = def_varErr_addAmr;
      goto errClean_fun10_sec03;

   overlfow_fun10_sec03:;
      *errSCPtr = def_overflow_addAmr;
      goto errClean_fun10_sec03;

   noVar_fun10_sec03:;
      *errSCPtr = def_noVar_addAmr;
      goto errClean_fun10_sec03;

   noRef_fun10_sec03:;
      *errSCPtr = def_noRef_addAmr;
      goto errClean_fun10_sec03;

   eof_fun10_sec03:;
      *errSCPtr = def_eof_addAmr;
      goto errClean_fun10_sec03;

   errClean_fun10_sec03:;
      if(amrHeapST)
         freeHeap_amrST(amrHeapST);
      amrHeapST = 0;
      goto ret_fun10_sec03;

   ret_fun10_sec03:;
      return amrHeapST;
} /*getVar_addAmr*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not possible, this code is under the
:   MIT license
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
: Copyright (c) 2025 jeremyButtler
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
