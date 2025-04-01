#ifndef LINGO_MENUPROC_H
#define LINGO_MENUPROC_H

void lingo_menu_proc_main_play(void * data);
void lingo_menu_proc_main_start_game(void * data);
void lingo_menu_proc_main_cancel(void * data);
void lingo_menu_proc_main_scores(void * data);
void lingo_menu_proc_main_options(void * data);
void lingo_menu_proc_main_leaderboard(void * data);
void lingo_menu_proc_main_tutorial(void * data);
void lingo_menu_proc_main_credits(void * data);
void lingo_menu_proc_main_quit(void * data);

void lingo_menu_proc_options_upload(void * data);
void lingo_menu_proc_options_back(void * data);

void lingo_menu_proc_game_main_bonus_letter(void * data);
void lingo_menu_proc_game_main_quit(void * data);

void lingo_menu_proc_game_over_play_again(void * data);
void lingo_menu_proc_game_over_exit(void * data);

void lingo_menu_proc_leaderboard_play_again(void * data);
void lingo_menu_proc_leaderboard_exit(void * data);
void lingo_menu_proc_leaderboard_back(void * data);

#endif
