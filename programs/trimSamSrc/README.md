# Use:

Trims soft masked bases from a sam file. This is a
  command line program.

# Build/Install:

## Linux:

```
make -f mkfile.linux
sudo make -f mkfile.linux install
```

## Mac:

```
make -f mkfile.mac
sudo make -f mkfile.mac install
```

## Windows:

Install visual studio. Then open a developer console
   "start menu->visual studio->developer command prompt".
   Make sure you have administrator priveleges.

```
cd C:\path\to\trimSamSrc
make /F mkfile.win

# as administrator
move trimSam.exe C:\Windows\System32
```

I could not get the script to do an auto install. For some
  reason Windows does not like me doing this. Probably
  because I am moving to System32. Not best location, but
  it does not require messing around with enviromental
  variables.

# Run:

You can get the help message with `trimSam -h`. You can
  run trimSam using `trimSam -sam file.sam > out.sam`.
