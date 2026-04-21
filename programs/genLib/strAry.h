/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' strAry SOF:
'   - functions to maintain and organize an array of
'     strings (max length is 63 characters)
'   o header:
'     - defined variables and guards
'   o fun01: mk_strAry
'     - make a string array
'   o fun02: realloc_strAry
'     - rellocates memory for a string array
'   o fun04: add_strAry
'     - adds a string to a string array
'   o .h fun03: get_strAry
'     - finds pointer of a string in a string array
'   o fun04: add_strAry
'     - adds a string to a string array
'   o fun05: swap_strAry
'     - swaps two strings in a string array
'   o fun06: cmp_strAry
'     - compares a string to a string array
'   o fun07: cmpIndex_strAry
'     - compares two strings in a string array
'   o fun08: findClose_strAry
'     - finds location to insert query in string array
'       (must be sorted)
'   o fun09: addSort_strAry
'     - adds a string to string array in a sorted positon
'   o fun10: sort_strAry
'     - sorts a string array from least to greatest; is
'       case sensitive
'   o fun11: sortSync_strAry
'     - sorts a string array from least to greatest, but
'       keeps the unsigned int array in sync with strings
'   o fun12: sortSyncUS_strAry
'     - sorts a string array from least to greatest, but
'       keeps unsigned short array in sync with strings
'   o fun13: find_strAry
'     - search for query in string array (must be sorted)
'   o fun14: findNoSort_strAry
'     - search for query in string array (dumb search)
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H

#define def_lenStr_strAry 64 /*max length of a string*/

/*-------------------------------------------------------\
| Fun01: mk_strAry
|   - make a string array
| Input:
|   - sizeUL:
|     o number of strings to store
| Output:
|   - Returns:
|     o pointer to string array
|     o 0 for memory error
\-------------------------------------------------------*/
signed char *
mk_strAry(
   unsigned long sizeUL
);

/*-------------------------------------------------------\
| Fun02: realloc_strAry
|   - rellocates memory for a string array
| Input:
|   - strAry:
|     o string array to reallocate memory for
|   - sizeUL:
|     o new number of strings to store
| Output:
|   - Returns:
|     o pointer to string array (if succeded)
|       - realloc frees only on success
|     o 0 for memory error
\-------------------------------------------------------*/
signed char *
realloc_strAry(
   signed char *strAry,
   unsigned long sizeUL
);

/*-------------------------------------------------------\
| Fun03: get_strAry
|   - finds pointer of a string in a string array
| Input:
|   - strAry:
|     o string array to get string index from
|   - indexUL:
|     o index of target string
| Output:
|   - Returns:
|     o pointer to string at indexUL
\-------------------------------------------------------*/
#define get_strAry(strAry, indexUL) ((strAry) + ((indexUL) * def_lenStr_strAry))

/*-------------------------------------------------------\
| Fun04: add_strAry
|   - adds a string to a string array
| Input:
|   - newStr:
|     o string to add to array; must be 63 char or shorter
|   - strAry:
|     o string array to add string to
|   - indexUL:
|     o index to add string at
| Output:
|   - Modifies:
|     o strAry to have newStr at indexUL
\-------------------------------------------------------*/
void
add_strAry(
   signed char *newStr,
   signed char *strAry,
   unsigned long indexUL
);

/*-------------------------------------------------------\
| Fun05: swap_strAry
|   - swaps two strings in a string array
| Input:
|   - strAry:
|     o string array to sort
|   - firstUL:
|     o first index to swap
|   - secUL:
|     o second index to swap
| Output:
|   - Modifies:
|     o strAry to have strings at firstUL and secUL swaped
\-------------------------------------------------------*/
void
swap_strAry(
   signed char *strAry,
   unsigned long firstUL,
   unsigned long secUL
);

/*-------------------------------------------------------\
| Fun06: cmp_strAry
|   - compares a string to a string array
| Input:
|   - qryStr:
|     o c-string with query to compare
|   - strAry:
|     o c-string array with strings to compare
|   - refUL:
|     o index of reference to compare
| Output:
|   - Returns:
|     o 0 if strings are equal
|     o > 0 if query is greater
|     o < 0 if reference is greater
\-------------------------------------------------------*/
signed long
cmp_strAry(
   signed char *qryStr,
   signed char *strAry,
   unsigned long refUL
);

/*-------------------------------------------------------\
| Fun07: cmpIndex_strAry
|   - compares two strings in a string array
| Input:
|   - strAry:
|     o string array with strings to compare
|   - qryUL:
|     o index of query to compare
|   - refUL:
|     o index of reference to compare
| Output:
|   - Returns:
|     o 0 if strings are equal
|     o > 0 if query is greater
|     o < 0 if reference is greater
\-------------------------------------------------------*/
signed long
cmpIndex_strAry(
   signed char *strAry,
   unsigned long qryUL,
   unsigned long refUL
);

/*-------------------------------------------------------\
| Fun08: findClose_strAry
|  - finds location to insert query in string array
|    (must be sorted)
| Input:
|  - strAry:
|    o string array
|  - qryStr:
|    o string to find insert location
|  - lenUL:
|    o length of strAry (index 1)
| Output:
|  - Returns:
|    o index to insert qryStr at
\-------------------------------------------------------*/
signed long
findClose_strAry(
   signed char *strAry,
   signed char *qryStr,
   signed long lenSL
);

/*-------------------------------------------------------\
| Fun09: addSort_strAry
|   - adds a string to a string array in a sorted positon
|   - assumes you do not manipulate array in other ways
| Input:
|   - newStr:
|     o string to add to array; must be 63 char or shorter
|   - strAry:
|     o string array to add string to
|   - lenUL:
|     o has number of elements in array
| Output:
|   - Modifies:
|     o strAry to have newStr at closest index
|   - Returns:
|     o index of added string
\-------------------------------------------------------*/
unsigned long
addSort_strAry(
   signed char *newStr, /*string to add to array*/
   signed char *strAry, /*string array to add newStr to*/
   unsigned long lenUL  /*number of elements in array*/
);

/*-------------------------------------------------------\
| Fun10: sort_strAry
|   - sorts a string array from least to greatest; is case
|     sensitive
| Input:
|   - strAry:
|     o string array to sort
|   - lenUL:
|     o length of strAry (index 1)
| Output:
|   - Modifies:
|     o strAry to be sorted
\-------------------------------------------------------*/
void
sort_strAry(
   signed char *strAry,
   unsigned long lenUL
);

/*-------------------------------------------------------\
| Fun11: sortSync_strAry
|   - sorts a string array from least to greatest, but
|     keeps the unsigned int array in sync with strings
| Input:
|   - strAry:
|     o string array to sort
|   - uiAry:
|     o unsigned int array to keep in sync with strAry
|   - lenUL:
|     o length of strAry (index 1)
| Output:
|   - Modifies:
|     o strAry to be sorted
\-------------------------------------------------------*/
void
sortSync_strAry(
   signed char *strAry,
   unsigned int *uiAry,
   unsigned long lenUL
);

/*-------------------------------------------------------\
| Fun12: sortSyncUS_strAry
|   - sorts a string array from least to greatest, but
|     keeps the unsigned short array in sync with strings
| Input:
|   - strAry:
|     o string array to sort
|   - uiAry:
|     o unsigned int array to keep in sync with strAry
|   - lenUL:
|     o length of strAry (index 1)
| Output:
|   - Modifies:
|     o strAry to be sorted
\-------------------------------------------------------*/
void
sortSyncUS_strAry(
   signed char *strAry,
   unsigned short *usAry,
   unsigned long lenUL
);

/*-------------------------------------------------------\
| Fun13: find_strAry
|  - search for query in string array (must be sorted)
| Input:
|  - strAry:
|    o string array
|  - qryStr:
|    o string to find
|  - lenUL:
|    o length of strAry (index 1)
| Output:
|  - Returns:
|    o index of qryStr in strAry
|    o -1 if qryStr is not in strAry
\-------------------------------------------------------*/
signed long
find_strAry(
   signed char *strAry,
   signed char *qryStr,
   signed long lenSL
);

/*-------------------------------------------------------\
| Fun14: findNoSort_strAry
|  - search for query in string array (dumb search)
| Input:
|  - strAry:
|    o string array
|  - qryStr:
|    o string to find
|  - lenUL:
|    o length of strAry (index 1)
| Output:
|  - Returns:
|    o index of qryStr in strAry
|    o -1 if qryStr is not in strAry
\-------------------------------------------------------*/
signed long
findNoSort_strAry(
   signed char *strAry,
   signed char *qryStr,
   signed long lenSL
);

#endif

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
