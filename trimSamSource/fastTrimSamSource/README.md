# Reason this exists

This version of trimSam uses my old samEntry structs,
  which was faster at read in an output, but was also
  less easy to work with. It would read the entry samEntry
  into a single buffer and then setup pointers, which
  resulted in any modification requiring a complete recopy
  of then entry.

Having to re-copy the buffer was a pain, so I changed how
  samEntry works. It now has a separate buffer for all
  eleven required sam entries and a separate buffer for
  all of the extra entries.

# Install:

```
make
sudo make install
```

# Use:

## Help message

`fastTrimSam -h`

## Trim a sam file

`fastTrimSam -sam file.sam > trimmed-file.sam`

Or

`minimap2 -a ref.fa reads.fq | fastTrimSam -sam - > trimmed-file.sam`
