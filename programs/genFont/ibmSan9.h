#ifndef LOAD_THE_IBM_SAN9_FONT_H
#define LOAD_THE_IBM_SAN9_FONT_H

struct font_fontST;

/*______________________________________________
` Fun95: loadFont_ibmSan9
`  - loads the font in ibmSan9.c/h to the input
`    font_fontST struct (as pionter)
`  - returns 1 for success or 0 for failure
`_____________________________________________*/
signed char
loadFont_ibmSan9(
   struct font_fontST *fontSTPtr
);

#endif
