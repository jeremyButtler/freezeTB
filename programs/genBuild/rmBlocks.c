/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' rmBlocks SOF: Start Of File
'   - remove blocks (start ---, end ---) from file and
'     merge block contents onto a line
'   - also removes all comments
'     - formats: #.*\n; #---.*---#; or #---.*---#
'   o header:
'     - included libraries
'   o fun01: str_rmBlocks
'     - converts blocks in string (---) to single lines
'   o license:
'     - licensing for this code (public domain / mit)
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

#include "rmBlocks.h"

#include <stdio.h>

#include "../genLib/ulCp.h"

/*for marking on a string*/
#define def_noStr_rmBlocks 0 /*do not change*/
#define def_onStr_rmBlocks 1
#define def_singleQuote_rmBlocks 2
#define def_doubleQuote_rmBlocks 4

#define def_block_rmBlocks 1
#define def_commentBlock_rmBlocks 2

/*-------------------------------------------------------\
| Fun01: str_rmBlocks
|   - converts blocks in string (---) to single lines
| Input:
|   - textStr:
|     o c-string with blocks to convert to lines
|   - lenUL:
|     o has length of textStr
|     o 0 if modifying original string
|   - retLenULPtr:
|     o pointer to unsigned long with new string length
|     o can set equal to lenUL
|     o if length not waned, use null
|   - keepCommentBl:
|     o 1: keep all non-block comments
|     o 0: remove all comments
| Output:
|   - Modifies:
|     o retLenULPtr to have length of modified string
|     0 textStr to have not blocks if lenUL is 0
|   - Returns:
|     o modified c-string with textStr with all blocks
|       (start and end with ---) symbols removed and
|       block entries converted to a single line
|       - is new pointer if lenUL is not 0
|       - pointer to textStr if lenUL is 0
|     o 0 for memory errors
<\-------------------------------------------------------*/
signed char *
str_rmBlocks(
   signed char *textStr,
   unsigned long lenUL,
   unsigned long *retLenULPtr,
   signed char keepCommentBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - converts blocks (#--- ---) to single lines
   '   o fun01 sec01:
   '     - variable declarations
   '   o fun01 sec02:
   '     - allocated return string memory
   '   o fun01 sec03:
   '     - remove blocks
   '   o fun01 sec04:
   '     - return string without blocks
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tmpStr = textStr;
   signed char *scanStr = textStr;

   signed char *retStr = 0;
   signed char *dupStr = 0;
   signed char blockBl = 0;
   unsigned char stringFlagUC = 0;

   unsigned long tmpUL = 0;
   signed char emptyBl = 0; /*marks if line white space*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - allocated return string memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! retLenULPtr)
      retLenULPtr = &tmpUL;

   *retLenULPtr = 0;

   if(lenUL)
   { /*If: can not modify input string*/
      retStr = malloc((lenUL + 9) * sizeof(signed char));

      if(! retStr)
         return 0;

      dupStr = retStr;
   } /*If: can not modify input string*/

   else
      dupStr = textStr; /*modifing input string*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - remove blocks
   ^   o fun01 sec03 sub01:
   ^     - get past shebang (if present)
   ^   o fun01 sec03 sub02:
   ^     - deal with escaped charaters + start loop
   ^   o fun01 sec03 sub03:
   ^     - deal with strings + start loop
   ^   o fun01 sec03 sub04:
   ^     - deal with comments
   ^   o fun01 sec03 sub05:
   ^     - deal with extra white space
   ^   o fun01 sec03 sub06:
   ^     - detect if working on a block
   ^   o fun01 sec03 sub07:
   ^     - merge lines in the bock + debuging white space
   ^   o fun01 sec03 sub08:
   ^     - move to next characer in block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec03 Sub01:
   *   - get past shebang (if present)
   \*****************************************************/

   if(
         tmpStr[0] == '#' 
      && tmpStr[1] == '!' 
   ){ /*If: have shebang*/
      while(*tmpStr != '\n')
      { /*Loop: copy the shebang*/
          if(*tmpStr == '\0')
             break;

          *dupStr++ = *tmpStr++;
         ++(*retLenULPtr);
      } /*Loop: copy the shebang*/

      if(*tmpStr == '\n')
      { /*If: not at end of string*/
         *dupStr++ = *tmpStr++;
         ++(*retLenULPtr);
      } /*If: not at end of string*/
   } /*If: have shebang*/

   /*****************************************************\
   * Fun01 Sec03 Sub02:
   *   - deal with escaped charaters + start loop
   \*****************************************************/

   while(*tmpStr != '\0')
   { /*Loop: scan for blocks*/
      if(tmpStr[0] == '\\')
      { /*If: have escaped value*/
         ++tmpStr;

         if(*tmpStr == '"')
         { /*If: escaped double quote string*/
            *dupStr++ = '"';
            ++tmpStr;
            ++(*retLenULPtr);
            continue;
         } /*If: escaped double qoute string*/

         if(*tmpStr == '\'')
         { /*If: escaped single quote string*/
            *dupStr++ = '"';
            ++tmpStr;
            ++(*retLenULPtr);
            continue;
         } /*If: escaped single qoute string*/

         else if(stringFlagUC != def_noStr_rmBlocks)
         { /*Else: in string, no special symbols*/
            --tmpStr; /*keep \\*/
            goto nextChar_fun01_sec03_sub04;
         } /*Else: in string, no special symbols*/

         else if(*tmpStr == '\\')
         { /*Else If: slash escape*/
            *dupStr++ = '\\';
            ++tmpStr;
            ++(*retLenULPtr);

            continue;
         } /*Else If: slash escape*/

         else if(*tmpStr == '#')
         { /*Else If: comment escape (no comment)*/
            *dupStr++ = '#';
            ++tmpStr;
            ++(*retLenULPtr);

            if(
                  tmpStr[0] == '-'
               && tmpStr[1] == '-'
               && tmpStr[2] == '-'
               && tmpStr[3] != '-'
            ) goto blockEscape_fun01_sec03_sub01;

            continue;
         } /*Else If: comment escape (no comment)*/

         else if(
               tmpStr[0] == '-'
            && tmpStr[1] == '-'
            && tmpStr[2] == '-'
            && tmpStr[3] != '-'
         ){ /*Else If: block escape*/
            blockEscape_fun01_sec03_sub01:;

            *dupStr++ = '-';
            ++tmpStr;
            ++(*retLenULPtr);

            *dupStr++ = '-';
            ++tmpStr;
            ++(*retLenULPtr);

            *dupStr++ = '-';
            ++tmpStr;
            ++(*retLenULPtr);

            continue;
         } /*Else If: block escape*/
      } /*Else If: have escaped value*/

      /**************************************************\
      * Fun01 Sec03 Sub03:
      *   - deal with strings
      \**************************************************/

      if(tmpStr[0] == '"')
      { /*If: have duoble quote string*/
         if(stringFlagUC == def_noStr_rmBlocks)
            stringFlagUC =
                 def_doubleQuote_rmBlocks
               | def_onStr_rmBlocks;
            /*starting a new string*/

         else if(stringFlagUC & def_doubleQuote_rmBlocks)
            stringFlagUC = def_noStr_rmBlocks;
            /*ending a string*/

         /*else: in single quoted string; do nothing*/

         goto nextChar_fun01_sec03_sub04;
      } /*If: have duoble quote string*/

      else if(tmpStr[0] == '\'')
      { /*Else If: single quote string*/
         if(stringFlagUC == def_noStr_rmBlocks)
            stringFlagUC =
                 def_singleQuote_rmBlocks
               | def_onStr_rmBlocks;
            /*starting a new string*/

         else if(stringFlagUC & def_singleQuote_rmBlocks)
            stringFlagUC = def_noStr_rmBlocks;
            /*ending a string*/

         /*else: in double quoted string; do nothing*/

         goto nextChar_fun01_sec03_sub04;
      } /*Else If: single quote string*/

      else if(stringFlagUC)
         goto nextChar_fun01_sec03_sub04;
         /*middle of string, nothing to check here*/

      /**************************************************\
      * Fun01 Sec03 Sub04:
      *   - deal with comments
      \**************************************************/

      if(tmpStr[0] == '#')
      { /*If: have comment*/

         ++tmpStr;

         if(
               tmpStr[0] == '-'
            && tmpStr[1] == '-'
            && tmpStr[2] == '-'
            && tmpStr[3] != '-'
         ){ /*If: have block comment*/
            tmpStr += 3; /*got off block start*/

            while(*tmpStr != '\0')
            { /*Loop: find end of block*/

               if( *tmpStr == '\\' )
               { /*If: may have an escaped block*/
                  ++tmpStr;

                  while(*tmpStr == '-')
                     ++tmpStr;

                  continue;
               } /*If: may have an escaped block*/

               if(
                     tmpStr[0] == '-'
                  && tmpStr[1] == '-'
                  && tmpStr[2] == '-'
               ){ /*If: have comment block*/
                  tmpStr += 3;

                  if(tmpStr[0] != '-')
                  { /*If: found end block*/
                     if(*tmpStr == '#')
                        ++tmpStr; /*if using ---# end*/

                     break;
                  } /*If: found end block*/

                  while(*tmpStr == '-')
                    ++tmpStr; /*move to end of dashes*/
               } /*If: have comment block*/

               else
                  ++tmpStr;

            } /*Loop: find end of block*/

            goto blockEnd_rmSpace_fun01_sec03_sub05;
         } /*If: have block comment*/

         else
         { /*Else: line comment (single line)*/
            if(
                  keepCommentBl
               && ! blockBl
            ) --tmpStr; /*get back onto comment*/

            while(
                  *tmpStr != '\n'
               && *tmpStr != '\r'
            ){ /*Loop: find end of comment*/
               if(*tmpStr == '\0') 
                  break;

               if(
                     keepCommentBl
                  && ! blockBl
               ){ /*If: keeping non-block comments*/
                  *dupStr++ = *tmpStr;
                  ++(*retLenULPtr);

                  if(lenUL && *retLenULPtr == lenUL)
                     break;
               } /*If: keeping non-block comments*/

               ++tmpStr;
            } /*Loop: find end of comment*/

            if(
                  ! keepCommentBl
               && ! blockBl
               && emptyBl
            ){ /*If: removing comment on empty line*/
               if(tmpStr[0] == '\r')
                  ++tmpStr;
               ++tmpStr; /*get off \n*/
               continue;
               /*line is only white space and comment,
               `  so I want to merge lines
               */
            } /*If: removing comment on empty line*/
         } /*Else: line comment (single line)*/

         continue;
      } /*If: have comment*/

      /**************************************************\
      * Fun01 Sec03 Sub06:
      *   - detect if working on a block
      \**************************************************/

      if(
            *(tmpStr) != '\\'
         && tmpStr[0] == '-'
         && tmpStr[1] == '-'
         && tmpStr[2] == '-'
         && tmpStr[3] != '-'
      ){ /*If: found a block marker*/
         blockBl = ! blockBl;
         tmpStr += 3; /*get off block*/

         if(! blockBl)
         { /*If: at end of block*/
            blockEnd_rmSpace_fun01_sec03_sub05:;

            scanStr = tmpStr;

            while(*tmpStr < 33)
            { /*Loop: remove extra white space*/
               if(*tmpStr == '\0')
                  break;

               if(*tmpStr == '\n')
                  break;

               ++tmpStr;
            } /*Loop: remove extra white space*/

            if(scanStr == tmpStr)
               continue; /*no white space at end*/

            scanStr = tmpStr;
            --tmpStr;

            while(*scanStr < 33)
            { /*Loop: check if have empty lines*/
               if(*scanStr == '\0')
               { /*If: end of string*/
                  tmpStr = scanStr;
                  break;
               } /*If: end of string*/

               if(*scanStr == '\n')
                  tmpStr = scanStr;

               ++scanStr;
            } /*Loop: check if have empty lines*/
         } /*If: at end of block*/

         continue;
      } /*If: found a block marker*/

      else if(! blockBl)
      { /*Else If: noto on a block*/
         if(tmpStr[0] == '\n')
            emptyBl = 1; /*reset white space for new line*/
         if(tmpStr[0] > 32)
            emptyBl = 0; /*if non-white space in line*/

         goto nextChar_fun01_sec03_sub04;
      } /*Else If: noto on a block*/

      /**************************************************\
      * Fun01 Sec03 Sub07:
      *   - merge lines in the bock + debuging white space
      \**************************************************/

      if(
            tmpStr[0] == '\n'
         || tmpStr[0] == '\r'
      ){ /*If: is new line in a block*/
         ++tmpStr;
         continue;
      } /*If: is new line in a block*/

      /*preserves indent white space in non-blocks*/
      if(
            tmpStr[0] <= 32
         && tmpStr[1] <= 32
         && tmpStr[1] != '\n'
      ){ /*If: extra white space*/
         while(tmpStr[1] <= 32)
         { /*Loop: remove extra white space*/
            if(tmpStr[0] == '\0')
               break;

            if(tmpStr[0] == '\n')
               break;

            ++tmpStr;
         } /*Loop: remove extra white space*/

         continue;
      } /*If: extra white space*/

      /**************************************************\
      * Fun01 Sec03 Sub08:
      *   - move to next characer in block
      \**************************************************/

      nextChar_fun01_sec03_sub04:;
         *dupStr++ = *tmpStr++;
         ++(*retLenULPtr);

         if(lenUL && *retLenULPtr == lenUL)
            break;
   } /*Loop: scan for blocks*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - return string without blocks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *dupStr = '\0';
   return retStr;
} /*str_rmBlocks*/

/*-------------------------------------------------------\
| Fun02: getBlock_rmBlocks
|   - gets next block/line as a single line
| Input:
|   - buffStrPtr:
|     o pointer to c-string to hold next block
|   - lenSLPtr:
|     o pointer to unsigned long with length of buffStrPtr
|   - commentBl:
|     o 0: discard line comments
|     o 1: keep line comments
|     o 2: keep and merge block comments
|     o 3: keep line comments and merged block comments
|   - trimWhiteBl:
|     o 0: do not trim white space
|     o 1: remove starting white space
|     o 2: remove ending white space
|     o 3: remove starting and ending white space
|   - inFILE:
|     o FILE pionter to file to get block from
| Output:
|   - Modifies:
|     o buffStrPtr to have next block/line from file
|     o retLenULPtr if buffStrPtr is resized (new length)
|     o inFILE to be on next block/line
|   - Returns:
|     o size of read in block/line
|     o def_memErr_rmBlocks for memory errors
|     o def_EOF_rmBlocks for end of file
\-------------------------------------------------------*/
signed long
getBlock_rmBlocks(
   signed char **buffStrPtr,/*buffer to hold block*/
   signed long *lenSLPtr, /*length of buffer*/
   signed char commentBl,  /*1: keep non-block comments*/
   signed char trimWhiteBl, /*trim start/end white space*/
   void *inFILE             /*file to get block from*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - converts blocks (#--- ---) to single lines
   '   o fun02 sec01:
   '     - check if buffer has memory
   '   o fun02 sec02:
   '     - allocated return string memory
   '   o fun02 sec03:
   '     - get block
   '   o fun02 sec04:
   '     - return string without blocks
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tmpStr = 0;
   signed char *dupStr = 0;

   unsigned char stringFlagUC = 0;
   signed char blockTypeSC = 0;
   signed long lenBlockSL = 0;
   signed char emptyBl = 0;
      /*marks if line is white space*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - check if buffer has memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! *buffStrPtr)
   { /*If: need to allocate buffer memory*/
      *lenSLPtr = 0;

      *buffStrPtr =
         malloc((128 + 8) * sizeof(signed char));

      if(! *buffStrPtr)
         goto memErr_fun02_sec04;

      *lenSLPtr = 128;
      dupStr = *buffStrPtr;
      tmpStr = *buffStrPtr;
   } /*If: need to allocate buffer memory*/

   else
   { /*Else: buffer has memory*/
      dupStr = *buffStrPtr;
      tmpStr = *buffStrPtr;
   } /*Else: buffer has memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - get block
   ^   o fun02 sec03 sub01:
   ^     - read in a single line
   ^   o fun02 sec03 sub02:
   ^     - scan line for blocks (multi line)
   ^   o fun02 sec03 sub03:
   ^     - check if on muli-line block/string + trim end
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec03 Sub01:
   *   - read in a single line
   \*****************************************************/

   dupStr = tmpStr;
   emptyBl = 1;

   nextLine_fun02_sec03_sub01:;

   tmpStr =
      (signed char *)
      fgets(
         (char *) dupStr,
         (*lenSLPtr - lenBlockSL),
         (FILE *) inFILE
      ); /*get first part of line*/

   if(! tmpStr)
      goto eof_fun02_sec04;

   tmpStr = &(*buffStrPtr)[lenBlockSL];
   lenBlockSL += endStr_ulCp(tmpStr);

   while(*tmpStr != '\n')
   { /*Loop: get full line*/
      nextLineLoop_fun02_sec03_sub01:;

      tmpStr =
         realloc(
            *buffStrPtr,
              (*lenSLPtr + (*lenSLPtr >> 1) + 8) 
            * sizeof(signed char)
         );

      if(! tmpStr)
         goto memErr_fun02_sec04;

      *buffStrPtr = tmpStr;
      dupStr = (*buffStrPtr) + (tmpStr - dupStr);
      tmpStr = &(*buffStrPtr)[lenBlockSL];
      lenBlockSL += endStr_ulCp(tmpStr);

      tmpStr =
         (signed char *)
         fgets(
            (char *) tmpStr,
            (*lenSLPtr - lenBlockSL),
            (FILE *) inFILE
         ); /*get first part of line*/
   } /*Loop: get full line*/

   /*****************************************************\
   * Fun02 Sec03 Sub02:
   *   - scan line for blocks (multi line)
   *     - deal whith comments + start loop
   *   o fun02 sec03 sub02 cat01:
   *     - move past (trim) starting white space (if trim)
   *   o fun02 sec03 sub02 cat02:
   *     - deal whith escaped values + start loop
   *   o fun02 sec03 sub02 cat03:
   *     - detect if in string, comment, or comment block
   *   o fun02 sec03 sub02 cat04:
   *     - if in comment bock; merge or remove
   *   o fun02 sec03 sub02 cat05:
   *     - if line comment; keep or discard
   *   o fun02 sec03 sub02 cat06:
   *     - detect white space
   *   o fun02 sec03 sub02 cat07:
   *     - detect strings (blocks kinda)
   *   o fun02 sec03 sub02 cat08:
   *     - detect blocks
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub02 Cat01:
   +   - move past (trim) starting white space (if trim)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = *buffStrPtr;

   if(trimWhiteBl & 1)
   { /*If: trimming starting white space*/
      while(*tmpStr < 32)
      { /*Loop: trim white space from start*/
         --lenBlockSL;
         ++tmpStr;
      } /*Loop: trim white space from start*/
   } /*If: trimming starting white space*/

   /*make sure at least on white space between lines.
   `   this is save because I remove a '\n' from each
   `   line always
   */
   if(! blockTypeSC) ;

   else if(
         (blockTypeSC & def_commentBlock_rmBlocks)
      && ( ! (commentBl & 2) )
   ) ; /*on comment block and not keeping*/

   else if(*dupStr != ' ')
      *dupStr++ = ' ';
      /*in multi-line, make sure at least on space*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub02 Cat02:
   +   - deal whith escaped values + start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   while(*tmpStr != '\0')
   { /*Loop: check for blocks*/

      if(blockTypeSC & def_commentBlock_rmBlocks)
         goto commentBlock_fun02_sub02_cat03;
         /*need to deal with comment blocks*/

      if(*tmpStr == '\\')
      { /*If: escaped value*/

         if(tmpStr[1] == '"')
         { /*If: escaping a quote*/
            /*very likely user is escaping for bash*/
            *dupStr++ = '\\';
            *dupStr++ = '"';
            tmpStr += 2;
            continue;
         } /*If: escaping a quote*/

         else if(tmpStr[1] == '\'')
         { /*Else If: escaping a quote*/
            /*very likely user is escaping for bash*/
            *dupStr++ = '\\';
            *dupStr++ = '\'';
            tmpStr += 2;
            continue;
         } /*Else If: escaping a quote*/

         else if(stringFlagUC)
         { /*If: escape in string*/
            *dupStr++ = *tmpStr++;
            continue;
         } /*If: escape in string*/

         if(tmpStr[1] == '\\')
         { /*If: escaping an escape*/
            *dupStr++ = '\\';
            --lenBlockSL;
            tmpStr += 2;
            continue;
         } /*If: escaping an escape*/

         else if(tmpStr[1] == '#')
         { /*Else If: escaping a comment*/
            *dupStr++ = '#';
            --lenBlockSL;
            tmpStr += 2;
            continue;
         } /*Else If: escaping a comment*/

         else if(tmpStr[1] == '-')
         { /*Else If: escaping a dash*/
            ++tmpStr;
            --lenBlockSL;

            while(*tmpStr == '-')
               *dupStr++ = *tmpStr++;

            continue;
         } /*Else If: escaping a dash*/

         /*else: not an escape*/
      } /*If: escaped value*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun02 Sec03 Sub02 Cat03:
      +   - detect if in string, comment, or comment block
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*needed before comments because # in string is
      `   not a comment
      */
      if(stringFlagUC)
      { /*If: escape in string*/
         *dupStr++ = *tmpStr++;
         continue;
      } /*If: escape in string*/

      else if(*tmpStr == '#')
      { /*Else If: found a comment*/

         if(
                tmpStr[1] == '-'
             && tmpStr[2] == '-'
             && tmpStr[3] == '-'
             && tmpStr[4] != '-'
         ){ /*If: found comment block*/
            if( ! (blockTypeSC & def_block_rmBlocks) )
               --lenBlockSL;
            else if(commentBl & 2)
                *dupStr++ = '#';

            blockTypeSC ^= def_commentBlock_rmBlocks;
            tmpStr += 4;
            lenBlockSL -= 3;
            continue;
         } /*If: found comment block*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun02 Sec03 Sub02 Cat04:
         +   - if in comment bock; merge or remove
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(blockTypeSC & def_commentBlock_rmBlocks)
         { /*Else If: in comment block*/

            commentBlock_fun02_sub02_cat03:;

            if(blockTypeSC & def_block_rmBlocks)
            { /*If: comment is inside of block*/
                  ++tmpStr;
                  --lenBlockSL;
                  continue;
            } /*If: comment is inside of block*/

            else if(*tmpStr == '\n')
            { /*Else If: new line, need to remove*/
               ++tmpStr;
               --lenBlockSL;
               continue;
            } /*Else If: new line, need to remove*/

            else if(*tmpStr == '\r')
            { /*If: carriage return, need to remove*/
               ++tmpStr;
               --lenBlockSL;
               continue;
            } /*If: carriage return, need to remove*/

            else
            { /*Else: keep item*/
               *dupStr++ = *tmpStr++;
               continue;
            } /*Else: keep item*/
         } /*Else If: in comment block*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun02 Sec03 Sub02 Cat05:
         +   - if comment; deal with line comments
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(blockTypeSC & def_block_rmBlocks)
            goto nextLine_fun02_sec03_sub01;
            /*comment is emmbeded in a block*/
        
         else if(commentBl)
         { /*Else If: keeping line comments*/
            while(*tmpStr != '\0')
               *dupStr++ = *tmpStr++;
            goto ret_fun02_sec04;
            /*keeping comment*/
         } /*Else If: keeping line comments*/

         else
         { /*Else: removing comment*/
            *tmpStr = '\0';

            if(emptyBl)
               goto nextLine_fun02_sec03_sub01;
               /*comment is line + white space*/
            else
               goto ret_fun02_sec04;
               /*something else (non-block) in line*/
         } /*Else: removing comment*/
      } /*Else If: found a comment*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun02 Sec03 Sub02 Cat05:
      +   - detect white space
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/


      if(*tmpStr > 32)
         emptyBl = 0; /*line has non-white space*/

      else
      { /*Else: white space*/
         *dupStr++ = *tmpStr++;
         continue;
      } /*Else: white space*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun02 Sec03 Sub02 Cat06:
      +   - detect strings (blocks kinda)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(*tmpStr == '"')
      { /*If: have duoble quote string*/
         *dupStr++ = *tmpStr++;

         if(stringFlagUC == def_noStr_rmBlocks)
            stringFlagUC =
                 def_doubleQuote_rmBlocks
               | def_onStr_rmBlocks;
            /*starting a new string*/

         else if(stringFlagUC & def_doubleQuote_rmBlocks)
            stringFlagUC = def_noStr_rmBlocks;
            /*ending a string*/

         /*else: in single quoted string; do nothing*/

         continue;
      } /*If: have duoble quote string*/

      else if(*tmpStr == '\'')
      { /*Else If: single quote string*/
         *dupStr++ = *tmpStr++;

         if(stringFlagUC == def_noStr_rmBlocks)
            stringFlagUC =
                 def_singleQuote_rmBlocks
               | def_onStr_rmBlocks;
            /*starting a new string*/

         else if(stringFlagUC & def_singleQuote_rmBlocks)
            stringFlagUC = def_noStr_rmBlocks;
            /*ending a string*/

         /*else: in single quoted string; do nothing*/

         continue;
      } /*Else If: single quote string*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun02 Sec03 Sub02 Cat07:
      +   - detect blocks
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(tmpStr[0] == '-')
      { /*If: might be a block*/
         if(
                   tmpStr[1] == '-'
                && tmpStr[2] == '-'
                && tmpStr[3] == '-'
                && tmpStr[4] == '#'
         ){ /*If: found a block*/
            if(tmpStr[4] == '#')
            { /*If: comment block end (form 1)*/
               blockTypeSC &=
                  ~def_commentBlock_rmBlocks;

               tmpStr += 4;
               lenBlockSL -= 4;

               continue;
            } /*If: comment block end (form 1)*/

            else
            { /*Else: is a block*/
               blockTypeSC ^= def_block_rmBlocks;
               tmpStr += 3;
               lenBlockSL -= 3;

               continue;
            } /*Else: is a bock*/
         } /*If: found a block*/

         else
         { /*Else: not block*/
            while(*tmpStr == '-')
              *dupStr++ = *tmpStr++; /*get to end of -*/
            continue;
         } /*Else: not block*/
      } /*If: might be a block*/
   } /*Loop: check for blocks*/

   /*****************************************************\
   * Fun02 Sec03 Sub03:
   *   - check if on muli-line block/string + trim end
   \*****************************************************/

   if(stringFlagUC) ; /*in string, no trimming*/

   else if((trimWhiteBl & 2))
   { /*Else If: trimming ending white space*/
      while(*dupStr < 32)
         --dupStr;
   } /*Else If: trimming ending white space*/


   if(blockTypeSC | stringFlagUC)
   { /*If: on a multi-line item*/
      
      if(lenBlockSL >= *lenSLPtr)
         goto nextLineLoop_fun02_sec03_sub01;
         /*Will handel memory allocation*/

      goto nextLine_fun02_sec03_sub01;
   } /*If: on a multi-line item*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - return string without blocks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ret_fun02_sec04:;
      *dupStr = '\0';
      goto clean_fun02_sec04;

   memErr_fun02_sec04:;
      lenBlockSL = def_memErr_rmBlocks;
      goto clean_fun02_sec04;

   eof_fun02_sec04:;
      lenBlockSL = def_EOF_rmBlocks;
      goto clean_fun02_sec04;

   clean_fun02_sec04:;
      return lenBlockSL;
} /*getBlock_rmBlocks*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not possible, this code is under the
:   MIT license
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
