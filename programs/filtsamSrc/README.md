# Use:

Filters reads in sam file by flag, length, coordinates,
  mapping quailty, mean q-score, or median q-scores. The
  output can be a sam file (default), fastq file, fasta
  file, or tsv file (with read stats).

This can also trim softmasking off bases or maybe by
  coordinate.

Not quite as good as samtools in speed, but then you can
  also trim masked bases or get the number of snps,
  deltions, and insertions in a stats file output.

# License:

Dual licensed under public domain or MIT. Pick the one
  that works best for you.

# Install:

To build filtsam, you will use `make -f` with the make
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

# Running:

To get the help message do `filtsam -h`.

To filter unmapped
  reads `filtsam -F 4 -sam reads.sam > out.sam`.

Change file output

- Get stats `filtsam -out-stats -sam reads.sam > out.tsv`.
- Get fastq `filtsam -out-fastq -sam reads.sam > out.tsv`.
- Get fasta `filtsam -out-fasta -sam reads.sam > out.tsv`.
