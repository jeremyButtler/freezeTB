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

The first column has the lineage names (can by any
  non-space or tab). The remaing columns are the primer
  names.

Each primer name has 5 items. The primer name, the forward
  primers starting position on the reference, the length
  of the foward primer, the reverse primer ending position
  (first reference base), and the reverse primers length.
  Each of these entrys are separated with an `.`.

Example of an primer header entry:

- test.300.20.500.18
  - Name: "test"
  - Forward primer starts at base 300
  - Forward primer ends at base 320
    - Forward primer length is 20
  - Reverse primer starts at base 500
  - Reverse primer ends at base 482
    - Reverse primer length is 18
 
For The remaning part of the database goes lineage, then
  the lengths expected for each primer. The lengths should
  include the forward primer length in them, but not the
  reverse primer length. This is an artifact from
  MIRUReader.
