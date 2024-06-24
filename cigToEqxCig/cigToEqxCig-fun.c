/*########################################################
# Name: cigToEqxCig-fun
#   - functions to convert an regular sam cigar to an eqx
#     cigar
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries and .c only defined variables
'   o .c tbl-01 baseToFiveBit:
'     - Table to convert bases to six bit values
'   o fun-01: samCigToEqxCig
'     - convert an sam entry cigar to an eqx cigar
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and .c only defined varaibles
|   o header sec-01:
|     - included libraries
|   o header sec-02:
|     - defined variables
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-01:
^   - included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "cigToEqxCig-fun.h"

#include <stdio.h>

#include "../generalLib/samEntryStruct.h"

/*No .c files (.h only)*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"
#include "../generalLib/ntToFiveBit.h" /*look up tables*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .h #include "../generalLib/base10StrToNum.h"
!   o .h #include "../generalLib/numToStr.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-02:
^   - defined variables
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_shiftCharToInt 2  /*Convert char to int*/

/*-------------------------------------------------------\
| Fun-01: cigToEqxCig 
|   - convert an sam entry cigar to an eqx cigar
| Input:
|   - samSTPtr:
|     o pointer to an samEntry struct to convert cigar
|       into an eqx cigar entry
|   - refStr:
|     o pointer to an reference sequence to use in the
|       conversion
|   - lenRefSI:
|     o length of the reference sequence (int)
|   - strBuffPtr:
|     o pointer to an c-string buffer to hold the cigar
|       entry temporarly
|   - valArySI:
|     o pointer to an int array to hold the cigar
|       entry temporarly
|   - lenBuffSI:
|     o pointer to current length of strBuffPtr and
|       valArySI
|   - nAsSnpBl:
|     o 1: Treat anymous bases as SNPs always
|     o 0: Treat annoymous bases as matches, if they match
| Output:
|   - Modifies:
|     o cigTypeStr in samSTPtr to be an eqx entry
|     o cigValAryI in samSTPtr to have counts for the
|       eqx entry
|     o strBuffPtr to be larger if needed
|     o valArySI to be larger if needed
|     o lenBuffSI to hold new buffer size if needed
|   - Returns:
|     o 0 for success
|     o 1 for memory error
|       - does not free strBuffPtr (you need to free)
|       - does not free valArySI   (you need to free)
\-------------------------------------------------------*/
signed char
cigToEqxCig(
   struct samEntry *samSTPtr,
   signed char *refStr,
   signed int lenRefSI,
   signed char **strBuffPtr,
   signed int **valArySI,
   signed int *lenBuffSI,
   signed char nAsSnpBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-01 TOC:
   '   - convert an sam entry cigar to an eqx cigar
   '   o fun-01 sec-01:
   '     - variable declerations
   ^ Fun-01 Sec-02:
   ^   - memory allocation for buffers
   ^ Fun-01 Sec-03:
   ^   - make the new cigar entry
   ^ Fun-01 Sec-04:
   ^   - copy new cigar entry to sam entry
   ^ Fun-01 Sec-05:
   ^   - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;

   sint siCig = 0;
   sint siNumNtCig = 0;
   sint siNewCig = 0;

   uint refPosUI = 0;
   uint seqPosUI = 0;

   schar refNtSC = 0;
   schar qryNtSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-02:
   ^   - memory allocation for buffers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*check if I have an cigar entry*/
   if(samSTPtr->cigTypeStr[0] == '*')
      goto finishAndExit_fun01_sec02_sub01;

   /*check if I have an sequence entry*/
   if(samSTPtr->seqStr[0] == '*')
      goto finishAndExit_fun01_sec02_sub01;

   if(*strBuffPtr == 0)
   { /*If: I need to allocate memory*/
      *lenBuffSI = samSTPtr->lenCigUI << 1;

      *strBuffPtr =
         malloc((*lenBuffSI + 1) * sizeof(schar));

      if(! strBuffPtr)
         goto memErr_fun01_sec02_sub02;

      *valArySI = malloc(*lenBuffSI * sizeof(sint));

      if(! valArySI)
         goto memErr_fun01_sec02_sub02;
   } /*If: I need to allocate memory*/

   else if(*lenBuffSI < samSTPtr->readLenUI)
   { /*Else If: I need to reallocate memory*/
      *lenBuffSI = samSTPtr->readLenUI << 1;

      tmpStr = malloc((*lenBuffSI +1) * sizeof(schar));

      if(! tmpStr)
         goto memErr_fun01_sec02_sub02;

      free(*strBuffPtr);
      *strBuffPtr = 0;
      *strBuffPtr = tmpStr;

      tmpStr= (schar *) malloc(*lenBuffSI * sizeof(sint));

      if(! valArySI)
         goto memErr_fun01_sec02_sub02;

      free(*valArySI);
      *valArySI = 0;
      *valArySI = (sint *) tmpStr;
   } /*Else If: I need to reallocate memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-03:
   ^   - make the new cigar entry
   ^   o fun-01 sec-03 sub-01:
   ^     - set up, start loop, and start cigar checks
   ^   o fun-01 sec-03 sub-02:
   ^     - handle SNP cases
   ^   o fun-01 sec-03 sub-03:
   ^     - handle match cases
   ^   o fun-01 sec-03 sub-04:
   ^     - handle SNP or match cases
   ^   o fun-01 sec-03 sub-05:
   ^     - handle insertion and sofmask cases
   ^   o fun-01 sec-03 sub-06:
   ^     - handle deletion cases
   ^   o fun-01 sec-03 sub-07:
   ^     - handle any other cases (hard mask likely)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-01 Sec-03 Sub-01:
   *   - set up, start loop, and start cigar checks
   \*****************************************************/

   refPosUI = samSTPtr->refStartUI;
   siNewCig = 0; /*So gets incurmented to zero*/
   (*strBuffPtr)[0] = 0;

   for(siCig = 0; siCig < samSTPtr->lenCigUI; ++siCig)
   { /*Loop: Check all cigar entries*/
      switch(samSTPtr->cigTypeStr[siCig])
      { /*Switch: Check the cigar entry type*/

         /***********************************************\
         * Fun-01 Sec-03 Sub-02:
         *   - handle SNP cases
         \***********************************************/

         case 'X':
         /*Case: SNPs*/
            if(! nAsSnpBl)
            { /*If: I an converting N's to matches*/
               siNumNtCig = samSTPtr->cigValAryI[siCig];

               while(siNumNtCig > 0)
               { /*Loop: break up by anonymous bases*/
                  qryNtSC =
                     ntToFiveBit[
                         (uchar)
                         samSTPtr->seqStr[seqPosUI]
                     ]; /*Convert query to code*/

                   refNtSC =
                     ntToFiveBit[
                        (uchar)
                        refStr[refPosUI]
                     ]; /*Convert reference to code*/

                  if(refNtSC & qryNtSC)
                  { /*If: Is match (aonymous bases)*/
                     if((*strBuffPtr)[siNewCig] != '=')
                     { /*If: Need to make an match entry*/
                        siNewCig+= ((*strBuffPtr)[0] !=0);

                        (*strBuffPtr)[siNewCig] = '=';
                        (*valArySI)[siNewCig] = 1;
                     } /*If: Need to make an match entry*/

                     else
                        ++( (*valArySI)[siNewCig] );
                  } /*If: Is match (aonymous bases)*/

                  else if((*strBuffPtr)[siNewCig] != 'X')
                  { /*Else If: started an new entry*/
                     siNewCig += ((*strBuffPtr)[0] != 0);

                     (*strBuffPtr)[siNewCig] = 'X';
                     (*valArySI)[siNewCig] = 1;
                  } /*Else If: started an new entry*/

                  else
                     ++( (*valArySI)[siNewCig] );

                  --siNumNtCig;
                  ++refPosUI;
                  ++seqPosUI;
               } /*Loop: break up by anonymous bases*/
            } /*If: I an converting N's to matches*/

            else
            { /*Else: I am doing no changes*/
               siNewCig += ((*strBuffPtr)[0] != 0);

               (*strBuffPtr)[siNewCig] = 'X';

               (*valArySI)[siNewCig] =
                  samSTPtr->cigValAryI[siCig];

               refPosUI += samSTPtr->cigValAryI[siCig];
               seqPosUI += samSTPtr->cigValAryI[siCig];
            } /*Else: I am doing no changes*/

            break;
         /*Case: SNPs*/

         /***********************************************\
         * Fun-01 Sec-03 Sub-03:
         *   - handle match cases
         \***********************************************/

         case '=':
         /*Case: Matches*/
            if(nAsSnpBl)
            { /*If: I an converting N's to matches*/
               siNumNtCig = samSTPtr->cigValAryI[siCig];

               while(siNumNtCig > 0)
               { /*Loop: break up by anonymous bases*/
                  qryNtSC =
                     ntToFiveBit[
                         (uchar)
                         samSTPtr->seqStr[seqPosUI]
                     ];

                   refNtSC =
                      ntToFiveBit[
                         (uchar)
                         refStr[refPosUI]
                      ];

                  if(
                        (refNtSC & n_fiveBit)
                     || (qryNtSC & n_fiveBit)
                  ){ /*If: Is match (aonymous bases)*/
                     if((*strBuffPtr)[siNewCig] != 'X')
                     { /*If: Need to make an SNP entry*/
                        siNewCig+= ((*strBuffPtr)[0] !=0);
                        (*strBuffPtr)[siNewCig] = 'X';
                        (*valArySI)[siNewCig] = 1;
                     } /*If: Need to make an SNP entry*/

                     else
                        ++( (*valArySI)[siNewCig] );
                  } /*If: Is match (aonymous bases)*/

                  else if((*strBuffPtr)[siNewCig] != '=')
                  { /*Else If: neither base is an N*/
                      siNewCig += ((*strBuffPtr)[0] != 0);
                     (*strBuffPtr)[siNewCig] = '=';
                     (*valArySI)[siNewCig] = 1;
                  } /*Else If: neither base is an N*/

                  else
                     ++( (*valArySI)[siNewCig] );

                  --siNumNtCig;
                  ++refPosUI;
                  ++seqPosUI;
               } /*Loop: break up by anonymous bases*/
            } /*If: I an converting N's to matches*/

            else
            { /*Else: I am doing no changes*/
               siNewCig += ((*strBuffPtr)[0] != 0);

               (*strBuffPtr)[siNewCig] = '=';

               (*valArySI)[siNewCig] =
                  samSTPtr->cigValAryI[siCig];

               refPosUI += samSTPtr->cigValAryI[siCig];
               seqPosUI += samSTPtr->cigValAryI[siCig];
            } /*Else: I am doing no changes*/

            break;
         /*Case: Matches*/

         /***********************************************\
         * Fun-01 Sec-03 Sub-04:
         *   - handle SNP or match cases
         *   o fun-01 sec-03 sub-04 cat-01:
         *     - Start loop and get sixBit code
         *   o fun-01 sec-03 sub-04 cat-02:
         *     - mismatch regardless if anonymous
         *   o fun-01 sec-03 sub-04 cat-03:
         *     - match, one base is anonymous
         *   o fun-01 sec-03 sub-04 cat-04:
         *     - match, nothing is anonymous
         *   o fun-01 sec-03 sub-04 cat-05:
         *     - move to next base
         *   o fun-01 sec-03 sub-04 cat-06:
         *     - move to next cigar entry and restart
         \***********************************************/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun-01 Sec-03 Sub-04 Cat-01:
         +   - Start loop and get sixBit code
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 'M':
         /*Case: Matches and SNPs*/
            siNumNtCig = samSTPtr->cigValAryI[siCig];

            while(siNumNtCig > 0)
            { /*Loop: break up by anonymous bases*/
               qryNtSC =
                  ntToFiveBit[
                      (uchar)
                      samSTPtr->seqStr[seqPosUI]
                  ];

                refNtSC =
                   ntToFiveBit[
                      (uchar)
                      refStr[refPosUI]
                   ];

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun-01 Sec-03 Sub-04 Cat-02:
               +   - mismatch regardless if anonymous
               \++++++++++++++++++++++++++++++++++++++++*/

               if(! (refNtSC & qryNtSC) )
               { /*If: This is not an match*/
                  if((*strBuffPtr)[siNewCig] != 'X')
                  { /*If: Need to make an SNP entry*/
                     siNewCig += ((*strBuffPtr)[0] != 0);
                     (*strBuffPtr)[siNewCig] = 'X';
                     (*valArySI)[siNewCig] = 1;
                  } /*If: Need to make an SNP entry*/

                  else
                     ++( (*valArySI)[siNewCig] );
               } /*If: This is not an match*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun-01 Sec-03 Sub-04 Cat-03:
               +   - match, one base is anonymous
               \++++++++++++++++++++++++++++++++++++++++*/

               else if(
                     (refNtSC & n_fiveBit)
                  || (qryNtSC & n_fiveBit)
               ){ /*Else If: Is aonymous:*/
                  if(nAsSnpBl)
                  { /*If: anonymous bases are SNPs*/
                     if((*strBuffPtr)[siNewCig] != 'X')
                     { /*If: Need to make an SNP entry*/
                        siNewCig+= ((*strBuffPtr)[0] !=0);
                        (*strBuffPtr)[siNewCig] = 'X';
                        (*valArySI)[siNewCig] = 1;
                     } /*If: Need to make an SNP entry*/

                     else
                        ++( (*valArySI)[siNewCig] );
                  } /*If: anonymous bases are SNPs*/

                  else
                  { /*Anonymous bases are matches*/
                     if((*strBuffPtr)[siNewCig] != '=')
                     { /*Else If: neither base is an N*/
                        siNewCig+= ((*strBuffPtr)[0] !=0);
                        (*strBuffPtr)[siNewCig] = '=';
                        (*valArySI)[siNewCig] = 1;
                     } /*Else If: neither base is an N*/

                     else
                        ++( (*valArySI)[siNewCig] );
                  } /*Anonymous bases are matches*/
               } /*Else If: is anonymous*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun-01 Sec-03 Sub-04 Cat-04:
               +   - match, nothing is anonymous
               \++++++++++++++++++++++++++++++++++++++++*/

               else
               { /*Else: Is an non-aonymous match*/
                  if((*strBuffPtr)[siNewCig] != '=')
                  { /*Else If: neither base is an N*/
                     siNewCig += ((*strBuffPtr)[0] != 0);
                     (*strBuffPtr)[siNewCig] = '=';
                     (*valArySI)[siNewCig] = 1;
                  } /*Else If: neither base is an N*/

                  else
                     ++( (*valArySI)[siNewCig] );
               } /*Else: Is an non-aonymous match*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun-01 Sec-03 Sub-04 Cat-05:
               +   - move to next base
               \++++++++++++++++++++++++++++++++++++++++*/

               --siNumNtCig;
               ++refPosUI;
               ++seqPosUI;
            } /*Loop: break up by anonymous bases*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun-01 Sec-03 Sub-04 Cat-06:
            +   - move to next cigar entry and restart
            \+++++++++++++++++++++++++++++++++++++++++++*/

            break;
         /*Case: Matches and SNPs*/

         /***********************************************\
         * Fun-01 Sec-03 Sub-05:
         *   - handle insertion and sofmask cases
         \***********************************************/

         case 'S':
         case 'I':
         /*Case: insertion or softmasking*/
            siNewCig += ((*strBuffPtr)[0] != 0);

            (*strBuffPtr)[siNewCig] =
               samSTPtr->cigTypeStr[siCig];

            (*valArySI)[siNewCig] =
               samSTPtr->cigValAryI[siCig];

            seqPosUI += samSTPtr->cigValAryI[siCig];
            break;
         /*Case: insertion or softmasking*/

         /***********************************************\
         * Fun-01 Sec-03 Sub-06:
         *   - handle deletion cases
         \***********************************************/

         case 'D':
         /*Case: deletion*/
            siNewCig += ((*strBuffPtr)[0] != 0);

            (*strBuffPtr)[siNewCig] = 'D';

            (*valArySI)[siNewCig] =
               samSTPtr->cigValAryI[siCig];

            refPosUI += samSTPtr->cigValAryI[siCig];
            break;
         /*Case: deletion*/

         /***********************************************\
         * Fun-01 Sec-03 Sub-07:
         *   - handle any other cases (hard mask likely)
         \***********************************************/

         default:
         /*Case: hard mask of some kind*/
            siNewCig += ((*strBuffPtr)[0] != 0);

            (*strBuffPtr)[siNewCig] =
               samSTPtr->cigTypeStr[siCig];

            (*valArySI)[siNewCig] =
               samSTPtr->cigValAryI[siCig];

            break;
         /*Case: hard mask of some kind*/
      } /*Switch: Check the cigar entry type*/
   } /*Loop: Check all cigar entries*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-04:
   ^   - copy new cigar entry to sam entry
   ^   o fun-01 sec-04 sub-01:
   ^     - if needed allocate more memory for new cigar
   ^   o fun-01 sec-04 sub-02:
   ^     - copy the new cigar entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-01 Sec-04 Sub-01:
   *   - if needed allocate more memory for new cigar
   \*****************************************************/

   ++siNewCig; /*Convert index 0 to index 1*/

   if(samSTPtr->lenCigBuffUI < (siNewCig + 1))
   { /*If: I need to resize the cigar arrays*/
      samSTPtr->lenCigBuffUI = (uint) siNewCig << 1;

      tmpStr =
         (schar *)
         malloc(samSTPtr->lenCigBuffUI * sizeof(char));

      if(! tmpStr)
         goto memErr_fun01_sec02_sub02;

      free(samSTPtr->cigTypeStr);
      samSTPtr->cigTypeStr = 0;
      samSTPtr->cigTypeStr = (char *) tmpStr;

      tmpStr =
         (schar *)
         malloc(samSTPtr->lenCigBuffUI * sizeof(int));

      if(! tmpStr)
         goto memErr_fun01_sec02_sub02;

      free(samSTPtr->cigValAryI);
      samSTPtr->cigValAryI = 0;
      samSTPtr->cigValAryI = (int *) tmpStr;
   } /*If: I need to resize the cigar arrays*/

   /*****************************************************\
   * Fun-01 Sec-04 Sub-02:
   *   - copy the new cigar entry
   \*****************************************************/

   samSTPtr->lenCigUI = (uint) siNewCig;

   for(siCig = 0; siCig < siNewCig; ++siCig)
   { /*Loop: Copy the new cigar over*/
      samSTPtr->cigTypeStr[siCig] = (*strBuffPtr)[siCig];
      samSTPtr->cigValAryI[siCig] = (*valArySI)[siCig];
   } /*Loop: Copy the new cigar over*/

   samSTPtr->cigTypeStr[siCig] = '\0';
   samSTPtr->cigValAryI[siCig] = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-05:
   ^   - clean up
   ^   o fun-01 sec-05 sub-01:
   ^     - clean up after success
   ^   o fun-01 sec-05 sub-02:
   ^     - error clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-01 Sec-05 Sub-01:
   *   - clean up after success
   \*****************************************************/

   finishAndExit_fun01_sec02_sub01:;

   return 0;

   /*****************************************************\
   * Fun-01 Sec-05 Sub-02:
   *   - error clean up
   \*****************************************************/

   memErr_fun01_sec02_sub02:;

   return 1;
} /*samCigToEqxCig*/


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
