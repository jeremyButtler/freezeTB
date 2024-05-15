#!/usr/bin/env bash

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o sec-01:
#     - Variable decleratoins
#   o sec-02:
#     - check user input
#   o sec-03:
#     - Run freezeTB and get stats
#   o sec-04:
#     - Run TBProfiler and get stats
#   o sec-05:
#     - compare consensuses
#   o sec-06:
#     - compare to an reference
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

##########################################################
# Name: TBOneBench
#   - Does one benchmarking run for TBProfiler and
#     freezeTB
# Input:
#   -reads: reads (fastq) to do benchmarking with
#   -ref: reference sequence
#   -stats: File to output AMR and lineage stats to
#   -con-stats: File to output consensus stats to
#   -prefix: prefix to name output
#   -head: print header
# Output:
##########################################################

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-01:
#   - Variable decleratoins
#   o sec-01 sub-01:
#       - user input variables
#   o sec-01 sub-02:
#       - Non user input variables
#   o sec-01 sub-03:
#       - Help message
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec-01 Sub-01:
#   - user input variables
#*********************************************************

readsStr="";
statsFileStr="out-stats.tsv";
conStatsFileStr="out-con-stats.tsv";
prefixStr="out";
pHeadBl=0;
refStr="";

#*********************************************************
# Sec-01 Sub-02:
#   - Non user input variables
#*********************************************************

timeRetStr=""; # GNU's times return value
statsStr="";   # Stats line I am going to print out
miruStr="";    # For printing out the miru entry
amrStr="";     # for freezeTB's amr file
spoligoStr=""; # for freezeTB's spoligotype file
scriptDirStr="$(dirname "$0")";

#*********************************************************
# Sec-01 Sub-03:
#   - Help message
#*********************************************************

helpStr="bash TBOneBench.sh reads.fastq stats.tsv prefix
Use:
   - Does one benchmark on freezeTB and TBProfiler
Input:
   -reads reads.fastq: [Required]
     o reads to profile
   -ref ref.fasta: [Optional]
     o Compare output consensuses to an reference
   -stats out-stats.tsv: [out-stats.tsv]
     o File to save AMR and lneage stats to
   -con-stats out-con-stats.tsv: [out-con-stats.tsv]
     o File to save the consensus stats to
   -head: [No]
     o Print the header for the stats file (-stats)
     o Turned off with -no-head
   -prefix prefix: [out]
     o Prefix for freezeTB and TBProfiler
Output:
   Prints or appends stats to files from -stats and
   -con-stats
";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-02:
#   - Get and check user input
#   o sec-02 sub-01:
#     - get the users input
#   o sec-02 sub-02:
#     - check the users input and check if printing header
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec-02 Sub-01:
#   - get the users input
#*********************************************************

while [[ $# -gt 0 ]]; do
# Loop: Get the users input
   case $1 in
      -reads) readsStr="$2"; shift;;
      -ref) refStr="$2"; shift;;
      -stats) statsFileStr="$2"; shift;;
      -con-stats) conStatsFileStr="$2"; shift;;
      -prefix) prefixStr="$2"; shift;;
      -head) pHeadBl="1";;

      -h) printf "%s\n" "$helpStr"; exit;;
      --h) printf "%s\n" "$helpStr"; exit;;
      help) printf "%s\n" "$helpStr"; exit;;
      -help) printf "%s\n" "$helpStr"; exit;;
      --help) printf "%s\n" "$helpStr"; exit;;

      *) printf "%s\n%s is unkown\n" "$1" "$helpStr";
         exit;;
   esac

   shift;
done # Loop: Get the users input

#*********************************************************
# Sec-02 Sub-02:
#   - check the users input and check if printing header
#*********************************************************

if [[ ! -f "$readsStr" ]]; then
   printf "Could not open $readsStr\n" >> /dev/stderr;
   exit;
fi

if [[ "$pHeadBl" -eq 1 ]]; then
   {
      printf "fastq\tname\tprogram\treadData\ttime";
      printf "\tuserTime\tsysTime\tmemory\tcpu";
      printf "\tdrug\tvarId\tvarTyep\tsupport\tidSpoligo";
      printf "\toctSpolito\tfamilySpligo\tSITSpoligo";
      printf "\tMIRU-0154\tMIRU-0424\tMIRU-0577";
      printf "\tMIRU-0588\tMIRU-0802\tMIRU-0960";
      printf "\tMIRU-1644\tMIRU-1955\tMIRU-2059";
      printf "\tMIRU-2163b\tMIRU-2165\tMIRU-2347";
      printf "\tMIRU-2401\tMIRU-2461\tMIRU-2531";
      printf "\tMIRU-2687\tMIRU-2996\tMIRU-3007";
      printf "\tMIRU-3171\tMIRU-3192\tMIRU-3690";
      printf "\tMIRU-4052\tMIRU-4156\tMIRU-4348\n";
   } > "$statsFileStr";

   {
      printf "fastq\tname\treference\tfragment_id\tstart";
      printf "\taln_len\tmatch_not_n\tmatch_as_n";
      printf "\tsnp_not_n\tsnp_as_n\tins_not_n";
      printf "\tins_as_n\tdel\tmask_not_n";
      printf "\tmask_as_n";
   } > "$conStatsFileStr";
fi # If: I am printing the header

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-03:
#   - run freezeTB and print out stats
#   o sec-03 sub-01:
#     - run freezeTB and get times/memory usage/cpu usage
#   o sec-03 sub-02:
#     - get the consensus stats (spoligtyping, AMRs, MIRU)
#   o sec-03 sub-03:
#     - print the consensus stats
#   o sec-03 sub-04:
#     - get MIRU lineage best supported by reads
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec-03 Sub-01:
#   - run freezeTB and get times/memory usage/cpu usage
#*********************************************************

/usr/bin/time \
    -f "%e\t%U\t%S\t%M\t%P" \
    -o tmp.tsv \
  bash "$scriptDirStr/run-freezeTB.sh" \
      "$readsStr" \
      "$prefixStr";

timeRetStr="$(cat tmp.tsv | sed 's/\%//g;')"; 
rm tmp.tsv;

#*********************************************************
# Sec-03 Sub-02:
#   - get the consensus stats (spoligtyping, AMRs, MIRU)
#*********************************************************

spoligoStr="$(
   awk '
      BEGIN{FS=OFS="\t"; getline;};
      {print $3, $4, $5, $6;};
    ' < "$prefixStr-con-spoligo.tsv"
)";

if [[ "$spoligoStr" == "" ]]; then
   spoligoStr="NA\tNA\tNA\tNA";
fi

miruStr="$(
   awk '
        BEGIN{
            FS=OFS="\t";
            siMiru = 0;
            miruStr = "";

            for(siMiru = 1; siMiru <= 24; ++siMiru)
               miruStr = miruStr "\tNA";

            getline; # get past header
        }; # BEGIN BLOCK

        { # MAIN BLOCK
           miruStr = "";

           for(siMiru = 2; siMiru <= NF; ++siMiru)
              miruStr = miruStr "\t" $siMiru;
        }; # MAIN BLOCK

        END{print miruStr;};
       ' \
    < "$prefixStr-con-miru.tsv"\
)";

#*********************************************************
# Sec-03 Sub-03:
#   - print the consensus stats
#*********************************************************

awk \
   -v readsStr="$readsStr" \
   -v prefixStr="$prefixStr" \
   -v timeStr="$timeRetStr" \
   -v spoligoStr="$spoligoStr" \
   -v miruStr="$miruStr" \
   '
      BEGIN{
         FS=OFS="\t";
         numLineSI = 0;
         getline;
      }; # BEGIN BLOCK

      { # MAIN BLOCK
         ++numLineSI;
         printf "%s\t%s\tfreezeTB", readsStr, prefixStr;
         printf "\tFalse\t%s\t%s\t%s\t1", timeStr, $3, $5;
         printf "\t%s\t%s\t%s\n", $6, spoligoStr, miruStr;
      } # MAIN BLOCK

      END{
         if(numLineSI == 0)
         { # If: There was not resistance
            printf "%s\t%s\tfreezeTB",readsStr,prefixStr;
            printf "\tFalse\t%s\tNA\tNA\tNA", timeStr;
            printf "\t%s\t%s\n", spoligoStr, miruStr;
            printf "\tNA\t%s\t%s\n", spoligoStr, miruStr;
         } # If: There was not resistance
      } # END BLOCK
   ' \
   < "$prefixStr-con-amrs.tsv" \
 >> "$statsFileStr";

#*********************************************************
# Sec-03 Sub-04:
#   - get MIRU lineage best supported by reads
#*********************************************************

miruStr="$(
   awk '
      BEGIN{
         FS=OFS="\t";

         miruStr = "";
         linAryStr[1] = "NA";
         hitsArySI[1] = 0;
         siMiru = 0;

         for(siMiru = 2; siMiru <= 24; ++siMiru)
         { # Loop: Initialize the linage arrays
            linAryStr[siMiru] = "NA";
            hitsArySI[siMiru] = 0;
         } # Loop: Initialize the linage arrays

         getline;
      } # BEGIN BLOCK

      { # MAIN BLOCK
         if($1 == "NA")
            next; # On final (no lineage) entry

         for(siMiru = 2; siMiru <= NF; siMiru += 3)
         { # Loop: Find the most supported lineage
            if($siMiru > hitsArySI[siMiru])
            { # If: This lineage is an better choice
               linAryStr[siMiru] = $1;
               hitsArySI[siMiru] = $siMiru;
            } # If: This lineage is an better choice
         } # Loop: Find the most supported lineage
      } # MAIN BLOCK

      END{
         printf "%s", linAryStr[1];

         for(siMiru = 2; siMiru <= 24; ++siMiru)
            printf "\t%s", linAryStr[siMiru];
      } # END BLOCK
   ' \
   < "$prefixStr-miru-read-tbl.tsv" \
)";

#*********************************************************
# Sec-03 Sub-05:
#   - Print out the reads stats
#*********************************************************


awk \
   -v readsStr="$readsStr" \
   -v prefixStr="$prefixStr" \
   -v timeStr="$timeRetStr" \
   -v miruStr="$miruStr" \
   '
      BEGIN{
         FS=OFS="\t";
         numLineSI = 0;
         getline;
      }; # BEGIN BLOCK

      { # MAIN BLOCK
         ++numLineSI;
         printf "%s\t%s\tfreezeTB", readsStr, prefixStr;
         printf "\tTrue\t%s\t%s\t%s", timeStr, $2, $4;
         printf "\t%s\t%s\tNA\tNA", $9 / 100, $5;
         printf "\tNA\tNA\t%s\n", miruStr;
      } # MAIN BLOCK

      END{
         if(numLineSI == 0)
         { # If: There was not resistance
            printf "%s\t%s\tfreezeTB",readsStr,prefixStr;
            printf "\tTrue\t%s\tNA\tNA\tNA", timeStr;
            printf "\tNA\tNA\tNA\tNA\t%s\n", miruStr;
         } # If: There was not resistance
      } # END BLOCK
   ' \
   < "$prefixStr-read-amrs.tsv" \
 >> "$statsFileStr";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-04:
#   - run TBProfiler and get stats
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/usr/bin/time \
    -f "%e\t%U\t%S\t%M\t%P" \
    -o tmp.tsv \
  bash "$scriptDirStr/run-tbprofiler.sh" \
     "$readsStr" \
     "$prefixStr";
  
timeRetStr="$(cat tmp.tsv | sed 's/\%//g;')"; 
rm tmp.tsv;

awk \
    -f "$scriptDirStr/TBProfiler-process.awk" \
    -v readsStr="$readsStr" \
    -v prefixStr="$prefixStr" \
    -v statsStr="$timeRetStr" \
    "results/$prefixStr.results.csv" \
 >> "$statsFileStr";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-05:
#   - compare consensuses
#   o sec-05 sub-01:
#     - Build the consensus for TBProfiler
#   o sec-05 sub-02:
#     - Convert the consensus for freezeTB
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#********************************************************
# Sec-05 Sub-01:
#   - Build the consensus for TBProfiler
#********************************************************

bcftools index "vcf/$prefixStr.targets.vcf.gz";

if [[ ! -f "$scriptDirStr/tbdb.fasta.fai" ]]; then
   samtools faidx "$scriptDirStr/tbdb.fasta";
fi # If: I need to index the reference

bcftools consensus \
    -f "$scriptDirStr/tbdb.fasta" \
    -o "$prefixStr-TBProfiler-con.fa" \
    "vcf/$prefixStr.targets.vcf.gz";

samtools depth "bam/$prefix.bam" |
  awk \
    '
      BEGIN{getline; FS=OFS="\t";};

      { # MAIN
         if(startSI == -1)
         { # If: I have no coordinates
            startSI = $2;
         }; # If: I have no coordinates

         if($3 > 20)
         { # If: I have good depth
            if(startSI != $2)
            { # If: I have an low depth region to print
               print "tbdb.fasta",
                     "tbprofiler",
                     "False",
                     startSI,
                     endUI;
            } # If: I have an low depth region to print

            startSI = -1;
            endUI = -1;
         } # If: I have good depth
 
        else
           endUI = $2; # Low depth region
      }; # MAIN

      END{
         if(startSI < endUI)
         { # If: I need to print out the last region
            print "tbdb.fasta",
                  "tbprofiler",
                  "False",
                  startSI,
                  endUI;
         } # If: I need to print out the last region
    ' > depth.tsv

minimap2 \
    -a \
    "$scriptDirStr/tbdb.fasta" \
    "$prefixStr-TBProfiler-con.fa" |
  primMask \
    -sam - \
    -depth.tsv |
  filtsam \
    -out-fasta \
  > "$prefixStr-TBProfiler-con-mask.fa";

rm -r results;
rm -r vcf;
rm -r bam;

#********************************************************
# Sec-05 Sub-02:
#   - Convert the consensus for freezeTB
#********************************************************

filtsam \
    -out-fasta \
    "$prefixStr-consensuses.sam" \
  > "$prefixStr-frezeTB-con.fa";

#********************************************************
# Sec-05 Sub-03:
#   - Compare consensuses
#********************************************************

minimap2 \
    --eqx \
    -a \
    "$prefixStr-TBProfiler-con.fa" \
    "$prefixStr-frezeTB-con.fa" |
  filtsam \
    -out-stats \
    -p-n |
  awk \
      -v fqStr="$readsStr" \
      -v prefStr="$prefixStr" \
      '
         BEGIN{FS=OFS="\t";};
         { # MAIN
            print fqStr,
                  prefStr,
                  "tbprofiler", # reference
                  $1,    # Fragment name
                  $5,    # start
                  $7,    # aligned length
                  $9,    # matches (not anonymous)
                  $10,   # matches (anonymous)
                  $12,   # snps (not anonymous)
                  $13,   # snps (anonymous)
                  $15,   # insertions (not anonymous)
                  $16,   # insertions (anonymous)
                  $17,   # deletions
                  $19,   # masking (not anonymous)
                  $10;   # masking (anonymous)
         }; # MAIN
      ' \
>> "$conStatsFileStr";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-06:
#   - compare to an reference
#   o sec-06 sub-01:
#     - TBProfiler to reference comparision
#   o sec-06 sub-02:
#     - freezeTB to reference comparision
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec-06 Sub-01:
#   - TBProfiler to reference comparision
#*********************************************************

if [[ -f "$refStr" ]]; then
# If: I am getting reference results
   minimap2 \
       --eqx \
       -a \
       "$refStr" \
       "$prefixStr-TBProfiler-con.fa" |
     filtsam \
       -out-stats \
       -p-n |
     awk \
         -v fqStr="$readsStr" \
         -v prefStr="$prefixStr" \
         -v refStr="$refStr" \
         '
            BEGIN{FS=OFS="\t";};
            { # MAIN
               print fqStr,
                     prefStr,
                     refStr, # reference
                     $1,     # Fragment name
                     $5,     # start
                     $7,     # aligned length
                     $9,     # matches (not anonymous)
                     $10,    # matches (anonymous)
                     $12,    # snps (not anonymous)
                     $13,    # snps (anonymous)
                     $15,    # insertions (not anonymous)
                     $16,    # insertions (anonymous)
                     $17,    # deletions
                     $19,    # masking (not anonymous)
                     $10;    # masking (anonymous)
            }; # MAIN
         ' \
   >> "$conStatsFileStr";

   #******************************************************
   # Sec-06 Sub-02:
   #   - freezeTB to reference comparision
   #******************************************************

   minimap2 \
       --eqx \
       -a \
       "$refStr" \
       "$prefixStr-frezeTB-con.fa" |
     filtsam \
       -out-stats \
       -p-n |
     awk \
         -v fqStr="$readsStr" \
         -v prefStr="$prefixStr" \
         -v refStr="$refStr" \
         '
            BEGIN{FS=OFS="\t";};
            { # MAIN
               print fqStr,
                     prefStr,
                     refStr, # reference
                     $1,     # Fragment name
                     $5,     # start
                     $7,     # aligned length
                     $9,     # matches (not anonymous)
                     $10,    # matches (anonymous)
                     $12,    # snps (not anonymous)
                     $13,    # snps (anonymous)
                     $15,    # insertions (not anonymous)
                     $16,    # insertions (anonymous)
                     $17,    # deletions
                     $19,    # masking (not anonymous)
                     $10;    # masking (anonymous)
            }; # MAIN
         ' \
   >> "$conStatsFileStr";

fi # If: I am getting reference results
