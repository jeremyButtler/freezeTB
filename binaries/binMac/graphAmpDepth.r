#!/usr/bin/env Rscript

library("ggplot2")
library("viridisLite")
library("data.table")
# sudo xbps-install R-cran-data.table

pGraphAmpDepthHelp = function(inStr){
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

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#  o sec01:
#    - Variable declerations
#  o sec02:
#    - Get user input
#  o sec03:
#    - Set up the gene Names column
#  o sec04:
#    - Final preperations and graphing
#  o sec05:
#    - Format and save the graph
#  o sec06:
#    - graphing (Mapping to genes)
#  o sec07:
#    - Format and save the graph (mapping to genes)
#   o license:
#     - Licensing for this code (public domain / mit)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#  - Variable declerations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

inputStr = commandArgs(); # Holds the user input
prefixStr = "Hufflepuff";   # name of output file
whoCatalogStr = "freezeTBScripts/who-tb-2023-catalog-tbAmr-format.tsv";

dataDF = NULL;
graphObj = NULL; # for holding the graph

# For AMR mappings
amrDF = NULL;
amrPosDF = NULL;
minLenSI = 50;   # amplicon covers at least 20 bp

# Are for pasting the gene names together
numBarsI = 0;
extStr = "tiff";
dataStr = "";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#  - Get user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(length(inputStr) < 6){
  print("nothing input");
}else{ # Else: have input

iArg = 6;
lenInputI = length(inputStr);

while(iArg <= lenInputI)
{ # Loop: Process the user input
   if(pGraphAmpDepthHelp(inputStr[iArg]) == TRUE) q("no");

   if(inputStr[iArg] == "-stats"){
      iArg = iArg + 1;

      if(iArg > lenInputI)
        print("-stats (from ampDepth) needs an arugment");

      dataStr = inputStr[iArg];
   }else if(inputStr[iArg] == "-prefix"){
      iArg = iArg + 1;

      if(iArg > lenInputI)
         print("-prefix (file prefix) needs an arugment");

      prefixStr = inputStr[iArg];
   }else if(inputStr[iArg] == "-ext"){
      iArg = iArg + 1;

      if(iArg > lenInputI)
         print("-ext (file extension) needs an arugment");

      extStr = inputStr[iArg];
   } else if(inputStr[iArg] == "-who"){
      iArg = iArg + 1;

      if(iArg > lenInputI)
         print("-who (who catalog) needs an arugment");

      whoCatalogStr = inputStr[iArg];
   } else if(inputStr[iArg] == "-min-len"){
      iArg = iArg + 1;
      minLenSI = as.numeric(inputStr[iArg]);
   } else{
      pGraphAmpDepthHelp("-h");
      print(paste(inputStr[iArg], "is not recongnzied"));
      q("no");
   } # else no arugment was input

   iArg = iArg + 1;
} # Loop: Process the user input

# For debuging
#dataStr = "del.tsv";
#whoCatalogStr = "../freezeTbFiles/who-2023.tsv";

if(file.exists(dataStr)){
   dataDF=setDT(read.csv(dataStr, sep="\t", header=TRUE));
}else{
   print(paste("Could not open -stats ", dataStr));
   q("no");
}

if(file.exists(whoCatalogStr)){
   amrDF =
      setDT(
         read.csv(
            whoCatalogStr,
            sep = "\t",
            header = TRUE
      ));
} # If: I have a who catalog with amr positions ot map

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#  - prepare data for graphing
#  o sec03 sub01:
#    - remove unused rows (offtarget/unmapped)
#  o sec03 sub02:
#    - set up gene Names column
#  o sec03 sub03:
#    - set up legends column
#  o sec03 sub04:
#    - remove short amplicons
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#  - remove unused rows (offtarget/unmapped)
#*********************************************************

dataDF =
   dataDF[
      dataDF$geneId != "x-off-target" &
      dataDF$geneId != "z-unmapped"
      ,
   ]; # Remove items I do not want to graph

#*********************************************************
# Sec03 Sub02:
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
# Sec03 Sub03:
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
# Sec03 Sub04:
#  - remove short amplicons
#*********************************************************

# remove dust
dataDF =
   dataDF[
      dataDF$ampGeneEnd - dataDF$ampGeneStart > minLenSI,
   ];


#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec04:
#  - graphing (read depth)
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
# Sec05:
#  - format and save graph (read depth)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# ADd color to the graph
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
   paste(prefixStr, "-readDepth.", extStr, sep = ""),
   device = extStr
); # Save the graph

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec06:
#  - graphing (Mapping to genes)
#  o sec06 sub01:
#    - Merge genes in an amplicon into single amplicons
#  o sec06 sub02:
#   - Add in the AMR data if provided
#  o sec06 sub03:
#   - Graph my data
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec06 Sub01:
#   - Merge genes in an amplicon into single amplicons
#*********************************************************

dataDF =
   dataDF[
      dataDF$geneId != "z-unmapped" &
      dataDF$geneId != "x-off-target"
      , 
   ]; # Remove the umapped and off-target columns

# Z is to force it to be last in the color scheme
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

#*********************************************************
# Sec06 Sub02:
#   - add AMR data if provided
#*********************************************************

# eis has a promoter region outside of the gene range

if(! is.null(amrDF)){
# If: have AMR coordinates
   # order my data structers by starting positoin
   amrDF = amrDF[order(amrDF$refPosition),];
   dataDF = dataDF[order(dataDF$refGeneEnd),];
   dataDF = dataDF[order(dataDF$refGeneStart),];

   # set up my vectors to store my variables
   amrPosI = amrDF$refPosition;
   amrStartI = amrDF$geneStart;
   amrEndI = amrDF$geneEnd;
   amrGeneStr = 0;
   amrMaxI = 0;
   amrMinI = 0;
   amrIdStr = "";

   dataStartArySI = dataDF$refGeneStart;
   dataGenEndArySII = dataDF$refGeneEnd;
   dataIdStr = dataDF$geneId;
   
   lenDataI = length(dataDF$geneGroup);
   lenAmrI = length(amrDF$geneStart);
   
   # My data positions seem to be siz of
   dataStartArySI = dataStartArySI - 6;
   dataGenEndArySII = dataGenEndArySII + 6;
   
   iAmr = 1;
   iData = 1;

   # Filter out amrs that are no tin my amplicons
   while(iAmr <= lenAmrI){
      if(iData > lenDataI){
         while(iAmr <= lenAmrI){
            amrGeneStr[iAmr] = 'd';
            amrPosI[iAmr] = -1;
            amrMaxI[iAmr] = -1;
            amrMinI[iAmr] = -1;
            amrStartI[iAmr] = -1;
            amrIdStr[iAmr] = 'd';
            iAmr = iAmr + 1;
         }
      }  else if(amrEndI[iAmr] > dataGenEndArySII[iData]){
         iData = iData + 1;
      } else if(amrStartI[iAmr] < dataStartArySI[iData]){
         amrGeneStr[iAmr] = 'd';
         amrPosI[iAmr] = -1;
         amrMaxI[iAmr] = -1;
         amrMinI[iAmr] = -1;
         amrStartI[iAmr] = -1;
         amrIdStr[iAmr] = 'd';
   
         iAmr = iAmr + 1;
      } else { # Else: I am keeping this value
         amrGeneStr[iAmr] = dataStr[iData];
         amrMaxI[iAmr] = dataGenEndArySII[iData];
         amrMinI[iAmr] = dataStartArySI[iData];;
         amrIdStr[iAmr] = dataIdStr[iData];
         iAmr = iAmr + 1;
      } # Else: I am keeping this value
   } # Loop add in gene gropu names
   
   # remove lazy marked valuesx
   amrIdStr = amrIdStr[amrIdStr != 'd'];
   amrGeneStr = amrGeneStr[amrGeneStr != 'd'];
   amrPosI = amrPosI[amrPosI > -1];
   amrMinI = amrMinI[amrMinI > -1];
   amrMaxI = amrMaxI[amrMaxI > -1];
   amrStartI = amrStartI[amrStartI > -1];

   # make amr dataframe
   amrPosDF =
      data.frame(
         amrPosI,
         amrMinI,
         amrMaxI,
         amrStartI,
         amrGeneStr,
         amrIdStr,
         rep("AMR-mutation", length(amrPosI)),
         rep("ref", length(amrPosI))
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

#*********************************************************
# Sec06 Sub03:
#   - Graph my data
#*********************************************************

legendAry =
   c(
      unique(dataDF$flag),
      "ref",     # ~ is last ascii character
      "AMR-mutation"
    ); # Array for legend orginzation

legendLabelsAry =
   c(
      unique(dataDF$flag),
      "ref",     # ~ is last ascii character
      "AMR mutation"
    ); # Array for legend orginzation

graphObj =
   scale_y_discrete(
      label = function(x) (gsub("ref", "ref", x))
   );

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

graphObj =
   graphObj +
   facet_wrap(vars(geneId), scales = "free_x");

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec07:
#  - Format and save the graph (mapping to genes)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# ADd color to the graph
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
   paste(prefixStr, "-ampMap.", extStr, sep = ""),
   device = extStr
); # Save the graph
} # Else: have input

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
