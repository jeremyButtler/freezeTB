/*########################################################
# Name: getTbMuts
#   - Prints out the mutations for each position in a
#     tb genome.
# Note:
#   - This is here to help me debug tbAmr
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   - header:
'     o Has included libraries
'   - main:
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - has included libraries
\-------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>

#include "../generalLib/samEntryStruct.h"
#include "../generalLib/ulCpStr.h"
#include "../generalLib/dataTypeShortHand.h"

/*Hiden libraries:
`  - ../generalLib/base10ToNumStr.h
`  - ../generalLib/numToStr.h
*/

/*-------------------------------------------------------\
| Main:
|   - 
\-------------------------------------------------------*/
#ifdef PLAN9
   void
   main(
#else
   int
   main(
#endif
   char *argsStrAry[],
   int *numArgsI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main:
   '   o main sec-01:
   '     - variable declerations
   '   o main sec-02:
   '     - Get input sam file
   '   o main sec-03:
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   char *samStr = 0;

   int snpHitsIAry[5000000];
   int delHitsIAry[5000000];
   int insHitsIAry[5000000];

   FILE *samFILE = 0;
} /*main*/
