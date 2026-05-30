#ifndef LOAD_THE_NORMAL_FONT_FONT_H
#define LOAD_THE_NORMAL_FONT_FONT_H

struct font_fontST;

/*______________________________________________
` Fun95: loadFont_normalFont
`  - loads the font in normalFont.c/h to the input
`    font_fontST struct (as pionter)
`  - returns 1 for success or 0 for failure
`_____________________________________________*/
signed char
loadFont_normalFont(
   struct font_fontST *fontSTPtr
);

#endif
