#!/usr/bin/env Rscript
# This locks onto Rscripts location

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o header:
#     - included libraries and general setup
#   o tof01:
#     - input gui functions (buttons/widgets)
#   o tof02:
#     - input gui hide/show menu functions + run
#   o tof03:
#     - functions to build input gui
#   o main:
#     - drive for freezeTBGui
#   o license:
#     - licensing for this code (public domain / mit)
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

slashSC = .Platform$file.sep;

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
   def_pathSplitSC = ':';

   def_rNameStr = "Rscript";

   def_altFilePathStr = "/usr/local/share/freezeTBFiles/";

   if("fs" %in% rownames(installed.packages())){
      def_filePathStr =
         fs::path_home("Documents", "freezeTBFiles");

      def_filePathStr =
         paste(def_filePathStr, slashSC, sep = "");
   } else{
      def_filePathStr = "$HOME/Documents/freezeTBFiles/";
   }
} else{
   def_pathSplitSC = ';';
   def_rNameStr = "Rscript.exe";

   def_altFilePathStr =
      paste(
         as.list(
            strsplit(
               Sys.getenv("PUBLIC"),
               def_pathSplitSC
             )
         )[[1]][1],
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

# two types of variables;
#   - def_ are defaults (never change)
#   - PathStr change with user input (called in code)

fqFilesStr = ""; # no default, user has to provide
outPathStr = ""; # default is nothing

def_amrTblStr =
   paste(def_filePathStr, "amrDb.tsv", sep = "");

if(! file.exists(def_amrTblStr)){
   def_amrTblStr =
      paste(def_altFilePathStr, "amrDb.tsv", sep = "");

   if(! file.exists(def_amrTblStr)){
      def_amrTblStr = "amrDb.tsv";

      if(! file.exists(def_amrTblStr))
         def_amrTblStr = "";
   } # If: I could not open the alternate path
} # If: I need to go to my aternate location

def_amrTblStr = normalizePath(def_amrTblStr);
amrTblPathStr = def_amrTblStr;


   
def_coordsStr =
  paste(def_filePathStr,"coords.tsv",sep="");

if(! file.exists(def_coordsStr)){
   def_coordsStr =
      paste(def_altFilePathStr, "coords.tsv", sep = "");

   if(! file.exists(def_coordsStr)){
      def_coordsStr = "coords.tsv";

      if(! file.exists(def_coordsStr))
         def_coordsStr = "";
   } # If: I could not open the altnerate path
} # If: I need to go to my aternate location

def_coordsStr = normalizePath(def_coordsStr);
coordsPathStr = def_coordsStr;



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

def_miruTblStr = normalizePath(def_miruTblStr);
miruTblPathStr = def_miruTblStr;


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

def_refStr = normalizePath(def_refStr);
refPathStr = def_refStr;


def_spolSpacersStr =
   paste(def_filePathStr, "spoligo-spacers.fa", sep="");

if(! file.exists(def_spolSpacersStr)){
   def_spolSpacersStr =
      paste(
         def_altFilePathStr,
         "spoligo-spacers.fa",
         sep = ""
      );

   if(! file.exists(def_spolSpacersStr)){
      def_spolSpacersStr = "spoligo-spacers.fa";

      if(! file.exists(def_spolSpacersStr))
         def_spolSpacersStr = "";
   } # If: I could not open the alternate path
} # If: I need to go to my aternate location

def_spolSpacersStr = normalizePath(def_spolSpacersStr);
spolSpacersPathStr = def_spolSpacersStr;


def_spolDbStr =
   paste(def_filePathStr, "spoligo-lineages.csv", sep="");

if(! file.exists(def_spolDbStr)){
   def_spolDbStr =
      paste(
         def_altFilePathStr,
         "spoligo-lineages.csv",
         sep = ""
      );

   if(! file.exists(def_spolDbStr)){
      def_spolDbStr = "spoligo-lineages.csv";

      if(! file.exists(def_spolDbStr))
         def_spolDbStr = "";
   } # If: I could not open the alternate path
} # If: I need to go to my aternate location

def_spolDbStr = normalizePath(def_spolDbStr);
spolDbPathStr = def_spolDbStr;


def_maskPrimStr =
   paste(def_filePathStr, "mask.tsv", sep="");

if(! file.exists(def_maskPrimStr)){
   def_maskPrimStr =
      paste(
         def_altFilePathStr,
         "mask.tsv",
         sep = ""
      );

   if(! file.exists(def_spolDbStr)){
      def_maskPrimStr = "mask.tsv";

      if(! file.exists(def_maskPrimStr))
         def_maskPrimStr = "";
   } # If: I could not open the alternate path
} # If: I need to go to my aternate location

def_maskPrimStr = normalizePath(def_maskPrimStr);
maskPrimPathStr = def_maskPrimStr;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec03:
#   - find program locations
#   o header sec03 sub01:
#     - get system path and add separator for program name
#   o header sec03 sub02:
#     - set up program names
#   o header sec03 sub03:
#     - search path for minimap2
#   o header sec03 sub04:
#     - search path for freezeTB
#   o header sec03 sub05:
#     - search path for ampDepth graphing script
#   o header sec03 sub06:
#     - find Rscript path
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
      ) # get system path
   ); # make sure is a list

for(siElm in 1:length(pathAryStr[[1]]))
{ # Loop: add diretory separator to end of path
   pathAryStr[[1]][siElm] =
      paste(
         pathAryStr[[1]][siElm],
         slashSC,
         sep = ''
      );
} # Loop: add diretory separator to end of path

#*********************************************************
# Header Sec03 Sub02:
#   - set up program names
#*********************************************************

graphAmpDepthPathStr = "graphAmpDepth.r";

if(.Platform$OS == "windows"){
   miniMapPathStr = "minimap2.exe";
   adjCoordsPathStr = "adjCoords.exe";
   freezeTBPathStr = "freezeTB.exe";

   freezeTBLocalStr =
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
   adjCoordsPathStr = "adjCoords";
   freezeTBPathStr = "freezeTB";

   freezeTBLocalStr =
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

for(strCall in c("", freezeTBLocalStr, pathAryStr[[1]]))
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

#*********************************************************
# Header Sec03 Sub04:
#   - search path for freezeTB
#*********************************************************

for(strCall in c("", freezeTBLocalStr, pathAryStr[[1]]))
{ # Loop: find freezeTB
   strCall =
      paste(
         strCall,
         freezeTBPathStr,
         sep = ''
      );

   if(file.exists(strCall)){
      freezeTBPathStr = normalizePath(strCall);
      break;
   } # If: found freezeTB
} # Loop: find freezeTB

#*********************************************************
# Header Sec03 Sub05:
#   - search path for graphAmpDepth.r
#*********************************************************

for(strCall in c("", freezeTBLocalStr, pathAryStr[[1]]))
{ # Loop: find graphAmpDepth.r
   strCall =
      paste(
         strCall,
         graphAmpDepthPathStr,
         sep = ''
      );

   if(file.exists(strCall)){
      graphAmpDepthPathStr = normalizePath(strCall);
      break;
   } # If: found graphAmpDepth.r
} # Loop: find graphAmpDepth.r

#*********************************************************
# Header Sec03 Sub06:
#   - find Rscript path
#*********************************************************

def_rPathStr =
   paste(
       dirname(commandArgs()[1]),
       slashSC,
       sep = ''
   ); # windows often has R.exe next to Rscript.exe

for(strCall in c("", def_rPathStr, pathAryStr[[1]]))
{ # Loop: find Rscript path
   strCall =
      paste(
          strCall,
          def_rNameStr,
          sep = ''
      );

   if(file.exists(strCall)){
      def_rPathStr = normalizePath(strCall);
      break;
   } # If: found rScript path
} # Loop: find Rscript path

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec04:
#   - declaring gui frames
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
graphInputTK = tkframe(optionsTK, borderwidth = 2);

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

spolVarTK =
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

# mixed infection detection
mixedInfectTK = tkframe(optionsTK, borderwidth = 2);

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Header Sec05:
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
MIN_DEPTH_TCL = tclVar(10);
MIN_LEN_TCL = tclVar(50);

# mixed infection defaults
CLUSTER_BL_TCL = tclVar(0);
DEPTH_PROFILE_BL_TCL = tclVar(0);
LENGTH_WEIGHT_CLUST_TCL = tclVar(2);
MIN_DEPTH_CLUST_TCL = tclVar(10);
MIN_PERC_DEPTH_CLUST_TCL = tclVar(0.005);
READ_ERROR_PERC_CLUST_TCL = tclVar(0.046);
CON_ERROR_PERC_CLUST_TCL = tclVar(0.023);
MAX_CON_SIMILARITAY_CLUST_TCL = tclVar(0.99);
ERROR_TO_VARIANT_RATIO_CLUST_TCL = tclVar(50);
MIN_PERC_OVERLAP_CLUST_TCL = tclVar(0.75);
WINDOW_LENGTH_CLUST_TCL = tclVar(500);
WINDOW_ERROR_TO_VAR_CLUST_TCL = tclVar(200);
MIN_INDEL_LEN_CLUST_TCL = tclVar(10);
CLUST_MIN_SNP_Q_TCL = tclVar(7);
MAX_MASK_IN_CON_CLUST_TCL = tclVar(0.05);

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

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# TOF01:
#   - input gui functions (buttons/widgets)
#   o tof01 fun01: getReads
#     - Gets a fastq file that the user selected
#   o tof01 fun02: getSaveDir
#     - Gets the directory to save everything to
#   o tof01 fun03: getRef
#     - Gets reference sequence fasta file form user
#   o tof01 fun04: getAmrTbl
#     - Gets an AMR table (tbAmr)
#   o tof01 fun05: getMiruTbl
#     - Gets an Miru table (tbMiru format)
#   o tof01 fun06: getSpoligoSeq
#     - Gets the spol space sequences for spoltyping
#   o tof01 fun07: getSpoligoDB
#     - Gets the lineage database for spoltyping
#   o tof01 fun08: getGeneTbl
#     - Gets the paf file with the genome coordinates
#   o tof01 fun09: getMaskPrim
#     - Gets the primer masking tsv file
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# required input gui
coordsTbl_LabTK = NULL;
fqLabelTK = NULL;
refLabelTK = NULL;
dirLabTK = NULL;

# masking gui
maskPrimTK = NULL;

# AMRs
amrDbLabelTK = NULL;

# lineages
miruDb_LabTK = NULL;
spolSeq_LabTK = NULL;
spolDb_LabTK = NULL;

# more global buttons
maskPrim_checkboxTK = NULL;
messg_labTK = NULL;

#---------------------------------------------------------
# TOF01 Fun01: getReads
#   - Gets a fastq file that the user selected
# Input:
# Output:
#   - Returns:
#     o An string with the selected file name
#---------------------------------------------------------
getReads =
function(
){
   siElm = 1;
   lenSI = 0;

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
                  "fastq*", ".gz" # mac does not like .*.
                ), # allowed files (filters)
             3, # four rows
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an fastq file

   if(! is.na(fileMacStr[1])){

      # this is needed to remove the directory that is
      # added with multi=TRUE file option
      
      siElm = 1;
      lenSI = length(fileMacStr);

      while(siElm <= lenSI)
      { # Loop: remove directories
         if(dir.exists(fileMacStr[siElm])){
            fileMacStr = fileMacStr[- 1];
            lenSI = lenSI - 1;
         }else if(! file.exists(fileMacStr[siElm])){
            fileMacStr = fileMacStr[- 1];
            lenSI = lenSI - 1;
         } else{
            siElm = siElm + 1;
         } # check if is a directory (not a file)

      } # Loop: remove directories

      if(length(fileMacStr) < 1)
         return(); # list only had directories

      assign(
         "fqFilesStr",
         normalizePath(fileMacStr),
         envir = .GlobalEnv
      ); # set up global variable for user input

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
                  slashSC,
                  "*.fastq*",
                  sep = ""
               )
          ); # add * to specify multiple files
      } # Else: have multipe files
   } # If: have files
} # getReads

#---------------------------------------------------------
# TOF01 Fun02: getSaveDir
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
# TOF01 Fun03: getRef
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
# TOF01 Fun04: getAmrTbl
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

   if(! is.na(fileMacStr[1])){
      assign(
         "amrTblPathStr",
         normalizePath(fileMacStr[1]),
         envir = .GlobalEnv
      ); # set up global variable for user input

      tkconfigure(
         amrDbLabelTK,
         text = fileMacStr[1]
       );
   } # If: AMR database input
} # getAmrTbl

#---------------------------------------------------------
# TOF01 Fun05: getMiruTbl
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

   if(! is.na(fileMacStr[1])){
      assign(
         "miruTblPathStr",
         normalizePath(fileMacStr[1]),
         envir = .GlobalEnv
      ); # set up global variable for user input

      tkconfigure(
         miruDb_LabTK,
         text = fileMacStr[1]
      );
   } # If: miru table input
} # getMiruTbl

#---------------------------------------------------------
# TOF01 Fun06: getSpoligoSeq
#   - Gets the spol space sequences for spoltyping
# Input:
# Output:
#---------------------------------------------------------
getSpoligoSeq =
function(
){
   fileMacStr =
      tk_choose.files(
         default = getwd(), # start of in working dir
         caption ="Fasta file with spoltype sequences",
         multi = FALSE,
         filters =
            matrix(
               c(
                  "fasta", ".fa",
                  "fasta", ".fasta"
                ), # allowed files (filters)
             2, # one row
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an AMR table

   if(! is.na(fileMacStr[1])){
      assign(
         "spolSpacersPathStr",
         normalizePath(fileMacStr[1]),
         envir = .GlobalEnv
      ); # set up global variable for user input

      tkconfigure(
         spolSeq_LabTK,
         text = fileMacStr[1]
      );
   } # If: have spoltype spacer sequences
} # getSpoliogSeq

#---------------------------------------------------------
# TOF01 Fun07: getSpoligoDB
#   - Gets the lineage database for spoltyping
# Input:
# Output:
#---------------------------------------------------------
getSpoligoDB =
function(
){
   fileMacStr =
      tk_choose.files(
         default = getwd(), # start of in working dir
         caption ="csv file with spoltype lineages",
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

   if(! is.na(fileMacStr[1])){
      assign(
         "spolDbPathStr",
         normalizePath(fileMacStr[1]),
         envir = .GlobalEnv
      ); # set up global variable for user input

      tkconfigure(
         spolDb_LabTK,
         text = fileMacStr[1]
      );
   } # If: have spoltype lineage database
} # getSpoligoDb

#---------------------------------------------------------
# TOf01 Fun08: getGeneTbl
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

   if(! is.na(fileMacStr[1])){
      assign(
         "coordsPathStr",
         normalizePath(fileMacStr[1]),
         envir = .GlobalEnv
      ); # set up global variable for user input

      tkconfigure(
         coordsTbl_LabTK,
         text = fileMacStr[1]
      );
   } # If: have gene coordinates file
} # getGeneTbl

#---------------------------------------------------------
# TOF01 Fun09: getMaskPrim
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
      assign(
         "maskPrimPathStr",
         normalizePath(fileMacStr[1]),
         envir = .GlobalEnv
      ); # set up global variable for user input

      tkconfigure(
         maskPrim_LabTK,
         text = fileMacStr[1]
      );

      if(! as.numeric(tclvalue(MASK_PRIM_BL_TCL)))
         tktoggle(maskPrim_checkboxTK);

      tkconfigure(
         maskPrim_checkboxTK,
         state = "normal"
       ); # wish it worked
   } # If: The user input an textbox
} # getGeneTbl

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# TOF02:
#   - input gui hide/show menu functions + run
#   o tof02 fun01: showReqInputMenu
#     - Shows the required input menu
#   o tof02 fun02: showFiltMenu
#     - Shows the read filtering input menu
#   o tof02 fun03: showAmrMenu
#     - Shows the AMR settings menu
#   o tof02 fun04: showLineageMenu
#     - Shows the MIRU lineage menu
#   o tof02 fun05: showConMenu
#     - Shows the extra settings for consensus building
#   o tof02 fun06: showMixedInfectMenu
#     - Shows settings for mixed infection
#   o tof02 fun07: showGraphMenu
#     - Shows the depth/coverage graphing menu
#   o tof02 fun08: showStatus
#     - Shows the status screen of freezeTB gui
#   o tof02 fun09: quitInput
#     - Quits freezeTb input gui
#   o tof02 fun10: runFreezeTb
#     - Runs freezeTb
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# TOF02 Fun01: showReqInputMenu
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
   tkpack.forget(mixedInfectTK);
   tkpack.forget(graphInputTK);

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
# TOF02 Fun02: showFiltMenu
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
   tkpack.forget(mixedInfectTK);
   tkpack.forget(graphInputTK);

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
# TOF02 Fun03: showAmrMenu
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
   tkpack.forget(mixedInfectTK);
   tkpack.forget(graphInputTK);

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
# TOF02 Fun04: showLineageMenu
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
   tkpack.forget(mixedInfectTK);
   tkpack.forget(graphInputTK);

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
# TOF02 Fun05: showConMenu
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
   tkpack.forget(mixedInfectTK);
   tkpack.forget(graphInputTK);

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
# TOF02 Fun06: showMixedInfectMenu
#   - Shows settings for mixed infection
# Input:
# Output:
#---------------------------------------------------------
showMixedInfectMenu =
function(
){
   tkpack.forget(reqInputTK);
   tkpack.forget(filtInputTK);
   tkpack.forget(amrInputTK);
   tkpack.forget(lineageInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(mixedInfectTK);
   tkpack.forget(graphInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   tkpack.forget(statusTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, mixedInfectTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showMixedInfectMenu

#---------------------------------------------------------
# TOF02 Fun07: showGraphMenu
#   - Shows the depth/coverage graphing menu
# Input:
# Output:
#---------------------------------------------------------
showGraphMenu =
function(
){
   tkpack.forget(reqInputTK);
   tkpack.forget(filtInputTK);
   tkpack.forget(amrInputTK);
   tkpack.forget(lineageInputTK);
   tkpack.forget(conInputTK);
   tkpack.forget(mixedInfectTK);
   tkpack.forget(graphInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   tkpack.forget(statusTK);
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, graphInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showGraphMenu

#---------------------------------------------------------
# TOF02 Fun08: showStatus
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
   tkpack.forget(mixedInfectTK);
   tkpack.forget(graphInputTK);

   # I am going to rebuild the gui so everything is in
   # the correct order
   tkpack.forget(menuTK);
   tkpack.forget(optionsTK);
   tkpack.forget(runQuitTK);
   tkpack.forget(statusTK);
   
   tkpack(statusTK, side = "top");
} # showStatus

#---------------------------------------------------------
# TOF02 Fun09: quitInput
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
# TOF02 Fun10: runFreezeTb
#   - Runs freezeTb
# Input:
# Output:
#   - Runs:
#     o Freeze tb
#---------------------------------------------------------
runFreezeTb =
function(
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF02 Fun10 TOC:
   #   - Runs freezeTb on input files
   #   o tof02 fun10 sec01:
   #     - Variable declerations
   #   o tof02 fun10 sec02:
   #     - Check if user provided an fastq file
   #   o tof02 fun10 sec03:
   #     - Build the prefix and minimap2 command
   #   o tof02 fun10 sec04:
   #     - Build freezeTb command
   #   o tof02 fun10 sec05:
   #     - Get minimap2 version, open log, ann map reads
   #   o tof02 fun10 sec06:
   #     - Run adjCoords (if requested)
   #   o tof02 fun10 sec07:
   #     - Run freezeTb
   #   o tof02 fun10 sec08:
   #     - Return and exit
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF02 Fun10 Sec01:
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

   amrFileStr =
      normalizePath(
         get(
            "amrTblPathStr",
            envir = .GlobalEnv
          )
       );

   miruFileStr =
      normalizePath(
         get(
            "miruTblPathStr",
            envir = .GlobalEnv
         )
      );

   coordsFileStr =
      normalizePath(
         get(
             "coordsPathStr",
             envir = .GlobalEnv
         )
      );


   maskPrimFileStr =
      normalizePath(
         get(
            "maskPrimPathStr",
            envir = .GlobalEnv
         )
      );

   # Spoligotype settings
   spolSeqFileStr =
      normalizePath(
         get(
             "spolSpacersPathStr",
             envir = .GlobalEnv
          )
      );

   spolDBFileStr =
      normalizePath(
         get(
             "spolDbPathStr",
             envir = .GlobalEnv
         )
      );

   drStartStr = as.character(tclvalue(DR_START));
   drEndStr = as.character(tclvalue(DR_END));

   samStr = "";

   prefixStr = tkcget(dirLabelTK, "-text");

   argsMinimapStr = "";

   tbCmdStr = ""; # Should be in users path

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF02 Fun10 Sec02:
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
   # TOF02 Fun10 Sec03:
   #   - Build the prefix and minimap2 command
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   prefixStr =
      paste(
         prefixStr,
         tclvalue(PREFIX_TCL),
         sep = slashSC
      ); # Paste new directory name

   if(! dir.exists(prefixStr)){
      dir.create(prefixStr);

      if(! dir.exists(prefixStr)){
         tk_messageBox(
            type = "ok",
            message =
               paste(
                  "Could not make",
                  prefixStr,
                  "directory",
                  sep = " "
               ),
            caption = "ERROR: make directory"
         ); # Message box for reference error

         showReqInputMenu();
         return();
      } # If: failed to make new directory
   } # If: need to make a new directory

   prefixStr =
      paste(
         prefixStr,
         tclvalue(PREFIX_TCL),
         sep = slashSC
      ); # Paste prefix to output file names
   
   logFileStr =
      paste(
         prefixStr,
         "-cmds-and-versions.txt",
         sep = ""
      ); # Make the version number file

   samStr = paste(prefixStr, "-map.sam", sep = "");

   argsMinimapStr =
      paste(
         ' -a -x map-ont -o "',
         samStr,
         '" "',
         refFileStr,
         '"',
         " ",
         paste(
            paste(
               '"',
               fqFileStr,
               '"',
               sep = ""
            ), # add "'s to guard spaces
            collapse = ' '
         ), # combine array into one string
         sep = ""
      ); # Build the minimap2 command

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF02 Fun10 Sec04:
   #   - Build freezeTb command
   #   o tof02 fun10 sec04 sub01:
   #     - Build read filtering part of freezeTb command
   #   o tof02 fun10 sec04 sub02:
   #     - Add AMR settings/check if AMR database exists
   #   o tof02 fun10 sec04 sub03:
   #     - Add MIRU lineage commands/check MIRU database
   #   o tof02 fun10 sec04 sub04:
   #     - Add in spoltype lineage comands
   #   o tof02 fun10 sec04 sub05:
   #     - Add in the consensus commands
   #   o tof02 fun10 sec04 sub06:
   #     - mixed infection (clustering) commands
   #   o tof02 fun10 sec04 sub07:
   #     - Add in the read depth/coverage commands
   #   o tof02 fun10 sec04 sub08:
   #     - Add in the required input commands
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF02 Fun10 Sec04 Sub01:
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

   if(file.exists(maskPrimFileStr)){
      tbCmdStr =
         paste(
           tbCmdStr,
           " -mask-prim \"",
           maskPrimFileStr,
           "\"",
           sep=""
      );
   }else{
      tbCmdStr =
         paste(
           tbCmdStr,
           "-mask-prim -",
           sep=" "
      );
   }

   #******************************************************
   # TOF02 Fun10 Sec04 Sub02:
   #   - Add AMR settings/check if AMR database exists
   #******************************************************

   # AMR settings
   tmpCmdStr =
       paste(
         tbCmdStr,
         " -amr-tbl \"",
         amrFileStr,
         "\"",
         " -min-amr-map-perc ",
         tclvalue(MIN_AMR_SUP_TCL),
         sep = ""
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
   # TOF02 Fun10 Sec04 Sub03:
   #   - Add in MIRU lineage commands/check MIRU database
   #******************************************************

   tbCmdStr =
      paste(
         tbCmdStr,
         " -fudge ",
         tclvalue(FUDGE_TCL),
         " -miru-tbl \"",
         miruFileStr,
         "\"",
         sep = ""
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
   # TOF02 Fun10 Sec04 Sub04:
   #   - Add in spoltype lineage comands
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

   if(length(spolSeqFileStr) == 0 ||
      ! file.exists(spolSeqFileStr)
   ){ # If: no spoltype file was input
      tkMessageStr =
         paste(
            "Spoligotype spacer sequence file",
            spolSeqFileStr,
            "could not be opened"
         );

      tk_messageBox(
         type = "ok",
         message = tkMessageStr,
         caption = "ERROR: Lineages"
      );

      showLineageMenu();
      return();
   } # If: no spoltype file was input

   if(length(spolDBFileStr) > 0 &&
      ! file.exists(spolDBFileStr)
   ){ # If: invalid spoltype lineage file input
      tkMessageStr =
         paste(
            "Could not open input spoltype lineage",
            "database (",
            spolDBFileStr,
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
   } # If: invalid spoltype lineage file input
 
   tbCmdStr =
      paste(
         tbCmdStr,
         " -spoligo \"",
         spolSeqFileStr,
         "\"",
         " -spoligo-min-score ",
         tclvalue(MIN_PERC_SCORE_TCL),
         " -dr-start ",
         drStartStr,
         " -dr-end ",
         drEndStr,
         sep = ""
      ); # Add in the spoltype commands

   if(length(spolDBFileStr))
   { # If: I have an spoltype lineage file
      tbCmdStr =
         paste(
            tbCmdStr,
            " -db-spoligo \"",
            spolDBFileStr,
            "\"",
            sep = ""
         );
   } # If: I have an spoltype lineage file

   #******************************************************
   # TOF02 Fun10 Sec04 Sub05:
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
         tclvalue(PRINT_DEL_PERC_SUP_TCL),
         sep = " "
      );

   #******************************************************
   # TOF02 Fun10 Sec04 Sub06:
   #   - mixed infection (clustering) commands
   #******************************************************

   if(as.numeric(tclvalue(CLUSTER_BL_TCL))){
      tbCmdStr = paste(tbCmdStr, "-clust", sep = " ");
   }else{
      tbCmdStr = paste(tbCmdStr, "-no-clust", sep = " ");
   } # check if clustering
    
   if(as.numeric(tclvalue(DEPTH_PROFILE_BL_TCL))){
      tbCmdStr =paste(tbCmdStr, "-depth-prof", sep = " ");
   }else{
      tbCmdStr =
         paste(tbCmdStr, "-no-depth-prof", sep = " ");
   } # check if doing depth profiling

   tbCmdStr =
      paste(
         tbCmdStr,
         "-len-weight",
         tclvalue(LENGTH_WEIGHT_CLUST_TCL),
         "-clust-depth",
         tclvalue(MIN_DEPTH_CLUST_TCL),
         "-clust-perc-depth",
         tclvalue(MIN_PERC_DEPTH_CLUST_TCL),
         "-read-err",
         tclvalue(READ_ERROR_PERC_CLUST_TCL),
         "-con-err",
         tclvalue(CON_ERROR_PERC_CLUST_TCL),
         "-con-sim",
         tclvalue(MAX_CON_SIMILARITAY_CLUST_TCL),
         "-err-to-var",
         tclvalue(ERROR_TO_VARIANT_RATIO_CLUST_TCL),
         "-overlap",
         tclvalue(MIN_PERC_OVERLAP_CLUST_TCL),
         "-win-len",
         tclvalue(WINDOW_LENGTH_CLUST_TCL),
         "-win-err",
         tclvalue(WINDOW_ERROR_TO_VAR_CLUST_TCL),
         "-indel-len",
         tclvalue(MIN_INDEL_LEN_CLUST_TCL),
         "-clust-q-snp",
         tclvalue(CLUST_MIN_SNP_Q_TCL),
         "-perc-n",
         tclvalue(MAX_MASK_IN_CON_CLUST_TCL),
         sep = " "
      );

   #******************************************************
   # TOF02 Fun10 Sec04 Sub07:
   #   - Add in the read depth/coverage commands
   #******************************************************

   tbCmdStr =
      paste(
         tbCmdStr,
         " -gene-coords \"",
         coordsFileStr,
         "\"",
         sep = ""
      ); # Build the freezeTb command up

   if(length(coordsFileStr) == 0 ||
      ! file.exists(coordsFileStr)
   ){ # If: The gene coordinates file does not exist
      tbCmdStr =
         paste(
            "Could not open gene coordinates table",
             coordsFileStr
         );
            
      tk_messageBox(
         type = "ok",
         message = tbCmdStr,
         caption = "ERROR"
      );

      showGraphMenu();
      return();
   } # If: The gene coordinates file does not exist

   # add in if adjusting coordiantes
   if(as.numeric(tclvalue(ADJ_COORDS_BL_TCL))){
      tbCmdStr =
         paste(
            tbCmdStr,
            "-adj-coords",
            sep = " "
         );
   }else{
      tbCmdStr =
         paste(
            tbCmdStr,
            "-no-adj-coords",
            sep = " "
         );
   }

   #******************************************************
   # TOF02 Fun10 Sec04 Sub08:
   #   - Add in the required input commands
   #******************************************************

   tbCmdStr =
      paste(
         tbCmdStr,
         " -sam \"",
         samStr,
         "\"",
         " -prefix \"",
         prefixStr,
         "\"",
         sep = ""
      ); # finsh off the command

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF02 Fun10 Sec05:
   #   - Get minimap2 version, open log, ann map reads
   #   o tof02 fun10 sec05 Sub01:
   #     - Get and save version for minimap2 to log
   #   o tof02 fun10 sec05 Sub02:
   #     - Map reads
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF02 Fun10 Sec05 Sub01:
   #   - Get and save version for minimap2 to log
   #******************************************************

   showStatus();
   tkconfigure(messg_labTK, text = "running minimap2");

   errSI =
      system2(
         command =
         get("miniMapPathStr", envir = .GlobalEnv),
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

      showReqInputMenu();

      return(0);
   } # If: I could not run minimap2

   versStr = 
      system2(
         command =
           get(
              "miniMapPathStr",
              envir = .GlobalEnv
           ), # minimap2 call
         args = "--version",
         wait = TRUE,
         timeout = 1,
         stdout = TRUE # Get output from stdout
      ); # Get the version number for minimap2

   sink(logFileStr);  # Open the log
   print(paste("Minimap2:", versStr, sep = " "));
   print(paste("minimap2", argsMinimapStr, sep = " "));

   #******************************************************
   # TOF02 Fun10 Sec05 Sub02:
   #   - Map reads
   #******************************************************
   
   errSI = 
      system2(
         command =
            get("miniMapPathStr", envir = .GlobalEnv),
         args = argsMinimapStr,# arugments for minimap2
         wait = TRUE,         # wait till miniMap finishes
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
      showReqInputMenu();
      return();
   } # If: I could not run minimap2

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF02 Fun10 Sec07:
   #   - Run freezeTB
   #   o tof02 fun10 sec07 sub01:
   #     - get freezeTB version
   #   o tof02 fun10 sec07 sub02:
   #     - run freezeTB
   #   o tof02 fun10 sec07 sub03:
   #     - build graphs for freezeTB
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF02 Fun10 Sec07 Sub01:
   #   - get freezeTB version
   #******************************************************

   tkconfigure(messg_labTK, text = "running freezeTB");

   errStr = 
      system2(
         command =
            get("freezeTBPathStr", envir = .GlobalEnv),
         args = "--version", # printing version
         wait = TRUE,        # wait till freezeTB finishes
         timeout = 1         # Do no time out
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

      showReqInputMenu();

      sink();
      return();
   } # If: I had an error running freezeTb

   print(errStr); # print the version numbers

   #******************************************************
   # TOF02 Fun10 Sec07 Sub02:
   #   - run freezeTB
   #******************************************************

   # print command given to freezeTB
   print(paste("freezeTB", tbCmdStr, sep = " "));

   errStr = 
      system2(
         command =
            get("freezeTBPathStr", envir = .GlobalEnv),
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

      showReqInputMenu();

      sink();
      return();
   } # If: I had an error running freezeTb

   #******************************************************
   # TOF02 Fun10 Sec07 Sub03:
   #   - build graphs for freezeTB
   #******************************************************

   if(
      file.exists(
         get(
            "graphAmpDepthPathStr",
             envir = .GlobalEnv
         )
       )
   ){ # If: can make graphs
      tbCmdStr =
         paste(
            "\"",
            get(
               "graphAmpDepthPathStr",
               envir = .GlobalEnv
            ),
            "\"",
            " -ext ",
            tclvalue(GRAPH_EXT_TCL),
            " -prefix \"",
            prefixStr,
            "\"",
            " -who \"",
            amrFileStr,
            "\"",
            " -stats ",
            paste(
               "\"",
               prefixStr,
               "-depths.tsv\"",
               sep = ''
            ), # make output stats file name
            sep = ""
         ) # rebulid graphing command to print

      system2(
            command =
               get(
                  "def_rPathStr",
                  envir = .GlobalEnv
               ),
         args = tbCmdStr,
         wait = TRUE,        # wait till freezeTB finishes
         timeout = 0         # do no time out
      ); # run graphAmpDepth.r

      showReqInputMenu();
   } # If: can make graphs

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF02 Fun10 Sec08:
   #   - Return and exit
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   sink(); # Close the log

   assign(
      "fqFilesStr",
      "",
      envir = .GlobalEnv
   ); # blank fastq entries

   tkconfigure(
      fqLabelTK,
      text = ""
   ); # remove last run fastq file(s)

   #quitInput(); # No longer need freezeTb gui
     # just keep running
} # runFreezeTb

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# TOF03:
#   - input gui hide/show menu functions + run
#   o tof03 fun01: reqInputSetup_freezeTBGui
#     - builds required input part of input gui
#   o tof03 fun02: menuSetup_freezeTBGui
#     - builds menu part of input gui
#   o tof03 fun03: exitRunSetup_freezeTBGui
#     - adds exit and run commands to input gui
#   o tof03 fun04: filtMenuSetup_freezeTBGui
#     - builds the filter settings frame
#   o tof03 fun05: amrMenuSetup_freezeTBGui
#     - builds the AMR settings frame
#   o tof03 fun06: lineageMenuSetup_freezeTBGui
#     - builds the lineage settings frame
#   o tof03 fun07: conMenuSetup_freezeTBGui
#     - builds the consensus settings frame
#   o tof03 fun08: mixefInfectSetup_freezeTBGui
#     - builds mixed infections settings frame
#   o tof03 fun09: graphMenuSetup_freezeTBGui
#     - builds the graph settings frame
#   o tof03 fun10: launchInGui_freezeTBGui
#     - launches input gui
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# TOF03 Fun01: reqInputSetup_freezeTBGui
#   - builds required input part of input gui
# Input:
# Output:
#   - Modifies:
#     o reqInputTK frame to have required input
#---------------------------------------------------------
reqInputSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF03 Fun01 TOC:
   #   - builds required input part of input gui
   #   o tof03 fun01 sec01:
   #     - add options to menu
   #   o tof03 fun01 sec02:
   #     - build required input menu
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun01 Sec01:
   #   - add options to menu
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   assign(
      "refLabelTK",
      tklabel(
         reqInputTK,
         text = def_refStr
      ), # Current reference file used
      envir = .GlobalEnv
   );
   
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


   adjCoords_checkboxTK =
      tkcheckbutton(
        parent = reqInputTK,
        text = "adjust reads by coordinates",
        variable = ADJ_COORDS_BL_TCL
      );
        
   if(TOOL_TIP_BL > 0){
      tk2tip(
         adjCoords_checkboxTK,
         paste(
            "Need to adjust coordinates (input",
            "reference(s) are not full genome)"
         )
      );
   } # If: I can make an tooltip

   coordsTbl_ButTK =
      tkbutton(
         reqInputTK,
         text = "coordinates",
         command = getGeneTbl
   ); # add the change gene coordiantes file button
   
   # There is here so systems without tcltk2 can work
   if(TOOL_TIP_BL > 0){
      tk2tip(
         coordsTbl_ButTK,
         "gene coordinates to adjust reference with"
      ); # gene table button tootip
   } # If: I can add an tooltip
   
   assign(
      "coordsTbl_LabTK",
      tklabel(
         reqInputTK,
         text = def_coordsStr
      ), # Current gene coordiantes table used
      envir = .GlobalEnv
   );

   
   assign(
      "fqLabelTK",
      tklabel(
         reqInputTK,
         text = ""
      ), # Holds the selected fastq file
      envir = .GlobalEnv
   );
   
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
         "Fastq file with reads to check AMRs/spoltypes"
      ); # fastq button tooltip
   } # If: I can add an tooltip
   
   assign(
      "dirLabelTK",
      tklabel(
         reqInputTK,
         text = getwd()
      ), # Holds the output directory
      envir = .GlobalEnv
   );
   
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

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun01 Sec02:
   #   - build required input menu
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tkgrid(refButTK, refLabelTK);

   tkgrid(adjCoords_checkboxTK);

   tkgrid(
      coordsTbl_ButTK,
      coordsTbl_LabTK
   );

   tkgrid(fqButTK, fqLabelTK);
   tkgrid(dirButTK, dirLabelTK);
   tkgrid(prefLabelTK, prefBoxTK);
   
   tkgrid.configure(
      refButTK,
      refLabelTK,
      adjCoords_checkboxTK,
      coordsTbl_ButTK,
      coordsTbl_LabTK,
      fqLabelTK,
      fqButTK,
      dirLabelTK,
      dirButTK,
      prefBoxTK,
      prefLabelTK,
      sticky = "w"
   ); # Configure the left side input buttons/prefix label
} # reqInpuSetup_freezeTBGui

#---------------------------------------------------------
# TOF03 Fun02: menuSetup_freezeTBGui
#   - builds menu part of input gui
# Input:
# Output:
#   - Modifies:
#     o reqInputTK frame to have menu buttons
#---------------------------------------------------------
menuSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF03 Fun02 TOC:
   #   - builds menu part of input gui
   #   o tof03 fun02 sec01:
   #     - add menu buttons to gui
   #   o tof03 fun02 sec02:
   #     - build menu
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun02 Sec01:
   #   - add menu buttons to gui
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

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
   
   graphMenu_ButTK =
      tkbutton(
         menuTK,
         text = "Graphing",
   		command = showGraphMenu
      ); # Show the graping menu
   
   # There is here so systems without tcltk2 can work
   if(TOOL_TIP_BL > 0){
      tk2tip(
         graphMenu_ButTK,
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

   mixedInfect_ButTK =
      tkbutton(
         menuTK,
         text = "mixed infection",
         command = showMixedInfectMenu
      ); # Show the extra consensus options
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         mixedInfect_ButTK,
         "mixed infection detection settings"
      ); # mixed infection settings tooltip
   } # If: I have tooltips

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun02 Sec02:
   #   - build menu
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tkpack(menuTK, inputMenu_ButTK, side = "left");
   tkpack(menuTK, filtMenu_ButTK, side = "left");
   tkpack(menuTK, amrMenu_ButTK, side = "left");
   tkpack(menuTK, miruMenu_ButTK, side = "left");
   tkpack(menuTK, conMenu_ButTK, side = "left");
   tkpack(menuTK, mixedInfect_ButTK, side = "left");
   tkpack(menuTK, graphMenu_ButTK, side = "left");
} # menuSetup_freezeTBGui

#---------------------------------------------------------
# TOF03 Fun03: exitRunSetup_freezeTBGui
#   - adds exit and run commands to input gui
# Input:
# Output:
#   - Modifies:
#     o reqInputTK frame to have exit and run buttons
#---------------------------------------------------------
exitRunSetup_freezeTBGui = function()
{
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

   tkpack(runQuitTK, runButTK, side = "left");
   tkpack(runQuitTK, qButTK, side = "left");
} # exitRunSetup_freezeTBGui

#---------------------------------------------------------
# TOF03 Fun04: filtMenuSetup_freezeTBGui
#   - builds the filter settings frame
# Input:
# Output:
#   - Modifies:
#     o reqInputTK frame to have exit and run buttons
#---------------------------------------------------------
filtMenuSetup_freezeTBGui = function()
{ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # TOF03 Fun04 TOC:
  #   - builds the filter settings frame
  #   o tof03 fun04 sec01:
  #     - set up filtering options
  #   o tof03 fun04 sec02:
  #     - add filtering options to gui
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  # TOF03 Fun04 Sec01:
  #   - set up filtering options
  #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

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
   
   
   assign(
      "maskPrim_checkboxTK",
      tkcheckbutton(
         parent = filtInputTK,
         text = "Mask primers",
         variable = MASK_PRIM_BL_TCL
       ),
       envir = .GlobalEnv
   );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         maskPrim_checkboxTK,
         "Mask primers using the mask primer file"
      );
   } # If: I can make an tooltip

   assign(
      "maskPrim_LabTK",
      tklabel(
         parent = filtInputTK,
         text = def_maskPrimStr
      ),
      envir = .GlobalEnv
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

  #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  # TOF03 Fun04 Sec02:
  #   - add filtering options to gui
  #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tkgrid(filtTitle_LabTK);
   tkgrid(filtMinMapq_LabTK, filtMinMapq_SlideTK);
   tkgrid(filtMinMedQ_LabTK, filtMinMedQ_SlideTK);
   tkgrid(filtMinMeanQ_LabTK, filtMinMeanQ_SlideTK);
   tkgrid(filtMinDepth_LabTK, filtMinDepth_SlideTK);

   tkgrid(
      maskPrim_checkboxTK,
      maskPrim_ButTK,
      maskPrim_LabTK
   );
   
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

} # filtMenuSetup_freezeTBGui

#---------------------------------------------------------
# TOF03 Fun05: amrMenuSetup_freezeTBGui
#   - builds AMR settings frame
# Input:
# Output:
#   - Modifies:
#     o amrInputTK frame to have amr settings
#---------------------------------------------------------
amrMenuSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF03 Fun05 TOC:
   #   - builds AMR settings frame
   #   o tof03 fun05 sec01:
   #    - setup AMR settinngs
   #   o tof03 fun05 sec02:
   #    - add AMR settinngs to gui
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun05 Sec01:
   #   - setup AMR settinngs
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

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
   
   assign(
      "amrDbLabelTK",
      tklabel(
         amrInputTK,
         text = def_amrTblStr
      ),
      envir = .GlobalEnv
   );
   
   amrDbButTK =
      tkbutton(
         amrInputTK,
         text = "AMR table        ",
         command = getAmrTbl
   ); # add the get AMR table button
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun05 Sec02:
   #   - add AMR settinngs to gui
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   # There is here so systems without tcltk2 can work
   if(TOOL_TIP_BL > 0){
      tk2tip(
         amrDbButTK,
         "Who 2023 catalog in tbAMR format"
      ); # amr table button tootip
   } # If: I can add an tooltip

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
} # amrMenuSetup_freezeTBGui

#---------------------------------------------------------
# TOF03 Fun06: lineageMenuSetup_freezeTBGui
#   - builds the lineage settings frame
# Input:
# Output:
#   - Modifies:
#     o input gui to have lineage settings
#---------------------------------------------------------
lineageMenuSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF03 Fun06 TOC:
   #   - builds the lineage settings frame
   #   o tof03 fun06 sec01:
   #     - miru lineage detection
   #   o tof03 fun06 sec02:
   #     - spoligotype lineage detection
   #   o tof03 fun06 sec03:
   #     - add lineage options to gui
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun06 Sec01:
   #   - miru lineage detection
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   miruTitle_LabTK =
      tklabel(
         miruVarTK,
         text = "MIRU lineage settings"
      );
   
   assign(
      "miruDb_LabTK",
      tklabel(
         miruVarTK,
         text = def_miruTblStr
      ), # Current MIRU lineage table used
      envir = .GlobalEnv
   );
   
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
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun06 Sec02:
   #   - spoligotype lineage detection
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   spolTitle_LabTK =
      tklabel(
         spolVarTK,
         text = "Spoligotyping settings"
      );
   
   assign(
      "spolSeq_LabTK",
      tklabel(
         spolVarTK,
         text = def_spolSpacersStr
      ),
      envir = .GlobalEnv
   );
   
   spolSeq_ButTK =
      tkbutton(
         spolVarTK,
         text = "Spoligo sequences",
         command = getSpoligoSeq
      );
   
   # There is here so systems without tcltk2 can work
   if(TOOL_TIP_BL > 0){
      tk2tip(
         spolSeq_ButTK,
         "Fasta file with spoltype space sequences (DR)"
      );
   } # If: I can add an tooltip
   
   assign(
      "spolDb_LabTK",
      tklabel(
         spolVarTK,
         text = def_spolDbStr
      ),
      envir = .GlobalEnv
   );
   
   spolDb_ButTK =
      tkbutton(
         spolVarTK,
         text = "Spoligo database",
         command = getSpoligoDB
      );
   
   # There is here so systems without tcltk2 can work
   if(TOOL_TIP_BL > 0){
      tk2tip(
         spolDb_ButTK,
         "csv file (data base) with spoltype lineages"
      );
   } # If: I can add an tooltip
   
   spolPerc_LabTK =
      tklabel(
         spolVarTK,
         text = "Min % score"
      );
   
   spolPerc_SlideTK =
      tkscale(
         spolVarTK,
         from = 0,
         to = 1,
         showvalue = TRUE,
         variable = MIN_PERC_SCORE_TCL,
         resolution = 0.01,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
     tk2tip(
      spolPerc_SlideTK,
      "Minimum percent score to count an spacer as mapped"
     );
   } # Add tooltip label
   
   spolStart_LabTK =
      tklabel(
         spolVarTK,
         text = "DR start"
      );
   
   spolStart_BoxTK =
      tkentry(
         spolVarTK,
         width = 12,
         textvariable = DR_START
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
        spolStart_BoxTK,
        "Start of direct repeat (DR) region in reference"
      );
   } # Add tooltip label
   
   spolEnd_LabTK =
      tklabel(
         spolVarTK,
         text = "DR end"
      );
   
   spolEnd_BoxTK =
      tkentry(
         spolVarTK,
         width = 12,
         textvariable = DR_END
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
        spolEnd_BoxTK,
        "End of direct repeat (DR) region in reference"
      );
   } # Add tooltip label

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun06 Sec03:
   #   - add lineage options to gui
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

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
   
   tkgrid(spolTitle_LabTK);
   tkgrid(spolSeq_ButTK, spolSeq_LabTK);
   tkgrid(spolDb_ButTK, spolDb_LabTK);
   tkgrid(spolPerc_LabTK, spolPerc_SlideTK);
   tkgrid(spolStart_LabTK, spolStart_BoxTK);
   tkgrid(spolEnd_LabTK, spolEnd_BoxTK);
   
   tkgrid.configure(
      spolTitle_LabTK,
      spolDb_ButTK,
      spolPerc_SlideTK,
      spolPerc_LabTK,
      spolStart_LabTK,
      spolEnd_LabTK,
      sticky = "w"
   );
   
   tkgrid.configure(
      spolDb_LabTK,
      spolStart_BoxTK,
      spolEnd_BoxTK,
      sticky = "w"
   );
   
   tkpack(lineageInputTK, miruVarTK, side = "left");
   tkpack(lineageInputTK, spolVarTK, side = "left");
} # lineageMenuSetup_freezeTBGui

#---------------------------------------------------------
# TOF03 Fun07: conMenuSetup_freezeTBGui
#   - builds the consensus settings frame
# Input:
# Output:
#   - Modifies:
#     o input gui to have consensus settings
#---------------------------------------------------------
conMenuSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF03 Fun07 TOC:
   #   - builds the consensus settings frame
   #   o tof03 fun07 sec01:
   #     - Consensus global (bulding) options
   #   o tof03 fun07 sec02:
   #     - Consensus collapsing (bulding) options
   #   o tof03 fun07 sec03:
   #     - Variant printing options for consensus
   #   o tof03 fun07 sec04:
   #     - add consensus menu to gui
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun07 Sec01:
   #   - Consensus global (bulding) options
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   conGlobTitle_LabTK =
      tklabel(
         conGlobalInputTK,
         text = "Consensus build/variant print settings"
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
         "Min Q-scrore to keep a base (consensus step)"
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
         "Min Q-scrore to keep a insertion (consensus)"
      ); # Minimum q-score button
   } # If: I can add an tooltip
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun07 Sec02:
   #   - Consensus collapsing (bulding) options
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
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
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun07 Sec03:
   #   - Variant printing options for consensus
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
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
         "Min read depth to print alternative variant/AMR"
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
        "Min percent support to print a match/snp variant"
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
        "Min percent support to print a insertion variant"
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
         "Min percent support to print a deletion variant"
      );
   } # If: I can add an tooltip

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun07 Sec04:
   #   - add consensus menu to gui
   #   o tof03 fun07 sec04 sub01:
   #     - set up the global consensus options frame
   #   o tof03 fun07 sec04 sub02:
   #     - set up the collapse consensus options frame
   #   o tof03 fun07 sec04 sub03:
   #     - set up print variants consensus options frame
   #   o tof03 fun07 sec04 sub04:
   #     - add consensus menu optoins to gui
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF03 Fun07 Sec04 Sub01
   #   - set up the global consensus options frame
   #******************************************************
   
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

   #******************************************************
   # TOF03 Fun07 Sec04 Sub02:
   #   - set up the collapse consensus options frame
   #******************************************************

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

   #******************************************************
   # TOF03 Fun07 Sec04 Sub03:
   #   - set up the print variants consensus options frame
   #******************************************************

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

   #******************************************************
   # TOF03 Fun07 Sec04 Sub04:
   #   - add consensus menu optoins to gui
   #******************************************************

   tkpack(conInputTK, conGlobalInputTK, side = "top");
   tkpack(conInputTK, conBuildInputTK, side = "left");
   tkpack(conInputTK, conPrintInputTK, side = "right");
} # conMenuSetup_freezeTBGui

#---------------------------------------------------------
# TOF03 Fun08: mixefInfectSetup_freezeTBGui
#   - builds mixed infections settings frame
# Input:
# Output:
#   - Modifies:
#     o input gui to have mixed infections settings
#---------------------------------------------------------
mixedInfectSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF03 Fun08 TOC:
   #   - builds mixed infections settings frame
   #   o tof03 fun08 sec0x:
   #     - add mixed infections settings to gui
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   clust_checkboxTK =
      tkcheckbutton(
         parent = mixedInfectTK,
         text = "mixed infection",
         variable = CLUSTER_BL_TCL
       );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         clust_checkboxTK,
         "attempt to detect mixed infections in sample"
      );
   } # If: I can make an tooltip



   profile_checkboxTK =
      tkcheckbutton(
         parent = mixedInfectTK,
         text = "depth profile",
         variable = DEPTH_PROFILE_BL_TCL
       );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         profile_checkboxTK,
         "only keep variants with min read depth"
      );
   } # If: I can make an tooltip
   

   lenWeight_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "Length weight"
      );
   
   lenWeight_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0,
         to = 10,
         showvalue = TRUE,
         variable = LENGTH_WEIGHT_CLUST_TCL,
         resolution = 0.1,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
       lenWeight_SlideTK,
       "score = median Q + (floor(log2(length)) * weight)"
      );
   } # If: I can add an tooltip


   minDepthClust_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "Min depth"
      );
   
   minDepthClust_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 1,
         to = 100,
         showvalue = TRUE,
         variable = MIN_DEPTH_CLUST_TCL,
         resolution = 1,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         minDepthClust_SlideTK,
         "minimum read depth to keep a cluster"
      );
   } # If: I can add an tooltip


   minPercDepthClust_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "Min % depth"
      );
   
   minPercDepthClust_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0,
         to = 1,
         showvalue = TRUE,
         variable = MIN_PERC_DEPTH_CLUST_TCL,
         resolution = 0.001,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         minPercDepthClust_SlideTK,
         "minimum percent read depth to keep a cluster"
      );
   } # If: I can add an tooltip


   readErrPerc_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "read error rate"
      );
   
   readErrPerc_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0,
         to = 0.25,
         showvalue = TRUE,
         variable = READ_ERROR_PERC_CLUST_TCL,
         resolution = 0.001,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         readErrPerc_SlideTK,
         paste(
           "percent error rate expected for read to read",
           "mapping"
         )
      );
   } # If: I can add an tooltip


   conErrPerc_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "consensus error rate"
      );
   
   conErrPerc_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0,
         to = 0.25,
         showvalue = TRUE,
         variable = CON_ERROR_PERC_CLUST_TCL,
         resolution = 0.001,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         conErrPerc_SlideTK,
         paste(
            "percent error rate expected for consensus",
            "to read mapping"
         )
      );
   } # If: I can add an tooltip


   maxSim_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "Max % similarity"
      );
   
   maxSim_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0.75,
         to = 1,
         showvalue = TRUE,
         variable = MAX_CON_SIMILARITAY_CLUST_TCL,
         resolution = 0.001,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         maxSim_SlideTK,
         paste(
           "maximum percent similarity between consensus",
           "before merging"
         )
      );
   } # If: I can add an tooltip


   errToVar_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "min variant:error"
      );
   
   errToVar_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0,
         to = 500,
         showvalue = TRUE,
         variable = ERROR_TO_VARIANT_RATIO_CLUST_TCL,
         resolution = 1,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         errToVar_SlideTK,
         "100 * (variants) / (length * error rate)"
      );
   }


   overlap_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "Min % overlap"
      );
   
   overlap_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0.5,
         to = 1,
         showvalue = TRUE,
         variable = MIN_PERC_OVERLAP_CLUST_TCL,
         resolution = 0.001,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         overlap_SlideTK,
         "minimum percent ovlerap to compare consensuses"
      );
   } # If: I can add an tooltip


   winLen_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "Window length"
      );
   
   winLen_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0,
         to = 10000,
         showvalue = TRUE,
         variable = WINDOW_LENGTH_CLUST_TCL,
         resolution = 10,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         winLen_SlideTK,
         "length of one window (for max window distance)"
      );
   } # If: I can add an tooltip


   winErr_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "Window variant:error"
      );
   
   winErr_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0,
         to = 500,
         showvalue = TRUE,
         variable = WINDOW_ERROR_TO_VAR_CLUST_TCL,
         resolution = 1,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         winErr_SlideTK,
         paste(
            "maximum variant to error ratio to in max",
            "to consider two reads/consensuses differnt"
         )
      );
   } # If: I can add an tooltip


   indelLen_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "min indel length"
      );
   
   indelLen_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 1,
         to = 30,
         showvalue = TRUE,
         variable = MIN_INDEL_LEN_CLUST_TCL,
         resolution = 1,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         indelLen_SlideTK,
         "minimum length to count indel in edit distance"
      );
   } # If: I can add an tooltip


   clustMinQ_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "min snp q-score"
      );
   
   clustMinQ_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0,
         to = 100,
         showvalue = TRUE,
         variable = CLUST_MIN_SNP_Q_TCL,
         resolution = 1,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         clustMinQ_SlideTK,
         "minimum q-score to count SNP in edit distance"
      );
   } # If: I can add an tooltip


   maxMask_LabTK =
      tklabel(
         parent = mixedInfectTK,
         text = "max % N's in consensus"
      );
   
   maxMask_SlideTK =
      tkscale(
         parent = mixedInfectTK,
         from = 0,
         to = 1,
         showvalue = TRUE,
         variable = MAX_MASK_IN_CON_CLUST_TCL,
         resolution = 0.001,
         orient = "horiz"
      );
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         maxMask_SlideTK,
         "maximum percent masking (N's) in consensus"
      );
   } # If: I can add an tooltip

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun08 Sec0x:
   #   - add mixed infections settings to gui
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tkgrid(clust_checkboxTK);
   tkgrid(profile_checkboxTK);
   tkgrid(lenWeight_LabTK, lenWeight_SlideTK);
   tkgrid(minDepthClust_LabTK, minDepthClust_SlideTK);

   tkgrid(
      minPercDepthClust_LabTK,
      minPercDepthClust_SlideTK
   );

   tkgrid(readErrPerc_LabTK, readErrPerc_SlideTK);
   tkgrid(conErrPerc_LabTK, conErrPerc_SlideTK);
   tkgrid(maxSim_LabTK, maxSim_SlideTK);
   tkgrid(errToVar_LabTK, errToVar_SlideTK);
   tkgrid(overlap_LabTK, overlap_SlideTK);
   tkgrid(winLen_LabTK, winLen_SlideTK);
   tkgrid(winErr_LabTK, winErr_SlideTK);
   tkgrid(indelLen_LabTK, indelLen_SlideTK);
   tkgrid(clustMinQ_LabTK, clustMinQ_SlideTK);
   tkgrid(maxMask_LabTK, maxMask_SlideTK);

   tkgrid.configure(
      clust_checkboxTK,
      profile_checkboxTK,
      lenWeight_LabTK,
      lenWeight_SlideTK,
      minDepthClust_LabTK,
      minDepthClust_SlideTK,
      minPercDepthClust_LabTK,
      minPercDepthClust_SlideTK,
      readErrPerc_LabTK,
      readErrPerc_SlideTK,
      conErrPerc_LabTK,
      conErrPerc_SlideTK,
      maxSim_LabTK,
      maxSim_SlideTK,
      errToVar_LabTK,
      errToVar_SlideTK,
      overlap_LabTK,
      overlap_SlideTK,
      winLen_LabTK,
      winLen_SlideTK,
      winErr_LabTK,
      winErr_SlideTK,
      indelLen_LabTK,
      indelLen_SlideTK,
      clustMinQ_LabTK,
      clustMinQ_SlideTK,
      maxMask_LabTK,
      maxMask_SlideTK,
      sticky = "w"
   ); # set up the slider labels locations

} # mixedInfectSetup_freezeTBGui

#---------------------------------------------------------
# TOF03 Fun09: graphMenuSetup_freezeTBGui
#   - builds the graph settings frame
# Input:
# Output:
#   - Modifies:
#     o input gui to have graph settings
#---------------------------------------------------------
graphMenuSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF03 Fun09 TOC:
   #   - builds the graph settings frame
   #   o tof03 fun09 sec01:
   #     - setup graphing options
   #   o tof03 fun09 sec02:
   #     - add graphing menu options to gui
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun09 Sec01:
   #   - setup graphing options
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   ext_LabTK =
      tklabel(
         parent = graphInputTK,
         text = "Select the file type to save graphs as"
      );

   
   svgExt_radioTK =
      tkradiobutton(
         parent = graphInputTK,
         text = "svg",
         variable = GRAPH_EXT_TCL,
         value = "svg" 
      );
   
   tiffExt_radioTK =
      tkradiobutton(
         parent = graphInputTK,
         text = "tiff",
         variable = GRAPH_EXT_TCL,
         value =  "tiff"
      );
   
   pngExt_radioTK =
      tkradiobutton(
         parent = graphInputTK,
         text = "png",
         variable = GRAPH_EXT_TCL,
         value =  "png"
      );
   
   pdfExt_radioTK =
      tkradiobutton(
         parent = graphInputTK,
         text = "pdf",
         variable = GRAPH_EXT_TCL,
         value =  "pdf"
      );
   
   jpegExt_radioTK =
      tkradiobutton(
         parent = graphInputTK,
         text = "jpeg",
         variable = GRAPH_EXT_TCL,
         value =  "jpeg"
      );

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun09 Sec02:
   #   - add graphing menu options to gui
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tkgrid(ext_LabTK);
   tkgrid(svgExt_radioTK);
   tkgrid(tiffExt_radioTK);
   tkgrid(pngExt_radioTK);
   tkgrid(pdfExt_radioTK);
   tkgrid(jpegExt_radioTK);
   
   tkgrid.configure(
      ext_LabTK,
      svgExt_radioTK,
      tiffExt_radioTK,
      pngExt_radioTK,
      pdfExt_radioTK,
      jpegExt_radioTK,
      sticky = "w"
   );
} # graphMenuSetup_freezeTBGui

#---------------------------------------------------------
# Fun10: launchInGui_freezeTBGui
#   - launches input gui
# Input:
# Output:
#   - runs freezeTB
#---------------------------------------------------------
launchInGui_freezeTBGui = function(){
   reqInputSetup_freezeTBGui();
   menuSetup_freezeTBGui();
   exitRunSetup_freezeTBGui();
   filtMenuSetup_freezeTBGui();
   amrMenuSetup_freezeTBGui();
   lineageMenuSetup_freezeTBGui();
   conMenuSetup_freezeTBGui();
   mixedInfectSetup_freezeTBGui();
   graphMenuSetup_freezeTBGui();

   # Not in correct place, but is just a place holder
   assign(
      "messg_labTK",
      tklabel(
         statusTK,
         text = "status"
      ),
      envir = .GlobalEnv
   );

   tkpack(statusTK, messg_labTK, side = "left");
   
   
   showReqInputMenu();

   # This provides a way to check if the user finished the
   #   input and avoids Rscript making and then rushing to
   #   finish the script before the user input anything.
   while(as.numeric(tkwinfo("exists", guiTK)))
      Sys.sleep(0.2);
} # launchInGui_freezeTBGui

#---------------------------------------------------------
# Main:
#   - drive for freezeTBGui
#---------------------------------------------------------

launchInGui_freezeTBGui();

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
