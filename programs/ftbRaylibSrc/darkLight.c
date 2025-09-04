/*this code is from github co-pilot, which
`  in turn called chatgpt 4. it is for
`  detecting light and dark mode for windows
*/

signed char
getMode_darkLight(
   void
){
   #ifdef WINDOWS
      HKEY hKey;
      DWORD value = 1;
      DWORD valueSize = sizeof(value);

      /*Open the registry key*/
      if(
         RegOpenKeyExA(
            HKEY_CURRENT_USER,
            "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
            0,
            KEY_READ,
            &hKey
         ) == ERROR_SUCCESS
      ){ /*If: can open theme registory*/
          if(
             RegQueryValueExA(
                hKey,
                "AppsUseLightTheme",
                NULL,
                NULL,
                (LPBYTE)&value,
                &valueSize
              ) == ERROR_SUCCESS
          ){ /*If: could get dark mode status*/
              RegCloseKey(hKey);

              if (value == 0)
                 goto retDark_fun001;
              else
                 goto retLight_fun001;
          }  /*If: could get dark mode status*/

          RegCloseKey(hKey);
      } /*If: can open theme registory*/

      goto retUnkown_fun001;
         /*could not get dark/light mode*/

      retUnkown_fun001:;
         return 0;
      retLight_fun001:;
         return 1;
      retDark_fun001:;
         return 2;
   #else
      return 0;
   #endif /*windows check*/
}/*getMode_darkLight*/
