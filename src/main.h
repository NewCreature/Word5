#ifndef LINGO_MAIN_H
#define LINGO_MAIN_H

#include <allegro5/allegro_font.h>

void lingo_draw_text(ALLEGRO_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text);
void lingo_draw_text_center(ALLEGRO_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text);
void lingo_select_view(void * data);

#endif
