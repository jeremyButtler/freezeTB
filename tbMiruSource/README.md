# Use:

tbMIRU is used to detect MIRU lineages in TB samples.

# Install:

## Linux:

```
make
sudo make install
```

## Mac:

```
make mac
sudo make install
```

Copy miruTbl.tsv a convient spot. You will need it for
  miruTbl.tsv.

## Windows:

I am going to say no. But, in theory this should be
  possible.

# How to build a database:

## The format

The first column has the lineage names (can by any
  non-space or tab). The remaing columns are the primer
  names.

Each primer name has 5 items. The primer name, the forward
  primers starting position on the reference, the length
  of the foward primer, the reverse primer ending position
  (first reference base), and the reverse primers length.
  Each of these entrys are separated with an `.`. To be
  honest only the starting and ending positions are used.
  However, I do not want to change the read in function,
  so that is why the lengths are here.

Example of an primer header entry:

- test.300.20.500.18
  - Name: "test"
  - Forward primer starts at base 300
  - Forward primer ends at base 320
    - Forward primer length is 20
  - Reverse primer starts at base 500
  - Reverse primer ends at base 482
    - Reverse primer length is 18

## Add in postion data

You can add in the primer coordinates automatically with
  mkMiruTbl. It takes in an fasta file of primers `-prim`
  (make sure the forward and reverse primers are next to
  each other), your unannotated table `-miru-tbl`, and an
  reference sequence `-ref`. It is not the fasest program,
  but it works.

### Install mkMiruTbl (linux)


```
cd mkMiruTbl;
make;
sudo make install
```

### Install mkMiruTbl (MAC)


```
cd mkMiruTbl;
make mac;
sudo make install
```

### run mkMiruTbl

```
mkMiruTbl -h; # help message

# Build an table
mkMiruTbl -ref ref.fasta -miru-tbl miru-tbl.tsv -prim primers.fa > out-miru-tbl.tsv;
```

The mkMiruTbl/pre-miruTbl.tsv file is the is the MIRU
  table from MIRUReader. The primer fasta file is also in
  mkMiruTbl.

### Set up primer file for mkMiruTbl

Primer file example:

```
> Primer-1 foward primer
atgaatt
> Primer-1 reverse primer
gtatcat
> Primer-2 foward primer
ctgagtg
> Primer-2 reverse primer
agaccat
> Primer-3 foward primer
gggagtc
> Primer-3 reverse primer
cccccat
.
.
.
> Primer-n foward primer
ggaattc
> Primer-n reverse primer
acgatag
```
