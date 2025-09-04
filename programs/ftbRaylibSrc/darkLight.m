/*this code is from github co-pilot, which
`  in turn called chatgpt 4. it is for
`  detecting light and dark mode for Mac
*/

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
