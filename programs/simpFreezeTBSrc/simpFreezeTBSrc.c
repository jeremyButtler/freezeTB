/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' simpFreezeTB SOF: Start Of File
'   - simiplified one click freezeTB 
'   o main:
'     - driver frunctio to run freezeTB
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef PLAN9
	#include <u.h>
   #include <libc.h>
#else
   #include <stdlib.c>

   #ifdef WINDOWS
      #define def_fileSep_simpFreezeTB = '\';
   #else
      #define def_fileSep_simpFreezeTB = '/';
#endif

signed char
   *glob_inPathStr = (signed char *) "freezeTB_input";

signed char
   *glob_outPathStr = (signed char *) "freezeTB_output";

int
main(
){
   signed char *homePathStr = (schar *) getenv("HOME");
   signed char inPathStr[2048];
   signed char outPathStr[2048];
   signed char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - check if input/output directories exist
   ^   o main sec02 sub01:
   ^     - check if input path exists
   ^   o main sec02 sub02:
   ^     - check if output path exists
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - check if input path exists
   \*****************************************************/

   tmpStr = inPathStr;

   tmpStr +=
      cpLine_ulCp(
         tmpSTr,
         homePathStr
      );

   *tmpStr++ = def_fileSep_simpFreezeTB;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         "freezeTB_input"
      );

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - check if output path exists
   \*****************************************************/

   tmpStr = outPathStr;

   tmpStr +=
      cpLine_ulCp(
         tmpSTr,
         homePathStr
      );

   *tmpStr++ = def_fileSep_simpFreezeTB;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         "freezeTB_output"
      );
} /*main*/
