/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainFreezeTB SOF: Start Of File
'   - driver function for freezeTB
'   o header:
'     o included libraries
'   o main:
'     - driver function for freezeTB
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef plan9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "freezeTB.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   o std #include <string.h>
!   o std #include <stdio.h>
!
!   > task specific libraries
!
!   o .c  #include "../genBio/trimSam.h"
!   o .c  #include "../genBio/maskPrim.h"
!   o .c  #include "../genBio/ampDepth.h"
!   o .c  #include "../genBio/adjCoords.h"
!   o .c  #include "../tbSpolSrc/spolFind.h" 
!   o .c  #include "../tbMiruSrc/miruTbl.h"
!   o .c  #include "../tbAmrSrc/checkAmr.h"
!   o .c  #include "../genBio/tbCon.h"
!   o .c  #include "../genClust/edClust.h"
!
!   > general library dependencies for tasks
!
!   o .c  #include "../genLib/base10str.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/ulCp.h"
!   o .c  #include "../genLib/charCp.h"
!   o .c  #include "../genLib/genMath.h"
!   o .c  #include "../genLib/shellSort.h"
!   o .c  #include "../genLib/strAry.h"
!
!   > general biology dependencys for task
!
!   o .c  #include "../genBio/geneCoord.h"
!   o .c  #include "../genBio/codonTbl.h"
!   o .c  #include "../genBio/seqST.h"
!   o .c  #include "../genBio/samEntry.h"
!   o .c  #include "../genBio/edDist.h"
!
!   > alignemnt library dependencies for tasks
!
!   o .c  #include "../genAln/indexToCoord.h"
!   o .c  #include "../genAln/alnSet.h"
!   o .c  #include "../genAln/memwater.h"
!   o .c  #include "../genAln/kmerFind.h"
!
!   > other library dependencys needed for a spefic task
!
!   o .c  #include "../tbSpolSrc/spolST.h"
!   o .c  #include "../tbAmrSrc/amrST.h"
!   o .c  #include "../tbAmrSrc/drugAry.h"
!   o .c  #include "../genClust/clustST.h"
!   o .c  #include "freezeTBPaths.h" (file paths)
!
!   > defualts and other .h only libraries
!   o .h  #include "../genLib/dataTypeShortHand.h"
!   o .h  #include "../genBio/tbConDefs.h"
!   o .h  #include "freezeTBDefs.h"
!   o .h  #include "../tbAmrSrc/tbAmrDefs.h"
!   o .h  #include "../tbMiruSrc/tbMiruDefs.h"
!   o .h  #include "../tbSpolSrc/tbSpolDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Main:
|   - driver function for freezeTB
|   - mainly here so can wrap command for tcltk
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   const char *argAryStr[]
){
   return
      run_freezeTB(
         numArgsSI,
         argAryStr
      ); /*really runs freezeTB program*/
} /*main*/
