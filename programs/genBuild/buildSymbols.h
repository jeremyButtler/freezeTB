/*build command identifiers*/
#define def_buildSymbolsSymbol_buildSymbols '='
#define def_varSet_buildSymbols '*'
    /*modify var or access if in varaible value*/

#define def_globVarSet_buildSymbols '^'
   /*modify global var or acces global if in value*/

#define def_varAppend_buildSymbols '@'
   /*save variable for latter accesss or append*/

#define def_cmdSymbol_buildSymbols '!'
#define def_buildSymbolsCmdSymbol_buildSymbols '+'
#define def_ignoreErrSymbol_buildSymbols '~'

/*for defining the variable space*/
#define def_osMark_buildSymbols ':'
#define def_bitMark_buildSymbols '$'
#define def_simdMark_buildSymbols '.'

#define def_escape_buildSymbols '\\'
