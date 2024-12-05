   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # gui SOF: Start Of File
   #   - freezeTB gui in tcltk (copy to guiFreezeTB.c)
   #   o header:
   #     - global variables
   #   o tcl01: glob_tcl_isInt_gui
   #     - checks if input key is numeric and if string
   #       is between max and min
   #   o gui01:
   #     - set up main frame for tcltk
   #   o gui02:
   #     - required input gui
   #   o gui03:
   #     - menu bar for gui
   #   o gui04:
   #     - read filtering menu
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #--------------------------------------------------
   # Header:
   #   - global variables
   #   o header sec01:
   #     - global variables (not modified by C)
   #   o header sec02:
   #     - program paths
   #   o header sec03:
   #     - global variables modified by C (remove)
   #   o header sec04:
   #     - file type lists
   #   o header sec05:
   #     - find freezeTB location (remove for C)
   #--------------------------------------------------

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Header Sec01:
   #   - global variables (not modified by C)
   #   o header sec01 sub01:
   #     - variables to store values
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #***************************************************
   # Header Sec01 Sub01:
   #   - variables to store values
   #***************************************************

   # required input
   variable glob_fqIn "" ;
   variable glob_dirOut "" ;
   variable glob_prefix "Hufflepuff" ;

   # read filtering settings
   variable glob_mask 0 ;    # mask primer sites
   variable glob_adjust 0 ;  # adjust mapp coordinates
   variable glob_srCheck "map-ont" ; # map setting

   # amr setting
   variable glob_frameshift 0 ; # AMR frameshift scan

   # clustering
   variable glob_clustBl 0 ;
   variable glob_depthProfBl 0 ;

   # output settings
   variable glob_outPref "" ; # report user is getting
   variable glob_outCur "" ;  # current shown report
   variable glob_outSnpSup 0.1 ;
   variable glob_outIndelSup 0.4 ;
   variable glob_mkGraphBl 1 ;

   variable glob_depthImg [image create photo -file ""] ;
   variable glob_coverImg [image create photo -file ""] ;

   variable 

   #***************************************************
   # Header Sec01 Sub02:
   #   - amr list
   #***************************************************

   # color pallete is from viridis
   set glob_noAmrCol "#440154" ; # viridis purple
   set glob_amrTextCol "#21908C" ; # light green
   set glob_amrCol "#FDE725" ; # viridis yellow

   # keep this list lower case
   set glob_amrList [list "amikacin" "bedaquiline" "capreomycin" "clofazimine" "delamanid" "ethambutol" "ethionamide" "fluoroquine" "isoniazid" "kanamycin" "levofloxacin" "linezolid" "moxifloxacin" "penicillin-myceial-dreg" "pyrazinamide" "rifampicin" "streptomycin" ] ;

   set glob_amrShort [list "Amk"  "Bdq"  "Cap"  "Cfz"  "Dlm"  "Emb"  "Eto"  "Flq"  "Inz"  "Kan"  "Lfx"  "Lzd"  "Mfx"  "Pmd"  "Pza"  "Rif"  "Str"  ] ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Header Sec02:
   #   - program paths
   #   o header sec02 sub01:
   #     - windows program detection
   #   o header sec02 sub02:
   #     - unix program detection
   #   o header sec02 sub03:
   #     - check/get Rscript version
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #***************************************************
   # Header Sec02 Sub01:
   #   - windows program detection
   #   o header sec02 sub01 cat01:
   #     - windows general setup 
   #   o header sec02 sub01 cat02:
   #     - windows detect minimap2
   #   o header sec02 sub01 cat03:
   #     - windows detect Rscript
   #   o header sec02 sub01 cat04:
   #     - windows detect graphing script
   #***************************************************

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Header Sec02 Sub01 Cat01:
   #   - windows general setup 
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   variable mapPath "" ; # minimap2 path
   variable rPath "" ;   # Rscript path

   if { [lindex $tcl_platform(os) 0] eq "Windows" } {
      # this is windows

      package require registry ;
         # registry is a base tcl package for windows
         # registry manipulations

      set programFiles $::env(PROGRAMFILES) ;

      #++++++++++++++++++++++++++++++++++++++++++++++++
      # Header Sec02 Sub01 Cat02:
      #   - windows detect minimap2
      #++++++++++++++++++++++++++++++++++++++++++++++++

      set mapPath "minimap2.exe" ;

      set status [catch {exec minimap2.exe --version} mapVersion ]  ; # get minimap2 version

      if { $status eq 0 } {
         # got minimap2 version, nothing else to do
      } else {
         set mapPath  [file join $programFiles "freezeTB" "minimap2.exe" ] ; # build alternate minimap2 path

         set status [catch {exec $mapPath --version} mapVersion ]  ; # get minimap2 version

         if { $status eq 0 } {
            # found minmap2; nothing else to do
         } else {
            tk_messageBox -message "Could not find minimap2.exe" -title "ERROR" ;
         } ; # If: check if could not find minimap2
      } ; # find minimap2 path

      #++++++++++++++++++++++++++++++++++++++++++++++++
      # Header Sec02 Sub01 Cat03:
      #   - windows detect Rscript
      #++++++++++++++++++++++++++++++++++++++++++++++++

      # find path to 
      # the keys came from https://stackoverflow.com/questions/62427584/find-rscript-exe-on-windows

      # NEED to try this on a windows machine
      registry get "HKCU\Software\Classes\RWorkspace\Shell\Open\Command" "Rscript.exe" ; # see if can find Rscript.exe in .RData

      registry get  "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall" "Rscript.exe" ; # see if can find Rscript.exe in Uninstall

      # last resort; try to find Rscript.exe in its
      # expected location
      set rPath [file join $programFiles "R" "*" "bin" "Rscript.exe" ]  ;

      if { [llength $rPath] eq 0 } {
         set $::glob_mkGraphBl 0 ;

         tk_messageBox -message "Rscript.exe not found" -title "ERROR" ;

         return false ;
      } else {
         set rPath [lindex $rPath 0] ;
         # make sure have only one Rscript.exe
      } ; # See if Rscript was finally found

      #++++++++++++++++++++++++++++++++++++++++++++++++
      # Header Sec02 Sub01 Cat04:
      #   - windows detect graphing script
      #++++++++++++++++++++++++++++++++++++++++++++++++
      
      set graphScript "graphAmpDepth.r" ;

      if { [file exists $graphScript] eq 0 } {
         set graphScript [file join $programFiles "freezeTB" $graphScript ] ;
         if { [file exists $graphScript] eq 0 } {
            set $::glob_mkGraphBl 0 ;

            tk_messageBox -message "graphAmpDepth.r not found" -title "no graphing script" ;

            set graphScript "" ;
         } ; # If: could not find graphing script
      } ; # If: did not find graphing script

   #***************************************************
   # Header Sec02 Sub02:
   #   - unix program detection
   #   o header sec02 sub02 cat01:
   #     - unix; find minimap2 version (check if have)
   #   o header sec02 sub02 cat02:
   #     - unix find graphing script
   #***************************************************

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Header Sec02 Sub02 Cat01:
   #   - unix; find minimap2 version (check if have)
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   } else {
      set mapPath "minimap2" ;
      set rPath "Rscript" ; # linux should be in path

      set status [catch {exec $mapPath --version} mapVersion ]  ; # seeing if minimap2 exists

      if { $status ne 0 } {
         set mapPath "./minimap2" ;

         set status [catch {exec $mapPath --version} mapVersion ]  ; # seeing if local minimap2 exists

         if { $status eq 0 } {
            tk_messageBox -message "minimap2 not found" -title "ERROR" ;

            return false ;
         } ; # If: minimap2 could not be found
      } ; # If: minimiap2 not in path or on system

      #++++++++++++++++++++++++++++++++++++++++++++++++
      # Header Sec02 Sub02 Cat02:
      #   - unix find graphing script
      #++++++++++++++++++++++++++++++++++++++++++++++++
      
      set graphScript "graphAmpDepth.r" ;

      if { [file exists $graphScript] eq 0 } {
         set graphScript [file join "/usr/local/bin" $graphScript] ;

         if { [file exists $graphScript] eq 0 } {
            set $::glob_mkGraphBl 0 ;

            tk_messageBox -message "graphAmpDepth.r not found" -title "no graphing script" ;

            set graphScript "" ;
         } ; # If: could not find graphing script
      } ; # If: did not find graphing script
   } ; # check if windows or linux

   #***************************************************
   # Header Sec02 Sub03:
   #   - check/get Rscript version
   #***************************************************

   set status [catch {exec $rPath --version} rVersion] ;

   if { $status eq 0 } {
   } else {
      set $::glob_mkGraphBl 0 ;

      tk_messageBox -message "Unable to run Rscript" -title "ERROR" ;

      return false;
   }

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Header Sec03:
   #   - global variables modified by C (remove)
   #   o header sec03 sub01:
   #     - set up default database finder function
   #   o header sec03 sub02:
   #     - database global variable set up
   #   o header sec03 sub03:
   #     - non-database default settings
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #***************************************************
   # Header Sec03 Sub01:
   #   - set up default database finder function
   #***************************************************

   proc getPath { fileStr } {
      
     set dbPath $fileStr ;

     if { [file exists $dbPath ] eq 0 } {
        set dbPath $::env(HOME) ;
        set dbPath [ file join $dbPath "Documents" ] ;
        set dbPath [ file join $dbPath "freezeTBFiles" ] ;
        set dbPath [ file join $dbPath $fileStr ] ;

        if { [file exists $dbPath ] eq 0 } {
           set osStr [lindex $::tcl_platform(os) 0] ;

           if { osStr eq "Windows" } {
              set dbPath [file join $::env(PUBLIC) "Documents" "freezeTBFiles" $fileStr ] ;

              if { [file exists $dbPath ] eq 0 } {
                 set dbPath "" ;
                 tk_messageBox -message [concat $fileStr "not found"] -title "missing database" ;
              } ; # If: database not in global location

           } else {
             set dbPath "/usr/local/share/freezeTBFiles" ;
              set dbPath [file join $dbPath $fileStr] ;

              if { [file exists $dbPath ] eq 0 } {
                 set dbPath "" ;
                 tk_messageBox -message [concat $fileStr "not found"] -title "missing database" ;
              } ; # If: database not in global location
           } ; # check if windows or unix
         } ; # If: database not in HOME/Documents
      } ; # If: database not in directory

      return $dbPath ;
   } ; # getPath

   #>>>script<<<#

   #***************************************************
   # Header Sec03 Sub02:
   #   - database global variable set up
   #***************************************************

   variable glob_refFa [getPath "NC000962.fa"] ;
   variable glob_coordsTsv [getPath "coords.tsv"] ;
   variable glob_amrDb [getPath "amrDb.tsv"] ;
   variable glob_maskCoords [getPath "mask.tsv"] ;
   variable glob_miruDb [getPath "miruTbl.tsv"] ;
   variable glob_spacer [getPath "spoligo-spacers.fa"] ;
   variable glob_spolDb [getPath "spoligo-lineages.csv"] ;

   #***************************************************
   # Header Sec03 Sub03:
   #   - non-database default settings
   #***************************************************

   # read filtering settings
   variable glob_mapq 15 ;
   variable glob_medQ 7 ;
   variable glob_meanQ 7 ;
   variable glob_minLen 50 ;

   # amr settings
   variable glob_amrPercSup 0.1 ;

   # lineage settings
   variable glob_miruFudge 15 ;
   variable glob_spolSim 0.9 ;
   variable glob_drStart 3119037 ;
   variable glob_drEnd 3123624 ;

   # consensus settings
   variable glob_depth 10 ;
   variable glob_snpq 3 ;
   variable glob_insq 3 ;
   variable glob_basePerc 0.4 ;
   variable glob_insPerc 0.7 ;
   variable glob_delPerc 0.7 ;

   # consensus variant settings
   variable glob_minVarDepth 10 ;
   variable glob_baseVarPerc 0.1 ;
   variable glob_insVarPerc 0.2 ;
   variable glob_delVarPerc 0.2 ;

   # cluster settings
   variable glob_minClustDepth 10 ;
   variable glob_minClustPercDepth 0.01 ;
   variable glob_minClustSnpQ 3 ;
   variable glob_minClustIndelLen 10 ;

   variable glob_clustReadErr 0.043 ;
   variable glob_clustConErr 0.023 ;
   variable glob_clustErrRatio 50 ;
   variable glob_clustWinErrRatio 200 ;
   variable glob_clustWinLen 500 ;
   variable glob_lenWeight 2 ;

   variable glob_maxClustSim 0.99 ;
   variable glob_minClustOverlap 0.75 ;
   variable glob_maxClustMask 0.05 ;
   variable glob_numRebuilds 1 ;

   #>>>script<<<#

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Header Sec04:
   #   - file type lists
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   # file types

   set fq_types {
      { {fastq}    {.fastq}    }
      { {fastq}    {.fq}       }
      { {fastq.gz} {.gz} }
      { {fastq.gz} {.gz}    }
   } ; # fastq file exentions
   
   set fa_types {
      { {fasta}    {.fasta}    }
      { {fasta}    {.fa}       }
   } ; # fastq file exentions

   set tsv_types {
      { {tsv} {.tsv} }
   } ; # fastq file exentions

   set csv_types {
      { {csv} {.csv} }
   } ; # fastq file exentions

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Header Sec05:
   #   - freezeTB command
   #   o header sec05 sub01:
   #     - windows find freezeTB + function declaration
   #   o header sec05 sub02:
   #     - unix find freezeTB
   #   o header sec05 sub03:
   #     - run freezeTB
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #>>>script<<<#

   #****************************************************
   # Header Sec05 Sub01:
   #   - windows find freezeTB + function declaration
   #***************************************************

   proc freezeTBProc { argsList } {

     if { [lindex $::tcl_platform(os) 0] eq "Windows" } {
         # this is windows
         set programFiles $::env(PROGRAMFILES) ;

         set freezeTBPath "freezeTB.exe" ;

         set status [catch {exec $freezeTBPath --version} freezeTBVer ]  ;

         if { $status eq 0 } {
            # found freezeTB
         } else {
            set freezeTBPath  [file join $programFiles "freezeTB" "minimap2.exe" ] ; # build alternate freezeTB path

            set status [catch {exec $freezeTBPath --version} freezeTBVer ]  ;

            if { $status eq 0 } {
               # found freezeTB
            } else {
               tk_messageBox -message "no freezeTB.exe detected" -title "ERROR" ;

               return 1 ;
            } ;
         } ; # find freezeTB path

      #************************************************
      # Header Sec05 Sub02:
      #   - unix find freezeTB
      #************************************************

      } else {
         set freezeTBPath "freezeTB" ;

         set status [catch {exec $freezeTBPath --version} freezeTBVer ]  ;

         if { $status ne 0 } {
            set freezeTBPath "./freezeTB" ;

            set status [catch {exec $freezeTBPath --version} freezeTBVer ]  ;

            if { $status eq 0 } {
               tk_messageBox -message "freezeTB not found" -title "ERROR" ;

               return 1 ;
            } ; # If: local freezeTB could not be found
         } ; # If: freezeTB not in path
      } ; # check if windows or linux

      #***************************************************
      # Header Sec05 Sub03:
      #   - set up freezeTB command
      #***************************************************

      set status [catch { eval exec $freezeTBPath $argsList } result ] ; # run freezeTB

      if { [ string equal $::errorCode NONE ] } {
         return 0 ;
      } else {
         tk_messageBox -message $result -title "ERROR running freezeTB" ;

         return 1 ;
      } ; # check for errors

      return 0 ;
   } ; # freezeTB

   #>>>script<<<#

   #--------------------------------------------------
   # Tcl01: tcl_isInt_gui
   #   - checks if input key is numeric and if string
   #     is between max and min
   # Input:
   #   - value:
   #     o value (%P) in entery box to check
   #   - index:
   #     o index of character to check (-1 for end)
   #   - min:
   #     o minimum value for value to be a match
   #   - max:
   #     o maximum value for value to be a match
   # Output:
   #   - Returns:
   #     o true if last character in value is numeric
   #       and is between min and max
   #     o false if non-numeric or not min/max range
   #--------------------------------------------------
   proc tcl_isInt_gui { value index min max } {
      if { $index < 0 } {
         set charIn [string index $value "end"] ;
      } else {
         set charIn [string index $value $index] ;
      } ; # Else: index of character input
   
      if { $value eq "" } {
         return true ;         # empty string
   
      } elseif { [string is integer $charIn] } {
         if { $value > $max } {
            return false ;
         } elseif { $value < $min } {
            return false ;      # outside min range
         } else {
            return true ;       # valid number
         } ; # Else If: new character is integer
      } else {
         return false ;         # non-numeric
      } ;
   
      return true ;            # empty case
   } ; # tcl_isInt_gui

   #---------------------------------------------------
   # Tcl02: tcl_isFloat_gui
   #   - checks if input key is numeric and if string
   #     is between max and min
   # Input:
   #   - value:
   #     o value (%P) in entery box to check
   #   - index:
   #     o index of character to check (-1 for end)
   #   - min:
   #     o minimum value for value to be a match
   #   - max:
   #     o maximum value for value to be a match
   # Output:
   #   - Returns:
   #     o true if last character in value is numeric
   #       and is between min and max
   #     o false if non-numeric or not min/max range
   #--------------------------------------------------
   proc tcl_isFloat_gui { value index min max } {
      if { $index < 0 } {
         set charIn [string index $value "end"] ;
      } else {
         set charIn [string index $value $index] ;
      } ; # Else: index of character input
   
      if { $value eq "" } {
         return true ;             # empty string

      } elseif { $charIn eq "." } {
         if {
            [regexp -all {[0-9]*.[0-9]*} $value] < 2
         } {
            return true ;   # first .
         } else {
            return false ;   # second .
         } ; # decimal point input

      } elseif { $value eq "." } {
        return true ;        # only decimal point

      } elseif { [string is integer $charIn] } {
         if { $value > $max } {
            return false ;      # outside max range
         } elseif { $value < $min } {
            return false ;      # outside min range
         } else {
            return true ;       # valid number
         } ; # check if number is in range
      } else {
         return false ;         # non-numeric
      }
   
      return true ;                # empty case
   } ; # tcl_isFloat_gui
   
   #--------------------------------------------------
   # Gui01:
   #   - set up main frame for tcltk
   #---------------------------------------------------
   
   wm title . "Required freezeTB"
   tk::frame .main -borderwidth 2 ;
   pack .main -side left ;
   
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Gui02 TOC:
   #   - required input gui
   #   o gui02 sec01:
   #     - make required input frame
   #   o gui02 sec02:
   #     - set up the fastq label/button
   #   o gui02 sec03:
   #     - set up output directory label/button
   #   o gui02 sec04:
   #     - set up prefix entry
   #   o gui02 sec05:
   #     - set up run and exit button
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui02 Sec01:
   #   - make required input frame
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   tk::frame .main.reqIn -borderwidth 2 ;
   pack .main.reqIn -anchor w -side top ;
   
   tk::frame .main.reqIn.fq -borderwidth 2 ;
   pack .main.reqIn.fq -anchor w -side top ;
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui02 Sec02:
   #   - set up the fastq label/button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   tk::label .main.reqIn.fq.lab -text "" ;
   
   tk::button .main.reqIn.fq.but -text "fastq files" -command { 
           set fqTitle "select fastq files" ;
   
           set fqFiles [
                 tk_getOpenFile -multiple TRUE -title $fqTitle -filetypes $fq_types 
           ] ;
   
           if {$fqFiles eq ""} {
   
           } elseif {[llength $fqFiles] == 1} {
              .main.reqIn.fq.lab configure -text $fqFiles ; 
   
             set ::glob_fqIn $fqFiles ; # update list
           } else {
             .main.reqIn.fq.lab configure -text [file dirname [lindex $fqFiles 1] ] ;
   
             set ::glob_fqIn $fqFiles ; # update list
           } ; # set up fastq label for input
   } ; # command button fires
   
   pack .main.reqIn.fq.but .main.reqIn.fq.lab -side left ; 
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui02 Sec03:
   #   - set up output directory label/button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   tk::frame .main.reqIn.out -borderwidth 2 ;
   pack .main.reqIn.out -anchor w -side top ;
   
   tk::label .main.reqIn.out.lab -text "" ;
   
   tk::button .main.reqIn.out.but -text "output directory" -command { 
            set outDir [
               tk_chooseDirectory -title "select output directory" 
            ] ;
            if {$outDir ne ""} {
               .main.reqIn.out.lab configure -text $outDir ; 

               set ::glob_dirOut $outDir ;
            } ; # If: have output directory
      } ; # output directory button
   
   pack .main.reqIn.out.but .main.reqIn.out.lab -side left -anchor w ; 
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui02 Sec04:
   #   - set up prefix entry
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   tk::frame .main.reqIn.prefix -borderwidth 2 ;
   pack .main.reqIn.prefix -anchor w -side top ;
   
   tk::entry .main.reqIn.prefix.lab -textvariable glob_prefix ; 
   
   pack .main.reqIn.prefix.lab -anchor w -side top

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui02 Sec05:
   #   - set up run and exit button
   #   o gui02 sec05 sub01:
   #     - set up frame for run and exit button
   #   o gui02 sec05 sub02:
   #     - run button
   #   o gui02 sec05 sub03:
   #     - exit button and pack (run/exit)
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #***************************************************
   # Gui02 Sec05 Sub01:
   #   - set up frame for run and exit button
   #***************************************************

   tk::frame .main.reqIn.runexit ;
   pack .main.reqIn.runexit -anchor w -side top ;

   #***************************************************
   # Gui02 Sec05 Sub02:
   #   - run button
   #   o gui02 sec05 sub02 cat01:
   #     - setup + fastq file and prefix checks
   #   o gui02 sec05 sub02 cat02:
   #     - set up minimap2 output file name
   #   o gui02 sec05 sub02 cat03:
   #     - run minimap2
   #   o gui02 sec05 sub02 cat04:
   #     - add freezeTB databases
   #   o gui02 sec05 sub02 cat05:
   #     - add freezeTB filtering settings
   #   o gui02 sec05 sub02 cat06:
   #     - add freezeTB lineage settings
   #   o gui02 sec05 sub02 cat07:
   #     - add freezeTB tbCon consensus settings
   #   o gui02 sec05 sub02 cat08:
   #     - add freezeTB tbCon and AMR print settings
   #   o gui02 sec05 sub02 cat09:
   #     - add freezeTB clustering settings
   #   o gui02 sec05 sub02 cat10:
   #     - run freezeTB
   #   o gui02 sec05 sub02 cat11:
   #     - run output part of gui
   #***************************************************

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui02 Sec05 Sub02 Cat01:
   #   - setup + fastq file and prefix checks
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   proc setMapStatus {} {
         .main.reqIn.runexit.statuslab configure  -text "running minimap2" ;

         wm title . "running minimap2 (freezeTB)" ;
   } ; # sets running minimap2 status

   proc setFreezeTBStatus {} {
         .main.reqIn.runexit.statuslab configure  -text "running freezeTB" ;

         wm title . "running freezeTB" ;
   } ; # sets running freezeTB stats

   tk::button .main.reqIn.runexit.runbut -text "run" -command { 
         if { $::glob_fqIn eq "" } {
            .main.menu.reqBut invoke ;

            tk_messageBox -message "no fastq files input" -title "ERROR" ;

            return false ; # no fastq files
         } ; # no fastq file input

         if { $::glob_prefix eq "" } {
            .main.menu.reqBut invoke ;

            tk_messageBox -message "no prefix input" -title "ERROR" ;
            return false;
         } ; #  no prefix (was deleted)

         #+++++++++++++++++++++++++++++++++++++++++++++
         # Gui02 Sec05 Sub02 Cat02:
         #   - set up minimap2 output file name
         #+++++++++++++++++++++++++++++++++++++++++++++

         set prefix [ file join $::glob_dirOut $::glob_prefix ] ; # build output file path*/

         file mkdir $prefix ;

         if { [file isdirectory $prefix] eq 0 } {
            tk_messageBox -message "failed to make directory" -title "ERROR" ;

           return false
         } ; # failed to make output directory

         set prefix [file join $prefix $::glob_prefix] ;

         set samFile $prefix ;
         append samFile "-map.sam" ;

         #+++++++++++++++++++++++++++++++++++++++++++++
         # Gui02 Sec05 Sub02 Cat03:
         #   - run minimap2
         #+++++++++++++++++++++++++++++++++++++++++++++

         setMapStatus ; # dispaly running minimap2

         .main.reqIn.runexit.statuslab configure  -text "running minimap2" ;

         wm title . "running minimap2 (freezeTB)" ;

         # run minimap2 command
         set status [catch { eval  exec $mapPath -a -x $::glob_srCheck  $::glob_refFa  $::glob_fqIn ">" $samFile  } result  ]  ;

         if { [ string equal $::errorCode NONE ] } {
            # using errorCode here because minimap2
            # outputs to stderr
         } else {
            tk_messageBox -message [concat "Error runing minimap2\n" $result ] -title "ERROR" ;

            return false ;
            # probably need 
         } ; # Else had error
           
         #+++++++++++++++++++++++++++++++++++++++++++++
         # Gui02 Sec05 Sub02 Cat04:
         #   - add freezeTB databases
         #+++++++++++++++++++++++++++++++++++++++++++++

         # set up databases

         set tbCmd [list "-sam" $samFile] ;
         lappend tbCmd "-prefix" $prefix ;
         lappend tbCmd "-amr-tbl" $::glob_amrDb ;
         lappend tbCmd "-gene-coords" ;
         lappend tbCmd $::glob_coordsTsv ;
         lappend tbCmd "-miru-tbl" $::glob_miruDb ;
         lappend tbCmd "-spoligo" $::glob_spacer ;
         lappend tbCmd "-db-spoligo" $::glob_spolDb ;

         if { $::glob_mask ne 0 } {
           lappend tbCmd "-mask-prim" $::glob_maskCoords ;
         } else {
            lappend tbCmd "-mask-prim" "-" ;
         } ; # check if masking primers

         #+++++++++++++++++++++++++++++++++++++++++++++
         # Gui02 Sec05 Sub02 Cat05:
         #   - add freezeTB filtering settings
         #+++++++++++++++++++++++++++++++++++++++++++++

         lappend tbCmd "-min-mapq" $::glob_mapq ;
         lappend tbCmd "-min-median-q" $::glob_medQ ;
         lappend tbCmd "-min-mean-q" $::glob_meanQ ;
         lappend tbCmd "-min-len" $::glob_minLen ;
            # put in consensus gui, but help is filter

         if { $::glob_adjust ne 0 } {
            lappend tbCmd "-adj-coords" ;
         } else {
            lappend tbCmd "-no-adj-coords" ;
         } ; # check if coordinate adjusting

         #+++++++++++++++++++++++++++++++++++++++++++++
         # Gui02 Sec05 Sub02 Cat06:
         #   - add freezeTB lineage settings
         #+++++++++++++++++++++++++++++++++++++++++++++

         lappend tbCmd "-fudge" $::glob_miruFudge ;
         lappend tbCmd "-dr-start" $::glob_drStart ;
         lappend tbCmd "-dr-end" $::glob_drEnd ;

         lappend tbCmd "-spoligo-min-score" ;
         lappend tbCmd $::glob_spolSim ;

         #+++++++++++++++++++++++++++++++++++++++++++++
         # Gui02 Sec05 Sub02 Cat07:
         #   - add freezeTB tbCon consensus settings
         #+++++++++++++++++++++++++++++++++++++++++++++

         lappend tbCmd "-min-depth" $::glob_depth ;
         lappend tbCmd "-min-q" $::glob_snpq ;
         lappend tbCmd "-min-q-ins" $::glob_insq ;
         lappend tbCmd "-perc-snp-sup" ;
         lappend tbCmd $::glob_basePerc ;
         lappend tbCmd "-perc-ins-sup" $::glob_insPerc ;
         lappend tbCmd "-perc-del-sup" $::glob_delPerc ;

         #+++++++++++++++++++++++++++++++++++++++++++++
         # Gui02 Sec05 Sub02 Cat08:
         #   - add freezeTB tbCon & AMR print settings
         #+++++++++++++++++++++++++++++++++++++++++++++

         lappend tbCmd "-min-amr-map-perc" ;
         lappend tbCmd $::glob_amrPercSup ;

         if { $::glob_frameshift ne 0 } {
            lappend tbCmd "-frameshift" ;
         } else {
            lappend tbCmd "-no-frameshift" ;
         } ; # check if doing frameshift checking
 
         lappend tbCmd "-p-min-depth" ;
         lappend tbCmd $::glob_minVarDepth ;

         lappend tbCmd "-p-perc-snp-sup" ;
         lappend tbCmd glob_baseVarPerc ;

         lappend tbCmd "-p-perc-ins-sup" ;
         lappend tbCmd $::glob_insVarPerc ;

         lappend tbCmd "-p-perc-del-sup" ;
         lappend tbCmd $::glob_delVarPerc ;

         #+++++++++++++++++++++++++++++++++++++++++++++
         # Gui02 Sec05 Sub02 Cat09:
         #   - add freezeTB clustering settings
         #+++++++++++++++++++++++++++++++++++++++++++++

         if { $::glob_clustBl ne 0 } {
            lappend tbCmd "-clust" ;

            lappend tbCmd "-len-weigth" ;
            lappend tbCmd $::glob_lenWeight ;
           
            lappend tbCmd "-clust-depth" ;
            lappend tbCmd $::glob_minClustDepth ;

            lappend tbCmd "-clust-perc-depth" ;
            lappend tbCmd $::glob_minClustPercDepth ;

            lappend tbCmd "-read-err" ;
            lappend tbCmd $::glob_clustReadErr ;

            lappend tbCmd "-con-err" ;
            lappend tbCmd $::glob_clustConErr ;

            lappend tbCmd "-con-sim" ;
            lappend tbCmd $::glob_maxClustSim ;

            lappend tbCmd "-overlap" ;
            lappend tbCmd $::glob_minClustOverlap ;

            lappend tbCmd "-perc-n" ;
            lappend tbCmd $::glob_maxClustMask ;

            if { $::glob_depthProfBl ne 0 } {
               lappend tbCmd "-depth-prof" ;
            } else {
               lappend tbCmd "-no-depth-prof" ;
            } ; # check if doing depth profiling

            lappend tbCmd "-err-to-var" ;
            lappend tbCmd $::glob_clustErrRatio ;

            lappend tbCmd "-win-len" ;
            lappend tbCmd $::glob_clustWinLen ;

            lappend tbCmd "-win-err" ;
            lappend tbCmd $::glob_clustWinErrRatio ;

            lappend tbCmd "-indel-len" ;
            lappend tbCmd $::glob_minClustIndelLen ;

            lappend tbCmd "-clust-q-snp" ;
            lappend tbCmd $::glob_minClustSnpQ ;

            lappend tbCmd "-con-iter" ;
            lappend tbCmd $::glob_numRebuilds ;
         } else {
            lappend tbCmd "-no-clust" ;
         } ; # check if clustering

         #+++++++++++++++++++++++++++++++++++++++++++++
         # Gui02 Sec05 Sub02 Cat10:
         #   - run freezeTB
         #+++++++++++++++++++++++++++++++++++++++++++++

         setFreezeTBStatus ;

         #>>>script<<<#
         if { [ freezeTBProc $tbCmd ] } {
         
         #>>>script<<<#
            tk_messageBox -message "freezeTB error" -title "ERROR" ; # catching stderr would be tricky
            # this stack overflow has a solution
            # https://stackoverflow.com/questions/64547745/capture-stdout-within-a-tcl-catch-command

            return false;
         } ; # If had error

         #+++++++++++++++++++++++++++++++++++++++++++++
         # Gui02 Sec05 Sub02 Cat11:
         #   - run output part of freezeTB
         #+++++++++++++++++++++++++++++++++++++++++++++

         wm title . "Required freezeTB" ;

         .main.reqIn.runexit.statuslab configure  -text "" ;

         set ::glob_outCur $prefix ;
         set ::glob_outPref $prefix ;

         .main.menu.outBut invoke ;
         .main.out.set.run.but invoke ;
      } ; # run freezeTB button

   #***************************************************
   # Gui02 Sec05 Sub03:
   #   - exit button and pack (run/exit)
   #***************************************************

   tk::button .main.reqIn.runexit.exitbut -text "exit" -command {exit}  ;

   tk::label .main.reqIn.runexit.statuslab -text "" ;

   pack .main.reqIn.runexit.runbut .main.reqIn.runexit.exitbut .main.reqIn.runexit.statuslab -anchor w -side left ;
   
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Gui03 TOC:
   #   - menu bar for gui
   #   o gui03 sec01:
   #     - make menu frame
   #   o gui03 sec02:
   #     - set up required input button
   #   o gui03 sec03:
   #     - set up read filtering button
   #   o gui03 sec04:
   #     - set up AMR menu button
   #   o gui03 sec05:
   #     - set up lineage menu button
   #   o gui03 sec06:
   #     - set up consensus menu button
   #   o gui03 sec07:
   #     - set up output settings menu
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui03 Sec01:
   #   - make menu frame
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   tk::frame .main.menu -borderwidth 2 ;
   pack .main.menu -anchor w -side top ;
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui03 Sec02:
   #   - set up required input button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   tk::button .main.menu.reqBut -text "required" -relief sunken -state disabled -command { 
         pack forget .main.reqIn ;
         pack forget .main.filt ;
         pack forget .main.amr ;
         pack forget .main.lin ;
         pack forget .main.con ;
         pack forget .main.menu ;
         pack forget .main.out ;
   
         pack .main.reqIn -anchor w -side top ;
         pack .main.menu -anchor w -side top ;

         .main.menu.reqBut configure -relief sunken ;
         .main.menu.filtBut configure -relief raised ;
         .main.menu.amrBut configure -relief raised ;
         .main.menu.linBut configure -relief raised ;
         .main.menu.conBut configure -relief raised ;
         .main.menu.outBut configure -relief raised ;

         .main.menu.reqBut configure -state disabled ;
         .main.menu.filtBut configure -state normal ;
         .main.menu.amrBut configure -state normal ;
         .main.menu.linBut configure -state normal ;
         .main.menu.conBut configure -state normal ;
         .main.menu.outBut configure -state normal ;

         wm title . "Required freezeTB" ;
   } ; # command button fires

   pack .main.menu.reqBut -anchor w -side left
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui03 Sec03:
   #   - set up read filtering button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   tk::button .main.menu.filtBut -text "filtering" -relief raised -state normal -command { 
         pack forget .main.reqIn ;
         pack forget .main.filt ;
         pack forget .main.amr ;
         pack forget .main.lin ;
         pack forget .main.con ;
         pack forget .main.menu ;
         pack forget .main.out ;
   
         pack .main.filt -anchor w -side top ;
         pack .main.menu -anchor w -side top ;

         .main.menu.reqBut configure -relief raised ;
         .main.menu.filtBut configure -relief sunken ;
         .main.menu.amrBut configure -relief raised ;
         .main.menu.linBut configure -relief raised ;
         .main.menu.conBut configure -relief raised ;
         .main.menu.outBut configure -relief raised ;

         .main.menu.reqBut configure -state normal ;
         .main.menu.filtBut configure -state disabled ;
         .main.menu.amrBut configure -state normal ;
         .main.menu.linBut configure -state normal ;
         .main.menu.conBut configure -state normal ;
         .main.menu.outBut configure -state normal ;

         wm title . "Filtering freezeTB" ;
   } ; # command button fires
   
   pack .main.menu.filtBut -anchor w -side left

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui03 Sec04:
   #   - set up AMR menu button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   tk::button .main.menu.amrBut -text "AMRs" -relief raised -state normal -command { 
         pack forget .main.reqIn ;
         pack forget .main.filt ;
         pack forget .main.amr ;
         pack forget .main.lin ;
         pack forget .main.con ;
         pack forget .main.menu ;
   
         pack .main.amr -anchor w -side top ;
         pack .main.menu -anchor w -side top ;

         .main.menu.reqBut configure -relief raised ;
         .main.menu.filtBut configure -relief raised ;
         .main.menu.amrBut configure -relief sunken ;
         .main.menu.conBut configure -relief raised ;
         .main.menu.outBut configure -relief raised ;

         .main.menu.reqBut configure -state normal ;
         .main.menu.filtBut configure -state normal ;
         .main.menu.amrBut configure -state disabled ;
         .main.menu.linBut configure -state normal ;
         .main.menu.conBut configure -state normal ;

         wm title . "AMRs freezeTB" ;
   } ; # command button fires
   
   pack .main.menu.amrBut -anchor w -side left
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui03 Sec05:
   #   - set up lineage menu button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::button .main.menu.linBut -text "lineage" -relief raised -state normal -command { 
         pack forget .main.reqIn ;
         pack forget .main.filt ;
         pack forget .main.amr ;
         pack forget .main.lin ;
         pack forget .main.con ;
         pack forget .main.menu ;
         pack forget .main.out ;
   
         pack .main.lin -anchor w -side top ;
         pack .main.menu -anchor w -side top ;

         .main.menu.reqBut configure -relief raised ;
         .main.menu.filtBut configure -relief raised ;
         .main.menu.amrBut configure -relief raised ;
         .main.menu.linBut configure -relief sunken ;
         .main.menu.conBut configure -relief raised ;
         .main.menu.outBut configure -relief raised ;

         .main.menu.reqBut configure -state normal ;
         .main.menu.filtBut configure -state normal ;
         .main.menu.amrBut configure -state normal ;
         .main.menu.linBut configure -state disabled ;
         .main.menu.conBut configure -state normal ;
         .main.menu.outBut configure -state normal ;

         wm title . "Lineage freezeTB" ;
   } ; # command button fires

   pack .main.menu.linBut -anchor w -side left

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui03 Sec06:
   #   - set up consensus menu button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::button .main.menu.conBut -text "consensus" -relief raised -state normal -command { 
         pack forget .main.reqIn ;
         pack forget .main.filt ;
         pack forget .main.amr ;
         pack forget .main.lin ;
         pack forget .main.con ;
         pack forget .main.menu ;
         pack forget .main.out ;
   
         pack .main.con -anchor w -side top ;
         pack .main.menu -anchor w -side top ;

         set conState [.main.con.menu.tbBut configure -state] ;
         set conClick [lsearch $conState "disabled"] ;

         set varState [.main.con.menu.varBut configure -state] ;
         set varClick [lsearch $varState "disabled"] ;

         if { $conClick >= 0 } {
            wm title . "Consensus tbCon freezeTB" ;
         } elseif { $varClick >= 0 } {
            wm title . "Consensus variant tbCon freezeTB" ;
         } elseif { $varClick >= 0 } {
         } else {
            wm title . "Consensus cluster freezeTB" ;
         }

         .main.menu.reqBut configure -relief raised ;
         .main.menu.filtBut configure -relief raised ;
         .main.menu.amrBut configure -relief raised ;
         .main.menu.linBut configure -relief raised ;
         .main.menu.conBut configure -relief sunken ;
         .main.menu.outBut configure -relief raised ;

         .main.menu.reqBut configure -state normal ;
         .main.menu.filtBut configure -state normal ;
         .main.menu.amrBut configure -state normal ;
         .main.menu.linBut configure -state normal ;
         .main.menu.conBut configure -state disabled ;
         .main.menu.outBut configure -state normal ;

   } ; # command button fires

   pack .main.menu.conBut -anchor w -side left

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui03 Sec07:
   #   - set up output settings menu
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::button .main.menu.outBut -text "output" -relief raised -state normal -command { 

         pack forget .main.reqIn ;
         pack forget .main.filt ;
         pack forget .main.amr ;
         pack forget .main.lin ;
         pack forget .main.con ;
         pack forget .main.menu ;
         pack forget .main.out ;

         .main.menu.reqBut configure -relief raised ;
         .main.menu.filtBut configure -relief raised ;
         .main.menu.amrBut configure -relief raised ;
         .main.menu.linBut configure -relief raised ;
         .main.menu.conBut configure -relief raised ;
         .main.menu.outBut configure -relief raised ;

         .main.menu.reqBut configure -state normal ;
         .main.menu.filtBut configure -state normal ;
         .main.menu.amrBut configure -state normal ;
         .main.menu.linBut configure -state normal ;
         .main.menu.conBut configure -state normal ;
         .main.menu.outBut configure -state normal ;

         pack .main.out ;
         .main.out.menu.setBut invoke ;
      } ; # output settings button

   pack .main.menu.outBut -anchor w -side left ;

   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Gui04 TOC:
   #   - read filtering menu
   #   o gui04 sec01:
   #     - make read filtering frame
   #   o gui04 sec02:
   #     - reference genome input
   #   o gui04 sec03:
   #     - coordinates file input
   #   o gui04 sec04:
   #     - mapq entry input
   #   o gui04 sec05:
   #     - medain q-score
   #   o gui04 sec06:
   #     - mean q-score
   #   o gui04 sec07:
   #     - minimum read/consensus length
   #   o gui04 sec08:
   #     - masking check box + button
   #   o gui04 sec09:
   #     - adjust coordinates text box
   #   o gui04 sec10:
   #     - short read mapping check box
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec01:
   #   - make read filtering frame
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   tk::frame .main.filt -borderwidth 2 ;
   
   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec02:
   #   - reference genome input
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.ref ;
   pack .main.filt.ref -anchor w -side top ;

   tk::label .main.filt.ref.lab -text $::glob_refFa ;

   tk::button .main.filt.ref.but -text "reference" -command { 
           set faFile [
               tk_getOpenFile -title "select reference (as fasta)" -filetypes $fa_types ] ; # find the reference fasta
   
           if {$faFile eq ""} {
   
           } else {
              .main.filt.ref.lab configure -text $faFile ;  
   
             set ::glob_refFa $faFile ; # update list
           } ; # Else: fasta selected
   } ; # select reference button

   pack .main.filt.ref.but .main.filt.ref.lab -anchor w -side left 

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec03:
   #   - coordinates file input
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.coords ;
   pack .main.filt.coords -anchor w -side top ;

   tk::label .main.filt.coords.lab -text $::glob_coordsTsv  ;

   tk::button .main.filt.coords.but -text "coordinates file" -command { 
           set tsvFile [
              tk_getOpenFile -title "select gene coordinate file" -filetypes $tsv_types  ] ; # find gene coordinates file
   
           if {$tsvFile eq ""} {
   
           } else {
              .main.filt.coords.lab configure -text $tsvFile  ;

             set ::glob_coordsTsv $tsvFile ;
           } ; # Else: file selected
   } ; # select reference button

   pack .main.filt.coords.but .main.filt.coords.lab -anchor w -side left 

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec04:
   #   - mapq entry input
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   
   tk::frame .main.filt.mapq
   pack .main.filt.mapq -anchor w -side top
   
   tk::label .main.filt.mapq.lab -text "mapq (0-93)" 

   tk::entry .main.filt.mapq.entry -textvariable glob_mapq -validate key  -vcmd { tcl_isInt_gui %P %i 0 93 }  ; # get mapqping quality
   
   pack .main.filt.mapq.lab .main.filt.mapq.entry -anchor w -side left ; 

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec05:
   #   - median q-score
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.medQ
   pack .main.filt.medQ -anchor w -side top

   tk::label .main.filt.medQ.lab -text "median Q (0-93)"  ; # medain q-score entry leabe

   tk::entry .main.filt.medQ.entry -textvariable glob_medQ -validate key -vcmd { tcl_isInt_gui %P %i 0 93 }  ; # get median q-score

   pack .main.filt.medQ.lab .main.filt.medQ.entry -anchor w -side left 

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec06:
   #   - mean q-score
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.meanQ
   pack .main.filt.meanQ -anchor w -side top

   tk::label .main.filt.meanQ.lab -text "min mean Q (0-93)"  ; # medain q-score entry leabe

   tk::entry .main.filt.meanQ.entry -textvariable glob_meanQ -validate key -vcmd { tcl_isInt_gui %P %i 0 93 }  ; # get mean q-score

   pack .main.filt.meanQ.lab .main.filt.meanQ.entry -anchor w -side left 

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec07:
   #   - minimum read/consensus length
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.len
   pack .main.filt.len -anchor w -side top

   tk::label .main.filt.len.lab -text "min length (> 0)"  ; # medain q-score entry leabe

   tk::entry .main.filt.len.entry -textvariable glob_minLen -validate key -vcmd { tcl_isInt_gui %P %i 1 2000000000 }  ; # get minimum read length

   pack .main.filt.len.lab .main.filt.len.entry -anchor w -side left 

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec08:
   #   - masking check box + button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.mask
   pack .main.filt.mask -anchor w -side top

   tk::checkbutton .main.filt.mask.check -text "mask primers"  -variable glob_mask  ; # if doing long or short reads

   tk::label .main.filt.mask.lab -text $::glob_maskCoords  ; # label for masking file

   tk::button .main.filt.mask.but -text "masking coordinates" -command { 
         set maskFile [
            tk_getOpenFile -title "maksing coordinates" -filetypes $tsv_types 
         ] ;

         if {$maskFile eq "" } {

         } else {
            .main.filt.mask.lab configure -text $maskFile ; 
            set ::glob_maskCoords $maskFile ;

            # turn on the check button
            .main.filt.mask.check select ;
         } ; # Else: masking coordinates input
      } ; # button to get mask coordinates file

   pack .main.filt.mask.check .main.filt.mask.but .main.filt.mask.lab -anchor w -side left 

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec09:
   #   - adjust coordinates text box
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.adj ;
   pack .main.filt.adj -anchor w -side top ;

   tk::checkbutton .main.filt.adj.check -text "adjust coordinates"  -variable glob_adjust  ; # if doing long or short reads

   pack .main.filt.adj.check -anchor w -side left ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec10:
   #   - short read mapping check box
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.sr
   pack .main.filt.sr -anchor w -side top

   tk::checkbutton .main.filt.sr.check -offvalue "-x map-ont"  -onvalue "-x sr"  -text "short reads"  -variable glob_srCheck  ; # if doing long or short reads

   pack .main.filt.sr.check -anchor w -side left ;

   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Gui05 TOC:
   #   - amr settings
   #   o gui05 sec01:
   #     - set up AMR frame
   #   o gui05 sec02:
   #     - get amr database
   #   o gui05 sec03:
   #     - min read AMR percent support
   #   o gui05 sec05:
   #     - frameshift check box
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui05 Sec01:
   #   - set up AMR frame
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.amr ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui05 Sec02:
   #   - get amr database
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.amr.db ;
   pack .main.amr.db -anchor w -side top ;

   tk::label .main.amr.db.lab -text $::glob_amrDb

   tk::button .main.amr.db.but -text "AMR database" -command { 
         set tsvFile [
            tk_getOpenFile -title "select AMR database" -filetypes $tsv_types ] ;
          if {$tsvFile eq "" } {

          } else {
            .main.amr.db.lab configure -text $tsvFile ;
            set ::glob_amrDb $tsvFile ;
          } ; # Else: AMR database input
     } ; # amr database button

   pack .main.amr.db.but .main.amr.db.lab -anchor w -side left  

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui05 Sec03:
   #   - min read AMR percent support
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.amr.sup ;
   pack .main.amr.sup -anchor w -side top ;

   tk::label .main.amr.sup.lab -text "% AMR support (0-1)"  ;

   tk::entry .main.amr.sup.entry -textvariable glob_amrPercSup -validate key -vcmd { tcl_isFloat_gui %P %i 0 1 }  ; # get minimum percent read depth for AMR

   pack .main.amr.sup.lab .main.amr.sup.entry -anchor w -side left  ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui05 Sec04:
   #   - frameshift check box
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.amr.frameshift ;
   pack .main.amr.frameshift -anchor w -side top ;

   tk::checkbutton .main.amr.frameshift.check -text "check frameshifts" -variable glob_frameshift  ;

   pack .main.amr.frameshift.check -anchor w -side left  ;

   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Gui06 TOC:
   #   - lineage settings
   #   o gui06 sec01:
   #     - set up lineage frame
   #   o gui06 sec02:
   #     - databases (MIRU and spoligo)
   #   o gui06 sec03:
   #     - MIRU-VNTR lineages
   #   o gui06 sec04:
   #     - spoligotyping lineages
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui06 Sec01:
   #   - set up lineage frame
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.lin ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui06 Sec02:
   #   - databases (MIRU and spoligo)
   #   o gui06 sec02 sub01:
   #     - make database frame
   #   o gui06 sec02 sub02:
   #     - set up miru database button
   #   o gui06 sec02 sub03:
   #     - set up spoligotype sequences button
   #   o gui06 sec02 sub04:
   #     - set up spoligotype database button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #***************************************************
   # Gui06 Sec02 Sub01:
   #   - make database frame
   #***************************************************

   tk::frame .main.lin.db ;
   pack .main.lin.db -anchor w -side top ;

   #***************************************************
   # Gui06 Sec02 Sub02:
   #   - set up miru database button
   #***************************************************

   tk::frame .main.lin.db.miru ;
   pack .main.lin.db.miru -anchor w -side top ;

   tk::label .main.lin.db.miru.lab -text $::glob_miruDb ;

   tk::button .main.lin.db.miru.but -text "MIRU-VNTR database" -command { 
         set tsvFile [
            tk_getOpenFile -title "select MIRU-VNTR database" -filetypes $tsv_types  ] ; # get MIRU databse

         if { $tsvFile eq "" } {

         } else {
            .main.lin.db.miru.lab configure -text $tsvFile ;
             set ::glob_miruDb $tsvFile ;
         } ; # Else: miru database selected
   } ; # -command get MIRU database

   pack .main.lin.db.miru.but .main.lin.db.miru.lab -anchor w -side left 

   #***************************************************
   # Gui06 Sec02 Sub03:
   #   - set up spoligotype sequences button
   #***************************************************

   tk::frame .main.lin.db.spacer ;
   pack .main.lin.db.spacer -anchor w -side top ;

   tk::label .main.lin.db.spacer.lab -text $::glob_spacer  ; # holds spacer database location

   tk::button .main.lin.db.spacer.but -text "spoligotype spacers" -command { 
         set spacerFa [
            tk_getOpenFile -title "spoligotype spacer sequences" -filetypes $fa_types  ] ; # get spacer sequence fasta file

         if {$spacerFa eq ""} {

         } else {
            .main.lin.db.spacer.lab configure -text $spacerFa ;
            set ::glob_spacer $spacerFa ;
         } ; # Else: user input fasta
   } ; # button for spacers database

   pack .main.lin.db.spacer.but .main.lin.db.spacer.lab -anchor w -side left  ;   

   #***************************************************
   # Gui06 Sec02 Sub04:
   #   - set up spoligotype database button
   #***************************************************

   tk::frame .main.lin.db.spol ;
   pack .main.lin.db.spol -anchor w -side top ;

   tk::label .main.lin.db.spol.lab -text $::glob_spolDb  ; # holds spol database location

   tk::button .main.lin.db.spol.but -text "spoligotype database" -command { 
         set spolTsv [
            tk_getOpenFile -title "spoligotype lineage database" -filetypes $csv_types  ] ;

         if {$spolTsv eq ""} {

         } else {
            .main.lin.db.spol.lab configure -text $spolTsv ;
            set ::glob_spolDb $spolTsv ;
         } ; # Else: user input csv
   } ; # button for spols database

   pack .main.lin.db.spol.but .main.lin.db.spol.lab -anchor w -side left  ;   

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui06 Sec03:
   #   - MIRU-VNTR lineages
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.lin.miru ;
   pack .main.lin.miru -anchor w -side top ;

   tk::label .main.lin.miru.lab -text "fudge (0-100)"  ;

   tk::entry .main.lin.miru.entry -textvariable glob_miruFudge -validate key -vcmd { tcl_isInt_gui %P %i 0 100 } ;  # -command for fudge entry

   pack .main.lin.miru.lab .main.lin.miru.entry -anchor w -side left  ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui06 Sec04:
   #   - spoligotyping lineages
   #   o gui06 sec04 sub01:
   #     - set up spoligotype frame
   #   o gui06 sec04 sub02:
   #     - set up min percent similarity
   #   o gui06 sec04 sub03:
   #     - direct repeat frame
   #   o gui06 sec04 sub04:
   #     - direct repeat starting coordinates
   #   o gui06 sec04 sub05:
   #     - direct repeat ending coordinates
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #***************************************************
   # Gui06 Sec04 Sub01:
   #   - set up spoligotype frame
   #***************************************************

   tk::frame .main.lin.spol ;
   pack .main.lin.spol -anchor w -side top ;

   #***************************************************
   # Gui06 Sec04 Sub02:
   #   - set up min percent similarity
   #***************************************************

   tk::frame .main.lin.spol.sim ;
   pack .main.lin.spol.sim -anchor w -side top ;

   tk::label .main.lin.spol.sim.lab -text "spoligo min % match (0-1)"  ;

   tk::entry .main.lin.spol.sim.entry -textvariable glob_spolSim -validate key -vcmd { tcl_isFloat_gui %P %i 0 1 }  ; # entry for minimum percent match

   pack .main.lin.spol.sim.lab .main.lin.spol.sim.entry -anchor w -side left  ;

   #***************************************************
   # Gui06 Sec04 Sub03:
   #   - direct repeat frame
   #***************************************************

   tk::frame .main.lin.spol.dr ;
   pack .main.lin.spol.dr -anchor w -side top ;

   tk::frame .main.lin.spol.dr.dr ;
   pack .main.lin.spol.dr.dr -anchor w -side top ;

   tk::label .main.lin.spol.dr.dr.lab -text "Direct Repeat Coordinates:"  ;

   pack .main.lin.spol.dr.dr.lab -anchor w -side left ;

   #***************************************************
   # Gui06 Sec04 Sub04:
   #   - direct repeat starting coordinates
   #***************************************************

   tk::frame .main.lin.spol.dr.start ;
   pack .main.lin.spol.dr.start -anchor w -side top ;

   tk::label .main.lin.spol.dr.start.lab -text "    start (> 0):"  ;

   tk::entry .main.lin.spol.dr.start.entry -textvariable glob_drStart -validate key -vcmd { tcl_isInt_gui %P %i 1 2000000000 }  ; # direct repeat start

   pack .main.lin.spol.dr.start.lab .main.lin.spol.dr.start.entry -anchor w -side left  ;

   #***************************************************
   # Gui06 Sec04 Sub05:
   #   - direct repeat ending coordinates
   #***************************************************

   tk::frame .main.lin.spol.dr.end ;
   pack .main.lin.spol.dr.end -anchor w -side top ;

   tk::label .main.lin.spol.dr.end.lab -text "    end (> 0):"  ;

   tk::entry .main.lin.spol.dr.end.entry -textvariable glob_drEnd -validate key -vcmd { tcl_isInt_gui %P %i 1 2000000000 }  ; # direct repeat end

   pack .main.lin.spol.dr.end.lab .main.lin.spol.dr.end.entry -anchor w -side left  ;

   

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui07 Sec01:
   #   - set up consensus frame
   #   o gui07 sec01 sub01:
   #     - set up consensus frame and sub frames
   #   o gui07 sec01 sub02:
   #     - set up tbCon menu button
   #   o gui07 sec01 sub03:
   #     - set up tbCon variant menu button
   #   o gui07 sec01 sub04:
   #     - set up cluster menu button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #***************************************************
   # Gui07 Sec01 Sub01:
   #   - set up consensus frame and sub frames
   #***************************************************

   tk::frame .main.con ;
   
   tk::frame .main.con.menu ;  # tbCon frame
   tk::frame .main.con.tb ;    # tbCon frame
   tk::frame .main.con.var ;   # tbCon frame
   tk::frame .main.con.clust ; # edClust frame

   pack .main.con.menu -anchor w -side top ;
   pack .main.con.tb -anchor w -side top ;

   #***************************************************
   # Gui07 Sec01 Sub02:
   #   - set up tbCon menu button
   #***************************************************

   tk::button .main.con.menu.tbBut -text "tbCon" -relief sunken -state disabled -command { 
         pack forget .main.con.menu ;
         pack forget .main.con.tb ;
         pack forget .main.con.var ;
         pack forget .main.con.clust ;

         pack .main.con.menu -anchor w -side top ;
         pack .main.con.tb -anchor w -side top ;

         wm title . "Consensus tbCon freezeTB" ;

         .main.con.menu.tbBut configure -state disabled -relief sunken ;
         .main.con.menu.varBut configure -state normal -relief raised ;
         .main.con.menu.clustBut configure -state normal -relief raised ;
      } ; # show tbCon menu

   pack .main.con.menu.tbBut -anchor w -side left ;

   #***************************************************
   # Gui07 Sec01 Sub03:
   #   - set up tbCon variant menu button
   #***************************************************

   tk::button .main.con.menu.varBut -text "tbCon variant" -relief raised -state normal -command { 
         pack forget .main.con.menu ;
         pack forget .main.con.tb ;
         pack forget .main.con.var ;
         pack forget .main.con.clust ;

         pack .main.con.menu -anchor w -side top ;
         pack .main.con.var -anchor w -side top ;

         wm title . "Consensus Variant tbCon freezeTB" ;
         .main.con.menu.tbBut configure -state normal -relief raised ;
         .main.con.menu.varBut configure -state disabled -relief sunken ;
         .main.con.menu.clustBut configure -state normal -relief raised ;
      } ; # show tbCon variant menu

   pack .main.con.menu.varBut -anchor w -side left ;

   #***************************************************
   # Gui07 Sec01 Sub04:
   #   - set up cluster menu button
   #***************************************************

   tk::button .main.con.menu.clustBut -text "cluster" -relief raised -state normal -command { 
         pack forget .main.con.menu ;
         pack forget .main.con.tb ;
         pack forget .main.con.var ;
         pack forget .main.con.clust ;

         pack .main.con.menu -anchor w -side top ;
         pack .main.con.clust -anchor w -side top ;

         wm title . "Consensus cluster freezeTB" ;

         .main.con.menu.tbBut configure -state normal -relief raised ;
         .main.con.menu.varBut configure -state normal -relief raised ;
         .main.con.menu.clustBut configure -state disabled -relief sunken ;
      } ; # show edClust menu

      pack .main.con.menu.clustBut -anchor w -side left ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui07 Sec02:
   #   - set up consensus frame
   #   o gui07 sec02 sub01:
   #     - min snp q-score
   #   o gui07 sec02 sub02:
   #     - min ins q-score
   #   o gui07 sec02 sub03:
   #     - min ins q-score
   #   o gui07 sec02 sub04:
   #     - min base percent
   #   o gui07 sec02 sub05:
   #     - min insertion percent
   #   o gui07 sec02 sub06:
   #     - min deletion percent
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #***************************************************
   # Gui07 Sec02 Sub01:
   #   - min snp q-score
   #***************************************************

   tk::frame .main.con.tb.snpq ;
   pack .main.con.tb.snpq -anchor w -side top ;

   tk::label .main.con.tb.snpq.lab -text "Min snp q-score (0-93)" ;

   tk::entry .main.con.tb.snpq.entry -textvariable glob_snpq -validate key -vcmd {tcl_isInt_gui %P %i 0 93 }  ; # min snp q-score entry

   pack .main.con.tb.snpq.lab .main.con.tb.snpq.entry -anchor w -side left ;

   #***************************************************
   # Gui07 Sec02 Sub02:
   #   - min ins q-score
   #***************************************************

   tk::frame .main.con.tb.insq ;
   pack .main.con.tb.insq -anchor w -side top ;

   tk::label .main.con.tb.insq.lab -text "Min insertion q-score (0-93)" ;

   tk::entry .main.con.tb.insq.entry -textvariable glob_insq -validate key -vcmd {tcl_isInt_gui %P %i 0 93 }  ; # min ins q-score entry

   pack .main.con.tb.insq.lab .main.con.tb.insq.entry -anchor w -side left ;

   #***************************************************
   # Gui07 Sec02 Sub03:
   #   - min read depth
   #***************************************************

   tk::frame .main.con.tb.depth ;
   pack .main.con.tb.depth -anchor w -side top ;

   tk::label .main.con.tb.depth.lab -text "Min read depth (> 0)" ;

   tk::entry .main.con.tb.depth.entry -textvariable glob_depth -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ;

   pack .main.con.tb.depth.lab .main.con.tb.depth.entry -anchor w -side left ;

   #***************************************************
   # Gui07 Sec02 Sub04:
   #   - min base percent
   #***************************************************

   tk::frame .main.con.tb.baseperc ;
   pack .main.con.tb.baseperc -anchor w -side top ;

   tk::label .main.con.tb.baseperc.lab -text "Min % base support (0-1)" ;

   tk::entry .main.con.tb.baseperc.entry -textvariable glob_basePerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # min percent depth for base

   pack .main.con.tb.baseperc.lab .main.con.tb.baseperc.entry -anchor w -side left ;

   #***************************************************
   # Gui07 Sec02 Sub05:
   #   - min insertion percent
   #***************************************************

   tk::frame .main.con.tb.insperc ;
   pack .main.con.tb.insperc -anchor w -side top ;

   tk::label .main.con.tb.insperc.lab -text "Min % insertion support (0-1)" ;

   tk::entry .main.con.tb.insperc.entry -textvariable glob_insPerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # min percent depth for ins

   pack .main.con.tb.insperc.lab .main.con.tb.insperc.entry -anchor w -side left ;

   #***************************************************
   # Gui07 Sec02 Sub06:
   #   - min deletion percent
   #***************************************************

   tk::frame .main.con.tb.delperc ;
   pack .main.con.tb.delperc -anchor w -side top ;

   tk::label .main.con.tb.delperc.lab -text "Min % deletion support (0-1)" ;

   tk::entry .main.con.tb.delperc.entry -textvariable glob_delPerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # min percent depth for del

   pack .main.con.tb.delperc.lab .main.con.tb.delperc.entry -anchor w -side left ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui07 Sec03:
   #   - set up variant report frame for consensus
   #   o gui07 sec03 sub01:
   #     - min read depth
   #   o gui07 sec03 sub02:
   #     - min base percent
   #   o gui07 sec03 sub03:
   #     - min insertion percent
   #   o gui07 sec03 sub04:
   #     - min deletion percent
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #***************************************************
   # Gui07 Sec03 Sub01:
   #   - min read depth
   #***************************************************

   tk::frame .main.con.var.depth ;
   pack .main.con.var.depth -anchor w -side top ;

   tk::label .main.con.var.depth.lab -text "Variant: min read depth (> 0)" ;

   tk::entry .main.con.var.depth.entry -textvariable glob_minVarDepth -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ; # min snp q-score entry

   pack .main.con.var.depth.lab .main.con.var.depth.entry -anchor w -side left ;

   #***************************************************
   # Gui07 Sec03 Sub04:
   #   - min base percent
   #***************************************************

   tk::frame .main.con.var.baseperc ;
   pack .main.con.var.baseperc -anchor w -side top ;

   tk::label .main.con.var.baseperc.lab -text "Variant: min % base support (0-1)" ;

   tk::entry .main.con.var.baseperc.entry -textvariable glob_baseVarPerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # min percent depth for base

   pack .main.con.var.baseperc.lab .main.con.var.baseperc.entry -anchor w -side left ;

   #***************************************************
   # Gui07 Sec03 Sub05:
   #   - min insertion percent
   #***************************************************

   tk::frame .main.con.var.insperc ;
   pack .main.con.var.insperc -anchor w -side top ;

   tk::label .main.con.var.insperc.lab -text "Variant: min % insertion support (0-1)" ;

   tk::entry .main.con.var.insperc.entry -textvariable glob_insVarPerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # min percent depth for ins

   pack .main.con.var.insperc.lab .main.con.var.insperc.entry -anchor w -side left ;

   #***************************************************
   # Gui07 Sec03 Sub05:
   #   - min deletion percent
   #***************************************************

   tk::frame .main.con.var.delperc ;
   pack .main.con.var.delperc -anchor w -side top ;

   tk::label .main.con.var.delperc.lab -text "Variant: min % deletion support (0-1)" ;

   tk::entry .main.con.var.delperc.entry -textvariable glob_delVarPerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # min percent depth for del

   pack .main.con.var.delperc.lab .main.con.var.delperc.entry -anchor w -side left ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui07 Sec04:
   #   - set up edClust menu
   #   o gui07 sec04 sub01:
   #     - set up cluster frame
   #   o gui07 sec04 sub02:
   #     - set up cluster flitering
   #   o gui07 sec04 sub03:
   #     - set up cluster calculations
   #   o gui07 sec04 sub04:
   #     - set up cluster consnesus filtering
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #***************************************************
   # Gui07 Sec04 Sub01:
   #   - set up cluster frame
   #***************************************************

   tk::frame .main.con.clust.mid ;

   tk::checkbutton .main.con.clust.check -text "cluster" -variable glob_clustBl 

   pack .main.con.clust.check -anchor w -side top ;
   pack .main.con.clust.mid -anchor w -side top ;

   tk::frame .main.con.clust.mid.filt -borderwidth 2 -relief groove ;

   tk::frame .main.con.clust.mid.calc -borderwidth 2 -relief groove ;

   tk::frame .main.con.clust.con -borderwidth 2 -relief groove ;

   pack .main.con.clust.mid.filt -anchor w -side left ;

   pack .main.con.clust.mid.calc -anchor e -side right ;

   pack .main.con.clust.con -anchor s -side top ;

   #***************************************************
   # Gui07 Sec04 Sub02:
   #   - set up cluster flitering
   #   o gui07 sec04 sub02 cat01:
   #     - min read depth
   #   o gui07 sec04 sub02 cat02:
   #     - min percent read depth
   #   o gui07 sec04 sub02 cat03:
   #     - min snp q-score
   #   o gui07 sec04 sub02 cat04:
   #     - min indel len
   #   o gui07 sec04 sub02 cat05:
   #     - depth profiling
   #***************************************************

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub02 Cat01:
   #   - min read depth
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.mid.filt.depth
   pack .main.con.clust.mid.filt.depth -anchor w -side top ;

   tk::label .main.con.clust.mid.filt.depth.lab -text "min depth (> 0)" 

   tk::entry .main.con.clust.mid.filt.depth.entry -textvariable glob_minClustDepth -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ; # min read depth for clustering

   pack .main.con.clust.mid.filt.depth.lab .main.con.clust.mid.filt.depth.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub02 Cat02:
   #   - min percent read depth
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.mid.filt.percdepth
   pack .main.con.clust.mid.filt.percdepth -anchor w -side top ;

   tk::label .main.con.clust.mid.filt.percdepth.lab -text "min % depth (0-1)" 

   tk::entry .main.con.clust.mid.filt.percdepth.entry -textvariable glob_minClustPercDepth -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # min percent read depth for clustering

   pack .main.con.clust.mid.filt.percdepth.lab .main.con.clust.mid.filt.percdepth.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub02 Cat03:
   #   - min snp q-score
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.mid.filt.snpq
   pack .main.con.clust.mid.filt.snpq -anchor w -side top ;

   tk::label .main.con.clust.mid.filt.snpq.lab -text "min snp Q (0-93)" 

   tk::entry .main.con.clust.mid.filt.snpq.entry -textvariable glob_minClustSnpQ -validate key -vcmd {tcl_isInt_gui %P %i 0 93 }  ; # min snp q-score

   pack .main.con.clust.mid.filt.snpq.lab .main.con.clust.mid.filt.snpq.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub02 Cat04:
   #   - min indel len
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.mid.filt.indellen
   pack .main.con.clust.mid.filt.indellen -anchor w -side top ;

   tk::label .main.con.clust.mid.filt.indellen.lab -text "min indel length (> 0)" 

   tk::entry .main.con.clust.mid.filt.indellen.entry -textvariable glob_minClustIndelLen -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ; # min snp q-score

   pack .main.con.clust.mid.filt.indellen.lab .main.con.clust.mid.filt.indellen.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub02 Cat05:
   #   - depth profiling
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::checkbutton .main.con.clust.mid.filt.depthprofile -text "Depth profile" -variable glob_depthProfBl  ; # min snp q-score

   pack .main.con.clust.mid.filt.depthprofile -anchor w -side left 
   #***************************************************
   # Gui07 Sec04 Sub03:
   #   - set up cluster calcuations
   #   o gui07 sec04 sub03 cat01:
   #     - expected read error rate
   #   o gui07 sec04 sub03 cat02:
   #     - expected consensus error rate
   #   o gui07 sec04 sub03 cat03:
   #     - variant to error ratio
   #   o gui07 sec04 sub03 cat04:
   #     - window variant to error ratio
   #   o gui07 sec04 sub03 cat05:
   #     - window length
   #***************************************************

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub03 Cat01:
   #   - expected read error rate
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.mid.calc.readerr
   pack .main.con.clust.mid.calc.readerr -anchor w -side top ;

   tk::label .main.con.clust.mid.calc.readerr.lab -text "read map read error rate (0-1)" 

   tk::entry .main.con.clust.mid.calc.readerr.entry -textvariable glob_clustReadErr -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # expected read error rate

   pack .main.con.clust.mid.calc.readerr.lab .main.con.clust.mid.calc.readerr.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub03 Cat02:
   #   - expected consensus error rate
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.mid.calc.conerr
   pack .main.con.clust.mid.calc.conerr -anchor w -side top ;

   tk::label .main.con.clust.mid.calc.conerr.lab -text "consensus map read error rate (0-1)" 

   tk::entry .main.con.clust.mid.calc.conerr.entry -textvariable glob_clustConErr -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # expected consensus error rate

   pack .main.con.clust.mid.calc.conerr.lab .main.con.clust.mid.calc.conerr.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub03 Cat03:
   #   - variant to error ratio
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.mid.calc.errratio
   pack .main.con.clust.mid.calc.errratio -anchor w -side top ;

   tk::label .main.con.clust.mid.calc.errratio.lab -text "var:err 100*(variant/error) (> 0)" 

   tk::entry .main.con.clust.mid.calc.errratio.entry -textvariable glob_clustErrRatio -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ;

   pack .main.con.clust.mid.calc.errratio.lab .main.con.clust.mid.calc.errratio.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub03 Cat04:
   #   - window variant to error ratio
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.mid.calc.winerr
   pack .main.con.clust.mid.calc.winerr -anchor w -side top ;

   tk::label .main.con.clust.mid.calc.winerr.lab -text "window var:err (> 0)" 

   tk::entry .main.con.clust.mid.calc.winerr.entry -textvariable glob_clustWinErrRatio -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ;

   pack .main.con.clust.mid.calc.winerr.lab .main.con.clust.mid.calc.winerr.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub03 Cat05:
   #   - window length
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.mid.calc.winlen
   pack .main.con.clust.mid.calc.winlen -anchor w -side top ;

   tk::label .main.con.clust.mid.calc.winlen.lab -text "window length (> 0)" 

   tk::entry .main.con.clust.mid.calc.winlen.entry -textvariable glob_clustWinLen -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ;

   pack .main.con.clust.mid.calc.winlen.lab .main.con.clust.mid.calc.winlen.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub03 Cat06:
   #   - length weight
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.mid.calc.lenweight
   pack .main.con.clust.mid.calc.lenweight -anchor w -side top ;

   tk::label .main.con.clust.mid.calc.lenweight.lab -text "score: median Q + log2(len)*input (> 0)" 

   tk::entry .main.con.clust.mid.calc.lenweight.entry -textvariable glob_lenWeight -validate key -vcmd {tcl_isFloat_gui %P %i 1 2000000000 }  ;

   pack .main.con.clust.mid.calc.lenweight.lab .main.con.clust.mid.calc.lenweight.entry -anchor w -side left 

   #***************************************************
   # Gui07 Sec04 Sub04:
   #   - set up cluster consnesus filtering
   #   o gui07 sec04 sub04 cat01:
   #     - max percent simularity
   #   o gui07 sec04 sub04 cat02:
   #     - min percent overlap
   #   o gui07 sec04 sub04 cat03:
   #     - maximum percent masking
   #   o gui07 sec04 sub04 cat04:
   #     - min percent simularity
   #***************************************************

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub04 Cat01:
   #   - max percent simularity
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.con.maxsim
   pack .main.con.clust.con.maxsim -anchor w -side top ;

   tk::label .main.con.clust.con.maxsim.lab -text "max % similarity (0-1)" 

   tk::entry .main.con.clust.con.maxsim.entry -textvariable glob_maxClustSim -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # max percent read depth for clustering

   pack .main.con.clust.con.maxsim.lab .main.con.clust.con.maxsim.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub04 Cat02:
   #   - minimu percent overlap
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.con.minoverlap
   pack .main.con.clust.con.minoverlap -anchor w -side top ;

   tk::label .main.con.clust.con.minoverlap.lab -text "min % consensus overlap (0-1)" 

   tk::entry .main.con.clust.con.minoverlap.entry -textvariable glob_minClustOverlap -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # max percent read depth for clustering

   pack .main.con.clust.con.minoverlap.lab .main.con.clust.con.minoverlap.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub04 Cat03:
   #   - maximum percent masking
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.con.maxmask
   pack .main.con.clust.con.maxmask -anchor w -side top ;

   tk::label .main.con.clust.con.maxmask.lab -text "max % masking (0-1)" 

   tk::entry .main.con.clust.con.maxmask.entry -textvariable glob_maxClustMask -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; # max percent read depth for clustering

   pack .main.con.clust.con.maxmask.lab .main.con.clust.con.maxmask.entry -anchor w -side left 

   #+++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui07 Sec04 Sub04 Cat04:
   #   - number rebuilds
   #+++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.con.clust.con.numbuild
   pack .main.con.clust.con.numbuild -anchor w -side top ;

   tk::label .main.con.clust.con.numbuild.lab -text "times to rebuild consensus (0-100)" 

   tk::entry .main.con.clust.con.numbuild.entry -textvariable glob_numRebuilds -validate key -vcmd {tcl_isInt_gui %P %i 0 100 }  ; # min percent read depth for clustering

   pack .main.con.clust.con.numbuild.lab .main.con.clust.con.numbuild.entry -anchor w -side left 

   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Gui08 TOC:
   #   - set up output settings/input frame
   #   o gui08 sec01:
   #     - set up output gui frame
   #   o gui08 sec02:
   #     - function setup; report file
   #   o gui08 sec03:
   #     - set up settings output gui frame
   #   o gui08 sec04:
   #     - set up output gui menu
   #   o gui08 sec05:
   #     - set up report gui
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui08 Sec01:
   #   - set up output gui frame
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.out ;

   tk::frame .main.out.set ;
   pack .main.out.set -anchor w -side top ;

   tk::frame .main.out.report ;
   tk::frame .main.out.depth ;
   tk::frame .main.out.cover ;
   tk::frame .main.out.amr ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui08 Sec02:
   #   - set up report file parsing functions
   #   o gui08 sec02 sub01:
   #     - functioun consensus AMR report
   #   o gui08 sec02 sub02:
   #     - function read AMR report
   #   o gui08 sec02 sub03:
   #     - function; consensus spoligotype
   #   o gui08 sec02 sub04:
   #     - function; read spoligotype
   #   o gui08 sec02 sub05:
   #     - function consensus MIRU-VNTR
   #   o gui08 sec02 sub06:
   #     - function; read depth graph
   #   o gui08 sec02 sub07:
   #     - function; coverage graph
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #**************************************************
   # Gui08 Sec02 Sub01:
   #   - function; consensus AMR report
   #**************************************************

   proc conAmrRep {prefixStr} {
      set fileStr $prefixStr ;
      append fileStr "-con-amrs.tsv" ;

      set openFILE [open $fileStr] ;
      gets $openFILE line ; # get header line
      set amrList "" ;

      while {[gets $openFILE line] > -1} {
         set line [split $line "\t"] ;
         set tmpStr [lindex $line 2] ;
         set amrList [concat $amrList $tmpStr] ;

         if { [lindex $line 3] ne "NA" } {
            set tmpStr [split [lindex $line 3] "_"] ;
            set amrList [concat $amrList $tmpStr] ;
         } ; # If: have cross resistance

         # remove duplicates
         set amrList [lsort -unique $amrList] ;
      } ; # Loop: read file by line

      # make sure always lower case
      close $openFILE ;
      set amrList [string tolower $amrList] ;

      for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr }  {
         set tmpStr [lindex $::glob_amrList $siAmr] ;
         set tmpStr [string tolower $tmpStr] ;
 
         set labStr [lindex $::glob_amrShort $siAmr] ;
         set labStr [string tolower $labStr] ;
         append labStr "lab" ;

         if { [lsearch $amrList $tmpStr] ne -1 } {
            .main.out.report.conAmr.$labStr configure -background $::glob_amrCol ;
         } else {
            .main.out.report.conAmr.$labStr configure -background $::glob_noAmrCol ;
         } ; # check if have AMR
      } ; # Loop: build amr labels
   } ; # conAmrRep

   #**************************************************
   # Gui08 Sec02 Sub02:
   #   - function read AMR report
   #**************************************************

   proc readAmrRep {prefixStr} {
      set fileStr $prefixStr ;
      append fileStr "-read-amrs.tsv" ;

      set openFILE [open $fileStr] ;
      gets $openFILE line ; # get header line
      set amrList "" ;

      while {[gets $openFILE line] > -1} {
         set line [split $line "\t"] ;
         set tmpStr [lindex $line 4] ;

         # get percent report and reduce to decimal
         set tmpF [lindex $line 8] ;
         set tmpF [expr $tmpF / 100] ;

         if {$tmpStr eq "snp"} {
            if {$tmpF < $::glob_outSnpSup} {
                continue ;
            } ; # If: support is to low
         } else {
            if {$tmpF < $::glob_outIndelSup} {
                continue ;
            } ; # If: support is to low
         } ; # check if snp or indel

         set tmpStr [lindex $line 1] ;
         set amrList [concat $amrList $tmpStr] ;

         if { [lindex $line 2] ne "NA" } {
            set tmpStr [split [lindex $line 2] "_"] ;
            set amrList [concat $amrList $tmpStr] ;
         } ; # If: have cross resistance

         # remove duplicates
         set amrList [lsort -unique $amrList] ;
      } ; # Loop: read file by line

      # make sure always lower case
      close $openFILE ;
      set amrList [string tolower $amrList] ;

      for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr }  {
         set tmpStr [lindex $::glob_amrList $siAmr] ;
         set tmpStr [string tolower $tmpStr] ;
 
         set labStr [lindex $::glob_amrShort $siAmr] ;
         set labStr [string tolower $labStr] ;
         append labStr "lab" ;

         if { [lsearch $amrList $tmpStr] ne -1 } {
            .main.out.report.readAmr.$labStr configure -background $::glob_amrCol ;
         } else {
            .main.out.report.readAmr.$labStr configure -background $::glob_noAmrCol ;
         } ; # check if have AMR
      } ; # Loop: build amr labels
   } ; # readAmrRep

   #**************************************************
   # Gui08 Sec02 Sub03:
   #   - function; consensus spoligotype
   #**************************************************

   proc conSpol {prefixStr} {
      set fileStr $prefixStr ;
      append fileStr "-con-spoligo.tsv" ;
      set openFILE [open $fileStr] ;

      gets $openFILE lineStr ; # get header line
      set statusBl [gets $openFILE lineStr ] ;
      close $openFILE ;

      if {$statusBl < 0} {
         .main.out.report.conspol.octal.reslab configure -text "NA" ;

         .main.out.report.conspol.sit.reslab configure -text "NA" ;

         .main.out.report.conspol.strain.reslab configure -text "NA" ;

         .main.out.report.conspol.orig.reslab configure -text "NA" ;

         return false ;
      } ; # If: no spoligotype entry

      set lineStr [split $lineStr "\t"] ;

      # each value is either NA or has actual value

      .main.out.report.conspol.octal.reslab configure -text [lindex $lineStr 3] ;

      .main.out.report.conspol.sit.reslab configure -text [lindex $lineStr 5] ;

      .main.out.report.conspol.strain.reslab configure -text [lindex $lineStr 1] ;

      .main.out.report.conspol.orig.reslab configure -text [lindex $lineStr 6] ;
   } ; # conSpol

   #**************************************************
   # Gui08 Sec02 Sub04:
   #   - function; read spoligotype
   #**************************************************

   proc readSpol {prefixStr} {
      set fileStr $prefixStr ;
      append fileStr "-read-spoligo.tsv" ;
      set openFILE [open $fileStr] ;

      gets $openFILE lineStr ; # get header line
      set statusBl [gets $openFILE lineStr ] ;
      close $openFILE ;

      if {$statusBl < 0} {
         .main.out.report.readspol.octal.reslab configure -text "NA" ;

         .main.out.report.readspol.sit.reslab configure -text "NA" ;

         .main.out.report.readspol.strain.reslab configure -text "NA" ;

         .main.out.report.readspol.orig.reslab configure -text "NA" ;

         return false ;
      } ; # If: no spoligotype entry

      set lineStr [split $lineStr "\t"] ;

      # each value is either NA or has actual value

      .main.out.report.readspol.octal.reslab configure -text [lindex $lineStr 3] ;

      .main.out.report.readspol.sit.reslab configure -text [lindex $lineStr 5] ;

      .main.out.report.readspol.strain.reslab configure -text [lindex $lineStr 1] ;

      .main.out.report.readspol.orig.reslab configure -text [lindex $lineStr 6] ;
   } ; # readSpol

   #**************************************************
   # Gui08 Sec02 Sub05:
   #   - function consensus MIRU-VNTR
   #**************************************************

   proc conMiru {prefixStr} {
      set fileStr $prefixStr ;
      append fileStr "-con-miru.tsv" ;
      set openFILE [open $fileStr] ;
      
      gets $openFILE lineStr ; # get header
      set status [gets $openFILE lineStr] ;
      close $openFILE ;

      if {$status < 0} {
         .main.out.report.miru.reslab configure -text "NA" ;
         return false ;
      } ; # If: no entry

      set lineStr [split $lineStr "\t"] ;

      # need to remove lineage index
      set lineStr [lreplace $lineStr 0 0];

      set tmpStr [lsort -unique $lineStr ] ;

      if {[llength $tmpStr] < 2} {
         if {[lindex $tmpStr 0] eq "NA"} {
            .main.out.report.miru.reslab configure -text "NA" ;
            return false ;
         } ; # If: no lineage detected
      } ; # If: have many duplicates

      .main.out.report.miru.reslab configure -text $lineStr ;

      return true ;
   } ; # conMiru

   #**************************************************
   # Gui08 Sec02 Sub06:
   #   - function; read depth graph display
   #**************************************************

   proc depthGraph {prefixStr} {
      # delete old graphs
      if {! [image inuse $::glob_depthImg] } {
            image delete $::glob_depthImg ;
      } ; # If: image exists

      if {$::glob_mkGraphBl ne 0 } {
         
         set tmpStr $prefixStr ;
         append tmpStr "-depths.tsv" ;

         set tmpPathStr $prefixStr ;
         append tmpPathStr "-readDepth.png" ;

         if { [file exists $tmpPathStr] eq 1 } {
            set ::glob_depthImg [image create photo -file $tmpPathStr] ;

            .main.out.depth.graph configure -image $::glob_depthImg ;

         } else {
            set status [catch {exec $::rPath $::graphScript -stats $tmpStr -who $::glob_amrDb -prefix $prefixStr } ] ; # run R to build graphs

            if { $status ne 0 } {
               tk_messageBox -message "failed to build depth graph" -title "ERROR" ;

               # turn off graphing
               .main.out.set.graph.check toggle ;
            } else {
               set ::glob_depthImg [image create photo -file $tmpPathStr ] ;

               .main.out.depth.graph configure -image $::glob_depthImg ;
            } ; # Else: add image
         } ; # check if need to build graphs
       } ; # check if users wants graphs displayed
   } ; # depthGraph

   #**************************************************
   # Gui08 Sec02 Sub07:
   #   - function; coverage graph dispaly
   #**************************************************

   proc coverageGraph {prefixStr} {
      # delete old graphs
      if {! [image inuse $::glob_coverImg] } {
            image delete $::glob_coverImg ;
      } ; # If: image exists

      if {$::glob_mkGraphBl ne 0 } {
         set tmpStr $prefixStr ;
         append tmpStr "-covers.tsv" ;

         set tmpPathStr $prefixStr ;
         append tmpPathStr "-coverage.png" ;

         if { [file exists $tmpPathStr] eq 1 } {
            set ::glob_coverImg [image create photo -file $tmpPathStr] ;

            .main.out.cover.graph configure -image $::glob_coverImg ;

         } else {
            set status [catch {exec $::rPath $::graphScript -stats $tmpStr -who $::glob_amrDb -prefix $prefixStr } ] ; # run R to build graphs

            if { $status ne 0 } {
               tk_messageBox -message "failed to build cover graph" -title "ERROR" ;

               # turn off graphing
               .main.out.set.graph.check toggle ;
            } else {
               set ::glob_coverImg [image create photo -file $tmpPathStr ] ;

               .main.out.cover.graph configure -image $::glob_coverImg ;
            } ; # Else: add image
         } ; # check if need to build graphs
       } ; # check if users wants graphs displayed
   } ; # coverageGraph

   #**************************************************
   # Gui08 Sec02 Sub08:
   #   - function; consnesus AMR table
   #   o gui08 sec02 sub08 cat01:
   #     - open file and get header lenghts
   #   o gui08 sec02 sub08 cat02:
   #     - get gene name length + start length loop
   #   o gui08 sec02 sub08 cat03:
   #     - get AMR (short name) length
   #   o gui08 sec02 sub08 cat04:
   #     - get AMR cross resistance column length
   #   o gui08 sec02 sub08 cat05:
   #     - find length of mutation column
   #   o gui08 sec02 sub08 cat06:
   #     - print no AMR case
   #   o gui08 sec02 sub08 cat07:
   #     - print header
   #   o gui08 sec02 sub08 cat08:
   #     - get gene ids + start print loop
   #   o gui08 sec02 sub08 cat09:
   #     - get primrary resistant drug (3 letter)
   #   o gui08 sec02 sub08 cat10:
   #     - get cross resistant drugs (3 letter)
   #   o gui08 sec02 sub08 cat11:
   #     - get mutation column
   #   o gui08 sec02 sub08 cat12:
   #     - add text to consensus AMR table + return
   #**************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec02 Sub08 Cat01:
   #   - open file and get header lenghts
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   proc conAmrTbl {prefixStr indentStr} {
      set fileStr $prefixStr ;
      append fileStr "-con-amrs.tsv" ;
      set openFILE [open $fileStr] ;

      set pad1 [string length "gene"] ;
      set pad2 [string length "drug"] ;
      set pad3 [string length "cross-res"] ;
      set pad5 [string length "mutant"];

      if {[gets $openFILE lineStr] < 0} {
         .main.out.amr.read.tbl.lab configure -text "NA" ;

         return false ;
      } ; # If; nothing in file

      #+++++++++++++++++++++++++++++++++++++++++++++++
      # Gui08 Sec02 Sub08 Cat02:
      #   - get gene name length + start length loop
      #+++++++++++++++++++++++++++++++++++++++++++++++

      while {[gets $openFILE lineStr] > -1} {
         set lineStr [split $lineStr "\t"] ;

         # gene name length
         set tmpStr [lindex $lineStr 1] ;
         set lenUI [string length $tmpStr] ;
         if {$pad1 < $lenUI} { set pad1 $lenUI ; } ;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub08 Cat03:
         #   - get AMR (short name) length
         #++++++++++++++++++++++++++++++++++++++++++++

         set tmpStr [lindex $lineStr 2] ;
         set tmpStr [string tolower $tmpStr] ;

         # loop converts drug name to short hand
         for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
            set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrShort $siAmr] ] ;
         } ; # Loop: convert AMR ids to short hand

         set lenUI [string length $tmpStr] ;
         if {$pad2 < $lenUI} { set pad2 $lenUI ; } ;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub08 Cat04:
         #   - get AMR cross resistance column length
         #++++++++++++++++++++++++++++++++++++++++++++

         # cross resistance length
         set tmpStr [lindex $lineStr 3] ;
         set tmpStr [string tolower $tmpStr] ;

         # loop converts drug names to three letters
         for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
            set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrShort $siAmr] ] ;
         } ; # Loop: convert AMR ids to short hand

         set lenUI [string length $tmpStr] ;
         if {$pad3 < $lenUI} { set pad3 $lenUI ; } ;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub08 Cat05:
         #   - find length of mutation column
         #++++++++++++++++++++++++++++++++++++++++++++

         set tmpStr [lindex $lineStr 5] ;
         set lenUI [string length $tmpStr] ;
         if {$pad5 < $lenUI} { set pad5 $lenUI ; } ;
      } ; # Loop: find longest entries per column

      close $openFILE ;

      #+++++++++++++++++++++++++++++++++++++++++++++++
      # Gui08 Sec02 Sub08 Cat06:
      #   - print no AMR case
      #+++++++++++++++++++++++++++++++++++++++++++++++

      if {$pad1 eq 0} {
         .main.out.amr.con.tbl.lab configure -text "NA" ;

         return false ;
      } ; # If; nothing in file

      #+++++++++++++++++++++++++++++++++++++++++++++++
      # Gui08 Sec02 Sub08 Cat07:
      #   - print header
      #+++++++++++++++++++++++++++++++++++++++++++++++

      set tmpStr [format "%-*s" $pad1 "gene"] ;
      append tblStr $tmpStr $indentStr;

      set tmpStr [format "%-*s" $pad2 "drug"] ;
      append tblStr $tmpStr $indentStr;

      set tmpStr [format "%-*s" $pad3 "cross-res"] ;
      append tblStr $tmpStr $indentStr;

      set tmpStr [format "%-*s" $pad5 "mutant"] ;
      append tblStr $tmpStr $indentStr;

      append tblStr "\n";

      set openFILE [open $fileStr] ;
      gets $openFILE lineStr ;

      #+++++++++++++++++++++++++++++++++++++++++++++++
      # Gui08 Sec02 Sub08 Cat08:
      #   - get gene ids + start print loop
      #+++++++++++++++++++++++++++++++++++++++++++++++

      while {[gets $openFILE lineStr] > -1} {
         set lineStr [split $lineStr "\t"] ;

         set tmpStr [lindex $lineStr 1] ; # gene id
         set tmpStr [format "%-*s" $pad1 $tmpStr] ;
         append tblStr $tmpStr $indentStr;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub08 Cat09:
         #   - get primrary resistant drug (3 letter)
         #++++++++++++++++++++++++++++++++++++++++++++

         set tmpStr [lindex $lineStr 2] ;
         set tmpStr [string tolower $tmpStr] ;

         # loop converts to three letter code
         for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
            set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrShort $siAmr] ] ;
         } ; # Loop: convert AMR ids to short hand
         set tmpStr [format "%-*s" $pad2 $tmpStr] ;
         append tblStr $tmpStr $indentStr;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub08 Cat10:
         #   - get cross resistant drugs (3 letter)
         #++++++++++++++++++++++++++++++++++++++++++++

         # cross resistance AMRs
         set tmpStr [lindex $lineStr 3] ;
         set tmpStr [string tolower $tmpStr] ;

         # loop converts drug to three letter codes
         for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
            set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrShort $siAmr] ] ;
         } ; # Loop: convert AMR ids to short hand

         set tmpStr [format "%-*s" $pad3 $tmpStr] ;
         append tblStr $tmpStr $indentStr;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub08 Cat11:
         #   - get mutation column
         #++++++++++++++++++++++++++++++++++++++++++++

         set tmpStr [lindex $lineStr 5] ; # mut type
         set tmpStr [format "%-*s" $pad5 $tmpStr] ;
         append tblStr $tmpStr $indentStr;

         append tblStr "\n" ; # next row
      } ; # Loop: get AMRs

      #+++++++++++++++++++++++++++++++++++++++++++++++
      # Gui08 Sec02 Sub08 Cat12:
      #   - add text to consensus AMR table + return
      #+++++++++++++++++++++++++++++++++++++++++++++++

      .main.out.amr.con.tbl.lab configure -text $tblStr ;

      close $openFILE ;
      return true ;
   } ; # conAmrTbl

   #**************************************************
   # Gui08 Sec02 Sub09:
   #   - function; read AMR table
   #   o gui08 sec02 sub09 cat01:
   #     - open file and get header lengths
   #   o gui08 sec02 sub09 cat02:
   #     - find gene id length + start length loop
   #   o gui08 sec02 sub09 cat03:
   #     - find primrary drug length
   #   o gui08 sec02 sub09 cat04:
   #     - find cross resitance drugs length
   #   o gui08 sec02 sub09 cat05:
   #     - find mutation and % support lengths
   #   o gui08 sec02 sub09 cat06:
   #     - print NA for nothing in file case
   #   o gui08 sec02 sub09 cat07:
   #     - build header for table
   #   o gui08 sec02 sub09 cat08:
   #     - add gene id to header + start table loop
   #   o gui08 sec02 sub09 cat09:
   #     - add primrary drug to header
   #   o gui08 sec02 sub09 cat10:
   #     - add cross-resistance drugs to header
   #   o gui08 sec02 sub09 cat11:
   #     - add mutation and % support to table
   #   o gui08 sec02 sub09 cat12:
   #     - put table into AMR read table label
   #**************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec02 Sub09 Cat01:
   #   - open file and get header lengths
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   proc readAmrTbl {prefixStr indentStr} {
      set fileStr $prefixStr ;
      append fileStr "-read-amrs.tsv" ;
      set openFILE [open $fileStr] ;

      set pad0 [string length "gene"] ;
      set pad1 [string length "drug"] ;
      set pad2 [string length "cross-res"] ;
      set pad4 [string length "mutant"];
      set pad8 [string length "%sup"];

      if {[gets $openFILE lineStr] < 0} {
         .main.out.amr.read.tbl.lab configure -text "NA" ;

         return false ;
      } ; # If; nothing in file

      #+++++++++++++++++++++++++++++++++++++++++++++++
      # Gui08 Sec02 Sub09 Cat02:
      #   - find gene id length + start length loop
      #+++++++++++++++++++++++++++++++++++++++++++++++

      while {[gets $openFILE lineStr] > -1} {
         set lineStr [split $lineStr "\t"] ;

         # gene name length
         set tmpStr [lindex $lineStr 0] ;
         set lenUI [string length $tmpStr] ;
         if {$pad0 < $lenUI} { set pad0 $lenUI ; } ;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub09 Cat03:
         #   - find primrary drug length
         #++++++++++++++++++++++++++++++++++++++++++++

         set tmpStr [lindex $lineStr 1] ;
         set tmpStr [string tolower $tmpStr] ;

         # reducing to three letter code if possible
         for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
            set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrShort $siAmr] ] ;
         } ; # Loop: convert AMR ids to short hand

         set lenUI [string length $tmpStr] ;
         if {$pad1 < $lenUI} { set pad1 $lenUI ; } ;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub09 Cat04:
         #   - find cross resitance drugs length
         #++++++++++++++++++++++++++++++++++++++++++++

         # cross resistance length
         set tmpStr [lindex $lineStr 2] ;
         set tmpStr [string tolower $tmpStr] ;

         # reducing to three letter code if possible
         for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
            set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrShort $siAmr] ] ;
         } ; # Loop: convert AMR ids to short hand

         set lenUI [string length $tmpStr] ;
         if {$pad2 < $lenUI} { set pad2 $lenUI ; } ;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub09 Cat05:
         #   - find mutation and % support lengths
         #++++++++++++++++++++++++++++++++++++++++++++

         # mutation length
         set tmpStr [lindex $lineStr 4] ;
         set lenUI [string length $tmpStr] ;
         if {$pad4 < $lenUI} { set pad4 $lenUI ; } ;

         # percent support length
         set tmpStr [lindex $lineStr 8] ;
         set lenUI [string length $tmpStr] ;
         if {$pad8 < $lenUI} { set pad8 $lenUI ; } ;
      } ; # Loop: find longest entries per column

      close $openFILE ;

      #+++++++++++++++++++++++++++++++++++++++++++++++
      # Gui08 Sec02 Sub09 Cat06:
      #   - print NA for nothing in file case
      #+++++++++++++++++++++++++++++++++++++++++++++++

      if {$pad0 eq 0} {
         .main.out.amr.read.tbl.lab configure -text "NA" ;

         return false ;
      } ; # If; nothing in file


      #+++++++++++++++++++++++++++++++++++++++++++++++
      # Gui08 Sec02 Sub09 Cat07:
      #   - build header for table
      #+++++++++++++++++++++++++++++++++++++++++++++++

      set tmpStr [format "%-*s" $pad0 "gene"] ;
      append tblStr $tmpStr $indentStr;

      set tmpStr [format "%-*s" $pad1 "drug"] ;
      append tblStr $tmpStr $indentStr;

      set tmpStr [format "%-*s" $pad2 "cross-res"] ;
      append tblStr $tmpStr $indentStr;

      set tmpStr [format "%-*s" $pad4 "mutant"] ;
      append tblStr $tmpStr $indentStr;

      set tmpStr [format "%-*s" $pad8 "%sup"] ;
      append tblStr $tmpStr $indentStr;

      append tblStr "\n";

      #+++++++++++++++++++++++++++++++++++++++++++++++
      # Gui08 Sec02 Sub09 Cat08:
      #   - add gene id to table + start table loop
      #+++++++++++++++++++++++++++++++++++++++++++++++

      set openFILE [open $fileStr] ;
      gets $openFILE lineStr ;

      while {[gets $openFILE lineStr] > -1} {
         set lineStr [split $lineStr "\t"] ;

         set tmpStr [lindex $lineStr 0] ; # gene id
         set tmpStr [format "%-*s" $pad0 $tmpStr] ;
         append tblStr $tmpStr $indentStr;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub09 Cat09:
         #   - add primrary drug to table
         #++++++++++++++++++++++++++++++++++++++++++++

         set tmpStr [lindex $lineStr 1] ;
         set tmpStr [string tolower $tmpStr] ;

         # try to reduce drug to three letter code
         for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
            set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrShort $siAmr] ] ;
         } ; # Loop: convert AMR ids to short hand
         set tmpStr [format "%-*s" $pad1 $tmpStr] ;
         append tblStr $tmpStr $indentStr;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub09 Cat10:
         #   - add cross-resistance drugs to table
         #++++++++++++++++++++++++++++++++++++++++++++

         set tmpStr [lindex $lineStr 2] ;
         set tmpStr [string tolower $tmpStr] ;

         # try to reduce drug to three letter code
         for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
            set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrShort $siAmr] ] ;
         } ; # Loop: convert AMR ids to short hand

         set tmpStr [format "%-*s" $pad2 $tmpStr] ;
         append tblStr $tmpStr $indentStr;

         #++++++++++++++++++++++++++++++++++++++++++++
         # Gui08 Sec02 Sub09 Cat11:
         #   - add mutation and % support to table
         #++++++++++++++++++++++++++++++++++++++++++++

         # mutation type
         set tmpStr [lindex $lineStr 4] ;
         set tmpStr [format "%-*s" $pad4 $tmpStr] ;
         append tblStr $tmpStr $indentStr;

         # percent of reads supporting
         set tmpStr [lindex $lineStr 8] ;
         set tmpStr [format "%-*s" $pad8 $tmpStr] ;
         append tblStr $tmpStr $indentStr;

         append tblStr "\n" ; # next row
      } ; # Loop: get AMRs

      #+++++++++++++++++++++++++++++++++++++++++++++++
      # Gui08 Sec02 Sub09 Cat12:
      #   - put table into AMR read table label
      #+++++++++++++++++++++++++++++++++++++++++++++++

      .main.out.amr.read.tbl.lab configure -text $tblStr ;

      close $openFILE ;
      return true ;
   } ; # readAmrTbl


   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui08 Sec03:
   #   - set up settings output gui frame
   #   o gui08 sec03 sub01:
   #     - set up file prefix finder
   #   o gui08 sec03 sub02:
   #     - AMR snp percent entry
   #   o gui08 sec03 sub03:
   #     - AMR indel percent entry
   #   o gui08 sec03 sub04:
   #     - graphing check button
   #   o gui08 sec03 sub05:
   #     - get report button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #**************************************************
   # Gui08 Sec03 Sub01:
   #   - set up file prefix finder
   #**************************************************

   tk::frame .main.out.set.prefix ;
   pack .main.out.set.prefix -anchor w -side top ;
   tk::label .main.out.set.prefix.lab -text "" ;

   tk::button .main.out.set.prefix.but -text "get prefix" -command { 
         set fileStr [
            tk_getOpenFile -title "select freezeTB output" 
         ] ;

         if { $fileStr eq "" } {

         } else {
            # get path and file name
            set dirStr [file dirname $fileStr] ;
            set prefStr [file tail $fileStr] ;

            # remove all possible extension types

            set prefStr [regsub con.* $prefStr ""] ;
            set prefStr [regsub map.* $prefStr ""] ;
            set prefStr [regsub read.* $prefStr ""] ;
            set prefStr [regsub tsv.* $prefStr ""] ;
            set prefStr [regsub id.* $prefStr ""] ;
            set prefStr [regsub dept.* $prefStr ""] ;
            set prefStr [regsub grap.* $prefStr ""] ;
            set prefStr [regsub coverage.* $prefStr ""] ;

            set prefStr [string range $prefStr 0 end-1] ; # remove dash regsub could not remove


            # save prefix and update label

            set ::glob_outPref [file join $dirStr $prefStr] ;

           .main.out.set.prefix.lab configure -text $::glob_outPref ;
         } ; # check if anything was input
      } ; # get file prefix

   pack .main.out.set.prefix.but .main.out.set.prefix.lab -anchor w -side left ;

   #**************************************************
   # Gui08 Sec03 Sub02:
   #   - AMR snp percent entry
   #**************************************************

   tk::frame .main.out.set.snp ;
   pack .main.out.set.snp -anchor w -side top ;

   tk::label .main.out.set.snp.lab -text "min AMR SNP % support" ;

   tk::entry .main.out.set.snp.entry -textvariable glob_outSnpSup -vcmd { tcl_isFloat_gui %P %i 0 1 } ;

   pack .main.out.set.snp.lab .main.out.set.snp.entry -anchor w -side left ;

   #**************************************************
   # Gui08 Sec03 Sub03:
   #   - AMR indel percent entry
   #**************************************************

   tk::frame .main.out.set.indel ;
   pack .main.out.set.indel -anchor w -side top ;

   tk::label .main.out.set.indel.lab -text "min AMR Indel % support" ;

   tk::entry .main.out.set.indel.entry -textvariable glob_outIndelSup -vcmd { tcl_isFloat_gui %P %i 0 1 } ;

   pack .main.out.set.indel.lab .main.out.set.indel.entry -anchor w -side left ;

   #**************************************************
   # Gui08 Sec03 Sub04:
   #   - graphing check button
   #**************************************************

   tk::frame .main.out.set.graph ;
   pack .main.out.set.graph -anchor w -side top ;

   tk::checkbutton .main.out.set.graph.check -text "build graphs" -variable glob_mkGraphBl ;

   pack .main.out.set.graph.check -anchor w -side left ;

   #**************************************************
   # Gui08 Sec03 Sub05:
   #   - get report button
   #**************************************************

   tk::frame .main.out.set.run ;
   pack .main.out.set.run -anchor w -side top ;

   tk::button .main.out.set.run.but -text "get report" -command { 
         conAmrRep $::glob_outPref ;
         readAmrRep $::glob_outPref ;
         depthGraph $::glob_outPref ;
         coverageGraph $::glob_outPref ;
         conSpol $::glob_outPref ;
         readSpol $::glob_outPref ;
         conMiru $::glob_outPref ;
         conAmrTbl $::glob_outPref "   " ;
         readAmrTbl $::glob_outPref "   " ;

         .main.out.menu.reportBut invoke ;
         .main.out.set.prefix.lab configure -text "" ;
         set ::glob_outCur "" ;
      } ; # build the report

   pack .main.out.set.run.but -anchor w -side left ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui08 Sec04:
   #   - set up output gui menu
   #   o gui08 sec04 sub01:
   #     - set up menu frame
   #   o gui08 sec04 sub02:
   #     - add settings button
   #   o gui08 sec04 sub03:
   #     - add report button
   #   o gui08 sec04 sub04:
   #     - add mean read depth graph button
   #   o gui08 sec04 sub05:
   #     - add coverage graph button
   #   o gui08 sec04 sub06:
   #     - add AMR table button
   #   o gui08 sec04 sub07:
   #     - add input button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #**************************************************
   # Gui08 Sec04 Sub01:
   #   - set up menu frame
   #**************************************************

   tk::frame .main.out.menu ;
   pack .main.out.menu -anchor w -side top ;

   #**************************************************
   # Gui08 Sec04 Sub02:
   #   - add settings button
   #**************************************************

   tk::button .main.out.menu.setBut -text "out" -command { 
         pack forget .main.out.set ;
         pack forget .main.out.report ;
         pack forget .main.out.depth ;
         pack forget .main.out.cover ;
         pack forget .main.out.amr ;
         pack forget .main.out.menu ;

         pack .main.out.set ;
         pack .main.out.menu ;
         wm title . "output select freezeTB" ;

         .main.out.menu.setBut configure -relief sunken -state disabled ;

         .main.out.menu.reportBut configure -relief raised -state normal ;

         .main.out.menu.depthBut configure -relief raised -state normal ;

         .main.out.menu.coverBut configure -relief raised -state normal ;

         .main.out.menu.amrBut configure -relief raised -state normal ;
      } ; # set up output setting button

   pack .main.out.menu.setBut -anchor w -side left ;

   #**************************************************
   # Gui08 Sec04 Sub03:
   #   - add report button
   #**************************************************

   tk::button .main.out.menu.reportBut -text "report" -command { 
         pack forget .main.out.set ;
         pack forget .main.out.report ;
         pack forget .main.out.depth ;
         pack forget .main.out.cover ;
         pack forget .main.out.amr ;
         pack forget .main.out.menu ;

         pack .main.out.report ;
         pack .main.out.menu ;

         wm title . [concat [file tail $::glob_outCur] "report freezeTB" ] ;

         .main.out.menu.setBut configure -relief raised -state normal ;

         .main.out.menu.reportBut configure -relief sunken -state disabled ;

         .main.out.menu.depthBut configure -relief raised -state normal ;

         .main.out.menu.coverBut configure -relief raised -state normal ;

         .main.out.menu.amrBut configure -relief raised -state normal ;
      } ;

   pack .main.out.menu.reportBut -anchor w -side left ;

   #**************************************************
   # Gui08 Sec04 Sub04:
   #   - add mean read depth graph button
   #**************************************************

   tk::button .main.out.menu.depthBut -text "read depth" -command { 
         pack forget .main.out.set ;
         pack forget .main.out.report ;
         pack forget .main.out.depth ;
         pack forget .main.out.cover ;
         pack forget .main.out.amr ;
         pack forget .main.out.menu ;

         pack .main.out.depth ;
         pack .main.out.menu ;

         wm title . [concat [file tail $::glob_outCur] "read depth freezeTB" ] ;

         .main.out.menu.setBut configure -relief raised -state normal ;

         .main.out.menu.reportBut configure -relief raised -state normal ;

         .main.out.menu.depthBut configure -relief sunken -state disabled ;

         .main.out.menu.coverBut configure -relief raised -state normal ;

         .main.out.menu.amrBut configure -relief raised -state normal ;
      } ;

   pack .main.out.menu.depthBut -anchor w -side left ;

   #**************************************************
   # Gui08 Sec04 Sub05:
   #   - add coverage graph button
   #**************************************************

   tk::button .main.out.menu.coverBut -text "coverage" -command { 
         pack forget .main.out.set ;
         pack forget .main.out.report ;
         pack forget .main.out.depth ;
         pack forget .main.out.cover ;
         pack forget .main.out.amr ;
         pack forget .main.out.menu ;

         pack .main.out.cover ;
         pack .main.out.menu ;

         wm title . [concat [file tail $::glob_outCur] "coverage freezeTB" ] ;

         .main.out.menu.setBut configure -relief raised -state normal ;

         .main.out.menu.reportBut configure -relief raised -state normal ;

         .main.out.menu.depthBut configure -relief raised -state normal ;

         .main.out.menu.coverBut configure -relief sunken -state disabled ;

         .main.out.menu.amrBut configure -relief raised -state normal ;
      } ;

   pack .main.out.menu.coverBut -anchor w -side left ;

   #**************************************************
   # Gui08 Sec04 Sub06:
   #   - add AMR table button
   #**************************************************

   tk::button .main.out.menu.amrBut -text "AMR table" -command { 
         pack forget .main.out.set ;
         pack forget .main.out.report ;
         pack forget .main.out.depth ;
         pack forget .main.out.cover ;
         pack forget .main.out.amr ;
         pack forget .main.out.menu ;

         pack .main.out.amr ;
         pack .main.out.menu ;

         wm title . [concat [file tail $::glob_outCur] "AMR table freezeTB" ] ;

         .main.out.menu.setBut configure -relief raised -state normal ;

         .main.out.menu.reportBut configure -relief raised -state normal ;

         .main.out.menu.depthBut configure -relief raised -state normal ;

         .main.out.menu.coverBut configure -relief raised -state normal ;

         .main.out.menu.amrBut configure -relief sunken -state disabled ;
      } ;

   pack .main.out.menu.amrBut -anchor w -side left ;

   #**************************************************
   # Gui08 Sec04 Sub07:
   #   - add input button
   #**************************************************

   tk::button .main.out.menu.inBut -text "input" -command { 
         pack forget .main.out.set ;
         pack forget .main.out.report ;
         pack forget .main.out.depth ;
         pack forget .main.out.cover ;
         pack forget .main.out.amr ;
         pack forget .main.out.menu ;

         .main.menu.reqBut invoke ;

         .main.out.menu.setBut configure -relief raised -state normal ;

         .main.out.menu.reportBut configure -relief raised -state normal ;

         .main.out.menu.depthBut configure -relief raised -state normal ;

         .main.out.menu.coverBut configure -relief raised -state normal ;

         .main.out.menu.amrBut configure -relief raised -state normal ;
      } ;

   pack .main.out.menu.inBut -anchor w -side left ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui08 Sec05:
   #   - set up report gui
   #   o gui08 sec05 sub01:
   #     - build reference AMR labels
   #   o gui08 sec05 sub02:
   #     - build read AMR labels
   #   o gui08 sec05 sub03:
   #     - consensus spoligotype
   #   o gui08 sec05 sub04:
   #     - read spoligotype
   #   o gui08 sec05 sub05:
   #     - MIRU lineage
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #**************************************************
   # Gui08 Sec05 Sub01:
   #   - build reference AMR labels
   #**************************************************

   tk::frame .main.out.report.conAmr ;
   pack .main.out.report.conAmr -anchor w -side top ;

   tk::label .main.out.report.conAmr.lab -text "con  AMRs:"  ;

   pack .main.out.report.conAmr.lab -anchor w -side left ;

   foreach amr $::glob_amrShort {
      set tmpStr [string tolower $amr] ;
      append tmpStr "lab" ;

      tk::label .main.out.report.conAmr.$tmpStr -text $amr -background $::glob_noAmrCol -fg $::glob_amrTextCol ;

      pack .main.out.report.conAmr.$tmpStr -anchor w -side left ;

      # add a space between the labels
      set tmpStr [string tolower $amr] ;
      append tmpStr "space" ;

      tk::label .main.out.report.conAmr.$tmpStr -text "" ;

      pack .main.out.report.conAmr.$tmpStr -anchor w -side left ;
   } ; # Loop: build amr labels

   #**************************************************
   # Gui08 Sec05 Sub02:
   #   - build read AMR labels
   #**************************************************

   # need to add some space; otherwise have overlap
   tk::frame .main.out.report.space ;
   tk::frame .main.out.report.readAmr ;

   pack .main.out.report.space .main.out.report.readAmr -anchor w -side top ;

   tk::label .main.out.report.readAmr.lab -text "read AMRs:"  ;

   pack .main.out.report.readAmr.lab -anchor w -side left ;

   foreach amr $::glob_amrShort {
      set tmpStr [string tolower $amr] ;
      append tmpStr "lab" ;

      tk::label .main.out.report.readAmr.$tmpStr -text $amr -background $::glob_noAmrCol -fg $::glob_amrTextCol ;

      pack .main.out.report.readAmr.$tmpStr -anchor w -side left ;

      # add a space between the labels
      set tmpStr [string tolower $amr] ;
      append tmpStr "space" ;

      tk::label .main.out.report.readAmr.$tmpStr -text "" ;

      pack .main.out.report.readAmr.$tmpStr -anchor w -side left ;
   } ; # Loop: build amr labels

   #**************************************************
   # Gui08 Sec05 Sub03:
   #   - consensus spoligotype
   #   o gui08 sec05 sub03 cat01:
   #     - consensus spoligotype label
   #   o gui08 sec05 sub03 cat02:
   #     - octal for consensus spoligotype
   #   o gui08 sec05 sub03 cat03:
   #     - SIT for consensus spoligotype
   #   o gui08 sec05 sub03 cat03:
   #     - strain for consensus spoligotype
   #   o gui08 sec05 sub03 cat0::
   #     - countries dectected in (use orig for short)
   #**************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec05 Sub03 Cat01:
   #   - consensus spoligotype label
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   # add space so does not overlap with amr labels
   tk::frame .main.out.report.space2 ;
   pack .main.out.report.space2 -anchor w -side top ;

   tk::frame .main.out.report.conspol ;
   pack .main.out.report.conspol -anchor w -side top ;

   tk::frame .main.out.report.conspol.head ;
   pack .main.out.report.conspol.head -anchor w -side top ;

   tk::label  .main.out.report.conspol.head.lab -text "Consensus spoligotype:" ;

   pack .main.out.report.conspol.head.lab -anchor w -side top ;

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec05 Sub03 Cat02:
   #   - octal for consensus spoligotype
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.out.report.conspol.octal ;
   pack .main.out.report.conspol.octal -anchor w -side top ;

   pack .main.out.report.conspol.octal -anchor w -side top ;

   tk::label  .main.out.report.conspol.octal.headlab -text "     Octal: " ;

   tk::label  .main.out.report.conspol.octal.reslab -text "NA" ;

   pack .main.out.report.conspol.octal.headlab .main.out.report.conspol.octal.reslab -anchor w -side left ;

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec05 Sub03 Cat03:
   #   - SIT for consensus spoligotype
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.out.report.conspol.sit ;
   pack .main.out.report.conspol.sit -anchor w -side top ;

   pack .main.out.report.conspol.sit -anchor w -side top ;

   tk::label  .main.out.report.conspol.sit.headlab -text "     SIT: " ;

   tk::label  .main.out.report.conspol.sit.reslab -text "NA" ;

   pack .main.out.report.conspol.sit.headlab .main.out.report.conspol.sit.reslab -anchor w -side left ;

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec05 Sub03 Cat03:
   #   - strain for consensus spoligotype
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.out.report.conspol.strain ;
   pack .main.out.report.conspol.strain -anchor w -side top ;

   pack .main.out.report.conspol.strain -anchor w -side top ;

   tk::label  .main.out.report.conspol.strain.headlab -text "     Strain: " ;

   tk::label  .main.out.report.conspol.strain.reslab -text "NA" ;

   pack .main.out.report.conspol.strain.headlab .main.out.report.conspol.strain.reslab -anchor w -side left ;

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec05 Sub03 Cat04:
   #   - countries dectected in (use orig for short)
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.out.report.conspol.orig ;
   pack .main.out.report.conspol.orig -anchor w -side top ;

   pack .main.out.report.conspol.orig -anchor w -side top ;

   tk::label  .main.out.report.conspol.orig.headlab -text "     Countries: " ;

   tk::label  .main.out.report.conspol.orig.reslab -text "NA" ;

   pack .main.out.report.conspol.orig.headlab .main.out.report.conspol.orig.reslab -anchor w -side left ;

   #**************************************************
   # Gui08 Sec05 Sub04:
   #   - read spoligotype
   #   o gui08 sec05 sub04 cat01:
   #     - read spoligotype label
   #   o gui08 sec05 sub04 cat02:
   #     - octal for read spoligotype
   #   o gui08 sec05 sub04 cat03:
   #     - SIT for read spoligotype
   #   o gui08 sec05 sub04 cat03:
   #     - strain for read spoligotype
   #   o gui08 sec05 sub04 cat0::
   #     - countries dectected in (use orig for short)
   #**************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec05 Sub04 Cat01:
   #   - read spoligotype label
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   # add space so does not overlap with amr labels
   tk::frame .main.out.report.space3 ;
   pack .main.out.report.space3 -anchor w -side top ;

   tk::frame .main.out.report.readspol ;
   pack .main.out.report.readspol -anchor w -side top ;

   tk::frame .main.out.report.readspol.head ;
   pack .main.out.report.readspol.head -anchor w -side top ;

   tk::label  .main.out.report.readspol.head.lab -text "Read spoligotype:" ;

   pack .main.out.report.readspol.head.lab -anchor w -side top ;

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec05 Sub04 Cat02:
   #   - octal for read spoligotype
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.out.report.readspol.octal ;
   pack .main.out.report.readspol.octal -anchor w -side top ;

   pack .main.out.report.readspol.octal -anchor w -side top ;

   tk::label  .main.out.report.readspol.octal.headlab -text "     Octal: " ;

   tk::label  .main.out.report.readspol.octal.reslab -text "NA" ;

   pack .main.out.report.readspol.octal.headlab .main.out.report.readspol.octal.reslab -anchor w -side left ;

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec05 Sub04 Cat03:
   #   - SIT for read spoligotype
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.out.report.readspol.sit ;
   pack .main.out.report.readspol.sit -anchor w -side top ;

   pack .main.out.report.readspol.sit -anchor w -side top ;

   tk::label  .main.out.report.readspol.sit.headlab -text "     SIT: " ;

   tk::label  .main.out.report.readspol.sit.reslab -text "NA" ;

   pack .main.out.report.readspol.sit.headlab .main.out.report.readspol.sit.reslab -anchor w -side left ;

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec05 Sub04 Cat03:
   #   - strain for read spoligotype
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.out.report.readspol.strain ;
   pack .main.out.report.readspol.strain -anchor w -side top ;

   pack .main.out.report.readspol.strain -anchor w -side top ;

   tk::label  .main.out.report.readspol.strain.headlab -text "     Strain: " ;

   tk::label  .main.out.report.readspol.strain.reslab -text "NA" ;

   pack .main.out.report.readspol.strain.headlab .main.out.report.readspol.strain.reslab -anchor w -side left ;

   #++++++++++++++++++++++++++++++++++++++++++++++++++
   # Gui08 Sec05 Sub04 Cat04:
   #   - countries dectected in (use orig for short)
   #++++++++++++++++++++++++++++++++++++++++++++++++++

   tk::frame .main.out.report.readspol.orig ;
   pack .main.out.report.readspol.orig -anchor w -side top ;

   pack .main.out.report.readspol.orig -anchor w -side top ;

   tk::label  .main.out.report.readspol.orig.headlab -text "     Countries: " ;

   tk::label  .main.out.report.readspol.orig.reslab -text "NA" ;

   pack .main.out.report.readspol.orig.headlab .main.out.report.readspol.orig.reslab -anchor w -side left ;

   #**************************************************
   # Gui08 Sec05 Sub05:
   #   - MIRU lineage
   #**************************************************

   # add space to keep theme
   tk::frame .main.out.report.space4 ;
   pack .main.out.report.space4 -anchor w -side top ;

   tk::frame .main.out.report.miru ;
   pack .main.out.report.miru -anchor w -side top ;

   tk::label .main.out.report.miru.headlab -text "MIRU-VNTR: " ;

   tk::label .main.out.report.miru.reslab -text "NA" ;

   pack .main.out.report.miru.headlab .main.out.report.miru.reslab -anchor w -side left ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui08 Sec06:
   #   - set up graphs
   #   o gui08 sec05 sub01:
   #     - build reference AMR labels
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::label .main.out.depth.graph ;
   pack .main.out.depth.graph -anchor w -side left ;

   tk::label .main.out.cover.graph ;
   pack .main.out.cover.graph -anchor w -side left ;

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui08 Sec07:
   #   - set up amr table
   #   o gui08 sec07 sub01:
   #     - set up consensus AMR table
   #   o gui08 sec07 sub02:
   #     - set up read AMR table
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   #**************************************************
   # Gui08 Sec07 Sub01:
   #   - set up consensus AMR table
   #**************************************************

   tk::frame .main.out.amr.con ;
   pack .main.out.amr.con -anchor nw -side left ;

   tk::frame .main.out.amr.con.head ;
   pack .main.out.amr.con.head -anchor w -side top ;

   tk::frame .main.out.amr.con.tbl ;
   pack .main.out.amr.con.tbl -anchor w -side top ;

   tk::label .main.out.amr.con.head.lab -font "Courier" -text "Consensus ARMs:" ;
   tk::label .main.out.amr.con.tbl.lab -font "Courier" -text "NA" ;
   pack .main.out.amr.con.head.lab .main.out.amr.con.tbl.lab -anchor nw -side left ;

   #**************************************************
   # Gui08 Sec07 Sub02:
   #   - set up read AMR table
   #**************************************************

   tk::frame .main.out.amr.read ;
   pack .main.out.amr.read -anchor nw -side right ;

   tk::frame .main.out.amr.read.head ;
   pack .main.out.amr.read.head -anchor w -side top ;

   tk::frame .main.out.amr.read.tbl ;
   pack .main.out.amr.read.tbl -anchor w -side top ;

   tk::label .main.out.amr.read.head.lab -font "Courier" -text "Read AMRs:" ;
   tk::label .main.out.amr.read.tbl.lab -font "Courier" -text "NA" ;
   pack .main.out.amr.read.head.lab .main.out.amr.read.tbl.lab -anchor nw -side left ;
