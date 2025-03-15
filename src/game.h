#ifndef LINGO_GAME_H
#define LINGO_GAME_H

#include "player.h"

#define LINGO_GAMEBOARD_X_OFFSET 296
#define LINGO_GAMEBOARD_Y_OFFSET 170

#define LINGO_GAME_MENU_MAIN        0
#define LINGO_GAME_MENU_OVER        1
#define LINGO_GAME_MENU_MAIN_DUMMY  2

#define LINGO_GAME_MODE_1P_SURVIVAL 0
#define LINGO_GAME_MODE_2P_STANDARD 1

#define LINGO_GAME_STATE_FIRST_LETTER   0
#define LINGO_GAME_STATE_TYPING         1
#define LINGO_GAME_STATE_CHECK_LETTERS  2
#define LINGO_GAME_STATE_SHOW_LETTERS   3
#define LINGO_GAME_STATE_CHOOSE_NUMBER  4
#define LINGO_GAME_STATE_OVER           5

#define LINGO_GAME_STATE_P_INVALID      6
#define LINGO_GAME_STATE_P_CORRECT      7
#define LINGO_GAME_STATE_P_INCORRECT    8
#define LINGO_GAME_STATE_P_BONUS_LETTER 9

#define LINGO_GAME_STATE_SHOW_CORRECT  10

typedef struct
{
	
	int mode;
	int players;
	
} LINGO_GAME_SETTINGS;

extern LINGO_PLAYER lingo_player[LINGO_MAX_PLAYERS];
extern char lingo_word[16];
extern char lingo_last_word[16];
extern char lingo_gameboard[6][6];
extern char lingo_gameboard_color[6][6];
extern ALLEGRO_COLOR lingo_gameboard_font_color[6][6];
extern int lingo_high_score;

extern int lingo_current_player;
extern int lingo_game_ticker;
extern int lingo_game_state;
extern int lingo_game_clock;

extern int lingo_game_check_pos;
extern int lingo_game_check_current_guess;
extern int lingo_game_check_correct;
extern char lingo_game_check_used[5];
extern char lingo_game_is_correct[5];

/* transition data */
extern int lingo_game_logo_y;
extern int lingo_game_stats_x;
extern int lingo_game_board_z;

void lingo_game_shift_board_up(void * data);
int lingo_game_give_bonus_letter(void * data);

void lingo_game_start(int mode, void * data);
void lingo_game_transition_in_logic(void * data);
void lingo_game_transition_out_logic(void * data);
void lingo_game_logic(void * data);
void lingo_game_render(void * data);

#endif
