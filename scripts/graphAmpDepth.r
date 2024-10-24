#!/usr/bin/env Rscript

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# graphAmpDepth.r SOF: Start Of File
#   - makes graphs for output from ampDepth
#   - you are limited to two flags
#   o part01:
#     - included libraries
#   o fun01: findFIle_graphAmpDepth
#     - checks to see if a freezeTB file exists
#   o fun02: phelp_graphAmpDepth
#     - help message function
#   o fun03: save_graphAmpDepth
#     - saves a graph (call before plotting graph)
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

#---------------------------------------------------------
# Fun01: findFIle_graphAmpDepth
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
   # Fun01 TOC:
   # fun01 sec01:
   #   - variable declarations
   # fun01 sec02:
   #   - set up and check if file is in pwd
   # fun01 sec03:
   #   - check if file is in home/Documents directory
   # fun01 sec04:
   #   - check if file is in global install location
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun01 Sec01:
   #   - variable declarations
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   slashSC = .Platform$file.sep;
   pathSplitStr = ':';
   targetStr = "HOME";

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun01 Sec02:
   #   - set up and check if file is in pwd
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(.Platform$OS == "windows")
      pathSplitStr = ';';
   
   if(file.exists(fileStr))
      return(fileStr); # found file location

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun01 Sec03:
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
   # Fun01 Sec04:
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
# Fun02: phelp_graphAmpDepth
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
phelp_graphAmpDepth = function(){
   amrFileStr = findFile_graphAmpDepth("amrDb.tsv");

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
   print(paste("  -who: [", amrFileStr, "]", sep=""));
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
} # pGraphAmpDephtHelp

#---------------------------------------------------------
# Fun03: save_graphAmpDepth
#   - saves a graph (call before plotting graph)
# Input:
#   - extStr:
#     o extension to save graph as
#       - png, tiff, jpeg, svg, pdf, 
# Output:
#   - Saves:
#     o current graph to file
#   - Note;
#     o make sure you call dev.off() to clear graph
#---------------------------------------------------------
save_graphAmpDepth = function(nameStr, extStr = "png"){
   extStr = tolower(extStr); # ignore case
   heightInt = 1000;
   widthInt = 1000;

   nameStr =
      paste(
         nameStr,
         extStr,
         sep = "."
      );


   if(extStr == "png"){
      png(
         filename = nameStr,
         width = widthInt,
         height = heightInt
      ); # save graph as file

   }else if(extStr == "tiff"){
      tiff(
         filename = nameStr,
         width = widthInt,
         height = heightInt
      ); # save graph as file

   }else if(extStr == "jpeg"){
      jpeg(
         filename = nameStr,
         width = widthInt,
         height = heightInt,
         quality = 100
      ); # save graph as file

   }else if(extStr == "jpg"){
      jpeg(
         filename = nameStr,
         width = widthInt,
         height = heightInt,
         quality = 100
      ); # save graph as file

   }else if(extStr == "svg"){
      widthInt = 7;
      heightInt = 7;

      svg(
         filename = nameStr,
         width = widthInt,
         height = heightInt
      ); # save graph as file

   }else if(extStr == "pdf"){
      widthInt = 7;
      heightInt = 7;

      pdf(
         filename = nameStr,
         width = widthInt,
         height = heightInt
      ); # save graph as file
   }
} # save_graphAmpDepth 

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

colPalAry =
   c(
      "#440154FF", # 1: viridus dark purple
      "#482173FF", # 2:
      "#433E85FF", # 3:
      "#38598CFF", # 4:
      "#2D708EFF", # 5: viridus blue
      "#25858EFF", # 6:
      "#1E9B8AFF", # 7:
      "#20A387FF", # 9:
      "#51C56AFF", # 10: virids light green
      "#85D54AFF", # 11:
      "#C2DF23FF", # 12: reserve for bars
      "#FDE725FF"  # 13: yellow (reserved for bars)
   ); # color pallete (12 colors)

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
extStr = "png";
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
numFlagsUI = 0; # number of flags (catagories)

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
   if(inputStr[iArg] == "-h"){
      phelp_graphAmpDepth();
      errBl = TRUE;
      break;
   }else if(inputStr[iArg] == "--h"){
      phelp_graphAmpDepth();
      errBl = TRUE;
      break;
   }else if(inputStr[iArg] == "help"){
      phelp_graphAmpDepth();
      errBl = TRUE;
      break;
   }else if(inputStr[iArg] == "-help"){
      phelp_graphAmpDepth();
      errBl = TRUE;
      break;
   }else if(inputStr[iArg] == "--help"){
      phelp_graphAmpDepth();
      errBl = TRUE;
      break;
   }else if(inputStr[iArg] == "-stats"){
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

# FOR DEBUGGING
#dataStr = "test.tsv";
#amrFileStr = "/usr/local/share/freezeTBFiles/amrDb.tsv";
#prefixStr = "delete"


dataDF =
   read.csv(
      dataStr,
      sep="\t",
      header=TRUE
    );

if(! is.null(amrFileStr)){
   amrDF =
      read.csv(
         amrFileStr,
         sep = "\t",
         header = TRUE
      );
} # If: I have a who catalog with amr positions ot map

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part02 Sec03:
#  - prepare data for graphing
#  o part02 sec03 sub01:
#    - remove unused rows (offtarget/unmapped)
#  o part02 sec03 sub02:
#    - set up gene Names column
#  o part02 sec03 sub03:
#    - assign colors to flags (+ get number bars)
#  o part02 sec03 sub04:
#    - set up legends column
#  o part02 sec03 sub05:
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

numFlagsUI = length(unique(dataDF$flag));

# Removing the _gene name (just need the number)
dataDF$geneId = gsub("gene_*", "", dataDF$geneId);

# This is so the amplicon names will print out in the 
# proper order (ampMap graphs)
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
   if(dataDF[siGene,]$flag != dataDF[siGene + 1,]$flag){
      if(numRepSI == 0){
         dataDF[siGene,]$repUI = numRepSI;
      } else{
         numRepSI = numRepSI + 1;
         dataDF[siGene,]$repUI = numRepSI;
         numRepSI = 0; # if new set of genes
      }
   }else if(
      dataDF[siGene,]$geneId == dataDF[siGene + 1,]$geneId
   ){ # Else If: same gene
      numRepSI = numRepSI + 1;
      dataDF[siGene,]$repUI = numRepSI;
   }else{
      numRepSI = 0;
      dataDF[siGene,]$repUI = numRepSI;
   } # check if same gene name

   siGene = siGene + 1;
} # Loop: merge gene names

#*********************************************************
# Part02 Sec03 Sub03:
#  - assign colors to flags (+ get number bars)
#*********************************************************

colAryStr =
   c(
      colPalAry[1],
      colPalAry[
         seq(
            from = 0,
            to = 10,
            by = 10 / (numFlagsUI - 1)
         )]
    );

# assign an index to each flag so I can assign color
dataDF$indexUI = as.numeric(factor(dataDF$flag));
dataDF$color = colAryStr[dataDF$indexUI];

#---------------------------------------------------------
# Part03:
#   - build read depth graph
#   o part02 sec01:
#     - build mean read depth graph
#   o part03 sec02:
#    - add legend to mean read depth graph
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part03 Sec01:
#   - build mean read depth graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

save_graphAmpDepth(
   paste(
      prefixStr,
      "-readDepth",
      sep = ""
    ),
    extStr
);

dataDF$boxPlot =
   seq(
      from = 1,
      to = length(dataDF$geneFlag),
      by = 1
   );

barplot(
   height =
      rep(
         0,
         length(unique(dataDF$geneId,))
      ), # make y-axis at 0
   names = unique(dataDF$geneId), # only keep unique names
   col = "WHITE",
   xlab = "",                    # x-axis title
   ylab = "mean read depth",     # y-axis title
   las = 2,                      # x-axis at 90 degrees
   cex.axis = 1,                 # y-axis ticks
   cex.names = 1,                # x-axis names
   ylim = c(0, max(c(dataDF$avgDepth, 100)))
); # setup x-axis


barplot(
   height = dataDF[dataDF$indexUI == 1 ,]$avgDepth, # y
   names = dataDF[dataDF$indexUI == 1 ,]$geneId,
   col = colPalAry[1],
   las = 2,                      # x-axis at 90 degrees
   cex.axis = 1,                 # y-axis ticks
   cex.names = 1,                # x-axis names
   add = TRUE                    # building up graph
); # plot unfiltered data

if(numFlagsUI >= 1){ 
   barplot(
      height = dataDF[dataDF$indexUI == 2 ,]$avgDepth, # y
   names = 
            dataDF[dataDF$indexUI == 2 ,]$geneId,
         #paste(
         #   dataDF[dataDF$indexUI == 2 ,]$geneId,
         #   dataDF[dataDF$indexUI == 2 ,]$repUI
         #),
      col = colPalAry[10],
      las = 2,                      # x-axis at 90 degrees
      cex.axis = 1,                 # y-axis ticks
      cex.names = 1,                # x-axis names
      add = TRUE                    # building up graph
   );
} # If: comparing two values

numBarsUI =
   length(dataDF[dataDF$indexUI == 2,]$avgDepth) *
   1.2; # need 20% offset to account for length

#segments(
#   x0 =
#      seq(
#         from = 0,
#         to = numBarsUI - 1,
#         by = 1
#      ), # add in x-axis starts (/5 to get missing bars)
#   x1 =
#      seq(
#         from = 1,
#         to = numBarsUI,
#         by = 1
#      ), # add in x-axis ends (/5 to get missing bars)
#   y0 = 10, # y-axis start
#   y1 = 10, # y-axis end
#   lwd = 1.5, # line width
#   col = colPalAry[12] # color (yellow)
#); # 10x read depth line
#
#segments(
#   x0 =
#      seq(
#         from = 0,
#         to = numBarsUI - 1,
#         by = 1
#      ), # add in x-axis starts (/5 to get missing bars)
#   x1 =
#      seq(
#         from = 1,
#         to = numBarsUI,
#         by = 1
#      ), # add in x-axis ends (/5 to get missing bars)
#   y0 = 100, # y-axis start
#   y1 = 100, # y-axis end
#   lwd = 1.5, # line width
#   col = colPalAry[5] # color (yellow)
#); # 100x read depth line

abline(
   h = 100,
   col = colPalAry[5],
   lwd = 2
); # 100x read depth

abline(
   h = 10,
   col = colPalAry[12],
   lwd = 2
); # 10x read depth

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part03 Sec02:
#  - add legend to mean read depth graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(numFlagsUI >= 1){ 
   legendAryStr =
      c(
         unique(dataDF$flag)[1:2], 
         "10x read depth",
         "100x read depth"
      );

   legendAryCol =
      c(
         colPalAry[1],
         colPalAry[10],
         colPalAry[12],
         colPalAry[5]
      );
}else{
   legendAryStr =
      c(
         unique(dataDF$flag)[1], 
         "10x read depth",
         "100x read depth"
      );

   legendAryCol =
      c(
         colPalAry[1],
         colPalAry[12],
         colPalAry[5]
      );
} # figure out legend labels

legend(
   "topleft",
   inset = c(0, -0.04), # gets legend outside of plot
   xpd = TRUE,      # allows legend outside of plot
   legend = legendAryStr, # names/labels to add to legend
   horiz = TRUE,    # horizontal legend
   ncol = 1,        # 1 column
   bty = 'n',       # no border
   lty = 1,         # add lines to legend
   lwd = 2,         # make lines thicker
   col = legendAryCol, # color for each line
   #cex = 0.75,       # text size
   cex = 1,         # text size
   #text.col = legendAryCol, # text color
   text.width = strwidth(legendAryStr) # lengend spacing
);

dev.off();

#---------------------------------------------------------
# Part04:
#   - build coverage graph
#   o part02 sec01:
#     - merge amplicons
#   o part03 sec02:
#     - get overlapping genes
#   o part03 sec03:
#     - assign AMRs to amplicons
#   o part03 sec04:
#     - build amplicon graph
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part04 Sec01:
#   - merge amplicons
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

while(siData > 1)
{ # Loop: merge missed end coordinates
   if(dataStartArySI[siData] == dataStartArySI[siData-1])
   { # If: same start position
      if(dataEndArySI[siData] > dataEndArySI[siData - 1]){
         dataEndArySI[siData - 1] = dataEndArySI[siData];
      } else if(dataEndArySI[siData] < dataEndArySI[siData-1]){
         dataEndArySI[siData] = dataEndArySI[siData - 1];
      } # check if need to merge amplicons
   } # If: same start position

   siData = siData - 1;
} # Loop: merge missed end coordinates

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
# Part04 Sec02:
#   - get overlapping genes
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

dataDF = dataDF[order(dataDF$refGeneStart), ];
geneAryStr = unique(dataDF$geneId);
amrStartArySI = unique(dataDF$refGeneStart);
amrEndArySI = unique(dataDF$refGeneStart);
   # for storing starts
geneEndArySI = unique(dataDF$refGeneEndUI);

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
      geneEndArySI[siAmr] = dataEndArySI[siData];
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
      geneEndArySI,
      amrEndArySI,
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

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part04 Sec03:
#   - find AMRs in overlaps
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(! is.null(amrDF)) {
   amrDF = amrDF[order(amrDF$refPosition) ,]; 
   amrPosArySI = amrDF$refPosition;
   amrStartArySI = amrDF$geneStart;
   
   lenAmrSI = length(amrPosArySI);
   lenDataSI = length(dataStartArySI);
   
   siAmr = 1;
   siData = 1;
   
   while(siAmr <= lenAmrSI)
   { # Loop: assign AMRs to overlaps
      if(siData > lenDataSI){
         amrStartArySI[siAmr] = -1;
         amrPosArySI[siAmr] = -1;
         siAmr = siAmr + 1;
      }else if(
         amrStartArySI[siAmr] >= dataStartArySI[siData] &&
         amrStartArySI[siAmr] <= dataEndArySI[siData]
      ){

         amrStartArySI[siAmr] = dataStartArySI[siData];
         siAmr = siAmr + 1;

      }else{
         if(amrPosArySI[siAmr] > dataEndArySI[siData]){
            siData = siData + 1;
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

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part04 Sec04:
#   - build amplicon graph
#   o part04 sec04 sub01:
#     - open saving interface
#   o part04 sec04 sub02:
#     - set up rows and columns
#   o part04 sec04 sub03:
#     - build the graphs in each facet
#   o part04 sec04 sub04:
#     - add x-axis label and legend to graph
#   o part04 sec04 sub05:
#     - close graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Part04 Sec04 Sub01:
#   - open saving interface
#*********************************************************

save_graphAmpDepth(
   paste(
      prefixStr,
      "-coverage",
      sep = ""
    ),
    extStr
);

#*********************************************************
# Part04 Sec04 Sub02:
#   - set up rows and columns
#*********************************************************

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

plot.new(); # start new plot (so can add top margin)

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

#*********************************************************
# Part04 Sec04 Sub03:
#   - build the graphs in each facet
#   o part04 sec04 sub03 cat01:
#     - plot main graph + start loop/setup
#   o part04 sec04 sub03 cat02:
#     - add AMRs to graph
#   o part04 sec04 sub03 cat03:
#     - add gene starts to graph
#   o part04 sec04 sub03 cat04:
#     - add gene names to graph
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Part04 Sec04 Sub03 Cat01:
#   - plot main graph + start loop/setup
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

refPosUI = 3;
flagAryStr = unique(dataDF$flag);

for( uiGraph in 1:length(ampMergeDF[,1]) ){

   startUI = unique(ampMergeDF[uiGraph, 1]);

   plot(
      x =
         # ampMergeDF[uiGraph,1:2]),
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
      col = colPalAry[1]
   ); # build initial plot

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Part04 Sec04 Sub03 Cat02:
   #   - add AMRs to graph
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
     col = colPalAry[12]
   ); # add AMRs to graph

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Part04 Sec04 Sub03 Cat03:
   #   - add gene starts to graph
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
      col = colPalAry[5]
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
      col = colPalAry[5]
   );

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Part04 Sec04 Sub03 Cat04:
   #   - add gene names to graph
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
         col = colPalAry[5]
      );
   } # If: have two flags

   if(refPosUI > 2){
     axis(
        2, # y axis
        at = c(1, 2, 3),
        las = 1,                      # rotate 90 degrees
        labels = c(flagAryStr[2], flagAryStr[1], "ref")
     );
   }else{
     axis(
        2, # place on left side of graph
        at = c(1, 2),
        labels = c(flagAryStr[1], "ref"),
        las = 1,                      # rotate 90 degrees
        tick = TRUE
     );
   } # set axis labels
}

#*********************************************************
# Part04 Sec04 Sub04:
#   - add x-axis label and legend to graph
#*********************************************************

grid(
   nx = 1, # add x-coordinate row
   ny = 1  # add y-coordinate row
); # add extra row to bottom

mtext(
   text = "Reference Position",
   side = 1,  # 1 = bottom, 2 = left, 3 = top 4 = rigth
   line = -0.25, # margin line to draw on
   outer = TRUE, # use outer margins (multi plot is edges)
   cex = 1    # text size
); # add title to amplicon coverage graphs

mtext(
   text = "- AMRs",
   side = 1,  # 1 = bottom, 2 = left, 3 = top 4 = rigth
   adj = 0.82,
   line = -0.25, # margin line to draw on
   outer = TRUE, # use outer margins (multi plot is edges)
   col = colPalAry[12],
   bg = colPalAry[1],   # does not work
   cex = 1    # text size
); # add AMR entry for legend

#20de, 228f, 25a0 (filled square), 26f6 (crosshair square)

mtext(
   text = "-- Gene Start",
   side = 1,  # 1 = bottom, 2 = left, 3 = top 4 = rigth
   adj = 0.99,
   line = -0.25, # margin line to draw on
   outer = TRUE, # use outer margins (multi plot is edges)
   col = colPalAry[5],
   cex = 1    # text size
); # add AMR entry for legend

#*********************************************************
# Part04 Sec04 Sub05:
#   - close graph
#*********************************************************

#dev.off() # since last graph, let R close graphing
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
