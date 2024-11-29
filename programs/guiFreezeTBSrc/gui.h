/*For GUI scripts I am using --- to mark the starts and
`   ends of blocks. I then use str_rmBlocks to remove the
`   block markers and convert the blocks single line. It
`   is a bit inefficent, but makes my tlctk life so much
`   more sain.
*/

static signed char *
   glob_guiScript_gui =
   (signed char *)
   "\n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
   # gui SOF: Start Of File                            \n\
   #   - freezeTB gui in tcltk (copy to guiFreezeTB.c) \n\
   #   o header:                                       \n\
   #     - global variables                            \n\
   #   o tcl01: glob_tcl_isInt_gui                     \n\
   #     - checks if input key is numeric and if string\n\
   #       is between max and min                      \n\
   #   o gui01:                                        \n\
   #     - set up main frame for tcltk                 \n\
   #   o gui02:                                        \n\
   #     - required input gui                          \n\
   #   o gui03:                                        \n\
   #     - menu bar for gui                            \n\
   #   o gui04:                                        \n\
   #     - read filtering menu                         \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
                                                       \n\
   #-------------------------------------------------- \n\
   # Header:                                           \n\
   #   - global variables                              \n\
   #   o header sec01:                                 \n\
   #     - global variables (not modified by C)        \n\
   #   o header sec02:                                 \n\
   #     - program paths                               \n\
   #   o header sec03:                                 \n\
   #     - global variables modified by C (remove)     \n\
   #   o header sec04:                                 \n\
   #     - file type lists                             \n\
   #-------------------------------------------------- \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Header Sec01:                                     \n\
   #   - global variables (not modified by C)          \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   # required input                                    \n\
   variable glob_fqIn \"\" ;                           \n\
   variable glob_dirOut \"\" ;                         \n\
   variable glob_prefix \"Hufflepuff\" ;               \n\
                                                       \n\
                                                       \n\
   # read filtering settings                           \n\
   variable glob_mask 0 ;    # mask primer sites       \n\
   variable glob_adjust 0 ;  # adjust mapp coordinates \n\
   variable glob_srCheck \"map-ont\" ; # map setting   \n\
                                                       \n\
   # amr setting                                       \n\
   variable glob_frameshift 0 ; # AMR frameshift scan  \n\
                                                       \n\
   variable glob_clustBl 0 ;                           \n\
   variable glob_depthProfBl 0 ;                       \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Header Sec02:                                     \n\
   #   - program paths                                 \n\
   #   o header sec02 sub01:                           \n\
   #     - windows program detection                   \n\
   #   o header sec02 sub02:                           \n\
   #     - unix program detection                      \n\
   #   o header sec02 sub03:                           \n\
   #     - check/get Rscript version                   \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   #***************************************************\n\
   # Header Sec02 Sub01:                               \n\
   #   - windows program detection                     \n\
   #   o header sec02 sub01 cat01:                     \n\
   #     - windows general setup                       \n\
   #   o header sec02 sub01 cat02:                     \n\
   #     - windows detect minimap2                     \n\
   #   o header sec02 sub01 cat03:                     \n\
   #     - windows detect Rscript                      \n\
   #***************************************************\n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Header Sec02 Sub01 Cat01:                         \n\
   #   - windows general setup                         \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   variable mapPath \"\" ; # minimap2 path             \n\
   variable rPath \"\" ;   # Rscript path              \n\
                                                       \n\
   if { [lindex $tcl_platform(os) 0] eq \"Windows\" } {\n\
      # this is windows                                \n\
                                                       \n\
      package require registry ;                       \n\
         # registry is a base tcl package for windows  \n\
         # registry manipulations                      \n\
                                                       \n\
      set programFiles $::env(\"Program Files\") ;     \n\
                                                       \n\
      #++++++++++++++++++++++++++++++++++++++++++++++++\n\
      # Header Sec02 Sub01 Cat02:                      \n\
      #   - windows detect minimap2                    \n\
      #++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
      set mapPath \"minimap2.exe\" ;                   \n\
                                                       \n\
      ---set                                           \n\
         status                                        \n\
         [catch                                        \n\
          {exec minimap2.exe --version} mapVersion     \n\
         ]                                             \n\
      --- ; # get minimap2 version                     \n\
                                                       \n\
      if { $status eq 0 } {                            \n\
         # got minimap2 version, nothing else to do    \n\
      } else {                                         \n\
         ---set mapPath                                \n\
            [file join                                 \n\
               $programFiles                           \n\
               \"freezeTB\"                            \n\
               \"minimap2.exe\"                        \n\
            ]                                          \n\
         ---; # build alternate minimap2 path          \n\
                                                       \n\
         ---set                                        \n\
            status                                     \n\
            [catch                                     \n\
             {exec $mapPath --version} mapVersion      \n\
            ]                                          \n\
         --- ; # get minimap2 version                  \n\
                                                       \n\
         if { $status eq 0 } {                         \n\
            # found minmap2; nothing else to do        \n\
         } else {                                      \n\
            ---tk_messageBox                           \n\
               -message \"Could not find minimap2.exe\"\n\
               -title \"ERROR\"                        \n\
            ---;                                       \n\
         } ; # If: check if could not find minimap2    \n\
      } ; # find minimap2 path                         \n\
                                                       \n\
      #++++++++++++++++++++++++++++++++++++++++++++++++\n\
      # Header Sec02 Sub01 Cat03:                      \n\
      #   - windows detect Rscript                     \n\
      #++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
      # find path to                                   \n\
      # the keys came from https://stackoverflow.com/questions/62427584/find-rscript-exe-on-windows\n\
                                                       \n\
      # NEED to try this on a windows machine          \n\
      ---registry                                      \n\
         get                                           \n\
         \"HKCU\\Software\\Classes\\RWorkspace\\Shell\\Open\\Command\"\n\
         \"Rscript.exe\"                               \n\
      ---; # see if can find Rscript.exe in .RData     \n\
                                                       \n\
      ---registry                                      \n\
         get                                           \n\
         \"HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\"\n\
         \"Rscript.exe\"                               \n\
      ---; # see if can find Rscript.exe in Uninstall  \n\
                                                       \n\
      # last resort; try to find Rscript.exe in its    \n\
      # expected location                              \n\
      ---set                                           \n\
        rPath                                          \n\
        [file join                                     \n\
           $programFiles                               \n\
           \"R\"                                       \n\
           \"*\"                                       \n\
           \"bin\"                                     \n\
           \"Rscript.exe\"                             \n\
        ] # build Rscript search path                  \n\
      ---;                                             \n\
                                                       \n\
      if { [llength $rPath] eq 0 } {                   \n\
         ---tk_messageBox                              \n\
            -message \"Rscript.exe not found\"         \n\
            -title \"ERROR\"                           \n\
         ---;                                          \n\
                                                       \n\
         return false ;                                \n\
      } else {                                         \n\
         set rPath [lindex $rPath 0] ;                 \n\
         # make sure have only one Rscript.exe         \n\
      } ; # See if Rscript was finally found           \n\
                                                       \n\
   #***************************************************\n\
   # Header Sec02 Sub02:                               \n\
   #   - unix program detection                        \n\
   #***************************************************\n\
                                                       \n\
   } else {                                            \n\
      set mapPath \"minimap2\" ;                       \n\
      set rPath \"Rscript\" ; # linux should be in path\n\
                                                       \n\
      ---set                                           \n\
         status                                        \n\
         [catch {exec $mapPath --version} mapVersion ] \n\
      --- ; # seeing if minimap2 exists                \n\
                                                       \n\
      if { $status ne 0 } {                            \n\
         set mapPath \"./minimap2\" ;                  \n\
                                                       \n\
         ---set                                        \n\
            status                                     \n\
            [catch                                     \n\
                {exec $mapPath --version}              \n\
                mapVersion                             \n\
            ]                                          \n\
         --- ; # seeing if local minimap2 exists       \n\
                                                       \n\
         if { $status eq 0 } {                         \n\
            ---tk_messageBox                           \n\
               -message \"minimap2 not found\"         \n\
               -title \"ERROR\"                        \n\
            ---;                                       \n\
                                                       \n\
            return false ;                             \n\
         } ; # If: minimap2 could not be found         \n\
      } ; # If: minimiap2 not in path or on system     \n\
   } ; # check if windows or linux                     \n\
                                                       \n\
   #***************************************************\n\
   # Header Sec02 Sub03:                               \n\
   #   - check/get Rscript version                     \n\
   #***************************************************\n\
                                                       \n\
   ---set                                              \n\
      status                                           \n\
      [catch {exec $rPath --version} rVersion]         \n\
   ---;                                                \n\
                                                       \n\
   if { $status eq 0 } {                               \n\
   } else {                                            \n\
      ---tk_messageBox                                 \n\
         -message \"Unable to run Rscript\"            \n\
         -title \"ERROR\"                              \n\
      ---;                                             \n\
                                                       \n\
      return false;                                    \n\
   }                                                   \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Header Sec03:                                     \n\
   #   - global variables modified by C (remove)       \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
                                                       \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Header Sec04:                                     \n\
   #   - file type lists                               \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   # file types                                        \n\
                                                       \n\
   set fq_types {                                      \n\
      { {fastq}    {.fastq}    }                       \n\
      { {fastq}    {.fq}       }                       \n\
      { {fastq.gz} {.fastq.gz} }                       \n\
      { {fastq.gz} {.fq.gz}    }                       \n\
   } ; # fastq file exentions                          \n\
                                                       \n\
   set fa_types {                                      \n\
      { {fasta}    {.fasta}    }                       \n\
      { {fasta}    {.fa}       }                       \n\
   } ; # fastq file exentions                          \n\
                                                       \n\
   set tsv_types {                                     \n\
      { {tsv} {.tsv} }                                 \n\
   } ; # fastq file exentions                          \n\
                                                       \n\
   set csv_types {                                     \n\
      { {csv} {.csv} }                                 \n\
   } ; # fastq file exentions                          \n\
                                                       \n\
   #-------------------------------------------------- \n\
   # Tcl01: tcl_isInt_gui                              \n\
   #   - checks if input key is numeric and if string  \n\
   #     is between max and min                        \n\
   # Input:                                            \n\
   #   - value:                                        \n\
   #     o value (%P) in entery box to check           \n\
   #   - index:                                        \n\
   #     o index of character to check (-1 for end)    \n\
   #   - min:                                          \n\
   #     o minimum value for value to be a match       \n\
   #   - max:                                          \n\
   #     o maximum value for value to be a match       \n\
   # Output:                                           \n\
   #   - Returns:                                      \n\
   #     o true if last character in value is numeric  \n\
   #       and is between min and max                  \n\
   #     o false if non-numeric or not min/max range   \n\
   #-------------------------------------------------- \n\
   proc tcl_isInt_gui { value index min max } {        \n\
      if { $index < 0 } {                              \n\
         set charIn [string index $value \"end\"] ;    \n\
      } else {                                         \n\
         set charIn [string index $value $index] ;     \n\
      } ; # Else: index of character input             \n\
                                                       \n\
      if { $value eq \"\" } {                          \n\
         return true ;         # empty string          \n\
                                                       \n\
      } elseif { [string is integer $charIn] } {       \n\
         if { $value > $max } {                        \n\
            return false ;                             \n\
         } elseif { $value < $min } {                  \n\
            return false ;      # outside min range    \n\
         } else {                                      \n\
            return true ;       # valid number         \n\
         } ; # Else If: new character is integer       \n\
      } else {                                         \n\
         return false ;         # non-numeric          \n\
      } ;                                              \n\
                                                       \n\
      return true ;            # empty case            \n\
   } ; # tcl_isInt_gui                                 \n\
                                                       \n\
   #---------------------------------------------------\n\
   # Tcl02: tcl_isFloat_gui                            \n\
   #   - checks if input key is numeric and if string  \n\
   #     is between max and min                        \n\
   # Input:                                            \n\
   #   - value:                                        \n\
   #     o value (%P) in entery box to check           \n\
   #   - index:                                        \n\
   #     o index of character to check (-1 for end)    \n\
   #   - min:                                          \n\
   #     o minimum value for value to be a match       \n\
   #   - max:                                          \n\
   #     o maximum value for value to be a match       \n\
   # Output:                                           \n\
   #   - Returns:                                      \n\
   #     o true if last character in value is numeric  \n\
   #       and is between min and max                  \n\
   #     o false if non-numeric or not min/max range   \n\
   #-------------------------------------------------- \n\
   proc tcl_isFloat_gui { value index min max } {      \n\
      if { $index < 0 } {                              \n\
         set charIn [string index $value \"end\"] ;    \n\
      } else {                                         \n\
         set charIn [string index $value $index] ;     \n\
      } ; # Else: index of character input             \n\
                                                       \n\
      if { $value eq \"\" } {                          \n\
         return true ;             # empty string      \n\
                                                       \n\
      } elseif { $charIn eq \".\" } {                  \n\
         if {                                          \n\
            [regexp -all {[0-9]*\\.[0-9]*} $value] < 2 \n\
         } {                                           \n\
            return true ;   # first .                  \n\
         } else {                                      \n\
            return false ;   # second .                \n\
         } ; # decimal point input                     \n\
                                                       \n\
      } elseif { $value eq \".\" } {                   \n\
        return true ;        # only decimal point      \n\
                                                       \n\
      } elseif { [string is integer $charIn] } {       \n\
         if { $value > $max } {                        \n\
            return false ;      # outside max range    \n\
         } elseif { $value < $min } {                  \n\
            return false ;      # outside min range    \n\
         } else {                                      \n\
            return true ;       # valid number         \n\
         } ; # check if number is in range             \n\
      } else {                                         \n\
         return false ;         # non-numeric          \n\
      }                                                \n\
                                                       \n\
      return true ;                # empty case        \n\
   } ; # tcl_isFloat_gui                               \n\
                                                       \n\
   #-------------------------------------------------- \n\
   # Gui01:                                            \n\
   #   - set up main frame for tcltk                   \n\
   #---------------------------------------------------\n\
                                                       \n\
   wm title . \"Required freezeTB\"                    \n\
   tk::frame .main -borderwidth 2 ;                    \n\
   pack .main -side left ;                             \n\
                                                       \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\
   # Gui02 TOC:                                        \n\
   #   - required input gui                            \n\
   #   o gui02 sec01:                                  \n\
   #     - make required input frame                   \n\
   #   o gui02 sec02:                                  \n\
   #     - set up the fastq label/button               \n\
   #   o gui02 sec03:                                  \n\
   #     - set up output directory label/button        \n\
   #   o gui02 sec04:                                  \n\
   #     - set up prefix entry                         \n\
   #   o gui02 sec05:                                  \n\
   #     - set up run and exit button                  \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Gui02 Sec01:                                      \n\
   #   - make required input frame                     \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   tk::frame .main.reqIn -borderwidth 2 ;              \n\
   pack .main.reqIn -anchor w -side top ;              \n\
                                                       \n\
   tk::frame .main.reqIn.fq -borderwidth 2 ;           \n\
   pack .main.reqIn.fq -anchor w -side top ;           \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Gui02 Sec02:                                      \n\
   #   - set up the fastq label/button                 \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   tk::label .main.reqIn.fq.lab -text \"\" ;           \n\
                                                       \n\
   ---tk::button                                       \n\
       .main.reqIn.fq.but                              \n\
       -text \"fastq files\"                           \n\
       -command {                                      \n\
   ---                                                 \n\
           set fqTitle \"select fastq files\" ;        \n\
           upvar 0 glob_fqIn fqIn ; # make global var  \n\
                                                       \n\
           set fqFiles [                               \n\
                 ---tk_getOpenFile                     \n\
                    -multiple TRUE                     \n\
                    -title $fqTitle                    \n\
                    -filetypes $fq_types               \n\
                 ---                                   \n\
           ] ;                                         \n\
                                                       \n\
           if {$fqFiles eq \"\"} {                     \n\
                                                       \n\
           } elseif {[llength $fqFiles] == 1} {        \n\
              ---.main.reqIn.fq.lab                    \n\
                 configure                             \n\
                 -text $fqFiles ;                      \n\
              ---                                      \n\
                                                       \n\
             set fqIn $fqFiles ; # update list         \n\
           } else {                                    \n\
             ---.main.reqIn.fq.lab                     \n\
              configure                                \n\
              -text [file dirname [lindex $fqFiles 1] ]\n\
             ---;                                      \n\
                                                       \n\
             set fqIn $fqFiles ; # update list         \n\
           } ; # set up fastq label for input          \n\
   } ; # command button fires                          \n\
                                                       \n\
   ---pack                                             \n\
      .main.reqIn.fq.but                               \n\
      .main.reqIn.fq.lab                               \n\
      -side left ;                                     \n\
   ---                                                 \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Gui02 Sec03:                                      \n\
   #   - set up output directory label/button          \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   tk::frame .main.reqIn.out -borderwidth 2 ;          \n\
   pack .main.reqIn.out -anchor w -side top ;          \n\
                                                       \n\
   tk::label .main.reqIn.out.lab -text \"\" ;          \n\
                                                       \n\
   ---tk::button                                       \n\
         .main.reqIn.out.but                           \n\
         -text \"output directory\"                    \n\
         -command {                                    \n\
   ---                                                 \n\
            upvar 0 glob_dirOut dirOut ;               \n\
                                                       \n\
            set outDir [                               \n\
               ---tk_chooseDirectory                   \n\
                  -title \"select output directory\"   \n\
               ---                                     \n\
            ] ;                                        \n\
            if {$outDir ne \"\"} {                     \n\
               ---.main.reqIn.out.lab                  \n\
                  configure -text $outDir ;            \n\
               ---                                     \n\
                                                       \n\
               set dirOut $outDir ;                    \n\
            } ; # If: have output directory            \n\
      } ; # output directory button                    \n\
                                                       \n\
   ---pack                                             \n\
      .main.reqIn.out.but                              \n\
      .main.reqIn.out.lab                              \n\
      -side left                                       \n\
      -anchor w ;                                      \n\
   ---                                                 \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Gui02 Sec04:                                      \n\
   #   - set up prefix entry                           \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   tk::frame .main.reqIn.prefix -borderwidth 2 ;       \n\
   pack .main.reqIn.prefix -anchor w -side top ;       \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.reqIn.prefix.lab                           \n\
      -textvariable glob_prefix ;                      \n\
   ---                                                 \n\
                                                       \n\
   pack .main.reqIn.prefix.lab -anchor w -side top     \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Gui02 Sec05:                                      \n\
   #   - set up run and exit button                    \n\
   #   o gui02 sec05 sub01:                            \n\
   #     - set up frame for run and exit button        \n\
   #   o gui02 sec05 sub02:                            \n\
   #     - run button                                  \n\
   #   o gui02 sec05 sub03:                            \n\
   #     - exit button and pack (run/exit)             \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   #***************************************************\n\
   # Gui02 Sec05 Sub01:                                \n\
   #   - set up frame for run and exit button          \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.reqIn.runexit ;                     \n\
   pack .main.reqIn.runexit -anchor w -side top ;      \n\
                                                       \n\
   #***************************************************\n\
   # Gui02 Sec05 Sub02:                                \n\
   #   - run button                                    \n\
   #   o gui02 sec05 sub02 cat01:                      \n\
   #     - setup + fastq file and prefix checks        \n\
   #   o gui02 sec05 sub02 cat02:                      \n\
   #     - set up minimap2 output file name            \n\
   #   o gui02 sec05 sub02 cat03:                      \n\
   #     - run minimap2                                \n\
   #   o gui02 sec05 sub02 cat04:                      \n\
   #     - add freezeTB databases                      \n\
   #   o gui02 sec05 sub02 cat05:                      \n\
   #     - add freezeTB filtering settings             \n\
   #   o gui02 sec05 sub02 cat06:                      \n\
   #     - add freezeTB lineage settings               \n\
   #   o gui02 sec05 sub02 cat07:                      \n\
   #     - add freezeTB tbCon consensus settings       \n\
   #   o gui02 sec05 sub02 cat08:                      \n\
   #     - add freezeTB tbCon and AMR print settings   \n\
   #   o gui02 sec05 sub02 cat09:                      \n\
   #     - add freezeTB clustering settings            \n\
   #   o gui02 sec05 sub02 cat10:                      \n\
   #     - run freezeTB                                \n\
   #   o gui02 sec05 sub02 cat11:                      \n\
   #     - build graphs                                \n\
   #***************************************************\n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui02 Sec05 Sub02 Cat01:                          \n\
   #   - setup + fastq file and prefix checks          \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   proc setMapStatus {} {                              \n\
         ---.main.reqIn.runexit.statuslab              \n\
            configure                                  \n\
            -text \"running minimap2\"                 \n\
         ---;                                          \n\
                                                       \n\
         wm title . \"running minimap2 (freezeTB)\" ;  \n\
   } ; # sets running minimap2 status                  \n\
                                                       \n\
   proc setFreezeTBStatus {} {                         \n\
         ---.main.reqIn.runexit.statuslab              \n\
            configure                                  \n\
            -text \"running freezeTB\"                 \n\
         ---;                                          \n\
                                                       \n\
         wm title . \"running freezeTB\" ;             \n\
   } ; # sets running freezeTB stats                   \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.reqIn.runexit.runbut                       \n\
      -text \"run\"                                    \n\
      -command {                                       \n\
   ---                                                 \n\
         if { $glob_fqIn eq \"\" } {                   \n\
            .main.menu.reqBut invoke ;                 \n\
                                                       \n\
            ---tk_messageBox                           \n\
               -message \"no fastq files input\"       \n\
               -title \"ERROR\"                        \n\
            ---;                                       \n\
                                                       \n\
            return false ; # no fastq files            \n\
         } ; # no fastq file input                     \n\
                                                       \n\
         if { $glob_prefix eq \"\" } {                 \n\
            .main.menu.reqBut invoke ;                 \n\
                                                       \n\
            ---tk_messageBox                           \n\
               -message \"no prefix input\"            \n\
               -title \"ERROR\"                        \n\
            ---;                                       \n\
            return false;                              \n\
         } ; #  no prefix (was deleted)                \n\
                                                       \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
         # Gui02 Sec05 Sub02 Cat02:                    \n\
         #   - set up minimap2 output file name        \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         ---set                                        \n\
            prefix                                     \n\
            [ file join $glob_dirOut $glob_prefix ]    \n\
         ---; # build output file path*/               \n\
                                                       \n\
         file mkdir $prefix ;                          \n\
                                                       \n\
         if { [file isdirectory $prefix] eq 0 } {      \n\
            ---tk_messageBox                           \n\
               -message \"failed to make directory\"   \n\
               -title \"ERROR\"                        \n\
            ---;                                       \n\
                                                       \n\
           return false                                \n\
         } ; # failed to make output directory         \n\
                                                       \n\
         ---set                                        \n\
            prefix                                     \n\
            [file join $prefix $glob_prefix]           \n\
         ---;                                          \n\
                                                       \n\
         set samFile $prefix ;                         \n\
         append samFile \"-map.sam\" ;                 \n\
                                                       \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
         # Gui02 Sec05 Sub02 Cat03:                    \n\
         #   - run minimap2                            \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         setMapStatus ; # dispaly running minimap2     \n\
                                                       \n\
         ---.main.reqIn.runexit.statuslab              \n\
            configure                                  \n\
            -text \"running minimap2\"                 \n\
         ---;                                          \n\
                                                       \n\
         wm title . \"running minimap2 (freezeTB)\" ;  \n\
                                                       \n\
         # run minimap2 command                        \n\
         ---set                                        \n\
            status                                     \n\
            [catch                                     \n\
               {                                       \n\
                  eval        # expands lists          \n\
                  exec                                 \n\
                  $mapPath                             \n\
                  -a                                   \n\
                  -x                                   \n\
                  \\$glob_srCheck # \\ escapes eval    \n\
                  \\$glob_refFa   # \\ escapes eval    \n\
                  $glob_fqIn                           \n\
                  \">\"                                \n\
                  \\$samFile      # \\ escapes eval    \n\
               }                                       \n\
               result # output from program            \n\
            ]                                          \n\
         ---;                                          \n\
                                                       \n\
         if { [ string equal $::errorCode NONE ] } {   \n\
            # using errorCode here because minimap2    \n\
            # outputs to stderr                        \n\
         } else {                                      \n\
            ---tk_messageBox                           \n\
               -message                                \n\
                  [concat                              \n\
                      \"Error runing minimap2\\n\"     \n\
                      $result                          \n\
                  ]                                    \n\
               -title \"ERROR\"                        \n\
            ---;                                       \n\
                                                       \n\
            return false ;                             \n\
            # probably need                            \n\
         } ; # Else had error                          \n\
                                                       \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
         # Gui02 Sec05 Sub02 Cat04:                    \n\
         #   - add freezeTB databases                  \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         # set up databases                            \n\
                                                       \n\
         set tbCmd [list \"-sam\" $samFile] ;          \n\
         lappend tbCmd \"-prefix\" $prefix ;           \n\
         lappend tbCmd \"-amr-tbl\" $glob_amrDb ;      \n\
         lappend tbCmd \"-gene-coords\" ;              \n\
         lappend tbCmd $glob_coordsTsv ;               \n\
         lappend tbCmd \"-miru-tbl\" $glob_miruDb ;    \n\
         lappend tbCmd \"-spoligo\" $glob_spacer ;     \n\
         lappend tbCmd \"-db-spoligo\" $glob_spolDb ;  \n\
                                                       \n\
         if { $glob_mask ne 0 } {                      \n\
            lappend tbCmd \"-mask-prim\" $glob_spolDb ;\n\
         } else {                                      \n\
            lappend tbCmd \"-mask-prim\" \"-\" ;       \n\
         } ; # check if masking primers                \n\
                                                       \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
         # Gui02 Sec05 Sub02 Cat05:                    \n\
         #   - add freezeTB filtering settings         \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         lappend tbCmd \"-min-mapq\" $glob_mapq ;      \n\
         lappend tbCmd \"-min-median-q\" $glob_medQ ;  \n\
         lappend tbCmd \"-min-mean-q\" $glob_meanQ ;   \n\
         lappend tbCmd \"-min-len\" $glob_minLen ;     \n\
            # put in consensus gui, but help is filter \n\
                                                       \n\
         if { $glob_adjust ne 0 } {                    \n\
            lappend tbCmd \"-adj-coords\" ;            \n\
         } else {                                      \n\
            lappend tbCmd \"-no-adj-coords\" ;         \n\
         } ; # check if coordinate adjusting           \n\
                                                       \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
         # Gui02 Sec05 Sub02 Cat06:                    \n\
         #   - add freezeTB lineage settings           \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         lappend tbCmd \"-fudge\" $glob_miruFudge ;    \n\
         lappend tbCmd \"-dr-start\" $glob_drStart ;   \n\
         lappend tbCmd \"-dr-end\" $glob_drEnd ;       \n\
                                                       \n\
         lappend tbCmd \"-spoligo-min-score\" ;        \n\
         lappend tbCmd $glob_spolSim ;                 \n\
                                                       \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
         # Gui02 Sec05 Sub02 Cat07:                    \n\
         #   - add freezeTB tbCon consensus settings   \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         lappend tbCmd \"-min-depth\" $glob_depth ;    \n\
         lappend tbCmd \"-min-q\" $glob_snpq ;         \n\
         lappend tbCmd \"-min-q-ins\" $glob_insq ;     \n\
         lappend tbCmd \"-perc-snp-sup\" ;             \n\
         lappend tbCmd $glob_basePerc ;                \n\
         lappend tbCmd \"-perc-ins-sup\" $glob_insPerc ;\n\
         lappend tbCmd \"-perc-del-sup\" $glob_delPerc ;\n\
                                                       \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
         # Gui02 Sec05 Sub02 Cat08:                    \n\
         #   - add freezeTB tbCon & AMR print settings \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         lappend tbCmd \"-min-amr-map-perc\" ;         \n\
         lappend tbCmd $glob_amrPercSup ;              \n\
                                                       \n\
         if { $glob_frameshift ne 0 } {                \n\
            lappend tbCmd \"-frameshift\" ;            \n\
         } else {                                      \n\
            lappend tbCmd \"-no-frameshift\" ;         \n\
         } ; # check if doing frameshift checking      \n\
                                                       \n\
         lappend tbCmd \"-p-min-depth\" ;              \n\
         lappend tbCmd $glob_minVarDepth ;             \n\
                                                       \n\
         lappend tbCmd \"-p-perc-snp-sup\" ;           \n\
         lappend tbCmd glob_baseVarPerc ;              \n\
                                                       \n\
         lappend tbCmd \"-p-perc-ins-sup\" ;           \n\
         lappend tbCmd $glob_insVarPerc ;              \n\
                                                       \n\
         lappend tbCmd \"-p-perc-del-sup\" ;           \n\
         lappend tbCmd $glob_delVarPerc ;              \n\
                                                       \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
         # Gui02 Sec05 Sub02 Cat09:                    \n\
         #   - add freezeTB clustering settings        \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         if { $glob_clustBl ne 0 } {                   \n\
            lappend tbCmd \"-clust\" ;                 \n\
                                                       \n\
            lappend tbCmd \"-len-weigth\" ;            \n\
            lappend tbCmd $glob_lenWeight ;            \n\
                                                       \n\
            lappend tbCmd \"-clust-depth\" ;           \n\
            lappend tbCmd $glob_minClustDepth ;        \n\
                                                       \n\
            lappend tbCmd \"-clust-perc-depth\" ;      \n\
            lappend tbCmd $glob_minClustPercDepth ;    \n\
                                                       \n\
            lappend tbCmd \"-read-err\" ;              \n\
            lappend tbCmd $glob_clustReadErr ;         \n\
                                                       \n\
            lappend tbCmd \"-con-err\" ;               \n\
            lappend tbCmd $glob_clustConErr ;          \n\
                                                       \n\
            lappend tbCmd \"-con-sim\" ;               \n\
            lappend tbCmd $glob_maxClustSim ;          \n\
                                                       \n\
            lappend tbCmd \"-overlap\" ;               \n\
            lappend tbCmd $glob_minClustOverlap ;      \n\
                                                       \n\
            lappend tbCmd \"-perc-n\" ;                \n\
            lappend tbCmd $glob_maxClustMask ;         \n\
                                                       \n\
            if { $glob_depthProfBl ne 0 } {            \n\
               lappend tbCmd \"-depth-prof\" ;         \n\
            } else {                                   \n\
               lappend tbCmd \"-no-depth-prof\" ;      \n\
            } ; # check if doing depth profiling       \n\
                                                       \n\
            lappend tbCmd \"-err-to-var\" ;            \n\
            lappend tbCmd $glob_clustErrRatio ;        \n\
                                                       \n\
            lappend tbCmd \"-win-len\" ;               \n\
            lappend tbCmd $glob_clustWinLen ;          \n\
                                                       \n\
            lappend tbCmd \"-win-err\" ;               \n\
            lappend tbCmd $glob_clustWinErrRatio ;     \n\
                                                       \n\
            lappend tbCmd \"-indel-len\" ;             \n\
            lappend tbCmd $glob_minClustIndelLen ;     \n\
                                                       \n\
            lappend tbCmd \"-clust-q-snp\" ;           \n\
            lappend tbCmd $glob_minClustSnpQ ;         \n\
                                                       \n\
            lappend tbCmd \"-con-iter\" ;              \n\
            lappend tbCmd $glob_numRebuilds ;          \n\
         } else {                                      \n\
            lappend tbCmd \"-no-clust\" ;              \n\
         } ; # check if clustering                     \n\
                                                       \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
         # Gui02 Sec05 Sub02 Cat10:                    \n\
         #   - run freezeTB                            \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         setFreezeTBStatus ;                           \n\
                                                       \n\
         if { [catch {eval freezeTB $tbCmd } out] } {  \n\
            ---tk_messageBox                           \n\
               -message \"freezeTB error\"             \n\
               -title \"ERROR\"                        \n\
            ---; # catching stderr would be tricky     \n\
            # this stack overflow has a solution       \n\
            # https://stackoverflow.com/questions/64547745/capture-stdout-within-a-tcl-catch-command\n\
                                                       \n\
            return false;                              \n\
         } ; # If had error                            \n\
                                                       \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
         # Gui02 Sec05 Sub02 Cat11:                    \n\
         #   - build graphs                            \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         wm title . \"Required freezeTB\" ;            \n\
                                                       \n\
         ---.main.reqIn.runexit.statuslab              \n\
            configure                                  \n\
            -text \"\"                                 \n\
         ---;                                          \n\
                                                       \n\
         .main.reqIn.fq.lab configure -text \"\" ;     \n\
      } ; # run freezeTB button                        \n\
                                                       \n\
   #***************************************************\n\
   # Gui02 Sec05 Sub03:                                \n\
   #   - exit button and pack (run/exit)               \n\
   #***************************************************\n\
                                                       \n\
   ---tk::button                                       \n\
      .main.reqIn.runexit.exitbut                      \n\
      -text \"exit\"                                   \n\
      -command {exit}                                  \n\
   --- ;                                               \n\
                                                       \n\
   tk::label .main.reqIn.runexit.statuslab -text \"\" ;\n\
                                                       \n\
   ---pack                                             \n\
      .main.reqIn.runexit.runbut                       \n\
      .main.reqIn.runexit.exitbut                      \n\
      .main.reqIn.runexit.statuslab                    \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\
   # Gui03 TOC:                                        \n\
   #   - menu bar for gui                              \n\
   #   o gui03 sec01:                                  \n\
   #     - make menu frame                             \n\
   #   o gui03 sec02:                                  \n\
   #     - set up required input button                \n\
   #   o gui03 sec03:                                  \n\
   #     - set up read filtering button                \n\
   #   o gui03 sec04:                                  \n\
   #     - set up AMR menu button                      \n\
   #   o gui03 sec05:                                  \n\
   #     - set up lineage menu button                  \n\
   #   o gui03 sec06:                                  \n\
   #     - set up consensus menu button                \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Gui03 Sec01:                                      \n\
   #   - make menu frame                               \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   tk::frame .main.menu -borderwidth 2 ;               \n\
   pack .main.menu -anchor w -side top ;               \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui03 Sec02:                                      \n\
   #   - set up required input button                  \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.menu.reqBut                                \n\
      -text \"required\"                               \n\
      -relief sunken                                   \n\
      -state disabled                                  \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.reqIn ;                     \n\
         pack forget .main.filt ;                      \n\
         pack forget .main.amr ;                       \n\
         pack forget .main.lin ;                       \n\
         pack forget .main.con ;                       \n\
         pack forget .main.menu ;                      \n\
                                                       \n\
         pack .main.reqIn -anchor w -side top ;        \n\
         pack .main.menu -anchor w -side top ;         \n\
                                                       \n\
         .main.menu.reqBut configure -relief sunken ;  \n\
         .main.menu.filtBut configure -relief raised ; \n\
         .main.menu.amrBut configure -relief raised ;  \n\
         .main.menu.linBut configure -relief raised ;  \n\
         .main.menu.conBut configure -relief raised ;  \n\
                                                       \n\
         .main.menu.reqBut configure -state disabled ; \n\
         .main.menu.filtBut configure -state normal ;  \n\
         .main.menu.amrBut configure -state normal ;   \n\
         .main.menu.linBut configure -state normal ;   \n\
         .main.menu.conBut configure -state normal ;   \n\
                                                       \n\
         wm title . \"Required freezeTB\" ;            \n\
   } ; # command button fires                          \n\
                                                       \n\
   pack .main.menu.reqBut -anchor w -side left         \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui03 Sec03:                                      \n\
   #   - set up read filtering button                  \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.menu.filtBut                               \n\
      -text \"filtering\"                              \n\
      -relief raised                                   \n\
      -state normal                                    \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.reqIn ;                     \n\
         pack forget .main.filt ;                      \n\
         pack forget .main.amr ;                       \n\
         pack forget .main.lin ;                       \n\
         pack forget .main.con ;                       \n\
         pack forget .main.menu ;                      \n\
                                                       \n\
         pack .main.filt -anchor w -side top ;         \n\
         pack .main.menu -anchor w -side top ;         \n\
                                                       \n\
         .main.menu.reqBut configure -relief raised ;  \n\
         .main.menu.filtBut configure -relief sunken ; \n\
         .main.menu.amrBut configure -relief raised ;  \n\
         .main.menu.linBut configure -relief raised ;  \n\
         .main.menu.conBut configure -relief raised ;  \n\
                                                       \n\
         .main.menu.reqBut configure -state normal ;   \n\
         .main.menu.filtBut configure -state disabled ;\n\
         .main.menu.amrBut configure -state normal ;   \n\
         .main.menu.linBut configure -state normal ;   \n\
         .main.menu.conBut configure -state normal ;   \n\
                                                       \n\
         wm title . \"Filtering freezeTB\" ;           \n\
   } ; # command button fires                          \n\
                                                       \n\
   pack .main.menu.filtBut -anchor w -side left        \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui03 Sec04:                                      \n\
   #   - set up AMR menu button                        \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.menu.amrBut                                \n\
      -text \"AMRs\"                                   \n\
      -relief raised                                   \n\
      -state normal                                    \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.reqIn ;                     \n\
         pack forget .main.filt ;                      \n\
         pack forget .main.amr ;                       \n\
         pack forget .main.lin ;                       \n\
         pack forget .main.con ;                       \n\
         pack forget .main.menu ;                      \n\
                                                       \n\
         pack .main.amr -anchor w -side top ;          \n\
         pack .main.menu -anchor w -side top ;         \n\
                                                       \n\
         .main.menu.reqBut configure -relief raised ;  \n\
         .main.menu.filtBut configure -relief raised ; \n\
         .main.menu.amrBut configure -relief sunken ;  \n\
         .main.menu.conBut configure -relief raised ;  \n\
                                                       \n\
         .main.menu.reqBut configure -state normal ;   \n\
         .main.menu.filtBut configure -state normal ;  \n\
         .main.menu.amrBut configure -state disabled ; \n\
         .main.menu.linBut configure -state normal ;   \n\
         .main.menu.conBut configure -state normal ;   \n\
                                                       \n\
         wm title . \"AMRs freezeTB\" ;                \n\
   } ; # command button fires                          \n\
                                                       \n\
   pack .main.menu.amrBut -anchor w -side left         \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui03 Sec05:                                      \n\
   #   - set up lineage menu button                    \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.menu.linBut                                \n\
      -text \"lineage\"                                \n\
      -relief raised                                   \n\
      -state normal                                    \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.reqIn ;                     \n\
         pack forget .main.filt ;                      \n\
         pack forget .main.amr ;                       \n\
         pack forget .main.lin ;                       \n\
         pack forget .main.con ;                       \n\
         pack forget .main.menu ;                      \n\
                                                       \n\
         pack .main.lin -anchor w -side top ;          \n\
         pack .main.menu -anchor w -side top ;         \n\
                                                       \n\
         .main.menu.reqBut configure -relief raised ;  \n\
         .main.menu.filtBut configure -relief raised ; \n\
         .main.menu.amrBut configure -relief raised ;  \n\
         .main.menu.linBut configure -relief sunken ;  \n\
         .main.menu.conBut configure -relief raised ;  \n\
                                                       \n\
         .main.menu.reqBut configure -state normal ;   \n\
         .main.menu.filtBut configure -state normal ;  \n\
         .main.menu.amrBut configure -state normal ;   \n\
         .main.menu.linBut configure -state disabled ; \n\
         .main.menu.conBut configure -state normal ;   \n\
                                                       \n\
         wm title . \"Lineage freezeTB\" ;             \n\
   } ; # command button fires                          \n\
                                                       \n\
   pack .main.menu.linBut -anchor w -side left         \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui03 Sec06:                                      \n\
   #   - set up consensus menu button                  \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.menu.conBut                                \n\
      -text \"consensus\"                              \n\
      -relief raised                                   \n\
      -state normal                                    \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.reqIn ;                     \n\
         pack forget .main.filt ;                      \n\
         pack forget .main.amr ;                       \n\
         pack forget .main.lin ;                       \n\
         pack forget .main.con ;                       \n\
         pack forget .main.menu ;                      \n\
                                                       \n\
         pack .main.con -anchor w -side top ;          \n\
         pack .main.menu -anchor w -side top ;         \n\
                                                       \n\
         ---set                                        \n\
            conState                                   \n\
            [.main.con.menu.tbBut configure -state]    \n\
         ---;                                          \n\
         ---set                                        \n\
            conClick [lsearch $conState \"disabled\"]  \n\
         ---;                                          \n\
                                                       \n\
         ---set                                        \n\
            varState                                   \n\
            [.main.con.menu.varBut configure -state]   \n\
         ---;                                          \n\
         ---set                                        \n\
            varClick [lsearch $varState \"disabled\"]  \n\
         ---;                                          \n\
                                                       \n\
         if { $conClick >= 0 } {                       \n\
            wm title . \"Consensus tbCon freezeTB\" ;  \n\
         } elseif { $varClick >= 0 } {                 \n\
            ---wm                                      \n\
               title                                   \n\
               .                                       \n\
               \"Consensus variant tbCon freezeTB\"    \n\
            ---;                                       \n\
         } elseif { $varClick >= 0 } {                 \n\
         } else {                                      \n\
            wm title . \"Consensus cluster freezeTB\" ;\n\
         }                                             \n\
                                                       \n\
         .main.menu.reqBut configure -relief raised ;  \n\
         .main.menu.filtBut configure -relief raised ; \n\
         .main.menu.amrBut configure -relief raised ;  \n\
         .main.menu.linBut configure -relief raised ;  \n\
         .main.menu.conBut configure -relief sunken ;  \n\
                                                       \n\
         .main.menu.reqBut configure -state normal ;   \n\
         .main.menu.filtBut configure -state normal ;  \n\
         .main.menu.amrBut configure -state normal ;   \n\
         .main.menu.linBut configure -state normal ;   \n\
         .main.menu.conBut configure -state disabled ; \n\
                                                       \n\
   } ; # command button fires                          \n\
                                                       \n\
   pack .main.menu.conBut -anchor w -side left         \n\
                                                       \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\
   # Gui04 TOC:                                        \n\
   #   - read filtering menu                           \n\
   #   o gui04 sec01:                                  \n\
   #     - make read filtering frame                   \n\
   #   o gui04 sec02:                                  \n\
   #     - reference genome input                      \n\
   #   o gui04 sec03:                                  \n\
   #     - coordinates file input                      \n\
   #   o gui04 sec04:                                  \n\
   #     - mapq entry input                            \n\
   #   o gui04 sec05:                                  \n\
   #     - medain q-score                              \n\
   #   o gui04 sec06:                                  \n\
   #     - mean q-score                                \n\
   #   o gui04 sec07:                                  \n\
   #     - minimum read/consensus length               \n\
   #   o gui04 sec08:                                  \n\
   #     - masking check box + button                  \n\
   #   o gui04 sec09:                                  \n\
   #     - adjust coordinates text box                 \n\
   #   o gui04 sec10:                                  \n\
   #     - short read mapping check box                \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui04 Sec01:                                      \n\
   #   - make read filtering frame                     \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.filt -borderwidth 2 ;               \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui04 Sec02:                                      \n\
   #   - reference genome input                        \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.filt.ref ;                          \n\
   pack .main.filt.ref -anchor w -side top ;           \n\
                                                       \n\
   tk::label .main.filt.ref.lab -text $glob_refFa ;    \n\
                                                       \n\
   ---tk::button                                       \n\
       .main.filt.ref.but                              \n\
       -text \"reference\"                             \n\
       -command {                                      \n\
   ---                                                 \n\
           upvar 0 glob_refFa refFa ;                  \n\
                                                       \n\
           set faFile [                                \n\
              --- tk_getOpenFile                       \n\
                 -title \"select reference (as fasta)\"\n\
                 -filetypes $fa_types                  \n\
              ---] ; # find the reference fasta        \n\
                                                       \n\
           if {$faFile eq \"\"} {                      \n\
                                                       \n\
           } else {                                    \n\
              ---.main.filt.ref.lab                    \n\
                 configure                             \n\
                 -text $faFile ; # update label        \n\
              ---                                      \n\
                                                       \n\
             set refFa $faFile ; # update list         \n\
           } ; # Else: fasta selected                  \n\
   } ; # select reference button                       \n\
                                                       \n\
   ---pack                                             \n\
      .main.filt.ref.but                               \n\
      .main.filt.ref.lab                               \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui04 Sec03:                                      \n\
   #   - coordinates file input                        \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.filt.coords ;                       \n\
   pack .main.filt.coords -anchor w -side top ;        \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.filt.coords.lab                            \n\
      -text $glob_coordsTsv                            \n\
   --- ;                                               \n\
                                                       \n\
   ---tk::button                                       \n\
       .main.filt.coords.but                           \n\
       -text \"coordinates file\"                      \n\
       -command {                                      \n\
   ---                                                 \n\
           upvar 0 glob_coordsTsv coordsTsv ;          \n\
                                                       \n\
           set tsvFile [                               \n\
              ---tk_getOpenFile                        \n\
                 -title \"select gene coordinate file\"\n\
                 -filetypes $tsv_types                 \n\
              --- ] ; # find gene coordinates file     \n\
                                                       \n\
           if {$tsvFile eq \"\"} {                     \n\
                                                       \n\
           } else {                                    \n\
              ---.main.filt.coords.lab                 \n\
                 configure                             \n\
                 -text $tsvFile # update label         \n\
              ---;                                     \n\
                                                       \n\
             set coordsTsv $tsvFile ;                  \n\
           } ; # Else: file selected                   \n\
   } ; # select reference button                       \n\
                                                       \n\
   ---pack                                             \n\
      .main.filt.coords.but                            \n\
      .main.filt.coords.lab                            \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui04 Sec04:                                      \n\
   #   - mapq entry input                              \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.filt.mapq                           \n\
   pack .main.filt.mapq -anchor w -side top            \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.filt.mapq.lab                              \n\
      -text \"mapq (0-93)\"                            \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.filt.mapq.entry                            \n\
      -textvariable glob_mapq                          \n\
      -validate key        # validate on key press     \n\
      -vcmd { tcl_isInt_gui %P %i 0 93 }               \n\
   --- ; # get mapqping quality                        \n\
                                                       \n\
   ---pack                                             \n\
      .main.filt.mapq.lab                              \n\
      .main.filt.mapq.entry                            \n\
      -anchor w                                        \n\
      -side left ;                                     \n\
   ---                                                 \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui04 Sec05:                                      \n\
   #   - median q-score                                \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.filt.medQ                           \n\
   pack .main.filt.medQ -anchor w -side top            \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.filt.medQ.lab                              \n\
      -text \"median Q (0-93)\"                        \n\
   --- ; # medain q-score entry leabe                  \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.filt.medQ.entry                            \n\
      -textvariable glob_medQ                          \n\
      -validate key                                    \n\
      -vcmd { tcl_isInt_gui %P %i 0 93 }               \n\
   --- ; # get median q-score                          \n\
                                                       \n\
   ---pack                                             \n\
      .main.filt.medQ.lab                              \n\
      .main.filt.medQ.entry                            \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui04 Sec06:                                      \n\
   #   - mean q-score                                  \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.filt.meanQ                          \n\
   pack .main.filt.meanQ -anchor w -side top           \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.filt.meanQ.lab                             \n\
      -text \"min mean Q (0-93)\"                      \n\
   --- ; # medain q-score entry leabe                  \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.filt.meanQ.entry                           \n\
      -textvariable glob_meanQ                         \n\
      -validate key                                    \n\
      -vcmd { tcl_isInt_gui %P %i 0 93 }               \n\
   --- ; # get mean q-score                            \n\
                                                       \n\
   ---pack                                             \n\
      .main.filt.meanQ.lab                             \n\
      .main.filt.meanQ.entry                           \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui04 Sec07:                                      \n\
   #   - minimum read/consensus length                 \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.filt.len                            \n\
   pack .main.filt.len -anchor w -side top             \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.filt.len.lab                               \n\
      -text \"min length (> 0)\"                       \n\
   --- ; # medain q-score entry leabe                  \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.filt.len.entry                             \n\
      -textvariable glob_minLen                        \n\
      -validate key                                    \n\
      -vcmd { tcl_isInt_gui %P %i 1 2000000000 }       \n\
   --- ; # get minimum read length                     \n\
                                                       \n\
   ---pack                                             \n\
      .main.filt.len.lab                               \n\
      .main.filt.len.entry                             \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui04 Sec08:                                      \n\
   #   - masking check box + button                    \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.filt.mask                           \n\
   pack .main.filt.mask -anchor w -side top            \n\
                                                       \n\
   ---tk::checkbutton                                  \n\
      .main.filt.mask.check                            \n\
      -text \"mask primers\"    # text for user        \n\
      -variable glob_mask                              \n\
   --- ; # if doing long or short reads                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.filt.mask.lab                              \n\
      -text $glob_maskCoords                           \n\
   --- ; # label for masking file                      \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.filt.mask.but                              \n\
      -text \"masking coordinates\"                    \n\
      -command {                                       \n\
   ---                                                 \n\
         upvar 0 glob_maskCoords maskCoords ;          \n\
                                                       \n\
         set maskFile [                                \n\
            ---tk_getOpenFile                          \n\
               -title \"maksing coordinates\"          \n\
               -filetypes $tsv_types                   \n\
            ---                                        \n\
         ] ;                                           \n\
                                                       \n\
         if {$maskFile eq \"\" } {                     \n\
                                                       \n\
         } else {                                      \n\
            ---.main.filt.mask.lab                     \n\
               configure                               \n\
               -text $maskFile ;                       \n\
            ---                                        \n\
            set maskCoords $maskFile ;                 \n\
                                                       \n\
            # turn on the check button                 \n\
            .main.filt.mask.check select ;             \n\
         } ; # Else: masking coordinates input         \n\
      } ; # button to get mask coordinates file        \n\
                                                       \n\
   ---pack                                             \n\
      .main.filt.mask.check                            \n\
      .main.filt.mask.but                              \n\
      .main.filt.mask.lab                              \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui04 Sec09:                                      \n\
   #   - adjust coordinates text box                   \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.filt.adj ;                          \n\
   pack .main.filt.adj -anchor w -side top ;           \n\
                                                       \n\
   ---tk::checkbutton                                  \n\
      .main.filt.adj.check                             \n\
      -text \"adjust coordinates\"    # text for user  \n\
      -variable glob_adjust                            \n\
   --- ; # if doing long or short reads                \n\
                                                       \n\
   pack .main.filt.adj.check -anchor w -side left ;    \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui04 Sec10:                                      \n\
   #   - short read mapping check box                  \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.filt.sr                             \n\
   pack .main.filt.sr -anchor w -side top              \n\
                                                       \n\
   ---tk::checkbutton                                  \n\
      .main.filt.sr.check                              \n\
      -offvalue \"-x map-ont\" # ONT long reads        \n\
      -onvalue \"-x sr\"       # illumina short reads  \n\
      -text \"short reads\"    # text for user         \n\
      -variable glob_srCheck                           \n\
   --- ; # if doing long or short reads                \n\
                                                       \n\
   pack .main.filt.sr.check -anchor w -side left ;     \n\
                                                       \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
   # Gui05 TOC:                                        \n\
   #   - amr settings                                  \n\
   #   o gui05 sec01:                                  \n\
   #     - set up AMR frame                            \n\
   #   o gui05 sec02:                                  \n\
   #     - get amr database                            \n\
   #   o gui05 sec03:                                  \n\
   #     - min read AMR percent support                \n\
   #   o gui05 sec05:                                  \n\
   #     - frameshift check box                        \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui05 Sec01:                                      \n\
   #   - set up AMR frame                              \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.amr ;                               \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui05 Sec02:                                      \n\
   #   - get amr database                              \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.amr.db ;                            \n\
   pack .main.amr.db -anchor w -side top ;             \n\
                                                       \n\
   tk::label .main.amr.db.lab -text $glob_amrDb        \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.amr.db.but                                 \n\
      -text \"AMR database\"                           \n\
      -command {                                       \n\
   ---                                                 \n\
         upvar 0 glob_amrDb amrDb ;                    \n\
                                                       \n\
         set tsvFile [                                 \n\
            ---tk_getOpenFile                          \n\
               -title \"select AMR database\"          \n\
               -filetypes $tsv_types                   \n\
            ---] ;                                     \n\
          if {$tsvFile eq \"\" } {                     \n\
                                                       \n\
          } else {                                     \n\
            .main.amr.db.lab configure -text $tsvFile ;\n\
            set amrDb $tsvFile ;                       \n\
          } ; # Else: AMR database input               \n\
     } ; # amr database button                         \n\
                                                       \n\
   ---pack                                             \n\
      .main.amr.db.but                                 \n\
      .main.amr.db.lab                                 \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui05 Sec03:                                      \n\
   #   - min read AMR percent support                  \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.amr.sup ;                           \n\
   pack .main.amr.sup -anchor w -side top ;            \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.amr.sup.lab                                \n\
      -text \"% AMR support (0-1)\"                    \n\
   --- ;                                               \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.amr.sup.entry                              \n\
      -textvariable glob_amrPercSup                    \n\
      -validate key                                    \n\
      -vcmd { tcl_isFloat_gui %P %i 0 1 }              \n\
   --- ; # get minimum percent read depth for AMR      \n\
                                                       \n\
   ---pack                                             \n\
      .main.amr.sup.lab                                \n\
      .main.amr.sup.entry                              \n\
      -anchor w                                        \n\
      -side left                                       \n\
   --- ;                                               \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui05 Sec04:                                      \n\
   #   - frameshift check box                          \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.amr.frameshift ;                    \n\
   pack .main.amr.frameshift -anchor w -side top ;     \n\
                                                       \n\
   ---tk::checkbutton                                  \n\
      .main.amr.frameshift.check                       \n\
      -text \"check frameshifts\"                      \n\
      -variable glob_frameshift                        \n\
   --- ;                                               \n\
                                                       \n\
   ---pack                                             \n\
      .main.amr.frameshift.check                       \n\
      -anchor w                                        \n\
      -side left                                       \n\
   --- ;                                               \n\
                                                       \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
   # Gui06 TOC:                                        \n\
   #   - lineage settings                              \n\
   #   o gui06 sec01:                                  \n\
   #     - set up lineage frame                        \n\
   #   o gui06 sec02:                                  \n\
   #     - databases (MIRU and spoligo)                \n\
   #   o gui06 sec03:                                  \n\
   #     - MIRU-VNTR lineages                          \n\
   #   o gui06 sec04:                                  \n\
   #     - spoligotyping lineages                      \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui06 Sec01:                                      \n\
   #   - set up lineage frame                          \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.lin ;                               \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui06 Sec02:                                      \n\
   #   - databases (MIRU and spoligo)                  \n\
   #   o gui06 sec02 sub01:                            \n\
   #     - make database frame                         \n\
   #   o gui06 sec02 sub02:                            \n\
   #     - set up miru database button                 \n\
   #   o gui06 sec02 sub03:                            \n\
   #     - set up spoligotype sequences button         \n\
   #   o gui06 sec02 sub04:                            \n\
   #     - set up spoligotype database button          \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #***************************************************\n\
   # Gui06 Sec02 Sub01:                                \n\
   #   - make database frame                           \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.lin.db ;                            \n\
   pack .main.lin.db -anchor w -side top ;             \n\
                                                       \n\
   #***************************************************\n\
   # Gui06 Sec02 Sub02:                                \n\
   #   - set up miru database button                   \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.lin.db.miru ;                       \n\
   pack .main.lin.db.miru -anchor w -side top ;        \n\
                                                       \n\
   tk::label .main.lin.db.miru.lab -text $glob_miruDb ;\n\
                                                       \n\
   ---tk::button                                       \n\
      .main.lin.db.miru.but                            \n\
      -text \"MIRU-VNTR database\"                     \n\
      -command {                                       \n\
   ---                                                 \n\
         upvar glob_miruDb miruDb ;                    \n\
                                                       \n\
         set tsvFile [                                 \n\
            ---tk_getOpenFile                          \n\
               -title \"select MIRU-VNTR database\"    \n\
               -filetypes $tsv_types                   \n\
            --- ] ; # get MIRU databse                 \n\
                                                       \n\
         if { $tsvFile eq \"\" } {                     \n\
                                                       \n\
         } else {                                      \n\
            ---.main.lin.db.miru.lab                   \n\
               configure                               \n\
               -text $tsvFile                          \n\
             ---;                                      \n\
             set miruDb $tsvFile ;                     \n\
         } ; # Else: miru database selected            \n\
   } ; # -command get MIRU database                    \n\
                                                       \n\
   ---pack                                             \n\
      .main.lin.db.miru.but                            \n\
      .main.lin.db.miru.lab                            \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #***************************************************\n\
   # Gui06 Sec02 Sub03:                                \n\
   #   - set up spoligotype sequences button           \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.lin.db.spacer ;                     \n\
   pack .main.lin.db.spacer -anchor w -side top ;      \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.lin.db.spacer.lab                          \n\
      -text                                            \n\
      $glob_spacer                                     \n\
   --- ; # holds spacer database location              \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.lin.db.spacer.but                          \n\
      -text \"spoligotype spacers\"                    \n\
      -command {                                       \n\
   ---                                                 \n\
         upvar 0 glob_spacer spacer ;                  \n\
                                                       \n\
         set spacerFa [                                \n\
            ---tk_getOpenFile                          \n\
               -title \"spoligotype spacer sequences\" \n\
               -filetypes $fa_types                    \n\
            --- ] ; # get spacer sequence fasta file   \n\
                                                       \n\
         if {$spacerFa eq \"\"} {                      \n\
                                                       \n\
         } else {                                      \n\
            ---.main.lin.db.spacer.lab                 \n\
               configure                               \n\
               -text $spacerFa                         \n\
            ---;                                       \n\
            set spacer $spacerFa ;                     \n\
         } ; # Else: user input fasta                  \n\
   } ; # button for spacers database                   \n\
                                                       \n\
   ---pack                                             \n\
      .main.lin.db.spacer.but                          \n\
      .main.lin.db.spacer.lab                          \n\
      -anchor w                                        \n\
      -side left                                       \n\
   --- ;                                               \n\
                                                       \n\
   #***************************************************\n\
   # Gui06 Sec02 Sub04:                                \n\
   #   - set up spoligotype database button            \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.lin.db.spol ;                       \n\
   pack .main.lin.db.spol -anchor w -side top ;        \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.lin.db.spol.lab                            \n\
      -text                                            \n\
      $glob_spolDb                                     \n\
   --- ; # holds spol database location                \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.lin.db.spol.but                            \n\
      -text \"spoligotype database\"                   \n\
      -command {                                       \n\
   ---                                                 \n\
         upvar 0 glob_spolDb spolDb ;                  \n\
                                                       \n\
         set spolTsv [                                 \n\
            ---tk_getOpenFile                          \n\
               -title \"spoligotype lineage database\" \n\
               -filetypes $csv_types                   \n\
            --- ] ;                                    \n\
                                                       \n\
         if {$spolTsv eq \"\"} {                       \n\
                                                       \n\
         } else {                                      \n\
            ---.main.lin.db.spol.lab                   \n\
               configure                               \n\
               -text $spolTsv                          \n\
            ---;                                       \n\
            set spolDb $spolTsv ;                      \n\
         } ; # Else: user input csv                    \n\
   } ; # button for spols database                     \n\
                                                       \n\
   ---pack                                             \n\
      .main.lin.db.spol.but                            \n\
      .main.lin.db.spol.lab                            \n\
      -anchor w                                        \n\
      -side left                                       \n\
   --- ;                                               \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui06 Sec03:                                      \n\
   #   - MIRU-VNTR lineages                            \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.lin.miru ;                          \n\
   pack .main.lin.miru -anchor w -side top ;           \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.lin.miru.lab                               \n\
      -text                                            \n\
      \"fudge (0-100)\"                                \n\
   --- ;                                               \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.lin.miru.entry                             \n\
      -textvariable glob_miruFudge                     \n\
      -validate key                                    \n\
      -vcmd { tcl_isInt_gui %P %i 0 100 }              \n\
   ; --- # -command for fudge entry                    \n\
                                                       \n\
   ---pack                                             \n\
      .main.lin.miru.lab                               \n\
      .main.lin.miru.entry                             \n\
      -anchor w                                        \n\
      -side left                                       \n\
   --- ;                                               \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui06 Sec04:                                      \n\
   #   - spoligotyping lineages                        \n\
   #   o gui06 sec04 sub01:                            \n\
   #     - set up spoligotype frame                    \n\
   #   o gui06 sec04 sub02:                            \n\
   #     - set up min percent similarity               \n\
   #   o gui06 sec04 sub03:                            \n\
   #     - direct repeat frame                         \n\
   #   o gui06 sec04 sub04:                            \n\
   #     - direct repeat starting coordinates          \n\
   #   o gui06 sec04 sub05:                            \n\
   #     - direct repeat ending coordinates            \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #***************************************************\n\
   # Gui06 Sec04 Sub01:                                \n\
   #   - set up spoligotype frame                      \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.lin.spol ;                          \n\
   pack .main.lin.spol -anchor w -side top ;           \n\
                                                       \n\
   #***************************************************\n\
   # Gui06 Sec04 Sub02:                                \n\
   #   - set up min percent similarity                 \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.lin.spol.sim ;                      \n\
   pack .main.lin.spol.sim -anchor w -side top ;       \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.lin.spol.sim.lab                           \n\
      -text \"spoligo min % match (0-1)\"              \n\
   --- ;                                               \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.lin.spol.sim.entry                         \n\
      -textvariable glob_spolSim                       \n\
      -validate key                                    \n\
      -vcmd { tcl_isFloat_gui %P %i 0 1 }              \n\
   --- ; # entry for minimum percent match             \n\
                                                       \n\
   ---pack                                             \n\
     .main.lin.spol.sim.lab                            \n\
     .main.lin.spol.sim.entry                          \n\
     -anchor w                                         \n\
     -side left                                        \n\
   --- ;                                               \n\
                                                       \n\
   #***************************************************\n\
   # Gui06 Sec04 Sub03:                                \n\
   #   - direct repeat frame                           \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.lin.spol.dr ;                       \n\
   pack .main.lin.spol.dr -anchor w -side top ;        \n\
                                                       \n\
   tk::frame .main.lin.spol.dr.dr ;                    \n\
   pack .main.lin.spol.dr.dr -anchor w -side top ;     \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.lin.spol.dr.dr.lab                         \n\
      -text \"Direct Repeat Coordinates:\"             \n\
   --- ;                                               \n\
                                                       \n\
   pack .main.lin.spol.dr.dr.lab -anchor w -side left ;\n\
                                                       \n\
   #***************************************************\n\
   # Gui06 Sec04 Sub04:                                \n\
   #   - direct repeat starting coordinates            \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.lin.spol.dr.start ;                 \n\
   pack .main.lin.spol.dr.start -anchor w -side top ;  \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.lin.spol.dr.start.lab                      \n\
      -text \"    start (> 0):\"                       \n\
   --- ;                                               \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.lin.spol.dr.start.entry                    \n\
      -textvariable glob_drStart                       \n\
      -validate key                                    \n\
      -vcmd { tcl_isInt_gui %P %i 1 2000000000 }       \n\
   --- ; # direct repeat start                         \n\
                                                       \n\
   ---pack                                             \n\
      .main.lin.spol.dr.start.lab                      \n\
      .main.lin.spol.dr.start.entry                    \n\
      -anchor w                                        \n\
      -side left                                       \n\
   --- ;                                               \n\
                                                       \n\
   #***************************************************\n\
   # Gui06 Sec04 Sub05:                                \n\
   #   - direct repeat ending coordinates              \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.lin.spol.dr.end ;                   \n\
   pack .main.lin.spol.dr.end -anchor w -side top ;    \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.lin.spol.dr.end.lab                        \n\
      -text \"    end (> 0):\"                         \n\
   --- ;                                               \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.lin.spol.dr.end.entry                      \n\
      -textvariable glob_drEnd                         \n\
      -validate key                                    \n\
      -vcmd { tcl_isInt_gui %P %i 1 2000000000 }       \n\
   --- ; # direct repeat end                           \n\
                                                       \n\
   ---pack                                             \n\
      .main.lin.spol.dr.end.lab                        \n\
      .main.lin.spol.dr.end.entry                      \n\
      -anchor w                                        \n\
      -side left                                       \n\
   --- ;                                               \n\
                                                       \n\
   #---~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\
   ' Gui07 TOC:                                        \n\
   '   - consensus settings                            \n\
   '   o gui07 sec01:                                  \n\
   '     - set up consensus frame                      \n\
   '   o gui07 sec02:                                  \n\
   '     - set up consensus frame                      \n\
   '   o gui07 sec03:                                  \n\
   '     - set up consensus frame                      \n\
   '   o gui07 sec04:                                  \n\
   '     - set up edClust menu                         \n\
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~---#\n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui07 Sec01:                                      \n\
   #   - set up consensus frame                        \n\
   #   o gui07 sec01 sub01:                            \n\
   #     - set up consensus frame and sub frames       \n\
   #   o gui07 sec01 sub02:                            \n\
   #     - set up tbCon menu button                    \n\
   #   o gui07 sec01 sub03:                            \n\
   #     - set up tbCon variant menu button            \n\
   #   o gui07 sec01 sub04:                            \n\
   #     - set up cluster menu button                  \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec01 Sub01:                                \n\
   #   - set up consensus frame and sub frames         \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con ;                               \n\
                                                       \n\
   tk::frame .main.con.menu ;  # tbCon frame           \n\
   tk::frame .main.con.tb ;    # tbCon frame           \n\
   tk::frame .main.con.var ;   # tbCon frame           \n\
   tk::frame .main.con.clust ; # edClust frame         \n\
                                                       \n\
   pack .main.con.menu -anchor w -side top ;           \n\
   pack .main.con.tb -anchor w -side top ;             \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec01 Sub02:                                \n\
   #   - set up tbCon menu button                      \n\
   #***************************************************\n\
                                                       \n\
   ---tk::button                                       \n\
      .main.con.menu.tbBut                             \n\
      -text \"tbCon\"                                  \n\
      -relief sunken                                   \n\
      -state disabled                                  \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.con.menu ;                  \n\
         pack forget .main.con.tb ;                    \n\
         pack forget .main.con.var ;                   \n\
         pack forget .main.con.clust ;                 \n\
                                                       \n\
         pack .main.con.menu -anchor w -side top ;     \n\
         pack .main.con.tb -anchor w -side top ;       \n\
                                                       \n\
         wm title . \"Consensus tbCon freezeTB\" ;     \n\
                                                       \n\
         ---.main.con.menu.tbBut                       \n\
            configure                                  \n\
            -state disabled                            \n\
            -relief sunken                             \n\
         ---;                                          \n\
         ---.main.con.menu.varBut                      \n\
            configure                                  \n\
            -state normal                              \n\
            -relief raised                             \n\
         ---;                                          \n\
         ---.main.con.menu.clustBut                    \n\
            configure                                  \n\
            -state normal                              \n\
            -relief raised                             \n\
         ---;                                          \n\
      } ; # show tbCon menu                            \n\
                                                       \n\
   pack .main.con.menu.tbBut -anchor w -side left ;    \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec01 Sub03:                                \n\
   #   - set up tbCon variant menu button              \n\
   #***************************************************\n\
                                                       \n\
   ---tk::button                                       \n\
      .main.con.menu.varBut                            \n\
      -text \"tbCon variant\"                          \n\
      -relief raised                                   \n\
      -state normal                                    \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.con.menu ;                  \n\
         pack forget .main.con.tb ;                    \n\
         pack forget .main.con.var ;                   \n\
         pack forget .main.con.clust ;                 \n\
                                                       \n\
         pack .main.con.menu -anchor w -side top ;     \n\
         pack .main.con.var -anchor w -side top ;      \n\
                                                       \n\
         ---wm                                         \n\
            title                                      \n\
            .                                          \n\
            \"Consensus Variant tbCon freezeTB\"       \n\
         ---;                                          \n\
         ---.main.con.menu.tbBut                       \n\
            configure                                  \n\
            -state normal                              \n\
            -relief raised                             \n\
         ---;                                          \n\
         ---.main.con.menu.varBut                      \n\
            configure                                  \n\
            -state disabled                            \n\
            -relief sunken                             \n\
         ---;                                          \n\
         ---.main.con.menu.clustBut                    \n\
            configure                                  \n\
            -state normal                              \n\
            -relief raised                             \n\
         ---;                                          \n\
      } ; # show tbCon variant menu                    \n\
                                                       \n\
   pack .main.con.menu.varBut -anchor w -side left ;   \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec01 Sub04:                                \n\
   #   - set up cluster menu button                    \n\
   #***************************************************\n\
                                                       \n\
   ---tk::button                                       \n\
      .main.con.menu.clustBut                          \n\
      -text \"cluster\"                                \n\
      -relief raised                                   \n\
      -state normal                                    \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.con.menu ;                  \n\
         pack forget .main.con.tb ;                    \n\
         pack forget .main.con.var ;                   \n\
         pack forget .main.con.clust ;                 \n\
                                                       \n\
         pack .main.con.menu -anchor w -side top ;     \n\
         pack .main.con.clust -anchor w -side top ;    \n\
                                                       \n\
         wm title . \"Consensus cluster freezeTB\" ;   \n\
                                                       \n\
         ---.main.con.menu.tbBut                       \n\
            configure                                  \n\
            -state normal                              \n\
            -relief raised                             \n\
         ---;                                          \n\
         ---.main.con.menu.varBut                      \n\
            configure                                  \n\
            -state normal                              \n\
            -relief raised                             \n\
         ---;                                          \n\
         ---.main.con.menu.clustBut                    \n\
            configure                                  \n\
            -state disabled                            \n\
            -relief sunken                             \n\
         ---;                                          \n\
      } ; # show edClust menu                          \n\
                                                       \n\
      ---pack                                          \n\
         .main.con.menu.clustBut                       \n\
         -anchor w                                     \n\
         -side left                                    \n\
      ---;                                             \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui07 Sec02:                                      \n\
   #   - set up consensus frame                        \n\
   #   o gui07 sec02 sub01:                            \n\
   #     - min snp q-score                             \n\
   #   o gui07 sec02 sub02:                            \n\
   #     - min ins q-score                             \n\
   #   o gui07 sec02 sub03:                            \n\
   #     - min ins q-score                             \n\
   #   o gui07 sec02 sub04:                            \n\
   #     - min base percent                            \n\
   #   o gui07 sec02 sub05:                            \n\
   #     - min insertion percent                       \n\
   #   o gui07 sec02 sub06:                            \n\
   #     - min deletion percent                        \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec02 Sub01:                                \n\
   #   - min snp q-score                               \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.tb.snpq ;                       \n\
   pack .main.con.tb.snpq -anchor w -side top ;        \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.tb.snpq.lab                            \n\
      -text \"Min snp q-score (0-93)\"                 \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.tb.snpq.entry                          \n\
      -textvariable glob_snpq                          \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 0 93 }                \n\
   --- ; # min snp q-score entry                       \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.tb.snpq.lab                            \n\
      .main.con.tb.snpq.entry                          \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec02 Sub02:                                \n\
   #   - min ins q-score                               \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.tb.insq ;                       \n\
   pack .main.con.tb.insq -anchor w -side top ;        \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.tb.insq.lab                            \n\
      -text \"Min insertion q-score (0-93)\"           \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.tb.insq.entry                          \n\
      -textvariable glob_insq                          \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 0 93 }                \n\
   --- ; # min ins q-score entry                       \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.tb.insq.lab                            \n\
      .main.con.tb.insq.entry                          \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec02 Sub03:                                \n\
   #   - min read depth                                \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.tb.depth ;                      \n\
   pack .main.con.tb.depth -anchor w -side top ;       \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.tb.depth.lab                           \n\
      -text \"Min read depth (> 0)\"                   \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.tb.depth.entry                         \n\
      -textvariable glob_depth                         \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 1 2000000000 }        \n\
   --- ;                                               \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.tb.depth.lab                           \n\
      .main.con.tb.depth.entry                         \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec02 Sub04:                                \n\
   #   - min base percent                              \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.tb.baseperc ;                   \n\
   pack .main.con.tb.baseperc -anchor w -side top ;    \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.tb.baseperc.lab                        \n\
      -text \"Min % base support (0-1)\"               \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.tb.baseperc.entry                      \n\
      -textvariable glob_basePerc                      \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # min percent depth for base                  \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.tb.baseperc.lab                        \n\
      .main.con.tb.baseperc.entry                      \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec02 Sub05:                                \n\
   #   - min insertion percent                         \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.tb.insperc ;                    \n\
   pack .main.con.tb.insperc -anchor w -side top ;     \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.tb.insperc.lab                         \n\
      -text \"Min % insertion support (0-1)\"          \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.tb.insperc.entry                       \n\
      -textvariable glob_insPerc                       \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # min percent depth for ins                   \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.tb.insperc.lab                         \n\
      .main.con.tb.insperc.entry                       \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec02 Sub06:                                \n\
   #   - min deletion percent                          \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.tb.delperc ;                    \n\
   pack .main.con.tb.delperc -anchor w -side top ;     \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.tb.delperc.lab                         \n\
      -text \"Min % deletion support (0-1)\"           \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.tb.delperc.entry                       \n\
      -textvariable glob_delPerc                       \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # min percent depth for del                   \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.tb.delperc.lab                         \n\
      .main.con.tb.delperc.entry                       \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui07 Sec03:                                      \n\
   #   - set up variant report frame for consensus     \n\
   #   o gui07 sec03 sub01:                            \n\
   #     - min read depth                              \n\
   #   o gui07 sec03 sub02:                            \n\
   #     - min base percent                            \n\
   #   o gui07 sec03 sub03:                            \n\
   #     - min insertion percent                       \n\
   #   o gui07 sec03 sub04:                            \n\
   #     - min deletion percent                        \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec03 Sub01:                                \n\
   #   - min read depth                                \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.var.depth ;                     \n\
   pack .main.con.var.depth -anchor w -side top ;      \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.var.depth.lab                          \n\
      -text \"Variant: min read depth (> 0)\"          \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.var.depth.entry                        \n\
      -textvariable glob_minVarDepth                   \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 1 2000000000 }        \n\
   --- ; # min snp q-score entry                       \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.var.depth.lab                          \n\
      .main.con.var.depth.entry                        \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec03 Sub04:                                \n\
   #   - min base percent                              \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.var.baseperc ;                  \n\
   pack .main.con.var.baseperc -anchor w -side top ;   \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.var.baseperc.lab                       \n\
      -text \"Variant: min % base support (0-1)\"      \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.var.baseperc.entry                     \n\
      -textvariable glob_baseVarPerc                   \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # min percent depth for base                  \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.var.baseperc.lab                       \n\
      .main.con.var.baseperc.entry                     \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec03 Sub05:                                \n\
   #   - min insertion percent                         \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.var.insperc ;                   \n\
   pack .main.con.var.insperc -anchor w -side top ;    \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.var.insperc.lab                        \n\
      -text \"Variant: min % insertion support (0-1)\" \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.var.insperc.entry                      \n\
      -textvariable glob_insVarPerc                    \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # min percent depth for ins                   \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.var.insperc.lab                        \n\
      .main.con.var.insperc.entry                      \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec03 Sub05:                                \n\
   #   - min deletion percent                          \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.var.delperc ;                   \n\
   pack .main.con.var.delperc -anchor w -side top ;    \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.var.delperc.lab                        \n\
      -text \"Variant: min % deletion support (0-1)\"  \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.var.delperc.entry                      \n\
      -textvariable glob_delVarPerc                    \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # min percent depth for del                   \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.var.delperc.lab                        \n\
      .main.con.var.delperc.entry                      \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui07 Sec04:                                      \n\
   #   - set up edClust menu                           \n\
   #   o gui07 sec04 sub01:                            \n\
   #     - set up cluster frame                        \n\
   #   o gui07 sec04 sub02:                            \n\
   #     - set up cluster flitering                    \n\
   #   o gui07 sec04 sub03:                            \n\
   #     - set up cluster calculations                 \n\
   #   o gui07 sec04 sub04:                            \n\
   #     - set up cluster consnesus filtering          \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec04 Sub01:                                \n\
   #   - set up cluster frame                          \n\
   #***************************************************\n\
                                                       \n\
   tk::frame .main.con.clust.mid ;                     \n\
                                                       \n\
   ---tk::checkbutton                                  \n\
      .main.con.clust.check                            \n\
      -text \"cluster\"                                \n\
      -variable glob_clustBl                           \n\
   ---                                                 \n\
                                                       \n\
   pack .main.con.clust.check -anchor w -side top ;    \n\
   pack .main.con.clust.mid -anchor w -side top ;      \n\
                                                       \n\
   ---tk::frame                                        \n\
      .main.con.clust.mid.filt                         \n\
      -borderwidth 2                                   \n\
      -relief groove                                   \n\
   ---;                                                \n\
                                                       \n\
   ---tk::frame                                        \n\
      .main.con.clust.mid.calc                         \n\
      -borderwidth 2                                   \n\
      -relief groove                                   \n\
   ---;                                                \n\
                                                       \n\
   ---tk::frame                                        \n\
      .main.con.clust.con                              \n\
      -borderwidth 2                                   \n\
      -relief groove                                   \n\
   ---;                                                \n\
                                                       \n\
   pack .main.con.clust.mid.filt -anchor w -side left ;\n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc                         \n\
      -anchor e                                        \n\
      -side right                                      \n\
   ---;                                                \n\
                                                       \n\
   pack .main.con.clust.con -anchor s -side top ;      \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec04 Sub02:                                \n\
   #   - set up cluster flitering                      \n\
   #   o gui07 sec04 sub02 cat01:                      \n\
   #     - min read depth                              \n\
   #   o gui07 sec04 sub02 cat02:                      \n\
   #     - min percent read depth                      \n\
   #   o gui07 sec04 sub02 cat03:                      \n\
   #     - min snp q-score                             \n\
   #   o gui07 sec04 sub02 cat04:                      \n\
   #     - min indel len                               \n\
   #   o gui07 sec04 sub02 cat05:                      \n\
   #     - depth profiling                             \n\
   #***************************************************\n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub02 Cat01:                          \n\
   #   - min read depth                                \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.mid.filt.depth            \n\
   ---pack                                             \n\
      .main.con.clust.mid.filt.depth                   \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.mid.filt.depth.lab               \n\
      -text \"min depth (> 0)\"                        \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.mid.filt.depth.entry             \n\
      -textvariable glob_minClustDepth                 \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 1 2000000000 }        \n\
   --- ; # min read depth for clustering               \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.filt.depth.lab               \n\
      .main.con.clust.mid.filt.depth.entry             \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub02 Cat02:                          \n\
   #   - min percent read depth                        \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.mid.filt.percdepth        \n\
   ---pack                                             \n\
      .main.con.clust.mid.filt.percdepth               \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.mid.filt.percdepth.lab           \n\
      -text \"min % depth (0-1)\"                      \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.mid.filt.percdepth.entry         \n\
      -textvariable glob_minClustPercDepth             \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # min percent read depth for clustering       \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.filt.percdepth.lab           \n\
      .main.con.clust.mid.filt.percdepth.entry         \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub02 Cat03:                          \n\
   #   - min snp q-score                               \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.mid.filt.snpq             \n\
   ---pack                                             \n\
      .main.con.clust.mid.filt.snpq                    \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.mid.filt.snpq.lab                \n\
      -text \"min snp Q (0-93)\"                       \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.mid.filt.snpq.entry              \n\
      -textvariable glob_minClustSnpQ                  \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 0 93 }                \n\
   --- ; # min snp q-score                             \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.filt.snpq.lab                \n\
      .main.con.clust.mid.filt.snpq.entry              \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub02 Cat04:                          \n\
   #   - min indel len                                 \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.mid.filt.indellen         \n\
   ---pack                                             \n\
      .main.con.clust.mid.filt.indellen                \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.mid.filt.indellen.lab            \n\
      -text \"min indel length (> 0)\"                 \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.mid.filt.indellen.entry          \n\
      -textvariable glob_minClustIndelLen              \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 1 2000000000 }        \n\
   --- ; # min snp q-score                             \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.filt.indellen.lab            \n\
      .main.con.clust.mid.filt.indellen.entry          \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub02 Cat05:                          \n\
   #   - depth profiling                               \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   ---tk::checkbutton                                  \n\
      .main.con.clust.mid.filt.depthprofile            \n\
      -text \"Depth profile\"                          \n\
      -variable glob_depthProfBl                       \n\
   --- ; # min snp q-score                             \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.filt.depthprofile            \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
   #***************************************************\n\
   # Gui07 Sec04 Sub03:                                \n\
   #   - set up cluster calcuations                    \n\
   #   o gui07 sec04 sub03 cat01:                      \n\
   #     - expected read error rate                    \n\
   #   o gui07 sec04 sub03 cat02:                      \n\
   #     - expected consensus error rate               \n\
   #   o gui07 sec04 sub03 cat03:                      \n\
   #     - variant to error ratio                      \n\
   #   o gui07 sec04 sub03 cat04:                      \n\
   #     - window variant to error ratio               \n\
   #   o gui07 sec04 sub03 cat05:                      \n\
   #     - window length                               \n\
   #***************************************************\n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub03 Cat01:                          \n\
   #   - expected read error rate                      \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.mid.calc.readerr          \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.readerr                 \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.mid.calc.readerr.lab             \n\
      -text \"read map read error rate (0-1)\"         \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.mid.calc.readerr.entry           \n\
      -textvariable glob_clustReadErr                  \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # expected read error rate                    \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.readerr.lab             \n\
      .main.con.clust.mid.calc.readerr.entry           \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub03 Cat02:                          \n\
   #   - expected consensus error rate                 \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.mid.calc.conerr           \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.conerr                  \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.mid.calc.conerr.lab              \n\
      -text \"consensus map read error rate (0-1)\"    \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.mid.calc.conerr.entry            \n\
      -textvariable glob_clustConErr                   \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # expected consensus error rate               \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.conerr.lab              \n\
      .main.con.clust.mid.calc.conerr.entry            \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub03 Cat03:                          \n\
   #   - variant to error ratio                        \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.mid.calc.errratio         \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.errratio                \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.mid.calc.errratio.lab            \n\
      -text \"var:err 100*(variant/error) (> 0)\"      \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.mid.calc.errratio.entry          \n\
      -textvariable glob_clustErrRatio                 \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 1 2000000000 }        \n\
   --- ;                                               \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.errratio.lab            \n\
      .main.con.clust.mid.calc.errratio.entry          \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub03 Cat04:                          \n\
   #   - window variant to error ratio                 \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.mid.calc.winerr           \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.winerr                  \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.mid.calc.winerr.lab              \n\
      -text \"window var:err (> 0)\"                   \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.mid.calc.winerr.entry            \n\
      -textvariable glob_clustWinErrRatio              \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 1 2000000000 }        \n\
   --- ;                                               \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.winerr.lab              \n\
      .main.con.clust.mid.calc.winerr.entry            \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub03 Cat05:                          \n\
   #   - window length                                 \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.mid.calc.winlen           \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.winlen                  \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.mid.calc.winlen.lab              \n\
      -text \"window length (> 0)\"                    \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.mid.calc.winlen.entry            \n\
      -textvariable glob_clustWinLen                   \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 1 2000000000 }        \n\
   --- ;                                               \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.winlen.lab              \n\
      .main.con.clust.mid.calc.winlen.entry            \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub03 Cat06:                          \n\
   #   - length weight                                 \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.mid.calc.lenweight        \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.lenweight               \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.mid.calc.lenweight.lab           \n\
      -text \"score: median Q + log2(len)*input (> 0)\"\n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.mid.calc.lenweight.entry         \n\
      -textvariable glob_lenWeight                     \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 1 2000000000 }      \n\
   --- ;                                               \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.mid.calc.lenweight.lab           \n\
      .main.con.clust.mid.calc.lenweight.entry         \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #***************************************************\n\
   # Gui07 Sec04 Sub04:                                \n\
   #   - set up cluster consnesus filtering            \n\
   #   o gui07 sec04 sub04 cat01:                      \n\
   #     - max percent simularity                      \n\
   #   o gui07 sec04 sub04 cat02:                      \n\
   #     - min percent overlap                         \n\
   #   o gui07 sec04 sub04 cat03:                      \n\
   #     - maximum percent masking                     \n\
   #   o gui07 sec04 sub04 cat04:                      \n\
   #     - min percent simularity                      \n\
   #***************************************************\n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub04 Cat01:                          \n\
   #   - max percent simularity                        \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.con.maxsim                \n\
   ---pack                                             \n\
      .main.con.clust.con.maxsim                       \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.con.maxsim.lab                   \n\
      -text \"max % similarity (0-1)\"                 \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.con.maxsim.entry                 \n\
      -textvariable glob_maxClustSim                   \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # max percent read depth for clustering       \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.con.maxsim.lab                   \n\
      .main.con.clust.con.maxsim.entry                 \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub04 Cat02:                          \n\
   #   - minimu percent overlap                        \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.con.minoverlap            \n\
   ---pack                                             \n\
      .main.con.clust.con.minoverlap                   \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.con.minoverlap.lab               \n\
      -text \"min % consensus overlap (0-1)\"          \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.con.minoverlap.entry             \n\
      -textvariable glob_minClustOverlap               \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # max percent read depth for clustering       \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.con.minoverlap.lab               \n\
      .main.con.clust.con.minoverlap.entry             \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub04 Cat03:                          \n\
   #   - maximum percent masking                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.con.maxmask               \n\
   ---pack                                             \n\
      .main.con.clust.con.maxmask                      \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.con.maxmask.lab                  \n\
      -text \"max % masking (0-1)\"                    \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.con.maxmask.entry                \n\
      -textvariable glob_maxClustMask                  \n\
      -validate key                                    \n\
      -vcmd {tcl_isFloat_gui %P %i 0 1 }               \n\
   --- ; # max percent read depth for clustering       \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.con.maxmask.lab                  \n\
      .main.con.clust.con.maxmask.entry                \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Gui07 Sec04 Sub04 Cat04:                          \n\
   #   - number rebuilds                               \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
   tk::frame .main.con.clust.con.numbuild              \n\
   ---pack                                             \n\
      .main.con.clust.con.numbuild                     \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.con.clust.con.numbuild.lab                 \n\
      -text \"times to rebuild consensus (0-100)\"     \n\
   ---                                                 \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.con.clust.con.numbuild.entry               \n\
      -textvariable glob_numRebuilds                   \n\
      -validate key                                    \n\
      -vcmd {tcl_isInt_gui %P %i 0 100 }               \n\
   --- ; # min percent read depth for clustering       \n\
                                                       \n\
   ---pack                                             \n\
      .main.con.clust.con.numbuild.lab                 \n\
      .main.con.clust.con.numbuild.entry               \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---                                                 \n\
                                                       \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
   # Gui08 TOC:                                        \n\
   #   - set up output settings/input frame            \n\
   #   o gui07 sec08:                                  \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui08 Sec01:                                      \n\
   #   -                                               \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
   # Gui09 TOC:                                        \n\
   #   - set up output report frame                    \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
                                                       \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
   # Gui10 TOC:                                        \n\
   #   - set up output read depth graph frame          \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
                                                       \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
   # Gui11 TOC:                                        \n\
   #   - set up output coverage graph frame            \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
                                                       \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  \n\
   # Gui12 TOC:                                        \n\
   #   - set up output amr table frame                 \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
"; /*glob_guiScript_gui*/
