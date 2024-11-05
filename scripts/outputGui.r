require("tcltk") || stop("tcltk is not present");
library("tcltk")

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# TOF01:
#   - output gui
#   o tof01 fun01: getAmpDepthData_freezeTBGui
#     - reads in tsv from ampDepth for graphing
#   o tof01 fun02: mkDepthGraph_freezeTBGui
#     - makes the read depth graph for freezeTB
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
getAmpDepthData_freezeTBGui = function(ampDepthTsvStr){
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
         ampDepthTsvStr,
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
      if(dataDF[siGene,]$flag != dataDF[siGene + 1,]$flag){
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
# TOF01 Fun02: mkDepthGraph_freezeTBGui
#   - makes the read depth graph for freezeTB
# Input:
#   - ampDepthDF:
#     o data frame from getAmpDepthData_freezeTBGui
# Output:
#   - builds read depth graph for freezeTB (to dev)
#---------------------------------------------------------
mkDepthPlot_freezeTBGui = function(ampDepthDF){
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun02 Sec01:
   #   - plot first (un-filtered) read depths
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   idAryStr = unique(dataDF$geneId);

   depthAryUI =
      rep(
         0,
         length(unique(dataDF$geneId))
      );
   
   ampGeneAryStr = dataDF[dataDF$indexUI == 1,]$geneId;
   ampDepthAryUI = dataDF[dataDF$indexUI == 1,]$avgDepth;
   
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
         col = colPalAry[1],
         xlab = "",                 # x-axis title
         ylab = "mean read depth",  # y-axis title
         las = 2,                   # x-axis at 90 degrees
         cex.axis = 1,              # y-axis ticks
         cex.names = 1,             # x-axis names
         ylim = c(0, max(c(dataDF$avgDepth, 100)))
      ); # setup x-axis
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun02 Sec02:
   #   - get and graph second flag read depths
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   if(numFlagsUI >= 1){ 
      idAryStr = unique(dataDF$geneId);
      depthAryUI =
         rep(
            0,
            length(unique(dataDF$geneId))
         );
      
      ampGeneAryStr = dataDF[dataDF$indexUI == 2,]$geneId;
      ampDepthAryUI=dataDF[dataDF$indexUI == 2,]$avgDepth;
      
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
         #height = dataDF[dataDF$indexUI == 2 ,]$avgDepth,
         #names = dataDF[dataDF$indexUI == 2 ,]$geneId,
         height = depthAryUI,
         names = idAryStr,
         col = colPalAry[10],
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
      col = colPalAry[12] # color (yellow)
   ); # 10x read depth line
   
   segments(
      x0 = posDF[,1] - 0.5,
      x1 = posDF[,1] + 0.5,
      y0 = 100, # y-axis start
      y1 = 100, # y-axis end
      lwd = 3, # line width
      col = colPalAry[5]
   ); # 100x read depth line
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # TOF01 Fun02 Sec04:
   #  - add legend
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
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
      legend = legendAryStr, # names/labels for legend
      horiz = TRUE,    # horizontal legend
      ncol = 1,        # 1 column
      bty = 'n',       # no border
      lty = 1,         # add lines to legend
      lwd = 2,         # make lines thicker
      col = legendAryCol, # color for each line
      #cex = 0.75,       # text size
      cex = 1,         # text size
      #text.col = legendAryCol, # text color
      text.width = strwidth(legendAryStr) # spacing
   );
} # mkDepthPlot_freezeTBGui

#---------------------------------------------------------
# TOF01 Fun03: outGUI_freezeTBGui
#   - displays output gui for freezeTB
# Input:
#   - ampDepthStr:
#     o string with path to ampDepth tsv file
#   - amrDBStr:
#     o string with path to tbAmr database (as tsv)
#   - conAmrsStr:
#     o string with path to detected consenses AMRs
#   - readAmrsStr:
#     o string with path to detected read AMRs
#   - conSpolStr:
#     o string with path to detected consensus spoligotype
#   - readSpolStr:
#     o string with path to detected read spoligotype
#   - conMiruStr:
#     o string with path to consensus MIRU VNTR lineage
#   - readMiruStr:
#     o string with path to read MIRU VNTR lineage
# Output:
#   - builds read depth graph for freezeTB (to dev)
#---------------------------------------------------------
outGUI_freezeTBGui = function(
   ampDepthStr,   # tsv of read depths from ampDepth
   amrDBStr,      # path to amr database (tsv)
   conAmrsStr,    # tsv with consensus AMRs
   readAmrsStr,   # tsv of read AMRs
   conSpolStr,    # tsv with consensus spoligotype
   readSpolStr,   # tsv of read spoligotype
   conMiruStr,    # tsv with consensus MIRU VNTR lineage
   readMiruStr    # tsv of read MIRU VNTR lineage
){
   outGuiTK = tktoplevel();
   tkwm.title(outGuiTK, "output");
   outMenuTK = tkframe(outGuiTK, borderwidth = 2);
   dataTK = tkframe(outGuiTK, borderwidth = 2);
   exitTK = tkframe(outGuiTK, borderwidth = 2);

   depthGraphTK = tkframe(dataTK, borderwidth = 2);

   depthDataDF = getAmpDepthData(ampDepthStr);

   if(dataDF)
   { # If: can make graphs
      depthObj =
         tkrplot(
            depthGraphTK,       
            mkDepthPlot_freezeTBBu(depthDataDF)
         );
   } # If: can make graphs

   tkpack(
      dataTK,
      side = "top"
   );

   tkpack(
      depthGraphTK,
      side = "top"
   );
} # outGUI_freezeTBGui

outGUI_freezeTBGui(
   "test-depth.tsv",
   "/usr/local/share/amrDb.tsv",
   "test-con-amr.tsv",
   "test-read-amr.tsv",
   "test-con-spoligo.tsv",
   "test-read-spoligo.tsv",
   "test-con-miru.tsv",
   "test-read-miru.tsv"
);
