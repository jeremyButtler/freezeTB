# Use:

Make or adds AMR entries to an tbAmr database.

Be aware, this is only for simple variants. It can not
  handle ids with multiple variants. Or even some of
  the more complex ids. It should be able to handle
  everything seen in the 2023 WHO catalog.

# Install:

## Unix:

```
if [ ! -f /usr/local/bin ];
then
   sudo mkdir /usr/local/bin;
fi;

cd ~/Downloads;

if [ ! -d freezeTB ];
then
   git clone https://github.com/jeremyButtler/freezeTB;
fi;

cd freezeTB/programs/addAmrSrc;
make -f mkfile.unix;
sudo make -f mkfile.unix install
```

# Using:

This program has three required inputs, a file with the
  coordinates of each gene (for tb (H37Rv) use
  ../../freezeTBFiles/all-genes-coords.tsv), a reference
  genome (for H37Rv use ../../freezeTBFiles/NC000962.fa),
  and a file with the variant ids to convert.

`addAmr -coords ../../freezeTBFiles/all-genes-coords.tsv -ref ../../freezeTBFiles/NC000962.fa -var <variants.tsv> > <database>.tsv;`

You can also provide a database to add the variant to with
  `-db database.tsv`. The only thing I am not sure on
  is what happens when you add new drugs.

For the variant id file, the first column is the variant,
  while the remaining columns (tabs and/or spaces) are
  meta-data or drugs resistant to. The only two required
  items are the variant id and at least one drug it is
  resistant to.

The only thing to be aware of is that amino acids are
  backwards translated for the AMR, but the sequence is
  used for the reference. This means for indel amino acid
  variants, you will see a different sequence for the
  reference and AMR.

| flags            | meaning in variant file      |
|:-----------------|:-----------------------------|
| -amk             | amikacin resistance          |
| -bdq             | bedaquiline resistance       |
| -cap             | capreomycin resistance       |
| -cfz             | clofazimine resistance       |
| -dlm             | delamanind resistance        |
| -emb             | ethambutol resistance        |
| -eto             | ethionamide resistance       |
| -flq             | fluoroquine resistance       |
| -inh             | isoniazid resistance         |
| -kan             | kanamycin resistance         |
| -lfx             | levofloxacin resistance      |
| -lzd             | linezolid resistance         |
| -mfx             | moxifloxacin resistance      |
| -pmd             | protonimid resistance        |
| -pza             | pyrazinamide resistance      |
| -rif             | rifampicin resistance        |
| -stm             | streptomycin resistance      |
| -other <drug>    | a drug not listed above      |
| -grade-1         | is a WHO grade 1 AMR         |
| -grade-2         | is a WHO grade 2 AMR         |
| -grade-3         | is a WHO grade 3 AMR         |
| -grade-4         | is a WHO grade 4 AMR         |
| -grade-5         | is a WHO grade 5 AMR         |
| -high-res        | causes high resistance       |
| -low-res         | causes low resistance        |
| -add-res         | resistances are addivitive   |
| -needs <gene>    | needs <gene> to be resistant |
| -note "note"     | your note about the AMR      |
| -effect <effect> | effect of AMR                |

Table: List of flags you can use in the variant file.

You can also use `-gene <gene>` to provided the gene the
  varaint is in. Only use this if the gene is not in the
  variant id (ex: you have `p.Lys23Gly` instead
  of `katG_p.Lys23Gly`).

Here is an example of a line from a variant
  file `rrl_n.2270G>C -lzd -note forward_gene`.

Here is the variant file used to test this program.

```
fgd1_LoF -dlm -note forward
fgd1_p.Asn112fs -other delamanid -note forward

rpoB_p.Ser431_Gln432insArg -other rifampicin -note forward
gyrB_p.Ser447Phe	-lfx -note forward

rpoB_p.Gln432_Met434del	-rif -note forward
rpoB_p.Asn437_Asn438del -rif -note forward
rpoB_p.Phe433dup	-grade-1 -note forward
rpoB_p.Gln432_Met434delinsLeu -rif -note forward

Rv0678_p.Met1?	-bdq -needs MmpL5 -note forward
Rv0678_p.Asp8fs	-cfz	-needs MmpL5	-note forward
Rv0678_p.Gln22* -bdq -needs MmpL5 -note forward

katG_LoF	-inh -high-res -note reverse
katG_p.Pro569fs	-inh	-high-res -note reverse
katG_p.Trp689*	-inh -high-res  -note reverse
katG_p.Trp328Leu -inh	-grade-1 -high-res -note reverse
katG_deletion -inh -high-res -note reverse
katG_p.Met1?	-grade-1 -inh -high-res -note reverse

pncA_p.Ter187Argext*? -pza -note reverse
pncA_c.-11A>G -pza -grade-1 -note reverse_gene

rrl_n.2270G>C -lzd -note forward_gene
rrl_n.2269_2270insT -lzd -note forward_gene

eis_c.-8delC -kan -note reverse_gene

inhA_c.-154G>A -eto -note forward_gene
inhA_c.-154G>A -inh -low-res -add-res -grade-1 -note low_res
```

# Problems:

Valgrind is reporting an unitialized variable error that
  I have not been able to track down. Might be a memory
  overflow. It is always on the first printed variant.
  Then a few others.

```
==18744== Memcheck, a memory error detector
==18744== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==18744== Using Valgrind-3.24.0 and LibVEX; rerun with -h for copyright info
==18744== Command: ./addAmr -coords /usr/local/share/freezeTBFiles/coords.tsv -ref /usr/local/share/freezeTBFiles/NC000962.fa -var test.tsv -out del.tsv
==18744== 
0
==18744== Conditional jump or move depends on uninitialised value(s)
==18744==    at 0x4061FAD: memchr (memchr.c:16)
==18744==    by 0x4062F75: strnlen (strnlen.c:5)
==18744==    by 0x405D33A: printf_core (vfprintf.c:594)
==18744==    by 0x405D4DA: vfprintf (vfprintf.c:683)
==18744==    by 0x4058166: fprintf (fprintf.c:9)
==18744==    by 0x111BB4: p_amrST (amrST.c:959)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Uninitialised value was created by a heap allocation
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x119F4D: main (mainAddAmr.c:1239)
==18744== 
==18744== Conditional jump or move depends on uninitialised value(s)
==18744==    at 0x4061FDC: memchr (memchr.c:17)
==18744==    by 0x4062F75: strnlen (strnlen.c:5)
==18744==    by 0x405D33A: printf_core (vfprintf.c:594)
==18744==    by 0x405D4DA: vfprintf (vfprintf.c:683)
==18744==    by 0x4058166: fprintf (fprintf.c:9)
==18744==    by 0x111BB4: p_amrST (amrST.c:959)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Uninitialised value was created by a heap allocation
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x119F4D: main (mainAddAmr.c:1239)
==18744== 
==18744== Conditional jump or move depends on uninitialised value(s)
==18744==    at 0x4062045: memchr (memchr.c:25)
==18744==    by 0x4062F75: strnlen (strnlen.c:5)
==18744==    by 0x405D33A: printf_core (vfprintf.c:594)
==18744==    by 0x405D4DA: vfprintf (vfprintf.c:683)
==18744==    by 0x4058166: fprintf (fprintf.c:9)
==18744==    by 0x111BB4: p_amrST (amrST.c:959)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Uninitialised value was created by a heap allocation
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x119F4D: main (mainAddAmr.c:1239)
==18744== 
==18744== Conditional jump or move depends on uninitialised value(s)
==18744==    at 0x405D354: printf_core (vfprintf.c:595)
==18744==    by 0x405D4DA: vfprintf (vfprintf.c:683)
==18744==    by 0x4058166: fprintf (fprintf.c:9)
==18744==    by 0x111BB4: p_amrST (amrST.c:959)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Uninitialised value was created by a heap allocation
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x119F4D: main (mainAddAmr.c:1239)
==18744== 
==18744== Syscall param writev(vector[0]) points to uninitialised byte(s)
==18744==    at 0x40568FC: __syscall3 (syscall_arch.h:29)
==18744==    by 0x40568FC: __stdio_write (__stdio_write.c:15)
==18744==    by 0x4056F9F: fflush (fflush.c:29)
==18744==    by 0x4056E25: fclose (fclose.c:12)
==18744==    by 0x1123A5: p_amrST (amrST.c:1214)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Address 0x50b3cb1 is 529 bytes inside a block of size 1,264 alloc'd
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x40561FF: fdopen (__fdopen.c:21)
==18744==    by 0x4057CDE: fopen (fopen.c:26)
==18744==    by 0x111820: p_amrST (amrST.c:870)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Uninitialised value was created by a heap allocation
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x119F4D: main (mainAddAmr.c:1239)
==18744== 
==18744== 
==18744== HEAP SUMMARY:
==18744==     in use at exit: 472 bytes in 4 blocks
==18744==   total heap usage: 48 allocs, 44 frees, 15,892,491 bytes allocated
==18744== 
==18744== LEAK SUMMARY:
==18744==    definitely lost: 0 bytes in 0 blocks
==18744==    indirectly lost: 0 bytes in 0 blocks
==18744==      possibly lost: 0 bytes in 0 blocks
==18744==    still reachable: 0 bytes in 0 blocks
==18744==         suppressed: 472 bytes in 4 blocks
==18744== 
==18744== ERROR SUMMARY: 6 errors from 5 contexts (suppressed: 0 from 0)
==18744== 
==18744== 1 errors in context 1 of 5:
==18744== Syscall param writev(vector[0]) points to uninitialised byte(s)
==18744==    at 0x40568FC: __syscall3 (syscall_arch.h:29)
==18744==    by 0x40568FC: __stdio_write (__stdio_write.c:15)
==18744==    by 0x4056F9F: fflush (fflush.c:29)
==18744==    by 0x4056E25: fclose (fclose.c:12)
==18744==    by 0x1123A5: p_amrST (amrST.c:1214)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Address 0x50b3cb1 is 529 bytes inside a block of size 1,264 alloc'd
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x40561FF: fdopen (__fdopen.c:21)
==18744==    by 0x4057CDE: fopen (fopen.c:26)
==18744==    by 0x111820: p_amrST (amrST.c:870)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Uninitialised value was created by a heap allocation
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x119F4D: main (mainAddAmr.c:1239)
==18744== 
==18744== 
==18744== 1 errors in context 2 of 5:
==18744== Conditional jump or move depends on uninitialised value(s)
==18744==    at 0x405D354: printf_core (vfprintf.c:595)
==18744==    by 0x405D4DA: vfprintf (vfprintf.c:683)
==18744==    by 0x4058166: fprintf (fprintf.c:9)
==18744==    by 0x111BB4: p_amrST (amrST.c:959)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Uninitialised value was created by a heap allocation
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x119F4D: main (mainAddAmr.c:1239)
==18744== 
==18744== 
==18744== 1 errors in context 3 of 5:
==18744== Conditional jump or move depends on uninitialised value(s)
==18744==    at 0x4062045: memchr (memchr.c:25)
==18744==    by 0x4062F75: strnlen (strnlen.c:5)
==18744==    by 0x405D33A: printf_core (vfprintf.c:594)
==18744==    by 0x405D4DA: vfprintf (vfprintf.c:683)
==18744==    by 0x4058166: fprintf (fprintf.c:9)
==18744==    by 0x111BB4: p_amrST (amrST.c:959)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Uninitialised value was created by a heap allocation
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x119F4D: main (mainAddAmr.c:1239)
==18744== 
==18744== 
==18744== 1 errors in context 4 of 5:
==18744== Conditional jump or move depends on uninitialised value(s)
==18744==    at 0x4061FDC: memchr (memchr.c:17)
==18744==    by 0x4062F75: strnlen (strnlen.c:5)
==18744==    by 0x405D33A: printf_core (vfprintf.c:594)
==18744==    by 0x405D4DA: vfprintf (vfprintf.c:683)
==18744==    by 0x4058166: fprintf (fprintf.c:9)
==18744==    by 0x111BB4: p_amrST (amrST.c:959)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Uninitialised value was created by a heap allocation
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x119F4D: main (mainAddAmr.c:1239)
==18744== 
==18744== 
==18744== 2 errors in context 5 of 5:
==18744== Conditional jump or move depends on uninitialised value(s)
==18744==    at 0x4061FAD: memchr (memchr.c:16)
==18744==    by 0x4062F75: strnlen (strnlen.c:5)
==18744==    by 0x405D33A: printf_core (vfprintf.c:594)
==18744==    by 0x405D4DA: vfprintf (vfprintf.c:683)
==18744==    by 0x4058166: fprintf (fprintf.c:9)
==18744==    by 0x111BB4: p_amrST (amrST.c:959)
==18744==    by 0x11A43D: main (mainAddAmr.c:1475)
==18744==  Uninitialised value was created by a heap allocation
==18744==    at 0x48B77E4: malloc (vg_replace_malloc.c:446)
==18744==    by 0x119F4D: main (mainAddAmr.c:1239)
==18744== 
--18744-- 
--18744-- used_suppression:      1 musl-dynlink4 /usr/libexec/valgrind/default.supp:609 suppressed: 424 bytes in 1 blocks
--18744-- used_suppression:      1 musl-dynlink2 /usr/libexec/valgrind/default.supp:587 suppressed: 48 bytes in 3 blocks
==18744== 
==18744== ERROR SUMMARY: 6 errors from 5 contexts (suppressed: 0 from 0)
```
