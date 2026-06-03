#!/usr/bin/env Rscript

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# coverGraph.r SOF: Start Of File
#   - makes the coverage graph for freezeTB
#   o fun01: save_coverGraph
#     - saves a graph (call before plotting graph)
#   o main:
#     - make the coverage graph
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

saveHeightSI = 700;
saveWidthSI = 700;

#---------------------------------------------------------
# Fun01: save_coverGraph
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
save_coverGraph = function(nameStr, extStr = "png"){
   extStr = tolower(extStr); # ignore case
   nameStr = paste(nameStr, extStr, sep = ".");

   if(extStr == "png"){
      png(
         filename = nameStr,
         width = saveWidthSI,
         height = saveHeightSI
      ); # save graph as file

   }else if(extStr == "tiff"){
      tiff(
         filename = nameStr,
         width = saveWidthSI,
         height = saveHeightSI
      ); # save graph as file

   }else if(extStr == "jpeg"){
      jpeg(
         filename = nameStr,
         width = saveWidthSI,
         height = saveHeightSI,
         quality = 100
      ); # save graph as file

   }else if(extStr == "jpg"){
      jpeg(
         filename = nameStr,
         width = saveWidthSI,
         height = saveHeightSI,
         quality = 100
      ); # save graph as file

   }else if(extStr == "svg"){
      saveWidthSI = 7;
      saveHeightSI = 7;

      svg(
         filename = nameStr,
         width = saveWidthSI,
         height = saveHeightSI
      ); # save graph as file

   }else if(extStr == "pdf"){
      saveWidthSI = 7;
      saveHeightSI = 7;

      pdf(
         filename = nameStr,
         width = saveWidthSI,
         height = saveHeightSI
      ); # save graph as file
   }
} # save_coverGraph 

#---------------------------------------------------------
# Main:
#   - build the coverage graph
#   o main sec01:
#     - variable declarations
#   o main sec02:
#     - get user input
#   o main sec03:
#     - build coverage graph
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Main Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

colPalAry =
   c(
      "#000004FF", # 1: magma black
      "#51127CFF", # 2: magma purple
      "#B63679FF", # 3: magma maroon (for 10x depth)
      "#FDE725FF", # 4: magma yellow for filter bars
      "#F1605DFF"  # 5: magma pink/dark orange; AMR
   ); # color pallete (12 colors)

extStr = "png";              # graph file type
ftbPrefixStr = "del";

amrDbDF = NULL;     # has amr data
geneCoverDF = NULL; # has gene coverage data
coordsDF = NULL;    # has coordinates file
hitsDF = NULL;      # amrs found

inputStr = commandArgs(); # user input arguments

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Main Sec02:
#   - get user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(length(inputStr) < 6){
   print("nothing input; use -h to get the help message");
}else if(inputStr[6] == "-h" || inputStr[6] == "--help"){
   outStr = "Rscript coverGraph.r ftb_prefix coords.tsv";
   outStr = paste(outStr, "catalog.tsv", sep = " ");

   print(outStr);
   print("  - builds the coverage graph for freezeTB");
   print("Input:");
   print("  o ftb_prefix: prefix used with freezeTB");
   print("  o coords.tsv: tsv file with the coordinates");
   print("    tsv file used to run freezeTB");
   print("  o catalog.tsv: AMR catalog freezeTB used");
   print("  o Optional: at end use 'tiff' to make a");
   print("    tiff file instead of a png file");
}else if(length(inputStr) < 7){
   print("missing the coordinates file, and AMR catalog");
}else if(length(inputStr) < 8){
   print("missing the AMR catalog");
}else{
   if(length(inputStr) > 8){
      extStr = inputStr[9];
   } # If: user provided an output extension

   ftbPrefixStr = inputStr[6];

   geneCoverDF =
     read.csv(
        paste(ftbPrefixStr,"-gene-coverage.tsv",sep=""),
        sep = "\t",
        header = TRUE
     );
   geneCoverDF =
      geneCoverDF[order(tolower(geneCoverDF$gene)),];
      # make sure this dataframe is sorted by gene name
      #   I am using tolower to ignore case

   hitsDF =
     read.csv(
        paste(ftbPrefixStr, "-read-amrs.tsv", sep=""),
        sep = "\t",
        header = TRUE
     );

   coordsDF =
     read.csv(inputStr[7], sep = "\t", header = TRUE);
   coordsDF = coordsDF[order(tolower(coordsDF[,1])),];
      # make sure this dataframe is sorted by gene name
      #   I am using tolower to ignore case

   amrDbDF =
     read.csv(inputStr[8], sep = "\t", header = TRUE);

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Main Sec03:
   #   - build coverage graph
   #   o main sec03 sub01:
   #     - setup the graph
   #   o main sec03 sub02:
   #    - start loop and plot expected length
   #   o main sec03 sub03:
   #    - graph all possible amrs can be found
   #   o main sec03 sub04:
   #    - graph covered regions and the detected amrs
   #   o main sec03 sub05:
   #    - add the gene name to the graph
   #   o main sec03 sub06:
   #    - add mean read depth to the upper right coner
   #   o main sec03 sub07:
   #     - add potential drugs to each graph
   #   o main sec03 sub08:
   #     - add y-axis labels
   #   o main sec03 sub09:
   #     - add the legend
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # Main Sec03 Sub01:
   #   - setup the graph
   #******************************************************

   save_coverGraph(
      paste(ftbPrefixStr, "-coverage", sep = ""),
      extStr
   );

   plot.new(); # allows me to add a top margin

   if(length(geneCoverDF->gene) < 4) {
      rowSI = 1;
      colSI = length(geneCoverDF$gene);
   }else{
      rowSI = ceiling(length(geneCoverDF$gene) / 4);
      colSI = 4;
   };

   par(
      mfrow = c(rowSI, colSI), # grid size
      mai = c(0.01, 0.01, 0.1, 0.1),#spacing between plots
         # bottom, left, top, right
      mar = c(2.75, 2.25, 0, 1), # margin sizes
         # bottom, left, top, right
      mgp = c(0, 1, 0),       # individual plot margins
        # 0 (plot title), 1 (axis), 0 (axis)
      oma = c(2, 1, 1, 0),
           # margins for the entire graph
      yaxp = c(1, 3, 1),      # y axis tick settings
      xpd = NA                # plot anywere on graph
   );

   #******************************************************
   # Main Sec03 Sub02:
   #   - start loop and plot expected length
   #******************************************************

   for(siGraph in 1:length(geneCoverDF$gene)){
      geneStartSI = coordsDF[siGraph, 4];
      geneEndSI = coordsDF[siGraph, 5];

      if(geneStartSI > geneEndSI){
         geneStartSI = coordsDF[siGraph, 5];
         geneEndSI = coordsDF[siGraph, 4];
      } # If: need to flip coordinates

      plot(
         x = c(geneStartSI, geneEndSI),
         y = c(1.8, 1.8),
         type = 'l',         # draw a solid line
         ylim = c(0.5, 2.6), # y axis limits
         xlab = "",
         ylab = "",
         lwd = 2,            # line width
         yaxt = 'n',         # no y-axis label
         xaxt = 'n',         # no y-axis label
         col = colPalAry[1]
      ); # plot the expected length
 
      #***************************************************
      # Main Sec03 Sub03:
      #   - graph all possible amrs can be found
      #***************************************************

      # graph the amrs that can be found
      amrPosArySI = 
        unique(
          amrDbDF[
           amrDbDF$refPosition >= geneStartSI &
           amrDbDF$refPosition <= geneEndSI
           ,
          ]$refPosition
        );  # get postions of all amrs that could be found
 
      if(length(amrPosArySI) >= 1){
         points(
            x = amrPosArySI,
            y = rep(1.8, times = length(amrPosArySI)),
            pch = 16,           # filled circle
            col = colPalAry[4], # color of database amr
            cex = 2
         );
      } # If: have amrs to detect
 
      #***************************************************
      # Main Sec03 Sub04:
      #   - graph covered regions and the detected amrs
      #***************************************************

      # graph covered regions
      geneCoverArySI =
         as.integer( # convert from dataframe to an array
            geneCoverDF[
               siGraph,
               6:length(geneCoverDF[siGraph,])
               # 6 is the first amplicon start region
            ]
         );
      geneCoverArySI = # remove NA's
         geneCoverArySI[! is.na(geneCoverArySI)];
 
      if(length(geneCoverArySI) >= 1){
         oddElmSI =
            seq(from=1, to=length(geneCoverArySI), by=2);
         segments(
            x0 = geneCoverArySI[oddElmSI],
            x1 = geneCoverArySI[oddElmSI + 1],
            y0 = rep(1.3, length(oddElmSI)),
            y1 = rep(1.3, length(oddElmSI)),
            lty = 1, # line type 2 is a solid line
            lwd = 2, # line thickness
            col = colPalAry[2]
         );
 
         foundAmrsArySI =
            unique(
               hitsDF[
                 hitsDF$ref_pos >= geneStartSI &
                 hitsDF$ref_pos <= geneEndSI
                 ,
               ]$ref_pos
            );
         if(length(foundAmrsArySI) >= 1){
            points(
               x = foundAmrsArySI,
               y=rep(1.3, times = length(foundAmrsArySI)),
               pch = 16,           # filled circle
               col = colPalAry[5], # color for drug resist
               cex = 2
            );
         } # If: have amr hits
      } # If: have coverage

      #***************************************************
      # Main Sec03 Sub05:
      #   - add the gene name to the graph
      #***************************************************
 
      text(
         x = geneStartSI,
         y = 2 + 0.3,
         labels = geneCoverDF$gene[siGraph],
         adj = c(0, 0.5),
         cex = 1.25
      );
 
      #***************************************************
      # Main Sec03 Sub06:
      #   - add mean read depth to the upper right coner
      #***************************************************

      textStr =
         paste(
            "md=",
            floor(geneCoverDF$mean_depth[siGraph]),
            sep = ""
         ); # build the mean read depth text

      text(
         x = geneEndSI - strwidth(textStr, cex = 1.25),
         y = 2 + 0.3,
         labels = textStr,
         adj = c(0, 0.5),
         cex = 1.25
      );
 
      #***************************************************
      # Main Sec03 Sub07:
      #   - add potential drugs to each graph
      #***************************************************

      # find the end of the drug column
      endColSI = 
         grep(
            "\\*",
            coordsDF[
                   siGraph,
                   6:length(coordsDF[siGraph,])
                ] # search collumns 6 to the end
         )[1] + 4 # + 4 to account for removed
                  #   columns, with a -1 for being on
                  #   the '*' column
      drugAryStr =
        coordsDF[, 6:endColSI][siGraph,]; # get the drugs
 
      drugAryStr = drugAryStr[! is.na(drugAryStr)];
      drugAryStr = drugAryStr[drugAryStr != "na"];
      drugAryStr = drugAryStr[drugAryStr != "Na"];
      drugAryStr = drugAryStr[drugAryStr != "NA"];
      drugAryStr = paste(drugAryStr, collapse = "  ");
 
      if(length(drugAryStr) >= 1){
         midSI = geneEndSI - geneStartSI;
         widthSI = strwidth(drugAryStr, cex = 1.25);
         midSI = midSI / 2;
         midSI = midSI - (widthSI / 2);
         midSI = midSI + geneStartSI;
         text(
            x = midSI,
            y = 0.75,
            labels = drugAryStr,
            cex = 1.25, # text size
            adj = c(0, 0.5)
         );
      } # If: have drugs for this gene

      #***************************************************
      # Main Sec03 Sub08:
      #   - add y-axis labels
      #***************************************************
 
      axis(
         2,            # place on left side of graph
         at = c(0.75, 1.3, 1.8),
         labels = c("drug", "tb", "ref"),
         las = 1,      # rotate text 90 degrees
         tick = TRUE,  # show axis ticks
         cex.axis = 1.4,
         hadj = 0.8 # adjust position from tick, default
                    #   is 1, 0 is center
      ); # y-axis labels

      if(siGraph %% 2){
         adjBl = -0.2;
      }else{
         adjBl = 0.8;
      }

      xLabArySI =
         c(
            geneStartSI,
            floor(
               geneStartSI + ((geneEndSI - geneStartSI)/2)
            ),
            geneEndSI
         ); # start, mid, and end coordinates
      axis(
         1,
         at = xLabArySI,
         labels = xLabArySI,
         tick = TRUE,
         cex.axis = 1
         #padj = adjBl
         #hadj = -1 * (adjBl / 10)
      ); # x-axis labels

      #text(
      #   x = xLabArySI,
      #   y = 0,
      #   labels = xLabArySI,
      #   xpd = TRUE,
      #   srt = -10,
      #   cex = 1.25
      #); # was to draw x-labels at an angle, but it
      #     did not work to well
   } # Loop: plot each graph

   #******************************************************
   # Main Sec03 Sub09:
   #   - add the legend
   #******************************************************

   fontSizeSI = 1;

   legend(
      "bottomright",
      inset = c(-0.3, -0.85),
         # adjust x position by -0.3 and the y position
         #   by -0.85
         # This setting is good for the output png, but
         #   may not look great when not saving a png
      legend =
         c(
            " catalog AMRs",
            "detected AMRs",
            "md=mean read depth"
         ), # tesx to add to the legend
       horiz = TRUE, # make the legend horizontal
       x.intersp = 0.5, # distance between symbol and text
       ncol = 1,  # number of columns for the legend
       bty = 'n', # do not use a border
       pch = c(16, 16, NA), # pionts for each label
       col = c(colPalAry[4], colPalAry[5], NA),
           # color for each point
       pt.cex = 3, # size of each point
       cex = 2, # size of the text
       text.width =
         strwidth(
            c(
               " catalog AMRs",
               "detected AMRs",
               "md=mean read depth"
            ),
            cex = 2 # size of the text
         ) # this makes it so the legend spacing is nice
   ); # legend attempt
} # Else: user input something
