#!/usr/bin/sh

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# getGenePerc.sh SOF: Start Of File 
#   - get percent gene depth for each gene and the drugs
#     that a mutation could provide resistance for
#   o sec01:
#     - variable declarations
#   o sec02:
#     - get and check user input
#   o sec03:
#     - get stats
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

tsvFileStr="";
fastqDirStr="";
fastqFileStr="";
outStr=""; # for tsv file reading
fqStr="";  # for tsv file reading
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";
tblStr="/usr/local/share/freezeTBFiles/coords.tsv";

helpStr="$(basename "$0") -fastq reads.fastq
  - gets gene percent coverage and adds AMR information
Input
  -fastq reads.fastq: fastq file (or fastq.gz) to get
     percent coverage for
  -dir /path/to/fastq/directory: dirctory with fastq
     (or fastq.gz) files to get percent coverage for
     - replaces -fastq
  -tsv file_list.tsv: tsv (tab or space separated file)
     with paths to the fastq files or directories to
     to get percent coverage for
     - format is \"<output_name>\t<path/to/fastq>\"
     - replaces -fastq and -dir
  -ref \"$refStr\": reference sequence to use
     - only replace this if it is not auto found (empty
       quotes)
  -tbl \"$tblStr\": tsv file with gene name and
       coordinates to look for
     - format: column one is gene name, column two is
       reference name, column three is direction (+ or -),
       column four is the first base in the gene, column
       five is the last base in the gene
     - first row is for the header (human only) and is
       always skipped
     - ex: \"atpE\tNC000962\t+\t1461045\t1461290\"
     - ex: \"katG\tNC000962\t-\t2153889\t2156111\"
Output:
   - if -dir or -fastq is used, then outputs table to the
     stdout (command line)
   - else if -tsv is used, to the files listed in the
     first column
Requires:
   - minimap2
   - ampDepth from bioTools (at least version 2025-10-03)
"

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - get and check user input
#   o sec02 sub01:
#     - auto find required input (non-fastq)
#   o sec02 sub02:
#     - get user input
#   o sec02 sub03:
#     - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec02 Sub01:
#   - auto find required input (non-fastq)
#*********************************************************

if [ ! -f "$refStr" ]; then
   refStr="${HOME}/Documents/freezeTBFiles/NC000962.fa";
   if [ ! -f "$refStr" ]; then
      refStr="NC000962.fa";
      if [ ! -f "$refStr" ]; then refStr=""; fi;
   fi;
fi;

if [ ! -f "$tblStr" ]; then
   tblStr="${HOME}/Documents/freezeTBFiles/coords.tsv";
   if [ ! -f "$tblStr" ]; then
      tblStr="coords.tsv";
      if [ ! -f "$tblStr" ]; then tblStr=""; fi;
   fi;
fi;

#*********************************************************
# Sec02 Sub02:
#   - get user input
#*********************************************************

while [ $# -gt 0 ];
do   # Loop: get input
   if [ "$1" = "-fastq" ]; then
      shift;
      fastqFileStr="$1";
      fastqDirStr="";
      tsvFileStr="";
   elif [ "$1" = "-dir" ]; then
      shift;
      fastqFileStr="";
      fastqDirStr="$1";
      tsvFileStr="";
   elif [ "$1" = "-tsv" ]; then
      shift;
      fastqFileStr="";
      fastqDirStr="";
      tsvFileStr="$1";
   elif [ "$1" = "-ref" ]; then
      shift;
      refStr="$1";
   elif [ "$1" = "-tbl" ]; then
      shift;
      tblStr="$1";
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
      printf "%s is not recognized\n" "$1";
      exit;
   fi;

   shift;
done # Loop: get input

#*********************************************************
# Sec02 Sub03:
#   - check user input
#*********************************************************

if [ ! -f "$fastqFileStr" ]; then
   if [ ! -d "$fastqDirStr" ]; then
      if [ ! -f "$tsvFileStr" ]; then
         printf "no file(s) provided by -fastq, -dir, or";
         printf " -tsv\n";
         exit;
      fi;
   fi;
fi;


if [ "$refStr" = "" ]; then
   printf "no reference sequence found, use -ref";
   printf " reference.fasta to provide one\n";
   exit;
fi;

if [ ! -f "$refStr" ]; then
   printf " -ref %s is not a file\n" "$refStr";
   exit;
fi;


if [ "$tblStr" = "" ]; then
   printf "no gene table found, use -tbl";
   printf " coordinatess.tsv to provide one\n";
   exit;
fi;

if [ ! -f "$tblStr" ]; then
   printf " -tbl %s is not a file\n" "$tblStr";
   exit;
fi;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - get stats
#   o sec03 sub01:
#     - store awk script in var so not in separate file
#   o sec03 sub02:
#     - get gene coverage for a file
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - store awk script in variable so not in separate file
#*********************************************************

awkScriptStr='
   BEGIN{
      OFS="\t";
      getline;
      print "gene", "percent_coverage", "drug_1",
            "drug_2", "drug_3", "drug_4";
   };

   {
      $1 = tolower($1);

      if($1 == "atpe")
         print $1, $2, "Bdq", "na", "na", "na";
      else if($1 == "ddn")
         print $1, $2, "Dlm", "Pmd", "na", "na";
      else if($1 == "eis")
         print $1, $2, "Amk", "Kan", "na", "na";
      else if($1 == "emba")
         print $1, $2, "Emb", "na", "na", "na";
      else if($1 == "embb")
         print $1, $2, "Emb", "na", "na", "na";
      else if($1 == "etha")
         print $1, $2, "Eto", "na", "na", "na";
      else if($1 == "fabg1")
         print $1, $2, "Eto", "Inh", "Pmd", "na";
      else if($1 == "fbia")
         print $1, $2, "Cfz", "Dlm", "Pmd", "na";
      else if($1 == "fbib")
         print $1, $2, "Cfz", "Dlm", "Pmd", "na";
      else if($1 == "fbic")
         print $1, $2, "Cfz", "Dlm", "Pmd", "na";
      else if($1 == "fgd1")
         print $1, $2, "Cfz", "Dlm", "na", "na";
      else if($1 == "gidb")
         print $1, $2, "Stm", "na", "na", "na";
      else if($1 == "gyra")
         print $1, $2, "Flq", "Lfx", "Mfx", "na";
      else if($1 == "gyrb")
         print $1, $2, "Flq", "Lfx", "Mfx", "na";
      else if($1 == "inha")
         print $1, $2, "Eto", "Inh", "na", "na";
      else if($1 == "katg")
         print $1, $2, "Inh", "na", "na", "na";
      else if($1 == "pnca")
         print $1, $2, "Pza", "na", "na", "na";
      else if($1 == "rplc")
         print $1, $2, "Lzd", "na", "na", "na";
      else if($1 == "rpob")
         print $1, $2, "Rif", "na", "na", "na";
      else if($1 == "rpsl")
         print $1, $2, "Stm", "na", "na", "na";
      else if($1 == "rrl")
         print $1, $2, "Cap", "Lzd", "na", "na";
      else if($1 == "rrs")
         print $1, $2, "Amk","Cap","Kan","Stm";
      else if($1 == "rv0678")
         print $1, $2, "Bdq", "Cfz", "na", "na";
      else if($1 == "rv2983")
         print $1, $2, "Dlm", "na", "na", "na";
      else if($1 == "tlya")
         print $1, $2, "Cap", "na", "na", "na";
      else
         print $1, $2, "na", "na", "na", "na";
   };
'

#*********************************************************
# Sec03 Sub02:
#   - get gene coverage
#*********************************************************

if [ -f "$fastqFileStr" ]; then
   minimap2 -a "$refStr" "$fastqFileStr" |
     ampDepth -gene-tbl "$tblStr" -p-gene-cover |
     awk "$awkScriptStr";
   exit;
fi;

if [ -d "$fastqDirStr" ]; then
   minimap2 -a "$refStr" "$fastqDirStr/"*.fastq* |
     ampDepth -gene-tbl "$tblStr" -p-gene-cover |
     awk "$awkScriptStr";
   exit;
fi;

while read -r lineStr;
do # Loop: get each input
   outStr="$(printf "%s" "$lineStr" | awk '{print $1;}')";
   fqStr="$(printf "%s" "$lineStr" | awk '{print $2;}')";

   if [ -d "$fqStr" ]; then
      minimap2 -a "$refStr" "$fqStr/"*.fastq* |
        ampDepth -gene-tbl "$tblStr" -p-gene-cover |
        awk "$awkScriptStr" \
        > "$outStr";
   elif [ -f "$fqStr" ]; then
      minimap2 -a "$refStr" "$fqStr" |
        ampDepth -gene-tbl "$tblStr" -p-gene-cover |
        awk "$awkScriptStr" \
        > "$outStr";
   else
      { # let user know issue
         printf "for %s could" "$outStr";
         printf "not open %s\n" "$fqStr";
      } # let user know issue
   fi;
done < "$tsvFileStr"; # Loop: get each input

exit;
