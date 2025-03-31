#ifndef LINFO_DRAW_TEXT_H
#define LINFO_DRAW_TEXT_H

#include "t3f/t3f.h"

void lingo_draw_text_with_shadow(T3F_FONT * fp, ALLEGRO_COLOR text_color, ALLEGRO_COLOR shadow_color, float x, float y, float z, float sx, float sy, int flags, const char * text);

#endif