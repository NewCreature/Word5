#include "t3f/t3f.h"

ALLEGRO_COLOR lingo_alpha_color(ALLEGRO_COLOR color, float alpha)
{
  float r, g, b, a;

  al_unmap_rgba_f(color, &r, &g, &b, &a);

  return al_map_rgba_f(r * alpha, g * alpha, b * alpha, alpha);
}
