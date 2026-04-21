/*this code is from github co-pilot, which
`  in turn called chatgpt 4. it is for
`  detecting light and dark mode for Mac
`I would like to use cc0, but co-pilot makes me unsure
`  if I can use any license
*/

#ifdef NO_AI
   signed char
   getMode_darkLight(
      void
   ){
      #ifdef DARK_MODE
         return 2;
      #else
         return 0;
      #endif
   }/*getMode_darkLight*/
#else
   #import <Cocoa/Cocoa.h>
   
   signed char
   getMode_darkLight(
      void
   ){
      NSString *style = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];
       if (style && [style isEqualToString:@"Dark"]) {
           return 2;
       } else {
           return 1;
       }
   } /*getMode_darkLight*/
#endif
