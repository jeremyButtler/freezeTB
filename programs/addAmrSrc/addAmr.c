/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' addAmr SOF: Start Of File
'   o fun01: aaToSeq
'     - change amino acids to a valid nucleotide sequence
'   o fun02: addNtSeq_addAmr
'     - adds nucleotide sequence to AMR
'   o fun03: addCodonPos_addAmr
'     - adds amino acid sequences for deletions and
'       large duplications, reading frame orientation
'       (forward/reverse), and the first reference
'       base in the codon to an amrST that has been
'       processed with addNtSeq_addAmr
'   o fun04: var_addAmr
'     - adds a variant to an AMR struct, but does not
'       complete (call addCodonPos_whoToAmr)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "../genLib/ulCp.h"
#include "../genLib/base10str.h"
#include "../genBio/geneCoord.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genLib/genMath.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: aaToSeq
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
aaToSeq(
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
} /*aaToSeq*/

/*-------------------------------------------------------/
| Fun02: addNtSeq_addAmr
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
|   - this does not fill in the aa AMR sequence, because
|     it might require reference information
\-------------------------------------------------------*/
signed char
addNtSeq_addAmr(
   signed char *varStr,   /*variant to add*/
   signed char *refStr,   /*reference sequence*/
   unsigned int startUI,  /*first base in gene*/
   unsigned int endUI,    /*last base in gene*/
   signed char revBl,     /*1: for reverse gene*/
   struct armST *amrSTPtr /*add varaint to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - adds nucleotide sequence to AMR
   '   o fun02 sec01:
   '     - variable declarations
   '   o fun02 sec02:
   '     - move to variant id type
   '   o fun02 sec03:
   '     - nucleotide variants
   '   o fun02 sec04:
   '     - amino acid variants
   '   o fun02 sec05:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siVar = 0;
   unsigned int uiRef = 0;
   signed int varStartSI = 0;
   signed int varEndSI = 0;
   unsigned int lenUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - move to variant id type
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(varStr[siVar] && varStr[siVar] != '.')
      ++siVar;

   if(! varStr[siVar])
   { /*If: at end of string*/
      if(! siVar)
         goto noVar_fun03_sec05; /*empty string*/
      else if( (varStr[siVar - 1] & ~32) == 'F' )
         goto lof_fun03_sec05; /*no coordinates*/
      else if( (varStr[siVar - 1] & ~32) == 'N' )
         goto del_fun03_sec05; /*deletion varaint*/
      else
         goto varErr_fun03_sec05; /*no idea what is*/
   } /*If: at end of string*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - nucleotide variants
   ^   o fun02 sec03 sub01:
   ^     - get coordiantes of variant
   ^   o fun02 sec03 sub03:
   ^     - copy insertion
   ^   o fun02 sec03 sub02:
   ^     - copy deletion
   ^   o fun02 sec03 sub04:
   ^     - copy snp
   ^   o fun02 sec03 sub05:
   ^     - .c (coding?) varaints, loop back to nucleotide
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec03 Sub01:
   *   - get coordiantes of variant
   \*****************************************************/

   --siVar; /*move to variant type*/
   if( (varStr[siVar] & ~32) == 'N' )
   { /*If: nucleotide variant*/
      addNtVar_fun02_sec05:;

      siVar += 2; /*move to first base*/
      siVar +=
        strToSI_base10str(&varStr[siVar], &varStartSI);

      if(revBl)
         varStartSI -= endUI;
      else
         varStartSI += startUI;
      amrSTPtr->refPosUI = varStartUI;

      if(varStr[siVar] == '_')
      { /*If: have end coordinate*/
         ++siVar;
         siVar +=
            strToSI_base10Str(&varStr[siVar], &varEndSI);
      } /*If: have end coordinate*/

      if(! varStr[siVar])
         goto varErr_fun03_sec05; /*no idea what is*/

      /**************************************************\
      * Fun02 Sec03 Sub02:
      *   - copy deletion
      \**************************************************/

      else if( (varStr[siVar] & ~32) == 'D')
      { /*Else If: deletion*/
         amrSTPtr->mutTypeStr[0] = 'd';
         amrSTPtr->mutTypeStr[1] = 'e';
         amrSTPtr->mutTypeStr[2] = 'l';
         amrSTPtr->mutTypeStr[3] = 0;

         siVar += 3; /*get of 'del' entry*/
         lenUI = lenStr_ulCp(&varStr[siVar]);

         if(revBl)
            varStartUI -= lenUI; /*get to first base*/

         amrSTPtr->refSeqStr =
            malloc((lenUI + 8) * sizeof(signed char));
         if(! amrStPtr->refSeqStr)
            goto memErr_fun02_sec05;

         amrSTPtr->lenRefSeqUI = lenUI;
         siRef = 0;
         while(varStr[siVar])
           amrSTPtr->refSeqStr[siRef++] = varStr[siVar++];

         /*set up no amr sequence flag*/
         amrSTPtr->amrSeqStr =
            malloc((1 + 8) * sizeof(signed char));
         if(! amrStPtr->amrSeqStr)
            goto memErr_fun02_sec05;
         amrStPtr->amrSeqStr[0] = '0';
         amrStPtr->amrSeqStr[1] = 0;
         amrSTPtr->lenAmrSeqUI = 1;
      } /*Else If: deletion*/

      /**************************************************\
      * Fun02 Sec03 Sub03:
      *   - copy insertion
      \**************************************************/

      else if( (varStr[siVar] & ~32) == 'I')
      { /*Else If: insertion*/
         amrSTPtr->mutTypeStr[0] = 'i';
         amrSTPtr->mutTypeStr[1] = 'n';
         amrSTPtr->mutTypeStr[2] = 's';
         amrSTPtr->mutTypeStr[3] = 0;

         siVar += 3; /*get of 'ins' entry*/
         lenUI = lenStr_ulCp(&varStr[siVar]);

         if(revBl)
            varStartUI -= lenUI; /*get to first base*/

         amrSTPtr->amrSeqStr =
            malloc((lenUI + 9) * sizeof(signed char));
         if(! amrStPtr->amrSeqStr)
            goto memErr_fun02_sec05;

         amrSTPtr->refSeqStr =
            malloc((1 + 8) * sizeof(signed char));
         if(! amrStPtr->refSeqStr)
            goto memErr_fun02_sec05;

         siRef = 0;
         amrSTPtr->lenAmrSeqUI = lenUI;
         amrSTPtr->amrSeqStr[siRef++]= refStr[varStartUI];
         while(varStr[siVar])
           amrSTPtr->amrSeqStr[siRef++] = varStr[siVar++];

         /*add base before insertion*/
         amrSTPtr->refSeqStr =
            malloc((1 + 8) * sizeof(signed char));
         if(! amrStPtr->refSeqStr)
            goto memErr_fun02_sec05;
         amrStPtr->refSeqStr[0] = refStr[varStartUI];
         amrStPtr->refSeqStr[1] = 0;
         amrSTPtr->lenRefSeqUI = 1;
      } /*Else If: insertion*/

      /**************************************************\
      * Fun02 Sec03 Sub04:
      *   - copy snp
      \**************************************************/

      else if(varStr[siVar] == '>') ;
      { /*Else If: bases*/
         amrSTPtr->mutTypeStr[0] = 's';
         amrSTPtr->mutTypeStr[1] = 'n';
         amrSTPtr->mutTypeStr[2] = 'p';
         amrSTPtr->mutTypeStr[3] = 0;

         siVar += 1; /*get of '>' entry*/
         lenUI = lenStr_ulCp(&varStr[siVar]);

         if(revBl)
            varStartUI -= lenUI; /*get to first base*/

         amrSTPtr->refSeqStr =
            malloc((lenUI + 8) * sizeof(signed char));
         if(! amrStPtr->refSeqStr)
            goto memErr_fun02_sec05;

         amrSTPtr->amrSeqStr =
            malloc((lenUI + 8) * sizeof(signed char));
         if(! amrStPtr->amrSeqStr)
            goto memErr_fun02_sec05;

         amrSTPtr->lenRefSeqUI = lenUI;
         amrSTPtr->lenAmrSeqUI = lenUI;
         siRef = 0;
         while(varStr[siVar])
         { /*Loop: copy mutation*/
           amrSTPtr->refSeqStr[siRef] =
              refStr[varStartUI++];
           amrSTPtr->amrSeqStr[siRef++] = varStr[siVar++];
         } /*Loop: copy mutation*/
      } /*Else If: bases*/
   } /*If: nucleotide variant*/

   /*****************************************************\
   * Fun02 Sec03 Sub05:
   *   - .c (coding?) varaints, loop back into nucleotide
   \*****************************************************/

   else if( (varStr[siVar] & ~32) == 'C' )
      goto addNtVar_fun02_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - amino acid variants
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if( (varStr[siVar] & ~32) == 'P' )
   { /*Else If: amino acid variant*/
      if(parseVarId_whoToAmr(amrSTPtr, 1))
         goto memErr_fun02_sec05;
         /*gives me the aa sequence*/
      
      uiRef = 0;
      siVar = 0;

      amrSTPtr->refSeqStr =
         malloc(
            (amrSTPtr->lenRefAaUI * 3 + 8)
               * sizeof(signed char)
         );
      if(! amrSTPtr->refSeqStr)
         goto memErr_fun02_sec05;

      amrSTPtr->lenRefSeqUI =
         aaToSeq(
            amrSTPtr->refAaStr,
            amrSTPtr->reqSeqStr,
            0
         );
      /*need to complete amr sequence after I call
      `  addCodonPos_whoToAmr
      */
   } /*Else If: amino acid variant*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec05:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto done_fun03_sec05;

   memErr_fun03_sec05:;
      errSC = def_memErr_addAmr;
      goto done_fun03_sec05;

   varErr_fun03_sec05:;
      errSC = def_varErr_addAmr;
      goto done_fun03_sec05;

   noVar_fun03_sec05:;
      errSC = def_noVar_addAmr;
      goto done_fun03_sec05;

   lof_fun03_sec05:;
      errSC = def_lof_addAmr;
      goto done_fun03_sec05;

   del_fun03_sec05:;
      errSC = def_del_addAmr;
      goto done_fun03_sec05;

   done_fun03_sec05:;
      return errSC;
} /*addNtSeq_addAmr*/

/*-------------------------------------------------------\
| Fun03: addCodonPos_addAmr
|   - adds amino acid sequences for deletions and
|     large duplications, reading frame orientation
|     (forward/reverse), and the first reference
|     base in the codon to an amrST that has been
|     processed with addNtSeq_addAmr
| Input:
|   - amrST:
|     o pointer to an array of amrST structs to update
|   - geneSTPtr:
|     o geneCoord struc with coordinates
|     o must be sorted by start (this is done when using
|       getCoords_geneCoord)
|   - numGenesUI:
|     o number of genes in geneSTPtr
|   - refStr:
|     o c-string with reference seqeunce
| Output:
|   - Modifies:
|     o amrST to have the amino acid sequence and starting
|       position of codon on the reference
\-------------------------------------------------------*/
void
addCodonPos_addAmr(
   struct amrST *amrSTPtr,   /*amr variant to update*/
   struct geneCoord geneSTPtr,/*has gene coordinates*/
   unsigned int numGenesUI,  /*number genes in geneSTPtr*/
   signed char *refStr       /*reference seqeunce*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC: addCodonPos_addAmr
   '   - adds amino acid sequences for deletions and
   '     large duplications, reading frame orientation
   '     (forward/reverse), and the first reference
   '     base in the codon to an amrST that has been
   '     processed with addNtSeq_addAmr
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - add gene coordinates and ammino acid sequences 
   '   o fun03 sec03:
   '     - finish reverese complement AA AMR sequences
   '   o fun03 sec04:
   '     - finish forward AA AMR sequences
   '   o fun03 sec05:
   '     - add in large duplication events
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01: addCodonPos_whoToAmr
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *cpStr = 0;
   signed char *dupStr = 0;

   /*for dealing with aa deletions*/
   signed int siCodon = 0;
   unsigned char oneNtUC = 0;
   unsigned char twoNtUC = 0;
   unsigned char threeNtUC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02: addCodonPos_whoToAmr
   ^   - add in gene coordinates and ammino acid sequences 
   ^   o fun03 sec02 sub01:
   ^     - find gene the AMR is on
   ^   o fun03 sec02 sub02:
   ^     - check if AMR is on multiple genes/find primary
   ^   o fun03 sec02 sub04:
   ^     - add in gene starting positions
   ^   o fun03 sec02 sub05:
   ^     - processing for reverse complement AMR genes
   ^   o fun03 sec02 sub06:
   ^     - add in large duplication events
   ^   o fun03 sec02 sub07:
   ^     - clean up allocated memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - find gene the AMR is on
   \*****************************************************/

    posSI =
       findStart_geneCoord(
          geneSTPtr,
          amrSTPtr->refPosUI,
          numGenesUI
       );

    if(posSI < 0)
       break; /*gene not in list*/

    /****************************************************\
    * Fun03 Sec02 Sub02:
    *   - check if AMR is on multiple genes/find primary
    \****************************************************/

    /*check I the sequence is inside gene bounds*/
    tmpUI = amrSTPtr->refPosUI + amrSTPtr->lenRefSeqUI -1;
        /*-1 to convert index 1 to index 0*/

    if(tmpUI > (unsigned int) geneSTPtr->endAryUI[posSI])
       goto findTrueGene_fun03_sec02_sub03;
    else if
          amrSTPtr->refPosUI
       < (unsigned int) geneSTPtr->startAryUI[posSI]
    ){ /*Else If: am outside of selected gene bounds*/
       findTrueGene_fun03_sec02_sub03:;

       /*see if this AMR is really on the next gene
       `   by comparing it to the last/next AMR
       */
       tmpUI =
          eqlNull_charCp(
             amrSTPtr->geneIdStr,
             geneSTPtr->idStrAry[posSI]
          ); /*check if I have the same ids*/

       if(! tmpUI)
          ; /*correct gene; do nothing*/

       else if(posSI > 0)
       { /*Else If: wrong gene; check previous gene*/
          tmpUI =
             eqlNull_ulCp(
                amrSTPtr->geneIdStr,
                geneSTPtr->idStrAry[posSI - 1]
             ); /*check if last gene was correct*/

             if(! tmpUI)
                --posSI; /*found gene*/
             else if(posSI < numGenesSL - 1)
                goto checkNextGene_fun03_sec02_sub03;
             else if(siIndex > 0)
                goto checkLastAmr_fun03_sec02_sub03;

          /*else: no idea which gene it is*/
       } /*If: wrong gene; check previous gene*/

       else if(posSI < numGenesUI - 1)
       { /*Else If: wrong gene: check next gene*/
           checkNextGene_fun03_sec02_sub03:;

           tmpUI =
              eqlNull_ulCp(
                 amrSTPtr->geneIdStr,
                 geneSTPtr->idStrAry[posSI + 1]
              ); /*check if next gene was correct*/

          if(! tmpUI)
             ++posSI;
          else if(siIndex > 0)
             goto checkLastAmr_fun03_sec02_sub03;
          /*else: no idea which gene it is*/
       } /*Else If: wrong gene: check next gene*/

       /*else: no idea what gene belongs to*/
    } /*Else If: am outside of selected gene bounds*/

    /*************************************************\
    * Fun03 Sec02 Sub03:
    *   - add in gene starting positions
    \*************************************************/

    /*make sure the start is first mapped base*/
    if(
         geneSTPtr->startAryUI[posSI]
       < geneSTPtr->endAryUI[posSI]
    ){ /*If: start comes first*/
       amrSTPtr->geneFirstRefUI =
          geneSTPtr->startAryUI[posSI];

       amrSTPtr->geneLastRefUI =
          geneSTPtr->endAryUI[posSI];
    } /*If: start comes first*/

    else
    { /*Else: start comes last*/
       amrSTPtr->geneLastRefUI =
          geneSTPtr->startAryUI[posSI];

       amrSTPtr->geneFirstRefUI =
          geneSTPtr->endAryUI[posSI];
    } /*Else: start comes last*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec03:
    ^   - finish reverese complement AA AMR sequences
    ^   o fun03 sec03 sub01:
    ^     - add direction, check if aa seq, find position
    ^   o fun03 sec03 sub02:
    ^     - get reference amino acid sequence for
    ^       AMR(s) in reverse complement genes
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun03 Sec03 Sub01:
    *   - add direction, check if aa seq, find position
    \****************************************************/

    if(geneSTPtr->dirAryUC[posSI])
    { /*If: reverse complement gene*/
       amrSTPtr->dirFlagSC = def_revCompDir_amrST;

       if(amrSTPtr->codonNumUI == 0)
       { /*If: not amino acid mutation*/
          ++siIndex;
          continue;
       } /*If: not amino acid mutation*/


       /*Update the starting position*/
       /*this assumes the codon number is correct. i
       `   had one case (pncA_p.Ter187Argext*?) were I
       `   am not sure if this was true. Ref: 2288680,
       `   codon: 187. The output was 2288675, which
       `   is off
       */
       amrSTPtr->codonPosUI =
            geneSTPtr->endAryUI[posSI]
          - ( (amrSTPtr->codonNumUI - 1) * 3 );
       
       if(amrSTPtr->endCodonNumUI == 0)
       { /*If: single aa change*/
          amrSTPtr->endCodonNumUI = amrSTPtr->codonNumUI;
          ++siIndex;
          continue;
       } /*If: single aa change*/

       /*check to see if more than 1 or 2 aa long*/
       tmpUI =
           amrSTPtr->endCodonNumUI - amrSTPtr->codonNumUI;

       if(tmpUI < 2)
       { /*If: single amino acid (already set up)*/
          ++siIndex;
          continue;
       } /*If: single amino acid (already set up)*/

       /*************************************************\
       * Fun03 Sec03 Sub02:
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
    ^ Fun03 Sec04:
    ^   - finish forward AA AMR sequences
    ^   o fun03 sec04 sub01:
    ^     - add direction, check if aa seq, find position
    ^   o fun03 sec04 sub02:
    ^     - get reference amino acid sequence for
    ^       AMR(s) in reverse complement genes
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun03 Sec02 Sub01:
    *   - add direction, check if aa seq, find position
    \****************************************************/

    else
    { /*Else: foward gene*/
       amrSTPtr->dirFlagSC = def_forwardDir_amrST;

       if(amrSTPtr->codonNumUI == 0)
       { /*If: no amino acid seqence*/
          ++siIndex;
          continue;
       } /*If: no amino acid seqence*/


       /*update the starting position*/
       /*this assumes the codon number is correct. I
       `   had one case (pncA_p.Ter187Argext*?) were I
       `   am not sure if this was true. Ref: 2288680,
       `   codon: 187. The output was 2288675, which
       `   is off. this was an reverse case
       */
       amrSTPtr->codonPosUI =
            geneSTPtr->startAryUI[posSI]
          + ( (amrSTPtr->codonNumUI - 1) * 3 );

       if(amrSTPtr->endCodonNumUI == 0)
       { /*If: single aa change*/
          amrSTPtr->endCodonNumUI = amrSTPtr->codonNumUI;
          ++siIndex;
          continue;
       } /*If: single aa change*/

       /*Check to see if more than 1 or 2 aa long*/
       tmpUI =
           amrSTPtr->endCodonNumUI - amrSTPtr->codonNumUI;

       if(tmpUI < 2)
       { /*If: single amino acid (already setup)*/
          ++siIndex;
          continue;
       } /*If: single amino acid (already setup)*/

       /*************************************************\
       * Fun03 Sec04 Sub02:
       *   - get reference amino acid sequence for
       *     AMR(s) in reverse complement genes
       \*************************************************/

       /*set up for getting the reference amino
       `   acids; the +1 is to ignore the first amino
       `   acid in the deletion (I already have it)
       */
       dupStr = amrSTPtr->refAaStr + 1;

       /*+3 to get off the first codon*/
       cpStr = refSeqHeapStr + amrSTPtr->codonPosUI + 3;

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
    ^ Fun03 Sec05:
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
} /*addCodonPos_addAmr*/

/*-------------------------------------------------------\
| Fun04: var_addAmr
|   - adds a variant to an AMR struct, but does not
|     complete (call addCodonPos_whoToAmr)
| Input:
|   - varStr:
|     o variant id to process, should be in form of
|       * "gene_p.refAAnumberAMRAA" (ex gyrB_p.Ser447Phe)
|       * "gene_LoF"
|       * "gene_deletoin"
|   - refPosUI:
|     o position of variant on reference
|   - genesFileStr:
|     o c-string with path/name of file with gene
|       coordiantes
|   - refFileStr:
|     o c-string with reference to use in finishing
|       variant id
|   - errSCPtr:
|     o pointer to signed char to get returned error type
| Output:
|   - Modifies:
|     o errSCPtr to have error types:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_fileErr_addAmr for file errors
|       * def_noVar_addAmr if variant id is empty
|   - Returns:
|     o pointer to amrST struct with added variant
|     o 0 for memory errors
\-------------------------------------------------------*/
amrST *
var_addAmr(
   signed char *varStr,      /*variant to add*/
   signed char *geneStr,     /*name of gene to add*/
   struct geneCoord *geneSTPtr,/*has gene coordaintes*/
   unsigned int numGenesUI,  /*number genes in geneSTPtr*/
   signed char *refStr,      /*reference sequence*/
   signed char *errSCPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - adds a variant to an AMR struct, but does not
   '     complete (call addCodonPos_whoToAmr)
   '   o fun04 sec01:
   '     - variable declarations
   '   o fun04 sec02:
   '     - add variant id and gene name to struct
   '   o fun04 sec03:
   '     - find gene and add AMR entries
   '   o fun04 sec04:
   '     - finish filling lof and gene deletion entries
   '   o fun04 sec05:
   '     - return and clean up if needed
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siPos = 0;
   signed char errSC = 0;
   struct amrST *amrHeapST = 0;

   signed int geneLenSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - add variant id and gene name to struct
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   amrHeapST = malloc(sizeof(struct amrST));
   if(! amrHeapST)
      goto memErr_fun03_sec0x;
   init_amrST(amrHeapST);

   amrHeapST->lenGeneIdUI = endStr_ulCp(geneStr);
   amrHeapST->geneIdStr =
      malloc(
        (amrHeapST->lenGeneIdUI + 8) * sizeof(signed char)
      );
   if(! amrHeapST->geneIdStr)
      goto memErr_fun03_sec0x;
   cpLen_ulCp(
      amrHeapST->geneIdStr,
      geneStr,
      amrHeapST->lenGeneIdUI
   );

   amrHeapST->lenVarIdUI =
      cpStr_ulCp(amrSTPtr->varIdStr, varStr);
   if(! amrSTPtr->lenVarIdUI)
      goto noVar_fun03_sec0x;

   while(varStr[siPos] != '.')
      ++siPos;
   --siPos;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - find gene and add AMR entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(siPos < (signed int) numGenesUI)
   { /*Loop: find gene*/
      if(
        ! eqlNull_ulCp(geneStr,geneSTPtr->idAryStr[siPos])
      ) break; /*found gene*/
      ++siPos;
   } /*Loop: find gene*/

   if(siPos >= (signed int) numGenesUI)
      goto noGeneErr_fun03_sec0x;
      /*could not find gene in list*/
   
   *errSCPtr =
      addNtSeq_addAmr(
         varStr,
         refStr,
         geneSTPtr->startAryUI[siPos],
         geneSTPtr->endAryUI[siPos],
         geneSTPtr->dirAryUC[siPos],
         amrHeapST
      ); /*get nucleotide sequence (if can)*/
         
   if(! *errSCPtr)
      ; /*no errors*/
   else if(*errSCPtr == def_memErr_addAmr)
      goto memErr_fun03_sec0x;
   else if(*errSCPtr == def_del_addAmr)
      ; /*gene deletion, add in at end*/
   else if(*errSCPtr == def_lof_addAmr)
      ; /*gene loss of function, add in at end*/
   else
      goto addErr_fun03_sec0x;

   addCodonPos_addAmr(
         amrHeapST,
         geneSTPtr,
         numGenesUI,
         refStr
      ); /*finish the amino acid entries*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - finish filling lof and gene deletion entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*errSCPtr)
   { /*If: had lof or gene deletion*/
      amrHeapST->refPosUI = geneSTPtr->startAryUI[siPos];
      amrHeapST->dirFlagSC =
         (signed char) geneSTPtr->dirAryUC[siPos];

      amrHeapST->geneFirstRefUI =
         geneSTPtr->startAryUI[siPos];

      amrHeapST->geneLastRefUI =
         geneSTPtr->startAryUI[siPos];

      if(*errSCPtr = def_lof_addAmr)
      { /*If: had loss of function*/
         amrHeapST->frameshiftBl = 1;
         amrHeapST->mutTypeSC[0] = 'l';
         amrHeapST->mutTypeSC[1] = 'o';
         amrHeapST->mutTypeSC[2] = 'f';
         amrHeapST->mutTypeSC[3] = 0;
      } /*If: had loss of function*/

      else
      { /*Else: is gene deletion*/
         amrHeapST->mutTypeSC[0] = 'd';
         amrHeapST->mutTypeSC[1] = 'e';
         amrHeapST->mutTypeSC[2] = 'l';
         amrHeapST->mutTypeSC[3] = 0;
      } /*Else: is gene deletion*/

      amrHeapST->wholeGeneFlagSC = 1;

      amrHeapST->lenRefSeqUI =
           geneSTPtr->endAryUI[siPos]
         - geneSTPtr->startAryUI[siPos];
      amrHeapST->refSeqStr =
         malloc(
            (amrHeapSTPtr->lenRefSeqUI + 8)
               * sizeof(signed char)
         );
      if(! amrHeapST->refSeqStr)
         goto memErr_fun03_sec0x;
      cpLen_ulCp(
         amrHeapST->refSeqStr,
         &refStr[geneSTPtr->startAryUI[siPos],
         amrHeapST->lenRefSeqUI
      );

      /*copy the entire gene for the event*/
      amrHeapST->amrSeqStr =
         malloc(9 * sizeof(signed char));
      if(! amrHeapST->amrSeqStr)
         goto memErr_fun03_sec0x;
      amrHeapST->amrSeqStr[0] = '0';
      amrHeapST->amrSeqStr[1] = 0;
      amrHeapST->lenAmrAaUI = 0;
   } /*If: had lof or gene deletion*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec05:
   ^   - return and clean up if needed
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun03_sec0x;
 
   memErr_fun03_sec0x:;
      *errSC = def_memErr_addAmr;
      goto errClean_fun03_sec0x;

   fileErr_fun03_sec0x:;
      *errSC = def_fileErr_addAmr;
      goto errClean_fun03_sec0x;

   noVar_fun03_sec0x:;
      *errSC = def_noVar_addAmr;
      goto errClean_fun03_sec0x;

   noGeneErr_fun03_sec0x:;
      *errSC = def_noGene_addAmr;
      goto errClean_fun03_sec0x;

   /*general error for failed to complete variant*/
   addErr_fun03_sec0x:;
      *errSC = def_addErr_addAmr;
      goto errClean_fun03_sec0x;

   errClean_fun03_sec0x:;
      if(amrHeapST)
         freeHeap_amrST(amrHeapST);
      amrHeapST = 0;
      goto ret_fun03_sec0x;

   ret_fun03_sec0x:;
      return amrHeapST;
} /*var_addAmr*/
