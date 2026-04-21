/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' ftbGraphs SOF: Start Of File
'   - make the graphs for freezeTB
'   o header:
'     - included libraries
'   o fun01: depthGraph_ftbGraphs
'     - makes the read depth graph for freezeTB
'   o fun02: meanDepthGraph_ftbGraphs
'     - makes the read depth graph for freezeTB
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

#include "../genLib/numToStr.h"
#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"

#include "../genBio/geneCoord.h"

#include "../genFont/fontST.h"
#include "../genFont/ibmSan9.h"

#include "../genPng/mkPng.h"
#include "../genPng/pngDraw.h"

#include "../genAmr/amrST.h"

/*using the .h file only*/
#include "../genLib/genMath.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/endin.h"
!   - .c  #include "../genLib/checksum.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genLib/64bit.h"
!   - .h  #include "../genLib/endLine.h"
!   - .h  #include "drugAry.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: depthGraph_ftbGraphs
|   - makes the read depth graph for freezeTB
| Input:
|   - prefixStr:
|     o c-string with the prefix for the output file name
|   - minDepthSI:
|     o minimum read depth (other depths are flagged)
|   - depthArySI:
|     o signed int array with read depths
|   - geneCoordSTPtr:
|     o geneCoord struct pointer with genes to print the
|       graph for (first 32 used)
|   - amrAryST:
|     o amrST struct array with all AMRs. Should have read
|       depths set for found AMRs
|   - amrLenSI:
|     o number of AMRs in amrAryST
|   - snpMinPercSupF:
|     o minimum percent support to keep a SNP AMR
|   - indelMinPercSupF:
|     o minimum percent support to keep an indel AMR
|   - frameMinPercSupF:
|     o minimum percent support to keep a frame shift AMR
|   - frameshiftBl:
|     o 1: keeping frame shift AMRs
|     o 0: ignoring frame shift AMRs
| Output:
|   - Prints:
|     o png file to <prefixStr>-depths.png
|   - Returns:
|     o 0 for no errors
|     o 1 for file errors
|     o 2 for memory errors
\-------------------------------------------------------*/
signed char
depthGraph_ftbGraphs(
   signed char *prefixStr, /*name for the output file*/
   signed int minDepthSI,  /*minimum read depth*/
   signed int *depthArySI, /*array with read depths*/
   struct geneCoord *geneCoordSTPtr, /*gene coordinates*/
   struct amrST *amrAryST, /*amrs database*/
   signed int amrLenSI,    /*number AMRs*/
   float snpMinPercSupF,   /*minimum % support; SNP AMR*/
   float indelMinPercSupF, /*min % support; indel AMR*/
   float frameMinPercSupF, /*min %; frameshift AMR*/
   signed char frameshiftBl /*looking for frameshifts*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - makes the read depth graph for freezeTB
   '   o fun01 sec01:
   '     - variable declarations
   '   o fun01 sec02:
   '     - initialize and allocate memory
   '   o fun01 sec03:
   '     - draw the graphs
   '   o fun01 sec04:
   '     - print the legend
   '   o fun01 sec05:
   '     - print the graph to a png file
   '   o fun01 sec06:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define pngHeight_fun01 700
   #define pngWidth_fun01 700
   #define rowSpace_fun01 4.0f

   signed char outStr[1024];

   signed int graphHeightSI = 0;
   signed int graphWidthSI = 0;
   signed int rowSpacingSI = 0; /*spacing between graphs*/
   signed int colSpacingSI = 0; /*spacing between graphs*/

   signed int siCol = 0;
   signed int siRow = 0;
   signed int graphSI = 0;
   signed int endSI = 0;

   signed int xSI = 0;
   signed int xStartSI = 0;
   signed int ySI = 0;
   signed int yStartSI = 0;

   signed char colUC = 0;
   signed int ntYSI = 0;
   signed int ntXSI = 0;

   signed int tmpSI = 0;
   float depthPerPixF = 0;
   float ntPerPixF = 0;
   signed int siNt = 0;
   signed int maxDepthSI = 0;

   float supF = 0;

   struct font_fontST fontStackST;
   struct st_mkPng *pngHeapST = 0;
   FILE *outFILE = 0;

   signed int amrPosSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - initialize and allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_font_fontST(&fontStackST);
   if( loadFont_ibmSan9(&fontStackST) )
      goto memErr_fun01_sec06;

   pngHeapST =
      mk_st_mkPng(pngWidth_fun01, pngHeight_fun01, 5);
      /*default is a four color pallete*/
   if(! pngHeapST)
      goto memErr_fun01_sec06;

   addCol_st_mkPng(pngHeapST, 252,253,191, 0, &outStr[0]);
   addCol_st_mkPng(pngHeapST, 241,96,93, 1, &outStr[0]);
   addCol_st_mkPng(pngHeapST, 183,51,121, 2, &outStr[0]);
   addCol_st_mkPng(pngHeapST, 114,31,129, 3, &outStr[0]);
   addCol_st_mkPng(pngHeapST, 0,0,4, 4, &outStr[0]);

   rowSpacingSI =
      (rowSpace_fun01 * fontStackST.heightSS) + 5 + 2;
      /*I want a 2.5 line gap between rows; 2 pixels for
      `  2 legend rows
      */
   graphHeightSI =
      (pngHeight_fun01 - (8 * rowSpacingSI)) / 9;
      /*I want a 8 rows of graphs*/

   colSpacingSI = (2.5 * fontStackST.heightSS) + 5;
      /*I want a 4 character grap between rows and a
      `  5 pixel gap between graphs
      */
   graphWidthSI =
        (pngWidth_fun01 - (4 * colSpacingSI)) / 5
      - (fontStackST.heightSS >> 1);
      /*4 graphs per row*/


   /*make sure no more then 32 graphs*/
   if(geneCoordSTPtr->lenSI <= 32)
      endSI = geneCoordSTPtr->lenSI;
   else
      endSI = 32;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - draw the graphs
   ^   o fun01 sec03 sub01:
   ^     - start loops and find depth/bases per pixel
   ^   o fun01 sec03 sub02:
   ^     - draw the axis's
   ^   o fun01 sec03 sub03:
   ^     - add the y-axis labels
   ^   o fun01 sec03 sub04:
   ^     - add the x-axis labels
   ^   o fun01 sec03 sub05:
   ^     - add the points
   ^   o fun01 sec03 sub07:
   ^     - draw the gene name
   ^   o fun01 sec03 sub08:
   ^     - move to the next graph
   ^   o fun01 sec03 sub09:
   ^     - print axis labels
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec03 Sub01:
   *   - start loops and find depth/bases per pixel
   \*****************************************************/

   yStartSI = (fontStackST.heightSS << 1) + graphHeightSI;
      /*for pngs the top left conner is 0,0*/

   graphSI = 0;

   for(siRow = 0; siRow < 8; ++siRow)
   { /*Loop: add in all rows*/
      xStartSI = fontStackST.heightSS * 1.5;
      xSI = xStartSI;
      ySI = yStartSI;

      for(siCol = 0; siCol < 4; ++siCol)
      { /*Loop: add in each graph*/
         if(graphSI >= endSI)
            break;

         ntPerPixF = geneCoordSTPtr->endAryUI[graphSI];
         ntPerPixF -= geneCoordSTPtr->startAryUI[graphSI];
         ntPerPixF /= (float) (graphWidthSI - 5);

         maxDepthSI = 0;
         for(
            siNt = geneCoordSTPtr->startAryUI[graphSI];
            siNt <
              (signed long)
              geneCoordSTPtr->endAryUI[graphSI];
            ++siNt
         ){ /*Loop: find the maximum read depth*/
             maxDepthSI =
                max_genMath(maxDepthSI, depthArySI[siNt]);
         }  /*Loop: find the maximum read depth*/

         depthPerPixF = (float) maxDepthSI;
         depthPerPixF /= (float) graphHeightSI;

         /***********************************************\
         * Fun01 Sec03 Sub02:
         *   - draw the axis's
         \***********************************************/

         xSI += fontStackST.heightSS * 4 + 3;
            /*move into the x-axis*/

         addBar_pngDraw(
            pngHeapST,
            xSI,
            ySI,
            graphWidthSI,    /*width of my x-axis*/
            2,                /*2 pixels high*/
            4                 /*darkest color possible*/
         ); /*add my x-axis*/

         addBar_pngDraw(
            pngHeapST,
            xSI,
            ySI - graphHeightSI,
            2,                /*2 pixels wide*/
            graphHeightSI,    /*length of my y-axis*/
            4                 /*darkest color possible*/
         ); /*add my y-axis*/

         /***********************************************\
         * Fun01 Sec03 Sub03:
         *   - add the y-axis labels
         \***********************************************/

         drawHorizText_pngDraw(
            (signed char *) "0",
            xSI - fontStackST.heightSS - 5,
            ySI - fontStackST.heightSS / 2,
              /*this is to avoid colisions with x-labels*/
            4,  /*darkest forground color in pallete*/
            -1, /*no backgournd color*/
            &fontStackST,
            pngHeapST
         ); /*0 depth*/

         addBar_pngDraw(
            pngHeapST,
            xSI - 5,
            ySI,
            5,  /*5 pixels wide*/
            2,  /*2 pixels high*/
            4   /*darkest forground color in pallete*/
         ); /*0 depth tick mark*/

         tmpSI = maxDepthSI / 2;
         if(tmpSI >= 1)
         { /*If: can add the mid and end tick marks*/
            tmpSI = numToStr(outStr, tmpSI);

            if(tmpSI > 4)
               tmpSI = 4 * fontStackST.heightSS;
               /*will invade the graph area*/
            else
               tmpSI =
                 measureText_font_fontST(
                    outStr,
                    &fontStackST
                 );

            drawHorizText_pngDraw(
               outStr,
               xSI - tmpSI - 5,
               ySI - (graphHeightSI / 2)
                  - (fontStackST.heightSS / 2),
               4,  /*darkest forground color in pallete*/
               -1, /*no backgournd color*/
               &fontStackST,
               pngHeapST
            ); /*mid depth*/

            addBar_pngDraw(
               pngHeapST,
               xSI - 5,
               ySI - (graphHeightSI / 2),
               5,  /*3 pixels long*/
               2,  /*2 pixels high*/
               4   /*darkest forground color in pallete*/
            ); /*mid depth tick mark*/


            tmpSI = numToStr(outStr, maxDepthSI);

            if(tmpSI > 4)
               tmpSI = 4 * fontStackST.heightSS;
               /*will invade the graph area*/
            else
               tmpSI =
                 measureText_font_fontST(
                    outStr,
                    &fontStackST
                 );

            drawHorizText_pngDraw(
               outStr,
               xSI - tmpSI - 5,
               ySI
                  - graphHeightSI
                  - fontStackST.heightSS / 2,
               4,  /*darkest forground color in pallete*/
               -1, /*no backgournd color*/
               &fontStackST,
               pngHeapST
            ); /*max depth*/

            addBar_pngDraw(
               pngHeapST,
               xSI - 5,
               ySI - graphHeightSI,
               5,  /*3 pixels long*/
               2,  /*2 pixels high*/
               4   /*darkest forground color in pallete*/
            ); /*max depth tick mark*/
         } /*If: can add the mid and end tick marks*/

         /***********************************************\
         * Fun01 Sec03 Sub04:
         *   - add the x-axis labels
         \***********************************************/

         numToStr(
            outStr,
            geneCoordSTPtr->startAryUI[graphSI]
         );
         tmpSI =
            measureText_font_fontST(outStr,&fontStackST);
         tmpSI >>= 1;

         drawHorizText_pngDraw(
            outStr,
            xSI - 2,
            ySI + fontStackST.heightSS,
            4,  /*darkest forground color in pallete*/
            -1, /*no backgournd color*/
            &fontStackST,
            pngHeapST
         ); /*1/4 nucleotide position*/

         addBar_pngDraw(
            pngHeapST,
            xSI,
            ySI + 1,
            2,  /*2 pixels wide*/
            5,  /*5 pixels long*/
            4   /*darkest forground color in pallete*/
         ); /*1/4 nucleotide position mark*/


         numToStr(
            outStr,
            geneCoordSTPtr->endAryUI[graphSI]
         );

         tmpSI =
            measureText_font_fontST(outStr,&fontStackST);

         drawHorizText_pngDraw(
            outStr,
            xSI + graphWidthSI - tmpSI + 2,
            ySI + fontStackST.heightSS,
            4,  /*darkest forground color in pallete*/
            -1, /*no backgournd color*/
            &fontStackST,
            pngHeapST
         ); /*3/4 nucleotide position*/

         addBar_pngDraw(
            pngHeapST,
            xSI + graphWidthSI - 2, /*width of my x-axis*/
            ySI + 1,
            2,  /*2 pixels wide*/
            5,  /*5 pixels long*/
            4   /*darkest forground color in pallete*/
         ); /*3/4 nucleotide position mark*/

         /***********************************************\
         * Fun01 Sec03 Sub05:
         *   - add the points
         \***********************************************/

         for(
            siNt = geneCoordSTPtr->startAryUI[graphSI];
            siNt <
              (signed long)
              geneCoordSTPtr->endAryUI[graphSI];
            ++siNt
         ){ /*Loop: find the maximum read depth*/
            if(depthPerPixF == 0)
               ntYSI = 0;
            else
               ntYSI = depthArySI[siNt] / depthPerPixF;

            ntXSI = siNt + 1; /*index 0 to index 1*/
            ntXSI -= geneCoordSTPtr->startAryUI[graphSI];
            ntXSI /= ntPerPixF;

            if(depthArySI[siNt] < minDepthSI)
               colUC = 2; /*low depth color in pallete*/
            else
               colUC = 4; /*darkest color in pallete*/

            addBar_pngDraw(
               pngHeapST,
               xSI + ntXSI + 1,
               ySI - ntYSI - 1,
               4,  /*4 pixels wide*/
               4,  /*4 pixels high*/
               colUC
            ); /*draw each read depth*/
         }  /*Loop: find the maximum read depth*/

         /***********************************************\
         * Fun01 Sec03 Sub06:
         *   - add AMRs
         \***********************************************/

         amrPosSI =
            getAmr_amrST(
               amrAryST,
               geneCoordSTPtr->startAryUI[graphSI],
               amrLenSI
            );

         for( ; amrPosSI < amrLenSI; ++amrPosSI)
         { /*Loop: graph missed or hit AMRs*/

            ntXSI = amrAryST[amrPosSI].refPosUI + 1;
            ntXSI -= geneCoordSTPtr->startAryUI[graphSI];
            ntXSI /= ntPerPixF;

            if(
                 amrAryST[amrPosSI].refPosUI
               > geneCoordSTPtr->endAryUI[graphSI]
            ) break; /*at end of AMRs*/

            if(
                 depthArySI[amrAryST[amrPosSI].refPosUI]
               < minDepthSI
            ){ /*If: low read depth*/
               addDiamond_pngDraw(
                  pngHeapST,
                  xSI + ntXSI + 1,
                  ySI - graphHeightSI,
                  9,
                  9,
                  3   /*under min read depth color*/
               );

               continue;
            }  /*If: low read depth*/

            else if(
                 amrAryST[amrPosSI].numSupReadsUI
               < (unsigned int) minDepthSI
            ) continue;

            supF =
               (float) amrAryST[amrPosSI].numSupReadsUI;
            supF /=
               (float) amrAryST[amrPosSI].numMapReadsUI;

            if(amrAryST[amrPosSI].frameshiftBl)
            { /*If: have a frameshift AMR*/
               if(! frameshiftBl)
                  continue;
               else if(supF < frameMinPercSupF)
                  continue;
            } /*If: have a frameshift AMR*/

            else if((
               amrAryST[amrPosSI].mutTypeStr[0] | 32)=='s'
            ){ /*If; have a SNP*/
               if(supF < snpMinPercSupF)
                  continue;
            }  /*If; have a SNP*/

            else
            { /*Else; have a indel*/
               if(supF < indelMinPercSupF)
                  continue;
            } /*Else; have a indel*/

            addDiamond_pngDraw(
               pngHeapST,
               xSI + ntXSI + 1,
               ySI - graphHeightSI,
               15,
               15,
               0   /*found AMR color*/
            ); /*outline my resistant diamond*/
            addDiamond_pngDraw(
               pngHeapST,
               xSI + ntXSI + 1,
               ySI - graphHeightSI,
               11,
               11,
               1   /*found AMR color*/
            );
         } /*Loop: graph missed or hit AMRs*/

         /***********************************************\
         * Fun01 Sec03 Sub07:
         *   - draw the gene name
         \***********************************************/

         cpStr_ulCp(
            outStr,
            geneCoordSTPtr->idStrAry[graphSI]
         );

         tmpSI =
            measureText_font_fontST(outStr, &fontStackST);

         drawHorizText_pngDraw(
            outStr,     /*has the gene name*/
            xSI + ((graphWidthSI - tmpSI) / 2),
            ySI
               - graphHeightSI
               - fontStackST.heightSS * 1.5,
              /*this is to avoid colisions with x-labels*/
            4,  /*darkest forground color in pallete*/
            -1, /*no backgournd color*/
            &fontStackST,
            pngHeapST
         ); /*0 depth*/

         /***********************************************\
         * Fun01 Sec03 Sub08:
         *   - move to the next graph
         \***********************************************/

         xSI += graphWidthSI;
         xSI += fontStackST.heightSS + 5;

         ++graphSI;
      } /*Loop: add in each graph*/

      if(graphSI >= endSI)
         break;
      else
         yStartSI += graphHeightSI;
      yStartSI +=
         (fontStackST.heightSS * rowSpace_fun01) + 5;
   } /*Loop: add in all rows*/

   /*****************************************************\
   * Fun01 Sec03 Sub09:
   *   - print axis labels
   \*****************************************************/

   cpStr_ulCp(outStr, (signed char *) "---Read Depth-->");
   tmpSI = measureText_font_fontST(outStr, &fontStackST);
   
   drawVertText_pngDraw(
      outStr,
      fontStackST.heightSS,
      pngHeight_fun01 / 2 - (tmpSI / 2),
      4,  /*darkest forground color in pallete*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*0 depth*/


   ySI += (fontStackST.heightSS * rowSpace_fun01) - 5;
   cpStr_ulCp(
      outStr,
      (signed char *) "---H37Rv Position-->"
   );
   tmpSI = measureText_font_fontST(outStr, &fontStackST);
   
   drawHorizText_pngDraw(
      outStr,
      (pngWidth_fun01 / 2) - (tmpSI / 2),
      ySI,
      4,  /*darkest forground color in pallete*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*0 depth*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - print the legend
   ^   o fun01 sec04 sub01:
   ^     - add in meets read depth legend entry
   ^   o fun01 sec04 sub02:
   ^     - add in under min read depth legend entry
   ^   o fun01 sec04 sub03:
   ^     - add found AMR legend entry
   ^   o fun01 sec04 sub04:
   ^     - add missed AMR legend entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec04 Sub01:
   *   - add in meets read depth legend entry
   \*****************************************************/

   ySI +=
        (fontStackST.heightSS * rowSpace_fun01)
      - 5 - rowSpace_fun01;
   xSI = 50;

   tmpSI = 0;
   tmpSI +=
      cpStr_ulCp(
         &outStr[tmpSI],
         (signed char *) "At Least "
      );
   tmpSI += numToStr(&outStr[tmpSI], minDepthSI);
   tmpSI +=
     cpStr_ulCp(
        &outStr[tmpSI],
        (signed char *) "x Read Depth"
     ); /*adds nulll at end*/
   outStr[tmpSI] = 0;
   tmpSI = measureText_font_fontST(outStr, &fontStackST);
   tmpSI += 13;

   addBar_pngDraw(
      pngHeapST,
      xSI,
      ySI + (fontStackST.heightSS / 3),
      4,  /*4 pixels wide*/
      4,  /*4 pixels high*/
      3   /*darkest color in pallete*/
   ); /*draw good read dpeth legend*/

   addBar_pngDraw(
      pngHeapST,
      xSI + 6,
      ySI + (fontStackST.heightSS / 3),
      4,  /*4 pixels wide*/
      4,  /*4 pixels high*/
      4   /*darkest color in pallete*/
   ); /*draw good read dpeth legend*/

   drawHorizText_pngDraw(
      outStr,     /*has the gene name*/
      xSI + 13,
      ySI,
      4,  /*darkest forground color in pallete*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*over min depth*/

   /*****************************************************\
   * Fun01 Sec04 Sub02:
   *   - add in under min read depth legend entry
   \*****************************************************/

   xSI += tmpSI + fontStackST.widthArySS[0] * 4;

   tmpSI = 0;
   tmpSI +=
      cpStr_ulCp(
         &outStr[tmpSI],
         (signed char *) "Less Than "
      );
   tmpSI += numToStr(&outStr[tmpSI], minDepthSI);
   tmpSI +=
     cpStr_ulCp(
        &outStr[tmpSI],
        (signed char *) "x Read Depth"
     ); /*adds nulll at end*/
   tmpSI += measureText_font_fontST(outStr, &fontStackST);
   tmpSI += 13;

   addBar_pngDraw(
      pngHeapST,
      xSI,
      ySI + (fontStackST.heightSS / 3),
      4,  /*4 pixels wide*/
      4,  /*4 pixels high*/
      2   /*bellow min read depth color*/
   ); /*draw good read dpeht legend*/

   addBar_pngDraw(
      pngHeapST,
      xSI + 6,
      ySI + (fontStackST.heightSS / 3),
      4,  /*4 pixels wide*/
      4,  /*4 pixels high*/
      2   /*bellow min read depth color*/
   ); /*draw good read dpeht legend*/

   drawHorizText_pngDraw(
      outStr,     /*has the gene name*/
      xSI + 13,
      ySI,
      2,  /*darkest forground color in pallete*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*under min depth*/

   /*****************************************************\
   * Fun01 Sec04 Sub03:
   *   - add found AMR legend entry
   \*****************************************************/

   xSI += tmpSI;
   tmpSI = 0;
   tmpSI +=
     cpStr_ulCp(
        &outStr[tmpSI],
        (signed char *) "Found AMR"
     ); /*adds nulll at end*/
   tmpSI += measureText_font_fontST(outStr, &fontStackST);
   tmpSI += 8;

   addDiamond_pngDraw(
      pngHeapST,
      xSI,
      ySI + 4,
      9,
      9,
      1
   );

   drawHorizText_pngDraw(
      outStr,     /*has the gene name*/
      xSI + 8,
      ySI,
      1,  /*darkest forground color in pallete*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*under min depth*/

   /*****************************************************\
   * Fun01 Sec04 Sub04:
   *   - add missed AMR legend entry
   \*****************************************************/

   xSI += tmpSI + fontStackST.widthArySS[0] * 4;

   tmpSI = 0;
   tmpSI +=
     cpStr_ulCp(
        &outStr[tmpSI],
        (signed char *) "Missed AMR"
     ); /*adds nulll at end*/
   tmpSI += measureText_font_fontST(outStr, &fontStackST);

   addDiamond_pngDraw(
      pngHeapST,
      xSI,
      ySI + 4,
      9,
      9,
      3
   );

   drawHorizText_pngDraw(
      outStr,     /*has the gene name*/
      xSI + 8,
      ySI,
      3,  /*darkest forground color in pallete*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*under min depth*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec05:
   ^   - print the graph to a png file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   graphWidthSI = cpStr_ulCp(outStr, prefixStr);
   cpStr_ulCp(
      &outStr[graphWidthSI],
      (signed char *) "-depth.png"
   );

   outFILE = fopen((char *) outStr, "w");
   if(! outFILE)
      goto fileErr_fun01_sec06;
   print_st_mkPng(pngHeapST, outFILE);
   fclose(outFILE);
   outFILE = 0;

   freeHeap_st_mkPng(pngHeapST);
   pngHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec06:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   graphWidthSI = 0;
   goto ret_fun01_sec06;

   fileErr_fun01_sec06:;
      graphWidthSI = 1;
      goto ret_fun01_sec06;

   memErr_fun01_sec06:;
      graphWidthSI = 2;
      goto ret_fun01_sec06;

   ret_fun01_sec06:;
      freeStack_font_fontST(&fontStackST);

      if(pngHeapST)
         freeHeap_st_mkPng(pngHeapST);
      pngHeapST = 0;

      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return graphWidthSI;
} /*depthGraph_ftbGraphs*/

/*-------------------------------------------------------\
| Fun02: meanDepthGraph_ftbGraphs
|   - makes the read depth graph for freezeTB
| Input:
|   - prefixStr:
|     o c-string with the prefix for the output file name
|   - minDepthSI:
|     o minimum read depth (other depths are flagged)
|   - depthArySI:
|     o signed int array with read depths
|   - geneCoordSTPtr:
|     o geneCoord struct pointer with genes to print the
|       graph for (first 32 used)
| Output:
|   - Prints:
|     o png file to <prefixStr>-depths.png
|   - Returns:
|     o 0 for no errors
|     o 1 for file errors
|     o 2 for memory errors
\-------------------------------------------------------*/
signed char
meanDepthGraph_ftbGraphs(
   signed char *prefixStr, /*name for the output file*/
   signed int minDepthSI,  /*minimum read depth*/
   signed int *depthArySI, /*array with read depths*/
   struct geneCoord *geneCoordSTPtr  /*gene coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - makes the mean read depth graph for freezeTB
   '   o fun02 sec01:
   '     - variable declarations
   '   o fun02 sec02:
   '     - initialize and allocate memory
   '   o fun02 sec03:
   '     - get xlabel size, max read depth, and mean depth
   '   o fun02 sec04:
   '     - draw the graphs
   '   o fun02 sec05:
   '     - print the legend
   '   o fun02 sec06:
   '     - print the graph to a png file
   '   o fun02 sec07:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define pngHeight_fun02 700
   #define pngWidth_fun02 700

   #define barGap_fun02 5 /*5 pixel gap between rows*/
   #define rowSpace_fun02 3.5f
   signed char outStr[1024];

   signed short xLabHeightSS = 0;
   signed short barWidthSS = 0;

   signed int siGene = 0;
   signed int endSI = 0;

   signed char colUC = 0;

   signed int tmpSI = 0;
   float depthPerPixF = 0;
   float *meanDepthHeapAryF = 0;
   signed long totalSL = 0;

   signed int siNt = 0;
   signed int maxDepthSI = 0;

   struct font_fontST fontStackST;
   struct st_mkPng *pngHeapST = 0;
   FILE *outFILE = 0;

   /*graph 0,0 positions*/
   signed int xSI = 0;
   signed int ySI = 0;
   signed int adjSI = 0; /*for 0 read depth*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - initialize and allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_font_fontST(&fontStackST);
   if( loadFont_ibmSan9(&fontStackST) )
      goto memErr_fun02_sec07;

   pngHeapST = mk_st_mkPng(0, 0, 0);
      /*default is a four color pallete*/
   if(! pngHeapST)
      goto memErr_fun02_sec07;

   addCol_st_mkPng(pngHeapST, 252,253,191, 0, &outStr[0]);
   addCol_st_mkPng(pngHeapST, 183,51,121, 1, &outStr[0]);
   addCol_st_mkPng(pngHeapST, 0,0,4, 2, &outStr[0]);


   /*make sure no more then 32 genes*/
   if(geneCoordSTPtr->lenSI <= 32)
      endSI = geneCoordSTPtr->lenSI;
   else
      endSI = 32;

   meanDepthHeapAryF = calloc(endSI, sizeof(float));
   if(! meanDepthHeapAryF)
      goto memErr_fun02_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - get x-label size, max read depth, and mean depth
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(siGene = 0; siGene < endSI; ++siGene)
   { /*Loop: find longest gene name, max depth, and mean*/
      cpStr_ulCp(
         outStr,
         geneCoordSTPtr->idStrAry[siGene]
      );
      xLabHeightSS =
         max_genMath(
            xLabHeightSS,
            measureText_font_fontST(outStr, &fontStackST)
         );

      totalSL = 0;
      for(
         siNt = geneCoordSTPtr->startAryUI[siGene];
         siNt <
           (signed long)
           geneCoordSTPtr->endAryUI[siGene];
         ++siNt
      ) totalSL += depthArySI[siNt]; /*find the mean*/

      meanDepthHeapAryF[siGene] =
           totalSL
         / (siNt - geneCoordSTPtr->startAryUI[siGene]);

      maxDepthSI =
         max_genMath(
            maxDepthSI,
            (signed int) meanDepthHeapAryF[siGene] + 1
         ); /*gets max mean depth*/
   } /*Loop: find longest gene name, max depth, and mean*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - draw the graphs
   ^   o fun02 sec03 sub01:
   ^     - find the dimensions
   ^   o fun02 sec04 sub02:
   ^     - draw the axis
   ^   o fun02 sec04 sub03:
   ^     - draw the gene names and mean depths
   ^   o fun02 sec04 sub04:
   ^     - add the points
   ^   o fun02 sec03 sub05:
   ^     - move to the next graph
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec04 Sub01:
   *   - find the dimensions
   \*****************************************************/

   barWidthSS =
         pngWidth_fun02
      - (fontStackST.heightSS * 8)
      - (barGap_fun02 * endSI);
      /*8 = 6 characters for numbers + one for start
      `   padding + one for end padding
      ` - (5 * endSI) is for the spacing around the bars
      */
   barWidthSS /= endSI;

   xSI = fontStackST.heightSS * 7;
   ySI = 
       pngHeight_fun02
     - (xLabHeightSS + fontStackST.heightSS * 5);

   depthPerPixF =
        (float) maxDepthSI
      / (float) (ySI - fontStackST.heightSS * 3);

   /*****************************************************\
   * Fun02 Sec04 Sub02:
   *   - draw the axis
   \*****************************************************/

   addBar_pngDraw(
      pngHeapST,
      xSI,
      ySI,
      (barWidthSS * endSI) + (barGap_fun02 * endSI),
      2, /*2 pixels thick (high)*/
      2  /*darkest color in the palette*/
   ); /*draw the x-axis*/

   addBar_pngDraw(
      pngHeapST,
      xSI,
      fontStackST.heightSS * 3,
      2, /*2 pixels wide*/
      maxDepthSI / depthPerPixF,
      2  /*darkest color in the palette*/
   ); /*draw the y-axis*/

   drawVertText_pngDraw(
      (signed char *) "Mean Read Depth",
      fontStackST.heightSS * 1.5,
      ySI / 2.5,
      2,  /*darkest color*/
      -1, /*no background color*/
      &fontStackST,
      pngHeapST
   );

   totalSL = 0;
   for(siGene = 0; siGene <= 10; ++siGene)
   { /*Loop: add the y-axis labels*/
      numToStr(outStr, totalSL);
      tmpSI =
         measureText_font_fontST(outStr, &fontStackST);

      drawHorizText_pngDraw(
         outStr,
         xSI - tmpSI - 5,
         ySI
            - (totalSL / depthPerPixF)
            - (fontStackST.heightSS / 2),
         2,  /*darkest color*/
         -1, /*no background color*/
         &fontStackST,
         pngHeapST
      );

      addBar_pngDraw(
         pngHeapST,
         xSI - 5,
         ySI - (totalSL / depthPerPixF),
         5,  /*5 pixels long*/
         2,  /*2 pixels high*/
         2   /*darkest forground color in pallete*/
      ); /*1/4 nucleotide position mark*/

      totalSL += maxDepthSI / 10;
   } /*Loop: add the y-axis labels*/

   /*****************************************************\
   * Fun02 Sec04 Sub03:
   *   - draw the gene names and mean depths
   \*****************************************************/

   xSI += barGap_fun02;
   tmpSI = xSI;

   for(siGene = 0; siGene < endSI; ++siGene)
   { /*Loop: add in genes*/
      drawVertText_pngDraw(
         geneCoordSTPtr->idStrAry[siGene],
         tmpSI + fontStackST.heightSS / 2 - 1,
         ySI + fontStackST.heightSS,
         2,  /*darkest forground color in pallete*/
         -1, /*no backgournd color*/
         &fontStackST,
         pngHeapST
      ); /*1/4 nucleotide position*/

      addBar_pngDraw(
         pngHeapST,
         tmpSI + fontStackST.heightSS / 2 + 2,
         ySI + 1,
         2,  /*2 pixels wide*/
         5,  /*5 pixels long*/
         2   /*darkest forground color in pallete*/
      ); /*1/4 nucleotide position mark*/


      /**************************************************\
      * Fun02 Sec04 Sub04:
      *   - add the points
      \**************************************************/

      if(depthPerPixF == 0)
         siNt = 0;
      else
         siNt =
              (float) meanDepthHeapAryF[siGene]
            / depthPerPixF;

      if(meanDepthHeapAryF[siGene] < minDepthSI)
         colUC = 1; /*low depth color in pallete*/
      else
         colUC = 2; /*darkest color in pallete*/

      if(siNt == 0)
         adjSI = 2;

      addBar_pngDraw(
         pngHeapST,
         tmpSI,
         ySI - siNt,
         barWidthSS,   /*width*/
         siNt + adjSI, /*height*/
         colUC
      ); /*draw each read depth*/

      /**************************************************\
      * Fun02 Sec03 Sub05:
      *   - move to the next graph
      \**************************************************/

      tmpSI += barWidthSS + 5;
   } /*Loop: add in each graph*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - print the legend
   ^   o fun02 sec04 sub01:
   ^     - add in meets read depth legend entry
   ^   o fun02 sec04 sub02:
   ^     - add in under min read depth legend entry
   ^   o fun02 sec04 sub03:
   ^     - add found AMR legend entry
   ^   o fun02 sec04 sub04:
   ^     - add missed AMR legend entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec04 Sub01:
   *   - add in meets read depth legend entry
   \*****************************************************/

   ySI += (fontStackST.heightSS * rowSpace_fun02) - 5;
   xSI = 170;

   tmpSI = 0;
   tmpSI +=
      cpStr_ulCp(
         &outStr[tmpSI],
         (signed char *) "At Least "
      );
   tmpSI += numToStr(&outStr[tmpSI], minDepthSI);
   tmpSI +=
     cpStr_ulCp(
        &outStr[tmpSI],
        (signed char *) "x Read Depth"
     ); /*adds nulll at end*/
   outStr[tmpSI] = 0;
   tmpSI = measureText_font_fontST(outStr, &fontStackST);
   tmpSI += 16;

   addBar_pngDraw(
      pngHeapST,
      xSI,
      fontStackST.heightSS + fontStackST.heightSS / 3.5,
      12, /*12 pixels wide*/
      6,  /*6 pixels high*/
      2   /*darkest color in pallete*/
   ); /*draw good read dpeht legend*/

   drawHorizText_pngDraw(
      outStr,     /*has the gene name*/
      xSI + 16,
      fontStackST.heightSS,
      2,  /*darkest forground color in pallete*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*over min depth*/

   /*****************************************************\
   * Fun02 Sec04 Sub02:
   *   - add in under min read depth legend entry
   \*****************************************************/

   xSI += tmpSI + (fontStackST.widthArySS[0] * 4);

   tmpSI = 0;
   tmpSI +=
      cpStr_ulCp(
         &outStr[tmpSI],
         (signed char *) "Less Than "
      );
   tmpSI += numToStr(&outStr[tmpSI], minDepthSI);
   tmpSI +=
     cpStr_ulCp(
        &outStr[tmpSI],
        (signed char *) "x Read Depth"
     ); /*adds nulll at end*/

   addBar_pngDraw(
      pngHeapST,
      xSI,
      fontStackST.heightSS + fontStackST.heightSS / 3.5,
      12, /*12 pixels wide*/
      6,  /*6 pixels high*/
      1   /*bellow min read depth color*/
   ); /*draw good read dpeht legend*/

   drawHorizText_pngDraw(
      outStr,     /*has the gene name*/
      xSI + 16,
      fontStackST.heightSS,
      1,  /*low depth color*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*under min depth*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec06:
   ^   - print the graph to a png file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpSI = cpStr_ulCp(outStr, prefixStr);
   cpStr_ulCp(
      &outStr[tmpSI],
      (signed char *) "-mean-depth.png"
   );

   outFILE = fopen((char *) outStr, "w");
   if(! outFILE)
      goto fileErr_fun02_sec07;
   print_st_mkPng(pngHeapST, outFILE);
   fclose(outFILE);
   outFILE = 0;

   freeHeap_st_mkPng(pngHeapST);
   pngHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec07:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siNt = 0;
   goto ret_fun02_sec07;

   fileErr_fun02_sec07:;
      siNt = 1;
      goto ret_fun02_sec07;

   memErr_fun02_sec07:;
      siNt = 2;
      goto ret_fun02_sec07;

   ret_fun02_sec07:;
      freeStack_font_fontST(&fontStackST);

      if(meanDepthHeapAryF)
         free(meanDepthHeapAryF);
      meanDepthHeapAryF = 0;

      if(pngHeapST)
         freeHeap_st_mkPng(pngHeapST);
      pngHeapST = 0;

      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return siNt;
} /*meanDepthGraph_ftbGraphs*/

/*-------------------------------------------------------\
| Fun03: coverGraph_ftbGraphs
|   - makes the coverage graph for freezeTB
| Input:
|   - prefixStr:
|     o c-string with the prefix for the output file name
|   - minDepthSI:
|     o minimum read depth (other depths are flagged)
|   - depthArySI:
|     o signed int array with read depths
|   - geneCoordSTPtr:
|     o geneCoord struct pointer with genes to print the
|       graph for (first 32 used)
|   - amrAryST:
|     o amrST struct array with all AMRs. Should have read
|       depths set for found AMRs
|   - amrLenSI:
|     o number of AMRs in amrAryST
|   - snpMinPercSupF:
|     o minimum percent support to keep a SNP AMR
|   - indelMinPercSupF:
|     o minimum percent support to keep an indel AMR
|   - frameMinPercSupF:
|     o minimum percent support to keep a frame shift AMR
|   - frameshiftBl:
|     o 1: keeping frame shift AMRs
|     o 0: ignoring frame shift AMRs
|   - coordsStr:
|     o c-string with file with the gene coordinates,
|       which also has the drugs assigned to each gene
| Output:
|   - Prints:
|     o png file to <prefixStr>-depths.png
|   - Returns:
|     o 0 for no errors
|     o 1 for file errors
|     o 2 for memory errors
\-------------------------------------------------------*/
signed char
coverGraph_ftbGraphs(
   signed char *prefixStr, /*name for the output file*/
   signed int minDepthSI,  /*minimum read depth*/
   signed int *depthArySI, /*array with read depths*/
   struct geneCoord *geneCoordSTPtr, /*gene coordinates*/
   struct amrST *amrAryST, /*amrs database*/
   signed int amrLenSI,    /*number AMRs*/
   float snpMinPercSupF,   /*minimum % support; SNP AMR*/
   float indelMinPercSupF, /*min % support; indel AMR*/
   float frameMinPercSupF, /*min %; frameshift AMR*/
   signed char frameshiftBl,/*looking for frameshifts*/
   signed char *coordsStr   /*has drug names/gene*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - makes the coverage graph for freezeTB
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - initialize and allocate memory
   '   o fun03 sec04:
   '     - draw the graphs
   '   o fun03 sec05:
   '     - print the legend
   '   o fun03 sec06:
   '     - print the graph to a png file
   '   o fun03 sec07:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define pngHeight_fun03 700
   #define pngWidth_fun03 700
   #define rowSpace_fun03 4.0f

   signed char outStr[1024];

   signed int graphHeightSI = 0;
   signed int graphWidthSI = 0;
   signed int rowSpacingSI = 0; /*spacing between graphs*/
   signed int colSpacingSI = 0; /*spacing between graphs*/

   signed int siCol = 0;
   signed int siRow = 0;
   signed int graphSI = 0;
   signed int endSI = 0;

   signed int xSI = 0;
   signed int xStartSI = 0;
   signed int ySI = 0;
   signed int yStartSI = 0;

   signed int ntXSI = 0;

   signed int tmpSI = 0;
   float ntPerPixF = 0;
   signed int siNt = 0;

   signed long totalSL = 0;
   int ampStartSI = 0;
   int ampEndSI = 0;

   float supF = 0;

   struct font_fontST fontStackST;
   struct st_mkPng *pngHeapST = 0;
   FILE *outFILE = 0;

   /*graphing positions*/
   signed int coverSegSI = 0; /*genome coverage lines*/
   signed int refSegSI = 0;   /*reference lines*/
   signed int meanDepthPosSI=0;/*position of mean depth*/
   signed int drugPosSI = 0;  /*position of drug names*/

   /*for dealing with the drug list for each gene*/
   signed char **drugHeapAryStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - initialize and allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_font_fontST(&fontStackST);
   if( loadFont_ibmSan9(&fontStackST) )
      goto memErr_fun03_sec07;

   pngHeapST =
      mk_st_mkPng(pngWidth_fun03, pngHeight_fun03, 4);
      /*default is a four color pallete*/
   if(! pngHeapST)
      goto memErr_fun03_sec07;

   addCol_st_mkPng(pngHeapST, 252,253,191, 0, &outStr[0]);
   addCol_st_mkPng(pngHeapST, 241,96,93, 1, &outStr[0]);
   addCol_st_mkPng(pngHeapST, 114,31,129, 2, &outStr[0]);
   addCol_st_mkPng(pngHeapST, 0,0,4, 3, &outStr[0]);

   rowSpacingSI =
      (rowSpace_fun03 * fontStackST.heightSS) + 5 + 2;
      /*I want a 2.5 line gap between rows; 2 pixels for
      `  2 legend rows
      */
   graphHeightSI =
      (pngHeight_fun03 - (8 * rowSpacingSI)) / 9;
      /*I want a 8 rows of graphs*/

   colSpacingSI = (2.5 * fontStackST.heightSS) + 5;
      /*I want a 4 character grap between rows and a
      `  5 pixel gap between graphs
      */
   graphWidthSI =
        (pngWidth_fun03 - (4 * colSpacingSI)) / 5
      - (fontStackST.heightSS >> 2);
      /*4 graphs per row*/


   /*make sure no more then 32 graphs*/
   if(geneCoordSTPtr->lenSI <= 32)
      endSI = geneCoordSTPtr->lenSI;
   else
      endSI = 32;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get in the drug names for each gene
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(coordsStr)
   { /*If: can get drug names*/
      drugHeapAryStr =
         calloc(
            geneCoordSTPtr->lenSI,
            sizeof(signed char *)
         );
      if(! drugHeapAryStr)
         goto memErr_fun03_sec07;

      outFILE = fopen((char *) coordsStr, "r");
      if(! outFILE)
      { /*If: could not open the coordinates file*/
         free(drugHeapAryStr);
         drugHeapAryStr = 0;
         goto skipDrugs_fun03_sec03;
      } /*If: could not open the coordinates file*/

      if( ! fgets((char *) outStr, 1023, outFILE) )
      { /*If: empty file*/
         free(drugHeapAryStr);
         drugHeapAryStr = 0;

         fclose(outFILE);
         outFILE = 0;

         goto skipDrugs_fun03_sec03;
      } /*If: empty file*/

      while( fgets((char *) outStr, 1023, outFILE) )
      { /*Loop: get the drug names*/
         /*gene id (skip)*/
         tmpSI = endWhite_ulCp(outStr);
         while(outStr[tmpSI] && outStr[tmpSI] < 33)
            ++tmpSI;
         if(! outStr[tmpSI])
            continue;

         /*reference name (skip)*/
         tmpSI += endWhite_ulCp(&outStr[tmpSI]);
         while(outStr[tmpSI] && outStr[tmpSI] < 33)
            ++tmpSI;
         if(! outStr[tmpSI])
            continue;

         /*gene direction (skip)*/
         tmpSI += endWhite_ulCp(&outStr[tmpSI]);
         while(outStr[tmpSI] && outStr[tmpSI] < 33)
            ++tmpSI;
         if(! outStr[tmpSI])
            continue;

         /*start postion*/
         tmpSI +=
            strToSI_base10str(&outStr[tmpSI],&ampStartSI);
         while(outStr[tmpSI] && outStr[tmpSI] < 33)
            ++tmpSI;
         if(! outStr[tmpSI])
            continue;

         /*end postion (skip)*/
         tmpSI += endWhite_ulCp(&outStr[tmpSI]);
            strToSI_base10str(&outStr[tmpSI], &ampEndSI);
         while(outStr[tmpSI] && outStr[tmpSI] < 33)
            ++tmpSI;
         if(! outStr[tmpSI])
            continue;

         /*get gene on*/
         graphSI =
            findStart_geneCoord(
               geneCoordSTPtr,
               ampStartSI,
               0 /*ignoring the reference*/
            );

         if(graphSI < 0)
            continue; /*gene not in coordinates*/
         else if(drugHeapAryStr[graphSI])
            continue; /*already have drugs for this gene*/

         ampStartSI = tmpSI;
         while(outStr[tmpSI] && outStr[tmpSI] != '*')
            ++tmpSI;

         while(tmpSI > ampStartSI && outStr[tmpSI -1] <33)
            --tmpSI;
         outStr[tmpSI] = 0; /*remove the `*`*/
         if(tmpSI == ampStartSI)
            continue; /*no drugs for this gene*/

         ampEndSI = tmpSI - ampStartSI;

         drugHeapAryStr[graphSI] =
            malloc((ampEndSI + 8) * sizeof(signed char));
         if(! drugHeapAryStr[graphSI])
            goto memErr_fun03_sec07;

         tmpSI = ampStartSI;
         siNt = 0;
         while(outStr[tmpSI])
         { /*Loop: remove extra white space*/
            if(
                  (outStr[tmpSI] | 32) == 'n'
               && (outStr[tmpSI + 1] | 32) == 'a'
               && outStr[tmpSI + 2] < 33
            ){ /*If: NA (blank) entry*/
               tmpSI += 2;
               while(outStr[tmpSI] && outStr[tmpSI] < 33)
                  ++tmpSI;
            }  /*If: NA (blank) entry*/

            else if(
                  outStr[tmpSI] < 33
               && (outStr[tmpSI + 1] | 32) == 'n'
               && (outStr[tmpSI + 2] | 32) == 'a'
               && outStr[tmpSI + 3] < 33
            ){ /*Else If: NA with white space*/
               tmpSI += 3;
               while(outStr[tmpSI] && outStr[tmpSI] < 33)
                  ++tmpSI;
            }  /*Else If: NA with white space*/

            else if(
               outStr[tmpSI] < 33 && outStr[tmpSI +1] < 33
            ) ++tmpSI; /*double white space*/

            else if(outStr[tmpSI] == '\t')
            { /*Else If: on a tab*/
               drugHeapAryStr[graphSI][siNt++] = ' ';
               ++tmpSI;
            } /*Else If: on a tab*/

            else
            { /*Else: if a unique drug*/
               while(outStr[tmpSI] > 32)
                  drugHeapAryStr[graphSI][siNt++] =
                     outStr[tmpSI++];
            } /*Else: if a unique drug*/
         } /*Loop: remove extra white space*/

         drugHeapAryStr[graphSI][siNt] = 0;
         drugHeapAryStr[graphSI][siNt + 1] = 0;
         drugHeapAryStr[graphSI][siNt + 2] = 0;
         drugHeapAryStr[graphSI][siNt + 3] = 0;
         drugHeapAryStr[graphSI][siNt + 4] = 0;
         drugHeapAryStr[graphSI][siNt + 5] = 0;
         drugHeapAryStr[graphSI][siNt + 6] = 0;
         drugHeapAryStr[graphSI][siNt + 7] = 0;
      } /*Loop: get the drug names*/

      fclose(outFILE);
      outFILE = 0;
   } /*If: can get drug names*/

   skipDrugs_fun03_sec03:;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - draw the graphs
   ^   o fun03 sec04 sub01:
   ^     - start loops, find bases per pixel, print drugs
   ^   o fun03 sec04 sub02:
   ^     - draw the axis's
   ^   o fun03 sec04 sub03:
   ^     - add the y-axis labels
   ^   o fun03 sec04 sub04:
   ^     - add the x-axis labels
   ^   o fun03 sec04 sub05:
   ^     - coverage lines
   ^   o fun03 sec04 sub06:
   ^     - mean read depth
   ^   o fun03 sec04 sub07:
   ^     - add AMRs
   ^   o fun03 sec04 sub08:
   ^     - draw the gene name
   ^   o fun03 sec04 sub09:
   ^     - move to the next graph
   ^   o fun03 sec04 sub10:
   ^     - print axis labels
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec04 Sub01:
   *   - start loops, find bases per pixel, print drugs
   \*****************************************************/

   yStartSI =
        (fontStackST.heightSS * 3.25)
      + graphHeightSI;
      /*for pngs the top left conner is 0,0*/

   graphSI = 0;

   for(siRow = 0; siRow < 8; ++siRow)
   { /*Loop: add in all rows*/
      xStartSI = fontStackST.heightSS * 2;
      xSI = xStartSI;
      ySI = yStartSI;

      for(siCol = 0; siCol < 4; ++siCol)
      { /*Loop: add in each graph*/
         if(graphSI >= endSI)
            break;

         ntPerPixF = geneCoordSTPtr->endAryUI[graphSI];
         ntPerPixF -= geneCoordSTPtr->startAryUI[graphSI];
         ntPerPixF /= (float) (graphWidthSI - 5);

         drugPosSI = ySI - 3;
         coverSegSI=drugPosSI - fontStackST.heightSS - 5;
         refSegSI = coverSegSI - 11;
         meanDepthPosSI = refSegSI - 11;

         /***********************************************\
         * Fun03 Sec04 Sub02:
         *   - draw the axis's
         \***********************************************/

         xSI += fontStackST.heightSS * 4 + 3;
            /*move into the x-axis*/

         addBar_pngDraw(
            pngHeapST,
            xSI,
            ySI,
            graphWidthSI,    /*width of my x-axis*/
            2,                /*2 pixels high*/
            3                 /*darkest color possible*/
         ); /*add my x-axis*/

         addBar_pngDraw(
            pngHeapST,
            xSI,
            ySI - graphHeightSI,
            2,                /*2 pixels wide*/
            graphHeightSI,    /*length of my y-axis*/
            3                 /*darkest color possible*/
         ); /*add my y-axis*/

         /***********************************************\
         * Fun03 Sec04 Sub03:
         *   - add the y-axis labels
         \***********************************************/

         tmpSI =
            measureText_font_fontST(
               (signed char *) "target",
               &fontStackST
            );

         drawHorizText_pngDraw(
            (signed char *) "target",
            xSI - tmpSI - 4,
            refSegSI - 3,
              /*this is to avoid colisions with x-labels*/
            3,  /*darkest forground color in pallete*/
            -1, /*no backgournd color*/
            &fontStackST,
            pngHeapST
         ); /*0 depth*/

         tmpSI =
            measureText_font_fontST(
               (signed char *) "real",
               &fontStackST
            );
         drawHorizText_pngDraw(
            (signed char *) "real",
            xSI - tmpSI - 4,
            coverSegSI - 3,
              /*this is to avoid colisions with x-labels*/
            3,  /*darkest forground color in pallete*/
            -1, /*no backgournd color*/
            &fontStackST,
            pngHeapST
         ); /*0 depth*/

         /***********************************************\
         * Fun03 Sec04 Sub04:
         *   - add the x-axis labels
         \***********************************************/

         numToStr(
            outStr,
            geneCoordSTPtr->startAryUI[graphSI]
         );
         tmpSI =
            measureText_font_fontST(outStr,&fontStackST);
         tmpSI >>= 1;

         drawHorizText_pngDraw(
            outStr,
            xSI - 2,
            ySI + fontStackST.heightSS,
            3,  /*darkest forground color in pallete*/
            -1, /*no backgournd color*/
            &fontStackST,
            pngHeapST
         ); /*last base in amplicon position*/

         addBar_pngDraw(
            pngHeapST,
            xSI,
            ySI + 1,
            2,  /*2 pixels wide*/
            5,  /*5 pixels long*/
            3   /*darkest forground color in pallete*/
         ); /*first base in amplicon tick mark*/


         numToStr(
            outStr,
            geneCoordSTPtr->endAryUI[graphSI]
         );

         tmpSI =
            measureText_font_fontST(outStr,&fontStackST);

         drawHorizText_pngDraw(
            outStr,
            xSI + graphWidthSI - tmpSI + 2,
            ySI + fontStackST.heightSS,
            3,  /*darkest forground color in pallete*/
            -1, /*no backgournd color*/
            &fontStackST,
            pngHeapST
         ); /*last base in amplicon*/

         addBar_pngDraw(
            pngHeapST,
            xSI + graphWidthSI - 2, /*width of my x-axis*/
            ySI + 1,
            2,  /*2 pixels wide*/
            5,  /*5 pixels long*/
            3   /*darkest forground color in pallete*/
         ); /*last base in amplicon position mark*/

         /***********************************************\
         * Fun03 Sec04 Sub05:
         *   - coverage lines
         \***********************************************/

         if(drugHeapAryStr && drugHeapAryStr[graphSI])
         { /*If: have drug names for this graph*/
            tmpSI =
               measureText_font_fontST(
                  drugHeapAryStr[graphSI],
                  &fontStackST
               );

            tmpSI = graphWidthSI / 2 - tmpSI / 2;
            if(tmpSI < 0)
               tmpSI = 0;

            drawHorizText_pngDraw(
               drugHeapAryStr[graphSI],
               xSI + tmpSI,
               drugPosSI - fontStackST.heightSS + 1,
               3,  /*darkest forground color in pallete*/
               -1, /*no backgournd color*/
               &fontStackST,
               pngHeapST
            ); /*drug names to add*/
         } /*If: have drug names for this graph*/


         ampEndSI = geneCoordSTPtr->endAryUI[graphSI];
         ampEndSI -= geneCoordSTPtr->startAryUI[graphSI];
         ampEndSI /= ntPerPixF;

         addBar_pngDraw(
            pngHeapST,
            xSI,
            refSegSI,
            ampEndSI, /*expected length of the amplicon*/
            4,  /*4 pixels high*/
            3   /*darkest color in pallete*/
         ); /*draw the amplicon length line*/

         totalSL = 0;
         ampStartSI = 0;
         ampEndSI = 0;
         for(
            siNt = geneCoordSTPtr->startAryUI[graphSI];
            siNt <
              (signed long)
              geneCoordSTPtr->endAryUI[graphSI];
            ++siNt
         ){ /*Loop: find the maximum read depth*/
             if(depthArySI[siNt] < minDepthSI)
             { /*If: beneath min read depth*/
                if(ampStartSI)
                { /*If: have an amplicon to graph*/
                   ampEndSI -= ampStartSI;
                   ampStartSI -=
                      geneCoordSTPtr->startAryUI[graphSI];

                   ampStartSI /= ntPerPixF;
                   ampEndSI /= ntPerPixF;

                   addBar_pngDraw(
                      pngHeapST,
                      xSI + ampStartSI,
                      coverSegSI,
                      ampEndSI, /*length*/
                      4,  /*4 pixels high*/
                      3   /*darkest color in pallete*/
                   ); /*draw the coverage line*/
                } /*If: have an amplicon to graph*/

                ampStartSI = 0;
                ampEndSI = 0;
             } /*If: beneath min read depth*/
             
             else if(! ampStartSI)
             { /*Else If: first base with good depth*/
                ampStartSI = siNt;
                ampEndSI = siNt;
             } /*Else If: first base with good depth*/

             else
                ampEndSI = siNt;

             totalSL += depthArySI[siNt];
         }  /*Loop: find the maximum read depth*/

         if(ampStartSI)
         { /*If: have an final amplicon to graph*/
            ampEndSI -= ampStartSI;
            ampStartSI -=
               geneCoordSTPtr->startAryUI[graphSI];

            ampStartSI /= ntPerPixF;
            ampEndSI /= ntPerPixF;

            addBar_pngDraw(
               pngHeapST,
               xSI + ampStartSI,
               coverSegSI,
               ampEndSI, /*length*/
               4,  /*4 pixels high*/
               3   /*darkest color in pallete*/
            ); /*draw the coverage line*/
         } /*If: have an final amplicon to graph*/

         /***********************************************\
         * Fun03 Sec04 Sub06:
         *   - mean read depth
         \***********************************************/

         /*find the mean depth*/
         ampEndSI = geneCoordSTPtr->endAryUI[graphSI];
         ampEndSI -= geneCoordSTPtr->startAryUI[graphSI];
         supF = (double) totalSL / (float) ampEndSI;

         tmpSI =
            cpStr_ulCp(
               outStr,
               (signed char *) "depth="
            );
         tmpSI += double_numToStr(&outStr[tmpSI],supF,1);
         outStr[tmpSI++] = 'x';
         outStr[tmpSI] = 0;

         tmpSI =
            measureText_font_fontST(outStr, &fontStackST);
         drawHorizText_pngDraw(
            outStr,
            xSI + tmpSI / 5,
            meanDepthPosSI - 3,
            3,  /*make a dark forground color*/
            -1, /*make a transparent color*/
            &fontStackST,
            pngHeapST
         );

         /***********************************************\
         * Fun03 Sec04 Sub07:
         *   - add AMRs
         \***********************************************/

         siNt =
            getAmr_amrST(
               amrAryST,
               geneCoordSTPtr->startAryUI[graphSI],
               amrLenSI
            );

         for( ; siNt < amrLenSI; ++siNt)
         { /*Loop: graph missed or hit AMRs*/

            ntXSI = amrAryST[siNt].refPosUI + 1;
            ntXSI -= geneCoordSTPtr->startAryUI[graphSI];
            ntXSI /= ntPerPixF;

            if(
                 amrAryST[siNt].refPosUI
               > geneCoordSTPtr->endAryUI[graphSI]
            ) break; /*at end of AMRs*/

            if(
                  amrAryST[siNt].numSupReadsUI
               < (unsigned int) minDepthSI
            ){ /*If: support*/
               noAmrSupport_fun03_sec04_sub07:;
                  addDiamond_pngDraw(
                     pngHeapST,
                     xSI + ntXSI + 1,
                     refSegSI + 1,
                     11,
                     11,
                     2   /*under min read depth color*/
                  );
                  continue;
            }  /*If: low read depth*/

            supF =
               (float) amrAryST[siNt].numSupReadsUI;
            supF /=
               (float) amrAryST[siNt].numMapReadsUI;

            if(amrAryST[siNt].frameshiftBl)
            { /*If: have a frameshift AMR*/
               if(! frameshiftBl)
                  goto noAmrSupport_fun03_sec04_sub07;
               else if(supF < frameMinPercSupF)
                  goto noAmrSupport_fun03_sec04_sub07;
            } /*If: have a frameshift AMR*/

            else if((
               amrAryST[siNt].mutTypeStr[0] | 32)=='s'
            ){ /*If; have a SNP*/
               if(supF < snpMinPercSupF)
                  goto noAmrSupport_fun03_sec04_sub07;
            }  /*If; have a SNP*/

            else
            { /*Else; have a indel*/
               if(supF < indelMinPercSupF)
                  goto noAmrSupport_fun03_sec04_sub07;
            } /*Else; have a indel*/

            addDiamond_pngDraw(
               pngHeapST,
               xSI + ntXSI + 1,
               coverSegSI + 1,
               11,
               11,
               1   /*found AMR color*/
            );
         } /*Loop: graph missed or hit AMRs*/

         /***********************************************\
         * Fun03 Sec04 Sub08:
         *   - draw the gene name
         \***********************************************/

         cpStr_ulCp(
            outStr,
            geneCoordSTPtr->idStrAry[graphSI]
         );

         tmpSI =
            measureText_font_fontST(outStr, &fontStackST);

         drawHorizText_pngDraw(
            outStr,     /*has the gene name*/
            xSI + ((graphWidthSI - tmpSI) / 2),
            ySI
               - graphHeightSI
               - fontStackST.heightSS * 1.75,
              /*this is to avoid colisions with x-labels*/
            3,  /*darkest forground color in pallete*/
            -1, /*no backgournd color*/
            &fontStackST,
            pngHeapST
         ); /*0 depth*/

         /***********************************************\
         * Fun03 Sec04 Sub09:
         *   - move to the next graph
         \***********************************************/

         xSI += graphWidthSI;
         xSI += fontStackST.heightSS + 5;

         ++graphSI;
      } /*Loop: add in each graph*/

      if(graphSI >= endSI)
         break;
      else
         yStartSI += graphHeightSI;
      yStartSI +=
         (fontStackST.heightSS * rowSpace_fun03) + 5;
   } /*Loop: add in all rows*/

   /*****************************************************\
   * Fun03 Sec04 Sub10:
   *   - print axis labels
   \*****************************************************/

   ySI += (fontStackST.heightSS * rowSpace_fun03) - 5;
   cpStr_ulCp(
      outStr,
      (signed char *) "---H37Rv Position-->"
   );
   tmpSI = measureText_font_fontST(outStr, &fontStackST);
   
   drawHorizText_pngDraw(
      outStr,
      (pngWidth_fun03 / 2) - (tmpSI / 2),
      ySI,
      3,  /*darkest forground color in pallete*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*0 depth*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec05:
   ^   - print the legend
   ^   o fun03 sec05 sub01:
   ^     - add found AMR legend entry
   ^   o fun03 sec05 sub02:
   ^     - add missed AMR legend entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec05 Sub01:
   *   - add found AMR legend entry
   \*****************************************************/

   ySI +=
        (fontStackST.heightSS * 3)
      - 5 - rowSpace_fun03;
   xSI = 240;

   tmpSI = 0;
   tmpSI +=
     cpStr_ulCp(
        &outStr[tmpSI],
        (signed char *) "Found AMR"
     ); /*adds nulll at end*/
   tmpSI += measureText_font_fontST(outStr, &fontStackST);
   tmpSI += 8;

   addDiamond_pngDraw(
      pngHeapST,
      xSI,
      ySI + 4,
      9,
      9,
      1
   );

   drawHorizText_pngDraw(
      outStr,     /*has the gene name*/
      xSI + 8,
      ySI,
      1,  /*found AMR color*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*under min depth*/

   /*****************************************************\
   * Fun03 Sec05 Sub02:
   *   - add missed AMR legend entry
   \*****************************************************/

   xSI += tmpSI + fontStackST.widthArySS[0] * 4;

   tmpSI = 0;
   tmpSI +=
     cpStr_ulCp(
        &outStr[tmpSI],
        (signed char *) "No AMR Found"
     ); /*adds nulll at end*/
   tmpSI += measureText_font_fontST(outStr, &fontStackST);

   addDiamond_pngDraw(
      pngHeapST,
      xSI,
      ySI + 4,
      9,
      9,
      2
   );

   drawHorizText_pngDraw(
      outStr,     /*has the gene name*/
      xSI + 8,
      ySI,
      2,  /*missing AMR color*/
      -1, /*no backgournd color*/
      &fontStackST,
      pngHeapST
   ); /*under min depth*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec06:
   ^   - print the graph to a png file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   graphWidthSI = cpStr_ulCp(outStr, prefixStr);
   cpStr_ulCp(
      &outStr[graphWidthSI],
      (signed char *) "-coverage.png"
   );

   outFILE = fopen((char *) outStr, "w");
   if(! outFILE)
      goto fileErr_fun03_sec07;
   print_st_mkPng(pngHeapST, outFILE);
   fclose(outFILE);
   outFILE = 0;

   freeHeap_st_mkPng(pngHeapST);
   pngHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec07:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   graphWidthSI = 0;
   goto ret_fun03_sec07;

   fileErr_fun03_sec07:;
      graphWidthSI = 1;
      goto ret_fun03_sec07;

   memErr_fun03_sec07:;
      graphWidthSI = 2;
      goto ret_fun03_sec07;

   ret_fun03_sec07:;
      freeStack_font_fontST(&fontStackST);

      if(pngHeapST)
         freeHeap_st_mkPng(pngHeapST);
      pngHeapST = 0;

      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      if(drugHeapAryStr)
      { /*If: need to free the drugs list*/
         for(
            siNt = 0;
            siNt < geneCoordSTPtr->lenSI;
            ++siNt
         ){ /*Loop: free all allocated c-strings*/
            if(drugHeapAryStr[siNt])
               free(drugHeapAryStr[siNt]);
            drugHeapAryStr[siNt] = 0;
         } /*Loop: free all allocated c-strings*/

         free(drugHeapAryStr);
         drugHeapAryStr = 0;
      } /*If: need to free the drugs list*/

      return graphWidthSI;
} /*coverGraph_ftbGraphs*/

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
