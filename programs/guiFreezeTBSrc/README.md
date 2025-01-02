# Use:

Gui program for freezeTB

# Install:

## Linux:

1. install tcltk devel files
   - `sudo apt-get install tcl-dev`
   - `sudo apt-get install tk-dev`
2. install R (if not on system)
   - `sudo apt-get install r-base;`
3. install minimap2 (if no on system)
   - `sudo apt-get install minimap2;`
4. follow install steps and pray the make script can
   lock onto the tcltk libraries

```
git clone https://github.com/jeremybuttler/freezeTB;
cd freezeTB/programs/guiFreezeTBSrc;
make -f mkfile.unix;
sudo cp ../../scripts/graphAmpDepth.r /usr/local/bin;
sudo chmod a+x /usr/local/bin/graphAmpDepth.r;
sudo cp ../../freezeTBFiles /usr/local/share;
sudo chmod -R a+r /usr/local/share/freezeTBFiles;
sudo chmod a+r /usr/local/share/freezeTBFiles/*;
sudo chmod a+rx /usr/local/share/freezeTBFiles/gui-FTB.tcl;
```

copy the guiFreezeTB binarary (program) to your desired
  location. Or install
  with `sudo make -f mkfile.unix install` and make a
  shortcut to /usr/local/bin/guiFreezeTB (no idea how to
  do this).

## Mac:

1. install homebrew (visit website)
2. install xquarz `brew install xquartz`
3. install tcltk `brew install tcl-tk`
4. install R (if missing) `brew install r`
5. install minimap2 (if missing) `brew install minimap2`
6. install freezeTB

```
if [[ ! -d "/usr/local/bin" ]]; then
   mkdir -p "/usr/local/bin";
fi

git clone https://github.com/jeremybuttler/freezeTB;
cd freezeTB/programs/guiFreezeTBSrc;
make -f mkfile.mac;
sudo cp ../../scripts/graphAmpDepth.r /usr/local/bin;
sudo chmod a+x /usr/local/bin/graphAmpDepth.r;
sudo cp ../../freezeTBFiles /usr/local/share;
sudo chmod -R a+r /usr/local/share/freezeTBFiles;
sudo chmod a+r /usr/local/share/freezeTBFiles/*;
sudo chmod a+rx /usr/local/share/freezeTBFiles/gui-FTB.tcl;
```

copy the guiFreezeTB binarary (program) to your desired
  location. Or install
  with `sudo make -f mkfile.mac install` and make a
  shortcut to /usr/local/bin/guiFreezeTB (no idea how to
  do this).

## Windows; no idea

# Running:

Double click guiFreezeTB (not guiFreezeTB.c). Fill in
  the required input options (fastq file(s)).
