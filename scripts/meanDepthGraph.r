#!/usr/bin/env Rscript

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# meanDepthGrap.r SOF: Start Of File
#   - makes the mean read depth graph for freezeTB
#   o fun01: save_meanDepthGraph
#     - saves a graph (call before plotting graph)
#   o main:
#     - make the mean read depth graph
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# Fun01: save_meanDepthGraph
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
save_meanDepthGraph = function(nameStr, extStr = "png"){
   extStr = tolower(extStr); # ignore case
   heightInt = 700;
   widthInt = 700;
   nameStr = paste(nameStr, extStr, sep = ".");

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
} # save_meanDepthGraph 

#---------------------------------------------------------
# Main:
#   - make the mean read depth graph
#   o main sec01:
#     - variable declartions and get input
#   o main sec02:
#     - get and check user input
#   o main sec03:
#     - build the mean read depth graph
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Main Sec01:
#   - variable declartions and get input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

colPalAry =
   c(
      "#000004FF", # 1: magma black
      "#51127CFF", # 2: magma purple
      "#B63679FF", # 3: magma maroon (for 10x depth)
      "#FDE725FF", # 4: magma yellow for filter bars
      "#F1605DFF"  # 5: magma pink/dark orange; AMR
   ); # color pallete (12 colors)

inputStr = commandArgs(); # get user command line input
extStr = "png"; # graph file type
prefixStr = "del-";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Main Sec02:
#   - get and check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(length(inputStr) < 6){
   print("The prefix used with freezeTB is needed");
}else if(inputStr[6] == "-h" || inputStr[6] == "--help"){
   print("Rscript meanDepthGraph prefix");
   print("  - makes mean read depth graph for freezeTB");
   print(" - prefix:");
   print("   o prefix used with freezeTB");
   print("  - Optional: at the end you can use 'tiff'");
   print("    to save the output as a tiff file");
}else{
   if(length(inputStr) > 6)
      extStr = inputStr[7];
   prefixStr = inputStr[6];
   dataDF =
      read.csv(
         paste(prefixStr, "-coverage.tsv", sep = ""),
         sep="\t",
         header=TRUE
      );

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Main Sec03:
   #   - build the mean read depth graph
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   save_meanDepthGraph(
      paste(prefixStr, "-mean-depth", sep=""),
      extStr
   ); # save graph as users extension

   par(mar = c(6.5, 4, 2, 2) + 0.1);
      # adjust the margins of the plot

   posDF = # posDF gets bar center coordinates
      barplot(
         height = dataDF$mean_target_depth, # read depth
         names = dataDF$gene, # x axis names
         col = colPalAry[4], # color to use for columns
         xlab = "",          # remove x-axis title
         ylab = "mean read depth", # y-axis title
         las = 2,            # x-axis labes 90 degrees
         cex.axis = 1,       # add y-axis ticks
         cex.names = 1,      # add x-axis names
         ylim = c(0, max(dataDF$mean_target_depth, 100))
           # y-axis limits
      );

   segments(
      x0 = posDF[,1] - 0.5, # start of column
      x1 = posDF[,1] + 0.5, # end of column
      y0 = 10,
      y1 = 10,
      lwd = 3, # nice thick line
      col = colPalAry[3], # color for 10x depth
      cex = 2
   ); # add 10x read depth line

   segments(
      x0 = posDF[,1] - 0.5, # start of column
      x1 = posDF[,1] + 0.5, # end of column
      y0 = 100,
      y1 = 100,
      lwd = 3, # nice thick line
      col = colPalAry[2], # color for 100x depth
      cex = 2
   ); # add 100x read depth line

   legend(
      "topleft",    # put the legend in top left connor
      inset = c(0, -0.04), # puts legend in top margin
      xpd = TRUE,   # allows legend outside of the plot
      legend = c("10x read depth", "100x read depth"),
         # legend entries
      horiz = TRUE, # make legend horizontal
      ncol = 1,     # have legend use only one column
      bty = 'n',    # remove the border
      lty = 1,      # add the lines to the legends
      lwd = 2,      # make the lines thicker
      col = c(colPalAry[3], colPalAry[2]),
         # legend colors to use
      cex = 1,      # use larger text
      text.width =
        strwidth(c("10x read depth", "100x read depth"))
        # this positions the legends nicely
   );
} # Else: have input
