#ifndef LINGO_MAIN_H
#define LINGO_MAIN_H

#include "t3f/t3f.h"

void lingo_draw_text(T3F_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text);
void lingo_draw_text_center(T3F_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text);
void lingo_select_view(void * data);

#endif
