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
   #--------------------------------------------------

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

   #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   # Remove thes global variables; defaults should be
   #   set by C
   #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   variable glob_refFa "../../freezeTBFiles/NC000962.fa" ;
   variable glob_coordsTsv "../../freezeTBFiles/coords.tsv" ;
   variable glob_amrDb "../../freezeTBFiles/amrDb.tsv" ;
   variable glob_maskCoords "../../freezeTBFiles/mask.tsv" ;
   variable glob_miruDb "../../freezeTBFiles/miruTbl.tsv" ;
   variable glob_spacer "../../freezeTBFiles/spoligo-spacers.tsv" ;
   variable glob_spolDb "../../freezeTBFiles/spoligo-lineages.csv" ;

   # read filtering settings
   variable glob_mapq 15 ;
   variable glob_medQ 7 ;
   variable glob_meanQ 7 ;
   variable glob_depth 10 ;

   # amr settings
   variable glob_amrPercSup 0.1 ;

   # lineage settings
   variable glob_miruFudge 15 ;
   variable glob_spolSim 0.9 ;
   variable glob_drStart 3119037 ;
   variable glob_drEnd 3123624 ;

   # consensus settings
   variable glob_snpq 3 ;
   variable glob_insq 3 ;
   variable glob_minLen 50 ;
   variable glob_basePerc 0.4 ;
   variable glob_insPerc 0.7 ;
   variable glob_delPerc 0.7 ;

   # consensus variant settings
   variable glob_minVarDepth 10 ;
   variable glob_baseVarPerc 0.1 ;
   variable glob_insVarPerc 0.2 ;
   variable glob_delVarPerc 0.2 ;

   # cluster settings
   variable glob_clustBl 0 ;
   variable glob_depthProfBl 0 ;

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

   # file types

   set fq_types {
      { {fastq}    {.fastq}    }
      { {fastq}    {.fq}       }
      { {fastq.gz} {.fastq.gz} }
      { {fastq.gz} {.fq.gz}    }
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
           upvar 0 glob_fqIn fqIn ; # make global var
   
           set fqFiles [
                 tk_getOpenFile -multiple TRUE -title $fqTitle -filetypes $fq_types 
           ] ;
   
           if {$fqFiles eq ""} {
   
           } elseif {[llength $fqFiles] == 1} {
              .main.reqIn.fq.lab configure -text $fqFiles ; 
   
             set fqIn $fqFiles ; # update list
           } else {
             .main.reqIn.fq.lab configure -text [file dirname [lindex $fqFiles 1] ] ;
   
             set fqIn $fqFiles ; # update list
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
            upvar 0 glob_dirOut dirOut ;

            set outDir [
               tk_chooseDirectory -title "select output directory" 
            ] ;
            if {$outDir ne ""} {
               .main.reqIn.out.lab configure -text $outDir ; 

               set dirOut $outDir ;
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
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.reqIn.runexit ;
   pack .main.reqIn.runexit -anchor w -side top ;

   tk::button .main.reqIn.runexit.runbut -text "run" -command { 
         if { $glob_fqIn eq "" } {
            .main.menu.reqBut invoke ;

            tk_messageBox -message "no fastq files input" -title "ERROR" ;

            return false ; # no fastq files
         } ; # no fastq file input

         if { $glob_prefix eq "" } {
            .main.menu.reqBut invoke ;

            tk_messageBox -message "no prefix input" -title "ERROR" ;
            return false;
         } ; #  no prefix (was deleted)

         set prefix [ file join $glob_dirOut $glob_prefix ] ; # build output file path*/

         file mkdir $prefix ;

         if { [file isdirectory $prefix] eq 0 } {
            tk_messageBox -message "failed to make directory" -title "ERROR" ;

           return false
         } ; # failed to make output directory

         set prefix [file join $prefix $glob_prefix] ;
         set samFile ">";
         append samFile $prefix "-map.sam" ;

         # run minimap2 command
         set status [catch { exec minimap2 -a -x $glob_srCheck $glob_refFa $glob_fqIn $samFile } result  ]  ;

         if { [string equal $::errorCode NONE] } {

         } else {
            tk_messageBox -message [concat "Error runing minimap2\n" $result ] -title "ERROR" ;
         } ; # Else had error
           
      } ; # run freezeTB button

   tk::button .main.reqIn.runexit.exitbut -text "exit" -command {exit}  ;

   pack .main.reqIn.runexit.runbut .main.reqIn.runexit.exitbut -anchor w -side left ;
   
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
   
         pack .main.reqIn -anchor w -side top ;
         pack .main.menu -anchor w -side top ;

         .main.menu.reqBut configure -relief sunken ;
         .main.menu.filtBut configure -relief raised ;
         .main.menu.amrBut configure -relief raised ;
         .main.menu.linBut configure -relief raised ;
         .main.menu.conBut configure -relief raised ;

         .main.menu.reqBut configure -state disabled ;
         .main.menu.filtBut configure -state normal ;
         .main.menu.amrBut configure -state normal ;
         .main.menu.linBut configure -state normal ;
         .main.menu.conBut configure -state normal ;

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
   
         pack .main.filt -anchor w -side top ;
         pack .main.menu -anchor w -side top ;

         .main.menu.reqBut configure -relief raised ;
         .main.menu.filtBut configure -relief sunken ;
         .main.menu.amrBut configure -relief raised ;
         .main.menu.linBut configure -relief raised ;
         .main.menu.conBut configure -relief raised ;

         .main.menu.reqBut configure -state normal ;
         .main.menu.filtBut configure -state disabled ;
         .main.menu.amrBut configure -state normal ;
         .main.menu.linBut configure -state normal ;
         .main.menu.conBut configure -state normal ;

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
   
         pack .main.lin -anchor w -side top ;
         pack .main.menu -anchor w -side top ;

         .main.menu.reqBut configure -relief raised ;
         .main.menu.filtBut configure -relief raised ;
         .main.menu.amrBut configure -relief raised ;
         .main.menu.linBut configure -relief sunken ;
         .main.menu.conBut configure -relief raised ;

         .main.menu.reqBut configure -state normal ;
         .main.menu.filtBut configure -state normal ;
         .main.menu.amrBut configure -state normal ;
         .main.menu.linBut configure -state disabled ;
         .main.menu.conBut configure -state normal ;

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

         .main.menu.reqBut configure -state normal ;
         .main.menu.filtBut configure -state normal ;
         .main.menu.amrBut configure -state normal ;
         .main.menu.linBut configure -state normal ;
         .main.menu.conBut configure -state disabled ;

   } ; # command button fires

   pack .main.menu.conBut -anchor w -side left

   
   
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

   tk::label .main.filt.ref.lab -text $glob_refFa ;

   tk::button .main.filt.ref.but -text "reference" -command { 
           upvar 0 glob_refFa refFa ;

           set faFile [
               tk_getOpenFile -title "select reference (as fasta)" -filetypes $fa_types ] ; # find the reference fasta
   
           if {$faFile eq ""} {
   
           } else {
              .main.filt.ref.lab configure -text $faFile ;  
   
             set refFa $faFile ; # update list
           } ; # Else: fasta selected
   } ; # select reference button

   pack .main.filt.ref.but .main.filt.ref.lab -anchor w -side left 

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec03:
   #   - coordinates file input
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.coords ;
   pack .main.filt.coords -anchor w -side top ;

   tk::label .main.filt.coords.lab -text $glob_coordsTsv  ;

   tk::button .main.filt.coords.but -text "coordinates file" -command { 
           upvar 0 glob_coordsTsv coordsTsv ;

           set tsvFile [
              tk_getOpenFile -title "select gene coordinate file" -filetypes $tsv_types  ] ; # find gene coordinates file
   
           if {$tsvFile eq ""} {
   
           } else {
              .main.filt.coords.lab configure -text $tsvFile  ;

             set coordsTsv $tsvFile ;
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
   #   - minimum read depth
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.depth
   pack .main.filt.depth -anchor w -side top

   tk::label .main.filt.depth.lab -text "min read depth (> 0)"  ; # medain q-score entry leabe

   tk::entry .main.filt.depth.entry -textvariable glob_depth -validate key -vcmd { tcl_isInt_gui %P %i 1 2000000000 }  ; # get minimum read depth

   pack .main.filt.depth.lab .main.filt.depth.entry -anchor w -side left 

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui04 Sec08:
   #   - masking check box + button
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   tk::frame .main.filt.mask
   pack .main.filt.mask -anchor w -side top

   tk::checkbutton .main.filt.mask.check -text "mask primers"  -variable glob_mask  ; # if doing long or short reads

   tk::label .main.filt.mask.lab -text $glob_maskCoords  ; # label for masking file

   tk::button .main.filt.mask.but -text "masking coordinates" -command { 
         upvar 0 glob_maskCoords maskCoords ;

         set maskFile [
            tk_getOpenFile -title "maksing coordinates" -filetypes $tsv_types 
         ] ;

         if {$maskFile eq "" } {

         } else {
            .main.filt.mask.lab configure -text $maskFile ; 
            set maskCoords $maskFile ;

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

   tk::label .main.amr.db.lab -text $glob_amrDb

   tk::button .main.amr.db.but -text "AMR database" -command { 
         upvar 0 glob_amrDb amrDb ;

         set tsvFile [
            tk_getOpenFile -title "select AMR database" -filetypes $tsv_types ] ;
          if {$tsvFile eq "" } {

          } else {
            .main.amr.db.lab configure -text $tsvFile ;
            set amrDb $tsvFile ;
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

   tk::label .main.lin.db.miru.lab -text $glob_miruDb ;

   tk::button .main.lin.db.miru.but -text "MIRU-VNTR database" -command { 
         upvar glob_miruDb miruDb ;

         set tsvFile [
            tk_getOpenFile -title "select MIRU-VNTR database" -filetypes $tsv_types  ] ; # get MIRU databse

         if { $tsvFile eq "" } {

         } else {
            .main.lin.db.miru.lab configure -text $tsvFile ;
             set miruDb $tsvFile ;
         } ; # Else: miru database selected
   } ; # -command get MIRU database

   pack .main.lin.db.miru.but .main.lin.db.miru.lab -anchor w -side left 

   #***************************************************
   # Gui06 Sec02 Sub03:
   #   - set up spoligotype sequences button
   #***************************************************

   tk::frame .main.lin.db.spacer ;
   pack .main.lin.db.spacer -anchor w -side top ;

   tk::label .main.lin.db.spacer.lab -text $glob_spacer  ; # holds spacer database location

   tk::button .main.lin.db.spacer.but -text "spoligotype spacers" -command { 
         upvar 0 glob_spacer spacer ;

         set spacerFa [
            tk_getOpenFile -title "spoligotype spacer sequences" -filetypes $fa_types  ] ; # get spacer sequence fasta file

         if {$spacerFa eq ""} {

         } else {
            .main.lin.db.spacer.lab configure -text $spacerFa ;
            set spacer $spacerFa ;
         } ; # Else: user input fasta
   } ; # button for spacers database

   pack .main.lin.db.spacer.but .main.lin.db.spacer.lab -anchor w -side left  ;   

   #***************************************************
   # Gui06 Sec02 Sub04:
   #   - set up spoligotype database button
   #***************************************************

   tk::frame .main.lin.db.spol ;
   pack .main.lin.db.spol -anchor w -side top ;

   tk::label .main.lin.db.spol.lab -text $glob_spolDb  ; # holds spol database location

   tk::button .main.lin.db.spol.but -text "spoligotype database" -command { 
         upvar 0 glob_spolDb spolDb ;

         set spolTsv [
            tk_getOpenFile -title "spoligotype lineage database" -filetypes $csv_types  ] ;

         if {$spolTsv eq ""} {

         } else {
            .main.lin.db.spol.lab configure -text $spolTsv ;
            set spolDb $spolTsv ;
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
   #   - min length
   #***************************************************

   tk::frame .main.con.tb.len ;
   pack .main.con.tb.len -anchor w -side top ;

   tk::label .main.con.tb.len.lab -text "Min length (> 0)" ;

   tk::entry .main.con.tb.len.entry -textvariable glob_minLen -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ; # min snp q-score entry

   pack .main.con.tb.len.lab .main.con.tb.len.entry -anchor w -side left ;

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

   

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Gui08 Sec01:
   #   - 
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   

   

   

   
