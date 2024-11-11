#!/usr/bin/env Rscript
# This locks onto Rscripts location

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o header:
#     - included libraries and general setup
#   o tof01:
#     - output gui
#   o tof02:
#     - input gui functions (buttons/widgets)
#   o tof03:
#     - input gui hide/show menu functions + run
#   o tof04:
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
glob_shortReadsBl_tcl = tclVar(0); # not doing short reads

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

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# TOF01:
#   - output gui
#   o tof01 fun01: getAmpDepthData_freezeTBGui
#     - reads in tsv from ampDepth for graphing
#   o tof01 fun02: mkDepthPlot_freezeTBGui
#     - makes the read depth graph for freezeTB
#   o tof01 fun03: mkCoveragePlot_freezeTBGui
#     - makes coverage graph for freezeTB
#   o tof01 fun04: outExit_freezeTBGui
#     - exit the output gui
#   o tof01 fun05: showReport_freezeTBGui
#     - displays front (default) gui
#   o tof01 fun06: showDepth_freezeTBGui
#     - displays mean gene read depth graph
#   o tof01 fun07: showCoverage_freezeTBGui
#     - displays amplicon coverage graph
#   o tof01 fun08: showAmr_freezeTBGui
#     - displays AMRs detected
#   o tof01 fun09: showSelect_freezeTBGui
#     - displays select screen
#   o tof01 fun10: mkAmrTbl_freezeTBGui
#     - builds consensus AMR table (as string) for GUI
#   o tof01 fun11: mkReadAmrTbl_freezeTBGui
#     - builds read AMR table (as string) for GUI
#   o tof01 fun12: mkReportGui_freezeTBGui
#     - makes primary display gui for freezeTB
#   o tof01 fun13: buildOutGUI_freezeTBGui
#     - builds output gui (adds all widgets)
#   o tof01 fun14: outGUI_freezeTBGui
#     - displays output gui for freezeTB
#   o tof01 fun15: getPrefix_freezeTBGui
#     - gets prefix from selected file
#   o tof01 fun17: getResults_freezeTBGui
#     - launches the output GUI for freezeTB
#   o tof01 fun18: buildSelect_freezeTBGui
#     - builds the output select GUI
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# TOF01 Header:
#   - global variables and TK frames
#   o tof01 header sec01:
#     - find AMR database location
#   o tof01 header sec02:
#     - global non-array variables
#   o tof01 header sec03:
#     - global array variables (drugs/color pallete)
#   o tof01 header sec04:
#     - TK frames
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# TOF01 Header Sec02:
#   - global non-array variables
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

glob_outAmrDbStr = def_amrTblStr;
glob_prefixStr = "";
glob_dirStr = "";

glob_minIndelPercF = tclVar(30);
glob_minPercSupF = tclVar(1);
glob_minDepthUI = tclVar(10);
glob_graphBl = 1;

glob_graphWidthUI = 700;
glob_graphHeightUI = 700;

glob_depthTsvStr = "-depths.tsv";
glob_depthPngStr = "-readDepth.png";
glob_coveragePngStr = "-coverage.png";
glob_conAmrTsvStr = "-con-amrs.tsv";
glob_readAmrTsvStr = "-read-amrs.tsv";
glob_spolTsvStr = "-read-spoligo.tsv";
glob_conSpolTsvStr = "-con-spoligo.tsv";
glob_miruTsvStr = "-con-miru.tsv";

glob_conAmrPathStr = ""; # path to consensus AMR frame
glob_readAmrPathStr = ""; # path to read AMR frame

glob_exitBl = FALSE;

glob_monoFontTK =
   tkfont.create(
      family = "courier",
      size = 9 # tk courier is massive (no idea why)
   ); # for tables

glob_monoBoldFontTK =
   tkfont.create(
      family = "courier",
      size = 9, # tk courier is massive (no idea why)
      weight = "bold"
   ); # for tables

glob_oldGraphBl = FALSE;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# TOF01 Header Sec03:
#   - global array variables (drugs/color pallete)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

glob_colPalAry =
   c(
      "#440154", # 1: viridus dark purple
      "#482173", # 2:
      "#433E85", # 3:
      "#38598C", # 4:
      "#2D708E", # 5: viridus blue
      "#25858E", # 6:
      "#1E9B8A", # 7:
      "#20A387", # 9:
      "#51C56A", # 10: virids light green
      "#85D54A", # 11:
      "#C2DF23", # 12: reserve for bars
      "#FDE725"  # 13: yellow (reserved for bars)
   ); # color pallete (12 colors from viridis)

glob_drugAry =
   c(
      "amikacin",
      "bedaquiline",
      "capreomycin",
      "clofazimine",
      "delamanid",
      "ethambutol",
      "ethionamide",
      "fluoroquine",
      "isoniazid",
      "kanamycin",
      "levofloxacin",
      "linezolid",
      "moxifloxacin",
      "penicillin-myceial-dreg",
      "pyrazinamide",
      "rifampicin",
      "streptomycin"
   ); # antibioticx

glob_drugAbvAry =
   c(
      "Amk", # amikacin
      "Bdq", # bedaquiline
      "Cap", # capreomycin
      "Cfz", # clorfazimine
      "Dlm", # delamanid
      "Emb", # ethambutol
      "Eto", # ethionamide
      "Flq", # fluoroquine
      "Inz", # isoniazid (also Iso, Inz, or Inh)
      "Kan", # kanamycin
      "Lfx", # levofloxacin
      "Lzd", # linezolid
      "Mfx", # moxifloxacin
      "Pmd", # penicillin-myceial-dreg
      "Pza", # pyrazinamide (Pyz, Pza, Pyr)
      "Rif", # rifampicin (Rif, Rmp, Rfm)
      "Str"  # strptomycin (Str, Stp, Stm)
   ); # abbrevations for antibioticx

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# TOF01 Header Sec04:
#   - TK frames
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

outMenuTK = tkframe(guiTK);
outDataTK = tkframe(guiTK);

outSelectTK = tkframe(outDataTK);
outAmrRepTK = tkframe(outDataTK);
outReportTK = tkframe(outDataTK);
outDepthTK = tkframe(outDataTK);
outCoverageTK = tkframe(outDataTK);

amrTblTK = tkframe(outDataTK);
conAmrTK = tkframe(amrTblTK, borderwidth = 2);
readAmrTK = tkframe(amrTblTK, borderwidth = 2);

outGuiDir_labTK = NULL;
outGuiPrefix_labTK = NULL;

depth_labTK = NULL;
coverage_labTK = NULL;

conAmr_labTK = NULL;
readAmr_labTK = NULL;

spolEntry_labTK = NULL;
spolOctal_labTK = NULL;
spolStrain_labTK = NULL;
spolSIT_labTK = NULL;
spolCountry_labTK = NULL;

conSpolEntry_labTK = NULL;
conSpolOctal_labTK = NULL;
conSpolStrain_labTK = NULL;
conSpolSIT_labTK = NULL;
conSpolCountry_labTK = NULL;

miru_labTK = NULL;

outGuiAmrDB_labTK = NULL;

#---------------------------------------------------------
# TOF01 Fun01: getAmpDepthData_freezeTBGui
#   - reads in tsv from ampDepth for graphing
# Input:
#   - ampDepthTsvStr:
#     o path to tsv made by ampDepth
# Output:
#   - data frame with ampDepth data prepared for graph
#     functions
#---------------------------------------------------------
getAmpDepthData_freezeTBGui =
function(
   prefixStr
){
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF01 Fun01 TOC:
   #   - reads in tsv from ampDepth for graphing
   #   o tof01 fun01 sec01:
   #     - read in data and remove unused rows
   #   o tof01 fun01 sec02:
   #     - group entries by gene mapped to
   #   o tof01 fun01 sec03:
   #     - turn flag into numeric value
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun01 Sec01:
   #   - read in data and remove unused rows
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   dataDF =
      read.csv(
         paste(
            prefixStr,
            get("glob_depthTsvStr", envir=.GlobalEnv),
            sep = ""
         ),
         sep="\t",
         header=TRUE
       );

   if( is.null(dataDF) )
      return(NULL);

   dataDF =
      dataDF[
         dataDF$geneId != "x-off-target" &
         dataDF$geneId != "z-unmapped"
         ,
      ]; # Remove items I do not want to graph

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun01 Sec02:
   #   - group entries by gene mapped to
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   dataDF =
      dataDF[
         order(
            dataDF$refGeneStart,
            dataDF$flag
         ),
      ];
   
   siGene = 1;
   numGenesSI = length(dataDF$geneId);
   dataDF$geneFlag = dataDF$geneId;
   dataDF$repUI = dataDF$ampNumber;
   numRepSI = 0;
   
   while(siGene <= numGenesSI - 1)
   { # Loop: find number replicates for each gene
      if(dataDF[siGene,]$flag != dataDF[siGene +1,]$flag){
         if(numRepSI == 0){
            dataDF[siGene,]$repUI = numRepSI;
         } else{
            numRepSI = numRepSI + 1;
            dataDF[siGene,]$repUI = numRepSI;
            numRepSI = 0; # if new set of genes
         }
      }else if(
         dataDF[siGene,]$geneId ==
            dataDF[siGene + 1,]$geneId
      ){ # Else If: same gene
         numRepSI = numRepSI + 1;
         dataDF[siGene,]$repUI = numRepSI;
      }else{
         numRepSI = 0;
         dataDF[siGene,]$repUI = numRepSI;
      } # check if same gene name
   
      siGene = siGene + 1;
   } # Loop: merge gene names

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun01 Sec03:
   #   - turn flag into numeric value
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   dataDF$indexUI = as.numeric(factor(dataDF$flag));

   return(dataDF);
} # getAmpDepthData_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun02: mkDepthPlot_freezeTBGui
#   - makes the read depth graph for freezeTB
# Input:
#   - ampDepthDF:
#     o data frame from getAmpDepthData_freezeTBGui
#   - prefixStr:
#     o what to name output saved graph
# Output:
#   - builds read depth graph for freezeTB
#   - builds and saves read depth graph to
#     prefixStr-readDepth.png
#   - Returns:
#     o TRUE if built graph
#     o FALSE if input data (ampDepthDF) was NULL
#---------------------------------------------------------
mkDepthPlot_freezeTBGui =
function(
   ampDepthDF,
   prefixStr = glob_prefixStr
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF01 Fun02 TOC:
   #   - makes the read depth graph for freezeTB
   #   o tof01 fun02 sec01:
   #     - plot first (un-filtered) read depths
   #   o tof01 fun02 sec02:
   #     - get and graph second flag read depths
   #   o tof01 fun02 sec03:
   #     - graph low depth and good depth lines
   #   o tof01 fun02 sec04:
   #     - add legend
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   if( is.null(ampDepthDF) )
      return(FALSE);

   numFlagsUI = length(unique(ampDepthDF$flag));

   widthInt = glob_graphWidthUI;
   heightInt = glob_graphHeightUI;

   png(
      filename =
         paste(
            prefixStr,
            get("glob_depthPngStr", envir = .GlobalEnv),
            sep = ""
         ),
      width = widthInt,
      height = heightInt
   ); # save graph as file

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun02 Sec01:
   #   - plot first (un-filtered) read depths
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   idAryStr = unique(ampDepthDF$geneId);

   depthAryUI =
      rep(
         0,
         length(unique(ampDepthDF$geneId))
      );
   
   ampGeneAryStr =
      ampDepthDF[ampDepthDF$indexUI == 1,]$geneId;

   ampDepthAryUI =
      ampDepthDF[ampDepthDF$indexUI == 1,]$avgDepth;
   
   siGene = 1;
   idUI = 1;
   lenAmpUI = length(ampGeneAryStr);
   
   while(siGene <= lenAmpUI)
   { # Loop: build graph vectors
      if(idAryStr[idUI] != ampGeneAryStr[siGene]){
         idUI = idUI + 1;
      }else{
         depthAryUI[idUI] =
            max(depthAryUI[idUI], ampDepthAryUI[siGene]);
   
         siGene = siGene + 1;
      } # check if is missing ampGene for set
   } # Loop: build graph vectors
   
   posDF =
      barplot(
         height = depthAryUI,
         names = idAryStr,
         col = glob_colPalAry[1],
         xlab = "",                 # x-axis title
         ylab = "mean read depth",  # y-axis title
         las = 2,                   # x-axis at 90 degrees
         cex.axis = 1,              # y-axis ticks
         cex.names = 1,             # x-axis names
         ylim = c(0, max(c(ampDepthDF$avgDepth, 100)))
      ); # setup x-axis
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun02 Sec02:
   #   - get and graph second flag read depths
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   if(numFlagsUI >= 1){ 
      idAryStr = unique(ampDepthDF$geneId);
      depthAryUI =
         rep(
            0,
            length(unique(ampDepthDF$geneId))
         );
      
      ampGeneAryStr =
         ampDepthDF[ampDepthDF$indexUI == 2,]$geneId;

      ampDepthAryUI =
         ampDepthDF[ampDepthDF$indexUI == 2,]$avgDepth;
      
      siGene = 1;
      idUI = 1;
      lenAmpUI = length(ampGeneAryStr);
      
      while(siGene <= lenAmpUI)
      { # Loop: build graph vectors
         if(idAryStr[idUI] != ampGeneAryStr[siGene]){
            idUI = idUI + 1;
         }else{
            depthAryUI[idUI] =
              max(depthAryUI[idUI],ampDepthAryUI[siGene]);
      
            siGene = siGene + 1;
         } # check if is missing ampGene for set
      } # Loop: build graph vectors
   
      barplot(
         height = depthAryUI,
         names = idAryStr,
         col = glob_colPalAry[10],
         las = 2,                   # x-axis at 90 degrees
         cex.axis = 1,              # y-axis ticks
         cex.names = 1,             # x-axis names
         add = TRUE                 # building up graph
      );
   } # If: comparing two values
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun02 Sec03:
   #   - graph low depth and good depth lines
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   segments(
      x0 = posDF[,1] - 0.5, # has bar center coordiantes
      x1 = posDF[,1] + 0.5,
      y0 = 10, # y-axis start
      y1 = 10, # y-axis end
      lwd = 3, # line width
      col = glob_colPalAry[12] # color (yellow)
   ); # 10x read depth line
   
   segments(
      x0 = posDF[,1] - 0.5,
      x1 = posDF[,1] + 0.5,
      y0 = 100, # y-axis start
      y1 = 100, # y-axis end
      lwd = 3, # line width
      col = glob_colPalAry[5]
   ); # 100x read depth line
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun02 Sec04:
   #  - add legend
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   if(numFlagsUI >= 1){ 
      legendAryStr =
         c(
            unique(ampDepthDF$flag)[1:2], 
            "10x read depth",
            "100x read depth"
         );
   
      legendAryCol =
         c(
            glob_colPalAry[1],
            glob_colPalAry[10],
            glob_colPalAry[12],
            glob_colPalAry[5]
         );
   }else{
      legendAryStr =
         c(
            unique(ampDepthDF$flag)[1], 
            "10x read depth",
            "100x read depth"
         );
   
      legendAryCol =
         c(
            glob_colPalAry[1],
            glob_colPalAry[12],
            glob_colPalAry[5]
         );
   } # figure out legend labels
   
   legend(
      "topleft",
      inset = c(0, -0.04), # gets legend outside of plot
      xpd = TRUE,      # allows legend outside of plot
      legend = legendAryStr, # names/labels for legend
      horiz = TRUE,    # horizontal legend
      ncol = 1,        # 1 column
      bty = 'n',       # no border
      lty = 1,         # add lines to legend
      lwd = 2,         # make lines thicker
      col = legendAryCol, # color for each line
      cex = 1,         # text size
      text.width = strwidth(legendAryStr) # spacing
   );

   dev.off();

   return(TRUE);
} # mkDepthPlot_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun03: mkCoveragePlot_freezeTBGui
#   - makes coverage graph for freezeTB
# Input:
#   - ampDepthDF:
#     o data frame from getAmpDepthData_freezeTBGui
#   - prefixStr:
#     o string with prefix to save output graph as
#   - amrFileStr:
#     o tsv file with AMR database
# Output:
#   - builds and saves coverage graph to
#     prefixStr-coverage.png
#   - Returns:
#     o TRUE if built graph
#     o FALSE if input data (ampDepthDF) was NULL
#---------------------------------------------------------
mkCoveragePlot_freezeTBGui =
function(
   ampDepthDF,
   prefixStr = glob_prefixStr,
   amrFileStr = NULL
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF01 Fun03 TOC:
   #   - makes coverage graph for freezeTB
   #   o tof01 fun03 sec01:
   #     - make file to save graph to
   #   o part02 sec02:
   #     - merge amplicons
   #   o part03 sec03:
   #     - get overlapping genes
   #   o part03 sec04:
   #     - assign AMRs to amplicons
   #   o part03 sec05:
   #     - build amplicon graph
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun03 Sec01:
   #   - make file to save graph to
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if( is.null(ampDepthDF) )
      return(FALSE);

   numFlagsUI = length(unique(ampDepthDF$flag));

   widthInt = glob_graphWidthUI;
   heightInt = glob_graphHeightUI;

   png(
      filename =
         paste(
            prefixStr,
            get("glob_coveragePngStr", envir=.GlobalEnv),
            sep = ""
         ),
      width = widthInt,
      height = heightInt
   ); # save graph as file

   plot.new(); # start new plot (so can add top margin)
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun03 Sec02:
   #   - merge amplicons
   #   o tof01 fun03 sec02 sub01:
   #     - set up variables for merging
   #   o tof01 fun03 sec02 sub02:
   #     - merge overlapping ranges
   #   o tof01 fun03 sec02 sub03:
   #     - clean up unmerged overlaps
   #   o tof01 fun03 sec02 sub04:
   #     - make amplicon data frame
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF01 Fun03 Sec02 Sub01:
   #   - set up variables for merging
   #******************************************************

   # get start and end of first gene in list
   ampDepthDF =
      ampDepthDF[
         with(
            ampDepthDF,
            order(
               refStart,
               refEnd
            )
         ),
      ]; # sort data by start and end
   
   refStartArySI = ampDepthDF$refStart;
   refEndArySI = ampDepthDF$refEnd;
   
   ampStartArySI = ampDepthDF$ampStart;
   ampEndArySI = ampDepthDF$ampEnd;
   geneAryStr = ampDepthDF$geneId;
   
   flagAryStr = ampDepthDF$flag
   
   siRef = 1;

   #******************************************************
   # TOF01 Fun03 Sec02 Sub02:
   #   - merge overlapping ranges
   #******************************************************
   
   while(siRef < length(refStartArySI) - 1)
   { # Loop: merge overlapping ranges
      if(
         refStartArySI[siRef + 1] <= refEndArySI[siRef]
      ){ # if: next amplicon merges into this one

         refStartArySI[siRef +1] = refStartArySI[siRef];
   
         if(refEndArySI[siRef] >= refEndArySI[siRef+1]){
            refEndArySI[siRef + 1] = refEndArySI[siRef];

         } else{
            refEndArySI[siRef] = refEndArySI[siRef + 1];
         } # find highest start

      }else if(
         refEndArySI[siRef + 1] <= refEndArySI[siRef]
      ){ # Else If: this merges into next amplicon

         refStartArySI[siRef +1] = refStartArySI[siRef];
         refEndArySI[siRef + 1] = refEndArySI[siRef];

      } # check if have overlap
   
      siRef = siRef + 1;
   } # Loop: merge overlapping ranges
   
   #******************************************************
   # TOF01 Fun03 Sec02 Sub03:
   #   - clean up unmerged overlaps
   #******************************************************

   while(siRef > 1)
   { # Loop: merge missed end coordinates

      if(refStartArySI[siRef] == refStartArySI[siRef-1])
      { # If: same start position

         # in some cases I may need to mege amplicons
         if(refEndArySI[siRef] > refEndArySI[siRef -1]){
            refEndArySI[siRef - 1] = refEndArySI[siRef];

         }else if(refEndArySI[siRef] <
                  refEndArySI[siRef - 1]
         ){refEndArySI[siRef] = refEndArySI[siRef - 1];}

      } # If: same start position
   
      siRef = siRef - 1;
   } # Loop: merge missed end coordinates
   
   #******************************************************
   # TOF01 Fun03 Sec02 Sub04:
   #   - make amplicon data frame
   #******************************************************

   ampsDF =
      data.frame(
         refStartArySI,
         refEndArySI,
         ampStartArySI,
         ampEndArySI,
         flagAryStr
      );
   
   names(ampsDF) =
      c(
         "start",
         "end",
         "ampStart",
         "ampEnd",
         "flag"
      );
   
   ampsDF$ref = "ref";
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun03 Sec03:
   #   - get overlapping genes
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   ampDepthDF =
      ampDepthDF[order(ampDepthDF$refGeneStart), ];

   geneAryStr = unique(ampDepthDF$geneId);
   ampStartArySI = unique(ampDepthDF$refGeneStart);
   ampEndArySI = unique(ampDepthDF$refGeneStart);
      # for storing starts

   geneEndArySI = unique(ampDepthDF$refGeneEndUI);
   
   lenAmrSI = length(geneAryStr);
   lenDataSI = length(refStartArySI);
   
   siAmp = 1;
   siRef = 1;
   
   while(siAmp <= lenAmrSI)
   { # Loop: assign AMRs to overlaps
      if(ampStartArySI[siAmp] >= refStartArySI[siRef] &&
         ampStartArySI[siAmp] <= refEndArySI[siRef]
      ){
         ampEndArySI[siAmp] = ampStartArySI[siAmp];
         ampStartArySI[siAmp] = refStartArySI[siRef];
         geneEndArySI[siAmp] = refEndArySI[siRef];
         siAmp = siAmp + 1;

      }else{
         if(ampStartArySI[siAmp] > refEndArySI[siRef]){
            siRef = siRef + 1;

         }else{
            siAmp = siAmp + 1;
         } # check which value am incurmenting
      } # check if AMR is in overalp
   } # Loop: assign AMRs to overlaps
   
   # rebuild AMR refframe
   geneDF =
      data.frame(
         ampStartArySI,
         geneEndArySI,
         ampEndArySI,
         geneAryStr
      );
   
   names(geneDF) =
      c(
         "start",
         "end",
         "ampStart",
         "gene"
      );
   
   geneDF$yCol = "label";
   geneDF$adjPos = geneDF$ampStart - geneDF$start;
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun03 Sec04:
   #   - find AMRs in overlaps
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(! is.null(amrFileStr)) {
      amrDF =
         read.csv(
            amrFileStr,
            header = TRUE,
            sep = "\t"
         );

      amrDF = amrDF[order(amrDF$refPosition) ,]; 
      amrPosArySI = amrDF$refPosition;
      amrStartArySI = amrDF$geneStart;
      
      lenAmrSI = length(amrPosArySI);
      lenDataSI = length(refStartArySI);
      
      siAmr = 1;
      siRef = 1;
      
      while(siAmr <= lenAmrSI)
      { # Loop: assign AMRs to overlaps
         if(siRef > lenDataSI){
            amrStartArySI[siAmr] = -1;
            amrPosArySI[siAmr] = -1;
            siAmr = siAmr + 1;
         }else if(
           amrStartArySI[siAmr] >= refStartArySI[siRef] &&
           amrStartArySI[siAmr] <= refEndArySI[siRef]
         ){
   
            amrStartArySI[siAmr] = refStartArySI[siRef];
            siAmr = siAmr + 1;
   
         }else{
            if(amrPosArySI[siAmr] > refEndArySI[siRef]){
               siRef = siRef + 1;

            }else{
               amrStartArySI[siAmr] = -1;
               amrPosArySI[siAmr] = -1;
               siAmr = siAmr + 1;
            } # check which value am incurmenting
         } # check if AMR is in overalp
      } # Loop: assign AMRs to overlaps
      
      # remove AMRs not in reads
      amrStartArySI = amrStartArySI[amrStartArySI >= 0];
      amrPosArySI = amrPosArySI[amrPosArySI >= 0];
   
      # rebuild AMR dataframe
      amrPosDF =
         data.frame(
            amrStartArySI,
            amrPosArySI
         );
      
      names(amrPosDF) =
         c(
            "geneStart",
            "position"
         );
      
      amrPosDF$legendFlag = "AMR-mutation";
      amrPosDF$flag = "ref";
   } # If: have AMRs to graph
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun03 Sec05:
   #   - build coverage graph
   #   o tof01 sec05 sub01:
   #     - set up rows and columns
   #   o tof01 sec05 sub02:
   #     - build the graphs in each facet
   #   o tof01 sec05 sub03:
   #     - add x-axis label and legend to graph
   #   o tof01 sec05 sub04:
   #     - close graph
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   #******************************************************
   # TOF01 Fun03 Sec05 Sub01:
   #   - set up rows and columns
   #******************************************************

   ampMergeDF =
      unique(
         data.frame(
            geneDF$start,
            geneDF$end
         )
      );
   
   rowUI = length(ampMergeDF[,1]);
   colUI = 5
   
   if(rowUI < colUI){
      colUI = 1;
   } else{
      rowUI = ceiling(rowUI / colUI);
   } # find number of columns/rows needed
   
   if(numFlagsUI >= 1){ 
      refPosUI = 3;
   }else{
      refPosUI = 2;
   } # check if have one or two reads
   
   par(
      mfrow =
         c(
            rowUI,
            colUI
         ), # grid size
      mai = c(
         0.01, # bottom
         0.01, # left
         0.1, # top
         0.1  # right
      ), # adjust spacing between plots
      mar = c(
         2.75, # bottom
         2.25, # left
         0, # top
         1  # right
      ), # adjust x and y axis lable spacing
      mgp =
         c(
            0, # set plot title margin to 0
            1, # axis (default)
            0 # axis (default)
         ), # adjust indivdual plot margins
      oma =
         c(
            1, # bottom text size
            1, # left text size
            0, # top text size
            0 # rigth text size
         ), # set individual graph margins (by text size)
      yaxp = c(1, 3, 1), # have only 3 y-axis ticks
      xpd = NA          # allow plotting anywere
   ); # set up for merging graphs
   
   #******************************************************
   # TOF01 Fun03 Sec05 Sub02:
   #   - build the graphs in each facet
   #   o tof01 sec05 sub02 cat01:
   #     - plot main graph + start loop/setup
   #   o tof01 sec05 sub02 cat02:
   #     - add AMRs to graph
   #   o tof01 sec05 sub02 cat03:
   #     - add gene starts to graph
   #   o tof01 sec05 sub02 cat04:
   #     - add gene names to graph
   #******************************************************
   
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # TOF01 Fun03 Sec05 Sub02 Cat01:
   #   - plot main graph + start loop/setup
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   
   refPosUI = 3;
   flagAryStr = unique(ampDepthDF$flag);
   
   for( uiGraph in 1:length(ampMergeDF[,1]) ){
   
      startUI = unique(ampMergeDF[uiGraph, 1]);
   
      plot(
         x =
            c(
               min(
                  ampMergeDF[uiGraph, 1],
   
                  amrPosDF[
                    amrPosDF$geneStart == startUI
                    ,
                  ]$position, # get AMR x coordinates
   
                  ampsDF[
                    ampsDF$ampstart == startUI
                    ,
                  ]$ampStart  # get amplicon coordinates
               ), # see if amr or amplicon comes first
   
               max(
                  ampMergeDF[uiGraph, 2],
   
                  amrPosDF[
                    amrPosDF$geneStart == startUI
                    ,
                  ]$position,  # get AMR x coordinates
   
                  ampsDF[
                    ampsDF$start == startUI
                    ,
                  ]$ampEnd  # get amplicon coordinates
               ) # see if amr or amplicon has largest end
             ), # x coordinates
         y = c(refPosUI, refPosUI),
         type = 'l',
         ylim = c(0.5, refPosUI + 0.6), # y-axis limits
         xlab = "",
         ylab = NA,
         lwd = 2,
         yaxt = 'n',        # no y-axis labels
         col = glob_colPalAry[1]
      ); # build initial plot
   
      #+++++++++++++++++++++++++++++++++++++++++++++++++++
      # TOF01 Fun03 Sec05 Sub02 Cat02:
      #   - add AMRs to graph
      #+++++++++++++++++++++++++++++++++++++++++++++++++++
   
      points(
         x =
           amrPosDF[
              amrPosDF$geneStart == startUI
              ,
           ]$position, # get AMR x coordinates
         y =
            rep(
               refPosUI,
               length(
                  amrPosDF[
                    amrPosDF$geneStart == startUI
                    ,
                  ]$position
               )
            ),
        pch = 16, # filled circle
        col = glob_colPalAry[12]
      ); # add AMRs to graph
   
      #+++++++++++++++++++++++++++++++++++++++++++++++++++
      # TOF01 Fun03 Sec05 Sub03 Cat03:
      #   - add gene starts to graph
      #+++++++++++++++++++++++++++++++++++++++++++++++++++
   
      coordsAry =
         c(
           unique(
             ampsDF[
                ampsDF$start == startUI &
                ampsDF$flag == flagAryStr[1]
                ,
             ],
           )$ampStart,
           unique(
             ampsDF[
                ampsDF$start == startUI &
                ampsDF$flag == flagAryStr[1]
                ,
             ]
           )$ampEnd
          ); # find x coordainates
   
      lines(
         x = coordsAry,
         y = rep( refPosUI - 1, length(coordsAry) ),
         lwd = 2,
         col = glob_colPalAry[5]
      );
   
      lenUI =
        length(geneDF[geneDF$start == startUI,]$ampStart);
   
      segments(
         x0 = geneDF[geneDF$start == startUI,]$ampStart,
         x1 = geneDF[geneDF$start == startUI,]$ampStart,
         y0 =
            rep(
               0.75,
               lenUI
            ),
         y1 =
            rep(
               refPosUI,
               lenUI
            ),
         lty = 2,
         lwd = 2,
         col = glob_colPalAry[5]
      );
   
      #+++++++++++++++++++++++++++++++++++++++++++++++++++
      # TOF01 Fun03 Sec05 Sub02 Cat04:
      #   - add gene names to graph
      #+++++++++++++++++++++++++++++++++++++++++++++++++++
   
      textAryStr =
         unique(geneDF[geneDF$start == startUI,]$gene);
   
      startArySI =
        unique(geneDF[geneDF$start == startUI,]$ampStart);
   
      lenUI = length(textAryStr);
   
      if(lenUI > 1){
         yCoordArySI =
            rep(
               c(
                  refPosUI + 0.2,
                  refPosUI + 0.45
               ),
               min(lenUI, 1)
            ); # offseting so no overlap issues
      }else{
         yCoordArySI = refPosUI + 0.2;
      } # get y-coordinates with offset
   
      text(
         x = startArySI + 0.45,
         y = yCoordArySI,
         labels = textAryStr,
         adj = c(0, 0.5)
      );
   
      if(refPosUI > 2){
         coordsAry =
            c(
              unique(
                ampsDF[
                   ampsDF$start == startUI &
                   ampsDF$flag == flagAryStr[2]
                   ,
                ],
              )$ampStart,
              unique(
                ampsDF[
                   ampsDF$start == startUI &
                   ampsDF$flag == flagAryStr[2]
                   ,
                ]
              )$ampEnd
             ); # find x coordainates
   
         lines(
            x = coordsAry,
            y = rep( refPosUI - 2, length(coordsAry) ),
            lwd = 2,
            col = glob_colPalAry[5]
         );
      } # If: have two flags
   
      if(refPosUI > 2){
        axis(
           2, # y axis
           at = c(1, 2, 3),
           las = 1,                 # rotate 90 degrees
           labels = c(flagAryStr[2], flagAryStr[1], "ref")
        );

      }else{
        axis(
           2, # place on left side of graph
           at = c(1, 2),
           labels = c(flagAryStr[1], "ref"),
           las = 1,                 # rotate 90 degrees
           tick = TRUE
        );
      } # set axis labels
   }
   
   #******************************************************
   # TOF01 Fun03 Sec05 Sub03:
   #   - add x-axis label and legend to graph
   #******************************************************
   
   grid(
      nx = 1, # add x-coordinate row
      ny = 1  # add y-coordinate row
   ); # add extra row to bottom
   
   mtext(
      text = "Reference Position",
      side = 1,  # 1 = bottom, 2 = left, 3 = top 4 = right
      line = -0.25, # margin line to draw on
      outer = TRUE, # use outer margins (multi plot edges)
      cex = 1    # text size
   ); # add title to amplicon coverage graphs
   
   mtext(
      text = "- AMRs",
      side = 1,  # 1 = bottom, 2 = left, 3 = top 4 = right
      adj = 0.82,
      line = -0.25, # margin line to draw on
      outer = TRUE, # use outer margins
      col = glob_colPalAry[12],
      bg = glob_colPalAry[1],   # does not work
      cex = 1    # text size
   ); # add AMR entry for legend
   
   mtext(
      text = "-- Gene Start",
      side = 1,  # 1 = bottom, 2 = left, 3 = top 4 = right
      adj = 0.99,
      line = -0.25, # margin line to draw on
      outer = TRUE, # use outer margins
      col = glob_colPalAry[5],
      cex = 1    # text size
   ); # add AMR entry for legend
   
   #******************************************************
   # TOF01 Fun03 Sec05 Sub05:
   #   - close graph
   #******************************************************
   
   dev.off() # since last graph, let R close graphing

   return(TRUE);
} # mkCoveragePlot_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun04: outExit_freezeTBGui
#   - exit the output gui
# Input:
# Output:
#   - destroys the output gui
#---------------------------------------------------------
outExit_freezeTBGui =
function(
){
   tkdestroy(guiTK);
} # outExit_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun05: showReport_freezeTBGui
#   - displays front (default) gui
# Input:
# Output:
#   - hides current option and reveals main screen
#---------------------------------------------------------
showReport_freezeTBGui =
function(
){
   tkpack.forget(outAmrRepTK);
   tkpack.forget(outSelectTK);
   tkpack.forget(outReportTK);
   tkpack.forget(outDepthTK);
   tkpack.forget(outCoverageTK);
   tkpack.forget(amrTblTK);
   tkpack.forget(outDataTK);
   tkpack.forget(outMenuTK);

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

   tkwm.title(
      guiTK,
      paste(
         "Report: ",
         glob_prefixStr,
         glob_dirStr
      )
   );

   tkpack(outMenuTK, side = "bottom");
   tkpack(outAmrRepTK, side = "top");
   tkpack(outDataTK, outReportTK, side = "top");
} # showReport_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun06: showDepth_freezeTBGui
#   - displays mean gene read depth graph
# Input:
# Output:
#   - hides current option and reveals mean read depth
#---------------------------------------------------------
showDepth_freezeTBGui =
function(
){
   tkpack.forget(outAmrRepTK);
   tkpack.forget(outSelectTK);
   tkpack.forget(outReportTK);
   tkpack.forget(outDepthTK);
   tkpack.forget(outCoverageTK);
   tkpack.forget(amrTblTK);
   tkpack.forget(outDataTK);
   tkpack.forget(outMenuTK);

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

   tkwm.title(
      guiTK,
      paste(
         "read depth",
         glob_prefixStr,
         glob_dirStr
      )
   );

   tkpack(outMenuTK, side = "bottom");
   tkpack(outAmrRepTK, side = "top");
   tkpack(outDataTK, outDepthTK, side = "top");
} # showDepth_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun07: showCoverage_freezeTBGui
#   - displays amplicon coverage graph
# Input:
# Output:
#   - hides current option and reveals coverage graph
#---------------------------------------------------------
showCoverage_freezeTBGui =
function(
){
   tkpack.forget(outAmrRepTK);
   tkpack.forget(outSelectTK);
   tkpack.forget(outReportTK);
   tkpack.forget(outDepthTK);
   tkpack.forget(outCoverageTK);
   tkpack.forget(amrTblTK);
   tkpack.forget(outDataTK);
   tkpack.forget(outMenuTK);

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

   tkwm.title(
      guiTK,
      paste(
         "genome coverage",
         glob_prefixStr,
         glob_dirStr
      )
   );

   tkpack(outDataTK, outMenuTK, side = "bottom");
   tkpack(outAmrRepTK, side = "top");
   tkpack(outDataTK, outCoverageTK, side = "top");
} # showCoverage_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun08: showAmr_freezeTBGui
#   - displays AMRs detected
# Input:
# Output:
#   - hides current option and reveals AMRs table
#---------------------------------------------------------
showAmr_freezeTBGui =
function(
){
   tkpack.forget(outAmrRepTK);
   tkpack.forget(outSelectTK);
   tkpack.forget(outReportTK);
   tkpack.forget(outDepthTK);
   tkpack.forget(outCoverageTK);
   tkpack.forget(amrTblTK);
   tkpack.forget(outDataTK);
   tkpack.forget(outMenuTK);

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

   tkwm.title(
      guiTK,
      paste(
         "AMRs (consensus/reads)",
         glob_prefixStr,
         glob_dirStr
      )
   );

   tkpack(outDataTK, outMenuTK, side = "bottom");
   tkpack(outAmrRepTK, side = "top");
   tkpack(outDataTK, amrTblTK, side = "top");
} # showAmr_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun09: showSelect_freezeTBGui
#   - displays select screen
# Input:
# Output:
#   - hides current option and reveals select screen
#---------------------------------------------------------
showSelect_freezeTBGui =
function(
){
   tkpack.forget(outAmrRepTK);
   tkpack.forget(outSelectTK);
   tkpack.forget(outReportTK);
   tkpack.forget(outDepthTK);
   tkpack.forget(outCoverageTK);
   tkpack.forget(amrTblTK);
   tkpack.forget(outDataTK);
   tkpack.forget(outMenuTK);

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

   tkwm.title(
      guiTK,
      paste(
         "select output",
         glob_prefixStr,
         glob_dirStr
      )
   );

   tkpack(outMenuTK, side = "bottom");
   tkpack(outDataTK, outSelectTK, side = "top");
} # showSelect_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun10: mkAmrTbl_freezeTBGui
#   - builds consensus AMR table (as string) for GUI
# Input:
#   - prefixStr:
#     o string with prefix to consensus AMR tsv
# Output:
#   - Returns:
#     o string with consensus AMR table
#     o null if no consensus tsv file found
#---------------------------------------------------------
mkConAmrTbl_freezeTBGui =
function(
   prefixStr
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF01 Fun10 TOC:
   #   - builds consensus AMR table (as string) for GUI
   #   o tof01 fun10 sec01:
   #     - get consensus AMRs
   #   o tof01 fun10 sec02:
   #     - build header
   #   o tof01 fun10 sec03:
   #     - add padding so equal distance
   #   o tof01 fun10 sec04:
   #     - build up table
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun10 Sec01:
   #   - get consensus AMRs
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   dataDF =
      read.csv(
         paste(
            prefixStr,
            get(
               "glob_conAmrTsvStr",
               envir = .GlobalEnv
            ),
            sep = ""
         ),
         header = TRUE,
         sep = "\t"
      );

   if( is.null(dataDF) )
      return(NULL);

   dataDF = dataDF[order(dataDF$drug, dataDF$type),];

   for( lenUI in 1:length(glob_drugAry) )
   { # Loop: rename antibiotic to abbreiviation
      dataDF$drug =
         gsub(
            glob_drugAry[lenUI],
            glob_drugAbvAry[lenUI],
            dataDF$drug
      );

      dataDF$cross_resistance =
         gsub(
            glob_drugAry[lenUI],
            glob_drugAbvAry[lenUI],
            dataDF$cross_resistance
      );
   } # Loop: rename antibiotic to abbreiviation

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun10 Sec02:
   #   - build header
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   numRowUI = length(dataDF$drug);

   dataDF = rbind(dataDF, dataDF[1,])
   dataDF[numRowUI + 1,] =
      gsub(".*", "NA", dataDF[numRowUI + 1,]);

   dataDF$drug[numRowUI + 1] = "drug";
   dataDF$variant_id[numRowUI + 1] = "varaint_id";
   dataDF$type[numRowUI + 1] = "mutation";

   dataDF$cross_resistance =
      gsub("_", ", ", dataDF$cross_resistance);

   dataDF$cross_resistance[numRowUI + 1] =
      "cross_resistance";

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun10 Sec03:
   #   - add padding so equal distance
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   maxLenUI = 0;

   lenUI = max( nchar(dataDF$drug), na.rm = TRUE );
   maxLenUI = maxLenUI + lenUI + 2;
   dataDF$drug = sprintf(" %-*s ", lenUI, dataDF$drug);
      # nchar gets lengths and max gets longest string
      # sprintf adds padding to end

   lenUI =
      max( nchar(dataDF$cross_resistance), na.rm = TRUE );
   maxLenUI = maxLenUI + lenUI + 2;
   dataDF$cross_resistance =
      sprintf(" %-*s ", lenUI, dataDF$cross_resistance);

   lenUI = max(nchar(dataDF$variant_id), na.rm = TRUE);
   maxLenUI = maxLenUI + lenUI + 2;
   dataDF$variant_id =
      sprintf(" %-*s ", lenUI, dataDF$variant_id);

   lenUI = max(nchar(dataDF$type), na.rm = TRUE);
   maxLenUI = maxLenUI + lenUI + 2;
   dataDF$type =
      sprintf(" %-*s ", lenUI, dataDF$type);

   # build the dividing line
   sepStr = paste(rep("-", maxLenUI), collapse = "");

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun10 Sec04:
   #   - build up table
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   labStr =
         paste(
            dataDF$drug[numRowUI + 1],
            dataDF$cross_resistance[numRowUI + 1],
            dataDF$variant_id[numRowUI + 1],
            dataDF$type[numRowUI + 1],
            "\n",
            sepStr,
            sep = ""
         ); # make header

   for( lenUI in 1:numRowUI )
   { # Loop: print out all rows
      labStr =
         paste(
            labStr,
            "\n",
            dataDF$drug[lenUI],
            dataDF$cross_resistance[lenUI],
            dataDF$variant_id[lenUI],
            dataDF$type[lenUI],
            sep = ""
         ); # make table
   } # Loop: print out all rows

   return(labStr);
} # mkConAmrTbl_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun11: mkReadAmrTbl_freezeTBGui
#   - builds read AMR table (as string) for GUI
# Input:
#   - prefixStr:
#     o string with prefix to read AMR tsv
# Output:
#   - Returns:
#     o string with consensus AMR table
#     o null if no consensus tsv file found
#---------------------------------------------------------
mkReadAmrTbl_freezeTBGui =
function(
   prefixStr
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF01 Fun11 TOC:
   #   - builds consensus AMR table (as string) for GUI
   #   o tof01 fun11 sec01:
   #     - get read AMRs
   #   o tof01 fun11 sec02:
   #     - build header
   #   o tof01 fun11 sec03:
   #     - add padding so equal distance
   #   o tof01 fun11 sec04:
   #     - build up table
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun11 Sec01:
   #   - get read AMRs
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   dataDF =
      read.csv(
         paste(
            prefixStr,
            get(
               "glob_readAmrTsvStr",
               envir = .GlobalEnv
            ),
            sep = ""
         ),
         header = TRUE,
         sep = "\t"
      );

   if( is.null(dataDF) )
      return(NULL);

   dataDF =
      dataDF[
         order(
            dataDF$drug,
            dataDF$type,
            dataDF$num_supporting_reads
         ),
      ];

   for( lenUI in 1:length(glob_drugAry) )
   { # Loop: rename antibiotic to abbreiviation
      dataDF$drug =
         gsub(
            glob_drugAry[lenUI],
            glob_drugAbvAry[lenUI],
            dataDF$drug
      );

      dataDF$cross_resistance =
         gsub(
            glob_drugAry[lenUI],
            glob_drugAbvAry[lenUI],
            dataDF$cross_resistance
      );
   } # Loop: rename antibiotic to abbreiviation

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun11 Sec02:
   #   - build header
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   numRowUI = length(dataDF$drug);

   dataDF = rbind(dataDF, dataDF[1,])
   dataDF[numRowUI + 1,] =
      gsub(".*", "NA", dataDF[numRowUI + 1,]);

   dataDF$drug[numRowUI + 1] = "drug";
   dataDF$variant_id[numRowUI + 1] = "varaint_id";
   dataDF$type[numRowUI + 1] = "mutation";

   dataDF$num_supporting_reads =
      as.character(dataDF$num_supporting_reads);
   dataDF$num_supporting_reads[numRowUI +1] = "num_reads";

   dataDF$num_map_reads =
      as.character(dataDF$num_map_reads);
   dataDF$num_map_reads[numRowUI +1] = "total_reads";

   dataDF$crossResistance =
      gsub("_", ", ", dataDF$cross_resistance);

   dataDF$cross_resistance[numRowUI + 1] =
      "cross_resistance";

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun11 Sec03:
   #   - add padding so equal distance
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   maxLenUI = 0;

   lenUI = max( nchar(dataDF$drug), na.rm = TRUE );
   maxLenUI = maxLenUI + lenUI + 2;
   dataDF$drug = sprintf(" %-*s ", lenUI, dataDF$drug);
      # nchar gets lengths and max gets longest string
      # sprintf adds padding to end

   lenUI =
      max( nchar(dataDF$cross_resistance), na.rm = TRUE );
   maxLenUI = maxLenUI + lenUI + 2;
   dataDF$cross_resistance =
      sprintf(" %-*s ", lenUI, dataDF$cross_resistance);

   lenUI = max(nchar(dataDF$variant_id), na.rm = TRUE);
   dataDF$variant_id =
      sprintf(" %-*s ", lenUI, dataDF$variant_id);
   maxLenUI = maxLenUI + lenUI + 2;

   lenUI =
      max(nchar(dataDF$num_supporting_reads), na.rm=TRUE);
   maxLenUI = maxLenUI + lenUI + 2;
   dataDF$num_supporting_reads =
      sprintf(" %-*s ",lenUI,dataDF$num_supporting_reads);

   lenUI = max(nchar(dataDF$num_map_reads), na.rm = TRUE);
   maxLenUI = maxLenUI + lenUI + 2;
   dataDF$num_map_reads =
      sprintf(" %-*s ", lenUI, dataDF$num_map_reads);

   lenUI = max(nchar(dataDF$type), na.rm = TRUE);
   maxLenUI = maxLenUI + lenUI + 2;
   dataDF$type =
      sprintf(" %-*s ", lenUI, dataDF$type);

   # build the dividing line
   sepStr = paste(rep("-", maxLenUI), collapse = "");

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun11 Sec04:
   #   - build up table
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   labStr =
         paste(
            dataDF$drug[numRowUI + 1],
            dataDF$cross_resistance[numRowUI + 1],
            dataDF$variant_id[numRowUI + 1],
            dataDF$num_supporting_reads[numRowUI + 1],
            dataDF$num_map_reads[numRowUI + 1],
            dataDF$type[numRowUI + 1],
            "\n",
            sepStr,
            sep = ""
         ); # make header

   for( lenUI in 1:numRowUI )
   { # Loop: print out all rows
      labStr =
         paste(
            labStr,
            "\n",
            dataDF$drug[lenUI],
            dataDF$cross_resistance[lenUI],
            dataDF$variant_id[lenUI],
            dataDF$num_supporting_reads[lenUI],
            dataDF$num_map_reads[lenUI],
            dataDF$type[lenUI],
            sep = ""
         ); # make table
   } # Loop: print out all rows

   return(labStr);
} # mkConAmrLabel_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun12: mkReportGui_freezeTBGui
#   - makes primary display gui for freezeTB
# Input:
#   - prefixStr:
#     o prefix assigned to file names
#   - minIndelPercF:
#     o minimum percentage (0-100) of reads to keep indel
#   - minPercSupF:
#     o minimum percentage (0-100) of reads to keep AMR
#   - minDepthUI:
#     o minimum read depth to keep AMR
# Output:
#   - Modifies:
#     o outGuiTK to have the first GUI set up
#---------------------------------------------------------
mkReportGui_freezeTBGui =
function(
   prefixStr,
   minIndelPercF = 30,
   minPercSupF = 1,
   minDepthUI = 10
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF01 Fun12 TOC:
   #   - makes primary display gui for freezeTB
   #   o tof01 fun12 sec01:
   #     - build consensus AMR table
   #   o tof01 fun12 sec02:
   #     - build read AMR table
   #   o tof01 fun12 sec03:
   #     - build read spoligotype table
   #   o tof01 fun12 sec04:
   #     - build consensus spoligotype table
   #   o tof01 fun12 sec05:
   #     - build MIRU VNTR table
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun12 Sec01:
   #   - build consensus AMR table
   #   o tof01 fun12 sec01 sub01:
   #     - read in data
   #   o tof01 fun12 sec01 sub02:
   #     - convert antibiotics to three letter ids
   #   o tof01 fun12 sec01 sub03:
   #     - set up color scheme for resitant antibiotics
   #   o tof01 fun12 sec01 sub04:
   #     - add consensus antibiotic table
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF01 Fun12 Sec01 Sub01:
   #   - read in data and filter out low quality AMRs
   #******************************************************

   dataDF =
      read.csv(
         paste(
            prefixStr,
            get(
               "glob_conAmrTsvStr",
               envir = .GlobalEnv
            ),
            sep = ""
         ),
         header = TRUE,
         sep = "\t"
      );

   dataDF =
      dataDF[
         order(
            dataDF$drug,
            dataDF$type
         ),
      ];

   #******************************************************
   # TOF01 Fun12 Sec01 Sub02:
   #   - convert antibiotics to three letter ids
   #******************************************************

   for( lenUI in 1:length(glob_drugAry) )
   { # Loop: rename antibiotic to abbreiviation
      dataDF$drug =
         gsub(
            glob_drugAry[lenUI],
            glob_drugAbvAry[lenUI],
            dataDF$drug
      );

      dataDF$cross_resistance =
         gsub(
            glob_drugAry[lenUI],
            glob_drugAbvAry[lenUI],
            dataDF$cross_resistance
      );
   } # Loop: rename antibiotic to abbreiviation

   #******************************************************
   # TOF01 Fun12 Sec01 Sub02:
   #   - set up color scheme for resitant antibiotics
   #******************************************************

   amrAryStr =
      intersect(
         glob_drugAbvAry,
         c(
            dataDF$drug,
            dataDF$cross_resistance
         )
      );

   colAry =
      rep(
         glob_colPalAry[1],
         length(glob_drugAbvAry)
      );

   for( strAmr in amrAryStr)
   { # Loop: assigned colors to AMRs
      if( ! is.null(strAmr) )
         colAry[which(glob_drugAbvAry == strAmr)] =
            glob_colPalAry[12];
   } # Loop: assigned colors to AMRs

   #******************************************************
   # TOF01 Fun12 Sec01 Sub03:
   #   - add consensus antibiotic table
   #******************************************************

   for( siDrug in 1:length(glob_drugAbvAry) )
   { # Loop: setup AMR label colors
      tcl(
         paste(
            glob_conAmrPathStr,
            ".con",
            glob_drugAbvAry[siDrug],
            "LabTK",
            sep = ""
         ),
         "configure",
         bg = colAry[siDrug]
      );
   } # Loop: setup AMR label colors

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun12 Sec02:
   #   - build read AMR table
   #   o tof01 fun12 sec02 sub01:
   #     - read in data and filter out low quality AMRs
   #   o tof01 fun12 sec02 sub02:
   #     - convert antibiotics to three letter ids
   #   o tof01 fun12 sec02 sub03:
   #     - set up color scheme for resitant antibiotics
   #   o tof01 fun12 sec02 sub04:
   #     - add read antibiotic table
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF01 Fun12 Sec02 Sub01:
   #   - read in data and filter out low quality AMRs
   #******************************************************

   dataDF =
      read.csv(
         paste(
            prefixStr,
            get(
               "glob_readAmrTsvStr",
               envir = .GlobalEnv
            ),
            sep = ""
         ),
         header = TRUE,
         sep = "\t"
      );

   dataDF =
      dataDF[
         (
            (
               dataDF$type == "del" |
               dataDF$type == "ins"
            ) & dataDF$perc_support_reads >= minIndelPercF
         ) |
         ( dataDF$type != "del" & dataDF$type != "ins")
         ,
       ];

   dataDF =
      dataDF[
         dataDF$num_supporting_reads >= minDepthUI &
         dataDF$perc_support_reads >= minPercSupF 
         ,
      ];

   dataDF =
      dataDF[
         order(
            dataDF$drug,
            dataDF$type,
            dataDF$num_supporting_reads
         ),
      ];

   #******************************************************
   # TOF01 Fun12 Sec02 Sub02:
   #   - convert antibiotics to three letter ids
   #******************************************************

   for( lenUI in 1:length(glob_drugAry) )
   { # Loop: rename antibiotic to abbreiviation
      dataDF$drug =
         gsub(
            glob_drugAry[lenUI],
            glob_drugAbvAry[lenUI],
            dataDF$drug
      );

      dataDF$cross_resistance =
         gsub(
            glob_drugAry[lenUI],
            glob_drugAbvAry[lenUI],
            dataDF$cross_resistance
      );
   } # Loop: rename antibiotic to abbreiviation

   #******************************************************
   # TOF01 Fun12 Sec02 Sub02:
   #   - set up color scheme for resitant antibiotics
   #******************************************************

   amrAryStr =
      intersect(
         glob_drugAbvAry,
         c(
            dataDF$drug,
            dataDF$cross_resistance
         )
      );

   colAry =
      rep(
         glob_colPalAry[1],
         length(glob_drugAbvAry)
      );

   for( strAmr in amrAryStr)
   { # Loop: assigned colors to AMRs
      if( ! is.null(strAmr) )
         colAry[which(glob_drugAbvAry == strAmr)] =
            glob_colPalAry[12];
   } # Loop: assigned colors to AMRs

   #******************************************************
   # TOF01 Fun12 Sec02 Sub03:
   #   - add read antibiotic table
   #******************************************************

   for( siDrug in 1:length(glob_drugAbvAry) )
   { # Loop: setup AMR label colors
      tcl(
         paste(
            glob_readAmrPathStr,
            ".read",
            glob_drugAbvAry[siDrug],
            "LabTK",
            sep = ""
         ),
         "configure",
         bg = colAry[siDrug]
      );
   } # Loop: setup AMR label colors

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun12 Sec03:
   #   - build read spoligotype table
   #   o tof01 fun12 sec03 sub01:
   #     - read in spoligotypes
   #   o tof01 fun12 sec03 sub02:
   #     - add spoligotype data to report
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF01 Fun12 Sec03 Sub01:
   #   - read in spoligotypes
   #******************************************************

   dataDF =
      read.csv(
         paste(
            prefixStr,
            get(
               "glob_spolTsvStr",
               envir = .GlobalEnv
            ),
            sep = ""
         ),
         header = TRUE,
         sep = "\t"
      ); # for spoligotyping reads are often used

   #******************************************************
   # TOF01 Fun12 Sec03 Sub02:
   #   - handle "no spoligotype" case
   #******************************************************

   if(length(dataDF$strain) == 0){
      tkconfigure(
         spolOctal_labTK,
         text = "Octal: NA"
      );
   
      tkconfigure(
         spolStrain_labTK,
         text = "Strain: NA"
      );
   
      tkconfigure(
         spolSIT_labTK,
         text = "SIT: NA"
      );
   
      tkconfigure(
         spolCountry_labTK,
         text = "Countries: NA"
      );

   }else{
      tkconfigure(
         spolOctal_labTK,
         text =
            paste(
               "Octal:",
               as.character(dataDF$octal)
            )
      );
   
      tkconfigure(
         spolStrain_labTK,
         text =
            paste(
               "Strain:",
               as.character(dataDF$strain)
            )
      );
   
      tkconfigure(
         spolSIT_labTK,
         text =
            paste(
               "SIT:",
               as.character(dataDF$SIT)
            )
      );

      tkconfigure(
         spolCountry_labTK,
         text =
            paste(
               "Countries:",
                as.character(dataDF$countries)
            )
      );
   } # see if had spoligotypes

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun12 Sec04:
   #   - build consensus spoligotype table
   #   o tof01 fun12 sec04 sub01:
   #     - read in spoligotypes
   #   o tof01 fun12 sec04 sub02:
   #     - add spoligotype data to report
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF01 Fun12 Sec04 Sub01:
   #   - read in spoligotypes
   #******************************************************

   dataDF =
      read.csv(
         paste(
            prefixStr,
            get(
               "glob_conSpolTsvStr",
               envir = .GlobalEnv
            ),
            sep = ""
         ),
         header = TRUE,
         sep = "\t"
      ); # for spoligotyping reads are often used

   #******************************************************
   # TOF01 Fun12 Sec04 Sub02:
   #   - handle "no spoligotype" case
   #******************************************************

   if(length(dataDF$strain) == 0){
      tkconfigure(
         conSpolOctal_labTK,
         text = "Octal: NA"
      );
   
      tkconfigure(
         conSpolStrain_labTK,
         text = "Strain: NA"
      );
   
      tkconfigure(
         conSpolSIT_labTK,
         text = "SIT: NA"
      );
   
      tkconfigure(
         conSpolCountry_labTK,
         text = "Countries: NA"
      );

   }else{
      tkconfigure(
         conSpolOctal_labTK,
         text =
            paste(
               "Octal:",
               as.character(dataDF$octal)
            )
      );
   
      tkconfigure(
         conSpolStrain_labTK,
         text =
            paste(
               "Strain:",
               as.character(dataDF$strain)
            )
      );
   
      tkconfigure(
         conSpolSIT_labTK,
         text =
            paste(
               "SIT:",
               as.character(dataDF$SIT)
            )
      );

      tkconfigure(
         conSpolCountry_labTK,
         text =
            paste(
               "Countries:",
                as.character(dataDF$countries)
            )
      );
   } # see if had conSpoligotypes

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun12 Sec05:
   #   - build MIRU VNTR table
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   dataDF =
      read.csv(
         paste(
            prefixStr,
            get(
               "glob_miruTsvStr",
               envir = .GlobalEnv
            ),
            sep = ""
         ),
         header = TRUE,
         sep = "\t"
      ); # for MIRU VNTR, the consensus is often used

   labStr =
      unique(
         as.character(
            dataDF[
               2,
               2:length(dataDF[1,])
            ]
         )
      );

   if(length(labStr) == 1 && labStr[1] == "NA"){
      tkconfigure(
         miru_labTK,
         text = "MIRU: NA\n"
      ); # pack label

   }else{   
      labStr = as.character(dataDF[2, lineageAryUI]);

      tkconfigure(
         miru_labTK,
         text =
            paste(
               "MIRU: ",
               paste(
                  labStr,
                  collapse = " "
               ), # merge lineage
               sep = ""
            ) # collapse lineage arrays into string
      );
   } # check if I have a MIRU VNTR lineage
} # mkReportGui_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun13: buildOutGUI_freezeTBGui
#   - builds output gui (adds all widgets)
# Input:
# Output:
#   - builds output GUI
#---------------------------------------------------------
buildOutGui_freezeTBGui =
function(
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF01 Fun13 TOC:
   #   - displays output gui for freezeTB
   #   o tof01 fun13 sec01:
   #     - build and or add graphs to guie
   #   o tof01 fun13 sec02:
   #     - build and add AMR tables to GUI
   #   o tof01 fun13 sec03:
   #     - build report GUI
   #   o tof01 fun13 sec04:
   #     - build menu
   #   o tof01 fun13 sec05:
   #     - dispaly GUI
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun13 Sec01:
   #   - graph part of gui
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   assign(
      "depth_labTK",
      tklabel(
         outDepthTK,
         image = "",
         compound = "top"
      ),
      envir = .GlobalEnv
   );

   tkpack(outDepthTK, depth_labTK);

   assign(
      "coverage_labTK",
      tklabel(
         outCoverageTK,
         image = "",
         compound = "top"
      ),
      envir = .GlobalEnv
   );

   tkpack(outCoverageTK, coverage_labTK);

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun13 Sec02:
   #   - build and add AMR tables to GUI
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   assign(
      "conAmr_labTK",
      tklabel(
         conAmrTK,
         text = "",
         font = glob_monoFontTK
      ),
      envir = .GlobalEnv
   );

   tkpack(conAmrTK, conAmr_labTK, side = "top");

   assign(
      "readAmr_labTK",
      tklabel(
         readAmrTK,
         text = "",
         font = glob_monoFontTK
      ),
      envir = .GlobalEnv
   );

   tkpack(readAmrTK, readAmr_labTK, side = "top");

   tkpack(amrTblTK, conAmrTK, side = "left");
   tkpack(amrTblTK, readAmrTK, side = "right");

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun13 Sec03:
   #   - build report GUI
   #   o tof01 fun13 sec03 sub01:
   #     - set up frames for report GUI
   #   o tof01 fun13 sec03 sub02:
   #     - set up consensus AMR labels
   #   o tof01 fun13 sec03 sub03:
   #     - set up read AMR labels
   #   o tof01 fun13 sec03 sub04:
   #     - set up read spoligotype labels
   #   o tof01 fun13 sec03 sub05:
   #     - set up consensus spoligotype labels
   #   o tof01 fun13 sec03 sub06:
   #     - set up MIRU VNTR label
   #   o tof01 fun13 sec03 sub07:
   #     - pack the report frames
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF01 Fun13 Sec03 Sub01:
   #   - set up frames for report GUI
   #******************************************************

   conAmrTK =
      tkframe(
         outAmrRepTK,
         borderwidth = 2
      );

   readAmrTK =
      tkframe(
         outAmrRepTK,
         borderwidth = 2
      );

   spolTK =
      tkframe(
         outReportTK,
         borderwidth = 2
      );

   miruTK =
      tkframe(
         outReportTK,
         borderwidth = 2
      );

   #******************************************************
   # TOF01 Fun13 Sec03 Sub02:
   #   - set up consensus AMR labels
   #******************************************************

   amrCon_labTK =
      tklabel(
         conAmrTK,
         text = "con__AMRs:"
      );

   tkpack(
      amrCon_labTK,
      side = "left"
   );

   assign(
      "glob_conAmrPathStr",
      as.character(
         tkwinfo(
            "parent",
            amrCon_labTK
         ) # get path of parent
      ), # convert to string
      envir = .GlobalEnv
   );

   for(strAmr in glob_drugAbvAry)
   { # Loop: build amr labels
      pathStr =
           paste(
              glob_conAmrPathStr,
              ".con",
              strAmr,
              "LabTK",
              sep = ""
           );

      tkpack(
         tcl(
           "label",
           as.character(pathStr),
           text = strAmr,
           bg = glob_colPalAry[1],
           fg = glob_colPalAry[8]
         ),
         side = "left"
      );

      tkpack(
         tklabel(
            conAmrTK,
            text = ""
         ), # spacing
         side = "left"
      );
   } # Loop: build amr labels

   tkpack(
      conAmrTK
   );

   #******************************************************
   # TOF01 Fun13 Sec03 Sub03:
   #   - set up read AMR labels
   #******************************************************

   amrRead_labTK =
      tklabel(
         readAmrTK,
         text = "read__AMRs:"
      );

   tkpack(
      amrRead_labTK,
      side = "left"
   );

   assign(
      "glob_readAmrPathStr",
      as.character(
         tkwinfo(
            "parent",
            amrRead_labTK
         ) # get path of parent
      ), # convert to string
      envir = .GlobalEnv
   );

   for(strAmr in glob_drugAbvAry)
   { # Loop: build amr labels
      pathStr =
           paste(
              glob_readAmrPathStr,
              ".read",
              strAmr,
              "LabTK",
              sep = ""
           );

      tkpack(
        tcl(
          "label",
          as.character(pathStr),
          text = strAmr,
          bg = glob_colPalAry[1],
          fg = glob_colPalAry[8]
        ),
        side = "left"
      );

      tkpack(
         tklabel(
            readAmrTK,
            text = ""
         ), # spacing
         side = "left"
      );
   } # Loop: build amr labels

   tkpack(
      readAmrTK
   );

   #******************************************************
   # TOF01 Fun13 Sec03 Sub04:
   #   - set up read spoligotype labels
   #******************************************************

    tkgrid(
       tklabel(
          spolTK,
          text = "read spoligotype:"
       ),
       sticky = "w"
   );

   assign(
      "spolOctal_labTK",
      tklabel(
         spolTK,
         text = "Octal: NA"
      ),
      envir = .GlobalEnv
    );

    tkgrid(
       tklabel(
          spolTK,
          text = ""
       ),
       spolOctal_labTK,
       sticky = "w"
   );

   assign(
      "spolStrain_labTK",
      tklabel(
         spolTK,
         text = "Strain: NA"
      ),
      envir = .GlobalEnv
    );

    tkgrid(
       tklabel(
          spolTK,
          text = ""
       ),
       spolStrain_labTK,
       sticky = "w"
   );


   assign(
      "spolSIT_labTK",
      tklabel(
         spolTK,
         text = "SIT: NA"
      ),
      envir = .GlobalEnv
    );

    tkgrid(
       tklabel(
          spolTK,
          text = ""
       ),
       spolSIT_labTK,
       sticky = "w"
   );

   assign(
      "spolCountry_labTK",
      tklabel(
         spolTK,
         text = "Countries: NA"
      ),
      envir = .GlobalEnv
    );

    tkgrid(
       tklabel(
          spolTK,
          text = ""
       ),
       spolCountry_labTK,
       sticky = "w"
   );

   #******************************************************
   # TOF01 Fun13 Sec03 Sub05:
   #   - set up consensus spoligotype labels
   #******************************************************

    tkgrid(
       tklabel(
          spolTK,
          text = "consensus spoligotype:"
       ),
       sticky = "w"
   );

   assign(
      "conSpolOctal_labTK",
      tklabel(
         spolTK,
         text = "Octal: NA"
      ),
      envir = .GlobalEnv
    );

    tkgrid(
       tklabel(
          spolTK,
          text = ""
       ),
       conSpolOctal_labTK,
       sticky = "w"
   );

   assign(
      "conSpolStrain_labTK",
      tklabel(
         spolTK,
         text = "Strain: NA"
      ),
      envir = .GlobalEnv
    );

    tkgrid(
       tklabel(
          spolTK,
          text = ""
       ),
       conSpolStrain_labTK,
       sticky = "w"
   );


   assign(
      "conSpolSIT_labTK",
      tklabel(
         spolTK,
         text = "SIT: NA"
      ),
      envir = .GlobalEnv
    );

    tkgrid(
       tklabel(
          spolTK,
          text = ""
       ),
       conSpolSIT_labTK,
       sticky = "w"
   );

   assign(
      "conSpolCountry_labTK",
      tklabel(
         spolTK,
         text = "Countries: NA"
      ),
      envir = .GlobalEnv
    );

    tkgrid(
       tklabel(
          spolTK,
          text = ""
       ),
       conSpolCountry_labTK,
       sticky = "w"
   );

   #******************************************************
   # TOF01 Fun13 Sec03 Sub06:
   #   - set up MIRU VNTR label
   #******************************************************

    assign(
       "miru_labTK",
       tklabel(
          miruTK,
          text = "MIRU: NA\n"
       ), # build label
       envir = .GlobalEnv
    );

    tkpack(
        miru_labTK,
        side = "left"
    ); # pack label

   #******************************************************
   # TOF01 Fun13 Sec03 Sub07:
   #   - pack the report frames
   #******************************************************

   tkgrid(
      spolTK,
      sticky = "w"
   );

   tkgrid(
      miruTK,
      sticky = "w"
   );

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun13 Sec04:
   #   - build menu
   #   o tof01 fun13 sec04 sub01:
   #     - build menu buttons always present
   #   o tof01 fun13 sec04 sub02:
   #     - build add graph buttons + build graph menu
   #   o tof01 fun13 sec04 sub03:
   #     - if no graphs, buid non graph menu
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF01 Fun13 Sec04 Sub01:
   #   - build menu buttons always present
   #******************************************************

   showReport_butTK =
      tkbutton(
         outMenuTK,
         text = "report",
         command = showReport_freezeTBGui
      );

   showAmr_butTK =
      tkbutton(
         outMenuTK,
         text = "AMRs",
         command = showAmr_freezeTBGui
      );

   depth_butTK =
      tkbutton(
         outMenuTK,
         text = "read depth",
         command = showDepth_freezeTBGui
      );

   coverage_butTK =
      tkbutton(
         outMenuTK,
         text = "coverage",
         command = showCoverage_freezeTBGui
      );

   showSelect_butTK =
      tkbutton(
         outMenuTK,
         text = "select results",
         command = showSelect_freezeTBGui
      );

   inputGUI_butTK =
      tkbutton(
         outMenuTK,
         text = "input",
         command = showReqInputMenu
      );

   outQuit_butTK =
      tkbutton(
         outMenuTK,
         text = "exit",
         command = outExit_freezeTBGui
      );


   tkgrid(
      showReport_butTK,
      depth_butTK,
      coverage_butTK,
      showAmr_butTK,
      showSelect_butTK,
      inputGUI_butTK,
      outQuit_butTK
   );

   tkpack(outDataTK, outReportTK);
} # buildOutGui_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun14: outGUI_freezeTBGui
#   - displays output gui for freezeTB
# Input:
#   - prefixStr:
#     o string with prefix for output files and for graphs
#   - amrDBStr:
#     o string with path to tbAmr database (as tsv)
#   - minIndelPercF:
#     o minimum percentage (0-100) of reads to keep indel
#   - minPercSupF:
#     o minimum percentage (0-100) of reads to keep AMR
#   - minDepthUI:
#     o minimum read depth to keep AMR
#   - graphBl:
#     o TRUE: build graphs
#     o FALSE: do not build graphs
# Output:
#   - builds read depth graph for freezeTB (to dev)
#---------------------------------------------------------
outGUI_freezeTBGui = function(
   prefixStr,     # prefix to call everything
   amrDBStr,      # path to amr database (tsv)
   minIndelPercF = 30,
   minPercSupF = 1,
   minDepthUI = 10,
   graphBl = TRUE # TRUE = build/dispaly graphs
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF01 Fun14 TOC:
   #   - displays output gui for freezeTB
   #   o tof01 fun14 sec01:
   #     - build and or add graphs to guie
   #   o tof01 fun14 sec02:
   #     - build and add AMR tables to GUI
   #   o tof01 fun14 sec03:
   #     - build report GUI and display
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun14 Sec01:
   #   - build and or add graphs to guie
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   haveGraphsBl = FALSE;

   readDepthGraphStr =
      paste(
         prefixStr,
         glob_depthPngStr,
         sep = ""
      );

   coverageGraphStr =
      paste(
         prefixStr,
         glob_coveragePngStr,
         sep = ""
      );

   if(glob_oldGraphBl)
   { # If: have previous graphs displayed
      tcl(
         "image",
         "delete",
         "depth_imageTK",
         "coverage_imageTK"
      );

      tkconfigure(
         depth_labTK,
         image = "",
         compound = "top"
      );

      tkconfigure(
         coverage_labTK,
         image = "",
         compound = "top"
      );
   } # If: have previous graphs displayed

   if(graphBl == TRUE)
   { # If: building graphs
      depthDataDF=getAmpDepthData_freezeTBGui(prefixStr);

      if( ! is.null(depthDataDF) )
      { # If: can make graphs
         haveGraphsBl = TRUE;

         if( ! file.exists(readDepthGraphStr) )
         { # If: no read depth graph made
            mkDepthPlot_freezeTBGui(
               depthDataDF,
               prefixStr
            );
         } # If: no read depth graph made

         tcl(
            "image",
            "create",
            "photo",
            "depth_imageTK",
            file = readDepthGraphStr
         );

         tkconfigure(
            depth_labTK,
            image = "depth_imageTK",
            compound = "top"
         );

         if( ! file.exists(coverageGraphStr) )
         { # If: no coverage graph made
            mkCoveragePlot_freezeTBGui(
               depthDataDF,
               prefixStr,
               amrDBStr
            );
         } # If: no coverage graph made

         tcl(
            "image",
            "create",
            "photo",
            "coverage_imageTK",
            file = coverageGraphStr
         );

         tkconfigure(
            coverage_labTK,
            image = "coverage_imageTK",
            compound = "top"
         );

         assign(
            "glob_oldGraphBl",
            TRUE,
            envir = .GlobalEnv
         );
      } # If: can make graphs
   } # If: building/displaying graphs

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun14 Sec02:
   #   - build and add AMR tables to GUI
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   labStr = mkConAmrTbl_freezeTBGui(prefixStr);

   tkconfigure(
      conAmr_labTK,
      text = labStr
   );

   labStr = mkReadAmrTbl_freezeTBGui(prefixStr);

   tkconfigure(
      readAmr_labTK,
      text = labStr
   );

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun14 Sec03:
   #   - build report GUI and display
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   mkReportGui_freezeTBGui(
      prefixStr,
      minIndelPercF = minIndelPercF,
      minPercSupF = minPercSupF,
      minDepthUI = minDepthUI
   );

   showReport_freezeTBGui();
} # outGUI_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun15: getPrefix_freezeTBGui
#   - gets prefix from selected file
# Input:
# Output:
#---------------------------------------------------------
getPrefix_freezeTBGui =
function(
){
   if(glob_dirStr == ""){
      dirStr = getwd();
   }else{
      dirStr = glob_dirStr;
   }

   fileStr =
      tk_choose.files(
         default = dirStr, # directory to start
         caption = "Select freezeTB output tsv/png",
         multi = FALSE,
         filters =
            matrix(
               c(
                  "tsv*", ".tsv",
                  "png*", ".png"
                ), # allowed files (filters)
             2, # two rows
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an fastq file

   if(! is.na(fileStr[1])){
      assign(
         "glob_dirStr",
         dirname(fileStr[1]),
         envir = .GlobalEnv
      );

      # find the file prefix
      fileStr = basename(fileStr);

      fileStr[1] =
         sub(
            "-con.*",
            "",
            fileStr[1]
         );

      fileStr[1] =
         sub(
            "-read.*",
            "",
            fileStr[1]
         );

      fileStr[1] =
         sub(
            "-depth.*",
            "",
            fileStr[1]
         );

      fileStr[1] =
         sub(
            "-coverage.*",
            "",
            fileStr[1]
         );

      fileStr[1] =
         sub(
            "-readDepth.*",
            "",
            fileStr[1]
         );

      assign(
         "glob_prefixStr",
         fileStr[1],
         envir = .GlobalEnv
      ); # assign prefix to global prefix variable

      tkconfigure(
         outGuiPrefix_labTK,
         text = glob_prefixStr
      ); # add file name to gui

      tkconfigure(
         outGuiDir_labTK,
         text = glob_dirStr
      ); # add file name to gui
   } # If: have files
} # getPrefix_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun16: getOutDB_freezeTBGui
#   - gets amr database for output gui
# Input:
# Output:
#---------------------------------------------------------
getOutDB_freezeTBGui =
function(
){
   if(glob_dirStr == ""){
      dirStr = getwd();
   }else{
      dirStr = glob_dirStr;
   }

   fileStr =
      tk_choose.files(
         default = dirStr, # directory to start
         caption = "Select freezeTB AMR database",
         multi = FALSE,
         filters =
            matrix(
               c(
                  "tsv*", ".tsv",
                ), # allowed files (filters)
             1, # two rows
             2, # two columns
             byrow = TRUE
         ) # File types allowd to see
      ); # Let the user select an fastq file

   if(! is.na(fileStr[1])){
      assign(
         "glob_outAmrDbStr",
         fileStr[1],
         envir = .GlobalEnv
      ); # assign prefix to global prefix variable

      tkconfigure(
         outGuiAmrDB_labTK,
         text = glob_outAmrDbStr
      ); # add file name to gui
   } # If: have files
} # getOutDB_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun17: getResults_freezeTBGui
#   - launches the output GUI for freezeTB
# Input:
# Output:
#---------------------------------------------------------
getResults_freezeTBGui =
function(
){
    if(
       ! file.exists(
          paste(
             glob_dirStr,
             slashSC,
             glob_prefixStr,
             glob_conAmrTsvStr,
             sep = ""
          ) # build consensus AMR file path
      )
   ){ # If: invalid file path
      tkconfigure(
         outGuiPrefix_labTK,
         text = "could not open input"
       );
   }else{
      if( ! as.numeric(tclvalue(glob_graphBl)) ){
         graphBl = FALSE;
      }else{
         graphBl = TRUE;
      }

      tkpack.forget(outSelectTK);

      outGUI_freezeTBGui(
         paste(glob_dirStr,glob_prefixStr,sep = slashSC),
         glob_outAmrDbStr,
         minIndelPercF =
            as.numeric(tclvalue(glob_minIndelPercF)),
         minPercSupF =
            as.numeric(tclvalue(glob_minPercSupF)),
         minDepthUI =
            as.numeric(tclvalue(glob_minDepthUI)),
         graphBl
      );
   } # Else: launch output GUI
} # getResults_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun18: buildSelect_freezeTBGui
#   - builds the output select GUI
# Input:
# Output:
#   - builds and then launches the select GUI for output
#---------------------------------------------------------
buildSelect_freezeTBGui =
function(
){
   buildOutGui_freezeTBGui();

   assign(
      "outGuiDir_labTK",
      tklabel(
         outSelectTK,
         text = glob_dirStr
      ),
      envir = .GlobalEnv
   );


   assign(
      "outGuiPrefix_labTK",
      tklabel(
         outSelectTK,
         text = glob_prefixStr
      ),
      envir = .GlobalEnv
   );

    outGuiPrefix_butTK =
      tkbutton(
         outSelectTK,
         text = "get prefix",
         command = getPrefix_freezeTBGui
      );

   tkgrid(
      outGuiPrefix_butTK,
      outGuiPrefix_labTK,
      outGuiDir_labTK,
      sticky = "w"
    );

   tkgrid(
       tklabel(
          parent = outSelectTK,
          text = "min % indel read support"
       ),
       tkscale(
          parent = outSelectTK,
          from = 0,
          to = 100,
          showvalue = TRUE,
          variable = glob_minIndelPercF,
          resolution = 1,
          orient = "horiz"
       ),
      sticky = "w"
   )

   tkgrid(
       tklabel(
          parent = outSelectTK,
          text = "min % read support"
       ),
       tkscale(
          parent = outSelectTK,
          from = 0,
          to = 100,
          showvalue = TRUE,
          variable = glob_minPercSupF,
          resolution = 1,
          orient = "horiz"
       ),
      sticky = "w"
   )

   tkgrid(
       tklabel(
          parent = outSelectTK,
          text = "min read depth"
       ),
       tkscale(
          parent = outSelectTK,
          from = 0,
          to = 100,
          showvalue = TRUE,
          variable = glob_minDepthUI,
          resolution = 1,
          orient = "horiz"
       ),
      sticky = "w"
   )

   tkgrid(
       tkcheckbutton(
          parent = outSelectTK,
          text = "build/show graphs",
          variable = glob_graphBl
       ),
      sticky = "w"
   ); # check box for building graphs


   assign(
      "outGuiAmrDB_labTK",
      tklabel(
         outSelectTK,
         text = glob_outAmrDbStr
      ),
      envir = .GlobalEnv
   );

   tkgrid(
      tkbutton(
         outSelectTK,
         text = "AMR database",
         command = getOutDB_freezeTBGui
      ),
      outGuiAmrDB_labTK
   ); # set up AMR database button/label

   tkgrid(
      tkbutton(
         outSelectTK,
         text = "get results",
         command = getResults_freezeTBGui
      ),
      sticky = "w"
   );


   tkpack(outDataTK);
   tkpack(outSelectTK);
} # buildSelect_freezeTBGui

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# TOF02:
#   - input gui functions (buttons/widgets)
#   o tof02 fun01: getReads
#     - Gets a fastq file that the user selected
#   o tof02 fun02: getSaveDir
#     - Gets the directory to save everything to
#   o tof02 fun03: getRef
#     - Gets reference sequence fasta file form user
#   o tof02 fun04: getAmrTbl
#     - Gets an AMR table (tbAmr)
#   o tof02 fun05: getMiruTbl
#     - Gets an Miru table (tbMiru format)
#   o tof02 fun06: getSpoligoSeq
#     - Gets the spol space sequences for spoltyping
#   o tof02 fun07: getSpoligoDB
#     - Gets the lineage database for spoltyping
#   o tof02 fun08: getGeneTbl
#     - Gets the paf file with the genome coordinates
#   o tof02 fun09: getMaskPrim
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

glob_readDirStr = ""; # last directory with reads

#---------------------------------------------------------
# TOF02 Fun01: getReads
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

   if( glob_readDirStr == "" ){
      dirStr = getwd();
   }else{
      dirStr = glob_readDirStr;
   }

   fileMacStr =
      tk_choose.files(
         default = dirStr, # start of in working dir
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
      assign(
         "glob_readDirStr",
         dirname(fileMacStr[1]),
         envir = .GlobalEnv
      ); # so can use as default directory later

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
# TOF02 Fun02: getSaveDir
#   - Gets the directory to save everything to
# Input:
# Output:
#   - Returns:
#     o An string with the selected file name
#---------------------------------------------------------
getSaveDir =
function(
){
   dirStr = tkcget(dirLabelTK, "-text");

   if( ! dir.exists(dirStr) )
      dirStr = getwd();

   dirMacStr =
      tk_choose.dir(
         default = dirStr, # start of in working dir
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
# TOF02 Fun03: getRef
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
# TOF02 Fun04: getAmrTbl
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
# TOF02 Fun05: getMiruTbl
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
# TOF02 Fun06: getSpoligoSeq
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
# TOF02 Fun07: getSpoligoDB
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
# TOF02 Fun09: getMaskPrim
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
# TOF03:
#   - input gui hide/show menu functions + run
#   o tof03 fun01: showReqInputMenu
#     - Shows the required input menu
#   o tof03 fun02: showFiltMenu
#     - Shows the read filtering input menu
#   o tof03 fun03: showAmrMenu
#     - Shows the AMR settings menu
#   o tof03 fun04: showLineageMenu
#     - Shows the MIRU lineage menu
#   o tof03 fun05: showConMenu
#     - Shows the extra settings for consensus building
#   o tof03 fun06: showMixedInfectMenu
#     - Shows settings for mixed infection
#   o tof03 fun07: showGraphMenu
#     - Shows the depth/coverage graphing menu
#   o tof03 fun08: showStatus
#     - Shows the status screen of freezeTB gui
#   o tof03 fun09: quitInput
#     - Quits freezeTb input gui
#   o tof03 fun10: runFreezeTb
#     - Runs freezeTb
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# TOF03 Fun01: showReqInputMenu
#   - Shows the required input menu
# Input:
# Output:
#---------------------------------------------------------
showReqInputMenu =
function(
){
   tkpack.forget(outMenuTK);
   tkpack.forget(outDataTK);
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

   tkwm.title(
      guiTK,
      "required input"
   );
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, reqInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showReqInputMenu

#---------------------------------------------------------
# TOF03 Fun02: showFiltMenu
#   - Shows the read filtering input menu
# Input:
# Output:
#---------------------------------------------------------
showFiltMenu =
function(
){
   tkpack.forget(outMenuTK);
   tkpack.forget(outDataTK);
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
   
   tkwm.title(
      guiTK,
      "mapping and filtering settings"
   );

   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, filtInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showReqInputMenu

#---------------------------------------------------------
# TOF03 Fun03: showAmrMenu
#   - Shows the AMR settings menu
# Input:
# Output:
#---------------------------------------------------------
showAmrMenu =
function(
){
   tkpack.forget(outMenuTK);
   tkpack.forget(outDataTK);
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

   tkwm.title(
      guiTK,
      "AMR settings"
   );
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, amrInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showFileMenu

#---------------------------------------------------------
# TOF03 Fun04: showLineageMenu
#   - Shows the MIRU lineage menu
# Input:
# Output:
#---------------------------------------------------------
showLineageMenu =
function(
){
   tkpack.forget(outMenuTK);
   tkpack.forget(outDataTK);
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

   tkwm.title(
      guiTK,
      "lineage detection settings"
   );
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, lineageInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showFileMenu

#---------------------------------------------------------
# TOF03 Fun05: showConMenu
#   - Shows the extra settings for consensus building
# Input:
# Output:
#---------------------------------------------------------
showConMenu =
function(
){
   tkpack.forget(outMenuTK);
   tkpack.forget(outDataTK);
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

   tkwm.title(
      guiTK,
      "consensus building settings"
   );
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, conInputTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showConMenu

#---------------------------------------------------------
# TOF03 Fun06: showMixedInfectMenu
#   - Shows settings for mixed infection
# Input:
# Output:
#---------------------------------------------------------
showMixedInfectMenu =
function(
){
   tkpack.forget(outMenuTK);
   tkpack.forget(outDataTK);
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

   tkwm.title(
      guiTK,
      "mixed infection detection settings"
   );
   
   tkpack(runQuitTK, optionsTK, side = "top");
   tkpack(optionsTK, mixedInfectTK, side = "bottom");

   tkpack(menuTK, optionsTK, side = "bottom");
   tkpack(runQuitTK, side = "bottom");
} # showMixedInfectMenu

#---------------------------------------------------------
# TOF03 Fun08: showStatus
#   - Shows the status screen of freezeTB gui
# Input:
# Output:
#---------------------------------------------------------
showStatus =
function(
){
   tkpack.forget(outMenuTK);
   tkpack.forget(outDataTK);
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
   
   tkwm.title(
      guiTK,
      "feezeTB is running"
   );

   tkpack(statusTK, side = "top");
} # showStatus

#---------------------------------------------------------
# TOF03 Fun09: quitInput
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
# TOF03 Fun10: runFreezeTb
#   - Runs freezeTb
# Input:
# Output:
#   - Runs:
#     o Freeze tb
#---------------------------------------------------------
runFreezeTb =
function(
){ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF03 Fun10 TOC:
   #   - Runs freezeTb on input files
   #   o tof03 fun10 sec01:
   #     - Variable declerations
   #   o tof03 fun10 sec02:
   #     - Check if user provided an fastq file
   #   o tof03 fun10 sec03:
   #     - Build the prefix and minimap2 command
   #   o tof03 fun10 sec04:
   #     - Build freezeTb command
   #   o tof03 fun10 sec05:
   #     - Get minimap2 version, open log, ann map reads
   #   o tof03 fun10 sec06:
   #     - Run adjCoords (if requested)
   #   o tof03 fun10 sec07:
   #     - Run freezeTb
   #   o tof03 fun10 sec08:
   #     - Return and exit
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun10 Sec01:
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

   assign(
      "glob_outAmrDbStr",
      amrFileStr,
      envir = .GlobalEnv
   ); # make sure output and input use same database

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

   assign(
      "glob_dirStr",
      prefixStr,
      envir = .GlobalEnv
   );

   argsMinimapStr = "";

   tbCmdStr = ""; # Should be in users path

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun10 Sec02:
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
   # TOF03 Fun10 Sec03:
   #   - Build the prefix and minimap2 command
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   assign(
      "glob_prefixStr",
      tclvalue(PREFIX_TCL),
      envir = .GlobalEnv
   );
      
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

   if( ! as.numeric(tclvalue(glob_shortReadsBl_tcl)) ){
      mapSetStr = "-a -x map-ont -o";
   }else{
      mapSetStr = "-a -x sr -o";
   }

   argsMinimapStr =
      paste(
         mapSetStr,
         '"',
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
   # TOF03 Fun10 Sec04:
   #   - Build freezeTb command
   #   o tof03 fun10 sec04 sub01:
   #     - Build read filtering part of freezeTb command
   #   o tof03 fun10 sec04 sub02:
   #     - Add AMR settings/check if AMR database exists
   #   o tof03 fun10 sec04 sub03:
   #     - Add MIRU lineage commands/check MIRU database
   #   o tof03 fun10 sec04 sub04:
   #     - Add in spoltype lineage comands
   #   o tof03 fun10 sec04 sub05:
   #     - Add in the consensus commands
   #   o tof03 fun10 sec04 sub06:
   #     - mixed infection (clustering) commands
   #   o tof03 fun10 sec04 sub07:
   #     - Add in the read depth/coverage commands
   #   o tof03 fun10 sec04 sub08:
   #     - Add in the required input commands
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF03 Fun10 Sec04 Sub01:
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
   # TOF03 Fun10 Sec04 Sub02:
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
   # TOF03 Fun10 Sec04 Sub03:
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
   # TOF03 Fun10 Sec04 Sub04:
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
   # TOF03 Fun10 Sec04 Sub05:
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
   # TOF03 Fun10 Sec04 Sub06:
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
   # TOF03 Fun10 Sec04 Sub07:
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
   # TOF03 Fun10 Sec04 Sub08:
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
   # TOF03 Fun10 Sec05:
   #   - Get minimap2 version, open log, ann map reads
   #   o tof03 fun10 sec05 Sub01:
   #     - Get and save version for minimap2 to log
   #   o tof03 fun10 sec05 Sub02:
   #     - Map reads
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF03 Fun10 Sec05 Sub01:
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
   # TOF03 Fun10 Sec05 Sub02:
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
   # TOF03 Fun10 Sec07:
   #   - Run freezeTB
   #   o tof03 fun10 sec07 sub01:
   #     - get freezeTB version
   #   o tof03 fun10 sec07 sub02:
   #     - run freezeTB
   #   o tof03 fun10 sec07 sub03:
   #     - build graphs for freezeTB
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF03 Fun10 Sec07 Sub01:
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
   # TOF03 Fun10 Sec07 Sub02:
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
   # TOF03 Fun10 Sec07 Sub03:
   #   - display output (also builds graphs)
   #******************************************************

   if( ! as.numeric(tclvalue(glob_graphBl)) ){
      graphBl = FALSE;
   }else{
      graphBl = TRUE;
   }

   outGUI_freezeTBGui(
      prefixStr,
      amrFileStr,
      minIndelPercF =
         as.numeric(tclvalue(glob_minIndelPercF)),
      minPercSupF =
         as.numeric(tclvalue(glob_minPercSupF)),
      minDepthUI =
         as.numeric(tclvalue(glob_minDepthUI)),
      graphBl
   );

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF03 Fun10 Sec08:
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
# TOF04:
#   - input gui hide/show menu functions + run
#   o tof04 fun01: reqInputSetup_freezeTBGui
#     - builds required input part of input gui
#   o tof04 fun02: menuSetup_freezeTBGui
#     - builds menu part of input gui
#   o tof04 fun03: exitRunSetup_freezeTBGui
#     - adds exit and run commands to input gui
#   o tof04 fun04: filtMenuSetup_freezeTBGui
#     - builds the filter settings frame
#   o tof04 fun05: amrMenuSetup_freezeTBGui
#     - builds the AMR settings frame
#   o tof04 fun06: lineageMenuSetup_freezeTBGui
#     - builds the lineage settings frame
#   o tof04 fun07: conMenuSetup_freezeTBGui
#     - builds the consensus settings frame
#   o tof04 fun08: mixefInfectSetup_freezeTBGui
#     - builds mixed infections settings frame
#   o tof04 fun10: launchInGui_freezeTBGui
#     - launches input gui
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# TOF04 Fun01: reqInputSetup_freezeTBGui
#   - builds required input part of input gui
# Input:
# Output:
#   - Modifies:
#     o reqInputTK frame to have required input
#---------------------------------------------------------
reqInputSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF04 Fun01 TOC:
   #   - builds required input part of input gui
   #   o tof04 fun01 sec01:
   #     - add options to menu
   #   o tof04 fun01 sec02:
   #     - build required input menu
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF04 Fun01 Sec01:
   #   - add options to menu
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   
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
   # TOF04 Fun01 Sec02:
   #   - build required input menu
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tkgrid(fqButTK, fqLabelTK);
   tkgrid(dirButTK, dirLabelTK);
   tkgrid(prefLabelTK, prefBoxTK);
   
   tkgrid.configure(
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
# TOF04 Fun02: menuSetup_freezeTBGui
#   - builds menu part of input gui
# Input:
# Output:
#   - Modifies:
#     o reqInputTK frame to have menu buttons
#---------------------------------------------------------
menuSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF04 Fun02 TOC:
   #   - builds menu part of input gui
   #   o tof04 fun02 sec01:
   #     - add menu buttons to gui
   #   o tof04 fun02 sec02:
   #     - build menu
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF04 Fun02 Sec01:
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


   viewOutput_but =
      tkbutton(
         menuTK,
         text = "view",
         command = showSelect_freezeTBGui
      ); # Show the extra consensus options
   
   if(TOOL_TIP_BL > 0){
      tk2tip(
         viewOutput_but,
         "view output from freezeTB"
      );
   } # If: I have tooltips

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF04 Fun02 Sec02:
   #   - build menu
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tkpack(menuTK, inputMenu_ButTK, side = "left");
   tkpack(menuTK, filtMenu_ButTK, side = "left");
   tkpack(menuTK, amrMenu_ButTK, side = "left");
   tkpack(menuTK, miruMenu_ButTK, side = "left");
   tkpack(menuTK, conMenu_ButTK, side = "left");
   tkpack(menuTK, mixedInfect_ButTK, side = "left");
   tkpack(menuTK, viewOutput_but, side = "left");
} # menuSetup_freezeTBGui

#---------------------------------------------------------
# TOF04 Fun03: exitRunSetup_freezeTBGui
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
# TOF04 Fun04: filtMenuSetup_freezeTBGui
#   - builds the filter settings frame
# Input:
# Output:
#   - Modifies:
#     o reqInputTK frame to have exit and run buttons
#---------------------------------------------------------
filtMenuSetup_freezeTBGui = function()
{ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # TOF04 Fun04 TOC:
  #   - builds the filter settings frame
  #   o tof04 fun04 sec01:
  #     - set up filtering options
  #   o tof04 fun04 sec02:
  #     - add filtering options to gui
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  # TOF04 Fun04 Sec01:
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


   assign(
      "refLabelTK",
      tklabel(
         filtInputTK,
         text = def_refStr
      ), # Current reference file used
      envir = .GlobalEnv
   );
   
   refButTK =
      tkbutton(
         filtInputTK,
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
        parent = filtInputTK,
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
         parent = filtInputTK,
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
         parent = filtInputTK,
         text = def_coordsStr
      ), # Current gene coordiantes table used
      envir = .GlobalEnv
   );


  sr_checkboxTK =
     tkcheckbutton(
        parent = filtInputTK,
        text = "map short reads",
        variable = glob_shortReadsBl_tcl
     );

   if(TOOL_TIP_BL > 0){
      tk2tip(
         sr_checkboxTK,
         "use short read settings (-x sr) with minimap2"
      ); # gene table button tootip
   } # If: I can add an tooltip

  #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  # TOF04 Fun04 Sec02:
  #   - add filtering options to gui
  #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tkgrid(filtTitle_LabTK);

   tkgrid(
      refButTK,
      refLabelTK
   );

   tkgrid(filtMinMapq_LabTK, filtMinMapq_SlideTK);
   tkgrid(filtMinMedQ_LabTK, filtMinMedQ_SlideTK);
   tkgrid(filtMinMeanQ_LabTK, filtMinMeanQ_SlideTK);
   tkgrid(filtMinDepth_LabTK, filtMinDepth_SlideTK);

   tkgrid(
      maskPrim_checkboxTK,
      maskPrim_ButTK,
      maskPrim_LabTK
   );

   tkgrid(
      adjCoords_checkboxTK,
      coordsTbl_ButTK,
      coordsTbl_LabTK
   );

   tkgrid(sr_checkboxTK);
   
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
      refButTK,
      refLabelTK,
      adjCoords_checkboxTK,
      coordsTbl_ButTK,
      coordsTbl_LabTK,
      sr_checkboxTK,
      sticky = "w"
   );

} # filtMenuSetup_freezeTBGui

#---------------------------------------------------------
# TOF04 Fun05: amrMenuSetup_freezeTBGui
#   - builds AMR settings frame
# Input:
# Output:
#   - Modifies:
#     o amrInputTK frame to have amr settings
#---------------------------------------------------------
amrMenuSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF04 Fun05 TOC:
   #   - builds AMR settings frame
   #   o tof04 fun05 sec01:
   #    - setup AMR settinngs
   #   o tof04 fun05 sec02:
   #    - add AMR settinngs to gui
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF04 Fun05 Sec01:
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
   # TOF04 Fun05 Sec02:
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
# TOF04 Fun06: lineageMenuSetup_freezeTBGui
#   - builds the lineage settings frame
# Input:
# Output:
#   - Modifies:
#     o input gui to have lineage settings
#---------------------------------------------------------
lineageMenuSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF04 Fun06 TOC:
   #   - builds the lineage settings frame
   #   o tof04 fun06 sec01:
   #     - miru lineage detection
   #   o tof04 fun06 sec02:
   #     - spoligotype lineage detection
   #   o tof04 fun06 sec03:
   #     - add lineage options to gui
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF04 Fun06 Sec01:
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
   # TOF04 Fun06 Sec02:
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
   # TOF04 Fun06 Sec03:
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
# TOF04 Fun07: conMenuSetup_freezeTBGui
#   - builds the consensus settings frame
# Input:
# Output:
#   - Modifies:
#     o input gui to have consensus settings
#---------------------------------------------------------
conMenuSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF04 Fun07 TOC:
   #   - builds the consensus settings frame
   #   o tof04 fun07 sec01:
   #     - Consensus global (bulding) options
   #   o tof04 fun07 sec02:
   #     - Consensus collapsing (bulding) options
   #   o tof04 fun07 sec03:
   #     - Variant printing options for consensus
   #   o tof04 fun07 sec04:
   #     - add consensus menu to gui
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF04 Fun07 Sec01:
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
   # TOF04 Fun07 Sec02:
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
   # TOF04 Fun07 Sec03:
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
   # TOF04 Fun07 Sec04:
   #   - add consensus menu to gui
   #   o tof04 fun07 sec04 sub01:
   #     - set up the global consensus options frame
   #   o tof04 fun07 sec04 sub02:
   #     - set up the collapse consensus options frame
   #   o tof04 fun07 sec04 sub03:
   #     - set up print variants consensus options frame
   #   o tof04 fun07 sec04 sub04:
   #     - add consensus menu optoins to gui
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # TOF04 Fun07 Sec04 Sub01
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
   # TOF04 Fun07 Sec04 Sub02:
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
   # TOF04 Fun07 Sec04 Sub03:
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
   # TOF04 Fun07 Sec04 Sub04:
   #   - add consensus menu optoins to gui
   #******************************************************

   tkpack(conInputTK, conGlobalInputTK, side = "top");
   tkpack(conInputTK, conBuildInputTK, side = "left");
   tkpack(conInputTK, conPrintInputTK, side = "right");
} # conMenuSetup_freezeTBGui

#---------------------------------------------------------
# TOF04 Fun08: mixefInfectSetup_freezeTBGui
#   - builds mixed infections settings frame
# Input:
# Output:
#   - Modifies:
#     o input gui to have mixed infections settings
#---------------------------------------------------------
mixedInfectSetup_freezeTBGui = function()
{  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # TOF04 Fun08 TOC:
   #   - builds mixed infections settings frame
   #   o tof04 fun08 sec0x:
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
   # TOF04 Fun08 Sec0x:
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

   buildSelect_freezeTBGui();
   
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
