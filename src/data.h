#ifndef LINGO_DATA_H
#define LINGO_DATA_H

#include "t3f/t3f.h"
#include "t3net/leaderboard.h"
#include "dictionary.h"
#include "title.h"
#include "game.h"

#define LINGO_MAX_IMAGES        256

#define LINGO_IMAGE_LOGO          0
#define LINGO_IMAGE_GAMEBOARD     1
#define LINGO_IMAGE_RED_SQUARE    2
#define LINGO_IMAGE_YELLOW_CIRCLE 3
#define LINGO_IMAGE_BG            4
#define LINGO_IMAGE_LOAD          5

#define LINGO_MAX_FONTS          16

#define LINGO_FONT_SPRINT_10      0
#define LINGO_FONT_SPRINT_20      1
#define LINGO_FONT_SPRINT_36      2
#define LINGO_FONT_ARIAL_36       3

#define LINGO_MAX_SAMPLES        32

#define LINGO_SAMPLE_MENU_HOVER   0
#define LINGO_SAMPLE_MENU_CLICK   1
#define LINGO_SAMPLE_FIRST_LETTER 2
#define LINGO_SAMPLE_TYPING       3
#define LINGO_SAMPLE_RED          4
#define LINGO_SAMPLE_YELLOW       5
#define LINGO_SAMPLE_CORRECT      6
#define LINGO_SAMPLE_INCORRECT    7
#define LINGO_SAMPLE_CORRECT_LETTER    8
#define LINGO_SAMPLE_INCORRECT_LETTER  9
#define LINGO_SAMPLE_YELLOW_LETTER    10
#define LINGO_SAMPLE_HINT             11
#define LINGO_SAMPLE_LOGO             12
#define LINGO_SAMPLE_TYPE             13
#define LINGO_SAMPLE_CLOCK            14

#define LINGO_MAX_OPTIONS        32
#define LINGO_OPTION_UPLOAD       0

extern T3F_VIEW * lingo_view;
extern ALLEGRO_BITMAP * lingo_image[LINGO_MAX_IMAGES];
extern ALLEGRO_FONT * lingo_font[LINGO_MAX_FONTS];
extern ALLEGRO_SAMPLE * lingo_sample[LINGO_MAX_SAMPLES];
extern int lingo_option[LINGO_MAX_OPTIONS];
extern T3NET_LEADERBOARD * lingo_leaderboard;
extern T3F_ATLAS * lingo_atlas;
extern LINGO_DICTIONARY * lingo_dictionary;
extern LINGO_GAME_SETTINGS lingo_game_settings;
extern int lingo_profiles;

extern LINGO_MENU lingo_menu[LINGO_MAX_MENUS];
extern int lingo_current_menu;
extern LINGO_MENU lingo_game_menu[LINGO_MAX_MENUS];
extern int lingo_current_game_menu;

extern int lingo_state;
extern int lingo_logic_counter;
extern int lingo_quit;
extern float rot;
extern int lingo_mouse_x;
extern int lingo_mouse_y;
extern int lingo_mouse_click;
extern int lingo_mouse_clicked;
extern char lingo_text_buffer[256];
extern int lingo_text_buffer_pos;
extern int lingo_text_buffer_state;
extern char lingo_upload_option_text[64];

#endif
