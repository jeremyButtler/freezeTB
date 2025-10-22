# Use:

freezeTB is a program that detects gene mutations
  associated with drug resistance from nanopore sequenced
  tuberculosis samples. It also outputs a set of
  consensus fragments (as a sam file) and checks for
  lineages by spoligotyping and/or MIRU-VNTR.

This is the command line version of freezeTB.

# License

All code that is unique to freezeTB (not from other
  peoples code) is under a dual license. The primary
  license is Unlicense. However, not all
  companies/countries like or will even respect the
  Unlicense. So, in those cases or cases were the
  Unlicense is not wanted/allowed, the freezeTB unique
  code is under the MIT license.

# Install

## Unix

For any unix OS (Mac/Linux/BSD) do:

```
git clone \
   https://github.com/jeremyButtler/freezeTB \
   "${HOME}/Downloads/freezeTB;
cd "${HOME}/Downloads/freezeTB/programs/freezeTBStr;
make -f mkfile.unix;
```

To install globally do `sudo make -f mkfile.unix install`.

To install locally
   do `make -f mkfile.unix PREFIX="${HOME}/bin" install;`

After installation put the `freezeTBFiles` directory
  (folder) in your Documents folder.

```
cp \
    -r \
    "${HOME}/Downloads/freezeTB/freezeTBFiles \
    "${HOME}/Documents;
chmod -R a+r "${HOME}/Documents/freezeTBFiles";
```

## Windows

Do not, unless you want to deal with Windows command line
  pain.

- Clone and extract the freezeTB git hub to Downloads
- You will need the visual studio build tools to get the
  developer terminal.
- Open an x64 terminal
- Cd into the `programs/freezeTBSrc` in the freezeTB
  github
  - the most likely path is `cd "%HOMEPATH%\Downloads\freezeTB-main\freezeTB-main\programs\freezeTBStr`
- Do `nmake /F mkfile.win`, then put the `freezeTB.exe`
  file in a good location.
- Copy the `freezeTBFiles` to you Documents, or in the
  same directory (folder) as `freezeTB.exe`.

The internal read mapper for freezeTB is usable, but slow,
  and not as sensitive (it will miss stuff) as minimap2. I
  would recommend you install minimap2. For compiling
  minimap2 on windows see
  [https://github-wiki-see.page/m/genotoul-bioinfo/dgenies/wiki/Compile-minimap2-for-Windows](
   https://github-wiki-see.page/m/genotoul-bioinfo/dgenies/wiki/Compile-minimap2-for-Windows).
  Once complited copy the minimap2.exe and the two
  needed `.dll` files to a good location.

# Using:

You can get the help message with `freezeTB -h`.

The recommended way is with minimap2.

```
mkdir good_name; # pick a good name to call the project
minimap2 \
    -a \
    "${HOME}/Documents/freezeTBFiles/NC000962.fa \
    /path/to/fastq_pass/barcode0x/* \
  > good_name/good_name.sam;
freezeTB \
    -sam good_name/good_name.sam \
    -prefix good_name/good_name-FTB;
```

However, if for some odd reason you do not want to use
  minimap2, you can use the slower and less sensitive
  internal read mapper:

```
mkdir good_name; # pick a good name to call the project
freezeTB \
    -prefix good_name/good_name-FTB \
    /path/to/fastq_pass/barcode0x/*;
```
