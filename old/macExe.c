#include <stdlib.h>
#include <stdio.h>

int
main(
   int siArg,
   char *argAryStr[]
){
   signed char *pathHeapStr = 0;
   signed char *tmpStr = (signed char *) argAryStr[0];
   signed char *cpStr = 0;
   signed char *dupStr = 0;

   pathHeapStr = malloc(2048 * sizeof(signed char));

   if(! pathHeapStr)
   { /*If: memory error*/
      printf("memory error\n");
      return 0;
   } /*If: memory error*/

   while(*tmpStr != '\0')
      ++tmpStr;

   while(*tmpStr != '/')
   { /*Loop: get past file name*/
      --tmpStr;

      if(tmpStr == (signed char *) argAryStr[0])
         break;
   } /*Loop: get past file name*/

   dupStr = pathHeapStr;
   cpStr = (signed char *) argAryStr[0];

   *dupStr++ = 's';
   *dupStr++ = 'h';
   *dupStr++ = ' ';

   while(cpStr != tmpStr)
      *dupStr++ = *cpStr++;

   *dupStr++ = '/';
   *dupStr++ = 'm';
   *dupStr++ = 'a';
   *dupStr++ = 'c';
   *dupStr++ = 'I';
   *dupStr++ = 'n';
   *dupStr++ = 's';
   *dupStr++ = 't';
   *dupStr++ = 'a';
   *dupStr++ = 'l';
   *dupStr++ = 'l';
   *dupStr++ = '\0';

   system((char *) pathHeapStr);
   free(pathHeapStr);
   return 0;
} /*main*/
