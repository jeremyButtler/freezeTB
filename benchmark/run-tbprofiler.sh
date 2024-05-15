#!/usr/bin/env bash

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o sec-01:
#     - Variable decleratoins
#   o sec-02:
#     - check user input
#   o sec-03:
#     - Run TBProfiler
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

##########################################################
# Name: run-tbprofiler
#   - Runs TBProfiler for naopore sequence reads
# Input:
#   -$1:
#     o reads to run TBProfiler on
#   -$2:
#     o prefix to name everything
# Output:
#   - TBProfiler saves results results/prefix.results.csv
#   - TBProfiler vcf to vcf/prefix.targets.vcf.gz
##########################################################

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-01:
#   - Variable decleratoins
#   o sec-01 sub-01:
#       - user input variables
#   o sec-01 sub-02:
#       - Help message
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec-01 Sub-01:
#   - user input variables
#*********************************************************

readsStr="$1";
prefixStr="$2";

#*********************************************************
# Sec-01 Sub-02:
#   - Help message
#*********************************************************

helpStr="bash run-tbprofiler.sh reads.fastq prefix
Use:
   - Profiles reads with TBProfiler
Input:
   reads.fastq: reads to profile
   prefix: Name of output
Output:
   TBProfiler outputs results to the results directory
"

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [[ ! -f "$readsStr" ]]; then
   if [[ "$readsStr" == "--help" ]]; then
      fprintf "%s\n" "$helpStr;
      exit;
   fi

   if [[ "$readsStr" == "-h" ]]; then
      fprintf "%s\n" "$helpStr;
      exit;
   fi

   fprintf "Could not open $readsStr\n" >> /dev/stderr;
   exit;
fi

if [[ "$prefixStr" == "" ]]; then
   if [[ "$prefixStr" == "--help" ]]; then
      fprintf "%s\n" "$helpStr;
      exit;
   fi

   if [[ "$prefixStr" == "-h" ]]; then
      fprintf "%s\n" "$helpStr;
      exit;
   fi

   prefixStr = "out";
fi

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-03:
#   - Run TBProfiler
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#python3 "$HOME/.local/bin/tb-profiler" \
#python3 "$HOME/miniforge3/bin/tb-profiler" \
#tb-profiler \
"$HOME/miniforge3/bin/tb-profiler" \
   profile \
   --platform nanopore \
   --prefix "$prefixStr" \
   --csv \
   --caller bcftools \
   --spoligotype \
   --read1 "$readsStr";
