#!/usr/bin/Rscript

library("ggplot2")
library("viridisLite")
library("data.table")
# sudo xbps-install R-cran-data.table

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#  o sec-01:
#    - Variable declerations
#  o sec-02:
#    - Get user input
#  o sec-03:
#    - Set up the gene Names column
#  o sec-04:
#    - Final preperations and graphing
#  o sec-05:
#    - Format and save the graph
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-01:
#  - Variable declerations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

inputStr = commandArgs(); # Holds the user input
nameStr = "Hufflepuff";   # name of output file

dataDF = NULL;
graphObj = NULL; # for holding the graph

# Are for pasting the gene names together
numBarsI = 0;
outputStr = "tiff";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-02:
#  - Get user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(length(inputStr) < 6)
  stop("No tsv file from ampDepth input");

if(inputStr[6] == "-h" ||
   inputStr[7] == "--h" ||
   inputStr[6] == "-help" ||
   inputStr[6] == "--help" ||
   inputStr[6] == "help"){
   print("RScript graphAmpDepth.r file.tsv prefix");
   print("  - Makes a graph of mapped amplicons from");
   print("    a tsv from ampDepth. The output graph is");
   print("    saved to prefix.svg (defualt is out)");
   q("no");
}

if(length(inputStr) > 6)
{ # If: there is a second argument
 nameStr = inputStr[7];

 if(inputStr[7] == "-h" ||
    inputStr[7] == "--h" ||
    inputStr[7] == "-help" ||
    inputStr[7] == "--help" ||
    inputStr[7] == "help"){
    print("RScript graphAmpDepth.r file.tsv prefix");
    print("  - Makes a graph of mapped amplicons from");
    print("    a tsv from ampDepth. The output graph is");
    print("    saved to prefix.svg (defualt is out)");
    q("no");
 }
} # If: there is a second argument

dataDF=setDT(read.csv(inputStr[6],sep="\t",header=TRUE));

# For deugging
#dataDF =
#   setDT(
#      read.csv(
#         "04-ONT-Illumina/04-merged-stats.tsv",
#         sep="\t",
#         header=TRUE
#));

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-03:
#  - Set up the gene Names column
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

dataDF =
   dataDF[
      dataDF$geneId != "x-off-target" &
      dataDF$geneId != "z-unmapped"
      ,
   ]; # Remove items I do not want to graph

# This is so the amplicon names will print out in the 
# proper order (ampMap graphs)
dataDF = dataDF[order(dataDF$refGeneStart)];

dataDF[
   ,
   genes:=paste(geneId, collapse="-"),
   by = c("ampNumber", "flag")
]; # Merge the gene names
   # I need collapse to merge the names not sep.
   # Collapses deals with vectors, sep deals with separete
   # srings?

# Removing the _gene name (just need the number)
dataDF$genes = gsub("gene_*", "", dataDF$genes);

# Add the flat to the gene names. This allows me to graph
# the amplicons for each gene separately
dataDF$genesTag = paste(dataDF$genes,dataDF$flag,sep=" ");

# Make an mean column for each flag. This is so I can
# overlay the means
dataDF$avgCol = paste(dataDF$flag, "mean"); # For legend

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-04:
#  - graphing (read depth)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

dataDF$depth20X = "20x read depth";
dataDF$depth100X = "100x read depth";

# Graph the main data
graphObj = ggplot(dataDF);

# The legend has an issue were it can not handle separate
# fill  and color values. So, every item needs to have
# an fill and color. Otherwise the color scheme is reused

# graph the maximum read depth data
graphObj =
   graphObj +
   geom_col(
       aes(x=genesTag,
       y=maxAmpDepth,
       fill=flag,
       col=flag # For legend
   ));

# Graph the mean read depth data over the maximum
graphObj =
   graphObj +
   geom_col(
      aes(
         x=genesTag,
         y=avgAmpDepth,
         fill=avgCol,
         col = avgCol # For legend
   ));

# Add in read depth markers
graphObj =
   graphObj +
   geom_segment(
      aes(
         x = 1 + ampNumber - 0.45,
         xend = 1 + ampNumber + 0.45,
         y = 20,
         yend = 20,
         col = depth20X,
         fill = depth20X # Errors out, but for legend
      ),
   );

graphObj =
   graphObj +
   geom_segment(
      aes(
         x = 1 + ampNumber - 0.45,
         xend = 1 + ampNumber + 0.45,
         y = 100,
         yend = 100,
         col = depth100X,
         fill = depth100X # errors out, but for legend
      ),
   );

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-05:
#  - Format and save the graph (read depth)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# ADd color to the graph
graphObj =
   graphObj +
   scale_fill_viridis_d(name = "Legend", direction = 1);

graphObj =
   graphObj +
   scale_color_viridis_d(name = "Legend", direction = 1);

graphObj = graphObj + xlab("Gene1-gene2-gene3-...-geneN");
graphObj = graphObj + ylab("Max read depth");
graphObj = graphObj + theme_classic();
graphObj = graphObj + theme(legend.position = "top");
graphObj =
   graphObj +
   theme(axis.text.x = element_text(angle = 90));

ggsave(
   paste(nameStr, "-readDepth.", outputStr, sep = ""),
   device = outputStr
); # Save the graph

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-06:
#  - graphing (Mapping to genes)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

dataDF =
   dataDF[
      dataDF$geneId != "z-unmapped" &
      dataDF$geneId != "x-off-target"
      , 
   ]; # Remove the umapped and off-target columns

# The Z is to force it to be last in the color scheme
dataDF$ref = "z ref";

# The 0 is to force it to be first in the color scheme
dataDF$geneEndFlag = "0 gene end";
dataDF$geneStartFlag = "0 gene start";

# I need this to handle when diffeernt genes are targeted
dataDF[
   ,
   newStart:= min(refStart),
   by = geneId
];

# Merege amplicons covering the same gene
dataDF[
   ,
   geneGroup:=paste(geneId, sep = "-", collapse = "-"),
   by = newStart
]; # merege gene names of amplicons togeether

dataDF$geneGroup = 
   sapply(
      strsplit(dataDF$geneGroup, "-"),
      function(x) paste(unique(x), collapse= "-")
   ); # Remove duplicate names from the gene groups

dataDF[, minRefStart:=min(refStart), by = geneGroup];
dataDF[, maxRefEnd:=max(refEnd), by = geneGroup];


# Graph the main data
graphObj = ggplot(dataDF);

# graph the maximum read depth data
graphObj =
   graphObj +
   geom_segment(
      aes(
         x = ampStart - minRefStart,
         xend = ampEnd - minRefStart,
         y = flag,
         yend = flag,
         col = flag
      ) # aes
   );

# graph the maximum read depth data
graphObj =
   graphObj +
   geom_segment(
      aes(
         x = 0,
         xend = maxRefEnd - minRefStart,
         y = ref,
         yend = ref,
         col = ref
      ) # aes
   );
# Graph the mean read depth data over the maximum

graphObj =
   graphObj +
   geom_vline(
      aes(
         xintercept = refGeneStart - minRefStart,
         col = geneStartFlag
      ) # aes
   );
graphObj =
   graphObj +
   facet_wrap(vars(geneGroup), scales = "free_x");

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-07:
#  - Format and save the graph (mapping to genes)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# ADd color to the graph
graphObj =
   graphObj +
   scale_color_viridis_d(name = "Legend", direction=-1);

graphObj = graphObj + xlab("Gene1-gene2-gene3-...-geneN");
graphObj = graphObj + ylab("Method");
graphObj = graphObj + theme_classic();
graphObj = graphObj + theme(legend.position = "top");
graphObj =
   graphObj +
   theme(axis.text.x = element_text(angle = 90));

ggsave(
   paste(nameStr, "-ampMap.", outputStr, sep = ""),
   device = outputStr
); # Save the graph
