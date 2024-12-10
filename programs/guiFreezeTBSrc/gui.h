/*For GUI scripts I am using --- to mark the starts and
`   ends of blocks. I then use str_rmBlocks to remove the
`   block markers and convert the blocks single line. It
`   is a bit inefficent, but makes my tlctk life so much
`   more sain.
*/

static signed char *
   glob_guiScript_gui =
   (signed char *)
" \n\
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
   #   o header sec05:                                 \n\
   #     - find freezeTB location (remove for C)       \n\
   #-------------------------------------------------- \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Header Sec01:                                     \n\
   #   - global variables (not modified by C)          \n\
   #   o header sec01 sub01:                           \n\
   #     - variables to store values                   \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   #***************************************************\n\
   # Header Sec01 Sub01:                               \n\
   #   - variables to store values                     \n\
   #***************************************************\n\
                                                       \n\
   # required input                                    \n\
   variable glob_fqIn \"\" ;                           \n\
   variable glob_dirOut \"\" ;                         \n\
   variable glob_prefix \"FTB_output\" ;               \n\
                                                       \n\
   # read filtering settings                           \n\
   variable glob_mask 0 ;    # mask primer sites       \n\
   variable glob_adjust 0 ;  # adjust mapp coordinates \n\
   variable glob_srCheck \"map-ont\" ; # map setting   \n\
                                                       \n\
   # amr setting                                       \n\
   variable glob_frameshift 0 ; # AMR frameshift scan  \n\
                                                       \n\
   # clustering                                        \n\
   variable glob_clustBl 0 ;                           \n\
   variable glob_depthProfBl 0 ;                       \n\
                                                       \n\
   # output settings                                   \n\
   variable glob_outPref \"\" ; # report user is getting\n\
   variable glob_outCur \"\" ;  # current shown report \n\
   variable glob_outSnpSup 0.1 ;                       \n\
   variable glob_outIndelSup 0.4 ;                     \n\
   variable glob_mkGraphBl 1 ;                         \n\
                                                       \n\
   variable glob_depthImg [image create photo -file \"\"] ;\n\
   variable glob_coverImg [image create photo -file \"\"] ;\n\
                                                       \n\
   #***************************************************\n\
   # Header Sec01 Sub02:                               \n\
   #   - amr list                                      \n\
   #***************************************************\n\
                                                       \n\
   # color pallete is from viridis                     \n\
   set glob_noAmrCol \"#440154\" ; # viridis purple    \n\
   set glob_amrTextCol \"#21908C\" ; # light green     \n\
   set glob_amrCol \"#FDE725\" ; # viridis yellow      \n\
                                                       \n\
   # keep this list lower case                         \n\
   ---set                                              \n\
      glob_amrList                                     \n\
      [list                                            \n\
         \"amikacin\"                                  \n\
         \"bedaquiline\"                               \n\
         \"capreomycin\"                               \n\
         \"clofazimine\"                               \n\
         \"delamanid\"                                 \n\
         \"ethambutol\"                                \n\
         \"ethionamide\"                               \n\
         \"fluoroquine\"                               \n\
         \"isoniazid\"                                 \n\
         \"kanamycin\"                                 \n\
         \"levofloxacin\"                              \n\
         \"linezolid\"                                 \n\
         \"moxifloxacin\"                              \n\
         \"penicillin-myceial-dreg\"                   \n\
         \"pyrazinamide\"                              \n\
         \"rifampicin\"                                \n\
         \"streptomycin\"                              \n\
      ]                                                \n\
   ---;                                                \n\
                                                       \n\
   ---set                                              \n\
      glob_amrShort                                    \n\
      [list                                            \n\
         \"Amk\" # amikacin                            \n\
         \"Bdq\" # bedaquiline                         \n\
         \"Cap\" # capreomycin                         \n\
         \"Cfz\" # clorfazimine                        \n\
         \"Dlm\" # delamanid                           \n\
         \"Emb\" # ethambutol                          \n\
         \"Eto\" # ethionamide                         \n\
         \"Flq\" # fluoroquine                         \n\
         \"Inz\" # isoniazid (also Iso, Inz, or Inh)   \n\
         \"Kan\" # kanamycin                           \n\
         \"Lfx\" # levofloxacin                        \n\
         \"Lzd\" # linezolid                           \n\
         \"Mfx\" # moxifloxacin                        \n\
         \"Pmd\" # penicillin-myceial-dreg             \n\
         \"Pza\" # pyrazinamide (Pyz, Pza, Pyr)        \n\
         \"Rif\" # rifampicin (Rif, Rmp, Rfm)          \n\
         \"Str\" # strptomycin (Str, Stp, Stm)         \n\
      ]                                                \n\
   ---;                                                \n\
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
   #   o header sec02 sub01 cat04:                     \n\
   #     - windows detect graphing script              \n\
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
      set programFiles $::env(PROGRAMFILES) ;          \n\
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
         set $::glob_mkGraphBl 0 ;                     \n\
                                                       \n\
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
      #++++++++++++++++++++++++++++++++++++++++++++++++\n\
      # Header Sec02 Sub01 Cat04:                      \n\
      #   - windows detect graphing script             \n\
      #++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
      set graphScript \"graphAmpDepth.r\" ;            \n\
                                                       \n\
      if { [file exists $graphScript] eq 0 } {         \n\
         ---set                                        \n\
            graphScript                                \n\
            [file join                                 \n\
             $programFiles                             \n\
             \"freezeTB\"                              \n\
             $graphScript                              \n\
            ]                                          \n\
         ---;                                          \n\
         if { [file exists $graphScript] eq 0 } {      \n\
            set $::glob_mkGraphBl 0 ;                  \n\
                                                       \n\
            ---tk_messageBox                           \n\
               -message \"graphAmpDepth.r not found\"  \n\
               -title \"no graphing script\"           \n\
            ---;                                       \n\
                                                       \n\
            set graphScript \"\" ;                     \n\
         } ; # If: could not find graphing script      \n\
      } ; # If: did not find graphing script           \n\
                                                       \n\
   #***************************************************\n\
   # Header Sec02 Sub02:                               \n\
   #   - unix program detection                        \n\
   #   o header sec02 sub02 cat01:                     \n\
   #     - unix; find minimap2 version (check if have) \n\
   #   o header sec02 sub02 cat02:                     \n\
   #     - unix find graphing script                   \n\
   #***************************************************\n\
                                                       \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
   # Header Sec02 Sub02 Cat01:                         \n\
   #   - unix; find minimap2 version (check if have)   \n\
   #+++++++++++++++++++++++++++++++++++++++++++++++++++\n\
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
                                                       \n\
      #++++++++++++++++++++++++++++++++++++++++++++++++\n\
      # Header Sec02 Sub02 Cat02:                      \n\
      #   - unix find graphing script                  \n\
      #++++++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
      set graphScript \"graphAmpDepth.r\" ;            \n\
                                                       \n\
      if { [file exists $graphScript] eq 0 } {         \n\
         ---set                                        \n\
            graphScript                                \n\
            [file join \"/usr/local/bin\" $graphScript]\n\
         ---;                                          \n\
                                                       \n\
         if { [file exists $graphScript] eq 0 } {      \n\
            set $::glob_mkGraphBl 0 ;                  \n\
                                                       \n\
            ---tk_messageBox                           \n\
               -message \"graphAmpDepth.r not found\"  \n\
               -title \"no graphing script\"           \n\
            ---;                                       \n\
                                                       \n\
            set graphScript \"\" ;                     \n\
         } ; # If: could not find graphing script      \n\
      } ; # If: did not find graphing script           \n\
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
      set $::glob_mkGraphBl 0 ;                        \n\
                                                       \n\
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
   #   o header sec03 sub01:                           \n\
   #     - set up default database finder function     \n\
   #   o header sec03 sub02:                           \n\
   #     - database global variable set up             \n\
   #   o header sec03 sub03:                           \n\
   #     - non-database default settings               \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
   #***************************************************\n\
   # Header Sec03 Sub01:                               \n\
   #   - set up default database finder function       \n\
   #***************************************************\n\
                                                       \n\
   proc getPath { fileStr } {                          \n\
                                                       \n\
     set dbPath $fileStr ;                             \n\
                                                       \n\
     if { [file exists $dbPath ] eq 0 } {              \n\
        set dbPath $::env(HOME) ;                      \n\
        set dbPath [ file join $dbPath \"Documents\" ] ;\n\
        set dbPath [ file join $dbPath \"freezeTBFiles\" ] ;\n\
        set dbPath [ file join $dbPath $fileStr ] ;    \n\
                                                       \n\
        if { [file exists $dbPath ] eq 0 } {           \n\
           set osStr [lindex $::tcl_platform(os) 0] ;  \n\
                                                       \n\
           if { osStr eq \"Windows\" } {               \n\
              ---set                                   \n\
                 dbPath                                \n\
                 [file join                            \n\
                  $::env(PUBLIC)                       \n\
                  \"Documents\"                        \n\
                  \"freezeTBFiles\"                    \n\
                  $fileStr                             \n\
                 ]                                     \n\
              ---;                                     \n\
                                                       \n\
              if { [file exists $dbPath ] eq 0 } {     \n\
                 set dbPath \"\" ;                     \n\
                 ---tk_messageBox                      \n\
                    -message [concat $fileStr \"not found\"]\n\
                    -title \"missing database\"        \n\
                 ---;                                  \n\
              } ; # If: database not in global location\n\
                                                       \n\
           } else {                                    \n\
             set dbPath \"/usr/local/share/freezeTBFiles\" ;\n\
              set dbPath [file join $dbPath $fileStr] ;\n\
                                                       \n\
              if { [file exists $dbPath ] eq 0 } {     \n\
                 set dbPath \"\" ;                     \n\
                 ---tk_messageBox                      \n\
                    -message [concat $fileStr \"not found\"]\n\
                    -title \"missing database\"        \n\
                 ---;                                  \n\
              } ; # If: database not in global location\n\
           } ; # check if windows or unix              \n\
         } ; # If: database not in HOME/Documents      \n\
      } ; # If: database not in directory              \n\
                                                       \n\
      return $dbPath ;                                 \n\
   } ; # getPath                                       \n\
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
      { {fastq.gz} {.gz} }                             \n\
      { {fastq.gz} {.gz}    }                          \n\
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
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\
   # Header Sec05:                                     \n\
   #   - freezeTB command                              \n\
   #   o header sec05 sub01:                           \n\
   #     - windows find freezeTB + function declaration\n\
   #   o header sec05 sub02:                           \n\
   #     - unix find freezeTB                          \n\
   #   o header sec05 sub03:                           \n\
   #     - run freezeTB                                \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\
                                                       \n\
                                                       \n\
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
                    -filetypes $::fq_types             \n\
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
   #     - run output part of gui                      \n\
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
                                                       \n\
                                                       \n\
         if { [catch {eval freezeTB $tbCmd } out] } {  \n\
                                                       \n\
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
         #   - run output part of freezeTB             \n\
         #+++++++++++++++++++++++++++++++++++++++++++++\n\
                                                       \n\
         wm title . \"Required freezeTB\" ;            \n\
                                                       \n\
         ---.main.reqIn.runexit.statuslab              \n\
            configure                                  \n\
            -text \"\"                                 \n\
         ---;                                          \n\
                                                       \n\
         upvar 0 glob_outCur curPrefix ;               \n\
         upvar 0 glob_outPref newPrefix ;              \n\
         set newPrefix $prefix ;                       \n\
         set curPrefix $prefix ;                       \n\
                                                       \n\
         .main.menu.outBut invoke ;                    \n\
         .main.out.set.run.but invoke ;                \n\
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
   #   o gui03 sec07:                                  \n\
   #     - set up output settings menu                 \n\
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
         pack forget .main.out ;                       \n\
                                                       \n\
         pack .main.reqIn -anchor w -side top ;        \n\
         pack .main.menu -anchor w -side top ;         \n\
                                                       \n\
         .main.menu.reqBut configure -relief sunken ;  \n\
         .main.menu.filtBut configure -relief raised ; \n\
         .main.menu.amrBut configure -relief raised ;  \n\
         .main.menu.linBut configure -relief raised ;  \n\
         .main.menu.conBut configure -relief raised ;  \n\
         .main.menu.outBut configure -relief raised ;  \n\
                                                       \n\
         .main.menu.reqBut configure -state disabled ; \n\
         .main.menu.filtBut configure -state normal ;  \n\
         .main.menu.amrBut configure -state normal ;   \n\
         .main.menu.linBut configure -state normal ;   \n\
         .main.menu.conBut configure -state normal ;   \n\
         .main.menu.outBut configure -state normal ;   \n\
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
         pack forget .main.out ;                       \n\
                                                       \n\
         pack .main.filt -anchor w -side top ;         \n\
         pack .main.menu -anchor w -side top ;         \n\
                                                       \n\
         .main.menu.reqBut configure -relief raised ;  \n\
         .main.menu.filtBut configure -relief sunken ; \n\
         .main.menu.amrBut configure -relief raised ;  \n\
         .main.menu.linBut configure -relief raised ;  \n\
         .main.menu.conBut configure -relief raised ;  \n\
         .main.menu.outBut configure -relief raised ;  \n\
                                                       \n\
         .main.menu.reqBut configure -state normal ;   \n\
         .main.menu.filtBut configure -state disabled ;\n\
         .main.menu.amrBut configure -state normal ;   \n\
         .main.menu.linBut configure -state normal ;   \n\
         .main.menu.conBut configure -state normal ;   \n\
         .main.menu.outBut configure -state normal ;   \n\
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
         .main.menu.outBut configure -relief raised ;  \n\
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
         pack forget .main.out ;                       \n\
                                                       \n\
         pack .main.lin -anchor w -side top ;          \n\
         pack .main.menu -anchor w -side top ;         \n\
                                                       \n\
         .main.menu.reqBut configure -relief raised ;  \n\
         .main.menu.filtBut configure -relief raised ; \n\
         .main.menu.amrBut configure -relief raised ;  \n\
         .main.menu.linBut configure -relief sunken ;  \n\
         .main.menu.conBut configure -relief raised ;  \n\
         .main.menu.outBut configure -relief raised ;  \n\
                                                       \n\
         .main.menu.reqBut configure -state normal ;   \n\
         .main.menu.filtBut configure -state normal ;  \n\
         .main.menu.amrBut configure -state normal ;   \n\
         .main.menu.linBut configure -state disabled ; \n\
         .main.menu.conBut configure -state normal ;   \n\
         .main.menu.outBut configure -state normal ;   \n\
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
         pack forget .main.out ;                       \n\
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
         .main.menu.outBut configure -relief raised ;  \n\
                                                       \n\
         .main.menu.reqBut configure -state normal ;   \n\
         .main.menu.filtBut configure -state normal ;  \n\
         .main.menu.amrBut configure -state normal ;   \n\
         .main.menu.linBut configure -state normal ;   \n\
         .main.menu.conBut configure -state disabled ; \n\
         .main.menu.outBut configure -state normal ;   \n\
                                                       \n\
   } ; # command button fires                          \n\
                                                       \n\
   pack .main.menu.conBut -anchor w -side left         \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui03 Sec07:                                      \n\
   #   - set up output settings menu                   \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.menu.outBut                                \n\
      -text \"output\"                                 \n\
      -relief raised                                   \n\
      -state normal                                    \n\
      -command {                                       \n\
   ---                                                 \n\
                                                       \n\
         pack forget .main.reqIn ;                     \n\
         pack forget .main.filt ;                      \n\
         pack forget .main.amr ;                       \n\
         pack forget .main.lin ;                       \n\
         pack forget .main.con ;                       \n\
         pack forget .main.menu ;                      \n\
         pack forget .main.out ;                       \n\
                                                       \n\
         .main.menu.reqBut configure -relief raised ;  \n\
         .main.menu.filtBut configure -relief raised ; \n\
         .main.menu.amrBut configure -relief raised ;  \n\
         .main.menu.linBut configure -relief raised ;  \n\
         .main.menu.conBut configure -relief raised ;  \n\
         .main.menu.outBut configure -relief raised ;  \n\
                                                       \n\
         .main.menu.reqBut configure -state normal ;   \n\
         .main.menu.filtBut configure -state normal ;  \n\
         .main.menu.amrBut configure -state normal ;   \n\
         .main.menu.linBut configure -state normal ;   \n\
         .main.menu.conBut configure -state normal ;   \n\
         .main.menu.outBut configure -state normal ;   \n\
                                                       \n\
         pack .main.out ;                              \n\
         .main.out.menu.setBut invoke ;                \n\
      } ; # output settings button                     \n\
                                                       \n\
   pack .main.menu.outBut -anchor w -side left ;       \n\
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
                 -filetypes $::fa_types                \n\
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
                 -filetypes $::tsv_types               \n\
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
               -filetypes $::tsv_types                 \n\
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
   tk::label .main.amr.db.lab -text $glob_amrDb ;      \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.amr.db.but                                 \n\
      -text \"AMR database\"                           \n\
      -command {                                       \n\
   ---                                                 \n\
         set tsvFile [                                 \n\
            ---tk_getOpenFile                          \n\
               -title \"select AMR database\"          \n\
               -filetypes $::tsv_types                 \n\
            ---] ;                                     \n\
          if {$tsvFile eq \"\" } {                     \n\
                                                       \n\
          } else {                                     \n\
            .main.amr.db.lab configure -text $tsvFile ;\n\
            set ::glob_amrDb $tsvFile ;                \n\
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
         set tsvFile [                                 \n\
            ---tk_getOpenFile                          \n\
               -title \"select MIRU-VNTR database\"    \n\
               -filetypes $::tsv_types                 \n\
            --- ] ; # get MIRU databse                 \n\
                                                       \n\
         if { $tsvFile eq \"\" } {                     \n\
                                                       \n\
         } else {                                      \n\
            ---.main.lin.db.miru.lab                   \n\
               configure                               \n\
               -text $tsvFile                          \n\
             ---;                                      \n\
             set ::glob_miruDb $tsvFile ;              \n\
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
               -filetypes $::fa_types                  \n\
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
               -filetypes $::csv_types                 \n\
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
   #   o gui08 sec01:                                  \n\
   #     - set up output gui frame                     \n\
   #   o gui08 sec02:                                  \n\
   #     - function setup; report file                 \n\
   #   o gui08 sec03:                                  \n\
   #     - set up settings output gui frame            \n\
   #   o gui08 sec04:                                  \n\
   #     - set up output gui menu                      \n\
   #   o gui08 sec05:                                  \n\
   #     - set up report gui                           \n\
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui08 Sec01:                                      \n\
   #   - set up output gui frame                       \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::frame .main.out ;                               \n\
                                                       \n\
   tk::frame .main.out.set ;                           \n\
   pack .main.out.set -anchor w -side top ;            \n\
                                                       \n\
   tk::frame .main.out.report ;                        \n\
   tk::frame .main.out.depth ;                         \n\
   tk::frame .main.out.cover ;                         \n\
   tk::frame .main.out.amr ;                           \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui08 Sec02:                                      \n\
   #   - set up report file parsing functions          \n\
   #   o gui08 sec02 sub01:                            \n\
   #     - functioun consensus AMR report              \n\
   #   o gui08 sec02 sub02:                            \n\
   #     - function read AMR report                    \n\
   #   o gui08 sec02 sub03:                            \n\
   #     - function; consensus spoligotype             \n\
   #   o gui08 sec02 sub04:                            \n\
   #     - function; read spoligotype                  \n\
   #   o gui08 sec02 sub05:                            \n\
   #     - function consensus MIRU-VNTR                \n\
   #   o gui08 sec02 sub06:                            \n\
   #     - function; read depth graph                  \n\
   #   o gui08 sec02 sub07:                            \n\
   #     - function; coverage graph                    \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec02 Sub01:                                \n\
   #   - function; consensus AMR report                \n\
   #************************************************** \n\
                                                       \n\
   proc conAmrRep {prefixStr} {                        \n\
      set fileStr $prefixStr ;                         \n\
      append fileStr \"-con-amrs.tsv\" ;               \n\
                                                       \n\
      set openFILE [open $fileStr] ;                   \n\
      gets $openFILE line ; # get header line          \n\
      set amrList \"\" ;                               \n\
                                                       \n\
      while {[gets $openFILE line] > -1} {             \n\
         set line [split $line \"\\t\"] ;              \n\
         set tmpStr [lindex $line 2] ;                 \n\
         set amrList [concat $amrList $tmpStr] ;       \n\
                                                       \n\
         if { [lindex $line 3] ne \"NA\" } {           \n\
            set tmpStr [split [lindex $line 3] \"_\"] ;\n\
            set amrList [concat $amrList $tmpStr] ;    \n\
         } ; # If: have cross resistance               \n\
                                                       \n\
         # remove duplicates                           \n\
         set amrList [lsort -unique $amrList] ;        \n\
      } ; # Loop: read file by line                    \n\
                                                       \n\
      # make sure always lower case                    \n\
      close $openFILE ;                                \n\
      set amrList [string tolower $amrList] ;          \n\
                                                       \n\
      ---for                                           \n\
         { set siAmr 0 }                               \n\
         { $siAmr < [llength $::glob_amrList] }        \n\
         { incr siAmr }                                \n\
      --- {                                            \n\
         set tmpStr [lindex $::glob_amrList $siAmr] ;  \n\
         set tmpStr [string tolower $tmpStr] ;         \n\
                                                       \n\
         set labStr [lindex $::glob_amrShort $siAmr] ; \n\
         set labStr [string tolower $labStr] ;         \n\
         append labStr \"lab\" ;                       \n\
                                                       \n\
         if { [lsearch $amrList $tmpStr] ne -1 } {     \n\
            ---.main.out.report.conAmr.$labStr         \n\
               configure                               \n\
               -background $::glob_amrCol              \n\
            ---;                                       \n\
         } else {                                      \n\
            ---.main.out.report.conAmr.$labStr         \n\
               configure                               \n\
               -background $::glob_noAmrCol            \n\
            ---;                                       \n\
         } ; # check if have AMR                       \n\
      } ; # Loop: build amr labels                     \n\
   } ; # conAmrRep                                     \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec02 Sub02:                                \n\
   #   - function read AMR report                      \n\
   #************************************************** \n\
                                                       \n\
   proc readAmrRep {prefixStr} {                       \n\
      set fileStr $prefixStr ;                         \n\
      append fileStr \"-read-amrs.tsv\" ;              \n\
                                                       \n\
      set openFILE [open $fileStr] ;                   \n\
      gets $openFILE line ; # get header line          \n\
      set amrList \"\" ;                               \n\
                                                       \n\
      while {[gets $openFILE line] > -1} {             \n\
         set line [split $line \"\\t\"] ;              \n\
         set tmpStr [lindex $line 4] ;                 \n\
                                                       \n\
         # get percent report and reduce to decimal    \n\
         set tmpF [lindex $line 8] ;                   \n\
         set tmpF [expr $tmpF / 100] ;                 \n\
                                                       \n\
         if {$tmpStr eq \"snp\"} {                     \n\
            if {$tmpF < $::glob_outSnpSup} {           \n\
                continue ;                             \n\
            } ; # If: support is to low                \n\
         } else {                                      \n\
            if {$tmpF < $::glob_outIndelSup} {         \n\
                continue ;                             \n\
            } ; # If: support is to low                \n\
         } ; # check if snp or indel                   \n\
                                                       \n\
         set tmpStr [lindex $line 1] ;                 \n\
         set amrList [concat $amrList $tmpStr] ;       \n\
                                                       \n\
         if { [lindex $line 2] ne \"NA\" } {           \n\
            set tmpStr [split [lindex $line 2] \"_\"] ;\n\
            set amrList [concat $amrList $tmpStr] ;    \n\
         } ; # If: have cross resistance               \n\
                                                       \n\
         # remove duplicates                           \n\
         set amrList [lsort -unique $amrList] ;        \n\
      } ; # Loop: read file by line                    \n\
                                                       \n\
      # make sure always lower case                    \n\
      close $openFILE ;                                \n\
      set amrList [string tolower $amrList] ;          \n\
                                                       \n\
      ---for                                           \n\
         { set siAmr 0 }                               \n\
         { $siAmr < [llength $::glob_amrList] }        \n\
         { incr siAmr }                                \n\
      --- {                                            \n\
         set tmpStr [lindex $::glob_amrList $siAmr] ;  \n\
         set tmpStr [string tolower $tmpStr] ;         \n\
                                                       \n\
         set labStr [lindex $::glob_amrShort $siAmr] ; \n\
         set labStr [string tolower $labStr] ;         \n\
         append labStr \"lab\" ;                       \n\
                                                       \n\
         if { [lsearch $amrList $tmpStr] ne -1 } {     \n\
            ---.main.out.report.readAmr.$labStr        \n\
               configure                               \n\
               -background $::glob_amrCol              \n\
            ---;                                       \n\
         } else {                                      \n\
            ---.main.out.report.readAmr.$labStr        \n\
               configure                               \n\
               -background $::glob_noAmrCol            \n\
            ---;                                       \n\
         } ; # check if have AMR                       \n\
      } ; # Loop: build amr labels                     \n\
   } ; # readAmrRep                                    \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec02 Sub03:                                \n\
   #   - function; consensus spoligotype               \n\
   #************************************************** \n\
                                                       \n\
   proc conSpol {prefixStr} {                          \n\
      set fileStr $prefixStr ;                         \n\
      append fileStr \"-con-spoligo.tsv\" ;            \n\
      set openFILE [open $fileStr] ;                   \n\
                                                       \n\
      gets $openFILE lineStr ; # get header line       \n\
      set statusBl [gets $openFILE lineStr ] ;         \n\
      close $openFILE ;                                \n\
                                                       \n\
      if {$statusBl < 0} {                             \n\
         ---.main.out.report.conspol.octal.reslab      \n\
            configure                                  \n\
            -text \"NA\"                               \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.report.conspol.sit.reslab        \n\
            configure                                  \n\
            -text \"NA\"                               \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.report.conspol.strain.reslab     \n\
            configure                                  \n\
            -text \"NA\"                               \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.report.conspol.orig.reslab       \n\
            configure                                  \n\
            -text \"NA\"                               \n\
         ---;                                          \n\
                                                       \n\
         return false ;                                \n\
      } ; # If: no spoligotype entry                   \n\
                                                       \n\
      set lineStr [split $lineStr \"\\t\"] ;           \n\
                                                       \n\
      # each value is either NA or has actual value    \n\
                                                       \n\
      ---.main.out.report.conspol.octal.reslab         \n\
         configure                                     \n\
         -text [lindex $lineStr 3]                     \n\
      ---;                                             \n\
                                                       \n\
      ---.main.out.report.conspol.sit.reslab           \n\
         configure                                     \n\
         -text [lindex $lineStr 5]                     \n\
      ---;                                             \n\
                                                       \n\
      ---.main.out.report.conspol.strain.reslab        \n\
         configure                                     \n\
         -text [lindex $lineStr 1]                     \n\
      ---;                                             \n\
                                                       \n\
      ---.main.out.report.conspol.orig.reslab          \n\
         configure                                     \n\
         -text [lindex $lineStr 6]                     \n\
      ---;                                             \n\
   } ; # conSpol                                       \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec02 Sub04:                                \n\
   #   - function; read spoligotype                    \n\
   #************************************************** \n\
                                                       \n\
   proc readSpol {prefixStr} {                         \n\
      set fileStr $prefixStr ;                         \n\
      append fileStr \"-read-spoligo.tsv\" ;           \n\
      set openFILE [open $fileStr] ;                   \n\
                                                       \n\
      gets $openFILE lineStr ; # get header line       \n\
      set statusBl [gets $openFILE lineStr ] ;         \n\
      close $openFILE ;                                \n\
                                                       \n\
      if {$statusBl < 0} {                             \n\
         ---.main.out.report.readspol.octal.reslab     \n\
            configure                                  \n\
            -text \"NA\"                               \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.report.readspol.sit.reslab       \n\
            configure                                  \n\
            -text \"NA\"                               \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.report.readspol.strain.reslab    \n\
            configure                                  \n\
            -text \"NA\"                               \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.report.readspol.orig.reslab      \n\
            configure                                  \n\
            -text \"NA\"                               \n\
         ---;                                          \n\
                                                       \n\
         return false ;                                \n\
      } ; # If: no spoligotype entry                   \n\
                                                       \n\
      set lineStr [split $lineStr \"\\t\"] ;           \n\
                                                       \n\
      # each value is either NA or has actual value    \n\
                                                       \n\
      ---.main.out.report.readspol.octal.reslab        \n\
         configure                                     \n\
         -text [lindex $lineStr 3]                     \n\
      ---;                                             \n\
                                                       \n\
      ---.main.out.report.readspol.sit.reslab          \n\
         configure                                     \n\
         -text [lindex $lineStr 5]                     \n\
      ---;                                             \n\
                                                       \n\
      ---.main.out.report.readspol.strain.reslab       \n\
         configure                                     \n\
         -text [lindex $lineStr 1]                     \n\
      ---;                                             \n\
                                                       \n\
      ---.main.out.report.readspol.orig.reslab         \n\
         configure                                     \n\
         -text [lindex $lineStr 6]                     \n\
      ---;                                             \n\
   } ; # readSpol                                      \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec02 Sub05:                                \n\
   #   - function consensus MIRU-VNTR                  \n\
   #************************************************** \n\
                                                       \n\
   proc conMiru {prefixStr} {                          \n\
      set fileStr $prefixStr ;                         \n\
      append fileStr \"-con-miru.tsv\" ;               \n\
      set openFILE [open $fileStr] ;                   \n\
                                                       \n\
      gets $openFILE lineStr ; # get header            \n\
      set status [gets $openFILE lineStr] ;            \n\
      close $openFILE ;                                \n\
                                                       \n\
      if {$status < 0} {                               \n\
         ---.main.out.report.miru.reslab               \n\
            configure                                  \n\
            -text \"NA\"                               \n\
         ---;                                          \n\
         return false ;                                \n\
      } ; # If: no entry                               \n\
                                                       \n\
      set lineStr [split $lineStr \"\\t\"] ;           \n\
                                                       \n\
      # need to remove lineage index                   \n\
      set lineStr [lreplace $lineStr 0 0];             \n\
                                                       \n\
      set tmpStr [lsort -unique $lineStr ] ;           \n\
                                                       \n\
      if {[llength $tmpStr] < 2} {                     \n\
         if {[lindex $tmpStr 0] eq \"NA\"} {           \n\
            ---.main.out.report.miru.reslab            \n\
               configure                               \n\
               -text \"NA\"                            \n\
            ---;                                       \n\
            return false ;                             \n\
         } ; # If: no lineage detected                 \n\
      } ; # If: have many duplicates                   \n\
                                                       \n\
      ---.main.out.report.miru.reslab                  \n\
         configure                                     \n\
         -text $lineStr                                \n\
      ---;                                             \n\
                                                       \n\
      return true ;                                    \n\
   } ; # conMiru                                       \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec02 Sub06:                                \n\
   #   - function; read depth graph display            \n\
   #************************************************** \n\
                                                       \n\
   proc depthGraph {prefixStr} {                       \n\
      # delete old graphs                              \n\
      if {! [image inuse $::glob_depthImg] } {         \n\
            image delete $::glob_depthImg ;            \n\
      } ; # If: image exists                           \n\
                                                       \n\
      if {$::glob_mkGraphBl ne 0 } {                   \n\
                                                       \n\
         set tmpStr $prefixStr ;                       \n\
         append tmpStr \"-depths.tsv\" ;               \n\
                                                       \n\
         set tmpPathStr $prefixStr ;                   \n\
         append tmpPathStr \"-readDepth.png\" ;        \n\
                                                       \n\
         if { [file exists $tmpPathStr] eq 1 } {       \n\
            ---set                                     \n\
              ::glob_depthImg                          \n\
              [image create photo -file $tmpPathStr]   \n\
            ---;                                       \n\
                                                       \n\
            ---.main.out.depth.graph                   \n\
               configure                               \n\
               -image $::glob_depthImg                 \n\
            ---;                                       \n\
                                                       \n\
         } else {                                      \n\
            ---set status                              \n\
               [catch                                  \n\
                  {exec                                \n\
                     $::rPath                          \n\
                     $::graphScript                    \n\
                     -stats $tmpStr                    \n\
                     -who $::glob_amrDb                \n\
                     -prefix $prefixStr                \n\
                  }                                    \n\
               ]                                       \n\
            ---; # run R to build graphs               \n\
                                                       \n\
            if { $status ne 0 } {                      \n\
               ---tk_messageBox                        \n\
                 -message \"failed to build depth graph\"\n\
                 -title \"ERROR\"                      \n\
               ---;                                    \n\
                                                       \n\
               # turn off graphing                     \n\
               .main.out.set.graph.check toggle ;      \n\
            } else {                                   \n\
               ---set                                  \n\
                 ::glob_depthImg                       \n\
                 [image                                \n\
                    create                             \n\
                    photo                              \n\
                    -file $tmpPathStr                  \n\
                 ]                                     \n\
               ---;                                    \n\
                                                       \n\
               ---.main.out.depth.graph                \n\
                  configure                            \n\
                  -image $::glob_depthImg              \n\
               ---;                                    \n\
            } ; # Else: add image                      \n\
         } ; # check if need to build graphs           \n\
       } ; # check if users wants graphs displayed     \n\
   } ; # depthGraph                                    \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec02 Sub07:                                \n\
   #   - function; coverage graph dispaly              \n\
   #************************************************** \n\
                                                       \n\
   proc coverageGraph {prefixStr} {                    \n\
      # delete old graphs                              \n\
      if {! [image inuse $::glob_coverImg] } {         \n\
            image delete $::glob_coverImg ;            \n\
      } ; # If: image exists                           \n\
                                                       \n\
      if {$::glob_mkGraphBl ne 0 } {                   \n\
         set tmpStr $prefixStr ;                       \n\
         append tmpStr \"-covers.tsv\" ;               \n\
                                                       \n\
         set tmpPathStr $prefixStr ;                   \n\
         append tmpPathStr \"-coverage.png\" ;         \n\
                                                       \n\
         if { [file exists $tmpPathStr] eq 1 } {       \n\
            ---set                                     \n\
              ::glob_coverImg                          \n\
              [image create photo -file $tmpPathStr]   \n\
            ---;                                       \n\
                                                       \n\
            ---.main.out.cover.graph                   \n\
               configure                               \n\
               -image $::glob_coverImg                 \n\
            ---;                                       \n\
                                                       \n\
         } else {                                      \n\
            ---set status                              \n\
               [catch                                  \n\
                  {exec                                \n\
                     $::rPath                          \n\
                     $::graphScript                    \n\
                     -stats $tmpStr                    \n\
                     -who $::glob_amrDb                \n\
                     -prefix $prefixStr                \n\
                  }                                    \n\
               ]                                       \n\
            ---; # run R to build graphs               \n\
                                                       \n\
            if { $status ne 0 } {                      \n\
               ---tk_messageBox                        \n\
                 -message \"failed to build cover graph\"\n\
                 -title \"ERROR\"                      \n\
               ---;                                    \n\
                                                       \n\
               # turn off graphing                     \n\
               .main.out.set.graph.check toggle ;      \n\
            } else {                                   \n\
               ---set                                  \n\
                 ::glob_coverImg                       \n\
                 [image                                \n\
                    create                             \n\
                    photo                              \n\
                    -file $tmpPathStr                  \n\
                 ]                                     \n\
               ---;                                    \n\
                                                       \n\
               ---.main.out.cover.graph                \n\
                  configure                            \n\
                  -image $::glob_coverImg              \n\
               ---;                                    \n\
            } ; # Else: add image                      \n\
         } ; # check if need to build graphs           \n\
       } ; # check if users wants graphs displayed     \n\
   } ; # coverageGraph                                 \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec02 Sub08:                                \n\
   #   - function; consnesus AMR table                 \n\
   #   o gui08 sec02 sub08 cat01:                      \n\
   #     - open file and get header lenghts            \n\
   #   o gui08 sec02 sub08 cat02:                      \n\
   #     - get gene name length + start length loop    \n\
   #   o gui08 sec02 sub08 cat03:                      \n\
   #     - get AMR (short name) length                 \n\
   #   o gui08 sec02 sub08 cat04:                      \n\
   #     - get AMR cross resistance column length      \n\
   #   o gui08 sec02 sub08 cat05:                      \n\
   #     - find length of mutation column              \n\
   #   o gui08 sec02 sub08 cat06:                      \n\
   #     - print no AMR case                           \n\
   #   o gui08 sec02 sub08 cat07:                      \n\
   #     - print header                                \n\
   #   o gui08 sec02 sub08 cat08:                      \n\
   #     - get gene ids + start print loop             \n\
   #   o gui08 sec02 sub08 cat09:                      \n\
   #     - get primrary resistant drug (3 letter)      \n\
   #   o gui08 sec02 sub08 cat10:                      \n\
   #     - get cross resistant drugs (3 letter)        \n\
   #   o gui08 sec02 sub08 cat11:                      \n\
   #     - get mutation column                         \n\
   #   o gui08 sec02 sub08 cat12:                      \n\
   #     - add text to consensus AMR table + return    \n\
   #************************************************** \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec02 Sub08 Cat01:                          \n\
   #   - open file and get header lenghts              \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   proc conAmrTbl {prefixStr indentStr} {              \n\
      set fileStr $prefixStr ;                         \n\
      append fileStr \"-con-amrs.tsv\" ;               \n\
      set openFILE [open $fileStr] ;                   \n\
                                                       \n\
      set pad1 [string length \"gene\"] ;              \n\
      set pad2 [string length \"drug\"] ;              \n\
      set pad3 [string length \"cross-res\"] ;         \n\
      set pad5 [string length \"mutant\"];             \n\
                                                       \n\
      if {[gets $openFILE lineStr] < 0} {              \n\
         ---.main.out.amr.read.tbl.lab                 \n\
             configure                                 \n\
             -text \"NA\"                              \n\
         ---;                                          \n\
                                                       \n\
         return false ;                                \n\
      } ; # If; nothing in file                        \n\
                                                       \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
      # Gui08 Sec02 Sub08 Cat02:                       \n\
      #   - get gene name length + start length loop   \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
      while {[gets $openFILE lineStr] > -1} {          \n\
         set lineStr [split $lineStr \"\\t\"] ;        \n\
                                                       \n\
         # gene name length                            \n\
         set tmpStr [lindex $lineStr 1] ;              \n\
         set lenUI [string length $tmpStr] ;           \n\
         if {$pad1 < $lenUI} { set pad1 $lenUI ; } ;   \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub08 Cat03:                    \n\
         #   - get AMR (short name) length             \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         set tmpStr [lindex $lineStr 2] ;              \n\
         set tmpStr [string tolower $tmpStr] ;         \n\
                                                       \n\
         # loop converts drug name to short hand       \n\
         ---for                                        \n\
           { set siAmr 0 }                             \n\
           { $siAmr < [llength $::glob_amrList] }      \n\
           { incr siAmr }                              \n\
         ---{                                          \n\
            ---set                                     \n\
               tmpStr                                  \n\
               [regsub                                 \n\
                  [lindex $::glob_amrList $siAmr]      \n\
                  $tmpStr                              \n\
                  [lindex $::glob_amrShort $siAmr]     \n\
               ]                                       \n\
            ---;                                       \n\
         } ; # Loop: convert AMR ids to short hand     \n\
                                                       \n\
         set lenUI [string length $tmpStr] ;           \n\
         if {$pad2 < $lenUI} { set pad2 $lenUI ; } ;   \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub08 Cat04:                    \n\
         #   - get AMR cross resistance column length  \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         # cross resistance length                     \n\
         set tmpStr [lindex $lineStr 3] ;              \n\
         set tmpStr [string tolower $tmpStr] ;         \n\
                                                       \n\
         # loop converts drug names to three letters   \n\
         ---for                                        \n\
           { set siAmr 0 }                             \n\
           { $siAmr < [llength $::glob_amrList] }      \n\
           { incr siAmr }                              \n\
         ---{                                          \n\
            ---set                                     \n\
               tmpStr                                  \n\
               [regsub                                 \n\
                  [lindex $::glob_amrList $siAmr]      \n\
                  $tmpStr                              \n\
                  [lindex $::glob_amrShort $siAmr]     \n\
               ]                                       \n\
            ---;                                       \n\
         } ; # Loop: convert AMR ids to short hand     \n\
                                                       \n\
         set lenUI [string length $tmpStr] ;           \n\
         if {$pad3 < $lenUI} { set pad3 $lenUI ; } ;   \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub08 Cat05:                    \n\
         #   - find length of mutation column          \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         set tmpStr [lindex $lineStr 5] ;              \n\
         set lenUI [string length $tmpStr] ;           \n\
         if {$pad5 < $lenUI} { set pad5 $lenUI ; } ;   \n\
      } ; # Loop: find longest entries per column      \n\
                                                       \n\
      close $openFILE ;                                \n\
                                                       \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
      # Gui08 Sec02 Sub08 Cat06:                       \n\
      #   - print no AMR case                          \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
      if {$pad1 eq 0} {                                \n\
         ---.main.out.amr.con.tbl.lab                  \n\
             configure                                 \n\
             -text \"NA\"                              \n\
         ---;                                          \n\
                                                       \n\
         return false ;                                \n\
      } ; # If; nothing in file                        \n\
                                                       \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
      # Gui08 Sec02 Sub08 Cat07:                       \n\
      #   - print header                               \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
      set tmpStr [format \"%-*s\" $pad1 \"gene\"] ;    \n\
      append tblStr $tmpStr $indentStr;                \n\
                                                       \n\
      set tmpStr [format \"%-*s\" $pad2 \"drug\"] ;    \n\
      append tblStr $tmpStr $indentStr;                \n\
                                                       \n\
      set tmpStr [format \"%-*s\" $pad3 \"cross-res\"] ;\n\
      append tblStr $tmpStr $indentStr;                \n\
                                                       \n\
      set tmpStr [format \"%-*s\" $pad5 \"mutant\"] ;  \n\
      append tblStr $tmpStr $indentStr;                \n\
                                                       \n\
      append tblStr \"\\n\";                           \n\
                                                       \n\
      set openFILE [open $fileStr] ;                   \n\
      gets $openFILE lineStr ;                         \n\
                                                       \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
      # Gui08 Sec02 Sub08 Cat08:                       \n\
      #   - get gene ids + start print loop            \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
      while {[gets $openFILE lineStr] > -1} {          \n\
         set lineStr [split $lineStr \"\\t\"] ;        \n\
                                                       \n\
         set tmpStr [lindex $lineStr 1] ; # gene id    \n\
         set tmpStr [format \"%-*s\" $pad1 $tmpStr] ;  \n\
         append tblStr $tmpStr $indentStr;             \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub08 Cat09:                    \n\
         #   - get primrary resistant drug (3 letter)  \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         set tmpStr [lindex $lineStr 2] ;              \n\
         set tmpStr [string tolower $tmpStr] ;         \n\
                                                       \n\
         # loop converts to three letter code          \n\
         ---for                                        \n\
           { set siAmr 0 }                             \n\
           { $siAmr < [llength $::glob_amrList] }      \n\
           { incr siAmr }                              \n\
         ---{                                          \n\
            ---set                                     \n\
               tmpStr                                  \n\
               [regsub                                 \n\
                  [lindex $::glob_amrList $siAmr]      \n\
                  $tmpStr                              \n\
                  [lindex $::glob_amrShort $siAmr]     \n\
               ]                                       \n\
            ---;                                       \n\
         } ; # Loop: convert AMR ids to short hand     \n\
         set tmpStr [format \"%-*s\" $pad2 $tmpStr] ;  \n\
         append tblStr $tmpStr $indentStr;             \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub08 Cat10:                    \n\
         #   - get cross resistant drugs (3 letter)    \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         # cross resistance AMRs                       \n\
         set tmpStr [lindex $lineStr 3] ;              \n\
         set tmpStr [string tolower $tmpStr] ;         \n\
                                                       \n\
         # loop converts drug to three letter codes    \n\
         ---for                                        \n\
           { set siAmr 0 }                             \n\
           { $siAmr < [llength $::glob_amrList] }      \n\
           { incr siAmr }                              \n\
         ---{                                          \n\
            ---set                                     \n\
               tmpStr                                  \n\
               [regsub                                 \n\
                  [lindex $::glob_amrList $siAmr]      \n\
                  $tmpStr                              \n\
                  [lindex $::glob_amrShort $siAmr]     \n\
               ]                                       \n\
            ---;                                       \n\
         } ; # Loop: convert AMR ids to short hand     \n\
                                                       \n\
         set tmpStr [format \"%-*s\" $pad3 $tmpStr] ;  \n\
         append tblStr $tmpStr $indentStr;             \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub08 Cat11:                    \n\
         #   - get mutation column                     \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         set tmpStr [lindex $lineStr 5] ; # mut type   \n\
         set tmpStr [format \"%-*s\" $pad5 $tmpStr] ;  \n\
         append tblStr $tmpStr $indentStr;             \n\
                                                       \n\
         append tblStr \"\\n\" ; # next row            \n\
      } ; # Loop: get AMRs                             \n\
                                                       \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
      # Gui08 Sec02 Sub08 Cat12:                       \n\
      #   - add text to consensus AMR table + return   \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
      ---.main.out.amr.con.tbl.lab                     \n\
         configure                                     \n\
         -text $tblStr                                 \n\
      ---;                                             \n\
                                                       \n\
      close $openFILE ;                                \n\
      return true ;                                    \n\
   } ; # conAmrTbl                                     \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec02 Sub09:                                \n\
   #   - function; read AMR table                      \n\
   #   o gui08 sec02 sub09 cat01:                      \n\
   #     - open file and get header lengths            \n\
   #   o gui08 sec02 sub09 cat02:                      \n\
   #     - find gene id length + start length loop     \n\
   #   o gui08 sec02 sub09 cat03:                      \n\
   #     - find primrary drug length                   \n\
   #   o gui08 sec02 sub09 cat04:                      \n\
   #     - find cross resitance drugs length           \n\
   #   o gui08 sec02 sub09 cat05:                      \n\
   #     - find mutation and % support lengths         \n\
   #   o gui08 sec02 sub09 cat06:                      \n\
   #     - print NA for nothing in file case           \n\
   #   o gui08 sec02 sub09 cat07:                      \n\
   #     - build header for table                      \n\
   #   o gui08 sec02 sub09 cat08:                      \n\
   #     - add gene id to header + start table loop    \n\
   #   o gui08 sec02 sub09 cat09:                      \n\
   #     - add primrary drug to header                 \n\
   #   o gui08 sec02 sub09 cat10:                      \n\
   #     - add cross-resistance drugs to header        \n\
   #   o gui08 sec02 sub09 cat11:                      \n\
   #     - add mutation and % support to table         \n\
   #   o gui08 sec02 sub09 cat12:                      \n\
   #     - put table into AMR read table label         \n\
   #************************************************** \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec02 Sub09 Cat01:                          \n\
   #   - open file and get header lengths              \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   proc readAmrTbl {prefixStr indentStr} {             \n\
      set fileStr $prefixStr ;                         \n\
      append fileStr \"-read-amrs.tsv\" ;              \n\
      set openFILE [open $fileStr] ;                   \n\
                                                       \n\
      set pad0 [string length \"gene\"] ;              \n\
      set pad1 [string length \"drug\"] ;              \n\
      set pad2 [string length \"cross-res\"] ;         \n\
      set pad4 [string length \"mutant\"];             \n\
      set pad8 [string length \"%sup\"];               \n\
                                                       \n\
      if {[gets $openFILE lineStr] < 0} {              \n\
         ---.main.out.amr.read.tbl.lab                 \n\
             configure                                 \n\
             -text \"NA\"                              \n\
         ---;                                          \n\
                                                       \n\
         return false ;                                \n\
      } ; # If; nothing in file                        \n\
                                                       \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
      # Gui08 Sec02 Sub09 Cat02:                       \n\
      #   - find gene id length + start length loop    \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
      while {[gets $openFILE lineStr] > -1} {          \n\
         set lineStr [split $lineStr \"\\t\"] ;        \n\
                                                       \n\
         # gene name length                            \n\
         set tmpStr [lindex $lineStr 0] ;              \n\
         set lenUI [string length $tmpStr] ;           \n\
         if {$pad0 < $lenUI} { set pad0 $lenUI ; } ;   \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub09 Cat03:                    \n\
         #   - find primrary drug length               \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         set tmpStr [lindex $lineStr 1] ;              \n\
         set tmpStr [string tolower $tmpStr] ;         \n\
                                                       \n\
         # reducing to three letter code if possible   \n\
         ---for                                        \n\
           { set siAmr 0 }                             \n\
           { $siAmr < [llength $::glob_amrList] }      \n\
           { incr siAmr }                              \n\
         ---{                                          \n\
            ---set                                     \n\
               tmpStr                                  \n\
               [regsub                                 \n\
                  [lindex $::glob_amrList $siAmr]      \n\
                  $tmpStr                              \n\
                  [lindex $::glob_amrShort $siAmr]     \n\
               ]                                       \n\
            ---;                                       \n\
         } ; # Loop: convert AMR ids to short hand     \n\
                                                       \n\
         set lenUI [string length $tmpStr] ;           \n\
         if {$pad1 < $lenUI} { set pad1 $lenUI ; } ;   \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub09 Cat04:                    \n\
         #   - find cross resitance drugs length       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         # cross resistance length                     \n\
         set tmpStr [lindex $lineStr 2] ;              \n\
         set tmpStr [string tolower $tmpStr] ;         \n\
                                                       \n\
         # reducing to three letter code if possible   \n\
         ---for                                        \n\
           { set siAmr 0 }                             \n\
           { $siAmr < [llength $::glob_amrList] }      \n\
           { incr siAmr }                              \n\
         ---{                                          \n\
            ---set                                     \n\
               tmpStr                                  \n\
               [regsub                                 \n\
                  [lindex $::glob_amrList $siAmr]      \n\
                  $tmpStr                              \n\
                  [lindex $::glob_amrShort $siAmr]     \n\
               ]                                       \n\
            ---;                                       \n\
         } ; # Loop: convert AMR ids to short hand     \n\
                                                       \n\
         set lenUI [string length $tmpStr] ;           \n\
         if {$pad2 < $lenUI} { set pad2 $lenUI ; } ;   \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub09 Cat05:                    \n\
         #   - find mutation and % support lengths     \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         # mutation length                             \n\
         set tmpStr [lindex $lineStr 4] ;              \n\
         set lenUI [string length $tmpStr] ;           \n\
         if {$pad4 < $lenUI} { set pad4 $lenUI ; } ;   \n\
                                                       \n\
         # percent support length                      \n\
         set tmpStr [lindex $lineStr 8] ;              \n\
         set lenUI [string length $tmpStr] ;           \n\
         if {$pad8 < $lenUI} { set pad8 $lenUI ; } ;   \n\
      } ; # Loop: find longest entries per column      \n\
                                                       \n\
      close $openFILE ;                                \n\
                                                       \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
      # Gui08 Sec02 Sub09 Cat06:                       \n\
      #   - print NA for nothing in file case          \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
      if {$pad0 eq 0} {                                \n\
         ---.main.out.amr.read.tbl.lab                 \n\
             configure                                 \n\
             -text \"NA\"                              \n\
         ---;                                          \n\
                                                       \n\
         return false ;                                \n\
      } ; # If; nothing in file                        \n\
                                                       \n\
                                                       \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
      # Gui08 Sec02 Sub09 Cat07:                       \n\
      #   - build header for table                     \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
      set tmpStr [format \"%-*s\" $pad0 \"gene\"] ;    \n\
      append tblStr $tmpStr $indentStr;                \n\
                                                       \n\
      set tmpStr [format \"%-*s\" $pad1 \"drug\"] ;    \n\
      append tblStr $tmpStr $indentStr;                \n\
                                                       \n\
      set tmpStr [format \"%-*s\" $pad2 \"cross-res\"] ;\n\
      append tblStr $tmpStr $indentStr;                \n\
                                                       \n\
      set tmpStr [format \"%-*s\" $pad4 \"mutant\"] ;  \n\
      append tblStr $tmpStr $indentStr;                \n\
                                                       \n\
      set tmpStr [format \"%-*s\" $pad8 \"%sup\"] ;    \n\
      append tblStr $tmpStr $indentStr;                \n\
                                                       \n\
      append tblStr \"\\n\";                           \n\
                                                       \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
      # Gui08 Sec02 Sub09 Cat08:                       \n\
      #   - add gene id to table + start table loop    \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
      set openFILE [open $fileStr] ;                   \n\
      gets $openFILE lineStr ;                         \n\
                                                       \n\
      while {[gets $openFILE lineStr] > -1} {          \n\
         set lineStr [split $lineStr \"\\t\"] ;        \n\
                                                       \n\
         set tmpStr [lindex $lineStr 0] ; # gene id    \n\
         set tmpStr [format \"%-*s\" $pad0 $tmpStr] ;  \n\
         append tblStr $tmpStr $indentStr;             \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub09 Cat09:                    \n\
         #   - add primrary drug to table              \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         set tmpStr [lindex $lineStr 1] ;              \n\
         set tmpStr [string tolower $tmpStr] ;         \n\
                                                       \n\
         # try to reduce drug to three letter code     \n\
         ---for                                        \n\
           { set siAmr 0 }                             \n\
           { $siAmr < [llength $::glob_amrList] }      \n\
           { incr siAmr }                              \n\
         ---{                                          \n\
            ---set                                     \n\
               tmpStr                                  \n\
               [regsub                                 \n\
                  [lindex $::glob_amrList $siAmr]      \n\
                  $tmpStr                              \n\
                  [lindex $::glob_amrShort $siAmr]     \n\
               ]                                       \n\
            ---;                                       \n\
         } ; # Loop: convert AMR ids to short hand     \n\
         set tmpStr [format \"%-*s\" $pad1 $tmpStr] ;  \n\
         append tblStr $tmpStr $indentStr;             \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub09 Cat10:                    \n\
         #   - add cross-resistance drugs to table     \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         set tmpStr [lindex $lineStr 2] ;              \n\
         set tmpStr [string tolower $tmpStr] ;         \n\
                                                       \n\
         # try to reduce drug to three letter code     \n\
         ---for                                        \n\
           { set siAmr 0 }                             \n\
           { $siAmr < [llength $::glob_amrList] }      \n\
           { incr siAmr }                              \n\
         ---{                                          \n\
            ---set                                     \n\
               tmpStr                                  \n\
               [regsub                                 \n\
                  [lindex $::glob_amrList $siAmr]      \n\
                  $tmpStr                              \n\
                  [lindex $::glob_amrShort $siAmr]     \n\
               ]                                       \n\
            ---;                                       \n\
         } ; # Loop: convert AMR ids to short hand     \n\
                                                       \n\
         set tmpStr [format \"%-*s\" $pad2 $tmpStr] ;  \n\
         append tblStr $tmpStr $indentStr;             \n\
                                                       \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
         # Gui08 Sec02 Sub09 Cat11:                    \n\
         #   - add mutation and % support to table     \n\
         #++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
         # mutation type                               \n\
         set tmpStr [lindex $lineStr 4] ;              \n\
         set tmpStr [format \"%-*s\" $pad4 $tmpStr] ;  \n\
         append tblStr $tmpStr $indentStr;             \n\
                                                       \n\
         # percent of reads supporting                 \n\
         set tmpStr [lindex $lineStr 8] ;              \n\
         set tmpStr [format \"%-*s\" $pad8 $tmpStr] ;  \n\
         append tblStr $tmpStr $indentStr;             \n\
                                                       \n\
         append tblStr \"\\n\" ; # next row            \n\
      } ; # Loop: get AMRs                             \n\
                                                       \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
      # Gui08 Sec02 Sub09 Cat12:                       \n\
      #   - put table into AMR read table label        \n\
      #+++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
      ---.main.out.amr.read.tbl.lab                    \n\
         configure                                     \n\
         -text $tblStr                                 \n\
      ---;                                             \n\
                                                       \n\
      close $openFILE ;                                \n\
      return true ;                                    \n\
   } ; # readAmrTbl                                    \n\
                                                       \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui08 Sec03:                                      \n\
   #   - set up settings output gui frame              \n\
   #   o gui08 sec03 sub01:                            \n\
   #     - set up file prefix finder                   \n\
   #   o gui08 sec03 sub02:                            \n\
   #     - AMR snp percent entry                       \n\
   #   o gui08 sec03 sub03:                            \n\
   #     - AMR indel percent entry                     \n\
   #   o gui08 sec03 sub04:                            \n\
   #     - graphing check button                       \n\
   #   o gui08 sec03 sub05:                            \n\
   #     - get report button                           \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec03 Sub01:                                \n\
   #   - set up file prefix finder                     \n\
   #************************************************** \n\
                                                       \n\
   tk::frame .main.out.set.prefix ;                    \n\
   pack .main.out.set.prefix -anchor w -side top ;     \n\
   tk::label .main.out.set.prefix.lab -text \"\" ;     \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.out.set.prefix.but                         \n\
      -text \"get prefix\"                             \n\
      -command {                                       \n\
   ---                                                 \n\
         set fileStr [                                 \n\
            ---tk_getOpenFile                          \n\
               -title \"select freezeTB output\"       \n\
            ---                                        \n\
         ] ;                                           \n\
                                                       \n\
         if { $fileStr eq \"\" } {                     \n\
                                                       \n\
         } else {                                      \n\
            # set up global variable                   \n\
            upvar 0 glob_outPref outPrefStr ;          \n\
                                                       \n\
            # get path and file name                   \n\
            set dirStr [file dirname $fileStr] ;       \n\
            set prefStr [file tail $fileStr] ;         \n\
                                                       \n\
            # remove all possible extension types      \n\
                                                       \n\
            set prefStr [regsub con.* $prefStr \"\"] ; \n\
            set prefStr [regsub map.* $prefStr \"\"] ; \n\
            set prefStr [regsub read.* $prefStr \"\"] ;\n\
            set prefStr [regsub tsv.* $prefStr \"\"] ; \n\
            set prefStr [regsub id.* $prefStr \"\"] ;  \n\
            set prefStr [regsub dept.* $prefStr \"\"] ;\n\
            set prefStr [regsub grap.* $prefStr \"\"] ;\n\
            set prefStr [regsub coverage.* $prefStr \"\"] ;\n\
                                                       \n\
            ---set                                     \n\
               prefStr                                 \n\
               [string range $prefStr 0 end-1]         \n\
            ---; # remove dash regsub could not remove \n\
                                                       \n\
                                                       \n\
            # save prefix and update label             \n\
                                                       \n\
            ---set                                     \n\
               outPrefStr                              \n\
               [file join $dirStr $prefStr]            \n\
            ---;                                       \n\
                                                       \n\
           ---.main.out.set.prefix.lab                 \n\
              configure                                \n\
              -text $outPrefStr                        \n\
           ---;                                        \n\
         } ; # check if anything was input             \n\
      } ; # get file prefix                            \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.set.prefix.but                         \n\
      .main.out.set.prefix.lab                         \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec03 Sub02:                                \n\
   #   - AMR snp percent entry                         \n\
   #************************************************** \n\
                                                       \n\
   tk::frame .main.out.set.snp ;                       \n\
   pack .main.out.set.snp -anchor w -side top ;        \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.set.snp.lab                            \n\
      -text \"min AMR SNP % support\"                  \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.out.set.snp.entry                          \n\
      -textvariable glob_outSnpSup                     \n\
      -vcmd { tcl_isFloat_gui %P %i 0 1 }              \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.set.snp.lab                            \n\
      .main.out.set.snp.entry                          \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec03 Sub03:                                \n\
   #   - AMR indel percent entry                       \n\
   #************************************************** \n\
                                                       \n\
   tk::frame .main.out.set.indel ;                     \n\
   pack .main.out.set.indel -anchor w -side top ;      \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.set.indel.lab                          \n\
      -text \"min AMR Indel % support\"                \n\
   ---;                                                \n\
                                                       \n\
   ---tk::entry                                        \n\
      .main.out.set.indel.entry                        \n\
      -textvariable glob_outIndelSup                   \n\
      -vcmd { tcl_isFloat_gui %P %i 0 1 }              \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.set.indel.lab                          \n\
      .main.out.set.indel.entry                        \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec03 Sub04:                                \n\
   #   - graphing check button                         \n\
   #************************************************** \n\
                                                       \n\
   tk::frame .main.out.set.graph ;                     \n\
   pack .main.out.set.graph -anchor w -side top ;      \n\
                                                       \n\
   ---tk::checkbutton                                  \n\
      .main.out.set.graph.check                        \n\
      -text \"build graphs\"                           \n\
      -variable glob_mkGraphBl                         \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
     .main.out.set.graph.check                         \n\
     -anchor w                                         \n\
     -side left                                        \n\
   ---;                                                \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec03 Sub05:                                \n\
   #   - get report button                             \n\
   #************************************************** \n\
                                                       \n\
   tk::frame .main.out.set.run ;                       \n\
   pack .main.out.set.run -anchor w -side top ;        \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.out.set.run.but                            \n\
      -text \"get report\"                             \n\
      -command {                                       \n\
   ---                                                 \n\
         upvar 0 glob_outCur curPrefix ;               \n\
         upvar 0 glob_outPref newPrefix ;              \n\
         set curPrefix $newPrefix ;                    \n\
                                                       \n\
         conAmrRep $curPrefix ;                        \n\
         readAmrRep $curPrefix ;                       \n\
         depthGraph $curPrefix ;                       \n\
         coverageGraph $curPrefix ;                    \n\
         conSpol $curPrefix ;                          \n\
         readSpol $curPrefix ;                         \n\
         conMiru $curPrefix ;                          \n\
         conAmrTbl $curPrefix \"   \" ;                \n\
         readAmrTbl $curPrefix \"   \" ;               \n\
                                                       \n\
         .main.out.menu.reportBut invoke ;             \n\
         .main.out.set.prefix.lab configure -text \"\" ;\n\
         set $newPrefix \"\" ;                         \n\
      } ; # build the report                           \n\
                                                       \n\
   pack .main.out.set.run.but -anchor w -side left ;   \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui08 Sec04:                                      \n\
   #   - set up output gui menu                        \n\
   #   o gui08 sec04 sub01:                            \n\
   #     - set up menu frame                           \n\
   #   o gui08 sec04 sub02:                            \n\
   #     - add settings button                         \n\
   #   o gui08 sec04 sub03:                            \n\
   #     - add report button                           \n\
   #   o gui08 sec04 sub04:                            \n\
   #     - add mean read depth graph button            \n\
   #   o gui08 sec04 sub05:                            \n\
   #     - add coverage graph button                   \n\
   #   o gui08 sec04 sub06:                            \n\
   #     - add AMR table button                        \n\
   #   o gui08 sec04 sub07:                            \n\
   #     - add input button                            \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec04 Sub01:                                \n\
   #   - set up menu frame                             \n\
   #************************************************** \n\
                                                       \n\
   tk::frame .main.out.menu ;                          \n\
   pack .main.out.menu -anchor w -side top ;           \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec04 Sub02:                                \n\
   #   - add settings button                           \n\
   #************************************************** \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.out.menu.setBut                            \n\
      -text \"out\"                                    \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.out.set ;                   \n\
         pack forget .main.out.report ;                \n\
         pack forget .main.out.depth ;                 \n\
         pack forget .main.out.cover ;                 \n\
         pack forget .main.out.amr ;                   \n\
         pack forget .main.out.menu ;                  \n\
                                                       \n\
         pack .main.out.set ;                          \n\
         pack .main.out.menu ;                         \n\
         wm title . \"output select freezeTB\" ;       \n\
                                                       \n\
         ---.main.out.menu.setBut                      \n\
             configure                                 \n\
             -relief sunken                            \n\
             -state disabled                           \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.reportBut                   \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.depthBut                    \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.coverBut                    \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.amrBut                      \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
      } ; # set up output setting button               \n\
                                                       \n\
   pack .main.out.menu.setBut -anchor w -side left ;   \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec04 Sub03:                                \n\
   #   - add report button                             \n\
   #************************************************** \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.out.menu.reportBut                         \n\
      -text \"report\"                                 \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.out.set ;                   \n\
         pack forget .main.out.report ;                \n\
         pack forget .main.out.depth ;                 \n\
         pack forget .main.out.cover ;                 \n\
         pack forget .main.out.amr ;                   \n\
         pack forget .main.out.menu ;                  \n\
                                                       \n\
         pack .main.out.report ;                       \n\
         pack .main.out.menu ;                         \n\
                                                       \n\
         ---wm                                         \n\
            title                                      \n\
            .                                          \n\
            [concat                                    \n\
               [file tail $glob_outCur]                \n\
               \"report freezeTB\"                     \n\
            ]                                          \n\
        ---;                                           \n\
                                                       \n\
         ---.main.out.menu.setBut                      \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.reportBut                   \n\
             configure                                 \n\
             -relief sunken                            \n\
             -state disabled                           \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.depthBut                    \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.coverBut                    \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.amrBut                      \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
      } ;                                              \n\
                                                       \n\
   pack .main.out.menu.reportBut -anchor w -side left ;\n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec04 Sub04:                                \n\
   #   - add mean read depth graph button              \n\
   #************************************************** \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.out.menu.depthBut                          \n\
      -text \"read depth\"                             \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.out.set ;                   \n\
         pack forget .main.out.report ;                \n\
         pack forget .main.out.depth ;                 \n\
         pack forget .main.out.cover ;                 \n\
         pack forget .main.out.amr ;                   \n\
         pack forget .main.out.menu ;                  \n\
                                                       \n\
         pack .main.out.depth ;                        \n\
         pack .main.out.menu ;                         \n\
                                                       \n\
         ---wm                                         \n\
            title                                      \n\
            .                                          \n\
            [concat                                    \n\
               [file tail $glob_outCur]                \n\
               \"read depth freezeTB\"                 \n\
            ]                                          \n\
        ---;                                           \n\
                                                       \n\
         ---.main.out.menu.setBut                      \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.reportBut                   \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.depthBut                    \n\
             configure                                 \n\
             -relief sunken                            \n\
             -state disabled                           \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.coverBut                    \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.amrBut                      \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
      } ;                                              \n\
                                                       \n\
   pack .main.out.menu.depthBut -anchor w -side left ; \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec04 Sub05:                                \n\
   #   - add coverage graph button                     \n\
   #************************************************** \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.out.menu.coverBut                          \n\
      -text \"coverage\"                               \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.out.set ;                   \n\
         pack forget .main.out.report ;                \n\
         pack forget .main.out.depth ;                 \n\
         pack forget .main.out.cover ;                 \n\
         pack forget .main.out.amr ;                   \n\
         pack forget .main.out.menu ;                  \n\
                                                       \n\
         pack .main.out.cover ;                        \n\
         pack .main.out.menu ;                         \n\
                                                       \n\
         ---wm                                         \n\
            title                                      \n\
            .                                          \n\
            [concat                                    \n\
               [file tail $glob_outCur]                \n\
               \"coverage freezeTB\"                   \n\
            ]                                          \n\
        ---;                                           \n\
                                                       \n\
         ---.main.out.menu.setBut                      \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.reportBut                   \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.depthBut                    \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.coverBut                    \n\
             configure                                 \n\
             -relief sunken                            \n\
             -state disabled                           \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.amrBut                      \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
      } ;                                              \n\
                                                       \n\
   pack .main.out.menu.coverBut -anchor w -side left ; \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec04 Sub06:                                \n\
   #   - add AMR table button                          \n\
   #************************************************** \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.out.menu.amrBut                            \n\
      -text \"AMR table\"                              \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.out.set ;                   \n\
         pack forget .main.out.report ;                \n\
         pack forget .main.out.depth ;                 \n\
         pack forget .main.out.cover ;                 \n\
         pack forget .main.out.amr ;                   \n\
         pack forget .main.out.menu ;                  \n\
                                                       \n\
         pack .main.out.amr ;                          \n\
         pack .main.out.menu ;                         \n\
                                                       \n\
         ---wm                                         \n\
            title                                      \n\
            .                                          \n\
            [concat                                    \n\
               [file tail $glob_outCur]                \n\
               \"AMR table freezeTB\"                  \n\
            ]                                          \n\
        ---;                                           \n\
                                                       \n\
         ---.main.out.menu.setBut                      \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.reportBut                   \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.depthBut                    \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.coverBut                    \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.amrBut                      \n\
             configure                                 \n\
             -relief sunken                            \n\
             -state disabled                           \n\
         ---;                                          \n\
      } ;                                              \n\
                                                       \n\
   pack .main.out.menu.amrBut -anchor w -side left ;   \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec04 Sub07:                                \n\
   #   - add input button                              \n\
   #************************************************** \n\
                                                       \n\
   ---tk::button                                       \n\
      .main.out.menu.inBut                             \n\
      -text \"input\"                                  \n\
      -command {                                       \n\
   ---                                                 \n\
         pack forget .main.out.set ;                   \n\
         pack forget .main.out.report ;                \n\
         pack forget .main.out.depth ;                 \n\
         pack forget .main.out.cover ;                 \n\
         pack forget .main.out.amr ;                   \n\
         pack forget .main.out.menu ;                  \n\
                                                       \n\
         .main.menu.reqBut invoke ;                    \n\
                                                       \n\
         ---.main.out.menu.setBut                      \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.reportBut                   \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.depthBut                    \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.coverBut                    \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
                                                       \n\
         ---.main.out.menu.amrBut                      \n\
             configure                                 \n\
             -relief raised                            \n\
             -state normal                             \n\
         ---;                                          \n\
      } ;                                              \n\
                                                       \n\
   pack .main.out.menu.inBut -anchor w -side left ;    \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui08 Sec05:                                      \n\
   #   - set up report gui                             \n\
   #   o gui08 sec05 sub01:                            \n\
   #     - build reference AMR labels                  \n\
   #   o gui08 sec05 sub02:                            \n\
   #     - build read AMR labels                       \n\
   #   o gui08 sec05 sub03:                            \n\
   #     - consensus spoligotype                       \n\
   #   o gui08 sec05 sub04:                            \n\
   #     - read spoligotype                            \n\
   #   o gui08 sec05 sub05:                            \n\
   #     - MIRU lineage                                \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec05 Sub01:                                \n\
   #   - build reference AMR labels                    \n\
   #************************************************** \n\
                                                       \n\
   tk::frame .main.out.report.conAmr ;                 \n\
   pack .main.out.report.conAmr -anchor w -side top ;  \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.conAmr.lab                      \n\
      -text \"con  AMRs:\"                             \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.conAmr.lab                      \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   foreach amr $glob_amrShort {                        \n\
      set tmpStr [string tolower $amr] ;               \n\
      append tmpStr \"lab\" ;                          \n\
                                                       \n\
      ---tk::label                                     \n\
         .main.out.report.conAmr.$tmpStr               \n\
         -text $amr                                    \n\
         -background $glob_noAmrCol                    \n\
         -fg $glob_amrTextCol                          \n\
      ---;                                             \n\
                                                       \n\
      ---pack                                          \n\
         .main.out.report.conAmr.$tmpStr               \n\
         -anchor w                                     \n\
         -side left                                    \n\
      ---;                                             \n\
                                                       \n\
      # add a space between the labels                 \n\
      set tmpStr [string tolower $amr] ;               \n\
      append tmpStr \"space\" ;                        \n\
                                                       \n\
      ---tk::label                                     \n\
         .main.out.report.conAmr.$tmpStr               \n\
         -text \"\"                                    \n\
      ---;                                             \n\
                                                       \n\
      ---pack                                          \n\
         .main.out.report.conAmr.$tmpStr               \n\
         -anchor w                                     \n\
         -side left                                    \n\
      ---;                                             \n\
   } ; # Loop: build amr labels                        \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec05 Sub02:                                \n\
   #   - build read AMR labels                         \n\
   #************************************************** \n\
                                                       \n\
   # need to add some space; otherwise have overlap    \n\
   tk::frame .main.out.report.space ;                  \n\
   tk::frame .main.out.report.readAmr ;                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.space                           \n\
      .main.out.report.readAmr                         \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.readAmr.lab                     \n\
      -text \"read AMRs:\"                             \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.readAmr.lab                     \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   foreach amr $glob_amrShort {                        \n\
      set tmpStr [string tolower $amr] ;               \n\
      append tmpStr \"lab\" ;                          \n\
                                                       \n\
      ---tk::label                                     \n\
         .main.out.report.readAmr.$tmpStr              \n\
         -text $amr                                    \n\
         -background $glob_noAmrCol                    \n\
         -fg $glob_amrTextCol                          \n\
      ---;                                             \n\
                                                       \n\
      ---pack                                          \n\
         .main.out.report.readAmr.$tmpStr              \n\
         -anchor w                                     \n\
         -side left                                    \n\
      ---;                                             \n\
                                                       \n\
      # add a space between the labels                 \n\
      set tmpStr [string tolower $amr] ;               \n\
      append tmpStr \"space\" ;                        \n\
                                                       \n\
      ---tk::label                                     \n\
         .main.out.report.readAmr.$tmpStr              \n\
         -text \"\"                                    \n\
      ---;                                             \n\
                                                       \n\
      ---pack                                          \n\
         .main.out.report.readAmr.$tmpStr              \n\
         -anchor w                                     \n\
         -side left                                    \n\
      ---;                                             \n\
   } ; # Loop: build amr labels                        \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec05 Sub03:                                \n\
   #   - consensus spoligotype                         \n\
   #   o gui08 sec05 sub03 cat01:                      \n\
   #     - consensus spoligotype label                 \n\
   #   o gui08 sec05 sub03 cat02:                      \n\
   #     - octal for consensus spoligotype             \n\
   #   o gui08 sec05 sub03 cat03:                      \n\
   #     - SIT for consensus spoligotype               \n\
   #   o gui08 sec05 sub03 cat03:                      \n\
   #     - strain for consensus spoligotype            \n\
   #   o gui08 sec05 sub03 cat0::                      \n\
   #     - countries dectected in (use orig for short) \n\
   #************************************************** \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec05 Sub03 Cat01:                          \n\
   #   - consensus spoligotype label                   \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   # add space so does not overlap with amr labels     \n\
   tk::frame .main.out.report.space2 ;                 \n\
   pack .main.out.report.space2 -anchor w -side top ;  \n\
                                                       \n\
   tk::frame .main.out.report.conspol ;                \n\
   pack .main.out.report.conspol -anchor w -side top ; \n\
                                                       \n\
   tk::frame .main.out.report.conspol.head ;           \n\
   pack .main.out.report.conspol.head -anchor w -side top ;\n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.conspol.head.lab                \n\
      -text \"Consensus spoligotype:\"                 \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.conspol.head.lab                \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec05 Sub03 Cat02:                          \n\
   #   - octal for consensus spoligotype               \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   tk::frame .main.out.report.conspol.octal ;          \n\
   ---pack                                             \n\
     .main.out.report.conspol.octal                    \n\
     -anchor w                                         \n\
     -side top                                         \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.conspol.octal                   \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.conspol.octal.headlab           \n\
      -text \"     Octal: \"                           \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.conspol.octal.reslab            \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.conspol.octal.headlab           \n\
      .main.out.report.conspol.octal.reslab            \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec05 Sub03 Cat03:                          \n\
   #   - SIT for consensus spoligotype                 \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   tk::frame .main.out.report.conspol.sit ;            \n\
   ---pack                                             \n\
      .main.out.report.conspol.sit                     \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.conspol.sit                     \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.conspol.sit.headlab             \n\
      -text \"     SIT: \"                             \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.conspol.sit.reslab              \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.conspol.sit.headlab             \n\
      .main.out.report.conspol.sit.reslab              \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec05 Sub03 Cat03:                          \n\
   #   - strain for consensus spoligotype              \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   tk::frame .main.out.report.conspol.strain ;         \n\
   ---pack                                             \n\
      .main.out.report.conspol.strain                  \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.conspol.strain                  \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.conspol.strain.headlab          \n\
      -text \"     Strain: \"                          \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.conspol.strain.reslab           \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.conspol.strain.headlab          \n\
      .main.out.report.conspol.strain.reslab           \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec05 Sub03 Cat04:                          \n\
   #   - countries dectected in (use orig for short)   \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   tk::frame .main.out.report.conspol.orig ;           \n\
   ---pack                                             \n\
     .main.out.report.conspol.orig                     \n\
     -anchor w                                         \n\
     -side top                                         \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.conspol.orig                    \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.conspol.orig.headlab            \n\
      -text \"     Countries: \"                       \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.conspol.orig.reslab             \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.conspol.orig.headlab            \n\
      .main.out.report.conspol.orig.reslab             \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec05 Sub04:                                \n\
   #   - read spoligotype                              \n\
   #   o gui08 sec05 sub04 cat01:                      \n\
   #     - read spoligotype label                      \n\
   #   o gui08 sec05 sub04 cat02:                      \n\
   #     - octal for read spoligotype                  \n\
   #   o gui08 sec05 sub04 cat03:                      \n\
   #     - SIT for read spoligotype                    \n\
   #   o gui08 sec05 sub04 cat03:                      \n\
   #     - strain for read spoligotype                 \n\
   #   o gui08 sec05 sub04 cat0::                      \n\
   #     - countries dectected in (use orig for short) \n\
   #************************************************** \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec05 Sub04 Cat01:                          \n\
   #   - read spoligotype label                        \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   # add space so does not overlap with amr labels     \n\
   tk::frame .main.out.report.space3 ;                 \n\
   pack .main.out.report.space3 -anchor w -side top ;  \n\
                                                       \n\
   tk::frame .main.out.report.readspol ;               \n\
   pack .main.out.report.readspol -anchor w -side top ;\n\
                                                       \n\
   tk::frame .main.out.report.readspol.head ;          \n\
   ---pack                                             \n\
      .main.out.report.readspol.head                   \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.readspol.head.lab               \n\
      -text \"Read spoligotype:\"                      \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.readspol.head.lab               \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec05 Sub04 Cat02:                          \n\
   #   - octal for read spoligotype                    \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   tk::frame .main.out.report.readspol.octal ;         \n\
   ---pack                                             \n\
     .main.out.report.readspol.octal                   \n\
     -anchor w                                         \n\
     -side top                                         \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.readspol.octal                  \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.readspol.octal.headlab          \n\
      -text \"     Octal: \"                           \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.readspol.octal.reslab           \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.readspol.octal.headlab          \n\
      .main.out.report.readspol.octal.reslab           \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec05 Sub04 Cat03:                          \n\
   #   - SIT for read spoligotype                      \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   tk::frame .main.out.report.readspol.sit ;           \n\
   ---pack                                             \n\
      .main.out.report.readspol.sit                    \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.readspol.sit                    \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.readspol.sit.headlab            \n\
      -text \"     SIT: \"                             \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.readspol.sit.reslab             \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.readspol.sit.headlab            \n\
      .main.out.report.readspol.sit.reslab             \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec05 Sub04 Cat03:                          \n\
   #   - strain for read spoligotype                   \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   tk::frame .main.out.report.readspol.strain ;        \n\
   ---pack                                             \n\
      .main.out.report.readspol.strain                 \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.readspol.strain                 \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.readspol.strain.headlab         \n\
      -text \"     Strain: \"                          \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.readspol.strain.reslab          \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.readspol.strain.headlab         \n\
      .main.out.report.readspol.strain.reslab          \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
   # Gui08 Sec05 Sub04 Cat04:                          \n\
   #   - countries dectected in (use orig for short)   \n\
   #++++++++++++++++++++++++++++++++++++++++++++++++++ \n\
                                                       \n\
   tk::frame .main.out.report.readspol.orig ;          \n\
   ---pack                                             \n\
     .main.out.report.readspol.orig                    \n\
     -anchor w                                         \n\
     -side top                                         \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.readspol.orig                   \n\
      -anchor w                                        \n\
      -side top                                        \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.readspol.orig.headlab           \n\
      -text \"     Countries: \"                       \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.readspol.orig.reslab            \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.readspol.orig.headlab           \n\
      .main.out.report.readspol.orig.reslab            \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec05 Sub05:                                \n\
   #   - MIRU lineage                                  \n\
   #************************************************** \n\
                                                       \n\
   # add space to keep theme                           \n\
   tk::frame .main.out.report.space4 ;                 \n\
   pack .main.out.report.space4 -anchor w -side top ;  \n\
                                                       \n\
   tk::frame .main.out.report.miru ;                   \n\
   pack .main.out.report.miru -anchor w -side top ;    \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.miru.headlab                    \n\
      -text \"MIRU-VNTR: \"                            \n\
   ---;                                                \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.report.miru.reslab                     \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
                                                       \n\
   ---pack                                             \n\
      .main.out.report.miru.headlab                    \n\
      .main.out.report.miru.reslab                     \n\
      -anchor w                                        \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui08 Sec06:                                      \n\
   #   - set up graphs                                 \n\
   #   o gui08 sec05 sub01:                            \n\
   #     - build reference AMR labels                  \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   tk::label .main.out.depth.graph ;                   \n\
   pack .main.out.depth.graph -anchor w -side left ;   \n\
                                                       \n\
   tk::label .main.out.cover.graph ;                   \n\
   pack .main.out.cover.graph -anchor w -side left ;   \n\
                                                       \n\
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\
   # Gui08 Sec07:                                      \n\
   #   - set up amr table                              \n\
   #   o gui08 sec07 sub01:                            \n\
   #     - set up consensus AMR table                  \n\
   #   o gui08 sec07 sub02:                            \n\
   #     - set up read AMR table                       \n\
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec07 Sub01:                                \n\
   #   - set up consensus AMR table                    \n\
   #************************************************** \n\
                                                       \n\
   tk::frame .main.out.amr.con ;                       \n\
   pack .main.out.amr.con -anchor nw -side left ;      \n\
                                                       \n\
   tk::frame .main.out.amr.con.head ;                  \n\
   pack .main.out.amr.con.head -anchor w -side top ;   \n\
                                                       \n\
   tk::frame .main.out.amr.con.tbl ;                   \n\
   pack .main.out.amr.con.tbl -anchor w -side top ;    \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.amr.con.head.lab                       \n\
      -font \"Courier\"                                \n\
      -text \"Consensus ARMs:\"                        \n\
   ---;                                                \n\
   ---tk::label                                        \n\
      .main.out.amr.con.tbl.lab                        \n\
      -font \"Courier\"                                \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
   ---pack                                             \n\
      .main.out.amr.con.head.lab                       \n\
      .main.out.amr.con.tbl.lab                        \n\
      -anchor nw                                       \n\
      -side left                                       \n\
   ---;                                                \n\
                                                       \n\
   #************************************************** \n\
   # Gui08 Sec07 Sub02:                                \n\
   #   - set up read AMR table                         \n\
   #************************************************** \n\
                                                       \n\
   tk::frame .main.out.amr.read ;                      \n\
   pack .main.out.amr.read -anchor nw -side right ;    \n\
                                                       \n\
   tk::frame .main.out.amr.read.head ;                 \n\
   pack .main.out.amr.read.head -anchor w -side top ;  \n\
                                                       \n\
   tk::frame .main.out.amr.read.tbl ;                  \n\
   pack .main.out.amr.read.tbl -anchor w -side top ;   \n\
                                                       \n\
   ---tk::label                                        \n\
      .main.out.amr.read.head.lab                      \n\
      -font \"Courier\"                                \n\
      -text \"Read AMRs:\"                             \n\
   ---;                                                \n\
   ---tk::label                                        \n\
      .main.out.amr.read.tbl.lab                       \n\
      -font \"Courier\"                                \n\
      -text \"NA\"                                     \n\
   ---;                                                \n\
   ---pack                                             \n\
      .main.out.amr.read.head.lab                      \n\
      .main.out.amr.read.tbl.lab                       \n\
      -anchor nw                                       \n\
      -side left                                       \n\
   ---;                                                \n\
"; /*glob_guiScript_gui*/
