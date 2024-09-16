#!/usr/bin/env Rscript

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# graphAmpDepth.r SOF: Start Of File
#   - makes graphs for output from ampDepth
#   o part01:
#     - included libraries
#   o fun01: phelp_graphAmpDepth
#     - help message function
#   o fun02: findFIle_graphAmpDepth
#     - checks to see if a freezeTB file exists
#   o part02:
#     - script variables, user input, and prep
#   o part03:
#     - read depth graph
#   o part04:
#     - gene coverage graphs
#   o part05:
#     - amplicon coverage graphs
#   o license:
#     - Licensing for this code (public domain / mit)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# Part01:
#   - included libraries
#---------------------------------------------------------

library("ggplot2")
library("viridisLite")
library("data.table")
# sudo xbps-install R-cran-data.table

#---------------------------------------------------------
# Fun01: phelp_graphAmpDepth
#   - help message function
# Input:
#   - inStr:
#     o input argument to check for help message
# Output:
#   - Prints:
#     o help mesesage to screen if inStr has help flag
#   - Returns:
#     o TRUE if printed help message
#     o FALSE if inStr is not help flag (no printing)
#---------------------------------------------------------
phelp_graphAmpDepth = function(inStr){
 if(inStr == "-h" ||
    inStr == "--h" ||
    inStr == "-help" ||
    inStr == "--help" ||
    inStr == "help"){
       print("RScript graphAmpDepth.r -stats stats.tsv");
       print("  - Makes a graph of mapped amplicons");
       print("Input:");
       print("  -stats: [Required]");
       print("    o Tsv file with amplicon stats output");
       print("      by ampDepth");
       print("  -prefix: [Hufflepuff]");
       print("    o Prefix to add to output file names");
       print("  -min-len: [50]");
       print("    o minimum length to keep amplicon");
       print("  -ext: [tiff]");
       print("    o File extension to save graphs as");
       print("  -who: [freezeTBScripts/who-tb-2023-catalog-tbAmr-format.tsv]");
       print("    o WHO catalog in tbAmr format (tsv)");
       print("      having AMR locations to add to the");
       print("      coverage gaph");
       print("    o This is not done if the file can");
       print("      not be opened");
       print("    o use \"-who -\" to disable\"");
       print("Output:");
       print("  - Saves the graphs as file.ext (-ext)");
       print("    o The -readDepth.ext has the read");
       print("      depths per amplicon");
       print("    o The -ampMap.ext has the amplicon");
       print("      coverage for each amplicon");

       return(TRUE);
   } # If: I am printig the help message

   return(FALSE);
} # pGraphAmpDephtHelp

#---------------------------------------------------------
# Fun02: findFIle_graphAmpDepth
#   - checks to see if a freezeTB file exists
# Input:
#   - fileStr:
#     o freezeTB file to hunt for
# Output:
#   - Returns:
#     o path to file if exists
#     o NULL if file could not be found
#---------------------------------------------------------
findFile_graphAmpDepth = function(fileStr){

   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Fun02 TOC:
   # fun02 sec01:
   #   - variable declarations
   # fun02 sec02:
   #   - set up and check if file is in pwd
   # fun02 sec03:
   #   - check if file is in home/Documents directory
   # fun02 sec04:
   #   - check if file is in global install location
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun02 Sec01:
   #   - variable declarations
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   slashSC = .Platform$file.sep;
   pathSplitStr = ':';
   targetStr = "HOME";

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun02 Sec02:
   #   - set up and check if file is in pwd
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(.Platform$OS == "windows")
      pathSplitStr = ';';
   
   if(file.exists(fileStr))
      return(fileStr); # found file location

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun02 Sec03:
   #   - check if file is in home/Documents directory
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(.Platform$OS == "windows"){
      targetStr = "HOMEPATH";
   } # set up home location variable

   tmpStr =
      as.list(
         strsplit(
            Sys.getenv(targetStr),
            pathSplitStr
         )
      )[[1]][1]; # get local file path

   tmpStr =
      paste(
          tmpStr,
          "Documents",
          "freezeTBFiles",
          fileStr,
          sep = slashSC
      ); # add database file

   if(file.exists(tmpStr))
      return(tmpStr); # found file location

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun02 Sec04:
   #   - check if file is in global install location
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(.Platform$OS == "windows"){
      tmpStr =
         as.list(
            strsplit(
               Sys.getenv("PUBLIC"),
               pathSplitStr
            )
         )[[1]][1]; # get local file path

      tmpStr =
         paste(
             tmpStr,
             "Documents",
             "freezeTBFiles",
             fileStr,
             sep = slashSC
         ); # add database file
   }else{
      tmpStr =
         paste(
            "/usr/local/share/freezeTBFiles",
             fileStr,
             sep = slashSC
         );
   } # Else: get global file path for unix
    
   if(file.exists(tmpStr))
      return(tmpStr); # found file location

   return(NULL);
} # findFile_graphAmpDepth


#---------------------------------------------------------
# Part02:
#   - script variables, user input, and prep
#   o part02 sec01:
#     - variable declarations
#   o part02 sec02:
#     - get user input
#   o part02 sec03:
#     - prepare data for graphing
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part02 Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

inputStr = commandArgs(); # Holds the user input
prefixStr = "Hufflepuff";   # name of output file
amrFileStr = findFile_graphAmpDepth("amrDb.tsv");
errBl = FALSE;

dataDF = NULL;
ampsDF = NULL;   # amplicon position data
geneDF = NULL;   # for amplicon gene names
graphObj = NULL; # for holding the graph

# For AMR mappings
amrDF = NULL;
amrPosDF = NULL;
minLenSI = 50;   # amplicon covers at least 20 bp

# Are for pasting the gene names together
numBarsI = 0;
extStr = "tiff";
dataStr = "";

# graph output
readDepthSuffixStr = "-readDepth";
geneCoverageSuffixStr = "-geneMap";
ampCoverageSuffixStr = "-ampMap";

# for building graph legends
legendAry = NULL;       # actual labels in dataframes
legendLabelsAry = NULL; # what to rename legends to

# AMR shifting variables
lenDataSI = 0; # length of dataframe
lenAmrSI = 0;  # length of AMR datafare

siAmr = 0;  # loop iterator for finding hit AMRs
siData = 0; # loop iterator for finding hit AMRs

# AMR shifting arrays (for building dataframes)
amrPosArySI = NULL;
amrStartArySI = NULL;
amrMaxArySI = NULL;
amrMinArySI = NULL;
amrIdAryStr = NULL;
geneAryStr = NULL;

dataStartArySI = NULL;
dataEndArySI = NULL;
dataIdAryStr = NULL;
dataFlagAryStr = NULL;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part02 Sec02:
#   - get user input
#   o part02 sec03 sub01:
#     - get user input
#   o part02 sec03 sub02:
#     - open user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Part02 Sec03 Sub01:
#   - get user input
#*********************************************************

if(length(inputStr) < 6){
  print("nothing input");
  errBl = TRUE
}else{ # Else: have input

iArg = 6;
lenInputI = length(inputStr);

while(iArg <= lenInputI)
{ # Loop: Process the user input
   if(phelp_graphAmpDepth(inputStr[iArg]) == TRUE){
      errBl = TRUE;
      break;
   }

   if(inputStr[iArg] == "-stats"){
      iArg = iArg + 1;

      if(iArg > lenInputI){
        print("-stats (from ampDepth) needs an arugment");
        errBl = TRUE;
        break;
      }

      if(! file.exists(inputStr[iArg]))
         print(
            paste(
               "could not open -stats",
               inputStr[iArg]
            )
         );

      dataStr = inputStr[iArg];
   }else if(inputStr[iArg] == "-prefix"){
      iArg = iArg + 1;

      if(iArg > lenInputI){
         print("-prefix (file prefix) needs an arugment");
         errBl = TRUE;
         break;
      }

      prefixStr = inputStr[iArg];
   }else if(inputStr[iArg] == "-ext"){
      iArg = iArg + 1;

      if(iArg > lenInputI){
         print("-ext (file extension) needs an arugment");
         errBl = TRUE;
         break;
      }

      extStr = inputStr[iArg];
   } else if(inputStr[iArg] == "-who"){
      iArg = iArg + 1;

      if(inputStr[iArg] == '-'){
         amrFileStr = NULL; # disabling input
      }else if(iArg > lenInputI){
         print("-who (who catalog) needs an arugment");
         errBl = TRUE;
         break;
      }

      else if(! file.exists(inputStr[iArg])){
         print(
            paste(
               "could not open -who; not adding AMRs in",
               inputStr[iArg]
            )
         );

         amrFileStr = NULL;
      } else{
         amrFileStr = inputStr[iArg];
      } # check if valid file
   } else if(inputStr[iArg] == "-min-len"){
      iArg = iArg + 1;
      minLenSI = as.numeric(inputStr[iArg]);
   } else{
      phelp_graphAmpDepth("-h");
      print(paste(inputStr[iArg], "is not recongnzied"));
      errBl = TRUE;
      break;
   } # else no arugment was input

   iArg = iArg + 1;
} # Loop: Process the user input
} # Else: have input

if(errBl == TRUE){
   # does nothing (skips to end)
} else{ # have valid input

#*********************************************************
# Part02 Sec03 Sub02:
#   - open user input
#*********************************************************

dataDF =
   setDT(
      read.csv(
         dataStr,
         sep="\t",
         header=TRUE
       )
   );

if(! is.null(amrFileStr)){
   amrDF =
      setDT(
         read.csv(
            amrFileStr,
            sep = "\t",
            header = TRUE
      ));
} # If: I have a who catalog with amr positions ot map

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part02 Sec03:
#  - prepare data for graphing
#  o part02 sec03 sub01:
#    - remove unused rows (offtarget/unmapped)
#  o part02 sec03 sub02:
#    - set up gene Names column
#  o part02 sec03 sub03:
#    - set up legends column
#  o part02 sec03 sub04:
#    - remove short amplicons
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Part02 Sec03 Sub01:
#  - remove unused rows (offtarget/unmapped)
#*********************************************************

dataDF =
   dataDF[
      dataDF$geneId != "x-off-target" &
      dataDF$geneId != "z-unmapped"
      ,
   ]; # Remove items I do not want to graph

#*********************************************************
# Part02 Sec03 Sub02:
#  - set up gene Names column
#*********************************************************

# Removing the _gene name (just need the number)
dataDF$geneId = gsub("gene_*", "", dataDF$geneId);

# This is so the amplicon names will print out in the 
# proper order (ampMap graphs)
dataDF = dataDF[order(dataDF$refGeneStart)];

dataDF[
   ,
   genes:=paste(geneId, collapse="-"),
   by = c("ampGeneStart", "flag")
]; # Merge the gene names
   # I need collapse to merge the names not sep.
   # Collapses deals with vectors, sep deals with separete
   # srings?

# Add the flag to the gene names. This allows me to graph
# the amplicons for each gene separately
dataDF$genesTag = paste(dataDF$genes,dataDF$flag,sep=" ");

numFlagsI =
   length(
      unique(
         paste(dataDF$flag, dataDF$geneId, sep = "")
      )
   );

#*********************************************************
# Part02 Sec03 Sub03:
#  - set up legends column
#*********************************************************

depthDF =
   data.frame(
      seq(from = 1, to = numFlagsI, by = 1),
      rep(20, times = numFlagsI),
      rep(100, times = numFlagsI),
      rep("z20x read depth", times = numFlagsI),
      rep(" 100x read depth", times = numFlagsI)
);

names(depthDF) =
   c("xPos", "y20x", "y100x", "depth20x", "depth100x");

legendAry =
   c(
      " 100x read depth",   # space forces to start
      unique(dataDF$flag),
      "[20x read depth"     # [ is after lowercase
    ); # Array for legend orginzation

legendLabelsAry =
   c(
      "100x read depth",
      unique(dataDF$flag),
      "20x read depth"
   ); # Names to accutally use in the legend

# Make an mean column for each flag. This is so I can
# overlay the means
dataDF$avgCol = paste(dataDF$flag, "mean"); # For legend

#*********************************************************
# Part02 Sec03 Sub04:
#  - remove short amplicons
#*********************************************************

# remove dust
dataDF =
   dataDF[
      dataDF$ampGeneEnd - dataDF$ampGeneStart > minLenSI,
   ];


#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part03:
#   - read depth graph
#   o part03 sec01:
#     - graph data and setup read depth markers
#   o part03 sec02:
#     - formant and save read depth graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part03 Sec01:
#   - graph data and setup read depth markers
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

dataDF$depth20X = "[ 20x read depth";
dataDF$depth100X = " 100x read depth";

# Graph the main data
graphObj = ggplot(dataDF);

# The legend has an issue were it can not handle separate
# fill  and color values. So, every item needs to have
# an fill and color. Otherwise the color scheme is reused

# graph the maximum read depth data
graphObj =
   graphObj +
   geom_col(
       aes(
          x = paste(geneId, flag, sep = "-"),
          y = avgAmpDepth,
          fill = flag,
          col = flag # For legend
   ));

# Add in read depth markers

graphObj =
   graphObj +
   geom_segment(
      data = depthDF,
      aes(
         x = xPos - 0.45,
         xend = xPos + 0.45,
         y = y20x,
         yend = y20x,
         col = depth20x,
         fill = depth20x # Errors out, but for legend
      ),
   );

graphObj =
   graphObj +
   geom_segment(
      data = depthDF,
      aes(
         x = xPos - 0.45,
         xend = xPos + 0.45,
         y = y100x,
         yend = y100x,
         col = depth100x,
         fill = depth100x # Errors out, but for legend
      ),
   );

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part03 Sec02:
#   - formant and save read depth graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# add color to the graph
graphObj =
   graphObj +
   scale_fill_viridis_d(
      name = "Legend",
      direction = 1,
      breaks = legendAry,      # this forsces order
      labels = legendLabelsAry # this corrects the names
   );

graphObj =
   graphObj +
   scale_color_viridis_d(
      name = "Legend",
      direction = 1,
      breaks = legendAry,      # This forces order
      labels = legendLabelsAry # This corrects the names
   );

graphObj = graphObj + xlab("Gene-method");
graphObj = graphObj + ylab("Mean read depth");
graphObj = graphObj + theme_classic();
graphObj = graphObj + theme(legend.position = "top");
graphObj =
   graphObj +
   theme(axis.text.x = element_text(angle = 90));

ggsave(
   paste(
      prefixStr,
      readDepthSuffixStr,
      ".",
      extStr,
      sep = ""
   ),
   device = extStr
); # Save the graph

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part04:
#   - gene coverage graphs
#   o part04 sec01:
#     - merge genes in an amplicon into single amplicons
#   o part04 sec03:
#     - make gene coverage graph
#   o part04 sec04:
#     - format and save the graph (mapping to genes)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part04 Sec01:
#   - merge genes in an amplicon into single amplicons
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

dataDF$ref = "ref";

# 0 is to force it to be first in the color scheme
dataDF$geneEndFlag = " gene end";
dataDF$geneStartFlag = " gene start";

# merge amplicons covering the same gene
dataDF[
   ,
   geneGroup:=paste(geneId, sep = "-", collapse = "-"),
   by = ampGeneStart
]; # merege gene names of amplicons togeether

dataDF$geneGroup = 
   sapply(
      strsplit(dataDF$geneGroup, "-"),
      function(x) paste(unique(x), collapse= "-")
   ); # remove duplicate names from the gene groups

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part04 Sec02:
#   - get AMR data
#   o part04 sec02 sub01:
#     - set up AMR detection variables
#   o part04 sec02 sub02:
#     - find AMRs not in genes hit by amplicons
#   o part04 sec02 sub03:
#     - remove AMRs not in genes hit by amplicons
#   o part04 sec02 sub04:
#     - build AMR dataframe
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Part04 Sec02 Sub01:
#   - set up AMR detection variables
#*********************************************************

if(! is.null(amrDF)){
# If: have AMR coordinates
   # order my data structers by starting positoin
   amrDF = amrDF[order(amrDF$refPosition),];

   dataDF =
      dataDF[
         with(
            dataDF,
            order(
               refGeneEnd,
               refGeneStart
             )
         ),
      ]; # sort dataframe by start and end
   #dataDF = dataDF[order(dataDF$refGeneEnd),];
   #dataDF = dataDF[order(dataDF$refGeneStart),];

   # set up my vectors to store my variables
   amrPosArySI = amrDF$refPosition;
   amrStartArySI = amrDF$geneStart;
   amrEndI = amrDF$geneEnd;
   geneAryStr = 0;
   amrMaxArySI = 0;
   amrMinArySI = 0;
   amrIdAryStr = "";

   dataStartArySI = dataDF$refGeneStart;
   dataEndArySI = dataDF$refGeneEnd;
   dataIdAryStr = dataDF$geneId;
   
   lenDataSI = length(dataDF$geneGroup);
   lenAmrSI = length(amrDF$geneStart);
   
   # My data positions seem to be siz of
   dataStartArySI = dataStartArySI - 6;
   dataEndArySI = dataEndArySI + 6;
   
   siAmr = 1;
   siData = 1;

   #******************************************************
   # Part04 Sec02 Sub02:
   #   - find AMRs not in genes hit by amplicons
   #******************************************************

   while(siAmr <= lenAmrSI){
      if(siData > lenDataSI){
         while(siAmr <= lenAmrSI){
            geneAryStr[siAmr] = 'd';
            amrPosArySI[siAmr] = -1;
            amrMaxArySI[siAmr] = -1;
            amrMinArySI[siAmr] = -1;
            amrStartArySI[siAmr] = -1;
            amrIdAryStr[siAmr] = 'd';
            siAmr = siAmr + 1;
         }
      }  else if(amrEndI[siAmr] > dataEndArySI[siData]){
         siData = siData + 1;
      } else if(
           amrStartArySI[siAmr] < dataStartArySI[siData]
        ){
         geneAryStr[siAmr] = 'd';
         amrPosArySI[siAmr] = -1;
         amrMaxArySI[siAmr] = -1;
         amrMinArySI[siAmr] = -1;
         amrStartArySI[siAmr] = -1;
         amrIdAryStr[siAmr] = 'd';
   
         siAmr = siAmr + 1;
      } else { # Else: I am keeping this value
         geneAryStr[siAmr] = dataStr[siData];
         amrMaxArySI[siAmr] = dataEndArySI[siData];
         amrMinArySI[siAmr] = dataStartArySI[siData];;
         amrIdAryStr[siAmr] = dataIdAryStr[siData];
         siAmr = siAmr + 1;
      } # Else: I am keeping this value
   } # Loop add in gene gropu names
   
   #******************************************************
   # Part04 Sec02 Sub03:
   #   - remove AMRs not in genes hit by amplicons
   #******************************************************

   # remove lazy marked valuesx
   amrIdAryStr = amrIdAryStr[amrIdAryStr != 'd'];
   geneAryStr = geneAryStr[geneAryStr != 'd'];
   amrPosArySI = amrPosArySI[amrPosArySI > -1];
   amrMinArySI = amrMinArySI[amrMinArySI > -1];
   amrMaxArySI = amrMaxArySI[amrMaxArySI > -1];
   amrStartArySI = amrStartArySI[amrStartArySI > -1];

   #******************************************************
   # Part04 Sec02 Sub04:
   #   - build AMR dataframe
   #******************************************************

   # make amr dataframe
   amrPosDF =
      data.frame(
         amrPosArySI,
         amrMinArySI,
         amrMaxArySI,
         amrStartArySI,
         geneAryStr,
         amrIdAryStr,
         rep("AMR-mutation", length(amrPosArySI)),
         rep("ref", length(amrPosArySI))
       );
   
   names(amrPosDF) =
      c(
         "amrStart",
         "minRefStart",
         "maxRefEnd",
         "geneStart",
         "geneGroup",
         "geneId",
         "legendFlag",
         "yCol"
      );
} # If: I have mar coordinates to add

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part04 Sec03:
#   - make gene coverage graph
#   o part04 sec03 sub01:
#     - build my legend
#   o part04 sec03 sub02:
#     - make graph
#   o part04 sec03 sub03:
#     - add in AMRs and gene lengths + facets
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Part04 Sec03 Sub01:
#   - build my legend
#*********************************************************

legendAry =
   c(
      unique(dataDF$flag),
      "ref",     # ~ is last ascii character
      "AMR-mutation"
   );

legendLabelsAry =
   c(
      unique(dataDF$flag),
      "ref",     # ~ is last ascii character
      "AMR mutation"
    ); # Array for legend orginzation

#*********************************************************
# Part04 Sec03 Sub02:
#   - make graph
#*********************************************************

# Graph the main data
graphObj = ggplot(dataDF);

# graph amplicon segments
graphObj =
   graphObj +
   geom_segment(
      aes(
         x = ampGeneStart - refGeneStart,
         xend = ampGeneEnd - refGeneStart,
         y = flag,
         yend = flag,
         col = flag
      ) # aes
   );

#*********************************************************
# Part04 Sec03 Sub03:
#   - add in AMRs and gene lengths + facets
#*********************************************************

# graph the amrs
if(! is.null(amrDF)){
   graphObj =
      graphObj +
      geom_point(
         data = amrPosDF,
         aes(
            x = amrStart - geneStart,
            y = yCol,
            col = legendFlag
         )
      );
} # If: have amr positions to graph
         
# graph gene length
graphObj =
   graphObj +
   geom_segment(
      aes(
         x = 0,
         xend = refGeneEnd - refGeneStart,
         y = ref,
         yend = ref,
         col = ref
      ) # aes
   );

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part04 Sec04:
#  - format and save the graph (mapping to genes)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

graphObj =
   graphObj +
   facet_wrap(vars(geneId), scales = "free_x");


# add color to the graph
graphObj =
   graphObj +
   scale_color_viridis_d(
      name = "Legend",
      direction=-1,
      breaks = legendAry,
      labels = legendLabelsAry
   );

graphObj = graphObj + xlab("Position in gene");
graphObj = graphObj + ylab("Method");
graphObj = graphObj + theme_classic();
graphObj = graphObj + theme(legend.position = "top");
graphObj =
   graphObj +
   theme(axis.text.x = element_text(angle = 90));

ggsave(
   paste(
      prefixStr,
      geneCoverageSuffixStr,
      ".",
      extStr,
      sep = ""
   ),
   device = extStr
); # Save the graph

#---------------------------------------------------------
# Part05:
#   - amplicon coverage graphs
#   o part05 sec01:
#     - find overlaps + merge overlaps into one range
#   o part05 sec02:
#     - find genes in overlaps
#   o part05 sec03:
#     - find AMRs in overlaps
#   o part05 sec04:
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part05 Sec01:
#   - find overlaps + merge overlaps into one range
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# get start and end of first gene in list
dataDF =
   dataDF[
      with(
         dataDF,
         order(
            refStart,
            refEnd
         )
      ),
   ]; # sort data by start and end

dataStartArySI = dataDF$refStart;
dataEndArySI = dataDF$refEnd;

amrStartArySI = dataDF$ampStart;
amrEndArySI = dataDF$ampEnd;
geneAryStr = dataDF$geneId;


dataFlagAryStr = dataDF$flag

siData = 1;

while(siData < length(dataStartArySI) - 1)
{ # Loop: merge overlapping ranges
   if(
      dataStartArySI[siData + 1] <= dataEndArySI[siData]
   ){
      dataStartArySI[siData + 1] = dataStartArySI[siData];

      if(dataEndArySI[siData] >= dataEndArySI[siData +1]){
         dataEndArySI[siData + 1] = dataEndArySI[siData];
      } else{
         dataEndArySI[siData] = dataEndArySI[siData + 1];
      } # find highest start
   }else if(
      dataEndArySI[siData + 1] <= dataEndArySI[siData]
   ){
      dataStartArySI[siData + 1] = dataStartArySI[siData];
      dataEndArySI[siData + 1] = dataEndArySI[siData];
   } # check if have overlap

   siData = siData + 1;
} # Loop: merge overlapping ranges

ampsDF =
   data.frame(
      dataStartArySI,
      dataEndArySI,
      amrStartArySI,
      amrEndArySI,
      dataFlagAryStr
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

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part05 Sec02:
#   - find genes in overlaps
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

dataDF = dataDF[order(refGeneStart), ];
geneAryStr = unique(dataDF$geneId);
amrStartArySI = unique(dataDF$refGeneStart);
amrEndArySI = unique(dataDF$refGeneStart);
   # for storing starts

lenAmrSI = length(geneAryStr);
lenDataSI = length(dataStartArySI);

siAmr = 1;
siData = 1;

while(siAmr <= lenAmrSI)
{ # Loop: assign AMRs to overlaps
   if(amrStartArySI[siAmr] >= dataStartArySI[siData] &&
      amrStartArySI[siAmr] <= dataEndArySI[siData]
   ){
      amrEndArySI[siAmr] = amrStartArySI[siAmr];
      amrStartArySI[siAmr] = dataStartArySI[siData];
      siAmr = siAmr + 1;
   }else{
      if(amrStartArySI[siAmr] > dataEndArySI[siData]){
         siData = siData + 1;
      }else{
         siAmr = siAmr + 1;
      } # check which value am incurmenting
   } # check if AMR is in overalp
} # Loop: assign AMRs to overlaps

# rebuild AMR dataframe
geneDF =
   data.frame(
      amrStartArySI,
      amrEndArySI,
      geneAryStr
   );

names(geneDF) =
   c(
      "start",
      "ampStart",
      "gene"
   );

geneDF$yCol = "label";
geneDF$adjPos = geneDF$ampStart - geneDF$start;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part05 Sec03:
#   - find AMRs in overlaps
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(! is.null(amrDF)) {
   amrPosDF = amrPosDF[order(amrPosDF$amrStart) ,]; 
   amrPosArySI = amrPosDF$amrStart;
   amrStartArySI = amrPosDF$amrStartArySI;
   
   lenAmrSI = length(amrPosArySI);
   lenDataSI = length(dataStartArySI);
   
   siAmr = 1;
   siData = 1;
   
   while(siAmr <= lenAmrSI)
   { # Loop: assign AMRs to overlaps
      if(amrPosArySI[siAmr] >= dataStartArySI[siData] &&
         amrPosArySI[siAmr] <= dataEndArySI[siData]){
         amrStartArySI[siAmr] = dataStartArySI[siData];
         siAmr = siAmr + 1;
      }else{
         if(amrPosArySI[siAmr] > dataEndArySI[siData]){
            siData = siData + 1;
         }else{
            siAmr = siAmr + 1;
         } # check which value am incurmenting
      } # check if AMR is in overalp
   } # Loop: assign AMRs to overlaps
   
   # rebuild AMR dataframe
   amrPosDF =
      data.frame(
         amrStartArySI,
         amrPosArySI
      );
   
   names(amrPosDF) =
      c(
         "start",
         "position"
      );
   
   amrPosDF$legendFlag = "AMR-mutation";
   amrPosDF$flag = "ref";
} # If: have AMRs to graph

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part05 Sec04:
#   - make amplicon coverage graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Part05 Sec04 Sub01:
#   - build legend for graph
#*********************************************************

legendAry =
   c(
      unique(dataDF$flag),
      "Ref",
      "AMR-mutation"
   );

legendLabelsAry =
   c(
      unique(dataDF$flag),
      "Ref",
      "AMR mutation"
    ); # Array for legend orginzation

#*********************************************************
# Part05 Sec04 Sub02:
#   - build graph
#*********************************************************

graphObj = ggplot(ampsDF);

# amplicon lines
graphObj =
   graphObj +
   geom_segment(
      data = ampsDF,
      aes(
         x = ampStart - start,   
         xend = ampEnd - start,
         y = flag,
         yend = flag,
         col = flag
      )
   ); # graph amplicon coverage

# reference line
graphObj =
   graphObj +
   geom_segment(
      aes(
         x = 0,
         xend = end - start,
         y = ref,
         yend = ref,
         col = ref
      )
    ); # graph reference amplicon

# AMR points
if(! is.null(amrDF)){
   graphObj =
      graphObj +
      geom_point(
         data = amrPosDF,
         aes(
            x = position - start,   
            y = flag,
            col = legendFlag
         ),
      ); # graph amplicon coverage
} # If: have AMRs to graph

# gene ids
graphObj =
   graphObj +
   geom_vline(
      data = geneDF,
      aes(
         xintercept = adjPos
      ),
      linetype = 2
   );

graphObj =
   graphObj +
   geom_text(
      data = geneDF,
      aes(
         label = gene,
         x = adjPos,
         y = "ref",
         vjust = -0.5, # negative moves up
         hjust = -0.25  # negative moves right
      ),
      stat 	= "unique",
      size = 2.9
   );



#*********************************************************
# Part05 Sec04 Sub02:
#   - format graph
#*********************************************************

graphObj =
   graphObj +
   facet_wrap(
      vars(start),
      scales = "free_x"
   );

# add color to the graph
graphObj =
   graphObj +
   scale_color_viridis_d(
      name = "Legend",
      direction=-1,
      breaks = legendAry,
      labels = legendLabelsAry
   );

graphObj = graphObj + xlab("Position in amplicon");
graphObj = graphObj + ylab("Method");
graphObj = graphObj + theme_classic();
graphObj = graphObj + theme(legend.position = "top");

graphObj =
   graphObj +
   theme(axis.text.x = element_text(angle = 90));

ggsave(
   paste(
      prefixStr,
      ampCoverageSuffixStr,
      ".",
      extStr,
      sep = ""
   ),
   device = extStr
);
   
} # Else: have valid input

#*=======================================================\
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
#=======================================================*/
