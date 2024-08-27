# Use:

Mask primer positions in an sam file using the primer
  coordaintes. The Q-score entry and cigar entries are
  not touched. Nor are indels removed.

# License:

Dual licensed under public domain or MIT. Pick the one
  that works best for you.

# Install

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

# Run

Help message: `maskPrim -h`

Version date (number): `maskPrim -v`

Run: `maskPrim -sam file.sam -prim primer-coordinates.tsv > out.sam`

# Input:

- sam file to mask `-sam file.sam`
- tsv or space deliminated file with primer coordinates to
  mask `-prim primer-coordinates.tsv`
  - The first row is ignored (header)
  - Maximum line length is 1023 characters
  - Column one is for user use only (ignored)
  - Column two is for user use only (ignored)
  - Column three tells if primers are "paired". This
    requires both primers to have at least one base
    present to mask. So, only one primers means no
    masking both primers means mask both regions.
    - 1, P, T, or + means the primers are an pair. Only
      the first character is checked, so you could use
      True, Paired, 1=paired, or anything else.
    - 0 means primers are not paired, though any value
      could be put here so long as it does not start with
      an 1, P, T, or +.
  - Column four foward primer starting position
    - use "NA" or 'N' for no primer
  - Column five foward primer ending position
  - Column six reverse primer starting position
    - use "NA" or 'N' for no primer
  - Column seven reverse primer ending position
- Optional: file to output to `-out output-file.sam`
- Optional: maximum number of nucleotides allowed before
  an foward primer or after an reverse primer to mask
  `-fudge -1`. Here to prevent primer sites in middle of
  sequences are not masked.
  - -1 is do not apply
- Optional: filter unmasked sequences `-filter`

This is what an space deliminated primer coordiantes
  table could look like. A + is the sam as an 1, P, or T.

```
reference   name    paired   start   end    start   end
test        tile-1    1      0       20     500     520
test        tile-2    1      350     370    850     870
test        tile-3    1      700     720    1300    1320
test        tile-4    1      1150    1170   1650    1670
test        region    0      1671    2999   NA      NA
test        region    0      NA      NA     1000    2000
test        dual      0      3000    3020   4000    4020
```

see miru-coords.tsv for another example. These are the
  coordinates for the MIRU-VNTR primers on NC000962.3
  (H32rv).
