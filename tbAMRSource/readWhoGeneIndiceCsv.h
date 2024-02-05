
#include <stdio.h>
#include "../generalLib/genMath.h"
#include "../generalLib/codonTbl.h"

typedef struct amrStruct{
   uint refPosUI;   /*Starting position on reference*/
   uint codonPosUI; /*Codon starting position on ref*/

   char geneIdStr[64]; /*Gene name*/
   char mutAaC;     /*What the mutant amino acid is*/
   char *refSeqStr; /*Reference sequence at refPosUI*/
   char *amrSeqStr; /*AMR sequence at refPosUI*/

   ulong armFlagsUL; /*flags for antibiotic classes*/
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

static struct * amrStruct readWhoAmrCsv(
   FILE *whoFILE,
   ulong *numAmrUL
){
   ushort lenBuffUS = 2048;
   char buffStr[LenBuffUS]; /*buffer*/
   char *lineStr = BuffStr; /*Holds a single line*/
   char *tmpStr = 0;

   uint  maxLineLenUI = 0;     /*max line length*/
   uint  lineLenUL = 0;        /*Length of one line*/
   ulong numLinesUL = 0;
   ulong charReadInUL = 0;

   uint uiIter = 0;        /*Iterator for loops*/
   uint uiColIter = 0;     /*For looping through columns*/
   uint uiAmr = 0;

   uint lenSeqUI = 0;
   uchar lenIdUC = 0;

   uint idIndexUI = 0;
   uint seqIndexUI = 0;

   struct amrStruct *amrST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-0? Sec-02:
   ^  - Find the number of lines in the who file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Read in the first section of the file*/
   CharReadInUL =
      fread(lineStr, sizeof(char), lenBuffUS, whoFILE);

   while(charReadInUL)
   { /*Loop: Get the number of lines in the file*/
      lineLenUL = 0;

      for(uiIter = 0; uiIter < charReadInUL; ++uiIter)
      { /*Loop: Count number of new lines & characters*/
         ++LineLen;         
         numLinesUL += lineStr[uiIter] == '\n';
      } /*Loop: Count number of new lines & characters*/

      maxLineLenUI = noBranchMax(maxLineLenUI, lineLenUL);

      CharReadInUL =
         fread(lineStr,sizeof(char),lenBuffUS,whoFILE);
   } /*Loop: Get the number of lines in the file*/

   if(numLinesUL < 2) return 0;

   --numLinesUL; /*Account for the header*/
   fseek(whoFILE, 0, SEEK_SET);
   maxLineLenUI += 2;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-0? Sec-03:
   ^  - Extract the AMR mutations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lineStr = malloc(maxLineLenUI, sizeof(char));

   if(lineStr == 0) return 0;

   amrST = calloc(numLinesUL, sizeof(char));

   if(AmrST == 0)
   { /*If: I had a memroy error*/
      free(lineStr);
      lineStr = 0;
      return 0;
   } /*If: I had a memroy error*/

   *numAmrUL = 0;

   fgets(lineStr, maxLineLenUI, whoFILE);

   while(fgets(LineStr, MaxLineLenUI, (whoFILE)))
   { /*Loop: Read in all lines in the WHO's amr file*/
     idIndexUI = 0;

     for(uiIter = 0; lineStr[uiIter] != ','; ++uiIter){}

     lenIdUI = uiIter - idIndexUI;

     ++uiIter;

     for(uiColIter = 0; uiColIter < 3; ++uiColIter)
     { /*Loop: Move to the genome_index column*/
        for(uiIter=uiIter;lineStr[uiIter]!=',';++uiIter){}
        ++uiIter;
     } /*Loop: Move to the genome_index column*/

     tmpStr =
        base10StrToUI(lineStr, amrST[*numAmrUL].refPosUI);

     uiIter = 1 + tmpStr - lineStr;

        armST[*numAmrUL].geneId[uiIter] =lineStr[uiIter];
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
        lenSeqUI = uiIter;
        lineStr[lenSeqUI] != ',';
        ++lenSeqUI
     ){} /*Loop: get length of the mutant sequence*/

     /*Make the buffer to hold the sequences*/
     ++lenSeqUI; /*Account for the null to terminate*/

     amrST[*numAmrUL].refSeqStr =
        malloc((lenSeqUI - uiIter( * sizeof(char));

     if(!amrST[*numAmrUL].refSeqStr)
     { /*If: I had a memroy error*/
        freeAmrStruct(amrST);
        free(lineStr);
        return 0;
     } /*If: I had a memroy error*/

     lenSeqUI = 0;

     /*Copy the reference sequence*/
     for(uiIter=uiIter; lineStr[uiIter] != ','; ++uiIter)
     { /*Loop: Copy the reference sequence*/
        amrST[*numAmrUL].refSeqStr[lenSeqUI] =
            lineStr[uiIter];

        ++lenSeqUI;
     }  /*Loop: Copy the reference sequence*/
     
     amrST[*numAmrUL].refSeqStr[lenSeqUI] = '\0';
     ++lenSeqUI;

     amrST[*numAmrUL].mutSeqStr =
        &amrST[*numAmrUL].refSeqStr[lenSeqUI];

     /*Copy the mutant (amr) sequence*/
     for(uiIter=uiIter; lineStr[uiIter] != ','; ++uiIter)
     { /*Loop: Copy the muterence sequence*/
        amrST[*numAmrUL].mutSeqStr[lenSeqUI] =
            lineStr[uiIter];

        ++lenSeqUI;
     }  /*Loop: Copy the muterence sequence*/

     amrST[*numAmrUL].mutSeqStr[lenSeqUI] = '\0';
     ++uiIter; /*Get off the ','*/

     /*Move to the mutant amino acid*/
     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter){}
     ++uiIter; /*Get off the ','*/

     /*I need to find the first base in the codon*/
     if(   lineStr[uiIter] != 'N'
        && lineStr[uiIter + 1] != 'A'
     ){ /*If: I have an amino acid change*/
        amrST[*numAmrUL].mutAaC =
          aaThreeLetteroChar(&lineStr[uiIter]);

        amrST[*numAmrUL].codonPosUL =
           amrST[*numAmrUL].refPosUI;

        if(amrST[*numAmrUL].refSeqStr[0]
           != amrST[*numAmrUL].mutSeqStr[0]
        ) goto amrCodonFirstBase;

        else if
              (amrST[*numAmrUL].refSeqStr[1]
           != amrST[*numAmrUL].mutSeqStr[1]
        ) --(amrST[*numAmrUL].condonPosUI);

        else (amrST[*numAmrUL].condonPosUI) -= 2;

        amrCondonFirstBase:
     } /*If: I have an amino acid change*/

     /*Move to the RIF AMR confidence column*/
     for(uiColIter = 0; uiColIter < 5; ++uiColIter)
     { /*Loop: Move to the genome_index column*/
        for(uiIter=uiIter;lineStr[uiIter]!=',';++uiIter){}
        ++uiIter;
     } /*Loop: Move to the genome_index column*/

     ++uiIter;

     amrST[*numAmrUL].armFlagsUL |=
        amrIsRes(lineStr[uiIter]);

     for(uiAmr = 1; uiAmr < 15; ++uiAmr)
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

        ++uiIter;

        amrST[*numAmrUL].armFlagsUL |=
           (amrIsRes(lineStr[uiIter]) << uiAmr);
     } /*Loop: Get the rating of all AMRs*/

     /*Move to the final position column*/
     for(uiIter=uiIter;lineStr[uiIter]!=',';++uiIter){}
     ++uiIter;
   
     tmpStr =
        base10StrToUI(lineStr, amrST[*numAmrUL].refPosUI);

     uiIter = 1 + tmpStr - lineStr;
     lenSeqUI = 0;

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter)
     { /*Loop: Copy the final reference decision*/
        amrST[*numAmrUL].refSeqStr[lenSeqUI] =
            lineStr[uiIter];

        ++lenSeqUI;
     } /*Loop: Copy the final reference decision*/
      
     amrST[*numAmrUL].refSeqStr[lenSeqUI] = '\0';
     ++uiIter; /*Get off the ','*/
     lenSeqUI = 0;

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter)
     { /*Loop: Copy the final mutant decision*/
        amrST[*numAmrUL].mutSeqStr[lenSeqUI] =
            lineStr[uiIter];

        ++lenSeqUI;
     } /*Loop: Copy the final mutant decision*/

     amrST[*numAmrUL].mutSeqStr[lenSeqUI] = '\0';
   } /*Loop: Read in all lines in the WHO's amr file*/

   return amrST;
} /*readWhoGenomeIndicieCsv*/
