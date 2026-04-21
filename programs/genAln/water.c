/*########################################################
# Name water
# Use:
#  o Holds functions to do a Smith Waterman pairwise
#    alignment
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' water SOF: Start Of Functions
'   - has function to do Smith Waterman pairwise alignment
'   o header:
'     - included libraries
'   o fun01 water:
'     - run a memory efficent Waterman Smith alignment on
'       input sequences
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "water.h"

#include "../genBio/seqST.h"

#include "alnSet.h"
#include "dirMatrix.h"
#include "indexToCoord.h"

/*.h files only*/
#include "../genLib/genMath.h" /*only using .h commands*/
#include "alnDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden files
!   o std #include <stdio.h>
!   o .c  #include "../genLib/base10str.h"
!   o .c  #include "../genLib/ulCp.h"
!   o .c  #include "../genLib/charCp.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/fileFun.h"
!   o .c  #include "../genBio/samEntry.h"
!   o .h  #include "../genLib/endStr.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: water
|   - run a Waterman Smith alignment on input sequences
| Input:
|   - qrySTPtr:
|     o pointer to seqST with query sequence 
|       - qrySTPtr->offsetSL; first query base to align
|       - qrySTPtr->endAlnSL; last query base to align
|   - refSTPtr:
|     o pointer to seqST with reference sequence 
|       - refSTPtr->offsetSL; 1st reference base to align
|       - refSTPtr->endAlnSL; last reference base to align
|   - matrixSTPtr:
|     o pointer to dirMatrix to use for the alingment
|   - alnSet:
|     o pointer to alnSet with alignment settings
| Output:
|  - Modifies:
|    o errSC in matrixSTPtr to def_memErr_water for
|      memory errors
|    o allocates memory for dirMatrixSC and scoreArySL
|      if they are to small
|    o updates lenMatrixSL and lenScoreSL if dirMatrixSC
|      or scoreArySL are resized
|    o sets errSC in matrixSTPtr to def_memErr_water if
|      had memory errors
|  - Returns:
|    o score for alignment
\-------------------------------------------------------*/
signed long
water(
   struct seqST *qrySTPtr, /*query sequence and data*/
   struct seqST *refSTPtr, /*ref sequence and data*/
   struct dirMatrix *matrixSTPtr, /*direction matrix*/
   struct alnSet *settings     /*settings for alignment*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC: WatermanAln
   '  - Run a Waterman Smith alignment on input sequences
   '  o fun01 sec01:
   '    - variable declerations
   '  o fun01 sec02:
   '    - allocate memory for alignment
   '  o fun01 sec03:
   '    - fill in initial negatives for ref
   '  o fun01 sec04:
   '    - fill the matrix with scores
   '  o fun01 sec05:
   '    - set up for returing matrix (clean up/wrap up)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01: Variable declerations
   ^  o fun01 sec01 sub01:
   ^    - variables dealing with the query and reference
   ^      starting positions
   ^  o fun01 sec01 sub02:
   ^    - variables holding the scores (only two rows)
   ^  o fun01 sec01 sub03:
   ^    - directinol matrix variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec01 Sub01:
   *  - variables dealing with the query and reference
   *    starting positions
   \*****************************************************/

   /*Get start & end of query and reference sequences*/
   signed char *refSeqStr =
      refSTPtr->seqStr + refSTPtr->offsetSL;

   signed char *qrySeqStr =
      qrySTPtr->seqStr + qrySTPtr->offsetSL;

   /*Find the length of the reference and query*/
   signed long qryLenSL =
      qrySTPtr->endAlnSL - qrySTPtr->offsetSL + 1;

   signed long refLenSL =
      refSTPtr->endAlnSL - refSTPtr->offsetSL + 1;
     /*The + 1 is to account for index 0 of endAlnSL*/

   signed long lenMatrixSL =
     (refLenSL + 1) * (qryLenSL + 1);
     /*+1 for the gap column and row*/

   signed long slRef = 0;
   signed long slQry = 0;

   /*Set up counters for the query and reference base
   `  index
   */
   /*****************************************************\
   * Fun01 Sec01 Sub02:
   *  - variables holding the scores (only two rows)
   \*****************************************************/

   signed long snpScoreSL = 0;    /*Score for deletion*/
   signed long nextSnpScoreSL = 0;/*Score for match/snp*/

   signed long insScoreSL = 0;    /*Score for deletion*/
   signed long delScoreSL = 0;    /*Score for deletion*/

   /*Marks when to reset score buffer (every second row)*/
   signed long *scoreArySL = 0;
      /*scoring row for alignment*/

   /*****************************************************\
   * Fun01 Sec01 Sub03:
   *  - directinol matrix variables
   \*****************************************************/

   /*Direction matrix (one cell holds one direction)*/
   signed char *dirMatrixSC = 0;/*Direction matrix*/
   signed char *insDir = 0;    /*Direction above cell*/
   signed long indexSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - allocate memory for alignment
   ^   o fun01 sec02 sub01:
   ^     - set up the directional matrix
   ^   o fun01 sec02 sub02:
   ^     - set up score array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec02 Sub01:
   *   - set up the directional matrix
   \*****************************************************/

   if(matrixSTPtr->lenMatrixSL < lenMatrixSL)
   { /*If: need to resize the matrix*/
      free(matrixSTPtr->dirMatrixSC);
      matrixSTPtr->dirMatrixSC = 0;
      matrixSTPtr->lenMatrixSL = 0;

      matrixSTPtr->dirMatrixSC =
         malloc((lenMatrixSL + 1) * sizeof(char));

      if(matrixSTPtr->dirMatrixSC == 0)
         goto memErr_fun01_sec05;

       matrixSTPtr->lenMatrixSL = lenMatrixSL;
   } /*If: need to resize the matrix*/

   dirMatrixSC = matrixSTPtr->dirMatrixSC;

   blank_dirMatrix(matrixSTPtr);

   matrixSTPtr->refLenSL = refLenSL;
   matrixSTPtr->refOffsetSL = refSTPtr->offsetSL;
   matrixSTPtr->refEndSL = refSTPtr->endAlnSL;

   matrixSTPtr->qryLenSL = qryLenSL;
   matrixSTPtr->qryOffsetSL = qrySTPtr->offsetSL;
   matrixSTPtr->qryEndSL = qrySTPtr->endAlnSL;

   /*****************************************************\
   * Fun01 Sec02 Sub02:
   *   - set up score array
   \*****************************************************/

   if(matrixSTPtr->lenScoreSL < refLenSL + 1)
   { /*If: need to make a larger score array*/
      free(matrixSTPtr->scoreArySL);
      matrixSTPtr->scoreArySL = 0;
      matrixSTPtr->lenScoreSL = 0;
      
      matrixSTPtr->scoreArySL =
         calloc((refLenSL + 1), sizeof(long));

      if(! matrixSTPtr->scoreArySL)
         goto memErr_fun01_sec05;

      matrixSTPtr->lenScoreSL = refLenSL;
   } /*If: need to make a larger score array*/

   scoreArySL = matrixSTPtr->scoreArySL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^  - fill in initial negatives for reference
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(
      indexSL = 0;
      indexSL <= refLenSL;
      ++indexSL
   ){ /*Loop: initialize the first row*/
      dirMatrixSC[indexSL] = def_mvStop_alnDefs;
      scoreArySL[indexSL] = 0;
   } /*Loop: initialize the first row*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^  - fill the matrix with scores
   ^  o fun01 sec04 sub01:
   ^    - get initial scores
   ^  o fun01 sec04 sub02:
   ^    - start loops
   ^  o fun01 sec04 sub03:
   ^    - find bests score for each base pair
   ^  o fun01 sec04 sub04:
   ^    - find high score
   ^  o fun01 sec04 sub05:
   ^    - check if keep score (score > 0)
   ^  o fun01 sec04 sub06:
   ^    - find deletion score and move to next index
   ^  o fun01 sec04 sub07:
   ^    - set up for scoring next row
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec04 Sub01:
   *  - get initial scores
   \*****************************************************/

   /*set up pointers*/
   insDir = dirMatrixSC;

   /*set up scores*/
   delScoreSL = 0;
   nextSnpScoreSL = 0;
   dirMatrixSC[indexSL] = def_mvStop_alnDefs;

   /*move to first base*/
   ++indexSL;
   refSeqStr = refSTPtr->seqStr + refSTPtr->offsetSL - 1;
   qrySeqStr = qrySTPtr->seqStr + qrySTPtr->offsetSL;

   /*****************************************************\
   * Fun01 Sec04 Sub02:
   *   - start loops
   \*****************************************************/

   /*Starting on the first sequence row*/
   for(
      slQry = 0;
      slQry < qryLenSL;
      ++slQry
   ){ /*loop; compare query base against all ref bases*/

      for(
         slRef = 1;
         slRef <= refLenSL;
         ++slRef
      ){ /*Loop; compare one query to one reference base*/

         /***********************************************\
         * Fun01 Sec04 Sub03:
         *   - find bests score for each base pair
         \***********************************************/

         snpScoreSL =
            getScore_alnSet(
               qrySeqStr[slQry],
               refSeqStr[slRef],
               settings
            ); /*Find the score for the base pairs*/

         snpScoreSL += nextSnpScoreSL;
         nextSnpScoreSL = scoreArySL[slRef];

         #ifdef NOEXTEND
            insScoreSL =
               scoreArySL[slRef] + settings->gapSS;
         #else
            insScoreSL = scoreArySL[slRef];
            insScoreSL +=
               settings->insArySS[insDir[slRef]];
         #endif

         /***********************************************\
         * Fun01 Sec04 Sub04:
         *   - find high score
         \***********************************************/

         scoreArySL[slRef] =
            max_genMath(
               insScoreSL,
               snpScoreSL
         ); /*find if ins/snp is best (5 Op)*/
 
         /*find direction (5 Op)*/
         dirMatrixSC[indexSL] =
            scoreArySL[slRef] > delScoreSL;
         dirMatrixSC[indexSL] +=
            (
                 (snpScoreSL <= insScoreSL)
               & dirMatrixSC[indexSL]
            );
         ++dirMatrixSC[indexSL];

         /*Logic:
         `   - noDel: maxSC > delSc:
         `     o 1 if deletion not max score
         `     o 0 if deletion is max score
         `   - type: noDel + ((snpSc < insSc) & noDel):
         `     o 1 + (1 & 1) = 2 if insertion is maximum
         `     o 1 + (0 & 1) = 1 if snp is maximum
         `     o 0 + (0 & 0) = 0 if del is max; snp > ins
         `     o 0 + (1 & 0) = 0 if del is max, ins >= snp
         `   - dir: type + 1
         `     o adds 1 to change from stop to direction
         */

         /*finish max*/
         scoreArySL[slRef] =
            max_genMath(
               delScoreSL,
               scoreArySL[slRef]
         ); /*find if del is best (5 Op)*/

         /***********************************************\
         * Fun01 Sec04 Sub05:
         *   - check if keep score (score > 0)
         \***********************************************/

         if(scoreArySL[slRef] <= 0)
         {
            dirMatrixSC[indexSL] = 0;
            scoreArySL[slRef] = 0;
         }

         else if(matrixSTPtr->scoreSL < scoreArySL[slRef])
         { /*Else If: have a new best score*/
            matrixSTPtr->scoreSL = scoreArySL[slRef];
            matrixSTPtr->indexSL = indexSL;
         } /*Else If: have a new best score*/


         /*check if have negative or positive score*/
         /*no branched version is slower here
         keepSL = (signed long) -(scoreArySL[slRef] > 0);
         dirMatrixSC[indexSL] &= keepSL;
         scoreArySL[slRef] &= keepSL;
         */

         /***********************************************\
         * Fun01 Sec04 Sub06:
         *   - find deletion score and move to next index
         \***********************************************/

         #ifdef NOEXTEND
            delScoreSL =
               scoreArySL[slRef] + settings->gapSS;
         #else
            delScoreSL = scoreArySL[slRef];
            delScoreSL +=
               settings->delArySS[dirMatrixSC[indexSL]];
         #endif

         ++indexSL;
      } /*Loop; compare one query to one reference base*/

      /**************************************************\
      *  Fun01 Sec04 Sub07:
      *    - set up for scoring next row
      \**************************************************/

      delScoreSL = 0;
      nextSnpScoreSL = 0; /*indel column is always 0*/

      insDir += slRef;
      dirMatrixSC[indexSL] = def_mvStop_alnDefs;

      ++indexSL;
   } /*loop; compare query base against all ref bases*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec05:
   ^  - set up for returing the matrix (clean up/wrap up)
   ^  o fun01 sec05 sub01:
   ^    - no error clean up and return
   ^  o fun01 sec05 sub02:
   ^    - error clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec05 Sub01:
   *   - no error clean up and return
   \*****************************************************/

   /*Move back to the lower right conor cell
   ` This is not needed, but is nice.
   */

   matrixSTPtr->errSC = 0;
   --indexSL; /*get off last -1*/
   dirMatrixSC[indexSL] = def_mvStop_alnDefs;
   return matrixSTPtr->scoreSL; /*best score*/

   /*****************************************************\
   * Fun01 Sec05 Sub02:
   *   - error clean up and return
   \*****************************************************/

   memErr_fun01_sec05:;
      matrixSTPtr->errSC = def_memErr_water;
      goto errCleanUp_fun01_sec05;

   errCleanUp_fun01_sec05:;
     return 0;
} /*water*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
