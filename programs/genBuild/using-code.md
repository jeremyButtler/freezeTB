# Goal:

Provide some basic guidence on how to use build's
  libraries. Not very great, but better then nothing?

I typically try to maintain a table of file contents I
  call the SOF (Start Of File) at the top of each .c and
  .h file. You can reference that for a list of functions
  and a quick description. It is not the best resource,
  and may at times be misleading, but it is better than
  nothing.

To look up fuction in the SOF; search for `Fun#`, were
  `#` is the function number. Ex: to find `fun01` search
  for `Fun01`. To look up a structure use `ST#`. Ex: for
  `st01` use `ST01`.

Also, to look up a fucntion

- TOC:
  - [osST](#osST): get OS/CPU specs

# osST:

Is the OS detection part of build. It holds the specs_osST
  structure, which in turn holds the OS specs. One thing
  to remember is that for build an OS is not a single OS,
  but also includes a family of OS's. For example, Linux
  is an OS, but it is in the Unix family. So, osST will
  assign two OS's for Linux. The same holds true for
  SIMD instructions. If a cpu and OS can support avx512,
  then it can also support avx2, sse, and scalar
  (non-SIMD) instructions.
  
- specs_osST:
  - osAryStr: is a string pionter array, with all
    possible OS's 
  - simdAryStr: is a string pionter array to the various
    SIMD's that a system can support
  - cpuAryStr: has the cpu + cpu version string arrays
  - osBitUS: is cpu (or detected OS) bit size
  - endinUC: holds if system is big endin or little endin

## How works:

Checks compile flags for OS names and SIMD flags. So, not
  always best results. It also checks the size of an
  `long` to deterime if a 32 bit or 64 bit OS/CPU.  It
  uses casting to find if little or big endin system.

For plan9 this behaves a little diferently. The OS type
  is found using the objtype envrioment variable. The
  32 bit or 64 bit flags are also assigned based on the
  ojbtype. This is here because plan9 treats a `long` as
  a 32 bit object and `long long` as a 64 bit. I am not
  sure if a `long long` is 32 or 64 bits on a plan9 32
  bit system.

## using specs_osST:

### Making specs_osST struct:

#### Heap specs_osST structure:

You can make a specs_osST structure using the
  mk_specs_osST function (Fun12). This will auto get the
  OS type, cpu type, cpu bit, if have big or little endin,
  and the SIMD support (from complier flags or OS). It
  will be allocated on the heap. It returns
  def_memErr_osST for memory errors.

To free a heap allocated specs_osST structure use
  freeHeap_specs_osST function (Fun10).

Behind the scenes mk_specs_OsST is calling
  init_specs_osST (Fun09) and setup_specs_osST (Fun11) to
  setup. For the setup step, array memory is allocated
  and the get<spec>_osST functions are used to detect
  OS/cpu specs.

```
int main(){
   struct specs_osST *osSTPtr = 0;

   if(mk_specs_osST(osSTPtr))
      /*handel memory error*/

   /*your code goes here*/
   freeHeap_specs_osST(osSTPtr);
   osSTPtr = 0;
   return 0;
} /*main*/

#### Stack specs_osST structure:

You could skip mk_specs_osST to do a stack allocation
  instead. You will need to call init_specs_osST and
  setup_specs_osST on your new structure. Then when
  finished use freeStack_specs_osST (Fun09) instead of
  freeHeap_specs_osST (since is on stack, not heap) to
  free the variables in your structure.

```
int main(){
   struct specs_osST osStackST;
   init_specs_osST(&osStackST);
   
   if(setup_specs_osST(&osStackST))
      /*handel memory error*/
   
   /*your code goes here*/
   freeStack_specs_osST(&osStackST);
   return 0;
} /*main*/
```

### Adding OS/CPU flags:

#### Adding flags:

To add a spec by flag use addFlag_specs_osST (Fun18). It
  takes in a c-string with the flag and a c-string with
  the value of the flag. It also takes in a pointer to
  a signed int to incurment based on if used value (2) or
  not (1). The interger is so you can loop through all
  values in `main(int argc, char *argv[])`.

Here is an example of getting user input

```
int main(int argc, char *argv[]) {
   signed char errorSC = 0;
   signed int siArg = 1;
   struct specs_osST *osSTPtr = mk_specs_osST();

   if(! osSTPtr)
      goto err_main;

   while(siArg < argc)
   { /*Loop: get user input*/
      if(siArg + 1 < argc)
         errorSC =
            addFlag_specs_osST(
               osSTPtr, argv[siArg], argv[siArg + 1],
               &siArg
            );
      else
         errorSC =
            addFlag_specs_osST(
               osSTPtr, argv[siArg], 0, &siArg
            );

      if(errorSC)
      { /*If: had error*/
         if(errorSC & def_badFlag_osST)
            /*handle flag error, maybe check next input
            ` function?
            */
         else
            goto err_main;
      } /*If: had error*/
   } /*Loop: get user input*/

   /*code for using osST goes here*/

   freeHeap_specs_osST(osSTPtr);
   osSTPtr = 0;
   return 0;

   err_main:;
      if(osSTPtr)
         freeHeap_specs_osST(osSTPtr);
      osSTPtr = 0;
      return 1;
} /*main*/
```

#### Printniing flag help message:

To print the help message for addFlag_specs_osST use
  phelp_specs_osST (Fun19). You will need to provide
  a file to print to (stdout is good) and an indent
  level for the help message (I like two spaces `"  "`).

Beware this help message is long.
  
## specs_osST values:

### specs_osST single values:

The osBitUS and endinUC are only set to on value, and
  use 0 for anything.

| cpu bit | flag   |
|:--------|:-------|
|  8 bit\*| -8bit  |
| 16 bit\*| -16bit |
| 32 bit  | -32bit |
| 64 bit  | -64bit |

Table: cpu bit flags, \*: note 8 and 16 bit not listed
  in help message because programs (expecially build)
  would very likely fail.


| cpu endin | defined variable     | flag          |
|:----------|:---------------------|:--------------|
| bit       | def_bigEndIn_osST    | -big-endin    |
| little    | def_littleEndIn_osST | -little-endin |

Table: cpu endin values

### osAryST:

As a rule, osST fills in the group OS level first.
  Followed by the subgroup and variant OS. The final
  OS (all) is always `all`. Which refers to any OS.

| OS level | type    | osAryStr index   | flag      |
|:---------|:--------|:-----------------|:----------|
| variant  | Andriod | def_varOS_osST   | -os-var   |
| subgroup | Linux   | def_subOS_osST   | -os-sub   |
| group    | Unix    | def_groupOS_osST | -os-group |
| default  | Any     | def_allOS_osST   | none      |

Table: list of build OS's for Andriod.

| OS level | type    | osAryStr index   | flag      |
|:---------|:--------|:-----------------|:----------|
| variant  | none    | def_varOS_osST   | -os-var   |
| subgroup | none    | def_subOS_osST   | -os-sub   |
| group    | Windows | def_groupOS_osST | -os-group |
| default  | Any     | def_allOS_osST   | none      |

Table: list of build OS's for Windows

### simdAryST:

As a rule, osST fills in the 0th level (priority) value
  with the most specific/fastest SIMD support. It then
  fills in the lower priorities (1-5) with less fast
  SIMD support. The final level is always `scalar`, which
  means no SIMD support.

| SIMD level | type   | osAryStr index        | flag    |
|:-----------|:-------|:----------------------|:--------|
| first      | avx512 | def_0thSIMD_osST      | -simd-0 |
| second     | avx2   | def_1stSIMD_osST      | -simd-1 |
| third      | sse4   | def_2ndSIMD_osST      | -simd-2 |
| fourth     | sse3   | def_3rdSIMD_osST      | -simd-3 |
| fith       | sse2   | def_4thSIMD_osST      | -simd-4 |
| six        | sse    | def_5thSIMD_osST      | -simd-5 |
| default    | scalar | def_def_lastSIMD_osST | none    |

Table: list of simd levels for AVX512

| SIMD level | type   | osAryStr index        | flag    |
|:-----------|:-------|:----------------------|:--------|
| first      | neon64 | def_0thSIMD_osST      | -simd-0 |
| second     | neon   | def_1stSIMD_osST      | -simd-1 |
| third      | none   | def_2ndSIMD_osST      | -simd-2 |
| fourth     | none   | def_3rdSIMD_osST      | -simd-3 |
| fith       | none   | def_4thSIMD_osST      | -simd-4 |
| six        | none   | def_5thSIMD_osST      | -simd-5 |
| default    | scalar | def_def_lastSIMD_osST | none    |

Table: list of simd levels for ARM NEON

### cpuAryST:

As a rule, osST fills out the CPU type first and for ARM
  CPUs will add a version number if detected. The last
  cpu type, is always `all` for all cpus.

| CPU level | type | osAryStr index   | flag      |
|:----------|:-----|:-----------------|:----------|
| version   | ARM8 | def_cpuVer_osST  | -cpu-ver  |
| type      | ARM  | def_cpuType_osST | -cpu-type |
| last      | none | def_lastCp_osST  | none      |

Table: list of simd levels for ARM NEON
