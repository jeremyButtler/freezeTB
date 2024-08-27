# Use:

Swaps references for a tbAmr database (freezeTB uses). All
  that is change is the reference coordinates. The AMR and
  orginal reference sequences (nucleotide/amino acid) are
  unchanged.

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

# Run

Get the help message with `swapDbRef -h`.

Get a sam file to use in changing the database with
  minimap2 [https://github.com/lh3/minimap2](
  https://github.com/lh3/minimap2) `minimap2 -a NC000963.fa new-ref.fa > new-ref.sam`.

Change references for the database
  database `swapDbRef -amr old-database.tsv -ref new-ref.sam > new-database.tsv`.
