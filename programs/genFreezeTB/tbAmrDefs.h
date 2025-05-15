/*########################################################
# Name: tbAmrDefs
#   - version number and defaults for tbAmr
########################################################*/

#define def_checkFramshift_tbAmrDefs 1
  /*1: check frameshifts*/
#define def_framshiftSup_tbAmrDefs 0.7f
  /*minium support to keep a frameshift*/
#define def_aaIndel_tbAmrDefs 0
  /*1: check indels in SNP amino acid AMRs
  `  ONT is quite poor at indels , so leave off
  */

#define def_minDepth_tbAmrDefs 10

#define def_minPercMapped_tbAmrDefs 0.1f
    /*At least 5% of reads with region support the amr*/

#define def_minIndelSup_tbAmrDefs 0.2f

#define def_minPercTotalReads_tbAmrDefs 0.00f
    /*0% (turn off) of all kept reads mapped to this amr
    `   region
    */

