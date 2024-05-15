#!/usr/bin/env Rscript
# This locks onto Rscripts location

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o header:
#     - Included libraries
#   o fun01: getReads
#     - Gets a fastq file that the user selected
#   o fun02: getSaveDir
#     - Gets the directory to save everything to
#   o fun03: getRef
#     - Gets reference sequence fasta file form user
#   o fun04: getAmrTbl
#     - Gets an AMR table (tbAmr)
#   o fun05: getMiruTbl
#     - Gets an Miru table (tbMiru format)
#   o fun06: getSpoligoSeq
#     - Gets the spoligo space sequences for spoligotyping
#   o fun07: getSpoligoDB
#     - Gets the lineage database for spoligotyping
#   o fun08: getGeneTbl
#     - Gets the paf file with the genome coordinates
#   o fun09: getMaskPrim
#     - Gets the primer masking tsv file
#   o fun10: showReqInputMenu
#     - Shows the required input menu
#   o fun11: showFiltMenu
#     - Shows the read filtering input menu
#   o fun12: showAmrMenu
#     - Shows the AMR settings menu
#   o fun13: showLineageMenu
#     - Shows the MIRU lineage menu
#   o fun14: showConMenu
#     - Shows the extra settings for consensus building
#   o fun15: showDepthMenu
#     - Shows the depth/coverage graphing menu
#   o fun16: showStatus
#     - Shows the status screen of freezeTB gui
#   o fun17: quitInput
#     - Quits freezeTb input gui
#   o fun18: runFreezeTb
#     - Runs freezeTb
#   o ingui:
#     - The code that runs the GUI
#   o license:
#     - Licensing for this code (public domain / mit)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# Header:
#   - Included libraries
#   o header sec01:
#     - Libraries
#   o header sec02:
#     - Default file paths
#   o header sec03:
#     - Declaring the gui frames
#   o header sec04:
#     - tcl global variables
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec01:
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

if(.Platform$OS != "windows"){
   def_filePathStr = "/usr/local/share/freezeTBFiles/";
} else{
   
   def_filePathStr =
       paste(
          Sys.getenv("PUBLIC"),
          "\\Documents\\freezeTBFiles\\",
          sep = ""
       );
} # Else: This is windows

if("fs" %in% rownames(installed.packages())){
   def_altFilePathStr =
      fs::path_home("Documents", "freezeTBFiles");

   def_altFilePathStr =
      paste(def_altFilePathStr, slashC, sep = "");
} else if(.Platform$OS != "windows"){
   def_altFilePathStr = "~/Documents/freezeTBFiles/";
} else{
   def_altFilePathStr = "$HOMEPATH$\\Documents\\freezeTBFiles\\";
} # Set up alternative file path

#*********************************************************
# Header Sec02 Sub02:
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

def_spoligoRefsStr =
   paste(def_filePathStr, "spoligotype-seq.fa", sep="");

if(! file.exists(def_spoligoRefsStr)){
   def_spoligoRefsStr =
      paste(
         def_altFilePathStr,
         "spoligotype-seq.fa",
         sep = ""
      );

   if(! file.exists(def_spoligoRefsStr)){
      def_spoligoRefsStr = "spoligotype-seq.fa";

      if(! file.exists(def_spoligoRefsStr))
         def_spoligoRefsStr = "";
   } # If: I could not open the alternate path
} # If: I need to go to my aternate location

def_spoligoDbStr =
   paste(def_filePathStr, "spoligo-lineages.csv", sep="");

if(! file.exists(def_spoligoDbStr)){
   def_spoligoDbStr =
      paste(
         def_altFilePathStr,
         "spoligo-lineages.csv",
         sep = ""
      );

   if(! file.exists(def_spoligoDbStr)){
      def_spoligoDbStr = "spoligo-lineages.csv";

      if(! file.exists(def_spoligoDbStr))
         def_spoligoDbStr = "";
   } # If: I could not open the alternate path
} # If: I need to go to my aternate location

def_primMaskStr =
   paste(def_filePathStr, "mask.tsv", sep="");

if(! file.exists(def_primMaskStr)){
   def_primMaskStr =
      paste(
         def_altFilePathStr,
         "mask.tsv",
         sep = ""
      );

   if(! file.exists(def_spoligoDbStr)){
      def_primMaskStr = "mask.tsv";

      if(! file.exists(def_primMaskStr))
         def_primMaskStr = "";
   } # If: I could not open the alternate path
} # If: I need to go to my aternate location

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec03:
#   - Declaring the gui frames
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

guiTK = tktoplevel();
tkwm.title(guiTK, "freezeTb");

menuTK = tkframe(guiTK, borderwidth = 2);
optionsTK = tkframe(guiTK, borderwidth = 2);
runQuitTK = tkframe(guiTK, borderwidth = 2);
statusTK = tkframe(guiTK, borderwidth = 2);

reqInputTK = tkframe(optionsTK, borderwidth = 2);
filtInputTK = tkframe(optionsTK, borderwidth = 2);
amrInputTK = tkframe(optionsTK, borderwidth = 2);
depthInputTK = tkframe(optionsTK, borderwidth = 2);

# relief options (for border) are
   # flat, raised, suken, solid, ridge, groove

# Frames for lineages

lineageInputTK = tkframe(optionsTK, borderwidth = 2);

miruVarTK =
   tkframe(
      lineageInputTK,
      borderwidth = 2,
      relief = "groove"
   );

spoligoVarTK =
   tkframe(
      lineageInputTK,
      borderwidth = 2,
      relief = "groove"
   );

# Frames for consensus building

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
# Header Sec04:
#   - tcl global variables
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

PREFIX_TCL = tclVar("Hufflepuff");
ADJ_COORDS_BL_TCL = tclVar(0); # Ajdust coordiantes
ADJ_COORDS_REF_TCL = tclVar("NC000962.3");

# Filtering settings
MIN_MAPQ_TCL = tclVar(15);
MIN_MEDQ_TCL = tclVar(7);
MIN_MEANQ_TCL = tclVar(7);

# primer masking
MASK_PRIM_BL_TCL = tclVar(0);

# AMR detection settings
MIN_AMR_SUP_TCL = tclVar(0.05);
FRAMESHIFT_BL_TCL = tclVar(0);

# Miru lineage settings
FUDGE_TCL = tclVar(15);

# Spoligotyping settings
MIN_PERC_SCORE_TCL = tclVar(0.9);
DR_START = tclVar("3119037");
DR_END = tclVar("3123624");

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
# Fun01: getReads
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
                  "fastq", ".fq"
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

   if(! is.na(dirMacStr[1]))
      tkconfigure(dirLabelTK, text = dirMacStr[1]);
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

   if(! is.na(fileMacStr[1]))
      tkconfigure(refLabelTK, text = fileMacStr[1]);
} # getRef

#---------------------------------------------------------
# Fun04: getAmrTbl
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
# Fun05: getMiruTbl
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
} # getMiruTbl

#---------------------------------------------------------
# Fun06: getSpoligoSeq
#   - Gets the spoligo space sequences for spoligotyping
# Input:
# Output:
#---------------------------------------------------------
getSpoligoSeq =
function(
){
   fileMacStr =
      tk_choose.files(
         default = getwd(), # start of in working dir
         caption ="Fasta file with spoligotype sequences",
         multi = FALSE,
         filters =
            matrix(
               c(
                  "fasta", ".fa",
                  "fasta", ".fasta"
                ), # allowed files (filters)
             1, # one row
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an AMR table

   if(! is.na(fileMacStr[1]))
      tkconfigure(spoligoSeq_LabTK, text = fileMacStr[1]);
} # getSpoliogSeq

#---------------------------------------------------------
# Fun07: getSpoligoDB
#   - Gets the lineage database for spoligotyping
# Input:
# Output:
#---------------------------------------------------------
getSpoligoDB =
function(
){
   fileMacStr =
      tk_choose.files(
         default = getwd(), # start of in working dir
         caption ="csv file with spoligotype lineages",
         multi = FALSE,
         filters =
            matrix(
               c(
                  "lineages", ".csv"
                ), # allowed files (filters)
             1, # one row
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an AMR table

   if(! is.na(fileMacStr[1]))
      tkconfigure(spoligoDb_LabTK, text = fileMacStr[1]);
} # getSpoligoDb

#---------------------------------------------------------
# Fun08: getGeneTbl
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
# Fun09: getMaskPrim
#   - Gets the primer masking tsv file
# Input:
# Output:
#---------------------------------------------------------
getMaskPrim =
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
                  "primer masking", ".tsv"
                ), # allowed files (filters)
             1, # one row
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an AMR table

   if(! is.na(fileMacStr[1]))
   { # If: The user input an textbox
      tkconfigure(maskPrim_LabTK, text = fileMacStr[1]);
      tkconfigure(maskPrim_checkboxTK, state = 0); # ??
   } # If: The user input an textbox
} # getGeneTbl

#---------------------------------------------------------
# Fun10: showReqInputMenu
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
   tkpack.forget(lineageInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   tkpack.forget(statusTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, reqInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showReqInputMenu

#---------------------------------------------------------
# Fun11: showFiltMenu
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
   tkpack.forget(lineageInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   tkpack.forget(statusTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, filtInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showReqInputMenu

#---------------------------------------------------------
# Fun12: showAmrMenu
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
   tkpack.forget(lineageInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   tkpack.forget(statusTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, amrInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showFileMenu

#---------------------------------------------------------
# Fun13: showLineageMenu
#   - Shows the MIRU lineage menu
# Input:
# Output:
#---------------------------------------------------------
showLineageMenu =
function(
){
   tkpack.forget(reqInputTK);
   tkpack.forget(filtInputTK);
   tkpack.forget(amrInputTK);
   tkpack.forget(lineageInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   tkpack.forget(statusTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, lineageInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showFileMenu

#---------------------------------------------------------
# Fun14: showConMenu
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
   tkpack.forget(lineageInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   tkpack.forget(statusTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, conInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showConMenu

#---------------------------------------------------------
# Fun15: showDepthMenu
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
   tkpack.forget(lineageInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   tkpack.forget(statusTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, depthInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showDepthMenu

#---------------------------------------------------------
# Fun16: showStatus
#   - Shows the status screen of freezeTB gui
# Input:
# Output:
#---------------------------------------------------------
showStatus =
function(
){
   tkpack.forget(reqInputTK);
   tkpack.forget(filtInputTK);
   tkpack.forget(amrInputTK);
   tkpack.forget(lineageInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(depthInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   tkpack.forget(statusTK);
   
   tkpack(statusTK, side = "top");
} # showStatus

#---------------------------------------------------------
# Fun17: quitInput
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
# Fun18: runFreezeTb
#   - Runs freezeTb
# Input:
# Output:
#   - Runs:
#     o Freeze tb
#---------------------------------------------------------
runFreezeTb =
function(
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Fun18 TOC:
   #   - Runs freezeTb on input files
   #   o fun18 sec01:
   #     - Variable declerations
   #   o fun18 sec02:
   #     - Check if user provided an fastq file
   #   o fun18 sec03:
   #     - Build the prefix and minimap2 command
   #   o fun18 sec04:
   #     - Build freezeTb command
   #   o fun18 sec05:
   #     - Get minimap2 version, open log, ann map reads
   #   o fun18 sec06:
   #     - Run adjCoords (if requested)
   #   o fun18 sec07:
   #     - Run freezeTb
   #   o fun18 sec08:
   #     - Return and exit
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun18 Sec01:
   #   - Variable declerations
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   # Set global varaibles
   logFileStr = ""; # version number and commands
   errSI = 0;

   fqFileStr = as.character(tkcget(fqLabelTK, "-text"));
   refFileStr = as.character(tkcget(refLabelTK, "-text"));

   maskPrimFileStr =
      as.character(tkcget(maskPrim_LabTK, "-text"));

   amrFileStr =
      as.character(tkcget(amrDbLabelTK, "-text"));

   miruFileStr =
      as.character(tkcget(miruDb_LabTK, "-text"));

   # Spoligotype settings
   spoligoSeqFileStr =
      as.character(tkcget(spoligoSeq_LabTK, "-text"));

   spoligoDBFileStr =
      as.character(tkcget(spoligoDb_LabTK, "-text"));

   drStartStr = as.character(tclvalue(DR_START));
   drEndStr = as.character(tclvalue(DR_END));

   depthFileStr =
      as.character(tkcget(depthGeneTbl_LabTK, "-text"));

   samStr = "";

   prefixStr = tkcget(dirLabelTK, "-text");

   argsMinimapStr = "";

   tbCmdStr = ""; # Should be in users path

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun18 Sec02:
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
   # Fun18 Sec03:
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
   # Fun18 Sec04:
   #   - Build freezeTb command
   #   o fun18 sec04 sub01:
   #     - Build read filtering part of freezeTb command
   #   o fun18 sec04 sub02:
   #     - Add AMR settings/check if AMR database exists
   #   o fun18 sec04 sub03:
   #     - Add MIRU lineage commands/check MIRU database
   #   o fun18 sec04 sub04:
   #     - Add in spoligotype lineage comands
   #   o fun18 sec04 sub05:
   #     - Add in the consensus commands
   #   o fun18 sec04 sub06:
   #     - Add in the read depth/coverage commands
   #   o fun18 sec04 sub07:
   #     - Add in the required input commands
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # Fun18 Sec04 Sub01:
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

   if(! as.numeric(tclvalue(MASK_PRIM_BL_TCL)))
      maskPrimFileStr = '-';

   if(length(maskPrimFileStr) == 0)
   { # If: No primer masking file was input
      tk_messageBox(
         type = "ok",
         message = "No primer masking file input",
         caption = "ERROR"
      );

      showFiltMenu();
      return();
   } # If: No primer masking file was input

   tbCmdStr =
      paste(
        tbCmdStr,
        "-mask-prim",
        maskPrimFileStr,
        sep=" "
   );

   #******************************************************
   # Fun18 Sec04 Sub02:
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
   # Fun18 Sec04 Sub03:
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

      showLineageMenu();
      return();
   } # If: I could not open the MIRU lineag table

   #******************************************************
   # Fun18 Sec04 Sub04:
   #   - Add in spoligotype lineage comands
   #******************************************************

   if(! grepl("^[0-9][0-9]*$", drStartStr))
   { # If: I have non-numeric coordinates for DR region
      tk_messageBox(
         type = "ok",
         message =
            "direct repeat start is not numeric",
         caption = "ERROR: Lineages"
      );

      showLineageMenu();
      return();
   } # If: I have non-numeric coordinates for DR region

   if(! grepl("^[0-9][0-9]*$", drEndStr))
   { # If: I have non-numeric coordinates for DR region
      tk_messageBox(
         type = "ok",
         message =
            "direct repeat end is not numeric",
         caption = "ERROR: Lineages"
      );

      showLineageMenu();
      return();
   } # If: I have non-numeric coordinates for DR region

   if(length(spoligoSeqFileStr) == 0 ||
      ! file.exists(spoligoSeqFileStr)
   ){ # If: no spoligotype file was input
      tkMessageStr =
         paste(
            "Spoligotype spacer sequence file",
            spoligoSeqFileStr,
            "could not be opened"
         );

      tk_messageBox(
         type = "ok",
         message = tkMessageStr,
         caption = "ERROR: Lineages"
      );

      showLineageMenu();
      return();
   } # If: no spoligotype file was input

   if(length(spoligoDBFileStr) > 0 &&
      ! file.exists(spoligoDBFileStr)
   ){ # If: invalid spoligotype lineage file input
      tkMessageStr =
         paste(
            "Could not open input spoligotype lineage",
            "database (",
            spoligoDBFileStr,
            ")",
            sep = ""
         );

      tk_messageBox(
         type = "ok",
         message = tkMessageStr,
         caption = "ERROR: Lineages"
      );

      showLineageMenu();
      return();
   } # If: invalid spoligotype lineage file input
 
   tbCmdStr =
      paste(
         tbCmdStr,
         "-spoligo",
         spoligoSeqFileStr,
         "-spoligo-min-score",
         tclvalue(MIN_PERC_SCORE_TCL),
         "-dr-start",
         drStartStr,
         "-dr-end",
         drEndStr
      ); # Add in the spoligotype commands

   if(length(spoligoDBFileStr))
   { # If: I have an spoligotype lineage file
      tbCmdStr =
         paste(tbCmdStr, "-db-spoligo", spoligoDBFileStr);
   } # If: I have an spoligotype lineage file

   #******************************************************
   # Fun18 Sec04 Sub05:
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
   # Fun18 Sec04 Sub06:
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
   # Fun18 Sec04 Sub07:
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
   # Fun18 Sec05:
   #   - Get minimap2 version, open log, ann map reads
   #   o fun18 sec05 Sub01:
   #     - Get and save version for minimap2 to log
   #   o fun18 sec05 Sub02:
   #     - Map reads
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # Fun18 Sec05 Sub01:
   #   - Get and save version for minimap2 to log
   #******************************************************

   showStatus();
   tkconfigure(messg_labTK, text = "running minimap2");

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
   # Fun18 Sec05 Sub02:
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
      tkdestroy(reportGuiTK);
      return();
   } # If: I could not run minimap2

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun18 Sec06:
   #   - Run adjCoords (if requested)
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(as.numeric(tclvalue(ADJ_COORDS_BL_TCL)))
   { # If: I am adjusting gene to reference coordinates
      tkconfigure(
         messg_labTK,
         text = "running adjCoords"
      );

      errSI = 
         system2(
            command = "adjCoords",
            args = 
               paste(
                  "-coords",
                  depthFileStr,
                  "-sam",
                  samStr,
                  "-ref",
                  as.character(ADJ_COORDS_REF_TCL),
                  "> tmp.sam",
                  sep = " "
               ),
            wait = TRUE,
            timeout = 0
         ); # Run adjCoords

      if(errSI)
      { # If: adjCoords errored out
         print("Error while running adjCoords");

         tk_messageBox(
            type = "ok",
            message = "Could run adjCoords",
            caption = "ERROR"
         );

         sink();
         tkdestroy(reportGuiTK);
         return();
      } # If: adjCoords errored out

      # move the adjusted file to the sam file
      file.rename(
         "tmp.sam",
          samStr
      );
   } # If: I am adjusting gene to reference coordinates

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun18 Sec07:
   #   - Run freezeTB
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tkconfigure(messg_labTK, text = "running freezeTB");
   print(paste("freezeTB", tbCmdStr, sep = " "));

   errStr = 
      system2(
         command = "freezeTB",# freezeTB command
         args = tbCmdStr,# arugments for minimap2
         wait = TRUE,        # wait till freezeTB finishes
         #stderr = TRUE,
         timeout = 0         # Do no time out
      ); # Run freezeTB

   if(errStr){
      showReqInputMenu();

      tbCmdStr =
         paste(
            "ERROR while running freezeTB",
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
   # Fun18 Sec08:
   #   - Return and exit
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   sink(); # Close the log

   quitInput(); # No longer need freezeTb gui
} # runFreezeTb

#---------------------------------------------------------
# Ingui TOC:
#   - The code that runs the GUI
#   o ingui01:
#     - Set up required input commands
#   o ingui02:
#     - Set up the hide/display extra options
#   o ingui03:
#     - Exit/run buttons
#   o ingui04:
#     - Read filtering menu
#   o ingui05:
#     - AMR detection settings menu
#   o ingui06:
#     - Lineage detection options
#   o ingui07:
#     - Set up the consnesus options menu
#   o ingui08:
#     - Read depth graphs options
#   o ingui09:
#     - Set up the gui
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui01:
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
# Ingui02:
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
      text = "Lineages",
      command = showLineageMenu
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
# Ingui03:
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
# Ingui04:
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


maskPrim_checkboxTK =
   tkcheckbutton(
      parent = filtInputTK,
      text = "Mask primers",
      variable = MASK_PRIM_BL_TCL
    );

if(TOOL_TIP_BL > 0){
   tk2tip(
      maskPrim_checkboxTK,
      "Mask primers using the mask primer file"
   );
} # If: I can make an tooltip
maskPrim_LabTK =
   tklabel(
      parent = filtInputTK,
      text = def_primMaskStr
   );

maskPrim_ButTK =
   tkbutton(
      parent = filtInputTK,
      text = "Mask primer file",
      command = getMaskPrim
   ); # Run freezeTb

if(TOOL_TIP_BL > 0){
   tk2tip(
      maskPrim_ButTK,
      "Tsv file to mask primer coordinates with"
   ); # Run freezeTb button tooltip
} # If: I can add an tooltip

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui05:
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
# Ingui06:
#   - Lineages detection options
#   o ingui06 sub01:
#     - Miru lineage detection
#   o ingui06 sub02:
#     - Spoligotype lineage detection
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Ingui06 Sub01:
#   - Miru lineage detection
#*********************************************************

miruTitle_LabTK =
   tklabel(
      miruVarTK,
      text = "MIRU lineage settings"
   );

miruDb_LabTK =
   tklabel(
      miruVarTK,
      text = def_miruTblStr
   ); # Current MIRU lineage table used

miruDb_ButTK =
   tkbutton(
      miruVarTK,
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
      miruVarTK,
      text = "Fudge factor"
   );

miruFudge_SlideTK =
   tkscale(
      miruVarTK,
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

#*********************************************************
# Ingui06 Sub02:
#   - Spoligotype lineage detection
#*********************************************************

spoligoTitle_LabTK =
   tklabel(
      spoligoVarTK,
      text = "Spoligotyping settings"
   );

spoligoSeq_LabTK =
   tklabel(
      spoligoVarTK,
      text = def_spoligoRefsStr
   );

spoligoSeq_ButTK =
   tkbutton(
      spoligoVarTK,
      text = "Spoligo sequences",
      command = getSpoligoSeq
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      spoligoSeq_ButTK,
      "Fasta file with spoligotype space sequences (DR)"
   );
} # If: I can add an tooltip

spoligoDb_LabTK =
   tklabel(
      spoligoVarTK,
      text = def_spoligoDbStr
   );

spoligoDb_ButTK =
   tkbutton(
      spoligoVarTK,
      text = "Spoligo database",
      command = getSpoligoDB
   );

# There is here so systems without tcltk2 can work
if(TOOL_TIP_BL > 0){
   tk2tip(
      spoligoDb_ButTK,
      "csv file (data base) with spoligotype lineages"
   );
} # If: I can add an tooltip

spoligoPerc_LabTK =
   tklabel(
      spoligoVarTK,
      text = "Min % score"
   );

spoligoPerc_SlideTK =
   tkscale(
      spoligoVarTK,
      from = 0,
      to = 1,
      showvalue = TRUE,
      variable = MIN_PERC_SCORE_TCL,
      resolution = 0.01,
      orient = "horiz"
   );

if(TOOL_TIP_BL > 0){
   tk2tip(
     spoligoPerc_SlideTK,
     "Minimum percent score to count an spacer as mapped"
   );
} # Add tooltip label

spoligoStart_LabTK =
   tklabel(
      spoligoVarTK,
      text = "DR start"
   );

spoligoStart_BoxTK =
   tkentry(
      spoligoVarTK,
      width = 12,
      textvariable = DR_START
   );

if(TOOL_TIP_BL > 0){
   tk2tip(
     spoligoStart_BoxTK,
     "Start of direct repeat (DR) region in reference"
   );
} # Add tooltip label

spoligoEnd_LabTK =
   tklabel(
      spoligoVarTK,
      text = "DR end"
   );

spoligoEnd_BoxTK =
   tkentry(
      spoligoVarTK,
      width = 12,
      textvariable = DR_END
   );

if(TOOL_TIP_BL > 0){
   tk2tip(
     spoligoEnd_BoxTK,
     "End of direct repeat (DR) region in reference"
   );
} # Add tooltip label

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Ingui07:
#   - Set up the consnesus options menu
#   o ingui07 frame-01:
#     - Consensus global (bulding) options
#   o ingui07 frame-02:
#     - Consensus collapsing (bulding) options
#   o ingui07 frame-03:
#     - Variant printing options for consensus
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Ingui07 Frame-01:
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
# Ingui07 Frame-02:
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
# Ingui07 Frame-03:
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
# Ingui08:
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

adjCoords_checkboxTK =
   tkcheckbutton(
     parent = depthInputTK,
     text = "Reference is genes (not full genome)",
     variable = ADJ_COORDS_BL_TCL
   );
     
if(TOOL_TIP_BL > 0){
   tk2tip(
      adjCoords_checkboxTK,
      "The reference fasta is a set of separate genes"
   );
} # If: I can make an tooltip

adjCoords_entryTK =
   tkentry(
      parent = depthInputTK,
      width = 12,
      textvariable = ADJ_COORDS_REF_TCL
    );

if(TOOL_TIP_BL > 0){
   tk2tip(
      adjCoords_entryTK,
      "Name of reference to adjust gene coordinates to"
   );
} # If: I can make an tooltip


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
# Ingui09:
#   - Set up the gui
#   o ingui09 sub01:
#     - Set up the required input frame
#   o ingui09 sub02:
#     - Set up the filtering menu frame
#   o ingui09 sub03:
#     - Set up the AMR menu frame
#   o ingui09 sub04:
#     - Set up the MIRU menu frame
#   o ingui09 sub05:
#     - Set up the consensus input frame
#   o ingui09 sub06:
#     - Set up the read depth graphing frame
#   o ingui09 sub07:
#     - Set up the menu buttons
#   o ingui09 sub08:
#     - Set up the run and quit buton frame
#   o ingui09 sub09:
#     - Pack all the frames together and run idle loop
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Ingui09 Sub01:
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
# Ingui09 Sub02:
#   - Set up the filtering menu frame
#*********************************************************

tkgrid(filtTitle_LabTK);
tkgrid(filtMinMapq_LabTK, filtMinMapq_SlideTK);
tkgrid(filtMinMedQ_LabTK, filtMinMedQ_SlideTK);
tkgrid(filtMinMeanQ_LabTK, filtMinMeanQ_SlideTK);
tkgrid(filtMinDepth_LabTK, filtMinDepth_SlideTK);
tkgrid(maskPrim_checkboxTK,maskPrim_ButTK,maskPrim_LabTK);

tkgrid.configure(
   filtTitle_LabTK,
   filtMinMapq_LabTK,
   filtMinMedQ_LabTK,
   filtMinMeanQ_LabTK,
   filtMinDepth_LabTK,
   maskPrim_checkboxTK,
   sticky = "w"
);

tkgrid.configure(
   filtMinMapq_SlideTK,
   filtMinMedQ_SlideTK,
   filtMinMeanQ_SlideTK,
   filtMinDepth_SlideTK,
   maskPrim_ButTK,
   maskPrim_LabTK,
   sticky = "w"
);

#*********************************************************
# Ingui09 Sub03:
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
# Ingui09 Sub04:
#   - Set up the lineages menu frame
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

tkgrid(spoligoTitle_LabTK);
tkgrid(spoligoSeq_ButTK, spoligoSeq_LabTK);
tkgrid(spoligoDb_ButTK, spoligoDb_LabTK);
tkgrid(spoligoPerc_LabTK, spoligoPerc_SlideTK);
tkgrid(spoligoStart_LabTK, spoligoStart_BoxTK);
tkgrid(spoligoEnd_LabTK, spoligoEnd_BoxTK);

tkgrid.configure(
   spoligoTitle_LabTK,
   spoligoDb_ButTK,
   spoligoPerc_SlideTK,
   spoligoPerc_LabTK,
   spoligoStart_LabTK,
   spoligoEnd_LabTK,
   sticky = "w"
);

tkgrid.configure(
   spoligoDb_LabTK,
   spoligoStart_BoxTK,
   spoligoEnd_BoxTK,
   sticky = "w"
);

tkpack(lineageInputTK, miruVarTK, side = "left");
tkpack(lineageInputTK, spoligoVarTK, side = "left");

#*********************************************************
# Ingui09 Sub05:
#   - Set up the consensus input frame
#   o ingui09 sub05 cat-01:
#     - Set up the global consensus options frame
#   o ingui09 sub05 cat-02:
#     - Set up the collapse consensus options frame
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Ingui09 Sub05 Cat-01:
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
# Ingui09 Sub05 Cat-02:
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
# Ingui09 Sub05 Cat-03:
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
# Ingui09 Sub05 Cat-03:
#   - Build the consensus options frames
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

tkpack(conInputTK, conGlobalInputTK, side = "top");
tkpack(conInputTK, conBuildInputTK, side = "left");
tkpack(conInputTK, conPrintInputTK, side = "right");

#*********************************************************
# Ingui09 Sub06:
#   - Set up the read depth graphing frame
#*********************************************************

tkgrid(depthTitle_LabTK);
tkgrid(depthGeneTbl_ButTK, depthGeneTbl_LabTK);
tkgrid(adjCoords_checkboxTK, adjCoords_entryTK);
tkgrid(ext_LabTK);
tkgrid(svgExt_radioTK);
tkgrid(tiffExt_radioTK);
tkgrid(pngExt_radioTK);
tkgrid(pdfExt_radioTK);
tkgrid(jpegExt_radioTK);

tkgrid.configure(
   depthTitle_LabTK,
   depthGeneTbl_ButTK,
   adjCoords_checkboxTK,
   adjCoords_entryTK,
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
# Ingui09 Sub07:
#   - Set up the menu buttons
#*********************************************************

tkpack(menuTK, inputMenu_ButTK, side = "left");
tkpack(menuTK, filtMenu_ButTK, side = "left");
tkpack(menuTK, amrMenu_ButTK, side = "left");
tkpack(menuTK, miruMenu_ButTK, side = "left");
tkpack(menuTK, conMenu_ButTK, side = "left");
tkpack(menuTK, depthMenu_ButTK, side = "left");

#*********************************************************
# Ingui09 Sub08:
#   - Set up the run and quit buton frame
#*********************************************************

tkpack(runQuitTK, runButTK, side = "left");
tkpack(runQuitTK, qButTK, side = "left");

#*********************************************************
# Ingui09 Sub09:
#   - Pack all the frames together and run idle loop
#*********************************************************

# Not in correct place, but is just a place holder
messg_labTK =
   tklabel(
      statusTK,
      text = "status"
   );
tkpack(statusTK, messg_labTK, side = "left");


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
