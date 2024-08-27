# Use:

Converts the WHO 2023 tuberculosis anti-microbial
  resitance catalog to a flat file without all the stats.

It looks like the crew behind TBProfiler have a parsing
  script for the 2023 catalog as well. I would recommend
  using their script before this repository. It will be
  much more better designed.
  [https://github.com/jodyphelan/who-catalogue-parsing](
   https://github.com/jodyphelan/who-catalogue-parsing
  ). 

I am grateful to the WHO for putting the 2023 catalog
  together. Also, I am very grateful for the WHO in
  taking time to review this repository and give me
  feedback. They gave me several points that helped in
  improving the output.

I have no idea if the 2021 catalog conversion still works.
  I had to do some changes to get stuff working on windows
  and have not tested the 2021 option.

# License:

Dual licensed under public domain or MIT. Pick the one
  that works best for you.

# Install

To build you will use `make -f` with the make
  file targeting your OS. The OS the make file was built
  for is the end of the name.

Make file names:
  - mkfile.unix: any unix (Mac/Linux/BSD) OS
  - mkfile.static: static build (Linux/BSD)
  - mkfile.win: windows (does not install)
    - use `nmake /F` instead of `make -f`

```
make -f mkfile.unix
sudo make install;
```

# Output

## The tsv file(s)

The output does not have every bit of data from the WHO
  catalog. Instead it focuses on the elements I think
  are of more usefull.

The format is as follows:

- Column one is the gene name
- Column two is the variant id
- Column three is/should be the position of the first base
  in the pattern on the reference.
- Column four is the direction of the gene
  - 'F' is forward
  - 'R' is reverse
  - "NA" is unknown (Sorry about the previous mistake)
- Column five is the mutation
  - An deletion (del) is assigned when  the AMR sequence
    is shorter than the reference.
  - An insertion (ins) is assigned when  the AMR sequence
    is longer than the reference.
  - snp is assigned if the AMR and reference sequences are
    the same length
  - lof is assigned if it is in the variant id
- Column six
  - 1 if there is a frame shift
  - 0 if there is no frame shift
- Column seven
  - Has the reference sequence
- Column eight
  - Has the AMR sequence
- Column nine has the position of the first reference base
  in the codon, or NA if there is no codon, or if it could
  not be found
- Column ten has the number of the first codon in the gene
  or NA if not present
- Column eleven has the number of the last codon in the
  gene or NA if not present
- Column twelve
  - has the reference amino acid sequence or
  - 0 if it is not present
    - I am not using NA, because both N and A are amino
      acid codes
- Column thirteen has the AMR amino acid sequence or 0
  - has the AMR amino acid sequence or
  - 0 if it is not present
    - I am not using NA, because both N and A are amino
      acid codes
- Column fourteen has the starting position of the
  reference gene.
- Column fifteen has the ending position of the reference
  gene.
- Column sixteen
  - 1 if the AMR is an high resistance
  - 0 if not an high resistant AMR
- Column seventeen
  - 1 if the AMR is an low resistance
  - 0 if the AMR is not low resistance (high/mid)
- Column eighteen
  - 1 if multiple low resitistance AMR's in the same gene
    can result in high resistance
  - 0 if not AMR effect is not additive
- Column nineteen
  - has a name of a gene that is is needed to be function
    to have the mutation confer antibiotic resistance
  - NA if there is no needed gene
- The next columns (until the endAntibiotics column) are
  specific antibiotics that a AMR confers resistance to.
  - 0 is no known resistance
  - 1 is resistance
  - 2 is cross-resistance
  - 3 is the entry appears in both the resistant and
    cross-resistance entries
- endAntibiotics column marks the end of the antibiotic
  entries. It is filled with '\*'s
- First column after endAntibiotics
  - has the WHO's recorded effects for the AMR
  - NA if no effect
- Second column after endAntibiotics
  - has the comment that the WHO had for the AMR
  - NA if there is no comment
  - This was the last column for the March 18, 2024
    version
- Third column after endAntibiotics (grade)
  - Has the WHOs grade for the primary antibiotic
  - 0 if there is no grade
- Fourth column after endAntibiotics (entireGene)
  - 1 if is this is an LoF entry (frameshift anywere in
    the genome).
  - 2 is for an entire gene deletion
  - 0 if the mutation only applies to a single position
- fith column after end antibiotics (unkownEntry)
  - 1 if I could not figure this entry out
  - 0 if the entry was catagorized
- There could be future columns added, so long as there
  always added at the end. So, always look for the last
  known column by looking for both newlines '\n' and tabs
  '\t' (in C you can do `*char < 31`).
  
# Running

Help message: `whoToTbAmr -h`

Catalog with only grades 1 and 2:

```
whoToTbAmr -ref NC000962.fa -coords gene-coordinates.tsv -master WHO-2023-TB-tab1-master.tsv -indice WHO-2023-TB-tab2-indices.tsv -out catalog.tsv
```

All grades:

```
whoToTbAmr -all-amrs -ref NC000962.fa -coords gene-coordinates.tsv -master WHO-2023-TB-tab1-master.tsv -indice WHO-2023-TB-tab2-indices.tsv -out catalog.tsv
```

# Building gene coordinates table

This is just a table of gene coordinates in the TB genome.

Do not use paf files from minimap2. For some odd reason
  the coordinates disagree with the sam file by a one or
  two bases.

```
minimap2 \
    -a \
    NC000962.fa \
    genes.fa |
  awk '
      BEGIN{FS=OFS="\t";};

      { # MAIN
         if($0 ~ /^@/) next; # Comment entry
         if($2 == 16) dirSC = "-"; # is reverse complement
         else dirSC = "+";         # is an foward gene

         sub(/M/, "", $6); #Remove matches; becomes length
         --$6; # Acount for index 1
               # Avoids adding two index 1 values

         geneEndSI = $4 + $6;
            # $4 +$6 = gene_start + gene_length = gene_end

         if(geneEndSI < $4)
            print $1, $3, dirSC, geneEndSI, $4;
         else
            print $1, $3, dirSC, $4, geneEndSI;
            # $1 = gene name
            # $3 = reference name
            # dirSC = direction (+ or -)
            # $4 = starting position (index 1)
      } # MAIN
' > gene-tbl.tsv
```

# Bugs/errors in output:

## Comments

This code is done as well as I could get it. However,
  there will likely still be problems. There are likely
  bugs or errors I missed.

I am using the database in my own code, so please report
  errors in the files or bugs in the code.

## Known bugs:

### With my program

I am not doing an AA translation for frame shifts beyond
  the variant id. However, This could be done. I just do
  not see a need for this.
