/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' seqStats SOF: Start Of File
'   - get stats for fx (fastq/fasta) files
'   o header:
'     - included libraries
'   o .c fun01: ln_seqStats
'     - natureal log value without using math.h (requies
'       -lm flag)
'     - TODO: fix issues with low values
'   o .c fun02: log10_seqStats
'     - log10 value without using math.h (requies -lm
'       flag)
'   o .c fun03: pow10_seqStats
'     - rases 10 to the input exponant
'   o .c fun04: qhistToMedQ_seqStats
'     - converts a q-score histogram to medain q-score
'   o .c fun05: qhistToMeanQ_seqStats
'     - converts a q-score histogram to mean q-score
'   o fun06: meanMedQ_seqStats
'     - gets the mean and median q-score for a sequence
'   o fun07: ontMeanMedQ_seqStats
'     - gets the mean and median q-score for a sequence
'       using ONT's method
'   o fun08: pReadStats_seqStats
'     - prints stats for a single read
'   o license:
'     - licensing for this code (CC0)
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

#include <stdio.h>

#ifdef NO_MATH_H
#else
   #include <math.h>
#endif

#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_qAdj_seqStats 33 /*convert char to q-score*/
#define def_maxQ_seqStats 94 /*highest q-score possible*/

/*-------------------------------------------------------\
| Fun01: ln_seqStats
|   - natureal log value without using math.h (requies
|     -lm flag)
'   - TODO: fix issues with low values
|   - from:
|     https://stackoverflow.com/questions/64894566/how-to-write-a-simple-logarithm-function-without-math-h
| Input:
|   - valDbl:
|     o value to find log10 for
| Output:
|   - Returns:
|     o log10 of valDbl
\-------------------------------------------------------*/
double
ln_seqStats(
   double valDbl
){
   double retDbl = 0;
   double powerDbl = 0;
   signed int roundSI = 0;
   signed int adjustSI = 0;

   #define EULER_CONST_fun03 2.718281828459045235f
      /*ln(EULER_CONST) = 1*/
   #define TAYLOR_ITERATIONS_fun03 8

   if(valDbl <= 0)
      return 0.0/0.0;

   /*move this to satisify the expansion range value
   `  of -1 < x <= 1
   */
   while(valDbl > 1.0)
   { /*Loop: remove whole logs and to taylor const min*/
      valDbl /= EULER_CONST_fun03;
      ++adjustSI;
   } /*Loop: remove whole logs and to taylor const min*/

   while(valDbl < 0.25)
   { /*Loop: move to lower talyor constant range*/
      valDbl *= EULER_CONST_fun03;
      --adjustSI;
   } /*Loop: move to lower talyor constant range*/

   /*find the decimal part of the natural log*/
   valDbl *= -1;
   powerDbl = valDbl;
   roundSI = 1;

   while(roundSI < TAYLOR_ITERATIONS_fun03)
   { /*Loop: find log2*/
      retDbl += powerDbl / roundSI;
      powerDbl *= valDbl;
      ++roundSI;

      retDbl += (powerDbl * -1) / roundSI;
      powerDbl *= valDbl;
      ++roundSI;
   }  /*Loop: find log2*/

   return retDbl + adjustSI;
} /*ln_seqStats*/

/*-------------------------------------------------------\
| Fun02: log10_seqStats
|   - log10 value without using math.h (requies -lm flag)
| Input:
|   - valDbl:
|     o value to find log10 for
| Output:
|   - Returns:
|     o log10 of valDbl
\-------------------------------------------------------*/
double
log10_seqStats(
   double valDbl
){
   return ln_seqStats(valDbl) / 2.30258509299;
      /*convert ln(x) to log10(x)*/
      /*ln(10) = 2.30258509299*/
} /*log10_seqStats*/

/*-------------------------------------------------------\
| Fun03: pow10_seqStats
|   - rases 10 to the input exponant
|   - from: CRC standard mathmatical tables 26 edition
| Input:
|   - exponatDbl:
|     o rase base to power
| Output:
|   - Returns:
|     o 10^exponatDbl
\-------------------------------------------------------*/
double
pow10_seqStats(
   double exponatDbl
){
   /*x^a = e^[a * ln(x)]*/
   /*ln(x^r) = r * ln(x)*/
   /*e^x = 1 + x/1! + x^2/2! + x^3/3! + x^4/4! + ...*/

   double factoralDbl = 1;
   double retDbl = 1.0;
   double xPowerDbl = 0;
   signed int siEnd = 2;
   signed char negBl = 0;

   #define def_endDecimal_fun04 12 /*only do 10 rounds*/

   if(exponatDbl < 0)
   { /*If: negative power*/
      exponatDbl *= -1;
      negBl = 1;
   } /*If: negative power*/

   exponatDbl *= 2.30258509299; /*ln(10)*/

   /*find e^x*/
   retDbl = 1.0 + exponatDbl; /*1 + x/1!*/
   siEnd = 2;
   xPowerDbl = exponatDbl;
   while(siEnd < def_endDecimal_fun04)
   { /*Loop: e^x rest of series*/
      xPowerDbl *= exponatDbl;
      factoralDbl *= siEnd++;
      xPowerDbl /= factoralDbl;
      retDbl += xPowerDbl;
   } /*Loop: e^x rest of series*/

   if(negBl)
      return 1.0 / retDbl;
   else
      return retDbl;
} /*pow10_seqStats*/

/*-------------------------------------------------------\
| Fun04: qhistToMedQ_seqStats
|   - converts a q-score histogram to medain q-score
| Input:
|   - qhistArySI:
|     o signed int array with q-scores
|   - seqLenSI:
|     o length of sequence
|   - ontMethodBl:
|     o 1: use ONT method (convert Q to error, then back
|          to q-score with log)
|     o 0: use regular mean
| Output:
|   - Returns median q-score
\-------------------------------------------------------*/
float
qhistToMedQ_seqStats(
   signed int *qhistArySI,
   signed int seqLenSI,
   signed char ontMethodBl
){
   signed int siNt = 0;
   signed int qSI = 0;
   signed int midSI = 0;
   float medianQF = 0;
   float tmpF = 0;

   midSI = seqLenSI >> 1;

   for(siNt = 0; siNt < def_maxQ_seqStats; ++siNt)
   { /*Loop: find median q-score*/
      qSI += qhistArySI[siNt];

      if(qSI >= midSI)
      { /*If: found the midpoint*/
         medianQF = (float) siNt;

         if(ontMethodBl)
         { /*If: need to adjust for ONT*/
            medianQF /= (float) -10;

            #ifdef NO_MATH_H
               medianQF = pow10_seqStats(medianQF);
            #else
               medianQF = pow(10, medianQF);
            #endif
         } /*If: need to adjust for ONT*/

         if(qSI > midSI || qSI & 1)
            ; /*If: one base for the median q-score*/

         else
         { /*Else: even & 2 differnt Q's at midpoint*/
            while(qhistArySI[siNt++] == 0)
               ;
            --siNt; /*account for being one off*/

            if(ontMethodBl)
            { /*If: need to adjust to ONTs method*/
               tmpF = siNt;
               tmpF /= (float) -10;

               #ifdef NO_MATH_H
                  tmpF = pow10_seqStats(tmpF);
               #else
                  tmpF = pow(10, tmpF);
               #endif
            } /*If: need to adjust to ONTs method*/

            else
               tmpF = siNt;

            medianQF += tmpF;
            medianQF /= (float) 2;
         } /*Else: even & 2 differnt Q's at midpoint*/

         break;
      } /*If: found the midpoint*/
   } /*Loop: find median q-score*/

   if(ontMethodBl)
   { /*If: using ont's q-score method*/
      #ifdef NO_MATH_H
         medianQF = -10 * log10_seqStats(medianQF);
      #else
         medianQF = -10 * log10(medianQF);
      #endif
   } /*If: using ont's q-score method*/

   return medianQF;
} /*qhistToMedQ_seqStats*/

/*-------------------------------------------------------\
| Fun05: qhistToMeanQ_seqStats
|   - converts a q-score histogram to mean q-score
| Input:
|   - qhistArySI:
|     o signed int array with q-scores
|   - seqLenSI:
|     o length of sequence
|   - ontMethodBl:
|     o 1: use ONT method (convert Q to error, then back
|          to q-score with log)
|     o 0: use regular mean
| Output:
|   - Returns mean q-score
\-------------------------------------------------------*/
float
qhistToMeanQ_seqStats(
   signed int *qhistArySI,
   signed int seqLenSI,
   signed char ontMethodBl
){
   signed int siNt = 0;
   double meanDbl = 0;
   float qF = 0;

   for(siNt = 0; siNt < def_maxQ_seqStats; ++siNt)
   { /*Loop: find median q-score*/
      if(! qhistArySI[siNt])
         ; /*no q-scores present*/

      else if(ontMethodBl)
      { /*Else If: using ont conversion*/
         qF = ((float) siNt / (float) -10);

         #ifdef NO_MATH_H
            qF = pow10_seqStats(qF);
         #else
            qF = pow(10, qF);
         #endif

         meanDbl += (qF * qhistArySI[siNt]) / seqLenSI;
      } /*Else If: using ont conversion*/

      else
         meanDbl += (qhistArySI[siNt] * siNt) / seqLenSI;
   } /*Loop: find median q-score*/

   if(ontMethodBl)
   { /*If: doing ont's conversion method*/
      #ifdef NO_MATH_H
         meanDbl = -10 * log10_seqStats(meanDbl);
      #else
         meanDbl = -10 * log10(meanDbl);
      #endif
   } /*If: doing ont's conversion method*/

   return meanDbl;
} /*qhistToMeanQ_seqStats*/

/*-------------------------------------------------------\
| Fun06: meanMedQ_seqStats
|   - gets the mean and median q-score for a sequence
| Input:
|   - qStr:
|     o c-string with q-score entry to get q-scores for
|   - seqLenSI:
|     o number of bases in the sequence (index 1)
|   - medianQFPtr:
|     o float pointer to get the median q-score
|   - ignoreXNtSI:
|     o number of nucleotides to ignore at the start of
|       the read (ONT uses 60)
| Output:
|   - Modifies:
|     o medainQFPtr to have the median q-score
|   - Returns:
|     o mean q-score
\-------------------------------------------------------*/
float
meanMedQ_seqStats(
   signed char *qStr,
   signed int seqLenSI,
   float *medianQFPtr,
   signed int ignoreXNtSI
){
   signed int histArySI[def_maxQ_seqStats];
   unsigned long totalQUL = 0;
   signed int siNt = 0;
   signed int qSI = 0;

   *medianQFPtr = 0;
   if(! qStr)
      return 0;
   else if(! qStr[0])
      return 0;
   else if(ignoreXNtSI >= seqLenSI)
      return 0;

   for(siNt = 0; siNt < def_maxQ_seqStats; ++siNt)
      histArySI[siNt] = 0;

   for(siNt = ignoreXNtSI; siNt < seqLenSI; ++siNt)
   { /*Loop: find the q-score*/
      qSI = qStr[siNt] - def_qAdj_seqStats;
      totalQUL += qSI;
      ++histArySI[qSI];
   } /*Loop: find the q-score*/

   *medianQFPtr =
      qhistToMedQ_seqStats(
         histArySI,
         seqLenSI - ignoreXNtSI,
         0
      );
   return
        (double) totalQUL
      / (float) (seqLenSI - ignoreXNtSI);
} /*meanMedQ_seqStats*/

/*-------------------------------------------------------\
| Fun07: ontMeanMedQ_seqStats
|   - gets the mean and median q-score for a sequence using
|     ONT's method
| Input:
|   - qStr:
|     o c-string with q-score entry to get q-scores for
|   - seqLenSI:
|     o number of bases in the sequence (index 1)
|   - medianQFPtr:
|     o float pointer to get the median q-score
|   - ignoreXNtSI:
|     o number of nucleotides to ignore at the start of
|       the read (ONT uses 60)
| Output:
|   - Modifies:
|     o medainQFPtr to have the median q-score
|   - Returns:
|     o mean q-score
\-------------------------------------------------------*/
float
ontMeanMedQ_seqStats(
   signed char *qStr,
   signed int seqLenSI,
   float *medianQFPtr,
   signed int ignoreXNtSI
){
   signed int histArySI[def_maxQ_seqStats];
   signed int siNt = 0;
   signed int qSI = 0;
   float qF = 0;

   *medianQFPtr = 0;
   if(! qStr)
      return 0;
   else if(! qStr[0])
      return 0;
   else if(ignoreXNtSI >= seqLenSI)
      return 0;

   for(siNt = 0; siNt < def_maxQ_seqStats; ++siNt)
      histArySI[siNt] = 0;

   for(siNt = ignoreXNtSI; siNt < seqLenSI; ++siNt)
   { /*Loop: find the q-score*/
      qSI = qStr[siNt++] - def_qAdj_seqStats;
      ++histArySI[qSI];
   } /*Loop: find the q-score*/

   *medianQFPtr =
      qhistToMedQ_seqStats(
         histArySI,
         seqLenSI - ignoreXNtSI,
         1
      );
   qF =
      qhistToMeanQ_seqStats(
          histArySI,
          seqLenSI - ignoreXNtSI,
          1
      );

   return qF;
} /*ontMeanMedQ_seqStats*/

/*-------------------------------------------------------\
| Fun08: pReadStats_seqStats
|   - prints stats for a single read
| Input:
|   - qStr:
|     o c-string with q-score entry to get q-scores for
|   - seqLenSI:
|     o number of bases in the sequence (index 1)
|   - idStr:
|     o c-string with id to print out
|   - pHeadBl:
|     o 1: print header and set to 0
|     o 0: do not print header
|   - ontNtSkipSI:
|     o number of bases to skip for the ONT q-score
|       calculation
|   - regNtSkipSI:
|     o number of bases to skip for the regular q-score
|       calculation
|   - statMedthodSC:
|     o 1: use ONT stat only
|     o 2: use mean/median (no error step) only
|     o 3: print both ONT and mean/median
|   - outFILE:
|     o FILE pionter to print stats to
| Output:
|   - Prints;
|     o read stats (and header if requested) to outFILE
\-------------------------------------------------------*/
void
pReadStats_seqStats(
   signed char *qStr,        /*quality score entry*/
   signed int seqLenSI,      /*bases in quality score*/
   signed char *idStr,       /*read id/name to print*/
   signed char pHeadBl,      /*1: print header*/
   signed int ontNtSkipSI,   /*number ONT bases to skip*/
   signed int regNtSkipSI,   /*number reg bases to skip*/
   signed char statMethodSC, /*1:ONT, 2:mean/med; 3:both*/
   void *outFILE             /*file to print to*/
){
   float meanF = 0;
   float medianF = 0;
   signed char outIdStr[126];
   signed int siNt = 0;


   if(pHeadBl)
   { /*If: printing the header*/
      fprintf((FILE *) outFILE, "id\tlength");
      if(statMethodSC & 1)
         fprintf(
            (FILE *) outFILE, 
            "\tont_mean_q\tont_median_q"
         );
      if(statMethodSC & 2)
         fprintf(
            (FILE *) outFILE, 
            "\tmean_q\tmedian_q%s",
            str_endLine
         );
   } /*If: printing the header*/


   while(idStr[siNt] > 32)
   { /*Loop: copy read id*/
      outIdStr[siNt] = idStr[siNt];
      ++siNt;
   }  /*Loop: copy read id*/
   outIdStr[siNt] = 0;

   fprintf((FILE *) outFILE,"%s\t%i",outIdStr,seqLenSI);

   if(statMethodSC & 1)
   { /*If: printing ONT method*/
      meanF =
         ontMeanMedQ_seqStats(
            qStr,
            seqLenSI,
            &medianF,
            ontNtSkipSI
         );
      fprintf(
         (FILE *) outFILE,
         "\t%0.2f\t%0.2f",
         meanF,
         medianF
      );
   } /*If: printing ONT method*/

   if(statMethodSC & 2)
   { /*If: printing mean/median method*/
      meanF =
         meanMedQ_seqStats(
            qStr,
            seqLenSI,
            &medianF,
            regNtSkipSI
         );
      fprintf(
         (FILE *) outFILE,
         "\t%0.2f\t%0.2f%s",
         meanF,
         medianF,
         str_endLine
      );
   } /*If: printing mean/median method*/

   else
      fprintf((FILE *) outFILE, "%s", str_endLine);
} /*pReadStats_seqStats*/

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
