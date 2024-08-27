# Use:

Adds primer coordinates to the MIRU table used with
  tbMiru.

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

# Run:

You can get the help message with `mkMiruTbl -h`.

For running mkMiruTbl you will need three files, the
  reference you wish to update the table for, the
  primers in the table, and the table to add primer
  coordinates to. Output is the modified (only headers
  modifed) table to stdout (terminal).

```
mkMiruTbl -ref reference.fasta -prim primers.fasta -miru-tbl miru-table.tsv > modifed-table.tsv
```

## Set up primer file for mkMiruTbl

The primers should be in the same order as the header in
  the MIRU table.

Header: `No.	primer-1	primer-2	primer-2	...	primer-n`

primer fasta file:

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
