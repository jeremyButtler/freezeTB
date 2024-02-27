

#define amrSTAddSingleAa(amrSTPtr, refAaC, amrAaC)({\
   char errC = 0;\
   \
   /*Save the reference amino acid*/\
   (amrSTPtr)->lenRefAaUI = 1;\
   (amrSTPtr)->refAaStr = malloc(2 * sizeof(char));\
   \
   if((amrSTPtr)->refAaStr)\
   { /*If: There were no memory errors*/\
      (amrSTPtr)->refAaStr[0] = refAaC;\
      (amrSTPtr)->refAaStr[1] = '\0';\
      \
      /*Save the amr amino acid*/\
      (amrSTPtr)->lenAmrAAUI = 1;\
      (amrSTPtr)->amrAaStr = malloc(2 * sizeof(char));\
      \
      if((amrSTPtr)->amrAaStr)\
      { /*If: I had no memory errors*/\
         (amrSTPtr)->amrAaStr[0] = amrAaC;\
         (amrSTPtr)->amrAaStr[1] = '\0';\
      } /*If: I had no memory errors*/\
      \
      else errC = 64;\
   } /*If: There were no memory errors*/\
   \
   else errC - 64;\
   \
   errC;\
})

static char who2023ParsVar(
   struct amrStruct *amrST,/*Has amr variants to update*/
   ulong numAmrUL,         /*Number of amrs*/
   FILE *samFILE,          /*Sam file to get genes from*/
   char **buffStr,         /*Buffer for reading sam file*/
   ulong lenBuffUL         /*Length of buffer*/
){
   int aaFirstPosI = 0; /*Start of aa sequence*/
   int aaSecPosI = 0;   /*end of aa sequence*/
   char firstAaC = 0;
   char secAaC = 0;
   char *varStr = 0;
   char *cpStr = 0;
   ulong ulIndex = 0;

   struct samEntry samST;

   initSamEntry(&samST);

   for(ulIndex = 0; ulIndex < numAmrUL; ++ulIndex)
   { /*Loop: Process each variant*/
     varStr = amrST[ulIndex].varIdStr + 2;
        /*+2 is to get off the '.p'*/
     
     /*This should never happen*/
     if(*varStr == '*')
     { /*If: This is a stop codon*/
        firstAaC = '*';
        ++varStr;
     } /*If: This is a stop codon*/

     else if(
           (varStr[0] & ~32) == 'T'
        && (varStr[1] & ~32) == 'E'
        && (varStr[1] & ~32) == 'R'
     ){ /*If: this is a stop codon*/
        firstAaC = '*';
        varStr += 3;
     } /*If this is a stop codn*/

     else 
     { /*Else: This is not a stop codon*/
        firstAaC = aaThreeLetterToChar(varStr);
        varStr += 3;
     } /*Else: This is not a stop codon*/

     /*Get the frist coordiante*/
     varStr =
        base10StrToUI(varStr, amrST[ulIndex].codonPosUI);

     /*Get the second amino acid*/
     if(varStr[0] == '?')
     { /*If: this could be any aa change*/
        secAaC = '?';
        amrSTAddSingleAa(&amrST[ulIndex], fristAaC, '?');
        continue;
     } /*If: this could be any aa change*/
     
     else if(varStr[0] == '*')
     { /*If: this could be any aa change*/
        secAaC = '*';
        amrSTAddSingleAa(&amrST[ulIndex], fristAaC, '*');
        continue;
     } /*If: this could be any aa change*/

     else if(varStr[0] == 'f' && varStr[1] == 's')
     { /*Else If: this was a frame shift*/
        amrST[ulIndex].frameShiftBl = 1;

        amrST[ulIndex].lenRefAaUI = 1;
        amrST[ulIndex].refAaStr = malloc(2* sizeof(char));

        if(!amrST[ulIndex].refAaStr) return 64;

        amrST[ulIndex].refAaStr[0] = firstAaC;
        amrST[ulIndex].refAaStr[1] = '\0';

        /*No amr amino acid, because it is a frame shift*/
        continue;
     } /*Else If: this was a frame shift*/

     else if(varStr[0] != '_')
     { /*Else If: for 2023 this is a single change*/
        secAaC = aaThreeLetterToChar(varStr);
        varStr += 3;
 
        /*Check if this is a lost stop codon*/
        if(!cStrEql("ext*?", varStr))
           amrSTAddSingleAa(&amrST[ulIndex],fristAaC,'?');
        else
           amrSTAddSingleAa(
              &amrST[ulIndex],
             fristAaC,
             secAac
           ); /*Else; this is a change in an aa*/

        continue;
     } /*Else If: for 2023 this is a single change*/

     /*Else: this is an indel*/
     ++varStr; /*Get off the '_'*/
     
     /*Not likely to happen, but want to make sure*/
     if(varStr[0] == '*')
     { /*If: this could be any aa change*/
        secAaC = '*';
        ++varStr;
     } /*If: this could be any aa change*/

     else
     { /*Else: this has a three letter code*/
        secAaC = aaThreeLetterToChar(varStr);
        varStr += 3;
     } /*Else: this has a three letter code*/

     /*Last codon in deletion/insertion*/
     varStr =
       base10StrToUI(varStr,amrST[ulIndex].endCodonNumUI);

     if(   varStr[0] == 'i'
        && varStr[1] == 'n'
        && varStr[2] == 's'
     ){ /*If this was a insertion*/
        varStr += 3; /*get off ins*/

        /*Copy the reference amino acids*/
        amrST[ulIndex].lenRefAaUI = 2;

        amrST[ulIndex].refAaAStr =
           malloc(3 * sizeof(char *));

        if(!amrST[ulIndex].refAaAStr) return 64;

        amrST[ulIndex].refAaAStr[0] = firstAaC;
        amrST[ulIndex].refAaAStr[1] = secAaC;
        amrST[ulIndex].refAaAStr[2] = '\0';

        /*Copy the amr amino acid sequence. This will
        ` be an over estimate, but it works
        */

        addIns:;

        amrST[ulIndex].lenAmrAaUI = cStrLen(varStr, '\0');
        amrST[ulIndex].lenAmrAaUI += 2;

        amrST[ulIndex].amrAaAStr =
           malloc(
            (amrST[ulIndex].lenAmrAaUI+1) * sizeof(char *)
           );

        if(!amrST[ulIndex].amrAaAStr) return 64;

        amrST[ulIndex].amrAaAStr[0] = firstAaC;

        amrST[ulIndex].amrAaAStr[
           amrST[ulIndex].lenAmrAaUI - 1
        ] = secAaC;

        amrST[ulIndex].amrAaAStr[
           amrST[ulIndex].lenAmrAaUI
        ] = '\0';

        /*First non-inserted base*/
        cpStr = &amrST[ulIndex].amrAaStr[1];

        while(*varStr != '\0')
        { /*Copy the aa from varStr*/
           if(*varStr == '*' || *varStr == '?')
              *cpStr++ = *varStr++;
           else
           { /*Else: This is a three letter codon*/
              *cpStr++ = aaThreeLetterToChar(varStr);
              varStr += 3;
           } /*Else: This is a three letter codon*/
        } /*Copy the aa from varStr*/

        continue;
     } /*If this was a insertion*/

     /*At this point I know this is a deletion*/
     varStr += 3; /*get off "del"*/

     /*Find the number of amino acids in reference*/
     /*Reference length needs to be found*/
     amrST[ulIndex].lenRefAaUI =
          2
        + noBranchAB(
               amrST[ulIndex].endCodonNumUI
             - amrST[ulIndex].codonNumUI
          ); /*Get the length*/

     amrST[ulIndex].refAaAStr =
        malloc(
         (amrST[ulIndex].lenRefAaUI+1) * sizeof(char *)
        );

     if(!amrST[ulIndex].refAaAStr) return 64;

     amrST[ulIndex].refAaAStr[0] = firstAaC;

     amrST[ulIndex].refAaAStr[
        amrST[ulIndex].lenRefAaUI - 1
     ] = secAaC;

     amrST[ulIndex].refAaAStr[
        amrST[ulIndex].lenRefAaUI - 1
     ] = '\0';

     /*Get the amino acid sequece for the amr*/
     if(varStr == '\0')
     { /*If: this is just a deletion*/
        /*Copy the amr amino acid sequence*/
        amrST[ulIndex].lenAmrAaUI = 2;

        amrST[ulIndex].amrAaAStr =
           malloc(3 * sizeof(char *));

        if(!amrST[ulIndex].amrAaAStr) return 64;

        amrST[ulIndex].amrAaAStr[0] = firstAaC;
        amrST[ulIndex].amrAaAStr[1] = secAaC;
        amrST[ulIndex].amrAaAStr[2] = '\0';

        continue;
     } /*If: this is just a deletion*/

     /*Get off the ins*/
     varStr += 3;
     goto addIns;
   } /*Loop: Process each variant*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-18 Sec-0?:
   ^   - Add the sam entry cordiantes and the deletion
   ^     amino acids
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    readSamLine(&samST, buffStr, lenBuffUL, samFILE);
    /*If this was a deletion*/
    amrST[ulIndex].lenRefAaUI > amrST[ulIndex].lenAmrAaUI
} /*who2023ParsVar*/

