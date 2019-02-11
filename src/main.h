#ifndef LINGO_MAIN_H
#define LINGO_MAIN_H

#include <allegro5/allegro_font.h>

#define LINGO_STATE_INTRO                    0
#define LINGO_STATE_TITLE                    1
#define LINGO_STATE_ENTER_NAME               2

#define LINGO_STATE_GAME                     3
#define LINGO_STATE_GAME_TRANSITION_IN       4
#define LINGO_STATE_GAME_TRANSITION_OUT      5

#define LINGO_STATE_TUTORIAL                 6
#define LINGO_STATE_TUTORIAL_TRANSITION_IN   7
#define LINGO_STATE_TUTORIAL_TRANSITION_OUT  8

#define LINGO_STATE_TITLE_TRANSITION_IN      9
#define LINGO_STATE_TITLE_TRANSITION_OUT    10

#define LINGO_STATE_LEADERBOARD             11

#define LINGO_COLOR_WHITE al_map_rgba_f(1.0, 1.0, 1.0, 1.0)

void lingo_draw_text(ALLEGRO_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text);
void lingo_draw_text_center(ALLEGRO_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text);
void lingo_select_view(void);

#endif
