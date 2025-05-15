#!/usr/bin/sh

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# readLenGraph.sh
#   - makes read length graphs for reads, see help message
#     for input and output
#   o sec01:
#     - variable declrations
#   o sec02:
#     - get input
#   o sec03:
#     - get read lengths
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declrations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

prefixStr="out";
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";
filesStr="";
colSI=6;
xLabStr="Read Length";
outPrefixStr="Read-Length";

helpStr="$(basename "$0") /path/to/fastq_pass/barcodexx/*
   - make graph with read depths
Input:
   -prefix out: what to name the ouput graph
   -ref /usr/local/share/freezeTBFiles/NC000962.fa
   o reference genome to map reads to
   /path/to/fastq_pass/barcodexx/*.fastq*
   o fastq files to get lengths for (mapped to -ref)
   -aln: graph reference aligned length instead of read
       length
   -h: print this help message and exit
Output:
   - Makes graph with read lengths saved to
     prefix-read-lengths.png
   - Prints total bases, N50, Mean length, and Median
     length to stdout (terminal)
";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - get input
#   o sec02 sub01:
#     - check if have input, then get input
#   o sec02 sub02:
#     - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec02 Sub01:
#   - check if have input, then get input
#*********************************************************

if [ $# -eq 0 ];
then
   printf "%s\n" "$helpStr";
   exit;
fi

while [ $# -gt 0 ];
do   # Loop: get input
   if [ "$1" = "-prefix" ]; then
        prefixStr="$2";
        shift;
     elif [ "$1" = "-p" ]; then
        prefixStr="$2";
        shift;
     elif [ "$1" = "-ref" ]; then
        refStr="$2";
        shift;
     elif [ "$1" = "-r" ]; then
        refStr="$2";
        shift;
     elif [ "$1" = "-aln" ]; then
        colSI="7";
        xLabStr="Reference Aligned Length";
        outPrefixStr="Aligned-Read-Length";
     elif [ -f "$1" ]; then
        if [ "$filesStr" = "" ]; then
           filesStr="$1";
        else
           filesStr="$filesStr $1";
        fi
     elif [ "$1" = "-h" ]; then
        printf "%s\n" "$helpStr";
        exit;
     elif [ "$1" = "--h" ]; then
        printf "%s\n" "$helpStr";
        exit;
     elif [ "$1" = "help" ]; then
        printf "%s\n" "$helpStr";
        exit;
     elif [ "$1" = "-help" ]; then
        printf "%s\n" "$helpStr";
        exit;
     elif [ "$1" = "--help" ]; then
        printf "%s\n" "$helpStr";
        exit;
     else
       printf "%s is unrecognized\n" "$1";
       exit;
   fi

   shift;
done # Loop: get input

#*********************************************************
# Sec02 Sub02:
#   - check user input
#*********************************************************

if [ "$filesStr" = "" ];
then
   printf "No fastq/fastq.gz files input\n";
   exit
fi

if [ ! -f "$refStr" ];
then
   printf "Could not open -ref %s\n" "$refStr";
   exit
fi

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - get read lengths
#   o sec03 sub01:
#     - prepare data for graping (for R)
#   o sec03 sub02:
#     - have R read in stdin and find number reads and
#       total length
#   o sec03 sub03:
#     - find mean, N50, and median
#   o sec03 sub04:
#     - build the graph
#   o sec03 sub05:
#     - add the x and y axis lables
#   o sec03 sub06:
#     - build the legend
#   o sec03 sub07:
#     - print out the stats
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - prepare data for graping (for R)
#*********************************************************

minimap2 -a --eqx "$refStr" "$filesStr" |
  filtsam \
    -F 4 \
    -F 256 \
    -F 2048 \
    -out-stats \
    -sam - |
  cut -f "$colSI" |
  sort -n |
  uniq -c |
  sed '
        1s/^/count /;      # add lable to first column
        s/^[ \t]*//;       # remove white space at start
        s/[ \t][ \t]*/ /g; # remove multiple tabs/spaces
        s/[ \t]*$//        # remove white space at end
      ' |
  R -e "
      #***************************************************
      # Sec03 Sub02:
      #   - have R read in stdin and find number reads and
      #     total length
      #***************************************************

      lengthsDF =
         read.delim('stdin', sep = ' ', header=TRUE);

      totalSI = 0;
      readCntSI = 0;
      posSI = 1;

      while(posSI <= length(lengthsDF[, 1])){
         readCntSI = readCntSI + lengthsDF[posSI,1];
         tmpSI = lengthsDF[posSI,2] * lengthsDF[posSI,1];
         totalSI = totalSI + tmpSI;
         posSI = posSI + 1;
      } # Loop: get total bases

      #***************************************************
      # Sec03 Sub03:
      #   - find mean, N50, and median
      #***************************************************

      meanSI = round(totalSI / readCntSI, digits = 1);
      middleSI = as.integer(readCntSI / 2);
         # mid piont (median point)

      n50SI = 0;
      cntSI = 0;
      posSI = 1;
      readsSI = 0;
      cntArySI = lengthsDF[,1];
         # using to avoid overwriting dataframe

      while(posSI <= length(lengthsDF[, 1])){
         if(cntArySI[posSI] < 0){
            posSI = posSI + 1;
         } else {
            cntArySI[posSI] = cntArySI[posSI] - 1;
         } # Check if move to next length
           # what I have is the number of reads for each
           #   length, so I need to make sure I have gone
           #   through all the lengths first

         readsSI = readsSI + 1;

         if(cntSI >= totalSI / 2 && n50SI == 0){
            n50SI = lengthsDF[posSI, 2];
         } else if(n50SI == 0){
            cntSI = cntSI + lengthsDF[posSI, 2];
         } # check if foun N50

         if(readsSI == middleSI){
            medianSI = lengthsDF[posSI, 2];

            if(readCntSI %% 1 == 1){
               if(cntArySI[posSI] == 0){
                  medianSI = medianSI +
                             lengthsDF[posSI + 1,2];
                  medianSI = medianSI / 2;
               } # If: need to average median
            } # If: odd number of reads
         } # If: at mid point
      } # Loop: get N50

      #***************************************************
      # Sec03 Sub04:
      #   - build the graph
      #***************************************************

      maxCntSI = max(lengthsDF[,1]);
      maxLenSI = max(lengthsDF[,2]);

      png(filename = '$prefixStr-$outPrefixStr.png');
      plot.new();
      plot.window(
         xlim = c(1, maxLenSI),
         ylim = c(1, maxCntSI)
      ); # set up empty plot so I can add data at end

      # add x and y axis titles
      title(xlab = '$xLabStr', ylab = 'Number Reads');

     abline(v = n50SI,lty = 1,lwd = 2,col = '#F1605D');
     abline(v = meanSI,lty = 2,lwd = 2,col = '#FDE727');
     abline(v = medianSI,lty = 3,lwd = 2,col = '#000004');

      points(
         x = lengthsDF[,2],
         y = lengthsDF[,1],
         col = '#000004'
      ); # add data

      #***************************************************
      # Sec03 Sub05:
      #   - add the x and y axis lables
      #***************************************************

      box(bty = 'l', col = '#000004'); # add axis lines

      minSI = min(lengthsDF[,2]);
      # add axis labels
      xTicks =
         seq(
            from = minSI,
            by = as.integer((maxLenSI - minSI) / 5),
            to = maxLenSI
         ); # get x-axis tick marks

      minSI = min(lengthsDF[,1]);
      yTicks =
         seq(
            from = minSI,
            by = as.integer((maxCntSI - minSI) / 5),
            to = maxCntSI
         ); # get y-axis tick marks

      # add in x and y axis
      axis(
         side = 1,        # 1 is bottom x-axis
         at = xTicks,     # x-axis tick mark locations
         labels = xTicks, # x-axis tick mark values
         col = '#000004'  # color for ticks and axix line
      );
      axis(
         side = 2,        # 2 is left y-axis
         at = yTicks,     # position of each tick
         labels = yTicks, # tick label
         col = '#000004'  # color for ticks and axix line
      );

      #***************************************************
      # Sec03 Sub06:
      #   - build the legend
      #***************************************************

      legTextStr =
         c(
            paste('N50_(', n50SI, ')', sep = ''),
            paste('Mean_(', meanSI, ')', sep = ''),
            paste('Median_(', medianSI, ')', sep = '')
         ); # build legend text

      legend(
         'topleft',
         inset = c(0, -0.08), # get out of plot
         legend = legTextStr,
         col = c('#F1605D', '#FDE727', '#000004'),
         bty = 'n',           # no border
         lty = c(1, 2, 3),    # line type for each entry
         lwd = 2,             # thick lines
         ncol = 1,            # 1 column in legend
         horiz = TRUE,        # make horizontal legend
         xpd = TRUE,          # put outside of plot
         cex = 1,             # otherwise text to large
         text.width = strwidth(legTextStr)
           # helps with spacing
      );

      #***************************************************
      # Sec03 Sub07:
      #   - print out the stats
      #***************************************************

      print(paste('total bases:', totalSI));
      print(paste('N50:', n50SI));
      print(paste('Mean length:', meanSI));
      print(paste('Median length:', medianSI));
    "
