#!/usr/bin/env Rscript
# This locks onto Rscripts location

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o header:
#     - Included libraries
#   o fun-01: getReads
#     - Gets a fastq file that the user selected
#   o fun-02: getSaveDir
#     - Gets the directory to save everything to
#   o fun-03: getRef
#     - Gets reference sequence fasta file form user
#   o fun-04: getAmrTbl
#     - Gets an AMR table (tbAmr)
#   o fun-05: getMiruTbl
#     - Gets an Miru table (tbMiru format)
#   o fun-06: getGeneTbl
#     - Gets the paf file with the genome coordinates
#   o fun-07: showReqInputMenu
#     - Shows the required input menu
#   o fun-08: showFiltMenu
#     - Shows the read filtering input menu
#   o fun-09: showAmrMenu
#     - Shows the AMR settings menu
#   o fun-10: showMiruMenu
#     - Shows the MIRU lineage menu
#   o fun-11: showConMenu
#     - Shows the extra settings for consensus building
#   o fun-12: showDepthMenu
#     - Shows the depth/coverage graphing menu
#   o fun-13: quitInput
#     - Quits freezeTb input gui
#   o fun-14: runFreezeTb
#     - Runs freezeTb
#   o ingui:
#     - The code that runs the GUI
#   o license:
#     - Licensing for this code (public domain / mit)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# Header:
#   - Included libraries
#   o header sec-01:
#     - Libraries
#   o header sec-02:
#     - Default file paths
#   o header sec-03:
#     - Declaring the gui frames
#   o header sec-04:
#     - tcl global variables
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec-01:
#   - Libraries
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if("fs" %in% rownames(installed.packages())){
   library("fs");
}

# Check if there is the widgetTools library (for tooltips)
# This trick is from:
#   https://stackoverflow.com/questions/15155814/check-if-r-package-is-installed-then-load-library
if("tcltk2" %in% rownames(installed.packages())){
   library("tcltk2");
   TOOL_TIP_BL = 1;
} else{
    # tcltk is a base R library, so should always be here
    require("tcltk") || stop("tcltk is not present");
    library("tcltk")
    TOOL_TIP_BL = 0;
} # Else I need to use old tcltk (no tooltips)

slashC = .Platform$file.sep;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec-02:
#   - Default variables
#   o header sec-02 sub-01:
#     - Check OS type (unix/windows) to get default file
#       paths
#   o header sec-02 sub-02:
#     - Set up default paths to each database
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Header Sec-02 Sub-01:
#   - Check OS type (unix/windows) to get default file
#     paths
#*********************************************************

if(.Platform$OS != "windows"){
   def_filePathStr = "/usr/local/share/freezeTbFiles/";
} else{
   
   def_filePathStr =
       paste(
          Sys.getenv("PUBLIC"),
          "\\Documents\\freezeTbFiles",
          sep = ""
       );
} # Else: This is windows

if("fs" %in% rownames(installed.packages())){
   def_altFilePathStr =
      fs::path_home("Documents", "freezeTbFiles");
} else if(.Platform$OS != "windows"){
   def_altFilePathStr = "~/Documents/freezeTbFiles";
} else{
   def_altFilePathStr = "$HOMEPATH$\\Documents\\freezeTbFiles";
} # Set up alternative file path

#*********************************************************
# Header Sec-02 Sub-02:
#   - Set up default paths to each database
#*********************************************************

def_amrTblStr =
   paste(def_filePathStr, "who-2023.tsv", sep = "");

if(! file.exists(def_amrTblStr)){
   def_amrTblStr =
      paste(def_altFilePathStr, "who-2023.tsv", sep = "");

   if(! file.exists(def_amrTblStr)){
      def_amrTblStr = "who-2023.tsv";

      if(! file.exists(def_amrTblStr))
         def_amrTblStr = "";
   } # If: I could not open the alternate path
} # If: I need to go to my aternate location

   
def_coordsStr =
  paste(def_filePathStr,"gene-tbl.tsv",sep="");

if(! file.exists(def_coordsStr)){
   def_coordsStr =
      paste(def_altFilePathStr, "gene-tbl.tsv", sep = "");

   if(! file.exists(def_coordsStr)){
      def_coordsStr = "gene-tbl.tsv";

      if(! file.exists(def_coordsStr))
         def_coordsStr = "";
   } # If: I could not open the altnerate path
} # If: I need to go to my aternate location


def_miruTblStr =
   paste(def_filePathStr, "miruTbl.tsv", sep="");

if(! file.exists(def_miruTblStr)){
   def_miruTblStr =
      paste(def_altFilePathStr, "miruTbl.tsv", sep = "");

   if(! file.exists(def_miruTblStr)){
      def_miruTblStr = "miruTbl.tsv";

      if(! file.exists(def_miruTblStr))
         def_miruTblStr = "";
   } # If: I could not open the alternate path
} # If: I need to go to my aternate location


def_refStr =
   paste(def_filePathStr, "NC000962.fa", sep="");

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

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec-03:
#   - Declaring the gui frames
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

guiTK = tktoplevel();
tkwm.title(guiTK, "freezeTb");

menuTK = tkframe(guiTK, borderwidth = 2);
optionsTK = tkframe(guiTK, borderwidth = 2);
runQuitTK = tkframe(guiTK, borderwidth = 2);

reqInputTK = tkframe(optionsTK, borderwidth = 2);
filtInputTK = tkframe(optionsTK, borderwidth = 2);
amrInputTK = tkframe(optionsTK, borderwidth = 2);
miruInputTK = tkframe(optionsTK, borderwidth = 2);
depthInputTK = tkframe(optionsTK, borderwidth = 2);

# Frames for consensus building

# relief options (for border) are
   # flat, raised, suken, solid, ridge, groove

conInputTK = tkframe(optionsTK, borderwidth = 2);

conGlobalInputTK =
   tkframe(
      conInputTK, 
      relief = "groove",
      borderwidth = 2
   );
conBuildInputTK =
   tkframe(
      conInputTK,
      relief = "groove",
      borderwidth = 2
   );
conPrintInputTK =
   tkframe(
      conInputTK,
      relief = "groove",
      borderwidth = 2
   );

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec-04:
#   - tcl global variables
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

PREFIX_TCL = tclVar("Hufflepuff");

# Filtering settings
MIN_MAPQ_TCL = tclVar(15);
MIN_MEDQ_TCL = tclVar(7);
MIN_MEANQ_TCL = tclVar(7);

# AMR detection settings
MIN_AMR_SUP_TCL = tclVar(0.05);
FRAMESHIFT_BL_TCL = tclVar(0);

# Miru lineage settings
FUDGE_TCL = tclVar(15);

# Cosensus defaults
MIN_BASE_Q_TCL = tclVar(7);
MIN_INS_Q_TCL = tclVar(7);
MIN_DEPTH_TCL = tclVar(20);
MIN_LEN_TCL = tclVar(200);

MIN_SNP_PERC_SUP_TCL = tclVar(0.5);
MIN_INS_PERC_SUP_TCL = tclVar(0.7);
MIN_DEL_PERC_SUP_TCL = tclVar(0.7);

# Consensus variant printing options
PRINT_DEPTH_TCL = tclVar(20);
PRINT_SNP_PERC_SUP_TCL = tclVar(0.1);
PRINT_INS_PERC_SUP_TCL = tclVar(0.1);
PRINT_DEL_PERC_SUP_TCL = tclVar(0.1);

# graphing
GRAPH_EXT_TCL = tclVar("tiff");

#---------------------------------------------------------
# Fun-01: getReads
#   - Gets a fastq file that the user selected
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
         multi = FALSE,
         filters =
            matrix(
               c(
                  "fastq", ".fastq",
                  "fastq", ".fq",
                  "fastq", "fastq.gz",
                  "fastq", "fq.gz"
                ), # allowed files (filters)
             4, # four rows
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an fastq file

   if(! is.na(fileMacStr[1]))
      tkconfigure(fqLabelTK, text = fileMacStr[1]);
} # getReads

#---------------------------------------------------------
# Fun-02: getSaveDir
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

   if(! is.na(dirMacStr[1]))
      tkconfigure(dirLabelTK, text = dirMacStr[1]);
} # getReads

#---------------------------------------------------------
# Fun-03: getRef
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

   if(! is.na(fileMacStr[1]))
      tkconfigure(refLabelTK, text = fileMacStr[1]);
} # getRef

#---------------------------------------------------------
# Fun-04: getAmrTbl
#   - Gets an AMR table (tbAmr)
# Input:
# Output:
#---------------------------------------------------------
getAmrTbl =
function(
){
   fileMacStr =
      tk_choose.files(
         default = getwd(), # start of in working dir
         caption = "Select an AMR table (tbAmr format)",
         multi = FALSE,
         filters =
            matrix(
               c(
                  "tbAmr", ".tsv"
                ), # allowed files (filters)
             1, # one row
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an AMR table

   if(! is.na(fileMacStr[1]))
      tkconfigure(amrDbLabelTK, text = fileMacStr[1]);
} # getAmrTbl

#---------------------------------------------------------
# Fun-05: getMiruTbl
#   - Gets an Miru table (tbMiru format)
# Input:
# Output:
#---------------------------------------------------------
getMiruTbl =
function(
){
   fileMacStr =
      tk_choose.files(
         default = getwd(), # start of in working dir
         caption = "Select an MIRU table (tbMiru format)",
         multi = FALSE,
         filters =
            matrix(
               c(
                  "tbMiru", ".tsv"
                ), # allowed files (filters)
             1, # one row
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an AMR table

   if(! is.na(fileMacStr[1]))
      tkconfigure(miruDb_LabTK, text = fileMacStr[1]);
} # getAmrTbgetMiruTbl

#---------------------------------------------------------
# Fun-06: getGeneTbl
#   - Gets the paf file with the genome coordinates
# Input:
# Output:
#---------------------------------------------------------
getGeneTbl =
function(
){
   fileMacStr =
      tk_choose.files(
         default = getwd(), # start of in working dir
         caption = "Select tsv with TB gene coordiantes",
         multi = FALSE,
         filters =
            matrix(
               c(
                  "gene coordiantes table", ".tsv"
                ), # allowed files (filters)
             1, # one row
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an AMR table

   if(! is.na(fileMacStr[1]))
      tkconfigure(depthGeneTbl_LabTK, text = fileMacStr[1]);
} # getGeneTbl

#---------------------------------------------------------
# Fun-07: showReqInputMenu
#   - Shows the required input menu
# Input:
# Output:
#---------------------------------------------------------
showReqInputMenu =
function(
){
   tkpack.forget(reqInputTK);
   tkpack.forget(filtInputTK);
   tkpack.forget(amrInputTK);
   tkpack.forget(miruInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);


   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, reqInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showReqInputMenu

#---------------------------------------------------------
# Fun-08: showFiltMenu
#   - Shows the read filtering input menu
# Input:
# Output:
#---------------------------------------------------------
showFiltMenu =
function(
){
   tkpack.forget(reqInputTK);
   tkpack.forget(filtInputTK);
   tkpack.forget(amrInputTK);
   tkpack.forget(miruInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, filtInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showReqInputMenu

#---------------------------------------------------------
# Fun-09: showAmrMenu
#   - Shows the AMR settings menu
# Input:
# Output:
#---------------------------------------------------------
showAmrMenu =
function(
){
   tkpack.forget(reqInputTK);
   tkpack.forget(filtInputTK);
   tkpack.forget(amrInputTK);
   tkpack.forget(miruInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, amrInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showFileMenu

#---------------------------------------------------------
# Fun-10: showMiruMenu
#   - Shows the MIRU lineage menu
# Input:
# Output:
#---------------------------------------------------------
showMiruMenu =
function(
){
   tkpack.forget(reqInputTK);
   tkpack.forget(filtInputTK);
   tkpack.forget(amrInputTK);
   tkpack.forget(miruInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, miruInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showFileMenu


#---------------------------------------------------------
# Fun-11: showConMenu
#   - Shows the extra settings for consensus building
# Input:
# Output:
#---------------------------------------------------------
showConMenu =
function(
){
   tkpack.forget(reqInputTK);
   tkpack.forget(filtInputTK);
   tkpack.forget(amrInputTK);
   tkpack.forget(miruInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, conInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showConMenu

#---------------------------------------------------------
# Fun-12: showDepthMenu
#   - Shows the depth/coverage graphing menu
# Input:
# Output:
#---------------------------------------------------------
showDepthMenu =
function(
){
   tkpack.forget(reqInputTK);
   tkpack.forget(filtInputTK);
   tkpack.forget(amrInputTK);
   tkpack.forget(miruInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, depthInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showDepthMenu


#---------------------------------------------------------
# Fun-13: quitInput
#   - Quits freezeTb input gui
# Input:
# Output:
#---------------------------------------------------------
quitInput =
function(
){
   tkdestroy(guiTK);
} # quitInput

#---------------------------------------------------------
# Fun-14: runFreezeTb
#   - Runs freezeTb
# Input:
# Output:
#   - Runs:
#     o Freeze tb
#---------------------------------------------------------
runFreezeTb =
function(
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Fun-14 TOC:
   #   - Runs freezeTb on input files
   #   o fun-14 sec-01:
   #     - Variable declerations
   #   o fun-14 sec-02:
   #     - Check if user provided an fastq file
   #   o fun-14 sec-03:
   #     - Build the prefix and minimap2 command
   #   o fun-14 sec-04:
   #     - Build freezeTb command
   #   o fun-14 sec-05:
   #     - Get minimap2 version, open log, ann map reads
   #   o fun-14 sec-06:
   #     - Run freezeTb
   #   o fun-14 sec-07:
   #     - Return and exit
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun-14 Sec-01:
   #   - Variable declerations
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   # Set global varaibles
   logFileStr = ""; # version number and commands
   errSI = 0;

   fqFileStr = as.character(tkcget(fqLabelTK, "-text"));
   refFileStr = as.character(tkcget(refLabelTK, "-text"));

   amrFileStr =
      as.character(tkcget(amrDbLabelTK, "-text"));

   miruFileStr =
      as.character(tkcget(miruDb_LabTK, "-text"));

   depthFileStr =
      as.character(tkcget(depthGeneTbl_LabTK, "-text"));

   samStr = "";

   prefixStr = tkcget(dirLabelTK, "-text");

   argsMinimapStr = "";

   tbCmdStr = ""; # Should be in users path

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun-14 Sec-02:
   #   - Check if user provided an fastq file
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(length(fqFileStr) == 0) {
      tk_messageBox(
         type = "ok",
         message = "A fastq file is required",
         caption = "ERROR: fastq file"
      );

      showReqInputMenu();
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

      showReqInputMenu();
      return();
   } # If: I could not open the reference file


   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun-14 Sec-03:
   #   - Build the prefix and minimap2 command
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
         "-cmds-and-versions.txt",
         sep = ""
      ); # Make the version number file

   samStr = paste(prefixStr, "-map.sam", sep = "");

   argsMinimapStr =
      paste(
         "-a -x map-ont",
         "-o",
         samStr,
         refFileStr,
         fqFileStr,
         sep = " "
      ); # Build the minimap2 command

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun-14 Sec-04:
   #   - Build freezeTb command
   #   o fun-14 sec-04 sub-01:
   #     - Build read filtering part of freezeTb command
   #   o fun-14 sec-04 sub-02:
   #     - Add AMR settings/check if AMR database exists
   #   o fun-14 sec-04 sub-03:
   #     - Add MIRU lineage commands/check MIRU database
   #   o fun-14 sec-04 sub-04:
   #     - Add in the consensus commands
   #   o fun-14 sec-04 sub-05:
   #     - Add in the read depth/coverage commands
   #   o fun-14 sec-04 sub-06:
   #     - Add in the required input commands
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # Fun-14 Sec-04 Sub-01:
   #   - Build the read filtering part of freezeTb command
   #******************************************************

   # General input
   tbCmdStr =
      paste(
         tbCmdStr,
         "-min-mapq",
         tclvalue(MIN_MAPQ_TCL),
         "-min-median-q",
         tclvalue(MIN_MEDQ_TCL),
         "-min-mean-q",
         tclvalue(MIN_MEANQ_TCL),
         "-min-depth",
         tclvalue(MIN_DEPTH_TCL),
         sep = " "
      );

   #******************************************************
   # Fun-14 Sec-04 Sub-02:
   #   - Add AMR settings/check if AMR database exists
   #******************************************************

   # AMR settings
   tmpCmdStr =
       paste(
         tbCmdStr,
         "-amr-tbl",
         amrFileStr,
         "-min-amr-map-perc",
         tclvalue(MIN_AMR_SUP_TCL),
         sep = " "
      ); # Add in the AMR settings

   if(as.numeric(tclvalue(FRAMESHIFT_BL_TCL))){
      tbCmdStr = paste(tbCmdStr, "-frameshift", sep=" ");
   } else {
      tbCmdStr = paste(tbCmdStr,"-no-frameshift",sep=" ");
   } # If: The user wanted frameshift checking

   if(length(amrFileStr) == 0 ||
      ! file.exists(amrFileStr)
   ){  # If: I could not open the AMR database
      tbCmdStr =
         paste(
            "Could not open AMR database",
             amrFileStr
         );
            
      tk_messageBox(
         type = "ok",
         message = tbCmdStr,
         caption = "ERROR: AMR database"
      );

      showAmrMenu();
      return();
   }  # If: I could not open the AMR database

   #******************************************************
   # Fun-14 Sec-04 Sub-03:
   #   - Add in MIRU lineage commands/check MIRU database
   #******************************************************

   tbCmdStr =
      paste(
         tbCmdStr,
         "-fudge",
         tclvalue(FUDGE_TCL),
         "-miru-tbl",
         miruFileStr,
         sep = " "
      );

   if(length(miruFileStr) == 0 ||
      ! file.exists(miruFileStr)
   ){ # If: I could not open the MIRU lineag table
      tbCmdStr =
         paste(
            "Could not open MIRU database",
             miruFileStr
         );
            
      tk_messageBox(
         type = "ok",
         message = tbCmdStr,
         caption = "ERROR: MIRU database"
      );

      showMiruMenu();
      return();
   } # If: I could not open the MIRU lineag table
 
   #******************************************************
   # Fun-14 Sec-04 Sub-04:
   #   - Add in the consensus commands
   #******************************************************

   tbCmdStr =
      paste(
         tbCmdStr,
         "-min-q",
         tclvalue(MIN_BASE_Q_TCL),
         "-min-q-ins",
         tclvalue(MIN_INS_Q_TCL),
         "-min-len",
         tclvalue(MIN_LEN_TCL),
         "-perc-snp-sup",
         tclvalue(MIN_SNP_PERC_SUP_TCL),
         "-perc-ins-sup",
         tclvalue(MIN_INS_PERC_SUP_TCL),
         "-perc-del-sup",
         tclvalue(MIN_DEL_PERC_SUP_TCL),
         "-p-min-depth",
         tclvalue(PRINT_DEPTH_TCL),
         "-p-perc-snp-sup",
         tclvalue(PRINT_SNP_PERC_SUP_TCL),
         "-p-perc-ins-sup",
         tclvalue(PRINT_INS_PERC_SUP_TCL),
         "-p-perc-del-sup",
         tclvalue(PRINT_DEL_PERC_SUP_TCL)
      );

   #******************************************************
   # Fun-14 Sec-04 Sub-05:
   #   - Add in the read depth/coverage commands
   #******************************************************

   tbCmdStr =
      paste(
         tbCmdStr,
         "-graph-ext",
         tclvalue(GRAPH_EXT_TCL),
         "-gene-coords",
         depthFileStr,
         sep = " "
      ); # Build the freezeTb command up

   if(length(depthFileStr) == 0 ||
      ! file.exists(depthFileStr)
   ){ # If: The gene coordinates file does not exist
      tbCmdStr =
         paste(
            "Could not open gene coordinates table",
             depthFileStr
         );
            
      tk_messageBox(
         type = "ok",
         message = tbCmdStr,
         caption = "ERROR"
      );

      showDepthMenu();
      return();
   } # If: The gene coordinates file does not exist

   #******************************************************
   # Fun-14 Sec-04 Sub-06:
   #   - Add in the required input commands
   #******************************************************

   tbCmdStr =
      paste(
         tbCmdStr,
         "-sam",
         samStr,
         "-prefix",
         prefixStr,
         sep = " "
      ); # finsh off the command

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun-14 Sec-05:
   #   - Get minimap2 version, open log, ann map reads
   #   o fun-14 sec-05 Sub-01:
   #     - Get and save version for minimap2 to log
   #   o fun-14 sec-05 Sub-02:
   #     - Map reads
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # Fun-14 Sec-05 Sub-01:
   #   - Get and save version for minimap2 to log
   #******************************************************
   errSI =
      system2(
         command = "minimap2", # Run minimap2
         args = "--version",
         wait = TRUE,          # Wait for minimap2
         timeout = 1           # error out after 1 second
      );

   if(errSI){
      tk_messageBox(
         type = "ok",
         message = "Could not find minimap2",
         caption = "ERROR"
      );
      return(0);
   } # If: I could not run minimap2

   versStr = 
      system2(
         command = "minimap2",
         args = "--version",
         wait = TRUE,
         timeout = 1,
         stdout = TRUE # Get output from stdout
      ); # Get the version number for minimap2

   sink(logFileStr);  # Open the log
   print(paste("Minimap2:", versStr, sep = " "));
   print(paste("minimap2", argsMinimapStr, sep = " "));

   #******************************************************
   # Fun-14 Sec-05 Sub-02:
   #   - Map reads
   #******************************************************
   
   errSI = 
      system2(
         command = "minimap2",# Minimap2 command
         args = argsMinimapStr,# arugments for minimap2
         wait = TRUE,         # wait till minimap finishes
         timeout = 0          # Do no time out
      ); # Run minimap2

   if(errSI){
      print("Error while running minimap2");

      tk_messageBox(
         type = "ok",
         message = "Could run minimap2",
         caption = "ERROR"
      );

      sink();
      return();
   } # If: I could not run minimap2

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun-14 Sec-06:
   #   - Run freezeTb
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   print(paste("freezeTb", tbCmdStr, sep = " "));

   errStr = 
      system2(
         command = "freezeTb",# Minimap2 command
         args = tbCmdStr,# arugments for minimap2
         wait = TRUE,         # wait till minimap finishes
         #stderr = TRUE,
         timeout = 0          # Do no time out
      ); # Run minimap2

   if(errStr){
      tbCmdStr =
         paste(
            "ERROR while running freezeTb",
            errStr,
            sep = " "
         );

      print(tbCmdStr);

      tk_messageBox(
         type = "ok",
         message = tbCmdStr,
         caption = "ERROR"
      );

      sink();
      return();
   } # If: I had an error running freezeTb

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun-14 Sec-07:
   #   - Return and exit
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   sink(); # Close the log
   quitInput();
} # runFreezeTb

#---------------------------------------------------------
# Ingui:
#   - The code that runs the GUI
#   o ingui-01:
#     - Set up required input commands
#   o ingui-02:
#     - Set up the hide/display extra options
#   o ingui-03:
#     - Exit/run buttons
#   o ingui-04:
#     - Read filtering menu
#   o ingui-05:
#     - AMR detection settings menu
#   o ingui-06:
#     - MIRU lineages locations options
#   o ingui-07:
#     - Set up the consnesus options menu
#   o ingui-08:
#     - Read depth graphs options
#   o ingui-09:
#     - Set up the gui
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui-01:
#   - Set up required input commands
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

refLabelTK =
   tklabel(
      reqInputTK,
      text = def_refStr
   ); # Current reference file used

refButTK =
   tkbutton(
      reqInputTK,
      text = "Reference        ",
      command = getRef
); # Add the change reference file button

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      refButTK,
      "Reference genome to map reads to (NC000962.3)"
   ); # reference genome button tootip
} # If: I can add an tooltip

fqLabelTK =
   tklabel(
      reqInputTK,
      text = ""
   ); # Holds the selected fastq file

fqButTK =
   tkbutton(
      reqInputTK,
      text = "reads (.fastq)",
      command = getReads
); # add the get fastq file button

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      fqButTK,
      "Fastq file with reads to check AMRs/spoligotypes"
   ); # fastq button tooltip
} # If: I can add an tooltip

dirLabelTK =
   tklabel(
      reqInputTK,
      text = getwd()
   ); # Holds the output directory

dirButTK =
   tkbutton(
      reqInputTK,
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

prefLabelTK =
   tklabel(
      reqInputTK,
      text = "Output Prefix"
   ); # Holds the prefix box name

prefBoxTK =
   tkentry(
      reqInputTK,
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
# Ingui-02:
#   - Set up the hide/display extra options
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

inputMenu_ButTK =
   tkbutton(
      menuTK,
      text = "Basic Input",
      command = showReqInputMenu
   ); # Hide the extra options

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      inputMenu_ButTK,
      "The required input"
   ); # input menu button tooltip
} # If: I can add an tooltip

depthMenu_ButTK =
   tkbutton(
      menuTK,
      text = "Graphing",
		command = showDepthMenu
   ); # Show the graping menu

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      depthMenu_ButTK,
      "Read depth/converage graph settings"
   ); # Database button tooltip
} # If: I can add an tooltip

filtMenu_ButTK =
   tkbutton(
      menuTK,
      text = "Read filtering",
		command = showFiltMenu
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      filtMenu_ButTK,
      "Read filtering settings"
   ); # Database button tooltip
} # If: I can add an tooltip

amrMenu_ButTK =
   tkbutton(
      menuTK,
      text = "AMR detection",
		command = showAmrMenu
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      amrMenu_ButTK,
      "AMR detection settings"
   ); # Database button tooltip
} # If: I can add an tooltip

miruMenu_ButTK =
   tkbutton(
      menuTK,
      text = "MIRU",
      command = showMiruMenu
   );

if(TOOL_TIP_BL > 0){
   tk2tip(
      miruMenu_ButTK,
      "Input for MIRU lineages (spologotyping)"
   );
}

conMenu_ButTK =
   tkbutton(
      menuTK,
      text = "Consensus settings",
      command = showConMenu
   ); # Show the extra consensus options

if(TOOL_TIP_BL > 0){
   tk2tip(
      conMenu_ButTK,
      "Settings for building consensus/variant printing"
   ); # Consensus settings tooltip
} # If: I have tooltips

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui-03:
#   - Exit/run buttons
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

runButTK =
   tkbutton(
      runQuitTK,
      text = "run freezeTb",
      command =
         runFreezeTb
   ); # Run freezeTb

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      runButTK,
      "Run freezeTb on input reads (fastq file)"
   ); # Run freezeTb button tooltip
} # If: I can add an tooltip

qButTK =
   tkbutton(
      runQuitTK,
      text="quit",
      #command = function() tkdestroy(guiTK)
      command = quitInput
   ); # Exit freezeTb

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      qButTK,
      "Exit freezeTb"
   ); # quit freezeTb button tooltip
} # If: I can add an tooltip

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui-04:
#   - Read filtering menu
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

filtTitle_LabTK =
   tklabel(
      filtInputTK,
      text = "Read filtering settings"
   );

filtMinMapq_LabTK =
   tklabel(
      filtInputTK,
      text = "Min mapping quality"
   );

filtMinMapq_SlideTK =
   tkscale(
      parent = filtInputTK,
      from = 0,
      to = 93,
      showvalue = TRUE,
      variable = MIN_MAPQ_TCL,
      resolution = 1,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      filtMinMapq_SlideTK,
      "Min mapping quality to keep an read"
   );
} # If: I can add an tooltip

filtMinMedQ_LabTK =
   tklabel(
      filtInputTK,
      text = "Min median Q"
   );

filtMinMedQ_SlideTK =
   tkscale(
      parent = filtInputTK,
      from = 0,
      to = 93,
      showvalue = TRUE,
      variable = MIN_MEDQ_TCL,
      resolution = 1,
      orient = "horiz"
   );

if(TOOL_TIP_BL > 0){
   tk2tip(
      filtMinMedQ_SlideTK,
      "Min median Q-score to keep an read"
   );
} # If: I can add an tooltip

filtMinMeanQ_LabTK =
   tklabel(
      filtInputTK,
      text = "Min mean Q"
   );

filtMinMeanQ_SlideTK =
   tkscale(
      parent = filtInputTK,
      from = 0,
      to = 93,
      showvalue = TRUE,
      variable = MIN_MEANQ_TCL,
      resolution = 1,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      filtMinMeanQ_SlideTK,
      "Min mean Q-score to keep an read"
   ); # Minimum read depth tooltip
} # If: I can add an tooltip

filtMinDepth_LabTK =
   tklabel(
      filtInputTK,
      text = "Min read depth"
   );

filtMinDepth_SlideTK =
   tkscale(
      parent = filtInputTK,
      from = 1,
      to = 100,
      showvalue = TRUE,
      variable = MIN_DEPTH_TCL,
      resolution = 1,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      filtMinDepth_SlideTK,
      "Min depth for consensus building/AMRs/Graphs"
   ); # Minimum read depth tooltip
} # If: I can add an tooltip


#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui-05:
#   - AMR detection settings menu
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

amrTitle_LabTK =
   tklabel(
      amrInputTK,
      text = "AMR detection settings"
);

minAmrSup_LabTK =
   tklabel(
      amrInputTK,
      text = "Min % support for AMR"
   );

minAmrSup_SlideTK =
   tkscale(
      parent = amrInputTK,
      from = .01,
      to = 1,
      showvalue = TRUE,
      variable = MIN_AMR_SUP_TCL,
      resolution = .01,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      minAmrSup_SlideTK,
      "Min % of reads supporting an AMR"
   ); # Minimum read depth tooltip
} # If: I can add an tooltip

frameshiftBl_checkboxTK =
   tkcheckbutton(
      amrInputTK,
      text = "Frameshift (not recommended)",
      variable = FRAMESHIFT_BL_TCL
    );

if(TOOL_TIP_BL > 0){
   tk2tip(
      frameshiftBl_checkboxTK,
      "Check consensuses for frameshift AMRs (avoid)"
   );
} # If: I can make an tooltip

amrDbLabelTK =
   tklabel(
      amrInputTK,
      text = def_amrTblStr
   );

amrDbButTK =
   tkbutton(
      amrInputTK,
      text = "AMR table        ",
      command = getAmrTbl
); # add the get AMR table button

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      amrDbButTK,
      "Who 2023 catalog in tbAMR format"
   ); # amr table button tootip
} # If: I can add an tooltip

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui-06:
#   - MIRU lineages locations options
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

miruTitle_LabTK =
   tklabel(
      miruInputTK,
      text = "MIRU lineage settings"
   );

miruDb_LabTK =
   tklabel(
      miruInputTK,
      text = def_miruTblStr
   ); # Current MIRU lineage table used

miruDb_ButTK =
   tkbutton(
      miruInputTK,
      text = "MIRU table",
      command = getMiruTbl
   ); # add the get miru table button

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      miruDb_ButTK,
      "Table with spologotypes (MIRU lineages)"
   ); # miru lineage button tootip
} # If: I can add an tooltip

miruFudge_LabTK =
   tklabel(
      miruInputTK,
      text = "Fudge factor"
   );

miruFudge_SlideTK =
   tkscale(
      miruInputTK,
      from = 0,
      to = 50,
      showvalue = TRUE,
      variable = FUDGE_TCL,
      resolution = 1,
      orient = "horiz"
   );

if(TOOL_TIP_BL > 0){
   tk2tip(
     miruFudge_SlideTK,
     "Range for MIRU lengths; how much to fudge length by"
   );
} # Add tooltip label

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui-07:
#   - Set up the consnesus options menu
#   o ingui-07 frame-01:
#     - Consensus global (bulding) options
#   o ingui-07 frame-02:
#     - Consensus collapsing (bulding) options
#   o ingui-07 frame-03:
#     - Variant printing options for consensus
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Ingui-07 Frame-01:
#   - Consensus global (bulding) options
#*********************************************************

conGlobTitle_LabTK =
   tklabel(
      conGlobalInputTK,
      text = "Cosensus building/variant printing settings"
);

minBaseQ_LabTK =
   tklabel(
      conGlobalInputTK,
      text = "Min Q-score (Base)"
   );

minBaseQ_SlideTK =
   tkscale(
      parent = conGlobalInputTK,
      from = 0,
      to = 93,
      showvalue = TRUE,
      variable = MIN_BASE_Q_TCL,
      resolution = 1,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      minBaseQ_SlideTK,
      "Mininmum Q-scrore to keep an base (consensus step)"
   ); # Minimum q-score button
} # If: I can add an tooltip

minInsQ_LabTK =
   tklabel(
      conGlobalInputTK,
      text = "Min Q-score (Insertion)"
   );

minInsQ_SlideTK =
   tkscale(
      parent = conGlobalInputTK,
      from = 0,
      to = 93,
      showvalue = TRUE,
      variable = MIN_INS_Q_TCL,
      resolution = 1,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      minInsQ_SlideTK,
      "Mininmum Q-scrore to keep an insertion (consensus)"
   ); # Minimum q-score button
} # If: I can add an tooltip

#*********************************************************
# Ingui-07 Frame-02:
#   - Consensus collapsing (bulding) options
#*********************************************************

conBuildTitle_LabTK =
   tklabel(
      conBuildInputTK,
      text = "Cosensus building only settings"
);


minLen_LabTK =
   tklabel(
      conBuildInputTK,
      text = "Min length"
   );

minLen_SlideTK =
   tkscale(
      parent = conBuildInputTK,
      from = 0,
      to = 1000,
      showvalue = TRUE,
      variable = MIN_LEN_TCL,
      resolution = 10,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      minLen_SlideTK,
      "Min cosensus fragement length to keep a fragment"
   ); # Minimum read depth tooltip
} # If: I can add an tooltip

minSnpSup_LabTK =
   tklabel(
      conBuildInputTK,
      text = "Min % snp support"
   );

minSnpSup_SlideTK =
   tkscale(
      parent = conBuildInputTK,
      from = 0,
      to = 1,
      showvalue = TRUE,
      variable = MIN_SNP_PERC_SUP_TCL,
      resolution = 0.01,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      minSnpSup_SlideTK,
      "Min percent support to not mask an snp/match"
   );
} # If: I can add an tooltip

minInsSup_LabTK =
   tklabel(
      conBuildInputTK,
      text = "Min % ins support"
   );

minInsSup_SlideTK =
   tkscale(
      parent = conBuildInputTK,
      from = 0,
      to = 1,
      showvalue = TRUE,
      variable = MIN_INS_PERC_SUP_TCL,
      resolution = 0.01,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      minInsSup_SlideTK,
      "Min percent support to not remove an insertion"
   ); # Minimum read depth tooltip
} # If: I can add an tooltip

minDelSup_LabTK =
   tklabel(
      conBuildInputTK,
      text = "Min % del support"
   );

minDelSup_SlideTK =
   tkscale(
      parent = conBuildInputTK,
      from = 0,
      to = 1,
      showvalue = TRUE,
      variable = MIN_DEL_PERC_SUP_TCL,
      resolution = 0.01,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      minDelSup_SlideTK,
      "Min percent support to not remove an deletion"
   ); # Minimum read depth tooltip
} # If: I can add an tooltip

#*********************************************************
# Ingui-07 Frame-03:
#   - Variant printing options for consensus
#*********************************************************

conPrintTitle_LabTK =
   tklabel(
      conPrintInputTK,
      text = "Variant printing only settings"
);

pDepth_LabTK =
   tklabel(
      parent = conPrintInputTK,
      text = "Min read depth"
   );

pDepth_SlideTK =
   tkscale(
      parent = conPrintInputTK,
      from = 1,
      to = 100,
      showvalue = TRUE,
      variable = PRINT_DEPTH_TCL,
      resolution = 1,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      pDepth_SlideTK,
      "Min read depth to conPrint alternative variant/AMR"
   );
} # If: I can add an tooltip

pSnpSup_LabTK =
   tklabel(
      conPrintInputTK,
      text = "Min % snp support"
   );

pSnpSup_SlideTK =
   tkscale(
      parent = conPrintInputTK,
      from = 0,
      to = 1,
      showvalue = TRUE,
      variable = PRINT_SNP_PERC_SUP_TCL,
      resolution = 0.01,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      pSnpSup_SlideTK,
      "Min percent support to conPrint an match/snp variant"
   );
} # If: I can add an tooltip

pInsSup_LabTK =
   tklabel(
      conPrintInputTK,
      text = "Min % ins support"
   );

pInsSup_SlideTK =
   tkscale(
      parent = conPrintInputTK,
      from = 0,
      to = 1,
      showvalue = TRUE,
      variable = PRINT_INS_PERC_SUP_TCL,
      resolution = 0.01,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      pInsSup_SlideTK,
      "Min percent support to conPrint an insertion variant"
   ); # Minimum read depth tooltip
} # If: I can add an tooltip

pDelSup_LabTK =
   tklabel(
      conPrintInputTK,
      text = "Min % del support"
   );

pDelSup_SlideTK =
   tkscale(
      parent = conPrintInputTK,
      from = 0,
      to = 1,
      showvalue = TRUE,
      variable = PRINT_DEL_PERC_SUP_TCL,
      resolution = 0.01,
      orient = "horiz"
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      pDelSup_SlideTK,
      "Min percent support to conPrint an deletion variant"
   );
} # If: I can add an tooltip

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui-08:
#   - Read depth graphs options
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

depthTitle_LabTK =
   tklabel(
      depthInputTK,
      text = "Graphing; read depth and coverage settings"
   );

depthGeneTbl_LabTK =
   tklabel(
      depthInputTK,
      text = def_coordsStr
   ); # Current gene coordiantes table used

depthGeneTbl_ButTK =
   tkbutton(
      depthInputTK,
      text = "Gene coordinates",
      command = getGeneTbl
); # add the change gene coordiantes file button

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      depthGeneTbl_ButTK,
      "table with coordinates for all genes in reference"
   ); # gene table button tootip
} # If: I can add an tooltip


ext_LabTK =
   tklabel(
      parent = depthInputTK,
      text = "Select the file type to save graphs as"
   );

svgExt_radioTK =
   tkradiobutton(
      parent = depthInputTK,
      text = "svg",
      variable = GRAPH_EXT_TCL,
      value = "svg" 
   );

tiffExt_radioTK =
   tkradiobutton(
      parent = depthInputTK,
      text = "tiff",
      variable = GRAPH_EXT_TCL,
      value =  "tiff"
   );

pngExt_radioTK =
   tkradiobutton(
      parent = depthInputTK,
      text = "png",
      variable = GRAPH_EXT_TCL,
      value =  "png"
   );

pdfExt_radioTK =
   tkradiobutton(
      parent = depthInputTK,
      text = "pdf",
      variable = GRAPH_EXT_TCL,
      value =  "pdf"
   );

jpegExt_radioTK =
   tkradiobutton(
      parent = depthInputTK,
      text = "jpeg",
      variable = GRAPH_EXT_TCL,
      value =  "jpeg"
   );

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui-09:
#   - Set up the gui
#   o ingui-09 sub-01:
#     - Set up the required input frame
#   o ingui-09 sub-02:
#     - Set up the filtering menu frame
#   o ingui-09 sub-03:
#     - Set up the AMR menu frame
#   o ingui-09 sub-04:
#     - Set up the MIRU menu frame
#   o ingui-09 sub-05:
#     - Set up the consensus input frame
#   o ingui-09 sub-06:
#     - Set up the read depth graphing frame
#   o ingui-09 sub-07:
#     - Set up the menu buttons
#   o ingui-09 sub-08:
#     - Set up the run and quit buton frame
#   o ingui-09 sub-09:
#     - Pack all the frames together and run idle loop
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Ingui-09 Sub-01:
#   - Set up the required input frame
#*********************************************************

tkgrid(refButTK, refLabelTK);
tkgrid(fqButTK, fqLabelTK);
tkgrid(dirButTK, dirLabelTK);
tkgrid(prefLabelTK, prefBoxTK);

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
# Ingui-09 Sub-02:
#   - Set up the filtering menu frame
#*********************************************************

tkgrid(filtTitle_LabTK);
tkgrid(filtMinMapq_LabTK, filtMinMapq_SlideTK);
tkgrid(filtMinMedQ_LabTK, filtMinMedQ_SlideTK);
tkgrid(filtMinMeanQ_LabTK, filtMinMeanQ_SlideTK);
tkgrid(filtMinDepth_LabTK, filtMinDepth_SlideTK);

tkgrid.configure(
   filtTitle_LabTK,
   filtMinMapq_LabTK,
   filtMinMedQ_LabTK,
   filtMinMeanQ_LabTK,
   filtMinDepth_LabTK,
   sticky = "w"
);

tkgrid.configure(
   filtMinMapq_SlideTK,
   filtMinMedQ_SlideTK,
   filtMinMeanQ_SlideTK,
   filtMinDepth_SlideTK,
   sticky = "w"
);

#*********************************************************
# Ingui-09 Sub-03:
#   - Set up the AMR menu frame
#*********************************************************

tkgrid(amrTitle_LabTK);
tkgrid(amrDbButTK, amrDbLabelTK);
tkgrid(minAmrSup_LabTK, minAmrSup_SlideTK);
tkgrid(frameshiftBl_checkboxTK);

tkgrid.configure(
   amrTitle_LabTK,
   amrDbButTK,
   minAmrSup_LabTK,
   frameshiftBl_checkboxTK,
   sticky = "w"
); # Configure the right side labels/prefix input

tkgrid.configure(
   amrDbLabelTK,
   minAmrSup_SlideTK,
   sticky = "w"
); # Configure the right side labels/prefix input


#*********************************************************
# Ingui-09 Sub-04:
#   - Set up the MIRU menu frame
#*********************************************************

tkgrid(miruTitle_LabTK);
tkgrid(miruDb_ButTK, miruDb_LabTK);
tkgrid(miruFudge_LabTK, miruFudge_SlideTK);

tkgrid.configure(
   miruTitle_LabTK,
   miruDb_ButTK,
   miruFudge_SlideTK,
   miruFudge_LabTK,
   sticky = "w"
);

tkgrid.configure(
   miruDb_LabTK,
   sticky = "w"
);

#*********************************************************
# Ingui-09 Sub-05:
#   - Set up the consensus input frame
#   o ingui-09 sub-05 cat-01:
#     - Set up the global consensus options frame
#   o ingui-09 sub-05 cat-02:
#     - Set up the collapse consensus options frame
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Ingui-09 Sub-05 Cat-01:
#   - Set up the global consensus options frame
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

tkgrid(conGlobTitle_LabTK);
tkgrid(minBaseQ_LabTK, minBaseQ_SlideTK);
tkgrid(minInsQ_LabTK, minInsQ_SlideTK);

tkgrid.configure(
   conGlobTitle_LabTK,
   minBaseQ_LabTK,
   minInsQ_LabTK,
   sticky = "w"
); # set up the slider labels locations

tkgrid.configure(
   minBaseQ_SlideTK,
   minInsQ_SlideTK,
   sticky = "w"
); # Set up the sliders locations

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Ingui-09 Sub-05 Cat-02:
#   - Set up the collapse consensus options frame
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

tkgrid(conBuildTitle_LabTK);
tkgrid(minLen_LabTK, minLen_SlideTK);
tkgrid(minSnpSup_LabTK, minSnpSup_SlideTK);
tkgrid(minInsSup_LabTK, minInsSup_SlideTK);
tkgrid(minDelSup_LabTK, minDelSup_SlideTK);

tkgrid.configure(
   conBuildTitle_LabTK,
   minLen_LabTK,
   minSnpSup_LabTK,
   minInsSup_LabTK,
   minDelSup_LabTK,
   sticky = "w"
); # set up the slider labels locations

tkgrid.configure(
   minLen_SlideTK,
   minSnpSup_SlideTK,
   minInsSup_SlideTK,
   minDelSup_SlideTK,
   sticky = "w"
); # Set up the sliders locations

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Ingui-09 Sub-05 Cat-03:
#   - Set up the print variants consensus options frame
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

tkgrid(conPrintTitle_LabTK);
tkgrid(pDepth_LabTK, pDepth_SlideTK);
tkgrid(pSnpSup_LabTK, pSnpSup_SlideTK);
tkgrid(pInsSup_LabTK, pInsSup_SlideTK);
tkgrid(pDelSup_LabTK, pDelSup_SlideTK);

tkgrid.configure(
   conPrintTitle_LabTK,
   pDepth_LabTK,
   pSnpSup_LabTK,
   pInsSup_LabTK,
   pDelSup_LabTK,
   sticky = "w"
); # set up the slider labels locations

tkgrid.configure(
   pDepth_SlideTK,
   pSnpSup_SlideTK,
   pInsSup_SlideTK,
   pDelSup_SlideTK,
   sticky = "w"
); # Set up the sliders locations

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Ingui-09 Sub-05 Cat-03:
#   - Build the consensus options frames
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

tkpack(conInputTK, conGlobalInputTK, side = "top");
tkpack(conInputTK, conBuildInputTK, side = "left");
tkpack(conInputTK, conPrintInputTK, side = "right");

#*********************************************************
# Ingui-09 Sub-06:
#   - Set up the read depth graphing frame
#*********************************************************

tkgrid(depthTitle_LabTK);
tkgrid(depthGeneTbl_ButTK, depthGeneTbl_LabTK);
tkgrid(ext_LabTK);
tkgrid(svgExt_radioTK);
tkgrid(tiffExt_radioTK);
tkgrid(pngExt_radioTK);
tkgrid(pdfExt_radioTK);
tkgrid(jpegExt_radioTK);

tkgrid.configure(
   depthTitle_LabTK,
   depthGeneTbl_ButTK,
   ext_LabTK,
   svgExt_radioTK,
   tiffExt_radioTK,
   pngExt_radioTK,
   pdfExt_radioTK,
   jpegExt_radioTK,
   sticky = "w"
);

tkgrid.configure(
   depthGeneTbl_LabTK,
   sticky = "w"
);

#*********************************************************
# Ingui-09 Sub-07:
#   - Set up the menu buttons
#*********************************************************

tkpack(menuTK, inputMenu_ButTK, side = "left");
tkpack(menuTK, filtMenu_ButTK, side = "left");
tkpack(menuTK, amrMenu_ButTK, side = "left");
tkpack(menuTK, miruMenu_ButTK, side = "left");
tkpack(menuTK, conMenu_ButTK, side = "left");
tkpack(menuTK, depthMenu_ButTK, side = "left");

#*********************************************************
# Ingui-09 Sub-08:
#   - Set up the run and quit buton frame
#*********************************************************

tkpack(runQuitTK, runButTK, side = "left");
tkpack(runQuitTK, qButTK, side = "left");

#*********************************************************
# Ingui-09 Sub-09:
#   - Pack all the frames together and run idle loop
#*********************************************************

showReqInputMenu();

# This provides a way to check if the user finished the
#   input and avoids Rscript making and then rushing to
#   finish the script before the user input anything.
while(as.numeric(tkwinfo("exists", guiTK)))
   Sys.sleep(0.2);

q("no");

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
