# Use:

Make or adds AMR entries to an tbAmr database.

Be aware, this is only for simple variants. It can not
  handle ids with multiple variants. Or even some of
  the more complex ids. It should be able to handle
  everything seen in the 2023 WHO catalog.

# Install:

## Unix:

```
if [ ! -f /usr/local/bin ]; then
   sudo mkdir /usr/local/bin;
fi;

cd ~/Downloads;

if [ ! -d freezeTB ]; then
   git clone https://github.com/jeremyButtler/freezeTB freezeTB;
fi;

cd freezeTB/programs/addAmrSrc;
make -f mkfile.unix;
sudo make -f mkfile.unix install
```

# Using addAmr

## intro and usage

You should be able to use addAmr to create a database for
  getAmr. This should work and the output looks good.
  However, I have not run a database thourgh getAmr yet.

The required input is a reference sequence, the
  coordinates for each gene (see ampDepth or `addAmr -h`),
  and a variants file with variants to convert.

You can get the help message with `addAmr -h`.

For addAmr, you can run:

```
addAmr \
    -ref reference.fasta \
    -coords coordinates.tsv \
    -var variants.tsv \
  > database.tsv`;
```

## variants file

You can create a database using addAmr using the variant
  ids from a file. The variant file has three required
  entries, the variant id, the reference id (`-ref <id>`),
  and any drug resistance. You can also add a comment with
  `-note <comment>`.

Here is the generalized example of the variant file.

```
variant_id	-ref	reference_id	-other	drug_name	-note	comment
```

You can see test.txt for some examples.

```
gyrB_p.Ser447Phe	-ref	NC_000962.3	-lfx	-note	levovloxcin_shorthand_flag
fgd1_LoF	-ref	NC_000962.3	-dlm	-note	delamanid_shorthand_flag
fgd1_p.Asn112fs	-ref	NC_000962.3	-other	delamanid	-note	and_drug_(this_is_delamanid)_flag
```

Each variant ID should start out with the gene name,
  followed by an underscore (`<gene_>`). The next part
  tells if the gene is coding (`<gene>_c.`), nucleotide
  (`<gene>_n.`), protein (`<gene>_p.`), gene loss of
  function (`<gene>_LoF`), or the entire gene was deleted
  deletion (`<gene>_deletion`). The last part tells the
  mutation type.

- General variant id examples:
  - Gene Loss of Function: `fgd1_LoF`
  - Gene deletion: `katG_deletion`


For nucleotide mutations, use position, reference, the
  symbol, and mutation. For a SNP it would look like
  `rrl_n.2270G>C`. For an insertion it would look like
  `eis_c.-8delc`. For an insertion you need to included
  the two bases the insertion, `ins` and then the base.
  For example, `rrl_n.2269_2270insT`.

- Nucleotide variant id examples:
  - SNP: `rrl_n.2270G>C`
  - insertion: `rrl_n.2269_2270insT`
  - multi-base insertion: `rrl_n.2269_2270insTAGG`
  - deletion `eis_c.-8delc`

For amino acid mutations, use the reference sequence,
  position, then mutation. The mutation can be `dup` for
  duplicates, `del` for deletion, `ins<AA>` for and
  insertion, `fs` for a frameshift, and an amino acid for
  an SNP mutation `<AA>`.

You can also use `?` for any amino acid and `*` for a
  stop mutation. You can use `ext*?` to get loss of
  stop codon. All start codons are treated as `Met`.

- Amino acid variant id examples:
  - SNP: `gyrB_p.Ser447Phe`
  - SNP: to stop: `Rv0678_p.Gln22*`
  - SNP: loss of stop: `pncA_p.Ter187Argext*?`
  - SNP: replace start with anything `Rv0678_p.Met1?`
  - deletion: `rpoB_p.Asn437_Asn438del`
  - insertion: `rpoB_p.Ser431_Gln432insArg`
  - frame shift: `Rv0678_p.Asp8fs`
  - duplication: `rpoB_p.Phe433dup`
  - larger duplication: `rpoB_p.Phe433_440dup`
    - note tested

## flags for drugs

addAmr has some flags for some drugs. So, you do not
  always need to use the `-other drug` flag.

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

## other notes

You can also use `-gene <gene>` to provided the gene the
  varaint is in. Only use this if the gene is not in the
  variant id (ex: you have `p.Lys23Gly` instead
  of `katG_p.Lys23Gly`).

Here is an example of a line from a variant
  file `rrl_n.2270G>C -lzd -note forward_gene`.

Here is the variant file used to test this program.

```
gyrB_p.Ser447Phe	-ref NC_000962.3	-lfx -note forward

fgd1_LoF -ref NC_000962.3 -dlm -note forward
fgd1_p.Asn112fs -ref NC_000962.3 -other delamanid -note forward

rpoB_p.Gln432_Met434del	-ref NC_000962.3	-rif -note forward
rpoB_p.Gln432_Met434delinsLeu -ref NC_000962.3  -rif -note forward
rpoB_p.Ser431_Gln432insArg -ref NC_000962.3  -other rifampicin -note forward
rpoB_p.Asn437_Asn438del -ref NC_000962.3  -rif -note forward
Rv0678_p.Met1?	-ref NC_000962.3	-bdq -needs MmpL5 -note forward
Rv0678_p.Asp8fs	-ref NC_000962.3	-cfz	-needs MmpL5	-note forward
Rv0678_p.Gln22* -ref NC_000962.3  -bdq -needs MmpL5 -note forward
rpoB_p.Phe433dup	-ref NC_000962.3	-grade-1 -note forward

katG_LoF	-ref NC_000962.3	-inh -high-res -note reverse
katG_p.Pro569fs	-ref NC_000962.3	-inh	-high-res -note reverse
katG_p.Trp689*	-ref NC_000962.3	-inh -high-res -note reverse
katG_p.Trp328Leu -ref NC_000962.3  -inh	-grade-1 -high-res -note reverse
katG_deletion -ref NC_000962.3  -inh -high-res -note reverse
katG_p.Met1?	-ref NC_000962.3	-grade-1 -inh -high-res -note reverse

pncA_p.Ter187Argext*? -ref NC_000962.3  -pza -note reverse
pncA_c.-11A>G -ref NC_000962.3  -pza -grade-1 -note reverse_gene

rrl_n.2269_2270insT -ref NC_000962.3  -lzd -note forward_gene
rrl_n.2270G>C -ref NC_000962.3 -lzd -note forward_gene
eis_c.-8delC -ref NC_000962.3 -kan -note reverse_gene

inhA_c.-154G>A -ref NC_000962.3 -eto -note forward_gene
inhA_c.-154G>A -ref NC_000962.3 -inh -low-res -add-res -grade-1 -note low_res
```
