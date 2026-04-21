/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' drugAry SOF: Start Of File
'   - holds fuctions to maintain an array of 32 length
'     c-strings. This is in the from of a matrix so you
'     can free using free(drug_cstring_array);
'   o header:
'     - Included libraries
'   o fun01: alloc_drugAry
'     - makes drug array (c-string) on heap
'   o fun02: realloc_drugAry
'     - returns pointer to rellocated memory for a drugAry
'   o .h fun03: get_drugAry
'     - gets pointer to a drug c-string in a drugAry
'   o fun04: cpDrug_drugAry
'     - copies an antibiotic (as lower case) to drug array
'   o fun05: find_drugAry()
'     - finds antibiotic in a drugAray c-string
'   o fun06: swap_drugAry
'     - swaps two drug names
'   o fun07: cpDrugCase_drugAry
'     - copies an antibiotic to a drug array and keeps the
'       input case (fun04 converts to lower case)
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "drugAry.h"

/*-------------------------------------------------------\
| Fun01: alloc_drugAry
|   - makes drug array (c-string) on heap
| Input:
|   - numStringsUI:
|     o number of strings to make
| Output:
|   - Returns:
|     o an pointer to the new arrray
|     o 0 for memory error
\-------------------------------------------------------*/
signed char *
alloc_drugAry(
   unsigned int numStringsUI
){
   return
      calloc(
         numStringsUI * def_strLen_drugAry,
         sizeof(char)
      );
} /*alloc_drugAry*/

/*-------------------------------------------------------\
| Fun02: realloc_drugAry
|   - returns pointer to rellocated memory for a drugAry
| Input:
|   - drugAryStr:
|     o pionter to drugAryStr to reallocate memory for
|   - numStringsUI:
|     o number of strings to make
| Output:
|   - Frees:
|     o drugArySTr if memory was succesfully allocated
|       (realloc does this for me)
|   - Returns:
|     o 0 for memory error (old memory freeded)
|     o pointer to new memory (old memory freeded)
\-------------------------------------------------------*/
signed char *
realloc_drugAry(
   signed char *drugAryStr,
   unsigned int numStrUI
){
   signed char * tmpStr = 0;

   tmpStr =
      realloc(
         drugAryStr,
           numStrUI
         * def_strLen_drugAry
         * sizeof(signed char)
      );
   
   return tmpStr;
} /*realloc_drugAry*/

/*-------------------------------------------------------\
| Fun04: cpDrug_drugAry
|   - copies an antibiotic (as lower case) to drug array
| Input:
|   - drugAryStr:
|     o drug array to copy the antibiotic name to
|   - drugStr:
|     o drug to copy
|   - indexUI:
|     o index to copy drug to
|   - delimSC:
|     o deliminator to stop copy at
|     o will not copy past '\0'
|     o use -1 to stop at any white space (< 33)
| Output:
|   - Modifies:
|     o durgAryStr to hold the new drug name
|   - Returns:
|     o length of the copied string
\-------------------------------------------------------*/
signed int
cpDrug_drugAry(
   signed char *drugAryStr,
   signed char *drugStr,
   unsigned int indexUI,
   signed char delimSC
){
   signed int siChar = 0;
   signed char *dupStr = 0;

   dupStr = get_drugAry(drugAryStr, indexUI);

   for(
      siChar=0;
      siChar < def_strLen_drugAry;
      ++siChar
   ){ /*Loop: copy drug name*/
      if(delimSC < 0 && drugStr[siChar] < 33)
         break;
      else if(drugStr[siChar] == delimSC)
         break;
      else if(! drugStr[siChar])
         break;

      dupStr[siChar] = drugStr[siChar] | 32;
   } /*Loop: copy drug name*/

   dupStr[siChar] = '\0';
   return siChar;
} /*cpDrug_drugAry*/

/*-------------------------------------------------------\
| Fun05: find_drugAry()
|   - finds antibiotic in a drugAray c-string
| Input:
|   - qryStr:
|     o drug to search for in drugAryStr
|   - drugAryStr:
|     o drug array (c-string) to search
|   - numDrugsSI:
|     o number of antibiotics in drugAryStr
| Output:
|   - Returns:
|     o index of the drug if qryStr is in drugAryStr
|     o -1 if the drug is not in drugAry
| Note:
|    - do to the way this is procesed, I can not use a
|      fast search method like a binary search, since
|      (there is no garuntee of order).
\-------------------------------------------------------*/
signed int
find_drugAry(
   signed char *qryStr,     /*drug to find*/
   signed char *drugAryStr, /*array to search*/
   signed int numDrugsSI,   /*number drugs in array*/
   signed char delimSC      /*deliminator to stop at*/
){ /*findDrug_in_durgAryStr*/
   signed int siIndex = 0;
   signed char *drugOnStr = 0;
   signed char *targStr = qryStr;

   for(siIndex=0; siIndex < numDrugsSI; ++siIndex)
   { /*Loop: find qryStr in drugAryStr*/
      drugOnStr = get_drugAry(drugAryStr, siIndex);
            /*get pointer to drug*/

      while( (*targStr | 32) == (*drugOnStr | 32) )
      { /*Loop: see if have match*/
         if(*drugOnStr == '\0')
            break;
         else if(*targStr == delimSC)
            break;
         ++targStr;
         ++drugOnStr;
      } /*Loop: see if have match*/

      if(*drugOnStr != '\0')
        ;
      else if(*targStr == delimSC)
         break; /*found target*/
      targStr = qryStr; /*reset to start of query*/
   } /*Loop: find qryStr in drugAryStr*/

   return
      siIndex | ((signed int) -(siIndex >= (numDrugsSI)));
      /*Logic:
      `   - siIndex >= numDrugsSI
      `     o 1 when I did not find the query
      `     o 0 when I found the query
      `   - -(siIndex >= numDrugsSI)
      `     o Changes 1 to -1 (all bits set) and 0 to 0
      `       (no bits set)
      `   - siIndex | -(siIndex >= numDrugsSI)
      `     o Converts siIndex to -1 when the query was
      `       not found [-(siIndex >= numDragsI) is -1]
      `     o Does nothing to siIndex when the query was
      `       found [-(siIndex >= numDragsI) is 0]
      */
} /*find_drugAry*/

/*-------------------------------------------------------\
| Fun06: swap_drugAry
|   - swaps two drug names
| Input:
|   - drugAryStr:
|     o drug array to with drugs to swap
|   - firstUI:
|     o first drug to swap
|   - secUI:
|     o second drug to swap
| Output:
|   - Modifies:
|     o dubAryStr to have drugs swapped
\-------------------------------------------------------*/
void
swap_drugAry(
   signed char *drugAryStr,
   unsigned int firstUI,
   unsigned int secUI
){
   signed char *firstStr = 0;
   signed char *secStr = 0;

   firstStr = get_drugAry(drugAryStr, firstUI);
   secStr = get_drugAry(drugAryStr, secUI);

   while(*firstStr != '\0')
   { /*Loop: copy strings*/
      *firstStr ^= *secStr;
      *secStr ^= *firstStr;
      *firstStr++ ^= *secStr++;

      if(*secStr == '\0')
      { /*If: at end*/
         while(*firstStr != '\0')
         { /*Loop: finish coping first id*/
            *firstStr ^= *secStr;
            *secStr ^= *firstStr;
            *firstStr++ ^= *secStr++;
         } /*Loop: finish coping first id*/

         *secStr = '\0';   /*let next loop know at null*/
      } /*If: at end*/
   } /*Loop: copy strings*/

   while(*secStr != '\0')
   { /*Loop: finishing coping second id*/
      *firstStr ^= *secStr;
      *secStr ^= *firstStr;
      *firstStr++ ^= *secStr++;
   } /*Loop: finishing coping second id*/

   *secStr = '\0';
   *firstStr = '\0';
} /*swap_drugAry*/

/*-------------------------------------------------------\
| Fun07: cpDrugCase_drugAry
|   - copies an antibiotic to a drug array and keeps the
|     input case
| Input:
|   - drugAryStr:
|     o drug array to copy the antibiotic name to
|   - drugStr:
|     o drug to copy
|   - indexUI:
|     o index to copy drug to
|   - delimSC:
|     o deliminator to stop copy at
|     o will not copy past '\0'
| Output:
|   - Modifies:
|     o dubAryStr to hold the new drug name
|   - Returns:
|     o length of the copied string
\-------------------------------------------------------*/
signed int
cpDrugCase_drugAry(
   signed char *drugAryStr,
   signed char *drugStr,
   unsigned int indexUI,
   signed char delimSC
){
   signed int siChar = 0;
   signed char *dupStr = 0;

   dupStr = get_drugAry(drugAryStr, indexUI);

   for(
      siChar=0;
      siChar < def_strLen_drugAry;
      ++siChar
   ){ /*Loop: copy drug name*/
      if(drugStr[siChar] == delimSC)
         break;
      else if(drugStr[siChar] == '\0')
         break;
      dupStr[siChar] = drugStr[siChar];
   } /*Loop: copy drug name*/

   dupStr[siChar] = '\0';
   return siChar;
} /*cpDrugCase_drugAry*/

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
