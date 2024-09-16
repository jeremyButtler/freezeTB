##########################################################
# Name: rDepends
#   - Installs R dependencies for freezeTb
##########################################################

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o header:
#     - Set up the local Rlibrary path
#   o fun-01: checkPkg
#     - Checks if an package is installed
#   o fun-02: installPkg
#     - Installs an package if it is not already installed
#   o main:
#     - Install packages
#   o license:
#     - Licensing for this code (public domain / mit)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# Header:
#   - Set up the local Rlibrary path
#   - This trick is from
#     https://stackoverflow.com/questions/39879424/automatically-create-personal-library-in-r
#---------------------------------------------------------

localLibStr = Sys.getenv("R_LIBS_USER");

dir.create(
   path = localLibStr,
   showWarnings = FALSE,
   recursive = TRUE
);

#---------------------------------------------------------
# Fun-01: checkPkg
#   - Checks if an package is installed
# Input:
#   - pkgStr:
#     o String with package to check if installed
# Output:
#   - Returns:
#     o TRUE if an package is installed
#     o FALSE if an package is not installed
# This trick is from:
#   https://stackoverflow.com/questions/15155814/check-if-r-package-is-installed-then-load-library
#---------------------------------------------------------
checkPkg =
function(
   pkgStr
){
   return(pkgStr %in% rownames(installed.packages()));
} # checkPkg

#---------------------------------------------------------
# Fun-02: installPkg
#   - Installs an package if it is not already installed
# Input:
#   - pkgStr:
#     o String with package to install
# Output:
#   - Prints:
#     o Install stats/problems to stdout
#   - Returns:
#     o TRUE if package was installed or already installed
#     o FALSE if an package could not be installed
#---------------------------------------------------------
installPkg =
function(
   pkgStr
){

   if(! checkPkg(pkgStr)){
      print(paste("Installing", pkgStr, sep = " "));  
   
      install.packages(
         pkgStr,
         repos="http://cran.us.r-project.org"
      );
   
      if(! checkPkg(pkgStr)){
   
         print(
            paste(
               "Failed to install", pkgStr,
               "; trying local install",
               sep = " "
            )
         );
   
         install.packages(
             pkgStr,
             lib = localLibStr,
             repos="http://cran.us.r-project.org"
         );
   
         
         if(! checkPkg(pkgStr)){
            print(
              paste("Unable to install",pkgStr,sep=" ")
            );

            return(FALSE);
         }
      } # If I could not install data.table
   
      print(paste(pkgStr, "installed", sep = " "));
   } else
      print(paste(pkgStr,"is already installed",sep=" "));

   return(TRUE);
} # installPkg

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Main:
#   - Install packages
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(! installPkg("ggplot2"))
   stop();

# packages are required, but should install with ggplot2
if(! installPkg("viridisLite"))
   stop();

if(! installPkg("data.table"))
   stop();

# These are  not hard dependencies
installPkg("fs");
installPkg("tcltk2");
installPkg("png");
installPkg("svglite");

#=========================================================
# License:
# 
# This code is under the unlicense (public domain).
#   However, for cases were the public domain is not
#   suitable, such as countries that do not respect the
#   public domain or were working with the public domain
#   is inconveint / not possible, this code is under the
#   MIT license
# 
# Public domain:
# 
# This is free and unencumbered software released into the
#   public domain.
# 
# Anyone is free to copy, modify, publish, use, compile,
#   sell, or distribute this software, either in source
#   code form or as a compiled binary, for any purpose,
#   commercial or non-commercial, and by any means.
# 
# In jurisdictions that recognize copyright laws, the
#   author or authors of this software dedicate any and
#   all copyright interest in the software to the public
#   domain. We make this dedication for the benefit of the
#   public at large and to the detriment of our heirs and
#   successors. We intend this dedication to be an overt
#   act of relinquishment in perpetuity of all present and
#   future rights to this software under copyright law.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
#   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
#   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
#   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
#   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
#   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
#   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
#   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
# 
# For more information, please refer to
#   <https://unlicense.org>
# 
# MIT License:
# 
# Copyright (c) 2024 jeremyButtler
# 
# Permission is hereby granted, free of charge, to any
#   person obtaining a copy of this software and
#   associated documentation files (the "Software"), to
#   deal in the Software without restriction, including
#   without limitation the rights to use, copy, modify,
#   merge, publish, distribute, sublicense, and/or sell
#   copies of the Software, and to permit persons to whom
#   the Software is furnished to do so, subject to the
#   following conditions:
# 
# The above copyright notice and this permission notice
#   shall be included in all copies or substantial
#   portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
#   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
#   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
#   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
#   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
#   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
#   USE OR OTHER DEALINGS IN THE SOFTWARE.
#=========================================================
