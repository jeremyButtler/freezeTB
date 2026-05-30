#ifndef LOAD_THE_SMALL_FONT_FONT_H
#define LOAD_THE_SMALL_FONT_FONT_H

struct font_fontST;

/*______________________________________________
` Fun95: loadFont_smallFont
`  - loads the font in smallFont.c/h to the input
`    font_fontST struct (as pionter)
`  - returns 1 for success or 0 for failure
`_____________________________________________*/
signed char
loadFont_smallFont(
   struct font_fontST *fontSTPtr
);

#endif
