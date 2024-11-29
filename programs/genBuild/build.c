/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' build SOF: Start Of File
'   - holds functions and structures to build C programs
'   o header:
'     - included libraries
'   o fun03: sysCmd_build
'     - runs a system command
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
|   o header sec01:
|     - included libraries
|   o header sec02:
|     - build flags
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"

#include "osDetect.h"
#include "rmBlocks.h"
#include "stBuild.h"

/*.h file only (no .c or .c not needed)*/
#include "../genLib/dataTypeShortHand.h"

#include "buildSymbols.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

static signed char
   glob_endCmd_build =
   (signed char *) ";;";
   /*optional ending of a build command
   `  - using ;; to not interfere with bash commands
   */

/*internal build commands*/
static signed char
   glob_getCmdStr_build =
   (signed char *) "get";

static signed char
   glob_libIncludeStr_build =
   (signed char *) "include"

static signed char
   glob_libExlucdeStr_build =
   (signed char *) "exclude"

static signed char
   glob_noLib_build =
   (signed char *) "nolib"

static signed char
   glob_exitCmdStr_build =
   (signed char *) "exit";

static signed char
   glob_cmdSec_build =
   (signed char *) ">>>COMMANDS<<<";

static signed char
   glob_configSec_build =
   (signed char *) ">>>CONFIGURE<<<";

/*-------------------------------------------------------\
| Fun01: parseVarId_build
|   - parses a variable id
| Input:
|   - idStr:
|     o c-string to variable id to parse
| Output:
|   - Returns:
|     o pointer to oneVar_build struct with variable id
|     o 0 for memory errors
\-------------------------------------------------------*/
struct oneVar_build *
parseVarId_build(
   signed char *idStr,    /*variable id to parse*/
   signed char *errSCPtr  /*returned error type*/
){
   struct oneVar_build *retVarHeapST = 0;
   schar loopFlagSC = 0;

   #define fun01_numVarFlags_build 3
   schar dupArySC[fun01_numVarFlags_build];
      /*keep track of how many times filled each type*/

   retVarHeapST = malloc(sizeof(oneVar_build));

   if(! retVarHeapST)
      goto err_fun01_sec0x;

   init_oneVar_build(retVarHeapST);

   if( setup_oneVar_build(retVarHeapST) )
      goto memErr_fun01_sec0x;

   while(*idStr > 32)
   { /*Loop: copy variable id*/

      /*this point should clear errors if the strings
      `   were the correct length. Logic is bit and simd
      `   cases set up for an error and the error is
      `   cleared if a new marker is found.
      */
      if(*idStr == def_osMark_build)
      { /*If: changing to OS id*/
         loopFlagSC = 1;
         ++idStr;
         ++dupAryStr[0];
         continue;
      } /*If: changing to OS id*/

      if(*idStr == def_bitMark_build)
      { /*If: changing to cpu bit type*/
         loopFlagSC = 2;
         ++dupAryStr[1];
         continue;
      } /*If: changing to cpu bit type*/

      if(*idStr == def_vectMark_build)
      { /*If: changing to simd type*/
         loopFlagSC = 3;
         ++dupAryStr[2];
         continue;
      } /*If: changing to simd type*/

      switch(loopFlagSC)
      { /*Switch: check part of id on*/
         case 0:
         /*Case 0: on variable id*/
            idStr +=
               addId_var_stBuild(
                  retVarHeapST,
                  idStr,
                  &errSC
               );

            if(errSC)
            { /*If: had error*/
               if(errSC == def_memErr_stBuild)
                  goto memErr_fun01_sec0x;
               else
                  goto invalidVar_fun01_sec0x;
            } /*If: had error*/

            break;
         /*Case 0: on variable id*/

         case 1:
         /*Case 1: on os id*/
            break;
         /*Case 1: on os id*/

         case 2:
         /*Case 2: on cpu bit type*/
            if(idStr[0] == '8')
            { /*If: 8 bit (build will never work)*/
               ++idStr;
               loopFlagSC = 5; /*set up for error*/
               retVarHeapST.bitSC = def_8bit_osDetect;
            } /*If: 8 bit (build will never work)*/

            else if(
                  idStr[0] == '1'
               && idStr[1] == '6'
            ){ /*If: 16 bit (will build work?)*/
               idStr += 2;
               loopFlagSC = 5; /*set up for error*/
               retVarHeapST.bitSC = def_16bit_osDetect;
            } /*If: 16 bit (will build work?)*/

            else if(
                  idStr[0] == '3'
               && idStr[1] == '2'
            ){ /*If: 32 bit*/
               idStr += 2;
               loopFlagSC = 5; /*set up for error*/
               retVarHeapST.bitSC = def_32bit_osDetect;
            } /*If: 32 bit*/

            else if(
                  idStr[0] == '6'
               && idStr[1] == '4'
            ){ /*If: 64 bit*/
               idStr += 2;
               loopFlagSC = 5; /*set up for error*/
               retVarHeapST.bitSC = def_64bit_osDetect;
            } /*If: 64 bit*/

            else
            ){ /*If: no idea; assume any cpu bit type*/
               ++idStr;
               loopFlagSC = 4; /*move to next entry*/
               retVarHeapST.bitSC = 0;
            } /*If: 64 bit*/

            break;
         /*Case 2: on cpu bit type*/

         case 3:
         /*Case 3: on simd type*/
            if(
               idStr[0] == 'n'
               idStr[1] == 'e'
               idStr[2] == 'o'
               idStr[3] == 'n'
            ){ /*If: neon variable*/
               idStr += 4;
               loopFlagSC = 5; /*set flag for error*/
               retVarHeapST.simdSC = def_neon_osDetect;
            } /*If: neon variable*/

            else if(
               idStr[0] == 's'
               idStr[1] == 's'
               idStr[2] == 'e'
            ){ /*Else If: sse simd variable*/
               idStr += 3;
               loopFlagSC = 5; /*set flag for error*/
               retVarHeapST.simdSC = def_sse_osDetect;
            } /*Else If: sse simd variable*/

            else if(
               idStr[0] == 's'
               idStr[1] == 's'
               idStr[2] == 'e'
               idStr[3] == '2'
            ){ /*Else If: sse2 simd variable*/
               idStr += 4;
               loopFlagSC = 5; /*set flag for error*/
               retVarHeapST.simdSC = def_sse2_osDetect;
            } /*Else If: sse2 simd variable*/

            else if(
               idStr[0] == 's'
               idStr[1] == 's'
               idStr[2] == 'e'
               idStr[3] == '4'
            ){ /*Else If: sse4 simd variable*/
               idStr += 4;
               loopFlagSC = 5; /*set flag for error*/
               retVarHeapST.simdSC = def_sse4_osDetect;
            } /*Else If: sse4 simd variable*/

            else if(
               idStr[0] == 'a'
               idStr[1] == 'v'
               idStr[2] == 'x'
               idStr[3] == '2'
            ){ /*Else If: avx2 simd variable*/
               idStr += 4;
               loopFlagSC = 5; /*set flag for error*/
               retVarHeapST.simdSC = def_avx2_osDetect;
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
               loopFlagSC = 4; /*set flag for error*/
               retVarHeapST.simdSC = def_avx512_osDetect;
            } /*Else If: avx512 simd variable*/

            else
            { /*Else: assuming scalar*/
               ++idStr;
               loopFlagSC = 4; /*move to next entry*/
               retVarHeapST.simdSC = 0;
            } /*Else: assuming scalar*/

            break;
         /*Case 3: on simd type*/

         case 4:
         /*Case 4: moving to next type*/
            ++idStr;
            break;
         /*Case 4: moving to next type*/

         case 5:
         /*Case 5: moving to next type*/
            goto invalidVar_fun01_sec0x;
            break;
         /*Case 5: moving to next type*/
      } /*Switch: check part of id on*/

      ++idStr;
   } /*Loop: copy variable id*/

   for(
      loopFlagSC = 0;
      loopFlagSC < fun01_numVarFlags_build;
      ++loopFlagSC
   ){ /*Loop: check if had duplicate entries*/
      if(dupArySC[loopFlagSC] > 1)
         goto invalidVar_fun01_sec0x;
   } /*Loop: check if had duplicate entries*/

   goto ret_fun01_sec0x;

   memErr_fun01_sec0x:;
      *errSCPtr = def_memErr_build;
      goto errCleanUp_fun01_sec0x:;

   invalidVar_fun01_sec0x:;
      *errSCPtr = def_noVar_build;
      goto errCleanUp_fun01_sec0x:;

   errCleanUp_fun01_sec0x:;
      if(retVarHeapST)
         freeHeap_oneVar_build(retVarHeapST);
      retVarHeapST = 0;
      goto ret_fun01_sec0x;

   ret_fun01_sec0x:;
      return retVarHeapST;
} /*parseVarId_build*/

/*-------------------------------------------------------\
| Fun02: parseVar_build
|   - parses variable to have sub variables filled in
| Input:
|   - valueStr:
|     o c-string with values (contents) of variable to
|       parse
|   - osStr:
|     o list of OS's trying targeting
|   - simdSC:
|     o simd flag using; see osDetect.h for list
|   - bitSC
|     o CPU bit (ex 64bit is def_64bit_osDetect); see
|       osDetect.h for list
|   - varInputSTPtr:
|     o pionter to a varInput_stBuild struct with user
|       input (zero (top) priority)
|   - primVarsSTPtr:
|     o pointer to var_stBuild structure with variables
|       having first priority
|   - secVarSTPtr:
|     o pointer to var_stBuild structure with variables
|       having second priority
|   - lenSLPtr:
|     o pointer to signed long to have variable length
|   - errSCPtr:
|     o poitner to signed char to hold errors
| Output:
|   - Modifies:
|     o lenSLPtr to have length of retuned c-string
|     o errSCPtr:
|       - 0 for no errors
|       - def_memErr_build for memory errors
|       - def_noVar_build if at least one interal
|         variable is missing
|   - Returns
|     o pointer to c-string with parsed variable contents
|     o 0 for error
\-------------------------------------------------------*/
signed char
parseVar_build(
   signed char *valueStr,  /*variable value to parse*/
   signed char *osStr[],  /*OS list to target*/
   signed char simdSC,    /*simd type using*/
   signed char bitSC,     /*cpu bit type*/
   struct varInput_stBuild *inputSTPtr, /*user variables*/
   struct var_stBuild *primVarsSTPtr,   /*primary vars*/
   struct var_stBuild *secVarsSTPtr,    /*secondary vars*/
   signed long *lenSLPtr,  /*length of returned c-string*/
   signed char *errSCPtr   /*has errors*/
){
   schar errSC =0;
   schar *retHeapStr = 0;
   slong sizeSL = 0;
   slong indexSL = 0;
   schar *tmpStr = 0;

   struct oneVar_build *oneVarHeapST = 0;

   *lenSLPtr = 0;

   while(*valueStr != '\0')
   { /*Loop: parse variable*/

      if((*lenSLPtr + 3) >= sizeSL)
      { /*If: need to resize the variable string*/
         sizeSL += (sizeSL >> 1);

         tmpStr =
            realloc(
               retHeapStr,
               (sizeSL + 1) * sizeof(schar)
            );

         if(! tmpStr)
            goto memErr_fun02_sec0x;

          retHeapStr = tmpStr;
      } /*If: need to resize the variable string*/

      if(
            valueStr[0] == def_escape_build
         && valueStr[1] == def_varAppend_build
         && valueStr[2] == def_buildSymbol_build
      ){ /*If: next value is escaped variable access*/
            *retHeapStr++ = def_escape_build;
            *retHeapStr++ = def_varAppend_build;
            *retHeapStr++ = def_buildSymbol_build;
            valueStr += 3;
            continue;
      } /*If: next value is escaped variable access*/

      if(
            valueStr[0] == def_varAppend_build
         && valueStr[1] == def_buildSymbol_build
      ){ /*If: copying variable*/
         oneVarHeapST =
            parseVarId_build(
               valueStr,    /*variable id to parse*/
               &errSC       /*returned error type*/
            );


         if(oneVarHeapST)
            freeHeap_oneVar_build(oneVarHeapST);
         oneVarHeapST = 0;
      } /*If: copying variable*/
   } /*Loop: parse variable*/

   if(oneVarHeapST)
      freeHeap_oneVar_build(oneVarHeapST);
   oneVarHeapST = 0;

   if(tmpHeapStr)
      free(tmpHeapStr);
   tmpHeapStr = 0;
} /*parseVar_build*/

/*-------------------------------------------------------\
| Fun03: sysCmd_build
|   - runs a system command
| Input:
|   - varStr:
|     o c-string with variable id of system command to run
|   - osStr:
|     o list of OS's trying targeting
|   - simdSC:
|     o simd flag using; see osDetect.h for list
|   - bitSC
|     o CPU bit (ex 64bit is def_64bit_osDetect); see
|       osDetect.h for list
|   - varInputSTPtr:
|     o pionter to a varInput_stBuild struct with user
|       input (zero (top) priority)
|   - primVarsSTPtr:
|     o pointer to var_stBuild structure with variables
|       having first priority
|   - secVarSTPtr:
|     o pointer to var_stBuild structure with variables
|       having second priority
| Output:
|   - Returns
|     o 0 if command succeded
|     o def_noVar_build if could not find variable with
|       system command
|     o def_cmdErr_build if system command failed to fire
\-------------------------------------------------------*/
signed char
sysCmd_build(
   signed char *varStr,   /*variable id to hunt for*/
   signed char *osStr[],  /*OS list to target*/
   signed char simdSC,    /*simd type using*/
   signed char bitSC,     /*cpu bit type*/
   struct varInput_stBuild *inputSTPtr, /*user variables*/
   struct var_stBuild *primVarsSTPtr,   /*primary vars*/
   struct var_stBuild *secVarsSTPtr     /*secondary vars*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 sysCmd_build
   '   - launch a system command
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - find variable with command
   '   o fun03 sec03:
   '     - run command
   '   o fun03 sec04:
   '     - return result of command
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *valueStr = 0;   /*pionts to variable contents*/
   schar *cmdHeapStr = 0; /*has command to fire*/
   slong indexSL = 0;
   schar errSC = 0;       /*error reporting*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - find variable with command
   ^   o fun03 sec02 sub01:
   ^     - check user input for variable
   ^   o fun03 sec02 sub02:
   ^     - check primary variable array for variable
   ^   o fun03 sec02 sub03:
   ^     - check secondary variable array for variable
   ^   o fun03 sec02 sub04:
   ^     - handel variable not found cases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - check user input for variable
   \*****************************************************/

   if(inputSTPtr)
   { /*If: input array provided*/
      indexSL =
         findVar_varInput_stBuild(
            inputSTPtr,
            varStr,
            osStr,
            simdSC,
            bitSC
         ); /*see if user provided this variable*/

      if( indexSL >= 0 )
      { /*If: user provided this variable*/
         valueStr = inputSTPtr->valAryStr[indexSL];
         goto fireCmd_fun03_sec03;
      } /*If: user provided this variable*/
   } /*If: input array provided*/

   /*****************************************************\
   * Fun03 Sec02 Sub02:
   *   - check primary variable array for variable
   \*****************************************************/

   if(primVarsSTPtr)
   { /*If: primary variable array provided*/
      indexSL =
         findVar_var_stBuild(
           primVarsSTPtr,
           valueStr,
           osStr,
           simdSC,
           bitSC
         ); /*see if variable in primary array*/

      if( indexSL >= 0 )
      { /*If: user provided this variable*/
         valueStr = primVarsSTPtr->valAryStr[indexSL];
         goto fireCmd_fun03_sec03;
      } /*If: user provided this variable*/
   } /*If: primary variable array provided*/

   /*****************************************************\
   * Fun03 Sec02 Sub03:
   *   - check secondary variable array for variable
   \*****************************************************/

   if(secVarsSTPtr)
   { /*If: secondary variable array provided*/
      indexSL =
         findVar_var_stBuild(
           secVarsSTPtr,
           valueStr,
           osStr,
           simdSC,
           bitSC
         ); /*see if variable in secary array*/

      if( indexSL >= 0 )
      { /*If: user provided this variable*/
         valueStr = secVarsSTPtr->valAryStr[indexSL];
         goto fireCmd_fun03_sec03;
      } /*If: user provided this variable*/
   } /*If: secondary variable array provided*/

   /*****************************************************\
   * Fun03 Sec02 Sub04:
   *   - handel variable not found cases
   \*****************************************************/

   goto noVar_fun03_sec04:;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - run command
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fireCmd_fun03_sec03:;

   cmdHeapStr =
      parseVar_build(
         valueStr,       /*variable with command to run*/
         osStr,          /*OS list to target*/
         simdSC,         /*simd type using*/
         bitSC,          /*cpu bit type*/
         inputSTPtr,     /*user variables*/
         primVarsSTPtr,  /*primary vars*/
         secVarsSTPtr,   /*secondary vars*/
         &indexSL,       /*will get new string length*/
         &errSC          /*holds errors*/
      ); /*build the system command*/

   if(errSC)
   { /*If: had error*/
      if(errSC == def_memErr_build)
         goto memErr_fun03_sec04;
      else
         goto noVar_fun03_sec04;
        /*at least one variable missing*/
   } /*If: had error*/

   /*run command*/
   if(system(cmdStr))
      goto cmdErr_fun03_sec04;

   free(cmdHeapStr);
   cmdHeapStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return result of command
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec04;

   memErr_fun03_sec04:;
      errSC = def_memErr_build;
      goto ret_fun03_sec04:;

   noVar_fun03_sec04:;
      errSC = def_noVar_build;
      goto ret_fun03_sec04;

   cmdErr_fun03_sec04:;
      errSC = def_cmdErr_build;
      goto ret_fun03_sec04;

   ret_fun03_sec04:;
      if(cmdHeapStr)
         free(cmdHeapStr);
      cmdHeapStr = 0;

      return errSC;
} /*sysCmd_build*/

/*-------------------------------------------------------\
| Fun0x: parseFile_build
|   - parses a file for build | Input:
|   - inFILE:
|     o pointer FILE with file to parse
|   - osAryStr:
|     o pointer to c-string array with OS list
|       - use '\0' to mark no OS for a level
|       - needs to have at least def_osDepth_osDetect
|         elements
|   - simdSC:
|     o has smid flag (see osDetect.h), use 0 for no simd
|   - bitSC:
|     o flag for 64 bit, 32 bit, 16 bit, or 8 bit cpu
|       - see def_*bit_osDectect defines in osDetect.h
|   - skipLibBl:
|     o 1: do not include any libraries
|     o 0: included libraries (withing settings)
|   - includeAryStr:
|     o string array of libraries to include
|   - excludeAryStr:
|     o string array of libraries to exlude
|   - errSCPtr:
|     o pointer to signed char to hold errors
| Output:
|   - Modifies:
|     o inFILE to be at last line in file
|     o errSCPtr:
|       - 0 for no errors
|       - def_eof_build for end of file
|       - def_memErr_bulid for memory errors
|       - def_fileErr_bulid if no file input and no buffer
|         input
|       - def_exit_bulid if user wanted exit command
|       - def_sysErr_bulid if user had system error
|   - Returns:
|     o pionter to file_build struct with file contents
|     o 0 for errors
\-------------------------------------------------------*/
struct file_stBuild *
parseFile_build(
   FILE *inFILE,
   signed char *osarystr[],      /*oss looking for*/
   signed char simdsc,           /*simd building for*/
   signed char bitSC,            /*cpu bit (64/32/16/8)*/
   signed char skipLibBl,
   signed char *includeAryStr[],
   signed char *exludeAryStr[],
   schar *errSCPtr /*holds errors*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun0x TOC:
   '   - get line or lines in command for build
   '   o fun0x sec01:
   '     - variable declarations
   '   o fun0x sec02:
   '     - memory allocation
   '   o fun0x sec03:
   '     - read in file and pre-process contents
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define def_buffInc_fun0x 4096  /*buffer size*/
   #define posInc_fun0x 16     /*number sub-blocks*/

   schar appendBl = 0;         /*appending to variable*/

   ulong ulGetCmd = (ulong *) glob_getCmdStr_build;
   ulong ulExitCmd = (ulong *) glob_exitCmdStr_build;
   ulong ulBlock = (ulong *) glob_blockStr_build;

   schar *tmpStr = 0;
   schar *dupStr = 0;     /*for c-string copying*/
   schar *cpStr = 0;     /*for c-string copying*/

   schar *inHeapStr = 0;  /*has input from file*/
   ulong lenInUL = 0;
   ulong sizeInUL  = 0;

   ulong bytesUL = 0;

   uint blockHistUI[4096]; /*indexs of subsections on*/
   uint depthUI = 0;       /*depth at in block(s)*/
   uint commentBl = 0;     /*comment block*/

   /*for error reporting*/
   ulong charUL = 0;  /*byte error was on in file*/
   ulong lineUL = 0;  /*line at in file*/
   ulong colUL = 0;   /*column at in file*/

   struct file_stBuild *fileHeapST = 0;
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec02:
   ^   - memory setup (initial allocation)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   inHeapStr =
      calloc(
         def_buffInc_fun0x + 16,
         sizeof(schar)
      );

   sizeInUL = def_buffInc_fun0x;

   if(! inHeapStr)
      goto memErr_fun0x_sec0x;


   fileHeapST = malloc(sizeof(file_stBuild));

   if(! fileHeapST)
      goto memErr_fun0x_sec0x;

   init_file_stBuild(fileHeapST);
   fileHeapST->sizeFileUL = 0;

   if(inFILE == 0)
      goto fileErr_fun0x_sec0x;
      /*nothing in buffStrPtr*/
    
   fileHeapST->fileStr =
      malloc((def_buffInc_fun0x + 16) * sizeof(schar))

   fileHeapST->sizeFileUL = def_buffInc_fun0x;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec03:
   ^   - read in file and pre-process contents
   ^   o fun0x sec03 sub01:
   ^     - read in file contents
   ^   o fun0x sec03 sub02:
   ^     - pre-process file to remove blocks and comments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun0x Sec03 Sub01:
   *   - read in file contents
   \*****************************************************/

   bytesUL =
      fread(
         inHeapStr,
         sizeof(schar),
         def_buffInc_fun0x,
         inFILE
      );

   if(! bytesUL)
      goto fileErr_tof08_fun0x_sec0x;
      /*nothing in file*/

   while(bytesUL)
   { /*Loop: read in file*/
      lenInUL += bytesUL;
      sizeInUL += def_buffInc_fun0x;

      tmpStr =
         realloc(
            inFileStr,
            (sizeInUL + 16) * sizeof(schar)
         ); /*resize buffer for next read*/

      if(! tmpStr)
         goto memErr_tof08_fun0x_sec0x;
         /*memory error*/

      inHeapStr = tmpStr;

      bytesUL =
         fread(
            &inHeapStr[inLenUL],
            sizeof(schar),
            def_buffInc_fun0x,
            inFILE
         );
   } /*Loop: read in file*/

   inHeapStr[inLenUL] = '\0';

   /*****************************************************\
   * Fun0x Sec03 Sub02:
   *   - pre-process file to remove blocks and comments
   \*****************************************************/

   str_rmBlocks(
      inHeapStr,
      0,        /*want to modify inHeapStr*/
      &lenInUL, /*new length of string*/
      0         /*0 means remove comments*/
   ); /*remove comments and blocks from file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec04:
   ^   - process file
   ^   o fun0x sec04 sub01:
   ^     - set up + start loop
   ^   o fun0x sec04 sub02:
   ^     - check if have comment or if escaped #
   ^   o fun0x sec04 sub03:
   ^     - interal commands
   ^   o fun0x sec04 sub04:
   ^     - system commands
   ^   o fun0x sec04 sub0x:
   ^     - at commands block
   ^   o fun0x sec04 sub0z:
   ^     - deal with white space
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun0x Sec04 Sub01:
   *   - set up + start loop
   \*****************************************************/

   tmpStr = inHeapStr;

   while(*tmpStr != '\0')
   { /*Loop: find blocks*/
      ++charUL;
      ++colUL;

      if(*tmpStr == def_escape_build)
      { /*If: user trying to escape a build command*/
         fprintf(
            stderr,
            "escaping build variables only works for\n"
         );

         fprintf(
            stderr,
            "  setting variables, comment blocks, and"
         );

         fprintf(
            stderr,
            "  system commands"
         );

         goto syntaxErr_tof08_fun0x_sec0x;
      } /*If: user trying to escape a build command*/

      /**************************************************\
      * Fun0x Sec04 Sub02:
      *   - variables
      \**************************************************/

      /*at this point all variables are global*/
      if(
            *tmpStr == def_varSet_build
         || *tmpStr == def_globVarSet_build
      ){ /*If: setting up a varaible*/
         ++tmpStr;

         if(*tmpStr == def_varAppend_build)
         { /*If: appending to variable*/
            appendBl = 1;
            ++tmpStr;
         } /*If: appending to variable*/

         else
            appendBl = 0;

         while(tmpStr > 33)
         { /*Loop: read in variable metadata*/
            if(*tmpStr == def_osMark_build)
            { /*If: found os name*/
               while(*tmpStr)
            } /*If: found os name*/

            ++tmpStr;
         } /*Loop: read in variable metadata*/

      } /*If: setting up a varaible*/

      /**************************************************\
      * Fun0x Sec04 Sub03:
      *   - interal commands
      \**************************************************/

      /**************************************************\
      * Fun0x Sec04 Sub04:
      *   - system commands
      \**************************************************/

      /**************************************************\
      * Fun0x Sec04 Sub0x:
      *   - at commands block
      \**************************************************/

      dupStr = tmpStr;
      cpStr = glob_cmdBlockStr;

      while(*dupStr++ == *cpStr++) ;

      if(
            *dupStr <= 32
         && *cpStr == '\0'
      ){ /*If: at commands block*/
         tmpStr = dupStr;

         if(*dupStr != '\0')
            ++tmpStr;

         break;
      } /*If: at commands block*/

      /**************************************************\
      * Fun0x Sec04 Sub0z:
      *   - deal with white space
      \**************************************************/

      while(
            *tmpStr < 33 /*all white space is < 33*/
         && *tmpStr > 0  /*0 == '\0'*/
      ) ++tmpStr; /*find end of white space*/
   } /*Loop: find blocks*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec03:
   ^   - get past blank lines
   ^   o fun0x sec03 sub01:
   ^     - get past white space
   ^   o fun0x sec03 sub02:
   ^     - see if have multi-line entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun0x Sec03 Sub01:
   *   - get past white space
   \*****************************************************/

   fileHeapST->[def_buffInc_fun0x] = '\0';
   fileHeapST->[def_buffInc_fun0x + 1] = '\0';
   fileHeapST->[def_buffInc_fun0x + 2] = '\0';
   fileHeapST->[def_buffInc_fun0x + 3] = '\0';
   fileHeapST->[def_buffInc_fun0x + 4] = '\0';
   fileHeapST->[def_buffInc_fun0x + 5] = '\0';
   fileHeapST->[def_buffInc_fun0x + 6] = '\0';
   fileHeapST->[def_buffInc_fun0x + 7] = '\0';
   fileHeapST->[def_buffInc_fun0x + 8] = '\0';
   fileHeapST->[def_buffInc_fun0x + 9] = '\0';
   fileHeapST->[def_buffInc_fun0x + 10] = '\0';
   fileHeapST->[def_buffInc_fun0x + 11] = '\0';
   fileHeapST->[def_buffInc_fun0x + 12] = '\0';
   fileHeapST->[def_buffInc_fun0x + 13] = '\0';
   fileHeapST->[def_buffInc_fun0x + 14] = '\0';
   fileHeapST->[def_buffInc_fun0x + 15] = '\0';
   bytesLeftUL = blockStPtr->sizeBlockUI;
   tmpStr = fileHeapST->blockStr;

   while(
      fgets(
         (char *) fileHeapST->blockStr,
         bytesLeftUL,
         inFILE
      ) /*get next line*/
   ){ /*Loop: move past blank lines at start*/
      ulStr = (ulong *) tmpStr;

      while(*tmpStr != '\0')
      { /*Loop: find end of buffer*/
         while(
               *tmpStr == ' '
            || *tmpStr == '\t'
         ) ++tmpStr;

         ulStr = (ulong *) tmpStr;

         if(ifEndLine_ulCp(ulStr))
         { /*If: near end of line*/
            for(
               errSC = 0;
               errSC < sizeof(ulong);
               ++errSC
            ){ /*Loop: see if have end of line*/
               if(*tmpStr > 32)
                  break;
               else
                  ++tmpStr;
            } /*Loop: see if have end of line*/

            ulStr = (ulong *) tmpStr;
            ulDup = (ulong *) fileHeapST->blockStr;
            *ulDup = *ulStr;

            errSC = sizeof(ulong) - errSC;
            tmpStr = fileHeapST->blockSt + errSC;
            bytesLeftUL = (ulong) errSC;

            tmpStr =
               fgets(
                  (char *) tmpStr,
                  bytesLeftUL,
                  inFILE
               ) /*get next line*/

            if(! tmpStr)
               goto eof_fun0x_sec0x; /*end of flie*/

            if(errSC)
               break; /*if found end of white space*/
            else
               continue;
         } /*If: near end of line*/

        else
           break; /*have line with value*/
   } /*Loop: move past blank lines at start*/

   /*****************************************************\
   * Fun0x Sec03 Sub02:
   *   - see if have multi-line entry
   \*****************************************************/

   if(
         tmpStr[0] == '-'
      && tmpStr[1] == '-'
      && tmpStr[2] == '-'
   ){ /*If: have a multi-line block*/
      blockBl = 1; /*multi line block*/
      tmpStr += 3; /*move off block start*/
   } /*If: have a multi-line block*/

   else
      blockBl = 0; /*block is on a single line*/

   if(tmpStr != fileHeapST->blockStr)
   { /*If: had white space or multi line block*/
      ulStr = (ulong *) tmpStr;
      ulDup = (ulong *) fileHeapST->blockStr;

      whlie(! ifEndStr_ulCp(ulStr))
         *ulDup++ = *ulStr++;

      *ulDup = *ulStr;
         /*oversized string, so worst case '\0's copied*/

      tmpStr = (schar *) ulStr;

      while(*tmpStr != '\0')
         ++tmpStr; /*find true end of string*/

      bytesInBuffUL -= (tmpStr - fileHeapST->blockStr);
      tmpStr = fileHeapST->blockStr

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec03:
   ^   - get full length entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(
            tmpStr[0] == '-'
         && tmpStr[1] == '-'
         && tmpStr[2] == '-'
      ){ /*If: have a multi-line block*/
         if(
               blockDepthUI
            && fileHeapST->numPosUI
         ){ /*If: is a sub block of a sub block*/
            fileHeapST->indexAryUI =
               bockSTPtr->numPosUI - 1;
               /*is sub block of a sub block*/
         } /*If: is a sub block of a sub block*/

         else
            fileHeapST->indexAryUI = -1;/*only sub block*/

         ++blockDepthUI;
         ++blockOnUI

         fileHeapST->startAryUI =
            tmpStr - bockSTPtr->blockStr;

         fileHeapST->startAryUI = bockSTPtr->numPosUI;

         ++fileHeapST->numPosUI;

         blockBl = 1; /*multi line block*/
         tmpStr += 3; /*move off block start*/
      } /*If: have a multi-line block*/

      if(tmpStr != fileHeapST->blockStr)
      { /*If: had white space or multi line block*/
         ulStr = (ulong *) tmpStr;
         ulDup = (ulong *) fileHeapST->blockStr;

         whlie(! ifEndStr_ulCp(ulStr))
            *ulDup++ = *ulStr++;

         *ulDup = *ulStr;
            /*oversized string, so worst case '\0's copied*/

         tmpStr = (schar *) ulStr;

         while(*tmpStr != '\0')
            ++tmpStr; /*find true end of string*/

         bytesInBuffUL -= (tmpStr - fileHeapST->blockStr);
         tmpStr = fileHeapST->blockStr
      } /*If: had white space or multi line block*/

   goto ret_tof08_fun0x_sec0x;

   memErr_tof08_fun0x_sec0x:;
      *errSCPtr = def_memErr_build;
      goto cleanErr_tof08_fun0x_sec0x;

   fileErr_tof08_fun0x_sec0x:;
      *errSCPtr = def_fileErr_build;
      goto cleanErr_tof08_fun0x_sec0x;

   syntaxErr_tof08_fun0x_sec0x:;
      fprintf(
         stderr,
         "file error on byte %lu; line %lu; column %lu\n",
         charUL,
         lineUL,
         colUL
      );

      *errSCPtr = def_fileErr_build;
      goto cleanErr_tof08_fun0x_sec0x;


   cleanErr_tof08_fun0x_sec0x:;
      if(fileHeapST)
         free_file_build(fileHeapST);
      fileHeapST = 0;

      goto ret_tof08_fun0x_sec0x;

   ret_tof08_fun0x_sec0x:;
      if(inHeapStr)
         free(inHeapStr);
      inHeapStr = 0;

      return fileHeapST;
}
