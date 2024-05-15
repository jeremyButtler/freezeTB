#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Awk-01 TOC:
#   - Parse the TBProfiler results file
#   o awk-01 sec-01:
#     - Set up tab separators and declare variables
#   o awk-01 sec-02:
#     - Main entry; process the file
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Awk-01 Sec-01:
#   - Set up tab separators and declare variables
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

BEGIN{
   FS = ",";
   OFS = "\t";

   spoligoFlag = 0;

   spoligoIdStr = "NA";
   spoligoOctalStr = "NA";
   spoligoFamilyStr = "NA";
   spoligoSITStr = "NA";

   variantFlag = 0;

   numVarSI = 0;
   geneAryStr[1] = "NA";
   varIdAryStr[1] = "NA";
   amrSupAryF[1] = "NA";
   drugAryStr[1] = "NA";
}; # BEGIN BLOCK

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Awk-01 Sec-02:
#   - Main entry; process the file
#   o awk-01 sec-02 sub-01:
#     - Handle spoligotype entries
#   o awk-01 sec-02 sub-02:
#     - Handle the antibiotic resistance entries
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

{  # MAIN BLOCK

   #************************************************
   # Awk-01 Sec-02 Sub-01:
   #   - Handle spoligotype entries
   #************************************************

   if(spoligoFlag ==0 && $1 =="Spoligotype report")
      spoligoFlag = 1;

   if(spoligoFlag == 1)
   { # If: I am on the spoligotype entry
      if($0 ~ /^-/)
         next; # On separator

      if($0 ~ /^Spo/)
         next; # On table name (header)

      if($0 ~ /^$/)
      { # If: On blank line (end of entry)
         spoligoFlag = 2;

         if(spoligoFamilyStr == "ATYPIC")
         { # If: there is no spoligotype
            spoligoIdStr = "NA";
            spoligoOctalStr = "NA";
            spoligoFamilyStr = "NA";
            spoligoSITStr = "NA";
         } # If: there is no spoligotype

         next;
      } # If: On blank line (end of entry)

      if($1 == "Binary")
         spoligoIdStr = $2;

      else if($1 == "Octal")
         spoligoOctalStr = $2;

      else if($1 == "Family")
         spoligoFamilyStr = $2;

      else if($1 == "SIT")
         spoligoSITStr = $2;
   } # If: I am on the spoligotype entry

   #************************************************
   # Awk-01 Sec-02 Sub-02:
   #   - Handle the antibiotic resistance entries
   #************************************************

   else if(spoligoFlag == 2 &&
        variantFlag == 0 &&
       $1 == "Resistance variants report")
   { # Else If: I am on the reistant report
      variantFlag = 1;
   } # Else If: I am on the reistant report

   if(variantFlag == 1)
   { # If: I have the variant list
      if($1 ~ /^-/)
         next; # On header separator

      if($0 ~ /^$/)
      { # If: On blank line (end of entry)
         variantFlag = 2;
         next;
      } # If: On blank line (end of entry)

      if($1 ~ /^G/)
          next; # On header

      if($1 ~ /^R/)
          next; # On the table name

      if($8 == "Uncertain significance")
         next;

      if($8 == "Not assoc w R")
         next;

      # Not from the 2023 catalog
      if($9 == "Mutation from literature")
         next;

      ++numVarSI;

      geneAryStr[numVarSI] = $3;
      varIdAryStr[numVarSI] = $5;
      amrSupAryF[numVarSI] = $6;
      drugAryStr[numVarSI] = $7;
   } # If: I have the variant list
}; # MAIN BLOCK

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Awk-01 Sec-03:
#   - End entry; print out the parsed results
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

END{
   miruStr = "";

   for(siMiru = 1; siMiru <= 24; ++siMiru)
      miruStr = miruStr "\t" "NA";

   for(siAmr = 1; siAmr <= numVarSI; ++siAmr)
   { # Loop: Print out the output tsv file
      print readsStr,
            prefixStr,
            "TBProfiler",
            "False",
            statsStr,
            drugAryStr[siAmr],
            varIdAryStr[siAmr],
            amrSupAryF[siAmr],
	    "NA",
            spoligoIdStr,
            spoligoOctalStr,
            spoligoFamilyStr,
            spoligoSITStr,
            miruStr;
   } # Loop: Print out the output tsv file

   if(numVarSI == 0)
   { # If: I had not varaints
      print readsStr,
            prefixStr,
            "TBProfiler",
            "False",
            statsStr,
            drugAryStr[1],
            varIdAryStr[1],
            amrSupAryF[1],
	    "NA",
            spoligoIdStr,
            spoligoOctalStr,
            spoligoFamilyStr,
            spoligoSITStr,
            miruStr;
     # Arrays have NA as default.
   } # If: I had not varaints
}; # END BLOCK
