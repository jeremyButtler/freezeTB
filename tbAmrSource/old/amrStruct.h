
/*
'  o fun-01: amrIsRes
'    - Dectects if an amr was classified as resistant or
'      is unknow/not resitance
'  o fun-02: blankAmrStruct
'    - Sets all non-pointer values in amrStructPtr to 0
'  o fun-03: initAmrStruct
'    - Sets all values, including pointers in amrStructPtr
'      to 0
'  o fun-04: freeAmrStructStack
'    - Frees the geneIdStr, refSeqStr, and amrSeqStr arrays
'      in amrStructPtr and sets all values to 0
'  o fun-05: freeAmrStruct
'    - Frees an heap allocated amrStruct structure
'  o fun-06: freeAmrStructArray
'    - Frees an heap allocated array of amrStruct
'      structures
'  o fun-07: sortAmrStructArray (TODO: MAKE)
'    - Sort on an amrStruct array structures by reference
'      coordiante (uses shell sort)
'  o fun-08: readWhoAmrCsv (TODO: UPDATE)
'    - Gets the amr data from the Who TB antibiotice
'      resistance catalog (genome indicie tab saved as
'      a csv).
'  o fun-09: findNearestAmr
'     - Finds the nearest amr at or after the input query
'       coordiante
*/

#ifndef AMR_STRUCT_H
#define AMR_STRUCT_H

#include <stdio.h>
#include "../generalLib/genMath.h"
#include "../generalLib/codonTbl.h"

typedef struct amrStruct{
   uint refPosUI;   /*Starting position on reference*/
   uint codonPosUI; /*Codon starting position on ref*/

   char amrAaC;     /*What the mutant amino acid is*/
   char *geneIdStr; /*Gene name*/

   char *refSeqStr; /*Reference sequence at refPosUI*/
   uint lenRefSeqUI;/*Length of reference sequence*/

   char *amrSeqStr; /*AMR sequence at refPosUI*/
   uint lenAmrSeqUI;/*Length of amr sequence*/

   char revBl;     /*-1: gene reverse complement, else 0*/
      /*This is marked in the gene_locus column as a c
      ' at the end
      */
   ulong amrFlagsUL; /*flags for antibiotic classes*/
}amrStruct;

/*-------------------------------------------------------\
| Fun-01: amrIsRes
|   - Dectects if an amr was classified as resistant or
|     is unknow/not resitance
| Input:
|   - amrC:
|     - Character with grade assigned by the WHO
| Output:
|   - Returns:
|     o 1 if was a grade 1/2 (resistance)
|     o 0 if was not a grade 1/2
\-------------------------------------------------------*/
#define amrIsRes(amrC)(\
   ((lineStr[uiIter] > 48) & (lineStr[uiIter] < 51))\
)
  /*Logic:
  `  - lineStr[uiIter] < 51
  `    o 1 if is a number < 3 or is a special symbol, or
  `      an invisible character
  '    o limits to special symbols and 0, 1, 2
  '      This should remove NA's, and grade 3, 4, and 5
  '  - lineStr[uiIter] > 48
  `    o 1 if is a number > 0 or a character + extra
  `    o 0 if not 1
  `  - (lineStr[uiIter] >) 48 & (lineStr[uiIter] < 51)
  `    o 1 if number is 1 or 2
  `    o 0 if character is not 1 or 2
  */     


/*-------------------------------------------------------\
| Fun-02: blankAmrStruct
|   - Sets all non-pointer values in amrStructPtr to 0
| Input:
|   - ampStructPtr:
|     - Pointer to amrStruct to blank
| Output:
|   - Modifies
|     o All non-pointer values in amrStructPtr to be 0
\-------------------------------------------------------*/
#define blankAmrStruct(amrStructPtr){\
   (amrStructPtr)->refPosUI = 0;\
   (amrStructPtr)->codonPosUI = 0;\
   (amrStructPtr)->amrAaC = 0;\
   (amrStructPtr)->revBl = 0;\
   (amrStructPtr)->amrFlagsUL = 0;\
   (amrStructPtr)->lenRefSeqUI = 0;\
   (amrStructPtr)->lenAmrSeqUI = 0;\
} /*blankAmrStruct*/

/*-------------------------------------------------------\
| Fun-03: initAmrStruct
|   - Sets all values, including pointers in amrStructPtr
|     to 0
| Input:
|   - ampStructPtr:
|     - Pointer to amrStruct to initialize
| Output:
|   - Modifies
|     o All values in amrStructPtr to be 0
\-------------------------------------------------------*/
#define initAmrStruct(amrStructPtr){\
   blankAmrStruct((amrStructPtr));\
   (amrStructPtr)->geneIdStr = 0;\
   (amrStructPtr)->refSeqStr = 0;\
   (amrStructPtr)->amrSeqStr = 0;\
} /*initAmrStruct*/

/*-------------------------------------------------------\
| Fun-04: freeAmrStructStack
|   - Frees the geneIdStr, refSeqStr, and amrSeqStr arrays
|     in amrStructPtr and sets all values to 0
| Input:
|   - ampStructPtr:
|     - Pointer to amrStruct to free memory from
| Output:
|   - Frees:
|     o refSeqStr, amrSeqStr, and geneIdStr (one buffer)
|   - Modifies:
|     o All values in amrStructPtr to be 0
\-------------------------------------------------------*/
#define freeAmrStructStack(amrStructPtr){\
   if((amrStructPtr)->geneIdStr != 0)\
     free((amrStructPtr)->geneIdStr);\
   \
   initAmrStruct((amrStructPtr));\
} /*freeAmrStructStack*/

/*-------------------------------------------------------\
| Fun-05: freeAmrStruct
|   - Frees an heap allocated amrStruct structure
| Input:
|   - ampStructPtr:
|     - Pointer to amrStruct to free
| Output:
|   - Frees:
|     o amrStructPtr and the variables inside amrStructPtr
\-------------------------------------------------------*/
#define freeAmrStruct(amrStructPtr){\
   freeAmrStructStack((amrStructPtr));\
   free(amrStructPtr);\
   amrStructPtr = 0;\
} /*freeAmrStruct*/

/*-------------------------------------------------------\
| Fun-06: freeAmrStructArray
|   - Frees an heap allocated array of amrStruct
|     structures
| Input:
|   - ampStructPtr:
|     - Pointer to start of amrStruct array to free
| Output:
|   - Frees:
|     o The amrStructPtr array and the variables inside
|       each amrStructPtr
\-------------------------------------------------------*/
#define freeAmrStructArray(amrStructAryPtr, numElm){\
   ulong ulElmMac = 0;\
   for(ulElmMac = 0; ulElmMac < numElm; ++ulElmMac)\
      freeAmrStructStack(&(amrStructAryPtr)[ulElmMac]);\
   \
   free(amrStructAryPtr);\
   amrStructAryPtr = 0;\
} /*freeAmrStructArray*/

/*-------------------------------------------------------\
| Fun-07: sortAmrStructArray
|   - Sort on an amrStruct array structures by reference
|     coordiante (uses shell sort)
| Input:
|   - ampStructPtr:
|     - Pointer to start of amrStruct array to sort
| Output:
|   - Modifies:
|     o The amrStructPtr array to be sorted by starting
|       reference coordiante
\-------------------------------------------------------*/

/*-------------------------------------------------------\
| Fun-08: readWhoAmrCsv
|   - Gets the amr data from the Who TB antibiotice
|     resistance catalog (genome indicie tab saved as
|     a csv).
| Input:
|   - whoFILE:
|     o Csv file for the WHO's genome indience tab of
|       the TB antibiotic catalog spread sheet
|   - numArmUL:
|     o Modified to hold the number of catalog entriex
| Output:
|   - Modifies:
|     o numArmUL to hold the number of extracted amr
|       mutations.
|   - Returns:
|     o An array of amrStruct's (free with
|       freeAmrStructArray)
\-------------------------------------------------------*/
static struct amrStruct * readWhoAmrCsv(
   FILE *whoFILE,
   ulong *numAmrUL
){
   ushort lenBuffUS = 2048;
   char buffStr[lenBuffUS]; /*buffer*/
   char *lineStr = buffStr; /*Holds a single line*/
   char *tmpStr = 0;

   uint  maxLineLenUI = 0;     /*max line length*/
   uint  lenLineUI = 0;        /*Length of one line*/
   ulong numLinesUL = 0;
   ulong charReadInUL = 0;

   uint uiIter = 0;        /*Iterator for loops*/
   uint uiColIter = 0;     /*For looping through columns*/
   uint uiAmr = 0;

   uint lenSeqUI = 0;
   uint lenIdUI = 0;

   uint idIndexUI = 0;

   struct amrStruct *amrST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-02:
   ^  - Find the number of lines in the who file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Read in the first section of the file*/
   charReadInUL =
      fread(lineStr, sizeof(char), lenBuffUS, whoFILE);

   while(charReadInUL)
   { /*Loop: Get the number of lines in the file*/
      lenLineUI = 0;

      for(uiIter = 0; uiIter < charReadInUL; ++uiIter)
      { /*Loop: Count number of new lines & characters*/
         ++lenLineUI;         
         numLinesUL += lineStr[uiIter] == '\n';
      } /*Loop: Count number of new lines & characters*/

      maxLineLenUI = noBranchMax(maxLineLenUI, lenLineUI);

      charReadInUL =
         fread(lineStr,sizeof(char),lenBuffUS,whoFILE);
   } /*Loop: Get the number of lines in the file*/

   if(numLinesUL < 2) return 0;

   --numLinesUL; /*Account for the header*/
   fseek(whoFILE, 0, SEEK_SET);
   maxLineLenUI += 2;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-03:
   ^  - Extract the AMR mutations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lineStr = malloc(maxLineLenUI * sizeof(char));

   if(lineStr == 0) return 0;

   amrST = calloc(numLinesUL, sizeof(struct amrStruct));
     /*There is some waste in memory usage, but this
     ` allows me to work with an array
     */

   if(amrST == 0)
   { /*If: I had a memroy error*/
      free(lineStr);
      lineStr = 0;
      return 0;
   } /*If: I had a memroy error*/

   *numAmrUL = 0;

   /*tmpStr is to avoid complents from ubuntus compiler*/
   tmpStr = fgets(lineStr, maxLineLenUI, whoFILE);

   while(fgets(lineStr, maxLineLenUI, whoFILE))
   { /*Loop: Read in all lines in the WHO's amr file*/

     /***************************************************\
     * Fun-08 Sec-03 Sub-02:
     *   - Find the length of the gene name
     \***************************************************/

     idIndexUI = 0;

     for(uiIter = 0; lineStr[uiIter] != ','; ++uiIter){}

     lenIdUI = uiIter - idIndexUI + 1; /*+1 for '\0'*/

     ++uiIter;

     /***************************************************\
     * Fun-08 Sec-03 Sub-03:
     *   - Check if it is reverse complement
     \***************************************************/

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter){}
    
     amrST[*numAmrUL].revBl = -(lineStr[uiIter -1] =='c');
     ++uiIter;
     
     /***************************************************\
     * Fun-08 Sec-03 Sub-04:
     *   - Get the reference coordinate of the mutation
     \***************************************************/

     for(uiColIter = 0; uiColIter < 2; ++uiColIter)
     { /*Loop: Move to the genome_index column*/
        for(uiIter=uiIter;lineStr[uiIter]!=',';++uiIter){}
        ++uiIter;
     } /*Loop: Move to the genome_index column*/

     tmpStr = &lineStr[uiIter];

     /*Get around multi entries (they used commas*/
     for(tmpStr = tmpStr; *tmpStr < 48; ++tmpStr){}

     tmpStr =
        base10StrToUI(tmpStr, amrST[*numAmrUL].refPosUI);

     /*The next entry is the sequence, so no numbers.
     ` Numbers denote a multi entry
     */
     if(lineStr[uiIter] == '"')
     { /*If: this had multiple positions*/
        for(
           uiIter = 1 + tmpStr - lineStr;
           lineStr[uiIter] != '"';
           ++uiIter
        ){}

        uiIter += 2;
     } /*If: this had multiple positions*/

     else
     { /*Else: This entry only had a single position*/
        for(tmpStr = tmpStr; *tmpStr != ','; ++tmpStr){};
        uiIter = 1 + tmpStr - lineStr;
     } /*Else: This entry only had a single position*/

     /***************************************************\
     * Fun-08 Sec-03 Sub-04:
     *   - Get the length of the reference and mutant
     *     sequence
     \***************************************************/

     /*Get the length of the reference and mutant
     ` sequence; My goal is to store both sequences in the
     ` same buffer
     */
     for(
        lenSeqUI = uiIter;
        lineStr[lenSeqUI] != ',';
        ++lenSeqUI
     ){} /*Loop: get length of reference sequence*/

     ++lenSeqUI; /*Account for the null/,*/

     for(
        lenSeqUI = lenSeqUI;
        lineStr[lenSeqUI] != ',';
        ++lenSeqUI
     ){} /*Loop: get length of the mutant sequence*/

     /*Make the buffer to hold the sequences*/
     ++lenSeqUI; /*Account for the null to terminate*/
     lenSeqUI -= uiIter; /*Account for offset*/

     /***************************************************\
     * Fun-08 Sec-03 Sub-05:
     *   - Allocate memory for the gene name and sequences
     \***************************************************/

     amrST[*numAmrUL].geneIdStr =
        malloc((lenIdUI + lenSeqUI) * sizeof(char));

     if(!amrST[*numAmrUL].geneIdStr)
     { /*If: I had a memroy error*/
        freeAmrStructArray(amrST, *numAmrUL);
        free(lineStr);
        return 0;
     } /*If: I had a memroy error*/

     /***************************************************\
     * Fun-08 Sec-03 Sub-06:
     *   - Copy the gene name
     \***************************************************/

     lenSeqUI = 0;

     for(
       idIndexUI = idIndexUI;
       lineStr[idIndexUI] != ',';
       ++idIndexUI
     ){ /*Loop: Copy the gene name*/
        amrST[*numAmrUL].geneIdStr[lenSeqUI] = 
           lineStr[idIndexUI];

        ++lenSeqUI;
     } /*Loop: Copy the gene name*/

     amrST[*numAmrUL].geneIdStr[lenSeqUI] = '\0';

     /***************************************************\
     * Fun-08 Sec-03 Sub-07:
     *   - Copy the reference sequence
     \***************************************************/

     amrST[*numAmrUL].refSeqStr =
        &(amrST[*numAmrUL].geneIdStr[lenSeqUI + 1]);
     
     lenSeqUI = 0;

     /*Copy the reference sequence*/
     for(uiIter=uiIter; lineStr[uiIter] != ','; ++uiIter)
     { /*Loop: Copy the reference sequence*/
        amrST[*numAmrUL].refSeqStr[lenSeqUI] =
            lineStr[uiIter];

        ++lenSeqUI;
     }  /*Loop: Copy the reference sequence*/
     
     amrST[*numAmrUL].refSeqStr[lenSeqUI] = '\0';
     ++uiIter; /*Get off the ','*/

     /***************************************************\
     * Fun-08 Sec-03 Sub-08:
     *   - Copy the mutant sequence
     \***************************************************/

     amrST[*numAmrUL].amrSeqStr =
        &(amrST[*numAmrUL].refSeqStr[lenSeqUI + 1]);

     lenSeqUI = 0;

     /*Copy the mutant (amr) sequence*/
     for(uiIter=uiIter; lineStr[uiIter] != ','; ++uiIter)
     { /*Loop: Copy the muterence sequence*/
        amrST[*numAmrUL].amrSeqStr[lenSeqUI] =
            lineStr[uiIter];

        ++lenSeqUI;
     }  /*Loop: Copy the muterence sequence*/

     amrST[*numAmrUL].amrSeqStr[lenSeqUI] = '\0';
     ++uiIter; /*Get off the ','*/

     /***************************************************\
     * Fun-08 Sec-03 Sub-09:
     *   - See if the mutation caused an aa change
     \***************************************************/

     /*Move to the mutant amino acid*/
     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter){}
     ++uiIter; /*Get off the ','*/

     /*I need to find the first base in the codon*/
     if(   lineStr[uiIter] != 'N'
        && lineStr[uiIter + 1] != 'A'
     ){ /*If: I have an amino acid change*/
        amrST[*numAmrUL].amrAaC =
          aaThreeLetterToChar(&lineStr[uiIter]);

        /*I am assuming nothing is reverse complement*/
        amrST[*numAmrUL].codonPosUI =
           amrST[*numAmrUL].refPosUI;

        if(amrST[*numAmrUL].refSeqStr[0]
           != amrST[*numAmrUL].amrSeqStr[0]
        ) ; /*Avoids label found at end of empty statement
            ` error on unbuntu
            */

        else if
              (amrST[*numAmrUL].refSeqStr[1]
           != amrST[*numAmrUL].amrSeqStr[1]
        ) amrST[*numAmrUL].codonPosUI -=
             (1 | amrST[*numAmrUL].revBl);

        else
           amrST[*numAmrUL].codonPosUI -=
             (2 | (amrST[*numAmrUL].revBl & -2));
     } /*If: I have an amino acid change*/

     /***************************************************\
     * Fun-08 Sec-03 Sub-10:
     *   - Get to the first antibiotic column
     \***************************************************/

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter){}
     ++uiIter;

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter){}
     ++uiIter;

     /***************************************************\
     * Fun-08 Sec-03 Sub-11:
     *   - Get the remaining antibiotics
     \***************************************************/

     for(uiAmr = 0; uiAmr < 15; ++uiAmr)
     { /*Loop: Get the rating of all AMRs*/
        /*Move to the next AMR confidence column*/
        for(uiColIter = 0; uiColIter < 3; ++uiColIter)
        { /*Loop: Move to the genome_index column*/
           for(
              uiIter = uiIter;
              lineStr[uiIter] != ',';
              ++uiIter
            ){}

           ++uiIter;
        } /*Loop: Move to the genome_index column*/

        amrST[*numAmrUL].amrFlagsUL |=
           (amrIsRes(lineStr[uiIter]) << uiAmr);
     } /*Loop: Get the rating of all AMRs*/


     if(amrST[*numAmrUL].amrFlagsUL == 0)
     { /*If: this mutant has no antibiotic resitance*/
        freeAmrStructStack(&amrST[*numAmrUL]);
        continue;
     } /*If: this mutant has no antibiotic resitance*/

     /***************************************************\
     * Fun-08 Sec-03 Sub-12:
     *   - Get the final decided position
     \***************************************************/

     /*Get of the last AMR entry*/
     for(uiIter=uiIter;lineStr[uiIter]!=',';++uiIter){}
     ++uiIter; /*Get of the final coma*/

     /*Get off the reference entry*/
     for(uiIter=uiIter;lineStr[uiIter]!=',';++uiIter){}
     ++uiIter;

     tmpStr =
        base10StrToUI(
           &lineStr[uiIter],
           amrST[*numAmrUL].refPosUI
        );

     for(tmpStr = tmpStr; *tmpStr != ','; ++tmpStr){};
     uiIter = 1 + tmpStr - lineStr;

     /***************************************************\
     * Fun-08 Sec-03 Sub-13:
     *   - Get the final decided reference sequence
     \***************************************************/

     lenSeqUI = 0;

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter)
     { /*Loop: Copy the final reference decision*/
        amrST[*numAmrUL].refSeqStr[lenSeqUI] =
            lineStr[uiIter];

        ++lenSeqUI;
     } /*Loop: Copy the final reference decision*/
      
     amrST[*numAmrUL].refSeqStr[lenSeqUI] = '\0';
     amrST[*numAmrUL].lenRefSeqUI = lenSeqUI;
     ++uiIter; /*Get off the ','*/

     /***************************************************\
     * Fun-08 Sec-03 Sub-14:
     *   - Get the final decided mutatn sequence
     \***************************************************/

     lenSeqUI = 0;

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter)
     { /*Loop: Copy the final mutant decision*/
        amrST[*numAmrUL].amrSeqStr[lenSeqUI] =
            lineStr[uiIter];

        ++lenSeqUI;
     } /*Loop: Copy the final mutant decision*/

     amrST[*numAmrUL].amrSeqStr[lenSeqUI] = '\0';
     amrST[*numAmrUL].lenAmrSeqUI = lenSeqUI;

     ++(*numAmrUL);
     initAmrStruct(&(amrST[*numAmrUL]));
   } /*Loop: Read in all lines in the WHO's amr file*/

   free(lineStr);
   return amrST;
} /*readWhoGenomeIndicieCsv*/


/*-------------------------------------------------------\
| Fun-09: findNearestAmr
|  - Does a binary search for the nearest amr at or after
|    to the input query coordiante
| Input:
|  - amrAryST:
|    o Pointer to an array of amrStruct structures to
|      search
|  - qryUI:
|    o Starting coordinate (query) to search for in
|      amrAryST
|  - numGenesUI:
|    o Number of amrStruct's in amrAryST
| Output:
|  - Returns:
|    o The nearest index at or after qryUI
|    o -1 for no amr genes after qryUI
\-------------------------------------------------------*/
static int findNearestAmr(
   struct amrStruct *amrAryST,
   uint qryUI,
   int numAmrI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ' Fun-09 TOC: findNearestAmr
   '   - Finds the nearest amr at or after the input query
   '     coordiante
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   int midI = numAmrI >> 1; /*divide by 2*/
   int lastMidI = numAmrI;
   int shiftI = 2; /*so loop fires*/

   while(shiftI > 1 && midI >= 0)
   { /*Loop: Search for the starting coordinate*/
      shiftI = lastMidI - midI;
      shiftI = noBranchAB(shiftI);
      shiftI = (shiftI >> 1) + (shiftI & 1);

      lastMidI = midI;

      if(amrAryST[midI].refPosUI > qryUI)
         midI -= ((shiftI & -(midI > 1)) - (midI <= 1));
         /*This is a bit hacky, but works for now. I
         ` need to fix this issue later
         */

      else if(amrAryST[midI].refPosUI < qryUI)
         midI += shiftI;

     else
     { /*Else: I found the query*/
        if(midI == 0) return 0;

        while(midI > 0)
        { /*Loop: Make sure I am on the first amr*/
           if(amrAryST[midI - 1].refPosUI != qryUI) break;
           --midI;
        } /*Loop: Make sure I am on the first amr*/

        return midI;
     } /*Else: I found the query*/

     /*branchless whith an if return was slower here*/
   } /*Loop: Search for the starting coordinate*/

   /*Make sure it is alwasy greater than*/
   midI += (amrAryST[midI].refPosUI < qryUI);

   /*I may have the same coordinates repeated*/
   while(midI > 0)
   { /*Loop: Make sure I am on the first amr*/
      if(amrAryST[midI - 1].refPosUI != qryUI) break;
      --midI;
   } /*Loop: Make sure I am on the first amr*/

   /*Make sure not at end of vector*/
   if(midI == numAmrI) return -1;

   return midI;
} /*findNearestAmr*/
 
#endif
