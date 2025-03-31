#include "t3f/t3f.h"

void lingo_draw_text(T3F_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text)
{
	t3f_draw_text(fp, color, x, y, 0, 0, text);
}

void lingo_draw_text_center(T3F_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text)
{
	t3f_draw_text(fp, color, x - t3f_get_text_width(fp, text) / 2, y, 0, 0, text);
}

void lingo_draw_text_right(T3F_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text)
{
	t3f_draw_text(fp, color, x, y, 0, T3F_FONT_ALIGN_RIGHT, text);
}

void lingo_draw_text_with_shadow(T3F_FONT * fp, ALLEGRO_COLOR text_color, ALLEGRO_COLOR shadow_color, float x, float y, float z, float sx, float sy, int flags, const char * text)
{
  t3f_draw_text(fp, shadow_color, x + sx, y + sy, z, flags, text);
  t3f_draw_text(fp, text_color, x, y, 0, flags, text);
}
