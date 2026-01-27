/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' getAmrDefs SOF: Start Of File
'   - default settings for getAmr
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define def_checkFramshift_getAmrDefs 1
  /*1: check frameshifts*/
#define def_framshiftSup_getAmrDefs 0.7f
  /*minium support to keep a frameshift*/
#define def_aaIndel_getAmrDefs 0
  /*1: check indels in SNP amino acid AMRs
  `  ONT is quite poor at indels , so leave off
  */

#define def_minDepth_getAmrDefs 10

#define def_minPercMapped_getAmrDefs 0.1f
    /*At least 5% of reads with region support the amr*/

#define def_minIndelSup_getAmrDefs 0.2f

#define def_minPercTotalReads_getAmrDefs 0.00f
    /*0% (turn off) of all kept reads mapped to this amr
    `   region
    */
