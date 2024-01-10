##########################################################
# Name: ampFig.r
# Use:
#   - Makes a bar plot for the gene table tsv from ampMap
#     (ampMapCounts)
# Input:
#   - table.tsv: [Required]
#     o Table to make a bar graph for
#   - prefix: [Hufflepuff-mappings]
#     o Prefix to name everything
# Call: 
#   - Rscript ampFig.r table.tsv prefix
#   - Rscript ampFig.r table.tsv
# Output:
#   - Saves bargraph as svg to prefix.svg
##########################################################

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#  - Graph the number of mapped reads for each gene
#  o sec-01:
#    - Libraries and variable declerations
#  o sec-02:
#    - Get user input and make the graph
#  o sec-03:
#    - Make the graph
#  o sec-04:
#    - Add in the lines marking key thresholds
#  o sec-05:
#    - Add in the data and apply scaling
#  o sec-06:
#    - Formatting and saving the graph
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-01:
#   - Libraries and variable declerations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

library("ggplot2")
library("viridisLite")

inputStr = commandArgs(); # get user input
dataDf = NA;
graphObj = NA;
nameStr = "Hufflepuff-mappings";

# Get the color for the horizontal line. The other two
# colors will be applied by scale_fill_viridis_d()

colorPalAry = viridis(n = 3);

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-02:
#   - Get user input and make the graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(length(inputStr) < 6)
   stop("No file input with Rscript ampFig.r");

dataDf = read.csv(inputStr[6], sep = "\t", header = TRUE);

# Check if the user provided a file name
if(length(inputStr) > 6) nameStr = inputStr[7];

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-03:
#   - Make the graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# These columns are for the legend. ggplot2 is a little
# odd in that it needs and aes fill/col/shape to make
# a legend. The first item used in the legend is the
# legends name.

dataDf$Legend = "Total mappings";
dataDf$Primary = "Primary mappings";

# Lower case comes after Uppercase. I want to use lower
# case to ensure the X-axis is orded alphabetically by
# gene name

dataDf$geneId = tolower(dataDf$geneId);

# I want a label for unmapped. Upercase comes before
# lower, so this should always be the left most column,
# unless a gene id starts with a number.

dataDf$geneId =
    sub("*", "Unmapped", dataDf$geneId, fixed = TRUE);

# Make the graph
graphObj = ggplot(data = dataDf);

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-04:
#   - Add in the lines marking key thresholds
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# 300x is were I start subsampling
graphObj =
   graphObj +
   geom_hline(aes(yintercept=300), col = colorPalAry[2]);

# 100x is a good level for variant calling
graphObj =
   graphObj +
   geom_hline(aes(yintercept=100), col = colorPalAry[2]);

# 30x is low, but possible. Accuracy will be lower
graphObj =
   graphObj +
   geom_hline(aes(yintercept=30), col = colorPalAry[2]);

# 20x is my bare minimum
graphObj =
   graphObj +
   geom_hline(aes(yintercept=20), col = colorPalAry[2]);

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-05:
#   - Add in the data and apply scaling
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

graphObj =
   graphObj +
   geom_col(
      aes(x=geneId, y=TotalMaps, fill = Legend)
   ); # total mappings (Primary + supplemental/secondary)

graphObj =
   graphObj +
   geom_col(
      aes(x = geneId, y = PrimaryMaps, fill = Primary)
   ); # Add in the primary mappings

# Use log scale for large numbers of reads
graphObj =
   graphObj +
   scale_y_log10(
      breaks =
         c(1, 10, 20, 30, 100, 300, 1000,
           10000, 100000, 10000000
          )
   ); # Log transform the data
   # I am adding some extra levels to the scales in for
   # future use. Primers can generate a lot of reads

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-06:
#   - Formatting and saving the graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# Add in color for my mappings
graphObj = graphObj + scale_fill_viridis_d(direction = 1);

# Add in x and y axix labels
graphObj = graphObj + xlab("Gene name");
graphObj = graphObj + ylab("Number of mapped reads");

# Remove the grid lines and grey background
graphObj = graphObj + theme_classic(); # Nice blank theme

# Rotate x-axis labels 90 degrees
graphObj =
    graphObj +
    theme(axis.text.x = element_text(angle = 90));

# Add the legend to the top of the plot
graphObj = graphObj + theme(legend.position = "top");

# Save graph to a file
ggsave(paste(nameStr, ".svg", sep = ""), device = svg);