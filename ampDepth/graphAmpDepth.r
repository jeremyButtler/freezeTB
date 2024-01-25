#!/usr/bin/Rscript

library("ggplot2")
library("viridisLite")

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
targIndexI = 0;
maxGenesI = 0;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-02:
#  - Get user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(length(inputStr) < 6)
  stop("No tsv file from ampDepth input");

if(inputStr[6] == "-h" ||
   inputStr[6] == "--h" ||
   inputStr[6] == "-help" ||
   inputStr[6] == "--help" ||
   inputStr[6] == "help"){
   print("RScript graphAmpDepth.r file.tsv prefix");
   print("  - Makes a graph of mapped amplicons from");
   print("    a tsv from ampDepth. The output graph is");
   print("    saved to prefix.svg (defualt is out)");
   q("no");
}

if(length(inputStr) > 7) nameStr = inputStr[7];

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
 
dataDF = read.csv(inputStr[6], sep = "\t", header = TRUE);

if(length(inputStr) > 7) nameStr = inputStr[7];

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-03:
#  - Set up the gene Names column
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# Find the maximum number of genes in an amplicon
targIndexI = seq(from=14, to=length(dataDF[1,]), by=6);
maxGenesI = length(targIndexI);

# Paste the gene names in an amplcon together
# This trick for merging columns is from
#   https://stackoverflow.com/questions/18115550/combine-two-or-more-columns-in-a-dataframe-into-a-new-column-with-a-new-name            

paste_noNA =
   function(x, sep="-"){
      gsub(
         ", ",
         sep,
         toString(x[!is.na(x) & x != "" & x != "NA"])
      )
} # paste_noNA fuction

dataDF$genes =
  apply(
      dataDF[,seq(from=14, to=maxGenesI * 6 + 6, by=6)],
      1,
      paste_noNA,
      sep="-"
   );

# Add the flat to the gene names. This allows me to graph
# the amplicons for each gene separately
dataDF$genes = paste(dataDF$genes, dataDF$flag, sep=" ");

# Removing the _gene name (just need the number)
dataDF$genes = gsub("gene_", "", dataDF$genes);

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-04:
#  - Final preperations and graphing
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# Make an mean column for each flag. This is so I can
# overlay the means
dataDF$avgCol = paste(dataDF$flag, "mean"); # For legend

# Graph the main data
graphObj = ggplot(dataDF);

# graph the maximum read depth data
graphObj =
   graphObj +
   geom_col(aes(x=genes, y=maxAmpDepth, fill=flag));

# Graph the mean read depth data over the maximum
graphObj =
   graphObj +
   geom_col(aes(x=genes, y=avgAmpDepth, fill=avgCol));

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-05:
#  - Format and save the graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# ADd color to the graph
graphObj = graphObj + scale_fill_viridis_d(direction = 1);

graphObj = graphObj + xlab("Gene1-gene2-gene3-...-geneN");
graphObj = graphObj + ylab("Max read depth");
graphObj = graphObj + theme_classic();
graphObj = graphObj + theme(legend.position = "top");
graphObj =
   graphObj +
   theme(axis.text.x = element_text(angle = 90));

ggsave(paste(nameStr, ".svg", sep = ""), device = svg);
