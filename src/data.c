#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include "t3f/t3f.h"
#include "t3net/t3net.h"
#include "main.h"
#include "data.h"
#include "title.h"
#include "game.h"

/* global data */
ALLEGRO_BITMAP * lingo_image[LINGO_MAX_IMAGES] = {NULL};
ALLEGRO_FONT * lingo_font[LINGO_MAX_FONTS] = {NULL};
ALLEGRO_SAMPLE * lingo_sample[LINGO_MAX_SAMPLES] = {NULL};
int lingo_option[LINGO_MAX_OPTIONS] = {0};
T3NET_LEADERBOARD * lingo_leaderboard = NULL;
T3F_ATLAS * lingo_atlas = NULL;

/* game data */
LINGO_DICTIONARY * lingo_dictionary;
LINGO_GAME_SETTINGS lingo_game_settings;

LINGO_MENU lingo_menu[LINGO_MAX_MENUS];
int lingo_current_menu = LINGO_MENU_MAIN;

LINGO_MENU lingo_game_menu[LINGO_MAX_MENUS];
int lingo_current_game_menu = LINGO_GAME_MENU_MAIN;

int lingo_profiles = 0;
int lingo_state = LINGO_STATE_TITLE_TRANSITION_IN;
int lingo_logic_counter = 0;
int lingo_quit = 0;
//int lingo_mouse_x = 0;
//int lingo_mouse_y = 0;
//int lingo_mouse_click = 0;
int lingo_mouse_clicked = 0;
char lingo_text_buffer[256] = {0};
int lingo_text_buffer_pos = 0;
int lingo_text_buffer_state = 0;
char lingo_upload_option_text[64] = {0};

float rot = 0.0;
