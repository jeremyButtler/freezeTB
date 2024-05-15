##########################################################
# Name: run-freezeTB
#   - Runs freezeTB for naopore sequence reads
# Input:
#   -$1:
#     o reads to run freezeTB on
#   -$2:
#     o prefix to name everything
# Output:
#   - a lot of files in the current directory or
#     directory pointed to by freezeTB
##########################################################

readsStr="$1";
prefixStr="$2";

minimap2 \
    -a \
    /usr/local/share/freezeTBFiles/NC000962.fa \
    "$readsStr" \
  > tmp.sam;

freezeTB -sam tmp.sam -prefix "$prefixStr";

rm tmp.sam;
