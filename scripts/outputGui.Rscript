#!/usr/bin/env Rscript

require("tcltk") || stop("tcltk is not present");
library("tcltk")

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# TOF00:
#   - unique to output GUI R script
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if(.Platform$OS != "windows"){
   slashSC = "/";
   def_pathSplitSC = ":";

   glob_outAmrDbStr =
      paste(
         as.list(
            strsplit(
               Sys.getenv("HOME"),
               def_pathSplitSC
             )
         )[[1]][1],
         "/Documents/freezeTBFiles/amrDb.tsv",
         sep = ""
      ); # amr database location

   if( ! file.exists(glob_outAmrDbStr) ){
      glob_outAmrDbStr =
          "/usr/local/share/freezeTBFiles/amrDb.tsv";

      if( ! file.exists(glob_outAmrDbStr) )
         glob_outAmrDbStr = ""; # no database found
   } # If: could not pind AMR database in primary location

} else{
   slashSC = "\\";
   def_pathSplitSC = ";";

   glob_outAmrDbStr =
      paste(
         as.list(
            strsplit(
               Sys.getenv("HOMEPATH"),
               def_pathSplitSC
            )
         )[[1]][1],
         "\\Documents\\freezeTBFiles\\amrDb.tsv",
         sep = ''
      ); # get local file location

   if( ! file.exists(glob_outAmrDbStr) ){
      glob_outAmrDbStr =
         paste(
            as.list(
               strsplit(
                  Sys.getenv("PUBLIC"),
                  def_pathSplitSC
                )
            )[[1]][1],
            "\\Documents\\freezeTBFiles\\amrDb.tsv",
            sep = ""
         ); # alternate location

      if( ! file.exists(glob_outAmrDbStr) )
         glob_outAmrDbStr = ""; # could not find database
   } # If: AMR database file not in primary location
} # find AMR database based on OS

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

outGuiTK = tktoplevel();
tkwm.title(outGuiTK, "output");

outMenuTK = tkframe(outGuiTK);
outDataTK = tkframe(outGuiTK);

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
   tkdestroy(outGuiTK);
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

   tkwm.title(
      outGuiTK,
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

   tkwm.title(
      outGuiTK,
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

   tkwm.title(
      outGuiTK,
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

   tkwm.title(
      outGuiTK,
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

   tkwm.title(
      outGuiTK,
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

   outQuit_butTK =
      tkbutton(
         outMenuTK,
         text = "exit",
         command = outExit_freezeTBGui
      );

   showSelect_butTK =
      tkbutton(
         outMenuTK,
         text = "select results",
         command = showSelect_freezeTBGui
      );

   tkgrid(
      showReport_butTK,
      depth_butTK,
      coverage_butTK,
      showAmr_butTK,
      showSelect_butTK,
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

   showSelect_freezeTBGui();
} # buildSelect_freezeTBGui

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# TOF02:
#   - run output gui
#   o tof02 fun01:
#     -
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

buildSelect_freezeTBGui();

# so Rscript does not quite after loading gui
while( as.numeric(tkwinfo("exists", outGuiTK)) )
   Sys.sleep(0.2);
