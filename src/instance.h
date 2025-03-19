#ifndef W5_INSTANCE_H
#define W5_INSTANCE_H

#include "t3f/t3f.h"
#include "t3net/leaderboard.h"
#include "dictionary.h"
#include "game.h"
#include "title.h"
#include "defines.h"

typedef struct
{

  /* global data */
  T3F_VIEW * view;
  T3F_BITMAP * image[LINGO_MAX_IMAGES];
  T3F_FONT * font[LINGO_MAX_FONTS];
  ALLEGRO_SAMPLE * sample[LINGO_MAX_SAMPLES];
  int option[LINGO_MAX_OPTIONS];
  T3NET_LEADERBOARD * leaderboard;
  T3F_ATLAS * atlas;

  /* game data */
  LINGO_DICTIONARY * dictionary;
  LINGO_GAME_SETTINGS game_settings;

  LINGO_MENU menu[LINGO_MAX_MENUS];
  int current_menu;

  LINGO_MENU game_menu[LINGO_MAX_MENUS];
  int current_game_menu;

  int profiles;
  int state;
  int logic_counter;
  int quit;
  int mouse_clicked;
  char text_buffer[256];
  int text_buffer_pos;
  int text_buffer_state;
  char upload_option_text[64];

  float rot;

  LINGO_PLAYER player[LINGO_MAX_PLAYERS];
  char word[16];
  char last_word[16];
  char gameboard[6][6];
  char gameboard_color[6][6];
  ALLEGRO_COLOR gameboard_font_color[6][6];
  int high_score;

  int current_player;
  int game_ticker;
  int game_state;
  int game_clock;

  int game_check_pos;
  int game_check_current_guess;
  int game_check_correct;
  char game_check_used[5];
  char game_is_correct[5];

  /* transition data */
  int game_logo_y;
  int game_stats_x;
  int game_board_z;

  int leaderboard_place;

  int tutorial_step;
  int tutorial_freeze_clock;
  int tutorial_timer;
  int tutorial_ticker;
  char tutorial_message[16][128];
  void (*tutorial_proc)();
  int tutorial_rtlx, tutorial_rtly, tutorial_rbrx, tutorial_rbry;
  int tutorial_show_text;

  float title_logo_z;

  int argc;
  char ** argv;

} APP_INSTANCE;

APP_INSTANCE * w5_create_instance(int argc, char * argv[]);
void w5_destroy_instance(APP_INSTANCE * app);

#endif
