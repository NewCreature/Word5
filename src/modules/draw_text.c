#include "t3f/t3f.h"

void lingo_draw_text_with_shadow(T3F_FONT * fp, ALLEGRO_COLOR text_color, ALLEGRO_COLOR shadow_color, float x, float y, float z, float sx, float sy, int flags, const char * text)
{
  t3f_draw_text(fp, shadow_color, x + sx, y + sy, z, flags, text);
  t3f_draw_text(fp, text_color, x, y, 0, flags, text);
}
