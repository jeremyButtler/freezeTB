
variable fq_input ""
tk::frame .main -borderwidth 2

tk::label .main.text_lab -text "hi"

pack .main .main.text_lab -side "left"

         tk::frame .main.reqIn -borderwidth 2          
         tk::label .main.reqIn.fq_lab -text ""
         pack .main.reqIn.fq_lab -side left            
                                                       
         set fq_types {
            { {fastq}    {.fastq}    }
            { {fastq}    {.fq}       }
            { {fastq.gz} {.fastq.gz} }
            { {fastq.gz} {.fq.gz}    }
         } ; # comment
                                                       
         tk::button .main.reqIn.fq_but -text "fastq files" -command {                                 
               set fq_title "select fastq files"

               set fq_files [
                  tk_getOpenFile -multiple TRUE -title $fq_title -filetypes $fq_types
               ]
                                                       
               if {$fq_files eq ""} {
               } elseif {[llength $fq_files] == 1} {          
                 .main.reqIn.fq_lab configure -text $fq_files
                 set $::fq_input fq_files ; # update global list
               } else {
                 .main.reqIn.fq_lab  configure -text [file dirname [lindex $fq_files 1]]        
                 set $::fq_input fq_files ; # update global list
               } ; # set up fastq label for input           
         } ; # command button fires                   
                                                       
         pack .main.reqIn.fq_but -side left            
         pack .main.reqIn -side left            
