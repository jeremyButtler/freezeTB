#!/usr/bin/env Rscript
# This locks onto Rscripts location

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o header:
#     - included libraries and general setup
#   o fun01: getReads
#     - gets a fastq file that the user selected
#   o fun02: getSaveDir
#     - gets the directory to save everything to
#   o fun03: getRef
#     - gets reference sequence fasta file form user
#   o fun04: showTab1
#     - shows tab 1
#   o fun05: showTab2
#     - shows second tab
#   o fun06: quitInput
#     - quits freezeTb input gui
#   o fun07: runMinimap
#     - Runs freezeTb
#   o ingui:
#     - The code that runs the GUI
#   o license:
#     - Licensing for this code (public domain / mit)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# Header:
#   - included libraries and general setup
#   o header sec01:
#     - libraries
#   o header sec02:
#     - default file paths
#   o header sec03:
#     - find program locations
#   o header sec04:
#     - declaring the gui frames
#   o header sec05:
#     - tcl global variables
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec01:
#   - libraries
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# Check if there is the widgetTools library (for tooltips)
# This trick is from:
#   https://stackoverflow.com/questions/15155814/check-if-r-package-is-installed-then-load-library
if("tcltk2" %in% rownames(installed.packages())){
   library("tcltk2");
   TOOL_TIP_BL = 1;   # use tool tips in gui (tcltk2 only)
} else{
    # tcltk is a base R library, so should always be here
    require("tcltk") || stop("tcltk is not present");
    library("tcltk")
    TOOL_TIP_BL = 0; # tcltk 1 does not support tooltips
} # Else I need to use old tcltk (no tooltips)

# for file extensions
slashC = .Platform$file.sep;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec02:
#   - Default variables
#   o header sec02 sub01:
#     - Check OS type (unix/windows) to get default file
#       paths
#   o header sec02 sub02:
#     - Set up default paths to each database
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Header Sec02 Sub01:
#   - Check OS type (unix/windows) to get default file
#     paths
#*********************************************************

# here I am setting up the default file paths.
# def_filePathStr is the prefered; local path
# I am saving my database files in freezeTBFiles
if(.Platform$OS != "windows"){
   def_pathSplitSC = ':';

   def_altFilePathStr = "/usr/local/share/freezeTBFiles/";

   if("fs" %in% rownames(installed.packages())){
      def_filePathStr =
         fs::path_home("Documents", "freezeTBFiles");

      def_filePathStr =
         paste(def_filePathStr, slashC, sep = "");
   } else{
      def_filePathStr = "$HOME/Documents/freezeTBFiles/";
   }
} else{
   
   def_pathSplitSC = def_pathSplitSC;

   def_altFilePathStr =
      paste(
         as.list(
            strsplit( # if have multiple items in path
               Sys.getenv("PUBLIC"), # path to public
               def_pathSplitSC    # paths are separated by a def_pathSplitSC
             )
         )[[1]][1],   # only keep the first path
         "\\Documents\\freezeTBFiles\\",
         sep = ""
      );

   def_filePathStr =
      paste(
         as.list(
            strsplit(
               Sys.getenv("HOMEPATH"),
               def_pathSplitSC
            )
         )[[1]][1],
         "\\Documents\\freezeTBFiles\\",
         sep = ''
      ); # get local file location
} # Else: This is windows

#*********************************************************
# Header Sec02 Sub02:
#   - Set up default paths to each database
#*********************************************************

# i will store the input fastq files and output file
#   paths in these two variables
fqFilesStr = ""; # no default, user has to provide
outPathStr = ""; # default is nothing

# two types of variables;
#   - def_ are defaults (never change)
#   - PathStr change with user input (called in code)

def_refStr =
   paste(def_filePathStr, "NC000962.fa", sep="");

# finding which file path system i should use
if(! file.exists(def_refStr)){
   def_refStr =
      paste(
         def_altFilePathStr,
         "NC000962.fa",
         sep = ""
      );

   if(! file.exists(def_refStr)){
      def_refStr = "NC000962.fa";

      if(! file.exists(def_refStr))
         def_refStr = "";
   } # If: I could not open the alternate path
} # If: I need to go to my aternate location

def_refStr = normalizePath(def_refStr);
refPathStr = def_refStr;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec03:
#   - find program locations
#   o header sec03 sub01:
#     - get system path and add separator for program name
#   o header sec03 sub02:
#     - set up minimap2 name and local path
#   o header sec03 sub03:
#     - search path for minimap2
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Header Sec03 Sub01:
#   - get system path and add separator for program name
#*********************************************************

pathAryStr =
   as.list(
      strsplit(
         Sys.getenv("PATH"),
         def_pathSplitSC
      ) # split into separate paths
   ); # make sure is a list

for(siElm in 1:length(pathAryStr[[1]]))
{ # Loop: add diretory separator to end of path
   pathAryStr[[1]][siElm] =
      paste(
         pathAryStr[[1]][siElm],
         slashC,
         sep = ''
      );
} # Loop: add diretory separator to end of path

#*********************************************************
# Header Sec03 Sub02:
#   - set up minimap2 name and local path
#*********************************************************

if(.Platform$OS == "windows"){
   miniMapPathStr = "minimap2.exe";

   # set up "C:\Program Files" path
   minimapLocalStr =
      paste(
         as.list(
            strsplit(
               Sys.getenv("PROGRAMFILES"),
               def_pathSplitSC
            )
         )[[1]][1],
         "\\freezeTB\\",
         sep = ''
      ); # set up path to freezeTB in program files
} else{
   miniMapPathStr = "minimap2";

   minimapLocalStr =
      paste(
         as.list(
            strsplit(
               Sys.getenv("HOME"),
               def_pathSplitSC
            )
         )[[1]][1],
         "/local/",
         sep = ''
      ); # set up path to freezeTB in program files
} # set up windows/linux program names

#*********************************************************
# Header Sec03 Sub03:
#   - search path for minimap2
#*********************************************************

# here i am finding were minimap2 is installed at
#  "" is for current directory
#  minimapLocalStr is for "C:\Program Files\freezeTB"
#  path is the locations in my path

for(strCall in c("", minimapLocalStr, pathAryStr[[1]]))
{ # Loop: find minimap2
   strCall =
      paste(
         strCall,
         miniMapPathStr,
         sep = ''
      );

   if(file.exists(strCall)){
      miniMapPathStr = normalizePath(strCall);
      break;
   } # If: found minimap2
} # Loop: find minimap2

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec04:
#   - declaring gui frames
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

guiTK = tktoplevel();          # top level gui
tkwm.title(guiTK, "freezeTb"); # name of gui

# upper most frame (not sure why did)
menuTK = tkframe(guiTK, borderwidth = 2);

# options holds buttons for "tabs"
optionsTK = tkframe(guiTK, borderwidth = 2);

# tabs to move between
tab1TK = tkframe(optionsTK, borderwidth = 2);
tab2TK = tkframe(optionsTK, borderwidth = 2);

# holds run and quit buttons
runQuitTK = tkframe(guiTK, borderwidth = 2);


# relief options (for border) are
   # flat, raised, suken, solid, ridge, groove

# left and right sides of tab2
tab2LeftTK =
   tkframe(
      tab2TK,
      relief = "groove",
      borderwidth = 2
   );

tab2RightTK =
   tkframe(
      tab2TK,
      relief = "groove",
      borderwidth = 2
   );

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec05:
#   - tcl global variables
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# name of output
PREFIX_TCL = tclVar("out");

# slider (for threads)
THREADS_TCL = tclVar(15);

# eqx checkbox
EQX_BL_TCL = tclVar(0);

# radio button to select sequence type
SETTING_TCL = tclVar("map-ont");

#---------------------------------------------------------
# Fun01: getReads
#   - gets one or fastq files that the user selected
# Input:
# Output:
#   - Returns:
#     o An string with the selected file name
#---------------------------------------------------------
getReads =
function(
){
   fileMacStr =
      tk_choose.files(
         default = getwd(), # start of in working dir
         caption = "Select fastq file",
         multi = TRUE,
         filters =
            matrix(
               c(
                  "fastq*", ".fastq",
                  "fastq*", ".fq",
                  "fastq*", ".fastq.gz",
                  "fastq*", ".fq.gz"
                ), # allowed files (filters)
             4, # four rows
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an fastq file

   if(! is.na(fileMacStr[1])){ # if user selected files
      # this is needed to remove the directory that is
      # added with the multi command
      siElm = 1;
      lenSI = length(fileMacStr);

      while(siElm <= lenSI)
      { # Loop: remove directories
         if( dir.exists(fileMacStr[siElm]) ){
            fileMacStr = fileMacStr[- 1];
            lenSI = lenSI - 1;
         } else{
            siElm = siElm + 1;
         } # check if is a directory (not a file)
      } # Loop: remove directories

      if(length(fileMacStr) < 1)
         return(); # list only had directories

      # need to update the global variable
      assign(
         "fqFilesStr",
         normalizePath(fileMacStr),
         envir = .GlobalEnv
      ); # set up global variable for user input

      # let user know the files selected
      if(length(fileMacStr) == 1){
         tkconfigure(
            fqLabelTK,
            text = fileMacStr[1]
          ); # add file name to gui
      }else{
         tkconfigure(
            fqLabelTK,
            text =
               paste(
                  dirname(fileMacStr[1]),
                  slashC,
                  "*.fastq*",
                  sep = ""
               )
          ); # add * to specify multiple files
      } # Else: have multipe files
   } # If: have files
} # getReads

#---------------------------------------------------------
# Fun02: getSaveDir
#   - Gets the directory to save everything to
# Input:
# Output:
#   - Returns:
#     o An string with the selected file name
#---------------------------------------------------------
getSaveDir =
function(
){
   dirMacStr =
      tk_choose.dir(
         default = getwd(), # start of in working dir
         caption = "Select directory to save dirs to"
      ); # Let the user select an fastq dir

   if(! is.na(dirMacStr[1])){
      assign(
         "outPathStr",
         normalizePath(dirMacStr[1]),
         envir = .GlobalEnv
      );

      tkconfigure(
         dirLabelTK,
         text = dirMacStr[1]
       );
   } # If: output path input
} # getReads

#---------------------------------------------------------
# Fun03: getRef
#   - Gets reference sequence fasta file form user
# Input:
# Output:
#---------------------------------------------------------
getRef =
function(
){
   fileMacStr =
      tk_choose.files(
         default = getwd(), # start of in working dir
         caption = "Select an reference fasta file",
         multi = FALSE,
         filters =
            matrix(
               c(
                  "fasta", ".fasta",
                  "fasta", ".fa"
                ), # allowed files (filters)
             2, # four rows
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an fasta file

   if(! is.na(fileMacStr[1])){
      assign(
         "refPathStr",
         normalizePath(fileMacStr[1]),
         envir = .GlobalEnv
      ); # set up global variable for user input

      tkconfigure(
         refLabelTK,
         text = fileMacStr[1]
       ); # update gui reference path
   } # If: have reference file
} # getRef

#---------------------------------------------------------
# Fun04: showTab1
#   - shows frist tab
# Input:
# Output:
#---------------------------------------------------------
showTab1 =
function(
){
   # forgetting a tab makes it invisible
   tkpack.forget(tab1TK);
   tkpack.forget(tab2TK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   
   # make GUIs that need to be seen visable; order matters
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, tab1TK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showTab1

#---------------------------------------------------------
# Fun05: showTab2
#   - shows second tab
# Input:
# Output:
#---------------------------------------------------------
showTab2 =
function(
){
   # forgetting a tab makes it invisible
   tkpack.forget(tab1TK);
   tkpack.forget(tab2TK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   
   # make GUIs that need to be seen visable; order matters
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, tab2TK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showTab2

#---------------------------------------------------------
# Fun06: quitInput
#   - quits freezeTb input gui
# Input:
# Output:
#---------------------------------------------------------
quitInput =
function(
){
   tkdestroy(guiTK);
} # quitInput

#---------------------------------------------------------
# Fun07: runMinimap
#   - runs freezeTb
# Input:
# Output:
#   - Runs:
#     o Freeze tb
#---------------------------------------------------------
runMinimap =
function(
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Fun07 TOC:
   #   - Runs freezeTb on input files
   #   o fun07 sec01:
   #     - Variable declerations
   #   o fun07 sec02:
   #     - Check if user provided an fastq file
   #   o fun07 sec03:
   #     - Build the prefix and minimap2 command
   #   o fun07 sec04:
   #     - get minimap2 version, open log, ann map reads
   #   o fun07 sec05:
   #     - return and exit
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun07 Sec01:
   #   - Variable declerations
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   # Set global varaibles
   logFileStr = ""; # version number and commands
   errSI = 0;

   # for file paths: i need to normalize path so that
   #   programs on different drives will not mess up
   fqFileStr =
      normalizePath(
         get(
            "fqFilesStr",
            envir = .GlobalEnv
          )
       ); # makes into a global path

   refFileStr =
      normalizePath(
         get(
             "refPathStr",
             envir = .GlobalEnv
          )
       );


   samStr = "";
   prefixStr = tkcget(dirLabelTK, "-text");
   argsMinimapStr = "";

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun07 Sec02:
   #   - check if user provided an fastq file
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(length(fqFileStr) == 0) {
      tk_messageBox(
         type = "ok",
         message = "A fastq file is required",
         caption = "ERROR: fastq file"
      );

      showTab1();
      return();
   } # If: No fastq file was input

   if(length(refFileStr) == 0 ||
      ! file.exists(refFileStr)
   ){ # If: I could not open the reference file
      tk_messageBox(
         type = "ok",
         message =
            paste(
               "Could not open reference",
               refFileStr,
               sep = " "
            ),
         caption = "ERROR: reference file"
      ); # Message box for reference error

      showTab1();
      return();
   } # If: I could not open the reference file


   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun07 Sec03:
   #   - build prefix and minimap2 command
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   prefixStr =
      paste(
         prefixStr,
         tclvalue(PREFIX_TCL),
         sep = slashC
      ); # Paste the prefix for file names together

   logFileStr =
      paste(
         prefixStr,
         "-log.txt",
         sep = ""
      ); # make log file name


   if(as.numeric(tclvalue(EQX_BL_TCL))){
      argsMinimapStr =
         paste(
            argsMinimapStr,
            "--eqx",
            sep=" "
         ); # add in --eqx argument
   } # If: wanted eqx argument

   samStr =
      paste(
         prefixStr,
         "-map.sam",
         sep = ""
      ); # add end of file name

   argsMinimapStr =
      paste(
         "-a",
         "-t",
         tclvalue(THREADS_TCL),
         "-x",
         tclvalue(SETTING_TCL),
         "-o",
         samStr,
         refFileStr,
         paste(fqFileStr, collapse = ' '),
         sep = " "
      ); # build minimap2 command

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun07 Sec05:
   #   - Get minimap2 version, open log, ann map reads
   #   o fun07 sec04 Sub01:
   #     - get and save version for minimap2 to log
   #   o fun07 sec04 Sub02:
   #     - map reads
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # Fun07 Sec04 Sub01:
   #   - get and save version for minimap2 to log
   #******************************************************

   errSI =
      system2(
         command =
            get(
               "miniMapPathStr",
               envir = .GlobalEnv
            ), # minimap2 location from global envrioment
         args = "--version",
         wait = TRUE,          # wait for minimap2
         timeout = 1           # error out after 1 second
      );

   if(errSI){
      tk_messageBox(
         type = "ok",
         message = "Could not find minimap2",
         caption = "ERROR"
      );

      return(0);
   } # If: could not run minimap2

   versStr = 
      system2(
         command = miniMapPathStr, # minimap2 call
         args = "--version",
         wait = TRUE,
         timeout = 1,
         stdout = TRUE # Get output from stdout
      ); # get version number for minimap2

   sink(logFileStr);  # Open the log

   print(
      paste(
         "Minimap2:",
         versStr,
         sep = " ")
      );

   print(
      paste(
         get(
            "miniMapPathStr",
            envir = .GlobalEnv
         ), # minimap2 location from global envrioment
         argsMinimapStr,
         sep = " "
      )
   ); # print command used to log

   #******************************************************
   # Fun07 Sec04 Sub02:
   #   - map reads
   #******************************************************
   
   errSI = 
      system2(
         command =
            get(
               "miniMapPathStr",
               envir = .GlobalEnv
            ),
         args = argsMinimapStr, # arugments for minimap2
         wait = TRUE,         # wait till miniMap finishes
         timeout = 0          # do no time out
      ); # run minimap2

   if(errSI){
      print("Error while running minimap2");

      tk_messageBox(
         type = "ok",
         message = "could not run minimap2",
         caption = "ERROR"
      );

      sink(); # close log
      return();
   } # If: could not run minimap2

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun07 Sec06:
   #   - return and exit
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   sink(); # close the log

   quitInput(); # no longer need freezeTb gui
} # runMinimap

#---------------------------------------------------------
# Ingui TOC:
#   - The code that runs the GUI
#   o ingui01:
#     - set up tab 1
#   o ingui02:
#     - set up tabs menu
#   o ingui03:
#     - set up exit/run buttons
#   o ingui04:
#     - set up tab 2
#   o ingui05:
#     - Set up the gui
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui01:
#    - set up tab 1
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# setu up the reference text box (input reference)
refLabelTK =
   tklabel(
      tab1TK,
      text = def_refStr
   ); # Current reference file used

# set up the label for the reference text box
refButTK =
   tkbutton(
      tab1TK,
      text = "Reference        ",
      command = getRef
); # Add the change reference file button

# set up the tooltip for the reference (tcltk2 only)
if(TOOL_TIP_BL > 0){
   tk2tip(
      refButTK,
      "Reference genome to map reads to (NC000962.3)"
   ); # reference genome button tootip
} # If: I can add an tooltip

# label to side of fastq button (tells input file)
fqLabelTK =
   tklabel(
      tab1TK,
      text = ""
   ); # Holds the selected fastq file

fqButTK =
   tkbutton(
      tab1TK,
      text = "reads (.fastq)",
      command = getReads
); # add fastq file button

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      fqButTK,
      "Fastq file with reads to check AMRs/spoltypes"
   ); # fastq button tooltip
} # If: I can add an tooltip

# output directory button

dirLabelTK =
   tklabel(
      tab1TK,
      text = getwd()
   ); # Holds the output directory

dirButTK =
   tkbutton(
      tab1TK,
      text = "Output directory",
      command = getSaveDir
); # add the get output directory button

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      dirButTK,
      "Directory to save output files in"
   ); # directory button tooltip
} # If: I can add an tooltip

# output prefix text box

prefLabelTK =
   tklabel(
      tab1TK,
      text = "Output Prefix"
   ); # Holds the prefix box name

prefBoxTK =
   tkentry(
      tab1TK,
      width = 20,
      textvariable = PREFIX_TCL
   ); # Set up the prefix name for the output

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      prefBoxTK,
      "Prefix to add to output file names"
   ); # Prefix text box tooltip
} # If: I can add an tooltip


#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui02:
#   - set up menu buttons
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

tab1_ButTK =
   tkbutton(
      menuTK,
      text = "Basic Input",
      command = showTab1
   ); # Hide the extra options

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      tab1_ButTK,
      "The required input"
   ); # input menu button tooltip
} # If: I can add an tooltip

tab2_ButTK =
   tkbutton(
      menuTK,
      text = "Read filtering",
		command = showTab2
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      tab2_ButTK,
      "a second tab"
   ); # Database button tooltip
} # If: I can add an tooltip

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui03:
#   - exit/run buttons
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

runButTK =
   tkbutton(
      runQuitTK,
      text = "map reads",
      command =
         runMinimap
   ); # Run freezeTb

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      runButTK,
      "map fastq file to reference with minimap2"
   ); # Run freezeTb button tooltip
} # If: I can add an tooltip

qButTK =
   tkbutton(
      runQuitTK,
      text="quit",
      command = quitInput
   ); # Exit freezeTb

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      qButTK,
      "Exit"
   ); # quit freezeTb button tooltip
} # If: I can add an tooltip

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui04:
#   - the second tab
#   o ingui04 sub01:
#     - title
#   o ingui04 sub02:
#     - threads slider
#   o ingui04 sub03:
#     - eqx check box
#   o ingui04 sub04:
#     - settings radio buttons
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Ingui04 Sub01:
#   - title
#*********************************************************

tab2Title_LabTK =
   tklabel(
      tab2TK,
      text = "second tab"
   );

tab2LeftTitle_LabTK =
   tklabel(
      tab2LeftTK,
      text = "single settings"
   );

tab2RightTitle_LabTK =
   tklabel(
      tab2LeftTK,
      text = "sequence technology"
   );

#*********************************************************
# Ingui04 Sub02:
#   - threads slider
#*********************************************************

threads_slideTK =
   tkscale(
      parent = tab2LeftTK,
      from = 1,
      to = 16,
      showvalue = TRUE,
      variable = THREADS_TCL, #get stored number from this
      resolution = 1,         # movement by one click
      orient = "horiz"
   );

threads_LabTK =
   tklabel(
      tab2LeftTK,
      text = "threads"
   );

if(TOOL_TIP_BL > 0){
   tk2tip(
      threads_slideTK,
      "Number of threads to use"
   );
} # If: I can add an tooltip

#*********************************************************
# Ingui04 Sub03:
#   - eqx check box
#*********************************************************

eqx_checkboxTK =
   tkcheckbutton(
      tab2LeftTK,
      text = "use --eqx",
      variable = EQX_BL_TCL
    );

if(TOOL_TIP_BL > 0){
   tk2tip(
      eqx_checkboxTK,
      "make an eqx cigar"
   );
} # If: I can make an tooltip

#*********************************************************
# Ingui04 Sub04:
#   - settings radio buttons
#*********************************************************

tech_LabTK =
   tklabel(
      tab2RightTK,
      text = "Sequencing technology"
   );

mapOnt_radioTK =
   tkradiobutton(
      parent = tab2RightTK,
      text = "ONT",
      variable = SETTING_TCL,
      value = "map-ont" 
   );

mapPb_radioTK =
   tkradiobutton(
      parent = tab2RightTK,
      text = "pacbio",
      variable = SETTING_TCL,
      value =  "map-pb"
   );

mapICLR_radioTK =
   tkradiobutton(
      parent = tab2RightTK,
      text = "Illumina Long",
      variable = SETTING_TCL,
      value =  "map-iclr"
   );

sr_radioTK =
   tkradiobutton(
      parent = tab2RightTK,
      text = "Illumina short",
      variable = SETTING_TCL,
      value =  "sr"
   );

mapHIFI_radioTK =
   tkradiobutton(
      parent = tab2RightTK,
      text = "Pacbio hifi",
      variable = SETTING_TCL,
      value =  "map-hifi"
   );

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui05:
#   - set up gui
#   o ingui05 sub01:
#     - set up tab1 (required input)
#   o ingui05 sub02:
#     - set up the filtering menu frame
#   o ingui05 sub03:
#     - Set up the menu buttons
#   o ingui05 sub04:
#     - Set up the run and quit buton frame
#   o ingui05 sub05:
#     - Pack all the frames together and run idle loop
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Ingui05 Sub01:
#   - set up tab1 (required input)
#*********************************************************

# paring buttons and labels together
tkgrid(refButTK, refLabelTK);
tkgrid(fqButTK, fqLabelTK);
tkgrid(dirButTK, dirLabelTK);
tkgrid(prefLabelTK, prefBoxTK);

# setting location in GUI
tkgrid.configure(
   refButTK,
   fqLabelTK,
   dirLabelTK,
   prefBoxTK,
   sticky = "w"
); # Configure the right side labels/prefix input

tkgrid.configure(
   refLabelTK,
   fqButTK,
   dirButTK,
   prefLabelTK,
   sticky = "w"
); # Configure the left side input buttons/prefix label

#*********************************************************
# Ingui05 Sub02:
#   - set up tab two
#*********************************************************

# title for tab 2
tkpack(tab2TK, tab2Title_LabTK);

# build left half of tab 2

tkgrid(tab2LeftTitle_LabTK);
tkgrid(threads_LabTK, threads_slideTK);
tkgrid(eqx_checkboxTK);

tkgrid.configure(
   tab2LeftTitle_LabTK,
   threads_LabTK,
   eqx_checkboxTK,
   sticky = "w"
);

tkgrid.configure(
   threads_slideTK,
   sticky = "w"
);

# build right side of tab 2

tkgrid(tab2RightTitle_LabTK);

tkgrid(tech_LabTK);
tkgrid(mapOnt_radioTK);
tkgrid(mapPb_radioTK);
tkgrid(mapICLR_radioTK);
tkgrid(sr_radioTK);
tkgrid(mapHIFI_radioTK);

tkgrid.configure(
   tab2RightTitle_LabTK,
   tech_LabTK,
   mapOnt_radioTK,
   mapPb_radioTK,
   mapICLR_radioTK,
   sr_radioTK,
   mapHIFI_radioTK,
   sticky = "w"
);

# set up tab 2

tkpack(tab2TK, tab2LeftTK, side = "left")
tkpack(tab2TK, tab2RightTK, side = "left")
   # tkpack is simpiler than tkgrid, but also has less
   # controll. you can only use one per frame


#*********************************************************
# Ingui05 Sub03:
#   - Set up the menu buttons
#*********************************************************

tkpack(menuTK, tab1_ButTK, side = "left");
tkpack(menuTK, tab2_ButTK, side = "left");

#*********************************************************
# Ingui05 Sub04:
#   - Set up the run and quit buton frame
#*********************************************************

tkpack(runQuitTK, runButTK, side = "left");
tkpack(runQuitTK, qButTK, side = "left");

#*********************************************************
# Ingui05 Sub05:
#   - Pack all the frames together and run idle loop
#*********************************************************

showTab1();

# This provides a way to check if the user finished the
#   input and avoids Rscript making and then rushing to
#   finish the script before the user input anything.
while(
   as.numeric(tkwinfo( "exists", guiTK))
) Sys.sleep(0.2);

#q("no");

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
