#ifndef _LINGO_CREDITS_H
#define _LINGO_CREDITS_H

#include "t3f/t3f.h"

typedef struct
{

  T3F_FONT * font;
  ALLEGRO_COLOR color;
  float x;
  float y;
  char * text;

} LINGO_CREDIT;

typedef struct
{

  LINGO_CREDIT * credit;
  int credit_size;
  int credit_count;

  /* playback data */
  float x;
  float y;
  float vy;

  /* credits boundaries */
  float top;
  float height;

} LINGO_CREDITS;

LINGO_CREDITS * lingo_create_credits(int max);
void lingo_destroy_credits(LINGO_CREDITS * cp);
void lingo_add_credit(LINGO_CREDITS * cp, T3F_FONT * fp, ALLEGRO_COLOR color, float x, float y, const char * text);

void lingo_start_credits(LINGO_CREDITS * cp, float top, float height, float x, float y, float vy);
bool lingo_process_credits(LINGO_CREDITS * cp);
void lingo_render_credits(LINGO_CREDITS * cp);

bool lingo_setup_credits(void * data);
void lingo_credits_logic(void * data);
void lingo_credits_render(void * data);

#endif
