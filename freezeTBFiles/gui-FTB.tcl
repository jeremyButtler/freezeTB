variable glob_fqIn "" ;
variable glob_dirOut [file join $::env(HOME) "Desktop"]  ; 
variable glob_prefix "FTB_output" ;
variable glob_mask 0 ;
variable glob_srCheck "map-ont" ;
variable glob_clustBl 0 ;
variable glob_depthProfBl 0 ;
variable glob_outPref "" ;
variable glob_outCur "" ;
variable glob_depthImg [image create photo -file ""] ;
variable glob_coverImg [image create photo -file ""] ;
variable mapVer "" ;
variable freezeTBVer "" ;
variable rVer "" ;
variable mapPath "" ;
variable glob_minimapFoundBl 0 ;
variable glob_useMinimapBl 0 ;
variable rPath "" ;
variable graphScript "" ;

set glob_noAmrCol "#000004" ;
set glob_noAmrTextCol "#FDE725" ;
set glob_amrCol "#F1605D" ;
set glob_amrTextCol "#000004" ;
set glob_lowDepthCol "#FDE725" ;
set glob_lowDepthTextCol "#000004" ;
set glob_amrList [list "amikacin" "bedaquiline" "capreomycin" "clofazimine" "delamanid" "ethambutol" "ethionamide" "fluoroquine" "isoniazid" "kanamycin" "levofloxacin" "linezolid" "moxifloxacin" "penicillin-myceial-dreg" "pyrazinamide" "rifampicin" "streptomycin" ] ;

set glob_amrGenes {
{"Amk" "eis" "rrs"}
{"Bdq" "atpE" "rv0678" "pepQ"}
{"Cap" "rrl" "rrs" "tlyA"}
{"Cfz" "fbiA" "fbiB" "fbiC" "fgd1" "rv0678" "pepQ"}
{"Dlm" "ddn" "fbiA" "fbiB" "fbiC" "fgd1" "rv2983"}
{"Emb" "embA" "embB"}
{"Eto" "ethA" "fabG1" "inhA"}
{"Flq" "gyrA" "gyrB"}
{"Inh" "fabG1" "inhA" "katG"}
{"Kan" "eis" "rrs"}
{"Lfx" "gyrA" "gyrB"}
{"Lzd" "rrl" "rplC"}
{"Mfx" "gyrA" "gyrB"}
{"Pmd" "ddn" "fbiA" "fbiB" "fbiC" "fgd1" "rv2983"}
{"Pza" "pncA"}
{"Rif" "rpoB"}
{"Stm" "rrs" "gidB" "rpsL"}
} ;
variable ::mapPath "" ;
variable ::rPath "" ;
if { [lindex $tcl_platform(os) 0] eq "Windows" } {
set programFiles $::env(PROGRAMFILES) ;
set appData $::env(LOCALAPPDATA);
set glob_minimapFoundBl 1 ;
set ::mapPath "minimap2.exe" ;
set status [catch {exec $::mapPath --version} ::mapVer ]  ; 
if { $status eq 0 } {
} else {
set ::mapPath [file join $appData "freezeTB" "minimap2.exe" ] ;
set status [catch {exec $::mapPath --version} ::mapVer ]  ;
if { $status eq 0 } {
} else {
set ::mapPath [file join $programFiles "freezeTB" "minimap2.exe" ] ;
set status [catch {exec $::mapPath --version} ::mapVer ]  ;
if { $status eq 0 } {
} else {
set glob_minimapFoundBl 0 ;
} ;
} ;
} ; 
set ::rPath [file join $programFiles "R"] ;
set errSC 1 ;  
set rDirList [glob -type d -nocomplain -directory $::rPath * ] ;
set lenSI [llength $rDirList] ;
for {set siDir 0} {$siDir < $lenSI} {incr siDir} {
set ::rPath [ lindex $rDirList $siDir ] ;
set ::rPath [ file join $::rPath "bin" ] ;
set ::rPath [ file join $::rPath "Rscript.exe" ] ;
set errSC [catch {exec $::rPath --version} ::rVer ] ;
if { $errSC eq 0 } {
break ; 
} ;
} ; 
if { $errSC eq 0 } {
} else {
set ::rPath "Rscript.exe" ;
set errSC [catch {exec $::rPath --version} ::rVer ] ;
if { $errSC ne 0 } {
set ::glob_mkGraphBl 0 ;
set ::rPath "" ;
tk_messageBox -message "Unable to run Rscript" -title "ERROR" ;
} ;
} ; 
if { $::rPath ne "" } {
set ::graphScript "graphAmpDepth.r" ;
if { [file exists $::graphScript] eq 0 } {
set ::graphScript [file join $appData "freezeTB" "graphAmpDepth.r" ] ;
if { [file exists $::graphScript] eq 0 } {
set ::graphScript [file join $programFiles "freezeTB" "graphAmpDepth.r" ] ;
if { [file exists $::graphScript] eq 0 } {
set ::glob_mkGraphBl 0 ;
tk_messageBox -message "graphAmpDepth.r not found" -title "no graphing script" ;
set ::graphScript "" ;
} ;
} ;
} ;
} ; 
} else {
set glob_minimapFoundBl 1 ;
set ::mapPath "minimap2" ;
set ::rPath "Rscript" ; 
set status [catch {exec $mapPath --version} ::mapVer ]  ; 
if { $status ne 0 } {
set ::mapPath "./minimap2" ;
set status [catch {exec $::mapPath --version} ::mapVer ]  ;
if { $status ne 0 } {
set mapPath $::env(HOME);
set ::mapPath [file join $mapPath "bin" "minimap2" ] ;
set status [catch {exec $::mapPath --version} ::mapVer ]  ;
if { $status eq 0 } {
} else {
set glob_minimapFoundBl 0 ;
} ;
} ;
} ; 
set status [catch {exec $::rPath --version} ::rVer] ;
if { $status eq 0 } {
} else {
set ::glob_mkGraphBl 0 ;
set ::rPath "" ;
tk_messageBox -message "Unable to run Rscript" -title "ERROR" ;
}
if { $::rPath ne "" } {
set ::graphScript "graphAmpDepth.r" ;
if { [file exists $::graphScript] eq 0 } {
set ::graphScript [file join "/usr/local/bin" "graphAmpDepth.r"] ;
if { [file exists $::graphScript] eq 0 } {
set ::graphScript $::env(HOME);
set ::graphScript [file join $::graphScript "bin" "graphAmpDepth.r" ]
if { [file exists $::graphScript] eq 0 } {
set ::glob_mkGraphBl 0 ;
set ::graphScript "" ;
tk_messageBox -message "graphAmpDepth.r not found" -title "no graphing script" ;
} ;
} ;
} ;
} else {
} ; 
} ;

set fq_types {
{ {fastq}    {.fastq}    }
{ {fastq}    {.fq}       }
{ {fastq.gz} {.gz} }
{ {fastq.gz} {.gz}    }
} ;
set fa_types {
{ {fasta}    {.fasta}    }
{ {fasta}    {.fa}       }
} ;
set tsv_types {
{ {tsv} {.tsv} }
} ;
set csv_types {
{ {csv} {.csv} }
} ;

proc tcl_isInt_gui { value index min max } {
if { $index < 0 } {
set charIn [string index $value "end"] ;
} else {
set charIn [string index $value $index] ;
} ; 
if { $value eq "" } {
return true ;
} elseif { [string is integer $charIn] } {
if { $value > $max } {
return false ;
} elseif { $value < $min } {
return false ;
} else {
return true ;       
} ;
} else {
return false ;
} ;
return true ;
} ;
proc tcl_isFloat_gui { value index min max } {
if { $index < 0 } {
set charIn [string index $value "end"] ;
} else {
set charIn [string index $value $index] ;
} ; 
if { $value eq "" } {
return true ;
} elseif { $charIn eq "." } {
if {
[regexp -all {[0-9]*\.[0-9]*} $value] < 2
} {
return true ;   
} else {
return false ;   
} ;
} elseif { $value eq "." } {
return true ;
} elseif { [string is integer $charIn] } {
if { $value > $max } {
return false ;
} elseif { $value < $min } {
return false ;
} else {
return true ;       
} ;
} else {
return false ;
}
return true ;
} ;
wm title . "Required freezeTB"
tk::frame .main -borderwidth 2 ;
pack .main -side left ;
tk::frame .main.reqIn -borderwidth 2 ;
pack .main.reqIn -anchor w -side top ;
tk::frame .main.reqIn.fq -borderwidth 2 ;
pack .main.reqIn.fq -anchor w -side top ;
tk::label .main.reqIn.fq.lab -text "" ;
tk::button .main.reqIn.fq.but -text "fastq files" -command { 
set fqTitle "select fastq files" ;
upvar 0 glob_fqIn fqIn ; 
set fqFiles [
tk_getOpenFile -multiple TRUE -title $fqTitle -filetypes $::fq_types
] ;
if {$fqFiles eq ""} {
} elseif {[llength $fqFiles] == 1} {
.main.reqIn.fq.lab configure -text $fqFiles ;
set fqIn $fqFiles ; 
} else {
.main.reqIn.fq.lab configure -text [file dirname [lindex $fqFiles 1] ] ;
set fqIn $fqFiles ; 
} ;
} ;
pack .main.reqIn.fq.but .main.reqIn.fq.lab -side left ; 
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
} ;
} ; 
pack .main.reqIn.out.but .main.reqIn.out.lab -side left -anchor w ; 
tk::frame .main.reqIn.prefix -borderwidth 2 ;
pack .main.reqIn.prefix -anchor w -side top ;
tk::entry .main.reqIn.prefix.lab -textvariable glob_prefix ; 
pack .main.reqIn.prefix.lab -anchor w -side top
tk::frame .main.reqIn.mapper -borderwidth 2 ;
pack .main.reqIn.mapper -anchor w -side top ;
set ::glob_useMinimapBl $::glob_minimapFoundBl ;
tk::checkbutton .main.reqIn.mapper.check -text "use minimap2" -variable ::glob_useMinimapBl ; 
if { $::glob_minimapFoundBl eq 0 } {
.main.reqIn.mapper.check configure -state disabled ;
} ;
pack .main.reqIn.mapper.check -anchor w -side top
tk::frame .main.reqIn.runexit ;
pack .main.reqIn.runexit -anchor w -side top ;
proc setMapStatus {} {
.main.reqIn.runexit.statuslab configure -text "running minimap2" ;
wm title . "running minimap2 (freezeTB)" ;
} ;
proc setFreezeTBStatus {} {
.main.reqIn.runexit.statuslab configure -text "running freezeTB" ;
wm title . "running freezeTB" ;
} ;
tk::button .main.reqIn.runexit.runbut -text "run" -command { 
if { $::glob_fqIn eq "" } {
.main.menu.reqBut invoke ;
tk_messageBox -message "no fastq files input" -title "ERROR" ;
return false ; 
} ;
if { $::glob_prefix eq "" } {
.main.menu.reqBut invoke ;
tk_messageBox -message "no prefix input" -title "ERROR" ;
return false;
} ;
if { $::glob_prefix eq "" } {
.main.menu.reqBut invoke ;
tk_messageBox -message "no prefix input" -title "ERROR" ;
return false;
} ;
if { $::glob_refFa eq "" } {
.main.menu.filtBut invoke ;
tk_messageBox -message "no reference input" -title "ERROR" ;
return false;
} ;
if { $::glob_coordsTsv eq "" } {
.main.menu.filtBut invoke ;
tk_messageBox -message "no coordinates input" -title "ERROR" ;
return false;
} ;
if { $::glob_amrDb eq "" } {
.main.menu.amrBut invoke ;
tk_messageBox -message "no AMR database input" -title "ERROR" ;
return false;
} ;
if { $::glob_miruDb eq "" } {
.main.menu.linBut invoke ;
tk_messageBox -message "no MIRU-VNTR database input" -title "ERROR" ;
return false;
} ;
if { $::glob_spacer eq "" } {
.main.menu.linBut invoke ;
tk_messageBox -message "no spoligotype sequences input" -title "ERROR" ;
return false;
} ;
set prefix [ file join $glob_dirOut $glob_prefix ] ;
file mkdir $prefix ;
if { [file isdirectory $prefix] eq 0 } {
tk_messageBox -message "failed to make directory" -title "ERROR" ;
return false
} ;
set prefix [file join $prefix $glob_prefix] ;
set logFile $prefix ;
append logFile "-log.txt" ;
set logFile [open $logFile a] ;
if { $::glob_useMinimapBl ne 0 } {
set samFile $prefix ;
append samFile "-map.sam" ;
puts $logFile [concat "minmimap2: " $::mapVer] ;
set cmdStr " -a" ;
append cmdStr " -x" ;
append cmdStr " " $::glob_srCheck ;
append cmdStr " \"" $::glob_refFa "\" " ;
foreach fqStr $::glob_fqIn {
append cmdStr " \"" $fqStr "\"" ;
} ;
append cmdStr " > \"" $samFile "\"" ;
setMapStatus ; 
.main.reqIn.runexit.statuslab configure -text "running minimap2" ;
puts $logFile [concat $::mapPath $cmdStr] ;
wm title . "running minimap2 (freezeTB)" ;
set status [catch {eval exec $::mapPath $cmdStr} result ] ;
if { [ string equal $::errorCode NONE ] } {
} else {
set errMsgStr [concat "Error runing minimap2\n ERROR: " $result ] ;
tk_messageBox -message $errMsgStr -title "ERROR" ;
puts $logFile $errMsgStr ;
clost $logFile ;
return false ;
} ;
set tbCmd [list "-sam" $samFile] ;
} else {
set tbCmd [list "-ref" $::glob_refFa ] ;
} ;
puts $logFile [concat "freezeTB: " $::freezeTBVer] ;
lappend tbCmd "-prefix" $prefix ;
lappend tbCmd "-amr-tbl" $::glob_amrDb ;
lappend tbCmd "-gene-coords" ;
lappend tbCmd $::glob_coordsTsv ;
lappend tbCmd "-miru-tbl" $::glob_miruDb ;
lappend tbCmd "-spoligo" $::glob_spacer ;
if { $::glob_spolDb ne ""} {
lappend tbCmd "-db-spoligo" ;
lappend tbCmd $::glob_spolDb ;
} else {
} ; 
if { $::glob_mask eq 0 } {
lappend tbCmd "-mask-prim" "-" ;
} else {
if { $::glob_maskCoords eq 0 } {
lappend tbCmd "-mask-prim" ;
lappend tbCmd $::glob_maskCoords ;
} else {
lappend tbCmd "-mask-prim" "-" ;
tk_messageBox -message [concat "no coordinates files for masking" "skipping masking step" ] -title "skipping step" ;
} ;
} ;
lappend tbCmd "-min-mapq" $::glob_mapq ;
lappend tbCmd "-min-median-q" $::glob_medQ ;
lappend tbCmd "-min-mean-q" $::glob_meanQ ;
lappend tbCmd "-min-len" $::glob_minLen ;
if { $::glob_rmHomoBl ne 0 } {
lappend tbCmd "-rmHomo" ;
lappend tbCmd "-rmHomo-homo" ;
lappend tbCmd $::glob_rmHomo_homoSI ;
lappend tbCmd "-rmHomo-indel" ;
lappend tbCmd $::glob_rmHomo_indelSI ;
} else {
lappend tbCmd "-no-rmHomo" ;
} ;
lappend tbCmd "-fudge" $glob_miruFudge ;
lappend tbCmd "-dr-start" $::glob_drStart ;
lappend tbCmd "-dr-end" $::glob_drEnd ;
lappend tbCmd "-spoligo-min-score" ;
lappend tbCmd $::glob_spolSim ;
lappend tbCmd "-min-depth" $glob_depth ;
lappend tbCmd "-min-q" $glob_snpq ;
lappend tbCmd "-min-q-ins" $glob_insq ;
lappend tbCmd "-perc-snp-sup" ;
lappend tbCmd $::glob_basePerc ;
lappend tbCmd "-perc-ins-sup" $::glob_insPerc ;
lappend tbCmd "-perc-del-sup" $::glob_delPerc ;
lappend tbCmd "-min-amr-map-perc" ;
lappend tbCmd $glob_amrPercSup ;
lappend tbCmd "-amr-indel-sup" ;
lappend tbCmd $::glob_amrIndelSupF ;
if { $glob_frameshift ne 0 } {
lappend tbCmd "-frameshift" ;
lappend tbCmd "-frameshift-sup" ;
lappend tbCmd $::glob_amrFrameSupF ;
} else {
lappend tbCmd "-no-frameshift" ;
} ;
lappend tbCmd "-p-min-depth" ;
lappend tbCmd $glob_minVarDepth ;
lappend tbCmd "-p-perc-snp-sup" ;
lappend tbCmd glob_baseVarPerc ;
lappend tbCmd "-p-perc-ins-sup" ;
lappend tbCmd $glob_insVarPerc ;
lappend tbCmd "-p-perc-del-sup" ;
lappend tbCmd $glob_delVarPerc ;
if { $glob_clustBl ne 0 } {
lappend tbCmd "-clust" ;
lappend tbCmd "-len-weight" ;
lappend tbCmd $glob_lenWeight ;
lappend tbCmd "-clust-depth" ;
lappend tbCmd $glob_minClustDepth ;
lappend tbCmd "-clust-perc-depth" ;
lappend tbCmd $glob_minClustPercDepth ;
lappend tbCmd "-read-err" ;
lappend tbCmd $glob_clustReadErr ;
lappend tbCmd "-con-err" ;
lappend tbCmd $glob_clustConErr ;
lappend tbCmd "-con-sim" ;
lappend tbCmd $glob_maxClustSim ;
lappend tbCmd "-overlap" ;
lappend tbCmd $glob_minClustOverlap ;
lappend tbCmd "-perc-n" ;
lappend tbCmd $glob_maxClustMask ;
if { $glob_depthProfBl ne 0 } {
lappend tbCmd "-depth-prof" ;
} else {
lappend tbCmd "-no-depth-prof" ;
} ;
lappend tbCmd "-err-to-var" ;
lappend tbCmd $glob_clustErrRatio ;
lappend tbCmd "-win-len" ;
lappend tbCmd $glob_clustWinLen ;
lappend tbCmd "-win-err" ;
lappend tbCmd $glob_clustWinErrRatio ;
lappend tbCmd "-indel-len" ;
lappend tbCmd $glob_minClustIndelLen ;
lappend tbCmd "-clust-q-snp" ;
lappend tbCmd $glob_minClustSnpQ ;
lappend tbCmd "-con-iter" ;
lappend tbCmd $glob_numRebuilds ;
} else {
lappend tbCmd "-no-clust" ;
} ;
if { $::glob_useMinimapBl eq 0 } {
lappend tbCmd $::glob_fqIn ;
} ;
puts $logFile [concat "freezeTB " $tbCmd ] ;
setFreezeTBStatus ;

if { [catch {eval freezeTB $tbCmd } out] } {

tk_messageBox -message "freezeTB error" -title "ERROR" ;
puts $logFile "freezeTB error" ;
close $logFile ;
return false;
} ;
close $logFile ;
wm title . "Required freezeTB" ;
.main.reqIn.runexit.statuslab configure -text "" ;
set ::glob_outPref $prefix ;
set ::glob_outCur $prefix ;
.main.menu.outBut invoke ;
.main.out.set.run.but invoke ;
} ; 
tk::button .main.reqIn.runexit.exitbut -text "exit" -command {exit}  ;
tk::label .main.reqIn.runexit.statuslab -text "" ;
pack .main.reqIn.runexit.runbut .main.reqIn.runexit.exitbut .main.reqIn.runexit.statuslab -anchor w -side left ;
tk::frame .main.menu -borderwidth 2 ;
pack .main.menu -anchor w -side top ;
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
} ;
pack .main.menu.reqBut -anchor w -side left
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
} ;
pack .main.menu.filtBut -anchor w -side left
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
} ;
pack .main.menu.amrBut -anchor w -side left
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
} ;
pack .main.menu.linBut -anchor w -side left
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
} ;
pack .main.menu.conBut -anchor w -side left
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
} ; 
pack .main.menu.outBut -anchor w -side left ;
tk::frame .main.filt -borderwidth 2 ;
tk::frame .main.filt.ref ;
pack .main.filt.ref -anchor w -side top ;
tk::label .main.filt.ref.lab -text $glob_refFa ;
tk::button .main.filt.ref.but -text "reference" -command { 
upvar 0 glob_refFa refFa ;
set faFile [
 tk_getOpenFile -title "select reference (as fasta)" -filetypes $::fa_types ] ;
if {$faFile eq ""} {
} else {
.main.filt.ref.lab configure -text $faFile ;
set refFa $faFile ; 
} ;
} ;
pack .main.filt.ref.but .main.filt.ref.lab -anchor w -side left 
tk::frame .main.filt.coords ;
pack .main.filt.coords -anchor w -side top ;
tk::label .main.filt.coords.lab -text $glob_coordsTsv  ;
tk::button .main.filt.coords.but -text "coordinates file" -command { 
upvar 0 glob_coordsTsv coordsTsv ;
set tsvFile [
tk_getOpenFile -title "select gene coordinate file" -filetypes $::tsv_types  ] ;
if {$tsvFile eq ""} {
} else {
.main.filt.coords.lab configure -text $tsvFile ;
set coordsTsv $tsvFile ;
} ;
} ;
pack .main.filt.coords.but .main.filt.coords.lab -anchor w -side left 
tk::frame .main.filt.mapq
pack .main.filt.mapq -anchor w -side top
tk::label .main.filt.mapq.lab -text "mapq (0-93)" 
tk::entry .main.filt.mapq.entry -textvariable glob_mapq -validate key -vcmd { tcl_isInt_gui %P %i 0 93 }  ; 
pack .main.filt.mapq.lab .main.filt.mapq.entry -anchor w -side left ; 
tk::frame .main.filt.medQ
pack .main.filt.medQ -anchor w -side top
tk::label .main.filt.medQ.lab -text "median Q (0-93)"  ; 
tk::entry .main.filt.medQ.entry -textvariable glob_medQ -validate key -vcmd { tcl_isInt_gui %P %i 0 93 }  ; 
pack .main.filt.medQ.lab .main.filt.medQ.entry -anchor w -side left 
tk::frame .main.filt.meanQ
pack .main.filt.meanQ -anchor w -side top
tk::label .main.filt.meanQ.lab -text "min mean Q (0-93)"  ; 
tk::entry .main.filt.meanQ.entry -textvariable glob_meanQ -validate key -vcmd { tcl_isInt_gui %P %i 0 93 }  ; 
pack .main.filt.meanQ.lab .main.filt.meanQ.entry -anchor w -side left 
tk::frame .main.filt.len
pack .main.filt.len -anchor w -side top
tk::label .main.filt.len.lab -text "min length (> 0)"  ; 
tk::entry .main.filt.len.entry -textvariable glob_minLen -validate key -vcmd { tcl_isInt_gui %P %i 1 2000000000 }  ; 
pack .main.filt.len.lab .main.filt.len.entry -anchor w -side left 
tk::frame .main.filt.mask
pack .main.filt.mask -anchor w -side top
tk::checkbutton .main.filt.mask.check -text "mask primers" -variable glob_mask  ; 
tk::label .main.filt.mask.lab -text $::glob_maskCoords  ; 
tk::button .main.filt.mask.but -text "masking coordinates" -command { 
set maskFile [
tk_getOpenFile -title "maksing coordinates" -filetypes $::tsv_types
] ;
if {$maskFile eq "" } {
} else {
.main.filt.mask.lab configure -text $maskFile ;
set ::glob_maskCoords $maskFile ;
.main.filt.mask.check select ;
} ;
} ; 
pack .main.filt.mask.check .main.filt.mask.but .main.filt.mask.lab -anchor w -side left 
tk::frame .main.filt.sr
pack .main.filt.sr -anchor w -side top
tk::checkbutton .main.filt.sr.check -offvalue "map-ont" -onvalue "sr" -text "short reads" -variable glob_srCheck  ; 
pack .main.filt.sr.check -anchor w -side left ;
tk::frame .main.filt.rmHomo
pack .main.filt.rmHomo -anchor w -side top
tk::checkbutton .main.filt.rmHomo.check -offvalue 0 -onvalue 1 -text "indel clean up" -variable ::glob_rmHomoBl  ; 
pack .main.filt.rmHomo.check -anchor w -side left ;
tk::frame .main.filt.homoSize
pack .main.filt.homoSize -anchor w -side top
tk::label .main.filt.homoSize.lab -text "min homopolymer size (indel clean)"  ; 
tk::entry .main.filt.homoSize.entry -textvariable ::glob_rmHomo_homoSI -validate key -vcmd { tcl_isInt_gui %P %i 1 100 }  ; 
pack .main.filt.homoSize.entry -anchor w -side left ;
pack .main.filt.homoSize.lab -anchor w -side left ;
tk::frame .main.filt.indelSize
pack .main.filt.indelSize -anchor w -side top
tk::label .main.filt.indelSize.lab -text "max indel size (indel clean)"  ; 
tk::entry .main.filt.indelSize.entry -textvariable ::glob_rmHomo_indelSI -validate key -vcmd { tcl_isInt_gui %P %i 1 100 }  ; 
pack .main.filt.indelSize.entry -anchor w -side left ;
pack .main.filt.indelSize.lab -anchor w -side left ;
tk::frame .main.amr ;
tk::frame .main.amr.db ;
pack .main.amr.db -anchor w -side top ;
tk::label .main.amr.db.lab -text $glob_amrDb ;
tk::button .main.amr.db.but -text "AMR database" -command { 
set tsvFile [
tk_getOpenFile -title "select AMR database" -filetypes $::tsv_types ] ;
if {$tsvFile eq "" } {
} else {
.main.amr.db.lab configure -text $tsvFile ;
set ::glob_amrDb $tsvFile ;
} ;
} ;
pack .main.amr.db.but .main.amr.db.lab -anchor w -side left 
tk::frame .main.amr.sup ;
pack .main.amr.sup -anchor w -side top ;
tk::label .main.amr.sup.lab -text "% AMR support (0-1)"  ;
tk::entry .main.amr.sup.entry -textvariable glob_amrPercSup -validate key -vcmd { tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.amr.sup.lab .main.amr.sup.entry -anchor w -side left  ;
tk::frame .main.amr.indelsup ;
pack .main.amr.indelsup -anchor w -side top ;
tk::label .main.amr.indelsup.lab -text "% indel AMR support (0-1)"  ;
tk::entry .main.amr.indelsup.entry -textvariable ::glob_amrIndelSupF -validate key -vcmd { tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.amr.indelsup.lab .main.amr.indelsup.entry -anchor w -side left  ;
tk::frame .main.amr.frameSup ;
pack .main.amr.frameSup -anchor w -side top ;
tk::label .main.amr.frameSup.lab -text "% indel AMR support (0-1)"  ;
tk::entry .main.amr.frameSup.entry -textvariable glob_amrFrameSupF -validate key -vcmd { tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.amr.frameSup.lab .main.amr.frameSup.entry -anchor w -side left  ;
tk::frame .main.amr.frameshift ;
pack .main.amr.frameshift -anchor w -side top ;
tk::checkbutton .main.amr.frameshift.check -text "check frameshifts" -variable glob_frameshift  ;
pack .main.amr.frameshift.check -anchor w -side left  ;
tk::frame .main.lin ;
tk::frame .main.lin.db ;
pack .main.lin.db -anchor w -side top ;
tk::frame .main.lin.db.miru ;
pack .main.lin.db.miru -anchor w -side top ;
tk::label .main.lin.db.miru.lab -text $glob_miruDb ;
tk::button .main.lin.db.miru.but -text "MIRU-VNTR database" -command { 
set tsvFile [
tk_getOpenFile -title "select MIRU-VNTR database" -filetypes $::tsv_types  ] ;
if { $tsvFile eq "" } {
} else {
.main.lin.db.miru.lab configure -text $tsvFile ;
set ::glob_miruDb $tsvFile ;
} ;
} ;
pack .main.lin.db.miru.but .main.lin.db.miru.lab -anchor w -side left 
tk::frame .main.lin.db.spacer ;
pack .main.lin.db.spacer -anchor w -side top ;
tk::label .main.lin.db.spacer.lab -text $glob_spacer  ; 
tk::button .main.lin.db.spacer.but -text "spoligotype spacers" -command { 
upvar 0 glob_spacer spacer ;
set spacerFa [
tk_getOpenFile -title "spoligotype spacer sequences" -filetypes $::fa_types  ] ;
if {$spacerFa eq ""} {
} else {
.main.lin.db.spacer.lab configure -text $spacerFa ;
set spacer $spacerFa ;
} ;
} ;
pack .main.lin.db.spacer.but .main.lin.db.spacer.lab -anchor w -side left  ;
tk::frame .main.lin.db.spol ;
pack .main.lin.db.spol -anchor w -side top ;
tk::label .main.lin.db.spol.lab -text $glob_spolDb  ; 
tk::button .main.lin.db.spol.but -text "spoligotype database" -command { 
upvar 0 glob_spolDb spolDb ;
set spolTsv [
tk_getOpenFile -title "spoligotype lineage database" -filetypes $::csv_types  ] ;
if {$spolTsv eq ""} {
} else {
.main.lin.db.spol.lab configure -text $spolTsv ;
set spolDb $spolTsv ;
} ;
} ;
pack .main.lin.db.spol.but .main.lin.db.spol.lab -anchor w -side left  ;
tk::frame .main.lin.miru ;
pack .main.lin.miru -anchor w -side top ;
tk::label .main.lin.miru.lab -text "fudge (0-100)"  ;
tk::entry .main.lin.miru.entry -textvariable glob_miruFudge -validate key -vcmd { tcl_isInt_gui %P %i 0 100 } ;  
pack .main.lin.miru.lab .main.lin.miru.entry -anchor w -side left  ;
tk::frame .main.lin.spol ;
pack .main.lin.spol -anchor w -side top ;
tk::frame .main.lin.spol.sim ;
pack .main.lin.spol.sim -anchor w -side top ;
tk::label .main.lin.spol.sim.lab -text "spoligo min % match (0-1)"  ;
tk::entry .main.lin.spol.sim.entry -textvariable glob_spolSim -validate key -vcmd { tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.lin.spol.sim.lab .main.lin.spol.sim.entry -anchor w -side left  ;
tk::frame .main.lin.spol.dr ;
pack .main.lin.spol.dr -anchor w -side top ;
tk::frame .main.lin.spol.dr.dr ;
pack .main.lin.spol.dr.dr -anchor w -side top ;
tk::label .main.lin.spol.dr.dr.lab -text "Direct Repeat Coordinates:"  ;
pack .main.lin.spol.dr.dr.lab -anchor w -side left ;
tk::frame .main.lin.spol.dr.start ;
pack .main.lin.spol.dr.start -anchor w -side top ;
tk::label .main.lin.spol.dr.start.lab -text "    start (> 0):"  ;
tk::entry .main.lin.spol.dr.start.entry -textvariable glob_drStart -validate key -vcmd { tcl_isInt_gui %P %i 1 2000000000 }  ; 
pack .main.lin.spol.dr.start.lab .main.lin.spol.dr.start.entry -anchor w -side left  ;
tk::frame .main.lin.spol.dr.end ;
pack .main.lin.spol.dr.end -anchor w -side top ;
tk::label .main.lin.spol.dr.end.lab -text "    end (> 0):"  ;
tk::entry .main.lin.spol.dr.end.entry -textvariable glob_drEnd -validate key -vcmd { tcl_isInt_gui %P %i 1 2000000000 }  ; 
pack .main.lin.spol.dr.end.lab .main.lin.spol.dr.end.entry -anchor w -side left  ;
tk::frame .main.con ;
tk::frame .main.con.menu ;
tk::frame .main.con.tb ;
tk::frame .main.con.var ;
tk::frame .main.con.clust ;
pack .main.con.menu -anchor w -side top ;
pack .main.con.tb -anchor w -side top ;
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
} ; 
pack .main.con.menu.tbBut -anchor w -side left ;
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
} ; 
pack .main.con.menu.varBut -anchor w -side left ;
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
} ; 
pack .main.con.menu.clustBut -anchor w -side left ;
tk::frame .main.con.tb.snpq ;
pack .main.con.tb.snpq -anchor w -side top ;
tk::label .main.con.tb.snpq.lab -text "Min snp q-score (0-93)" ;
tk::entry .main.con.tb.snpq.entry -textvariable glob_snpq -validate key -vcmd {tcl_isInt_gui %P %i 0 93 }  ; 
pack .main.con.tb.snpq.lab .main.con.tb.snpq.entry -anchor w -side left ;
tk::frame .main.con.tb.insq ;
pack .main.con.tb.insq -anchor w -side top ;
tk::label .main.con.tb.insq.lab -text "Min insertion q-score (0-93)" ;
tk::entry .main.con.tb.insq.entry -textvariable glob_insq -validate key -vcmd {tcl_isInt_gui %P %i 0 93 }  ; 
pack .main.con.tb.insq.lab .main.con.tb.insq.entry -anchor w -side left ;
tk::frame .main.con.tb.depth ;
pack .main.con.tb.depth -anchor w -side top ;
tk::label .main.con.tb.depth.lab -text "Min read depth (> 0)" ;
tk::entry .main.con.tb.depth.entry -textvariable glob_depth -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ;
pack .main.con.tb.depth.lab .main.con.tb.depth.entry -anchor w -side left ;
tk::frame .main.con.tb.baseperc ;
pack .main.con.tb.baseperc -anchor w -side top ;
tk::label .main.con.tb.baseperc.lab -text "Min % base support (0-1)" ;
tk::entry .main.con.tb.baseperc.entry -textvariable glob_basePerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.tb.baseperc.lab .main.con.tb.baseperc.entry -anchor w -side left ;
tk::frame .main.con.tb.insperc ;
pack .main.con.tb.insperc -anchor w -side top ;
tk::label .main.con.tb.insperc.lab -text "Min % insertion support (0-1)" ;
tk::entry .main.con.tb.insperc.entry -textvariable glob_insPerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.tb.insperc.lab .main.con.tb.insperc.entry -anchor w -side left ;
tk::frame .main.con.tb.delperc ;
pack .main.con.tb.delperc -anchor w -side top ;
tk::label .main.con.tb.delperc.lab -text "Min % deletion support (0-1)" ;
tk::entry .main.con.tb.delperc.entry -textvariable glob_delPerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.tb.delperc.lab .main.con.tb.delperc.entry -anchor w -side left ;
tk::frame .main.con.var.depth ;
pack .main.con.var.depth -anchor w -side top ;
tk::label .main.con.var.depth.lab -text "Variant: min read depth (> 0)" ;
tk::entry .main.con.var.depth.entry -textvariable glob_minVarDepth -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ; 
pack .main.con.var.depth.lab .main.con.var.depth.entry -anchor w -side left ;
tk::frame .main.con.var.baseperc ;
pack .main.con.var.baseperc -anchor w -side top ;
tk::label .main.con.var.baseperc.lab -text "Variant: min % base support (0-1)" ;
tk::entry .main.con.var.baseperc.entry -textvariable glob_baseVarPerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.var.baseperc.lab .main.con.var.baseperc.entry -anchor w -side left ;
tk::frame .main.con.var.insperc ;
pack .main.con.var.insperc -anchor w -side top ;
tk::label .main.con.var.insperc.lab -text "Variant: min % insertion support (0-1)" ;
tk::entry .main.con.var.insperc.entry -textvariable glob_insVarPerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.var.insperc.lab .main.con.var.insperc.entry -anchor w -side left ;
tk::frame .main.con.var.delperc ;
pack .main.con.var.delperc -anchor w -side top ;
tk::label .main.con.var.delperc.lab -text "Variant: min % deletion support (0-1)" ;
tk::entry .main.con.var.delperc.entry -textvariable glob_delVarPerc -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.var.delperc.lab .main.con.var.delperc.entry -anchor w -side left ;
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
tk::frame .main.con.clust.mid.filt.depth
pack .main.con.clust.mid.filt.depth -anchor w -side top ;
tk::label .main.con.clust.mid.filt.depth.lab -text "min depth (> 0)" 
tk::entry .main.con.clust.mid.filt.depth.entry -textvariable glob_minClustDepth -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ; 
pack .main.con.clust.mid.filt.depth.lab .main.con.clust.mid.filt.depth.entry -anchor w -side left 
tk::frame .main.con.clust.mid.filt.percdepth
pack .main.con.clust.mid.filt.percdepth -anchor w -side top ;
tk::label .main.con.clust.mid.filt.percdepth.lab -text "min % depth (0-1)" 
tk::entry .main.con.clust.mid.filt.percdepth.entry -textvariable glob_minClustPercDepth -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.clust.mid.filt.percdepth.lab .main.con.clust.mid.filt.percdepth.entry -anchor w -side left 
tk::frame .main.con.clust.mid.filt.snpq
pack .main.con.clust.mid.filt.snpq -anchor w -side top ;
tk::label .main.con.clust.mid.filt.snpq.lab -text "min snp Q (0-93)" 
tk::entry .main.con.clust.mid.filt.snpq.entry -textvariable glob_minClustSnpQ -validate key -vcmd {tcl_isInt_gui %P %i 0 93 }  ; 
pack .main.con.clust.mid.filt.snpq.lab .main.con.clust.mid.filt.snpq.entry -anchor w -side left 
tk::frame .main.con.clust.mid.filt.indellen
pack .main.con.clust.mid.filt.indellen -anchor w -side top ;
tk::label .main.con.clust.mid.filt.indellen.lab -text "min indel length (> 0)" 
tk::entry .main.con.clust.mid.filt.indellen.entry -textvariable glob_minClustIndelLen -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ; 
pack .main.con.clust.mid.filt.indellen.lab .main.con.clust.mid.filt.indellen.entry -anchor w -side left 
tk::checkbutton .main.con.clust.mid.filt.depthprofile -text "Depth profile" -variable glob_depthProfBl  ; 
pack .main.con.clust.mid.filt.depthprofile -anchor w -side left 
tk::frame .main.con.clust.mid.calc.readerr
pack .main.con.clust.mid.calc.readerr -anchor w -side top ;
tk::label .main.con.clust.mid.calc.readerr.lab -text "read map read error rate (0-1)" 
tk::entry .main.con.clust.mid.calc.readerr.entry -textvariable glob_clustReadErr -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.clust.mid.calc.readerr.lab .main.con.clust.mid.calc.readerr.entry -anchor w -side left 
tk::frame .main.con.clust.mid.calc.conerr
pack .main.con.clust.mid.calc.conerr -anchor w -side top ;
tk::label .main.con.clust.mid.calc.conerr.lab -text "consensus map read error rate (0-1)" 
tk::entry .main.con.clust.mid.calc.conerr.entry -textvariable glob_clustConErr -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.clust.mid.calc.conerr.lab .main.con.clust.mid.calc.conerr.entry -anchor w -side left 
tk::frame .main.con.clust.mid.calc.errratio
pack .main.con.clust.mid.calc.errratio -anchor w -side top ;
tk::label .main.con.clust.mid.calc.errratio.lab -text "var:err 100*(variant/error) (> 0)" 
tk::entry .main.con.clust.mid.calc.errratio.entry -textvariable glob_clustErrRatio -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ;
pack .main.con.clust.mid.calc.errratio.lab .main.con.clust.mid.calc.errratio.entry -anchor w -side left 
tk::frame .main.con.clust.mid.calc.winerr
pack .main.con.clust.mid.calc.winerr -anchor w -side top ;
tk::label .main.con.clust.mid.calc.winerr.lab -text "window var:err (> 0)" 
tk::entry .main.con.clust.mid.calc.winerr.entry -textvariable glob_clustWinErrRatio -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ;
pack .main.con.clust.mid.calc.winerr.lab .main.con.clust.mid.calc.winerr.entry -anchor w -side left 
tk::frame .main.con.clust.mid.calc.winlen
pack .main.con.clust.mid.calc.winlen -anchor w -side top ;
tk::label .main.con.clust.mid.calc.winlen.lab -text "window length (> 0)" 
tk::entry .main.con.clust.mid.calc.winlen.entry -textvariable glob_clustWinLen -validate key -vcmd {tcl_isInt_gui %P %i 1 2000000000 }  ;
pack .main.con.clust.mid.calc.winlen.lab .main.con.clust.mid.calc.winlen.entry -anchor w -side left 
tk::frame .main.con.clust.mid.calc.lenweight
pack .main.con.clust.mid.calc.lenweight -anchor w -side top ;
tk::label .main.con.clust.mid.calc.lenweight.lab -text "score: median Q + log2(len)*input (> 0)" 
tk::entry .main.con.clust.mid.calc.lenweight.entry -textvariable glob_lenWeight -validate key -vcmd {tcl_isFloat_gui %P %i 1 2000000000 }  ;
pack .main.con.clust.mid.calc.lenweight.lab .main.con.clust.mid.calc.lenweight.entry -anchor w -side left 
tk::frame .main.con.clust.con.maxsim
pack .main.con.clust.con.maxsim -anchor w -side top ;
tk::label .main.con.clust.con.maxsim.lab -text "max % similarity (0-1)" 
tk::entry .main.con.clust.con.maxsim.entry -textvariable glob_maxClustSim -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.clust.con.maxsim.lab .main.con.clust.con.maxsim.entry -anchor w -side left 
tk::frame .main.con.clust.con.minoverlap
pack .main.con.clust.con.minoverlap -anchor w -side top ;
tk::label .main.con.clust.con.minoverlap.lab -text "min % consensus overlap (0-1)" 
tk::entry .main.con.clust.con.minoverlap.entry -textvariable glob_minClustOverlap -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.clust.con.minoverlap.lab .main.con.clust.con.minoverlap.entry -anchor w -side left 
tk::frame .main.con.clust.con.maxmask
pack .main.con.clust.con.maxmask -anchor w -side top ;
tk::label .main.con.clust.con.maxmask.lab -text "max % masking (0-1)" 
tk::entry .main.con.clust.con.maxmask.entry -textvariable glob_maxClustMask -validate key -vcmd {tcl_isFloat_gui %P %i 0 1 }  ; 
pack .main.con.clust.con.maxmask.lab .main.con.clust.con.maxmask.entry -anchor w -side left 
tk::frame .main.con.clust.con.numbuild
pack .main.con.clust.con.numbuild -anchor w -side top ;
tk::label .main.con.clust.con.numbuild.lab -text "times to rebuild consensus (0-100)" 
tk::entry .main.con.clust.con.numbuild.entry -textvariable glob_numRebuilds -validate key -vcmd {tcl_isInt_gui %P %i 0 100 }  ; 
pack .main.con.clust.con.numbuild.lab .main.con.clust.con.numbuild.entry -anchor w -side left 
tk::frame .main.out ;
tk::frame .main.out.set ;
pack .main.out.set -anchor w -side top ;
tk::frame .main.out.report ;
tk::frame .main.out.depth ;
tk::frame .main.out.cover ;
tk::frame .main.out.amr ;
proc setAmrLab {prefixStr pathStr} {
set fileStr $prefixStr ;
append fileStr "-depths.tsv" ; 
set openFILE [open $fileStr] ;
gets $openFILE lineStr ;
set geneList "" ;
while {[gets $openFILE lineStr] > 1} {
set lineStr [split $lineStr "\t"] ; 
set depthUI [lindex $lineStr 6] ;
if {$depthUI >= $::glob_depth} {
set geneStr [lindex $lineStr 9] ; 
set geneStr [string tolower $geneStr] ;
set geneList [concat $geneList $geneStr] ;
} ;
} ; 
set geneList [lsort -unique $geneList] ;
foreach drugList $::glob_amrGenes {
set lowDepthBl 0 ;
for {set siGene 1} {$siGene  < [llength $drugList]} {incr siGene}  {
set geneStr [lindex $drugList $siGene] ;
set geneStr [string tolower $geneStr] ;
if {$geneStr eq "na"} {
break ;
} elseif {[lsearch $geneList $geneStr] eq -1 } {
set lowDepthBl 1 ;
break ;
} ;
} ;
set tmpStr [lindex $drugList 0] ; 
set tmpStr [string tolower $tmpStr] ;
set labStr $pathStr ;
append labStr "." $tmpStr "lab" ; 
if {$lowDepthBl eq 0} {
$labStr configure -background $::glob_noAmrCol -fg $::glob_noAmrTextCol ;
} else {
$labStr configure -background $::glob_lowDepthCol -fg $::glob_lowDepthTextCol ;
} ;
} ; 
close $openFILE ;
} ;

proc readAmrRep {prefixStr} {
set fileStr $prefixStr ;
append fileStr "-read-amrs.tsv" ;
set openFILE [open $fileStr] ;
gets $openFILE line ; 
set amrList "" ;
setAmrLab $prefixStr .main.out.report.readAmr ;
while {[gets $openFILE line] > -1} {
set line [split $line "\t"] ;
set tmpStr [lindex $line 4] ;
set tmpF [lindex $line 8] ;
set tmpF [expr $tmpF / 100] ;
if {$tmpStr eq "snp"} {
if {$tmpF < $::glob_outSnpSup} {
continue ;
} ;
} else {
if {$tmpF < $::glob_outIndelSup} {
continue ;
} ;
} ;
set tmpStr [lindex $line 1] ;
set amrList [concat $amrList $tmpStr] ;
if { [lindex $line 2] ne "NA" } {
set tmpStr [split [lindex $line 2] "_"] ;
set amrList [concat $amrList $tmpStr] ;
} ;
set amrList [lsort -unique $amrList] ;
} ; 
close $openFILE ;
set amrList [string tolower $amrList] ;
for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr }  {
set tmpStr [lindex $::glob_amrList $siAmr] ;
set tmpStr [string tolower $tmpStr] ;
set labStr [lindex $::glob_amrGenes $siAmr 0] ;
set labStr [string tolower $labStr] ;
append labStr "lab" ;
if { [lsearch $amrList $tmpStr] ne -1 } {
.main.out.report.readAmr.$labStr configure -background $::glob_amrCol -fg $::glob_amrTextCol ;
} ;
} ; 
} ;

proc readSpol {prefixStr} {
set fileStr $prefixStr ;
append fileStr "-read-spoligo.tsv" ;
set openFILE [open $fileStr] ;
gets $openFILE lineStr ; 
set statusBl [gets $openFILE lineStr ] ;
close $openFILE ;
if {$statusBl < 0} {
.main.out.report.readspol.octal.reslab configure -text "NA" ;
.main.out.report.readspol.sit.reslab configure -text "NA" ;
.main.out.report.readspol.strain.reslab configure -text "NA" ;
.main.out.report.readspol.orig.reslab configure -text "NA" ;
return false ;
} ; 
set lineStr [split $lineStr "\t"] ;
.main.out.report.readspol.octal.reslab configure -text [lindex $lineStr 3] ;
.main.out.report.readspol.sit.reslab configure -text [lindex $lineStr 5] ;
.main.out.report.readspol.strain.reslab configure -text [lindex $lineStr 1] ;
.main.out.report.readspol.orig.reslab configure -text [lindex $lineStr 6] ;
} ;
proc readMiru {prefixStr} {
set fileStr $prefixStr ;
append fileStr "-read-mirulin.tsv" ;
set openFILE [open $fileStr] ;
gets $openFILE lineStr ; 
set status [gets $openFILE lineStr] ;
close $openFILE ;
if {$status < 0} {
.main.out.report.miru.reslab configure -text "NA" ;
return false ;
} ; 
set lineStr [split $lineStr "\t"] ;
set lineStr [lreplace $lineStr 0 0];
set tmpStr [lsort -unique $lineStr ] ;
if {[llength $tmpStr] < 2} {
if {[lindex $tmpStr 0] eq "NA"} {
.main.out.report.miru.reslab configure -text "NA" ;
return false ;
} ;
} ; 
.main.out.report.miru.reslab configure -text $lineStr ;
return true ;
} ;
proc depthGraph {prefixStr} {
set dbStr "\"" ;
append dbStr $::glob_amrDb "\"" ;
set graphStr "\"" ;
append graphStr $::graphScript "\"" ;
if {! [image inuse $::glob_depthImg] } {
image delete $::glob_depthImg ;
} ; 
if {$::glob_mkGraphBl ne 0 } {
set tmpStr "\"" ;
append tmpStr $prefixStr "-depths.tsv\"" ;
set tmpPathStr $prefixStr ;
append tmpPathStr "-readDepth.png" ;
if { [file exists $tmpPathStr] eq 1 } {
set ::glob_depthImg [image create photo -file $tmpPathStr] ;
.main.out.depth.graph configure -image $::glob_depthImg ;
} else {
set quoteStr "\"" ;
append quoteStr $prefixStr "\"" ;
set prefixStr $quoteStr ;
set status [catch {eval exec \$::rPath " " $graphStr " -stats " $tmpStr " -who " $dbStr " -prefix " $prefixStr } ] ;
if { $status ne 0 } {
tk_messageBox -message "failed to build depth graph" -title "ERROR" ;
.main.out.set.graph.check toggle ;
} else {
set ::glob_depthImg [image create photo -file $tmpPathStr ] ;
.main.out.depth.graph configure -image $::glob_depthImg ;
} ;
} ;
} ;
} ;
proc coverageGraph {prefixStr} {
set dbStr "\"" ;
append dbStr $::glob_amrDb "\"" ;
set graphStr "\"" ;
append graphStr $::graphScript "\"" ;
if {! [image inuse $::glob_coverImg] } {
image delete $::glob_coverImg ;
} ; 
if {! [image inuse $::glob_depthImg] } {
image delete $::glob_depthImg ;
} ; 
if {$::glob_mkGraphBl ne 0 } {
set statsTsv "\"" ;
append statsTsv $prefixStr "-depths.tsv\"" ;
set amrTblStr "\"" ;
append amrTblStr $prefixStr "-read-amrs.tsv\"" ;
set coverPathStr $prefixStr ;
append coverPathStr "-coverage.png" ;
set depthPathStr $prefixStr ;
append depthPathStr "-readDepth.png" ;
if { ([file exists $coverPathStr] eq 1) && ([file exists $depthPathStr] eq 1) } {
set ::glob_coverImg [image create photo -file $coverPathStr] ;
.main.out.cover.graph configure -image $::glob_coverImg ;
set ::glob_depthImg [image create photo -file $depthPathStr] ;
.main.out.depth.graph configure -image $::glob_depthImg ;
} else {
set quoteStr "\"" ;
append quoteStr $prefixStr "\"" ;
set prefixStr $quoteStr ;
set status [catch {eval exec $::rPath " " $::graphScript " -stats " $statsTsv " -amrs " $amrTblStr " -who " $::glob_amrDb " -prefix " $prefixStr } ] ;
if { $status ne 0 } {
tk_messageBox -message "failed to build cover graph" -title "ERROR" ;
.main.out.set.graph.check toggle ;
} else {
set ::glob_coverImg [ image create photo -file $coverPathStr ] ;
.main.out.cover.graph configure -image $::glob_coverImg ;
if { [file exists $coverPathStr] eq 1 } {
set ::glob_depthImg [image create photo -file $depthPathStr] ;
.main.out.depth.graph configure -image $::glob_depthImg ;
} ;
} ;
} ;
} ;
} ;
proc conAmrTbl {prefixStr indentStr} {
set fileStr $prefixStr ;
append fileStr "-con-amrs.tsv" ;
set openFILE [open $fileStr] ;
set pad1 [string length "gene"] ;
set pad2 [string length "drug"] ;
set pad3 [string length "cross-res"] ;
set pad4 [string length "var_id"] ;
set pad5 [string length "mutant"] ;
set pad9 [string length "level"] ;
set pad10 [string length "additive"] ;
set pad11 [string length "needs"] ;
if {[gets $openFILE lineStr] < 0} {
.main.out.amr.read.tbl.lab configure -text "NA" ;
return false ;
} ; 
while {[gets $openFILE lineStr] > -1} {
set lineStr [split $lineStr "\t"] ;
set tmpStr [lindex $lineStr 1] ;
set lenUI [string length $tmpStr] ;
if {$pad1 < $lenUI} { set pad1 $lenUI ; } ;
set tmpStr [lindex $lineStr 2] ;
set tmpStr [string tolower $tmpStr] ;
for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrGenes $siAmr 0] ] ;
} ;
set lenUI [string length $tmpStr] ;
if {$pad2 < $lenUI} { set pad2 $lenUI ; } ;
set tmpStr [lindex $lineStr 3] ;
set tmpStr [string tolower $tmpStr] ;
for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrGenes $siAmr 0] ] ;
} ;
set lenUI [string length $tmpStr] ;
if {$pad3 < $lenUI} { set pad3 $lenUI ; } ;
set tmpStr [lindex $lineStr 4] ;
set lenUI [string length $tmpStr] ;
if {$pad4 < $lenUI} { set pad4 $lenUI ; } ;
set tmpStr [lindex $lineStr 5] ;
set lenUI [string length $tmpStr] ;
if {$pad5 < $lenUI} { set pad5 $lenUI ; } ;
set tmpStr [lindex $lineStr 9] ;
set lenUI [string length $tmpStr] ;
if {$pad9 < $lenUI} { set pad9 $lenUI ; } ;
set tmpStr [lindex $lineStr 10] ;
set lenUI [string length $tmpStr] ;
if {$pad10 < $lenUI} { set pad10 $lenUI ; } ;
set tmpStr [lindex $lineStr 11] ;
set lenUI [string length $tmpStr] ;
if {$pad11 < $lenUI} { set pad11 $lenUI ; } ;
} ; 
close $openFILE ;
if {$pad1 eq 0} {
.main.out.amr.con.tbl.lab configure -text "NA" ;
return false ;
} ; 
set tmpStr [format "%-*s" $pad1 "gene"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad2 "drug"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad3 "cross-res"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad4 "var_id"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad5 "mutant"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad9 "level"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad10 "additive"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad11 "needs"] ;
append tblStr $tmpStr $indentStr;
append tblStr "\n";
set openFILE [open $fileStr] ;
gets $openFILE lineStr ;
while {[gets $openFILE lineStr] > -1} {
set lineStr [split $lineStr "\t"] ;
set tmpStr [lindex $lineStr 1] ; 
set tmpStr [format "%-*s" $pad1 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 2] ;
set tmpStr [string tolower $tmpStr] ;
for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrGenes $siAmr 0] ] ;
} ;
set tmpStr [format "%-*s" $pad2 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 3] ;
set tmpStr [string tolower $tmpStr] ;
for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrGenes $siAmr 0] ] ;
} ;
set tmpStr [format "%-*s" $pad3 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 4] ; 
set tmpStr [format "%-*s" $pad4 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 5] ; 
set tmpStr [format "%-*s" $pad5 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 9] ; 
set tmpStr [format "%-*s" $pad9 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 10] ; 
set tmpStr [format "%-*s" $pad10 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 11] ; 
set tmpStr [format "%-*s" $pad11 $tmpStr] ;
append tblStr $tmpStr $indentStr;
append tblStr "\n" ; 
} ; 
.main.out.amr.con.tbl.txt configure -state normal ;
.main.out.amr.con.tbl.txt delete 0.0 end ;
.main.out.amr.con.tbl.txt insert end $tblStr ;
.main.out.amr.con.tbl.txt configure -state disabled ; 
close $openFILE ;
return true ;
} ;
proc readAmrTbl {prefixStr indentStr} {
set fileStr $prefixStr ;
append fileStr "-read-amrs.tsv" ;
set openFILE [open $fileStr] ;
set pad0 [string length "gene"] ;
set pad1 [string length "drug"] ;
set pad2 [string length "cross-res"] ;
set pad3 [string length "var_id"] ;
set pad4 [string length "mutant"] ;
set pad8 [string length "%sup"] ;
set pad9 [string length "gene-depth"] ;
set pad10 [string length "level"] ;
set pad11 [string length "additive"] ;
set pad12 [string length "needs"] ;
if {[gets $openFILE lineStr] < 0} {
.main.out.amr.read.tbl.lab configure -text "NA" ;
return false ;
} ; 
while {[gets $openFILE lineStr] > -1} {
set lineStr [split $lineStr "\t"] ;
set tmpStr [lindex $lineStr 0] ;
set lenUI [string length $tmpStr] ;
if {$pad0 < $lenUI} { set pad0 $lenUI ; } ;
set tmpStr [lindex $lineStr 1] ;
set tmpStr [string tolower $tmpStr] ;
for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrGenes $siAmr 0] ] ;
} ;
set lenUI [string length $tmpStr] ;
if {$pad1 < $lenUI} { set pad1 $lenUI ; } ;
set tmpStr [lindex $lineStr 2] ;
set tmpStr [string tolower $tmpStr] ;
for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrGenes $siAmr 0] ] ;
} ;
set lenUI [string length $tmpStr] ;
if {$pad2 < $lenUI} { set pad2 $lenUI ; } ;
set tmpStr [lindex $lineStr 4] ;
set lenUI [string length $tmpStr] ;
if {$pad4 < $lenUI} { set pad4 $lenUI ; } ;
set tmpStr [lindex $lineStr 8] ;
set lenUI [string length $tmpStr] ;
if {$pad8 < $lenUI} { set pad8 $lenUI ; } ;
set tmpStr [lindex $lineStr 9] ;
set lenUI [string length $tmpStr] ;
if {$pad9 < $lenUI} { set pad9 $lenUI ; } ;
set tmpStr [lindex $lineStr 3] ;
set lenUI [string length $tmpStr] ;
if {$pad3 < $lenUI} { set pad3 $lenUI ; } ;
set tmpStr [lindex $lineStr 10] ;
set lenUI [string length $tmpStr] ;
if {$pad10 < $lenUI} { set pad10 $lenUI ; } ;
set tmpStr [lindex $lineStr 11] ;
set lenUI [string length $tmpStr] ;
if {$pad11 < $lenUI} { set pad11 $lenUI ; } ;
set tmpStr [lindex $lineStr 12] ;
set lenUI [string length $tmpStr] ;
if {$pad12 < $lenUI} { set pad12 $lenUI ; } ;
} ; 
close $openFILE ;
if {$pad0 eq 0} {
.main.out.amr.read.tbl.lab configure -text "NA" ;
return false ;
} ; 
set tmpStr [format "%-*s" $pad0 "gene"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad1 "drug"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad2 "cross-res"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad3 "var_id"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad4 "mutant"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad8 "%sup"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad9 "gene-depth"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad10 "level"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad11 "additive"] ;
append tblStr $tmpStr $indentStr;
set tmpStr [format "%-*s" $pad12 "needs"] ;
append tblStr $tmpStr $indentStr;
append tblStr "\n";
set openFILE [open $fileStr] ;
gets $openFILE lineStr ;
while {[gets $openFILE lineStr] > -1} {
set lineStr [split $lineStr "\t"] ;
set tmpF [lindex $lineStr 8] ;
set tmpF [expr $tmpF / 100] ;
if {[lindex $tmpStr 4] eq "snp"} {
if { $tmpF < $::glob_outSnpSup} {
continue ;
} ;
} else {
if { $tmpF < $::glob_outIndelSup} {
continue ;
} ;
} ;
set tmpStr [lindex $lineStr 0] ; 
set tmpStr [format "%-*s" $pad0 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 1] ;
set tmpStr [string tolower $tmpStr] ;
for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrGenes $siAmr 0] ] ;
} ;
set tmpStr [format "%-*s" $pad1 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 2] ;
set tmpStr [string tolower $tmpStr] ;
for { set siAmr 0 } { $siAmr < [llength $::glob_amrList] } { incr siAmr } {
set tmpStr [regsub [lindex $::glob_amrList $siAmr] $tmpStr [lindex $::glob_amrGenes $siAmr 0] ] ;
} ;
set tmpStr [format "%-*s" $pad2 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 3] ;
set tmpStr [format "%-*s" $pad3 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 4] ;
set tmpStr [format "%-*s" $pad4 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 8] ;
set tmpStr [format "%-*s" $pad8 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 9] ;
set tmpStr [format "%-*s" $pad9 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 10] ;
set tmpStr [format "%-*s" $pad10 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 11] ;
set tmpStr [format "%-*s" $pad11 $tmpStr] ;
append tblStr $tmpStr $indentStr;
set tmpStr [lindex $lineStr 12] ;
set tmpStr [format "%-*s" $pad12 $tmpStr] ;
append tblStr $tmpStr $indentStr;
append tblStr "\n" ; 
} ; 
.main.out.amr.read.tbl.txt configure -state normal ;
.main.out.amr.read.tbl.txt delete 0.0 end ;
.main.out.amr.read.tbl.txt insert end $tblStr ;
.main.out.amr.read.tbl.txt configure -state disabled ; 
close $openFILE ;
return true ;
} ;
tk::frame .main.out.set.prefix ;
pack .main.out.set.prefix -anchor w -side top ;
tk::label .main.out.set.prefix.lab -text "" ;
tk::button .main.out.set.prefix.but -text "get prefix" -command { 
set fileStr [
tk_getOpenFile -title "select freezeTB output"
] ;
if { $fileStr eq "" } {
} else {
upvar 0 glob_outPref outPrefStr ;
set dirStr [file dirname $fileStr] ;
set prefStr [file tail $fileStr] ;
set prefStr [regsub con.* $prefStr ""] ;
set prefStr [regsub map.* $prefStr ""] ;
set prefStr [regsub read.* $prefStr ""] ;
set prefStr [regsub tsv.* $prefStr ""] ;
set prefStr [regsub id.* $prefStr ""] ;
set prefStr [regsub dept.* $prefStr ""] ;
set prefStr [regsub grap.* $prefStr ""] ;
set prefStr [regsub coverage.* $prefStr ""] ;
set prefStr [string range $prefStr 0 end-1] ;
set outPrefStr [file join $dirStr $prefStr] ;
.main.out.set.prefix.lab configure -text $outPrefStr ;
} ;
} ; 
pack .main.out.set.prefix.but .main.out.set.prefix.lab -anchor w -side left ;
tk::frame .main.out.set.snp ;
pack .main.out.set.snp -anchor w -side top ;
tk::label .main.out.set.snp.lab -text "min AMR SNP % support" ;
tk::entry .main.out.set.snp.entry -textvariable glob_outSnpSup -validate key -vcmd { tcl_isFloat_gui %P %i 0 1 } ;
pack .main.out.set.snp.lab .main.out.set.snp.entry -anchor w -side left ;
tk::frame .main.out.set.indel ;
pack .main.out.set.indel -anchor w -side top ;
tk::label .main.out.set.indel.lab -text "min AMR Indel % support" ;
tk::entry .main.out.set.indel.entry -textvariable glob_outIndelSup -validate key -vcmd { tcl_isFloat_gui %P %i 0 1 } ;
pack .main.out.set.indel.lab .main.out.set.indel.entry -anchor w -side left ;
tk::frame .main.out.set.graph ;
pack .main.out.set.graph -anchor w -side top ;
tk::checkbutton .main.out.set.graph.check -text "build graphs" -variable glob_mkGraphBl ;
pack .main.out.set.graph.check -anchor w -side left ;
tk::frame .main.out.set.run ;
pack .main.out.set.run -anchor w -side top ;
tk::button .main.out.set.run.but -text "get report" -command { 
set ::glob_outCur $::glob_outPref ;
readAmrRep $::glob_outCur ;
coverageGraph $::glob_outCur ;
readSpol $::glob_outCur ;
readMiru $::glob_outCur ;
conAmrTbl $::glob_outCur "   " ;
readAmrTbl $::glob_outCur "   " ;
.main.out.menu.reportBut invoke ;
.main.out.set.prefix.lab configure -text "" ;
set ::glob_outPref "" ;
} ; 
pack .main.out.set.run.but -anchor w -side left ;
tk::frame .main.out.menu ;
pack .main.out.menu -anchor w -side top ;
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
} ; 
pack .main.out.menu.setBut -anchor w -side left ;
tk::button .main.out.menu.reportBut -text "report" -command { 
pack forget .main.out.set ;
pack forget .main.out.report ;
pack forget .main.out.depth ;
pack forget .main.out.cover ;
pack forget .main.out.amr ;
pack forget .main.out.menu ;
pack .main.out.report ;
pack .main.out.menu ;
wm title . [concat [file tail $glob_outCur] "report freezeTB" ] ;
.main.out.menu.setBut configure -relief raised -state normal ;
.main.out.menu.reportBut configure -relief sunken -state disabled ;
.main.out.menu.depthBut configure -relief raised -state normal ;
.main.out.menu.coverBut configure -relief raised -state normal ;
.main.out.menu.amrBut configure -relief raised -state normal ;
} ;
pack .main.out.menu.reportBut -anchor w -side left ;
tk::button .main.out.menu.depthBut -text "read depth" -command { 
pack forget .main.out.set ;
pack forget .main.out.report ;
pack forget .main.out.depth ;
pack forget .main.out.cover ;
pack forget .main.out.amr ;
pack forget .main.out.menu ;
pack .main.out.depth ;
pack .main.out.menu ;
wm title . [concat [file tail $glob_outCur] "read depth freezeTB" ] ;
.main.out.menu.setBut configure -relief raised -state normal ;
.main.out.menu.reportBut configure -relief raised -state normal ;
.main.out.menu.depthBut configure -relief sunken -state disabled ;
.main.out.menu.coverBut configure -relief raised -state normal ;
.main.out.menu.amrBut configure -relief raised -state normal ;
} ;
pack .main.out.menu.depthBut -anchor w -side left ;
tk::button .main.out.menu.coverBut -text "coverage" -command { 
pack forget .main.out.set ;
pack forget .main.out.report ;
pack forget .main.out.depth ;
pack forget .main.out.cover ;
pack forget .main.out.amr ;
pack forget .main.out.menu ;
pack .main.out.cover ;
pack .main.out.menu ;
wm title . [concat [file tail $glob_outCur] "coverage freezeTB" ] ;
.main.out.menu.setBut configure -relief raised -state normal ;
.main.out.menu.reportBut configure -relief raised -state normal ;
.main.out.menu.depthBut configure -relief raised -state normal ;
.main.out.menu.coverBut configure -relief sunken -state disabled ;
.main.out.menu.amrBut configure -relief raised -state normal ;
} ;
pack .main.out.menu.coverBut -anchor w -side left ;
tk::button .main.out.menu.amrBut -text "AMR table" -command { 
pack forget .main.out.set ;
pack forget .main.out.report ;
pack forget .main.out.depth ;
pack forget .main.out.cover ;
pack forget .main.out.amr ;
pack forget .main.out.menu ;
pack .main.out.amr ;
pack .main.out.menu ;
wm title . [concat [file tail $glob_outCur] "AMR table freezeTB" ] ;
.main.out.menu.setBut configure -relief raised -state normal ;
.main.out.menu.reportBut configure -relief raised -state normal ;
.main.out.menu.depthBut configure -relief raised -state normal ;
.main.out.menu.coverBut configure -relief raised -state normal ;
.main.out.menu.amrBut configure -relief sunken -state disabled ;
} ;
pack .main.out.menu.amrBut -anchor w -side left ;
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

tk::frame .main.out.report.space ;
tk::frame .main.out.report.readAmr ;
pack .main.out.report.space .main.out.report.readAmr -anchor w -side top ;
tk::label .main.out.report.readAmr.lab -text "read AMRs:" ;
pack .main.out.report.readAmr.lab -anchor w -side left ;
foreach amr $::glob_amrGenes {
set tmpStr [lindex $amr 0] ; 
set tmpStr [string tolower $tmpStr] ;
append tmpStr "lab" ;
tk::label .main.out.report.readAmr.$tmpStr -text [lindex $amr 0] -background $::glob_noAmrCol -fg $::glob_noAmrTextCol ;
pack .main.out.report.readAmr.$tmpStr -anchor w -side left ;
set tmpStr [string tolower $amr] ;
append tmpStr "space" ;
tk::label .main.out.report.readAmr.$tmpStr -text "" ;
pack .main.out.report.readAmr.$tmpStr -anchor w -side left ;
} ;
tk::frame .main.out.report.legSpace ;
tk::frame .main.out.report.amrLegend ;
pack .main.out.report.legSpace -side top -anchor w ;
pack .main.out.report.amrLegend -side top -anchor w ;
tk::label .main.out.report.amrLegend.legLab -text "legend: " ;
tk::label .main.out.report.amrLegend.spaceOne ;
tk::label .main.out.report.amrLegend.spaceTwo ;
tk::label .main.out.report.amrLegend.noAmrLab -background $::glob_noAmrCol -fg $::glob_noAmrTextCol -text "No drug resistance" ;
tk::label .main.out.report.amrLegend.lowDepthLab -background $::glob_lowDepthCol -fg $::glob_lowDepthTextCol -text "No resistance, but missing genes" ;
tk::label .main.out.report.amrLegend.amrLab -background $::glob_amrCol -fg $::glob_amrTextCol -text "Drug resistance" ;
pack .main.out.report.amrLegend.legLab .main.out.report.amrLegend.noAmrLab .main.out.report.amrLegend.spaceOne .main.out.report.amrLegend.lowDepthLab .main.out.report.amrLegend.spaceTwo .main.out.report.amrLegend.amrLab -anchor w -side left ;

tk::frame .main.out.report.space3 ;
pack .main.out.report.space3 -anchor w -side top ;
tk::frame .main.out.report.readspol ;
pack .main.out.report.readspol -anchor w -side top ;
tk::frame .main.out.report.readspol.head ;
pack .main.out.report.readspol.head -anchor w -side top ;
tk::label .main.out.report.readspol.head.lab -text "Read spoligotype:" ;
pack .main.out.report.readspol.head.lab -anchor w -side top ;
tk::frame .main.out.report.readspol.octal ;
pack .main.out.report.readspol.octal -anchor w -side top ;
pack .main.out.report.readspol.octal -anchor w -side top ;
tk::label .main.out.report.readspol.octal.headlab -text "     Octal: " ;
tk::label .main.out.report.readspol.octal.reslab -text "NA" ;
pack .main.out.report.readspol.octal.headlab .main.out.report.readspol.octal.reslab -anchor w -side left ;
tk::frame .main.out.report.readspol.sit ;
pack .main.out.report.readspol.sit -anchor w -side top ;
pack .main.out.report.readspol.sit -anchor w -side top ;
tk::label .main.out.report.readspol.sit.headlab -text "     SIT: " ;
tk::label .main.out.report.readspol.sit.reslab -text "NA" ;
pack .main.out.report.readspol.sit.headlab .main.out.report.readspol.sit.reslab -anchor w -side left ;
tk::frame .main.out.report.readspol.strain ;
pack .main.out.report.readspol.strain -anchor w -side top ;
pack .main.out.report.readspol.strain -anchor w -side top ;
tk::label .main.out.report.readspol.strain.headlab -text "     Strain: " ;
tk::label .main.out.report.readspol.strain.reslab -text "NA" ;
pack .main.out.report.readspol.strain.headlab .main.out.report.readspol.strain.reslab -anchor w -side left ;
tk::frame .main.out.report.readspol.orig ;
pack .main.out.report.readspol.orig -anchor w -side top ;
pack .main.out.report.readspol.orig -anchor w -side top ;
tk::label .main.out.report.readspol.orig.headlab -text "     Countries: " ;
tk::label .main.out.report.readspol.orig.reslab -text "NA" ;
pack .main.out.report.readspol.orig.headlab .main.out.report.readspol.orig.reslab -anchor w -side left ;
tk::frame .main.out.report.space4 ;
pack .main.out.report.space4 -anchor w -side top ;
tk::frame .main.out.report.miru ;
pack .main.out.report.miru -anchor w -side top ;
tk::label .main.out.report.miru.headlab -text "MIRU-VNTR: " ;
tk::label .main.out.report.miru.reslab -text "NA" ;
pack .main.out.report.miru.headlab .main.out.report.miru.reslab -anchor w -side left ;
tk::label .main.out.depth.graph ;
pack .main.out.depth.graph -anchor w -side left ;
tk::label .main.out.cover.graph ;
pack .main.out.cover.graph -anchor w -side left ;
tk::frame .main.out.amr.con ;
pack .main.out.amr.con -anchor nw -side left ;
tk::frame .main.out.amr.con.head ;
pack .main.out.amr.con.head -anchor w -side top ;
tk::frame .main.out.amr.con.tbl ;
pack .main.out.amr.con.tbl -anchor w -side top ;
tk::label .main.out.amr.con.head.lab -font "Courier" -text "Consensus ARMs:" ;
text .main.out.amr.con.tbl.txt -height 20 -width 81 -yscrollcommand ".main.out.amr.con.tbl.scroll set" ; 
.main.out.amr.con.tbl.txt configure -state disabled ;
scrollbar .main.out.amr.con.tbl.scroll -command ".main.out.amr.con.tbl.txt yview" -orient v ; 
pack .main.out.amr.con.head.lab .main.out.amr.con.tbl.txt .main.out.amr.con.tbl.scroll -anchor nw -side left ;
tk::frame .main.out.amr.read ;
pack .main.out.amr.read -anchor nw -side right ;
tk::frame .main.out.amr.read.head ;
pack .main.out.amr.read.head -anchor w -side top ;
tk::frame .main.out.amr.read.tbl ;
pack .main.out.amr.read.tbl -anchor w -side top ;
tk::label .main.out.amr.read.head.lab -font "Courier" -text "Read AMRs:" ;
text .main.out.amr.read.tbl.txt -height 20 -width 110 -yscrollcommand ".main.out.amr.read.tbl.scroll set" ; 
.main.out.amr.read.tbl.txt configure -state disabled ;
scrollbar .main.out.amr.read.tbl.scroll -command ".main.out.amr.read.tbl.txt yview" -orient v ; 
pack .main.out.amr.read.head.lab .main.out.amr.read.tbl.txt .main.out.amr.read.tbl.scroll -anchor nw -side left ;
