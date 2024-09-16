#!/usr/bin/env bash

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o sec-01:
#     - Variable declerations
#   o sec-02:
#     - Check user input
#   o sec-03:
#     - merge the primers
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-01:
#   - Variable declerations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

refStr="";
primStr="";
tsvStr="";
prefixStr="primers";
numMaskSI=20;

helpStr="
bash catPrimers.sh -ref reference.fa -primers primers.fa
   - Merges the amplicons made by the input primers
Input:
   -ref/-r Reference.fa: [Required]
     o Also \"-r reference.fa\"
     o Has the reference sequence
   -tsv: [Required if -primers not used]
     o At least a three column tsv file with the primer
       sequences
     o Each row starts of with the name, then the foward
       sequence, followed by the reverse sequence. Both
       spaces and tabs are treated as deliminators (no
       spaces allowed).
       - Header: The first column needs to be named
         \"gene\". This can be left out.
       - Rows:
         \"name\tforward_sequence\treverse_sequence\"
   -len-mask: [$numMaskSI]
     o Length of N's to add to end/start of each amplicon
   -prefix: [pimers]
     o Prefix to add to file names
   -primers primers.fa: [Required if -tsv not used]
     o Also \"-p primers.fa\"
     o Has the input primers, with the reverse primer
       following the forward primer.
       >primer-1-F
       ataga
       >primer-1-R
       ggaaa
       >primer-2-F
       ccata
       >primer-2-R
       ttatac
       >primer-3-F
       gtaca
       >primer-3-R
       tgatca
        .
        .
        .
       >primer-n-F
       cgaga
       >primer-n-R
       tgacag
Output:
   - prefix-merged-amplicons.fa is the fasta file with the
     merged sequences
   - prefix-coordinates.tsv is a tsv file with the primer
     coordiantes \"name\tstart\tend\"
   - prefix-primers-map.sam a sam file from minimap2
     sorted by the starting position
   - prefix-primers.fa is the tsv file converted to a
     fasta file
   - Prints an error message for every primer that had an
     amplicon longer then 3kb. These are likely primers
     that had alternative binding sites.
Requires:
   - Minimap2 and standard unix programs
";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-02:
#   - Check user input
#   o sec-02 sub-01:
#     - Get user input
#   o sec-02 sub-01:
#     - Check if the user input valid input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec-02 Sub-01:
#   - Get user input
#*********************************************************

while [[ $# -gt 0 ]]; do
# Loop: Get user input
   case $1 in
      -ref) refStr=$2; shift;;
      -r) refStr=$2; shift;;

      -primers) primStr=$2; shift;;
      -p) primStr=$2; shift;;

      -tsv) tsvStr=$2; shift;;

      -prefix) prefixStr=$2; shift;;

      -len-mask) numMaskSI=$2; shift;;

      -h) printf "%s\n" "$helpStr"; exit;;
      --h) printf "%s\n" "$helpStr"; exit;;
      -help) printf "%s\n" "$helpStr"; exit;;
      --help) printf "%s\n" "$helpStr"; exit;;
      help) printf "%s\n" "$helpStr"; exit;;

      *) printf \
           "%s\n%s is not reconognized\n" \
            "$helpStr" \
            "$1";
         exit;;
   esac

   shift; # Move of the argument
done # Loop: Get user input

#*********************************************************
# Sec-02 Sub-02:
#   - Check if the user input valid input
#*********************************************************

if [[ ! -f "$refStr" ]]; then
# If: Invalid reference file input
   { # Print invadlid reference file error
      printf "%s\nCould not open the";
      printf " reference file (-ref %s)\n" "$refStr";
   } # Print invadlid reference file error

    exit;
fi # If: Invalid reference file input

if [[ ! -f "$primStr" && ! -f "$tsvStr" ]]; then
# If: Invalid primer file input
   { # Print invadlid reference file error
      printf "%s\nCould not open the" "$helpStr";
      printf " primer file (-primers %s) or" "$primStr";
      printf " the tsv file (-tsv %s)\n" "$tsvStr";
   } # Print invadlid reference file error

    exit;
fi # If: Invalid primer file input

if [[ -f "$primStr" && -f "$tsvStr" ]]; then
# If: the user provide both a tsv and primer file
   printf " -tsv and -primers can not be used together\n";
   exit;
fi # If: the user provide both a tsv and primer file

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-03:
#   - merge the primers
#   o sec-03 sub-01:
#     - Translate tsv into fasta if user input
#   o sec-03 sub-02:
#     - Build the paf file with the primer coordinates
#   o sec-03 sub-03:
#     - Merge the amplicons
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec-03 Sub-01:
#   - Translate tsv into fasta if user input
#*********************************************************

if [[ -f "$tsvStr" ]]; then
# If: The user input a tsv file with primers
   awk '
         { # MAIN
            if(NR == 1 && $1 == "gene") next; # header
            printf ">%s_F\n%s\n", $1, $2; # Foward primer
            printf ">%s_R\n%s\n", $1, $3; # Reverse primer
         } # Main
      ' \
      "$tsvStr" \
     > "$prefixStr-primers.fa";

   primStr="$prefixStr-primers.fa";
fi # If: The user input a tsv file with primers

#*********************************************************
# Sec-03 Sub-02:
#   - Build the paf file with the primer coordinates
#*********************************************************

minimap2\
    -k5 \
    -w1 \
    -s 20 \
    -m 15 \
    -a \
    --secondary="no" \
    "$refStr" \
    "$primStr" 2>/dev/null |
  sort -n -k 8 \
  > "$prefixStr-primer-maps.sam";

   # Parmaters for  minimap2:
   #   -k is minimizer k-mer length (15 is half primer)
   #   -w Minimizer window size (no windows)
   #   -s Minimap peak alignment score (not total score)
   #   -P Retain all chains (ignore -p and - N)
   #   -m Discard shanbeds with score less than m
   # I am using sort -k 8 to sort the mappings by there
   #   starting positions (in column 8)

#*********************************************************
# Sec-03 Sub-03:
#   - Merge the amplicons
#*********************************************************

awk  \
    -v prefixStr="$prefixStr" \
    -v maskSI="$numMaskSI" \
    '
      # This block fires at the start. Here I am just
      #   setting some initial values
      BEGIN{
         numPairsI = 0;
         seqBl = 0;
         outStr = prefixStr "-coodinates.tsv";
         maskStr = "";
         outMaskStr = prefixStr "-mask-primers.tsv";

         for(siN = 0; siN < maskSI; ++siN)
            maskStr = maskStr "N";
      } # BEGIN

      { # Main
         # This first file is the reference file. I need
         #   its name so that I can know when I changed
         #   files
         if(refStr == "") refStr = FILENAME;

         # Here I am merging the reference sequence into
         #   a single line
         if(refStr == FILENAME)
         { # If: I am on the reference sequence
            if($0 ~ /^>/)
            { # If: this is a header (skip)
               if(seqBl == 1) exit; # More then 1 sequence

               seqBl = 1;
               next; # Move to the next line in the file
            } # If: this is a header (skip)

            seqStr = seqStr $0; # mege the sequences
         } # If: I am on the reference sequence

         # This is the output from minimap2
         else
         { # Else: I am working on the paf file
            if($0 ~ /^@/) next; # On header

            if(numPrimI % 2 == 0) 
            { # If: this is a foward primer (new primer)
               ++numPrimI;
               ++numPairsI; # this is a new pair

               sub(/_.*/, "", $1); # clean up primer name
               primStr[numPairsI] = $1; # save the name

               # record the start position
               startIAry[numPairsI] = $4;

               /*Find the alingned length*/
               matchStr = $6;
               delStr = $6;

               gsub(/[0-9]*[IDSH]/, "", matchStr);
               gsub(/[X=]/, "M", matchStr);
               gsub(/[0-9]*[IX=MSH]/, "", delStr);

               lenMatchSI= split(matchStr,matchArySI,"M");
               lenDelSI = split(delStr, delArySI, "D");
              
               numNtSI = 0;

               for(iNt = 1; iNt <= lenMatchSI; ++iNt)
                  numNtSI += matchArySI[iNt];

               for(iNt = 1; iNt <= lenDelSI; ++iNt)
                  numNtSI += delArySI[iNt];

               forEndArySI[numPairsI] = $4 + numNtSI;
            } # If: this is a foward primer (new primer)

            else
            { # Else: I am on the reverse primer
               ++numPrimI;

               # record the end position
               revStartArySI[numPairsI] = $4;

               /*Find the alingned length*/
               matchStr = $6;
               delStr = $6;

               gsub(/[0-9]*[IDSH]/, "", matchStr);
               gsub(/[X=]/, "M", matchStr);
               gsub(/[0-9]*[IX=MSH]/, "", delStr);

               lenMatchSI= split(matchStr,matchArySI,"M");
               lenDelSI = split(delStr, delArySI, "D");
              
               numNtSI = 0;

               for(iNt = 1; iNt <= lenMatchSI; ++iNt)
                  numNtSI += matchArySI[iNt];

               for(iNt = 1; iNt <= lenDelSI; ++iNt)
                  numNtSI += delArySI[iNt];

               revEndArySI[numPairsI] = $4 + numNtSI;
            } # Else: I am on the reverse primer
         } # Else: I am working on the paf file
      } # Main

      END{
         printf "ref\tgene\tpaired" > outMaskStr;
         printf "\tforStart\tforEnd" >> outMaskStr;
         printf "\trevStart\trevEnd\n" >> outMaskStr;

         gsub(/[ \t]/, "_", refStr);
         sub(/\.fasta\.?g?z?$/, "", refStr);
         sub(/\.fa\.?g?z?$/, "", refStr);

         printf "ref\tid\tampStart\tampEnd\n" > outStr;

         for(iPrim = 1; iPrim <= numPairsI; ++iPrim)
         { # Loop: Merge the ampicons together

            if(revEndArySI[iPrim] > startIAry[iPrim])
            { # If: this is on a forward gene
               startI = startIAry[iPrim];
               lenI = revEndArySI[iPrim] - startI;
            } # If: this is on a forward gene

            else
            { # Else: This is on a reverse gene
               lenI = startIAry[iPrim]-revEndArySI[iPrim];
               startI = revEndArySI[iPrim] - lenI;
            } # Else: This is on a reverse gene

            # At times a primers best mapping may not
            #   be the intented mapping. In this case I
            #   can do nothing
            if(lenI > 3000)
            { # If: this primer was messed up
               printf "Primer %s is to long (%i to %i)\n",
                  primStr[iPrim],
                  startIAry[iPrim] ,
                  revEndArySI[iPrim] > "/dev/stderr";

               continue;
            } # If: this primer was messed up


            # Output coordinates
            printf "%s\t%s\t%i\t%i\n",
               refStr,
               primStr[iPrim],
               startIAry[iPrim],
               revEndArySI[iPrim] >> outStr;

            printf "%s\t%s\tFalse\t%i\t%i\t%i\t%i\n",
               refStr,
               primStr[iPrim],
               startIAry[iPrim],
               forEndArySI[iPrim],
               revStartArySI[iPrim],
               revEndArySI[iPrim] >> outMaskStr;

            # Extract the amplicon
            ampStr = substr(seqStr,startIAry[iPrim],lenI);

            # Merge the amplicons
            catStr = catStr maskStr;
            catStr = catStr ampStr;
         } # Loop: Merge the ampicons together

         printf ">mergedAmps\n%s%s\n", catStr, maskStr;
      } # END
      ' \
    "$refStr" \
    "$prefixStr-primer-maps.sam" \
  > "$prefixStr-merged-amplicons.fa";

exit;
