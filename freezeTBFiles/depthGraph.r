#!/usr/bin/env Rscript

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# graphDepth.r SOF: start of file
#   - graph each genes read depth and mark low depth bases
#   o fun01: save_graphDepth
#     - saves a graph (call before plotting graph)
#   o main:
#     - make the depth graph (<prefix>-depth.<ext>)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

saveHeightSI = 700;
saveWidthSI = 700;

#---------------------------------------------------------
# Fun01: save_graphDepth
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
save_graphDepth = function(nameStr, extStr = "png"){
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
} # save_graphDepth 

#---------------------------------------------------------
# Main:
#   - build the depth graph
#   o main sec01:
#     - variable declarations
#   o main sec02:
#     - get user input and read in files
#   o main sec03:
#     - build the graph
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Main Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# user input
prefixStr = "delete-gene-depth.tsv";
minDepthSI = 10;

# hardcoded variables
colPalAry =
   c(
      "#000004FF", # 1: magma black
      "#51127CFF", # 2: magma purple
      "#B63679FF", # 3: magma maroon (for 10x depth)
      "#FDE725FF", # 4: magma yellow for filter bars
      "#F1605DFF"  # 5: magma pink/dark orange; AMR
   ); # color pallete (12 colors)

prefixStr = "delete";
extStr = "png";              # graph file type
depthDF = NULL;
amrDF = NULL ;
amrDbDF = NULL ;
geneAryStr = NULL;

inputStr = commandArgs(); # user input

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Main Sec02:
#   - get user input and read in files
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(length(inputStr) < 7){
   print("nothing input");
   print("Rscript graphDepth.r prefix amr_database.tsv");
   print("or Rscript graphDepth.r prefix amr_database.tsv 10");
   print("  - prefix:");
   print("    o prefix used to run freezeTB");
   print("  - amr_database.tsv:");
   print("    o amr database used with freezeTB");
   print("  - 10:");
   print("    o minimum read depth (10x is default)");
   print("Makes graph named prefix-depth.png from the");
   print("  freezeTB output");
}else if(inputStr[6] == "-h" || inputStr[6] == "--help"){
   print("nothing input");
   print("Rscript graphDepth.r prefix amr_database.tsv");
   print("or Rscript graphDepth.r prefix amr_database.tsv 10");
   print("  - prefix:");
   print("    o prefix used to run freezeTB");
   print("  - amr_database.tsv:");
   print("    o amr database used with freezeTB");
   print("  - 10:");
   print("    o minimum read depth (10x is default)");
   print("Makes graph named prefix-depth.png from the");
   print("  freezeTB output");
}else{
   prefixStr = inputStr[6];
   #prefixStr = "delete"; # for debugging

   if(length(inputStr) > 7){
      minDepthSI = inputStr[8];
   }

   depthDF =
      read.csv(
         sprintf("%s-gene-depth.tsv", prefixStr),
         header = TRUE,
         sep = "\t"
      );
   geneAryStr = unique(depthDF$flag);
   amrDF =
      read.csv(
         sprintf("%s-read-amrs.tsv", prefixStr),
         header = TRUE,
         sep = "\t"
      );

   amrDbDF =
      read.csv(inputStr[7], header = TRUE, sep = "\t");
      #read.csv(
      #   "/home/wander/Documents/freezeTBFiles/amrDb.tsv",
      #   header = TRUE,
      #   sep = "\t"
      #); # for debuging

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Main Sec03:
   #   - build the graph
   #   o main sec03 sub01:
   #     - setup the graph for graphing
   #   o main sec03 sub02:
   #     - graph the read depths
   #   o main sec03 sub03:
   #     - graph the AMRs
   #   o main sec03 sub04:
   #     - graph AMRs missed to low read depth
   #   o main sec03 sub05:
   #     - add the axis labels
   #   o main sec03 sub06:
   #     - add the gene name to teh graph
   #   o main sec03 sub07:
   #     - add the legend
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #******************************************************
   # Main Sec03 Sub01:
   #   - setup the graph for graphing
   #******************************************************

   save_graphDepth(sprintf("%s-depth", prefixStr),extStr);
   plot.new(); # allows me to add a top margin

   if(length(geneAryStr) < 4) {
      rowSI = 1;
      colSI = length(geneAryStr);
   }else{
      rowSI = ceiling(length(geneAryStr) / 4);
      colSI = 4;
   };

   par(
      mfrow = c(rowSI, colSI), # grid size
      mai = c(0.01, 0.01, 0.1, 0.1),#spacing between plots
         # bottom, left, top, right
      mar = c(2.75, 2.25, 0.3, 1), # margin sizes
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
   #   - graph the read depths
   #******************************************************

   for(siGene in 1:length(geneAryStr)) {
   # Loop: plot depths for each gene
      tmpDF=depthDF[depthDF$flag == geneAryStr[siGene],];
      lowDepthAry = which(tmpDF$depth < minDepthSI);
      startSI = min(tmpDF$base);
      endSI = max(tmpDF$base);

      # set my color and shape for read depth
      tmpDF$col = colPalAry[2];
      tmpDF$col[lowDepthAry] = colPalAry[4];
      tmpDF$shape = 16;              # solid circle
      tmpDF$shape[lowDepthAry] = 16;

      maxDepthSI = max(tmpDF$depth);
      plot(
         x = tmpDF$base,
         y = tmpDF$depth,
         type = 'p', # plot points
         ylim = c(0, maxDepthSI + 10),
         xlab = "",
         ylab = "",
         cex = 1,    # point size
         yaxt = 'n', # do not print the y-axis label
         xaxt = 'n', # do not print the x-axis label
         col = tmpDF$col,
            # to many points to do an outline
         pch = tmpDF$shape
      ); # build my plot

      #***************************************************
      # Main Sec03 Sub03:
      #   - graph the detected AMRs
      #***************************************************

      amrPosArySI =
         amrDF[
            amrDF$ref_pos >= startSI &
               amrDF$ref_pos <= endSI
            ,
         ]$ref_pos;
      if(length(amrPosArySI) > 0){
         yPosSI = maxDepthSI;
         points(
            x = amrPosArySI,
            y = rep(yPosSI, times = length(amrPosArySI)),
            pch = 23, # diamond
            col = colPalAry[1],
            bg = colPalAry[3],
            cex = 3
         );
      } # If: have amrs to plot

      #***************************************************
      # Main Sec03 Sub04:
      #   - graph AMRs missed to low read depth
      #***************************************************

      amrPosArySI =
         unique(
            amrDbDF[
               amrDbDF$refPosition >= startSI &
                  amrDbDF$refPosition <= endSI
               ,
            ]$refPosition
         ); # get the reference positions of the AMRs
            #   in my database that might be in my gene

      if(length(amrPosArySI) > 0){
         amrPosArySI = match(amrPosArySI, tmpDF$base);
            # find the index's of the amr positions in
            #   my gene
         amrDepthDF = tmpDF[amrPosArySI,];
         amrPosArySI =
            which(amrDepthDF$depth < minDepthSI);
            # get AMRs in low read depth positions
         amrPosArySI = amrDepthDF[amrPosArySI,]$base;
            # get the positions of my low read depth AMRs
 
         if(length(amrPosArySI) > 0){
            if(maxDepthSI > 10){
               yPosSI = maxDepthSI;
            }else{
               yPosSI = 10;
            }

            points(
               x = amrPosArySI,
               y = rep(yPosSI, times=length(amrPosArySI)),
               pch = 23, # diamond
               col = colPalAry[4],
               bg = colPalAry[4],
               cex = 2
            );
         } # If: have amrs to plot
      } # If: have AMRs

      #***************************************************
      # Main Sec03 Sub05:
      #   - add the axis labels
      #***************************************************

      # add the y-axis labels
      axis(
         2,            # 2 is left y-axis
         las = 1,      # 1 is horizontal text
         at = c(0, floor(maxDepthSI / 2), maxDepthSI),
         labels = c(0, floor(maxDepthSI / 2), maxDepthSI),
         tick = TRUE,  # show axis ticks
         cex.axis = 1.25,
         hadj = 0.8  # adjust position from tick, default
                    #   is 1, 0 is center
      );

      # add the x-axis labels
      quarterSI = floor((endSI - startSI) / 4);
      firstQuarterSI = startSI + quarterSI;
      lastQuarterSI = endSI - quarterSI;

      axis(
         1,            # 1 is bottom x-axis
         las = 1,      # 1 is horizontal text
         at = c(firstQuarterSI, lastQuarterSI, endSI),
         labels = c(firstQuarterSI, lastQuarterSI, endSI),
            # I need three positions to force drawing two
         tick = TRUE,  # show axis ticks
         cex.axis = 1.25,
         hadj = 0.8  # adjust position from tick, default
                     #   is 1, 0 is center
      );

      #***************************************************
      # Main Sec03 Sub06:
      #   - add the gene name to teh graph
      #***************************************************

      widthSI = strwidth(geneAryStr[siGene], cex = 1.25);
      heightSI = strheight(geneAryStr[siGene], cex = 1.25);
      tmpStr = geneAryStr[siGene];

      text(
         x = startSI + (endSI - startSI) / 2,
         y = max(10, maxDepthSI),
         pos = 1,
         #adj = c(0, -0.5),
         offset = -1,
         labels =
            parse(
               text = sprintf("bold(%s)", tmpStr),
               tmpStr
            ), # parse is used to get bold text
         cex = 1.25,
         col = colPalAry[1]
      );
   } # Loop: plot depths for each gene

   #******************************************************
   # Main Sec03 Sub07:
   #   - add the legend
   #******************************************************

   legendTextStr =
      c(
         sprintf(">= %sx read depth", minDepthSI),
         sprintf("< %sx read depth", minDepthSI),
         "detected AMRs",
         "missed AMRs"
      );

   legend(
      "bottomright",
      inset = c(-0.85, -0.8), # adjust x and y position
      legend = legendTextStr,
      horiz = TRUE, # hoizontel lengend
      x.intersp = 1, # distance between symbol and text
      ncol = 1,        # only use 1 column (row)
      bty = 'n',       # no border
      pch = c(16, 16, 18, 18), # shapes used
      col =
         c(
            colPalAry[2],
            colPalAry[3],
            colPalAry[3],
            colPalAry[4]
         ),
      pt.cex = 3,      # makes points easy to see
      cex = 2,         # nice large text
      text.width = strwidth(legendTextStr, cex = 2)
   );
}
