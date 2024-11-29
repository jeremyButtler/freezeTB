tk::frame .main -borderwidth 2
variable glob_mapq 15

tk::frame .main.mapq
pack .main
pack .main.mapq -anchor w -side top

tk::label .main.mapq.lab -text "min mapq (0-93)"
pack .main.mapq.lab -anchor w -side left

proc isNumeric { value max min } {
   set charIn [string index $value "end"] ;

   if { $value eq "" } {
      return true ; # nothing in string
   } elseif { [string is integer $charIn] } {
      if { $value > $max } {
         return false ;
      } elseif { $value < $min } {
         return false ;
      }
      return true ;
   } else {
     return false ;
   }

   return true ; # empty case
} ; # isNumeric

tk::entry .main.mapq.entry -textvariable glob_mapq -validate key -vcmd {isNumeric %P 93 0}
       #expr { ( [string is integer %P] && %P <= 93 ) || %P == ""}
    #}

# exp { [string is interger -strict %P] && %P <= 93 && %P > 0 }

pack .main.mapq.entry -anchor w -side left
