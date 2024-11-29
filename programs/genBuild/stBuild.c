
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' stBuild SOF: Start Of File
'   - has build structures and their supporting functions
'   o header:
'     - included libraries
'   o tof01:
'     - var_stBuild structure and dedicated functions
'   o tof02:
'     - input_stBuild structure and dedicated functions
'   o tof02:
'     - input_stBuild structure and dedicated functions
'   o tof03:
'     - cmd_stBuild structure and dedicated functions
'   o tof04:
'     - config_stBuild structure and dedicated functions
'   o tof05:
'     - file_stBuild structure and dedicated functions
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Tof01:
'   - var_stBuild structure and dedicated functions
'   o .h st01: var_stBuild
'     - holds the variables used in build
'   o fun01: blank_var_stBuild
'     - blanks (sets strings to to null) a var_stBuild
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Fun01: blank_var_stBuild
|   - blanks (sets strings to to null) a var_stBuild
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild struct to blank
| Output:
|    - Modifies:
|      o all lengths to be 0
|      o all array values to be to '\0', 0, or null
\-------------------------------------------------------*/
void
blank_var_stBuild(
   struct var_stBuild *varSTPtr
){
   ulong ulCnt = 0;
   ulong endUI = 0;

   endUL = varSTPtr->lenVarUL;

   if(varSTPtr->idAryStr)
   { /*If: have varibale id*/
      varSTPtr->idAryStr[0] = '\0';
      varSTPtr->lenIdUS[0] = '\0';
   } /*If: have varibale id*/

   if(varSTPtr->valStr)
   { /*If: have value array*/
      varSTPtr->valStr[0] = '\0';
      varSTPtr->lenValUI = 0;
   } /*If: have value array*/

   if(varSTPtr->osAryStr)
   { /*If: have OS value*/
      varSTPtr->osStr[0] = '\0';
      varSTPtr->lenOSUS = 0;
   } /*If: have OS value*/

   varSTPtr->simdSC = 0;
   varSTPtr->bitSC = 0;
} /*blank_var_stBuild*/

/*-------------------------------------------------------\
| Fun02: init_var_stBuild
|   - initalizes a var_stBuild struct
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild struct to initialize
| Output:
|    - Modifies:
|      o sets all valueues and arrays to 0
\-------------------------------------------------------*/
void
init_var_stBuild(
   struct var_stBuild *varSTPtr
){
   varSTPtr->idStr = 0;
   varSTPtr->sizeIdUs = 0;

   varSTPtr->valStr = 0;
   varSTPtr->sizeValUI = 0;

   osSTPtr->osStr = 0;
   osSTPtr->sizeOsUS = 0;

   blank_var_stBuild(varSTPtr);
} /*init_var_stBuild

/*-------------------------------------------------------\
| Fun03: freeStack_var_stBuild
|   - freecs variables in a var_stBuild struct
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild with variables to free
| Output:
|    - Frees:
|      o varAryStr, varLenAryUS, and varSizeAryUS in
|        varSTPtr
|    - Modifies:
|      o initializes all variables after freeing
\-------------------------------------------------------*/
void
freeStack_var_stBuild(
   struct var_stBuild *varSTPtr
){
   uint uiString = 0;

   if(! varSTPtr)
      return; /*nothing to free*/

   if(varSTPtr->idStr)
      free(varSTPtr->idStr);

   if(varSTPtr->valStr)
      free(varSTPtr->valStr);

   if(varSTPtr->osStr)
      free(varSTPtr->osStr);

   init_var_stBuild(varSTPtr);
} /*freeStack_var_stBuild*/

/*-------------------------------------------------------\
| Fun04: freeHeap_var_stBuild
|   - frees a var_stBuild struct
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild struct to free
| Output:
|    - Frees:
|      o varSTPtr (you must set to 0)
\-------------------------------------------------------*/
void
freeHeap_var_stBuild(
   struct var_stBuild *varSTPtr
){
   if(! varSTPtr)
      return; /*nothing to free*/

   freeStack_var_stBuild(varSTPtr);
   free(varSTPtr);
} /*freeHeap_var_stBuild*/

/*-------------------------------------------------------\
| Fun05: mk_var_stBuild
|   - heap allocates a var_stBuild struct
| Input:
| Output:
|    - Returns:
|      o pointer to an initilized var_stBuild struct
|      o 0 for memory error
\-------------------------------------------------------*/
var_stBuild *
mk_var_stBuild(
){
   struct var_stBuild *retHeapST = 0;

   retHeapST = malloc(sizeof(struct var_stBuild));

   if(! retHeapStr)
      return 0;

   init_var_stBuild(retHeapST);

   return retHeapST;
} /*mk_var_stBuild*/

/*-------------------------------------------------------\
| Fun06: addId_var_stBuild
|   - add variable id to var_stBuild struct
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild struct to add OS to
|   - idStr:
|     o c-string with OS to add
|   - errSCPtr:
|     o pointer to signed char to hold error value
| Output:
|    - Modifies:
|      o osStr in varSTPtr to have id (resized if needed)
|      o lenOSUS in varSTPtr to have id length
|      o sizeOSUS in varSTPtr if idStr was resized
|      o errSCPtr:
|        - 0 for no errors
|        - def_memErr_stBuild for memory errors
|        - for invalid character set to problematic ascii
|          character
|    - Returns:
|      o length of id
|      o 0 for errors
\-------------------------------------------------------*/
unsigned int
addId_var_stBuild(
   struct var_stBuild *varSTPtr,
   signed char *idStr,
   signed char *errSCPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '   - add variable id to var_stBuild struct
   '   o fun06 sec01:
   '     - variable declarations
   '   o fun06 sec02:
   '     - find length of id
   '   o fun06 sec03:
   '     - allocate memory for id
   '   o fun06 sec04:
   '     - copy variable id
   '   o fun06 sec05:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   unsigned int lenUI = 0;
   signed char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - find length of id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(*tmpStr > 32)
   { /*Loop: find id length*/
      /*special variable flags (end of id)*/
      if(*tmpStr == def_osMark_buildSymbols)
         break;

      if(*tmpStr == def_bitMark_buildSymbols)
         break;

      if(*tmpStr == def_simdMark_buildSymbols)
         break;

      if(*tmpStr < 48)
         goto invalidChar_fun06_sec05;

      if(
            *tmpStr > 57
         && *tmpStr < 65
      ) goto invalidChar_fun06_sec05; /*not number*/

      if(
            *tmpStr > 57
         && *tmpStr < 65
      ) goto invalidChar_fun06_sec05; /*not number*/

      if(
            *tmpStr > 90
         && *tmpStr < 95
      ) goto invalidChar_fun06_sec05; /*not upper or _*/

      if(*tmpStr == 96)
         goto invalidChar_fun06_sec05; /*grave `*/

      if(*tmpStr > 123)
         goto invalidChar_fun06_sec05; /*not lower case*/

      ++tmpStr;
      ++lenUI;
   } /*Loop: find id length*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - allocate memory for id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   if(varSTPtr->sizeIdUI < lenUI)
   { /*If: need more room for id*/
      free(varSTPtr);
      varSTPtr = 0;

      goto alloc_fun06_sec03;
   } /*If: need more room for id*/
   
   if(! varSTPtr->idStr)
   { /*If: need to allocated memory*/
      alloc_fun06_sec03:;

      varSTPtr->idStr =
         malloc((lenUI + 8) * sizeof(schar));
         /*+ 8 to avoid read out of bound from ulCp*/

      if(! varSTPtr->idStr)
         goto memErr_fun06_sec05;

      varSTPtr->sizeIdUI = lenUI;
   } /*If: need to allocated memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec04:
   ^   - copy variable id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cpLen_ulCp(
      varSTPtr->idStr,
      idStr,
      lenUI
   );

   varSTPtr->lenIdUI = lenUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec05:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun06_sec05;

   memErr_fun06_sec05:;
      *errSCPtr = def_memErr_stBuild;
      goto ret_fun06_sec05;

   invalidChar_fun06_sec05:;
      *errSCPtr = *tmpStr;
      goto ret_fun06_sec05;

   ret_fun06_sec05;
      return lenUI;
} /*addId_var_stBuild*/

/*-------------------------------------------------------\
| Fun07: addVal_var_stBuild
|   - add variable value to var_stBuild struct
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild struct to add value to
|   - valStr:
|     o c-string with value to add
|   - errSCPtr:
|     o pointer to signed char to hold error value
| Output:
|    - Modifies:
|      o valStr in varSTPtr to have value (may be resized)
|      o lenValUS in varSTPtr to have value length
|      o sizeValUS in varSTPtr if valStr was resized
|      o errSCPtr:
|        - 0 for no errors
|        - def_memErr_stBuild for memory errors
|        - for invalid character set to problematic ascii
|          character
|    - Returns:
|      o length of valStr string
|      o 0 for errors
\-------------------------------------------------------*/

/*-------------------------------------------------------\
| Fun08: addOS_var_stBuild
|   - add variable OS to var_stBuild struct
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild struct to add os to
|   - osStr:
|     o c-string with os to add
|   - errSCPtr:
|     o pointer to signed char to hold error value
| Output:
|    - Modifies:
|      o osStr in varSTPtr to have OS (resized if needed)
|      o lenOSUS in varSTPtr to have OS length
|      o sizeOSUS in varSTPtr if osStr was resized
|      o errSCPtr:
|        - 0 for no errors
|        - def_memErr_stBuild for memory errors
|        - for invalid character set to problematic ascii
|          character
|    - Returns:
|      o length of OS string
|      o 0 for errors
\-------------------------------------------------------*/
unsigned int
addOS_var_stBuild(
   struct var_stBuild *varSTPtr,
   signed char *osStr,
   signed char *errSCPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '   - add variable id to var_stBuild struct
   '   o fun08 sec01:
   '     - variable declarations
   '   o fun08 sec02:
   '     - find length of id
   '   o fun08 sec03:
   '     - allocate memory for id
   '   o fun08 sec04:
   '     - copy variable id
   '   o fun08 sec05:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   unsigned int lenUI = 0;
   signed char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - find length of id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(*tmpStr > 32)
   { /*Loop: find id length*/
      /*special variable flags (end of id)*/
      if(*tmpStr == def_osMark_buildSymbols)
         break;

      if(*tmpStr == def_bitMark_buildSymbols)
         break;

      if(*tmpStr == def_simdMark_buildSymbols)
         break;

      if(*tmpStr < 48)
         goto invalidChar_fun08_sec05;

      if(
            *tmpStr > 57
         && *tmpStr < 65
      ) goto invalidChar_fun08_sec05; /*not number*/

      if(
            *tmpStr > 57
         && *tmpStr < 65
      ) goto invalidChar_fun08_sec05; /*not number*/

      if(
            *tmpStr > 90
         && *tmpStr < 95
      ) goto invalidChar_fun08_sec05; /*not upper or _*/

      if(*tmpStr == 96)
         goto invalidChar_fun08_sec05; /*grave `*/

      if(*tmpStr > 123)
         goto invalidChar_fun08_sec05; /*not lower case*/

      ++tmpStr;
      ++lenUI;
   } /*Loop: find id length*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - allocate memory for OS
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   if(varSTPtr->sizeOSUI < lenUI)
   { /*If: need more room for os*/
      free(varSTPtr);
      varSTPtr = 0;

      goto alloc_fun08_sec03;
   } /*If: need more room for os*/
   
   if(! varSTPtr->osStr)
   { /*If: need to allocated memory*/
      alloc_fun08_sec03:;

      varSTPtr->osStr =
         malloc((lenUI + 8) * sizeof(schar));
         /*+ 8 to avoid read out of bound from ulCp*/

      if(! varSTPtr->osStr)
         goto memErr_fun08_sec05;

      varSTPtr->sizeOSUI = lenUI;
   } /*If: need to allocated memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec04:
   ^   - copy variable id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cpLen_ulCp(
      varSTPtr->osStr,
      osStr,
      lenUI
   );

   varSTPtr->lenOSUI = lenUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec05:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun08_sec05;

   memErr_fun08_sec05:;
      *errSCPtr = def_memErr_stBuild;
      goto ret_fun08_sec05;

   invalidChar_fun08_sec05:;
      *errSCPtr = *tmpStr;
      goto ret_fun08_sec05;

   ret_fun08_sec05;
      return lenUI;
} /*addOS_var_stBuild*/

/*-------------------------------------------------------\
| Fun09: addSIMD_var_stBuild
|   - add variable simd value to var_stBuild struct
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild struct to add os to
|   - simdStr:
|     o c-string with simd value to add
|   - errSCPtr:
|     o pointer to signed char to hold error value
| Output:
|    - Modifies:
|      o simdSC in varSTPtr to have correct simd flag
|      o errSCPtr:
|        - 0 for no errors
|        - for invalid character set to problematic ascii
|          character
|    - Returns:
|      o length of simdStr string
|      o 0 for errors
\-------------------------------------------------------*/
unsigned int
addSIMD_var_stBuild(
   struct var_stBuild *varSTPtr,
   signed char *simdStr,
   signed char *errSCPtr
){
   unsigned int lenUI = 0;
   uint simdUI = 0; /*marks if found simd type*/

   while(*simdStr > 32)
   { /*Loop: get SIMD type*/
      /*special variable flags (end of simd)*/
      if(*tmpStr == def_osMark_buildSymbols)
         break;

      if(*tmpStr == def_bitMark_buildSymbols)
         break;

      if(*tmpStr == def_simdMark_buildSymbols)
         break;

      if(
         idStr[0] == 'n'
         idStr[1] == 'e'
         idStr[2] == 'o'
         idStr[3] == 'n'
      ){ /*If: neon variable*/
         idStr += 4;
         varSTPtr.simdSC = def_neon_osDetect;
         ++simdUI;
      } /*If: neon variable*/

      else if(
         idStr[0] == 's'
         idStr[1] == 's'
         idStr[2] == 'e'
      ){ /*Else If: sse simd variable*/
         idStr += 3;
         varSTPtr.simdSC = def_neon_osDetect;
         ++simdUI;
      } /*Else If: sse simd variable*/

      else if(
         idStr[0] == 's'
         idStr[1] == 's'
         idStr[2] == 'e'
         idStr[3] == '2'
      ){ /*Else If: sse2 simd variable*/
         idStr += 4;
         varSTPtr.simdSC = def_neon_osDetect;
         ++simdUI;
      } /*Else If: sse2 simd variable*/

      else if(
         idStr[0] == 's'
         idStr[1] == 's'
         idStr[2] == 'e'
         idStr[3] == '4'
      ){ /*Else If: sse4 simd variable*/
         idStr += 4;
         varSTPtr.simdSC = def_neon_osDetect;
         ++simdUI;
      } /*Else If: sse4 simd variable*/

      else if(
         idStr[0] == 'a'
         idStr[1] == 'v'
         idStr[2] == 'x'
         idStr[3] == '2'
      ){ /*Else If: avx2 simd variable*/
         idStr += 4;
         varSTPtr.simdSC = def_neon_osDetect;
         ++simdUI;
      } /*Else If: avx2 simd variable*/

      else if(
         idStr[0] == 'a'
         idStr[1] == 'v'
         idStr[2] == 'x'
         idStr[3] == '5'
         idStr[4] == '1'
         idStr[5] == '2'
      ){ /*Else If: avx512 simd variable*/
         idStr += 6;
         varSTPtr.simdSC = def_neon_osDetect;
         ++simdUI;
      } /*Else If: avx512 simd variable*/

      else if(
         idStr[0] == 'n'
         idStr[1] == 'o'
         idStr[2] == 'n'
         idStr[3] == 'e'
      ){ /*Else If: no simd support*/
         idStr += 4;
         varSTPtr.simdSC = 0;
         ++simdUI;
      } /*Else If: no simd support*/

      else
         ++idStr;
   } /*Loop: get SIMD type*/

   id(! simdUI)
      goto noSIMD_fun09_sec0x;

   id(simdUI > 1)
      goto multiSIMD_fun09_sec0x;

   /*see if have a valid ending*/
   if(*tmpStr < 33)
      goto ret_fun09_sec0x;

   else if(*tmpStr == def_osMark_buildSymbols)
      goto ret_fun09_sec0x;

   else if(*tmpStr == def_bitMark_buildSymbols)
      goto ret_fun09_sec0x;

   else if(*tmpStr == def_simdMark_buildSymbols)
      goto ret_fun09_sec0x;

   else
      goto invalidSIMD_fun09_sec0x;
}

/*-------------------------------------------------------\
| Fun09: cmp_var_stBuild
|   - compares two variables to see if same
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild with variables to compare
|   - fristUI:
|     o index of frist variable to compare
|   - secUI:
|     o index of second variable to compare
| Output:
|   - Returns:
|     o 0 if variables have same name and os
|     o > 0 if frist variables is greater then second
|     o < 0 if frist variables is less then second
\-------------------------------------------------------*/
signed char
cmp_var_stBuild(
   struct var_stBuild *varSTPtr,/*has variable list*/
   uint firstUI,          /*first index to compare*/
   uint secUI             /*second index to compare*/
){
   schar *firstStr = varSTPtr->varAryStr[firstUI];
   schar *secStr = varSTPtr->varAryStr[secUI];


   /*see if same variable name*/


   while(*firstStr == *secStr)
   { /*Loop: till found difference*/
      if(*fristStr == '\0')
         break;

      if(*secStr == '\0')
         break;

      ++firstStr;
      ++secStr;
   } /*Loop: till found difference*/

   if(*fristStr != *secStr)
      return *fristStr - secStr;


   /*see if have same OS*/


   firstStr = varSTPtr->osAryStr[firstUI];
   secStr = varSTPtr->osAryStr[secUI];

   while(*firstStr == *secStr)
   { /*Loop: till found difference*/
      if(*fristStr == '\0')
         break;

      if(*secStr == '\0')
         break;

      ++firstStr;
      ++secStr;
   } /*Loop: till found difference*/

   return *fristStr - secStr;
} /*cmp_var_stBuild*/

/*-------------------------------------------------------\
| Fun10: cmpQry_var_stBuild
|   - comapres query to var in a a var_stBuild struct
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild with variables to compare
|   - refUI:
|     o index of reference variable to compare to
|   - qryVarStr:
|     o c-string with query variable name
|   - qryOSStr:
|     o c-string with query OS name
| Output:
|   - Returns:
|     o 0 if variables have same name and os
|     o > 0 if frist variables is greater then second
|     o < 0 if frist variables is less then second
\-------------------------------------------------------*/
signed char
cmpQry_var_stBuild(
   struct var_stBuild *varSTPtr,/*has variable list*/
   uint refUI,          /*reference index to compare*/
   schar *qryVarStr,    /*variable name of query*/
   schar *qryOsStr      /*query OS query*/
){
   schar *firstStr = varSTPtr->varAryStr[refUI];

   /*see if same variable name*/

   while(*firstStr == *qryVarStr)
   { /*Loop: till found difference*/
      if(*fristStr == '\0')
         break;

      if(*qryVarStr == '\0')
         break;

      ++firstStr;
      ++qryVarStr;
   } /*Loop: till found difference*/

   if(*fristStr != *qryVarStr)
      return *fristStr - qryVarStr;


   /*see if have same OS*/


   firstStr = varSTPtr->osAryStr[refUI];

   while(*firstStr == *qryOsStr)
   { /*Loop: till found difference*/
      if(*fristStr == '\0')
         break;

      if(*qryOsStr == '\0')
         break;

      ++firstStr;
      ++qryOsStr;
   } /*Loop: till found difference*/

   return *fristStr - qryOsStr;
} /*cmpQry_var_stBuild*/

/*-------------------------------------------------------\
| Fun11: swap_var_stBuild
|   - swaps two variables in a var_stBuild struct
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild with variables to swap
|   - fristUI:
|     o index of frist variable to swap
|   - secUI:
|     o index of second variable to swap
| Output:
|   - Modifies:
|     o varAryStr, varLenAryUS, and varSizeAryUS at
|       firstIndexUI to be values at secIndexUI
|     o varAryStr, varLenAryUS, and varSizeAryUS at
|       secIndexUI to be values at firstIndexUI
\-------------------------------------------------------*/
void
swap_var_stBuild(
   struct var_stBuild *varSTPtr,/*has variable list*/
   uint firstUI,          /*first index to swap*/
   uint secUI             /*second index to swap*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - swaps two variables in a var_stBuild struct
   '   o fun11 sec01:
   '     - variable declarations
   '   o fun11 sec02:
   '     - swap variable name array elements
   '   o fun11 sec03:
   '     - swap os array elements
   '   o fun11 sec04:
   '     - swap value array elements
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *swapStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - swap variable name array elements
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *swapStr = varSTPtr->varAryStr[firstUI];
   varSTPtr->varAryStr[firstUI] =
      varSTPtr->varAryStr[secUI];
   varSTPtr->varAryStr[secUI] = swapStr;

   varSTPtr->varLenAryUI[firstUI] ^=
      varSTPtr->varLenAryUI[secUI];
   varSTPtr->varLenAryUI[secUI] ^=
      varSTPtr->varLenAryUI[firstUI];
   varSTPtr->varLenAryUI[firstUI] ^=
      varSTPtr->varLenAryUI[secUI];

   varSTPtr->varSizeAryUI[firstUI] ^=
      varSTPtr->varSizeAryUI[secUI];
   varSTPtr->varSizeAryUI[secUI] ^=
      varSTPtr->varSizeAryUI[firstUI];
   varSTPtr->varSizeAryUI[firstUI] ^=
      varSTPtr->varSizeAryUI[secUI];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - swap os array elements
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *osStr = varSTPtr->osAryStr[firstUI];
   osSTPtr->osAryStr[firstUI] =
      osSTPtr->osAryStr[secUI];
   osSTPtr->osAryStr[secUI] = swapStr;

   varSTPtr->osLenAryUI[firstUI] ^=
      varSTPtr->osLenAryUI[secUI];
   varSTPtr->osLenAryUI[secUI] ^=
      varSTPtr->osLenAryUI[firstUI];
   varSTPtr->osLenAryUI[firstUI] ^=
      varSTPtr->osLenAryUI[secUI];

   varSTPtr->osSizeAryUI[firstUI] ^=
      varSTPtr->osSizeAryUI[secUI];
   varSTPtr->osSizeAryUI[secUI] ^=
      varSTPtr->osSizeAryUI[firstUI];
   varSTPtr->osSizeAryUI[firstUI] ^=
      varSTPtr->osSizeAryUI[secUI];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - swap value array elements
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *valueStr = varSTPtr->valueAryStr[firstUI];
   valueSTPtr->valueAryStr[firstUI] =
      valueSTPtr->valueAryStr[secUI];
   valueSTPtr->valueAryStr[secUI] = swapStr;

   varSTPtr->valueLenAryUI[firstUI] ^=
      varSTPtr->valueLenAryUI[secUI];
   varSTPtr->valueLenAryUI[secUI] ^=
      varSTPtr->valueLenAryUI[firstUI];
   varSTPtr->valueLenAryUI[firstUI] ^=
      varSTPtr->valueLenAryUI[secUI];

   varSTPtr->valueSizeAryUI[firstUI] ^=
      varSTPtr->valueSizeAryUI[secUI];
   varSTPtr->valueSizeAryUI[secUI] ^=
      varSTPtr->valueSizeAryUI[firstUI];
   varSTPtr->valueSizeAryUI[firstUI] ^=
      varSTPtr->valueSizeAryUI[secUI];
} /*swap_var_stBuild*/

/*-------------------------------------------------------\
| Fun12: sort_var_stBuild
|   - sorts arrays in var_stBuild struct (id, then os)
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild with variables to sort
| Output:
|   - Modifies:
|     o arrays in varSTPtr to be sorted by id and os
\-------------------------------------------------------*/
void
sort_var_stBuild(
   struct var_stBuild *varSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun12 TOC:
   '   - sorts arrays in var_stBuild struct (id, then os)
   '   o fun12 sec01:
   '     - variable declerations
   '   o fun12 sec02:
   '     - find the number of rounds to sort for
   '   o fun12 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   ulong long numElmUL = varSTPtr->lenUI;

   /*Number of sorting roun12*/
   ulong subUL = 0;
   ulong nextUL = 0;
   ulong lastUL = 0;
   ulong onUL = 0;

   /*Variables to incurment loops*/
   ulong ulIndex = 0;
   ulong ulElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - find the max search value (number roun12 to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numElmUL < 2)
      return; /*nothing to sort*/

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all roun12*/
      for(
         ulIndex = 0;
         ulIndex <= subUL;
         ++ulIndex
      ){ /*Loop: though sub array*/
         ulElm = ulIndex;

         for(
            ulElm = ulIndex;
            ulElm + subUL <= endUL;
            ulElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = ulElm + subUL;

            if(
               cmp_var_stBuild(
                  varSTPtr,
                  ulElm,
                  nextElm
               ) < 0
            ){ /*If I need to swap an element*/
               swap_var_stBuild(
                  varSTPtr,
                  ulElm,
                  nextElm
               );

               lastUL = ulElm;
               onUL = ulElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(
                     cmp_var_stBuild(
                        varSTPtr,
                        onUL,
                        lastUL
                     ) < 0
                  ) break;

                  swap_var_stBuild(
                     varSTPtr,
                     onUL,
                     lastUL
                  );

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next roun12/
   } /*Loop: all roun12*/
} /*sort_var_stBuild*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Tof02:
'   - input_stBuild structure and dedicated functions
'   o .h st02: input_stBuild
'     - holds user input variables (over rides all else)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Tof03:
'   - cmd_stBuild structure and dedicated functions
'   o .h st03: cmd_stBuild
'     - holds a user defined build command
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Tof04:
'   - config_stBuild structure and dedicated functions
'   o .h st04: config_stBuild
'     - holds user defined configure commands
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Tof05:
'   - file_stBuild structure and dedicated functions
'   o .h st05: file_stBuild
'     - holds a files variables
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
