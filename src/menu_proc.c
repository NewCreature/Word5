#include <allegro5/allegro5.h>
#include "main.h"
#include "data.h"
#include "game.h"
#include "tutorial.h"
#include "title.h"
#include "leaderboard.h"

void lingo_menu_proc_main_play(void)
{
	lingo_current_menu = LINGO_MENU_ENTER_NAME;
	lingo_text_buffer_state = 1;
	lingo_text_buffer_pos = strlen(lingo_player[0].name);
	strcpy(lingo_menu[LINGO_MENU_ENTER_NAME].item[1].name, lingo_player[0].name);
	t3f_clear_chars();
}

static bool blank_string(char * string)
{
	int i;

	if(!string)
	{
		return true;
	}
	if(strlen(string) <= 0)
	{
		return true;
	}
	for(i = 0; i < strlen(string); i++)
	{
		if(string[i] >= 32 && string[i] < 127)
		{
			return false;
		}
	}
	return true;
}

void lingo_menu_proc_main_start_game(void)
{
//	lingo_stop_music();
	lingo_text_buffer_state = 0;
	lingo_game_start(LINGO_GAME_MODE_1P_SURVIVAL);

	/* set up transition */
	lingo_game_logo_y = 70;
	lingo_game_stats_x = -320;
	lingo_game_board_z = -640;
	lingo_game_menu[LINGO_GAME_MENU_MAIN].x = -320 - 150;
	lingo_game_menu[LINGO_GAME_MENU_MAIN].current_item = -1;
	lingo_game_menu[LINGO_GAME_MENU_OVER].x = -320 - 150;
	lingo_game_menu[LINGO_GAME_MENU_OVER].current_item = -1;
	lingo_game_menu[LINGO_GAME_MENU_MAIN_DUMMY].x = -320 - 150;
	lingo_game_menu[LINGO_GAME_MENU_MAIN_DUMMY].current_item = -1;
	lingo_state = LINGO_STATE_GAME_TRANSITION_IN;

	if(blank_string(lingo_player[0].name))
	{
		strcpy(lingo_player[0].name, "Player 1");
	}
	al_set_config_value(t3f_config, "Game", "Player Name", lingo_player[0].name);
}

void lingo_menu_proc_main_cancel(void)
{
	lingo_current_menu = LINGO_MENU_MAIN;
	lingo_text_buffer_state = 0;
}

void lingo_menu_proc_main_options(void)
{
	sprintf(lingo_menu[LINGO_MENU_OPTIONS].item[1].name, "%s", lingo_option[LINGO_OPTION_UPLOAD] ? "Yes" : "No");
	lingo_current_menu = LINGO_MENU_OPTIONS;
}

void lingo_menu_proc_main_leaderboard(void)
{
	if(lingo_leaderboard)
	{
		t3net_destroy_leaderboard(lingo_leaderboard);
	}
	al_stop_timer(t3f_timer);
	lingo_leaderboard = t3net_get_leaderboard("http://www.t3-i.com/t3net2/leaderboards/query.php", "word5", "1.2", "normal", "0", 10, 0);
	if(lingo_leaderboard)
	{
		lingo_leaderboard_place = -1;
		lingo_state = LINGO_STATE_LEADERBOARD;
		lingo_current_menu = LINGO_MENU_LEADERBOARD_VIEW;
	}
	al_start_timer(t3f_timer);
}

void lingo_menu_proc_main_tutorial(void)
{
//	lingo_stop_music();
	lingo_tutorial_start(LINGO_GAME_MODE_1P_SURVIVAL);

	/* set up transition */
	lingo_game_logo_y = 70;
	lingo_game_stats_x = -320;
	lingo_game_board_z = -640;
	lingo_game_menu[LINGO_GAME_MENU_MAIN].x = -320 - 150;
	lingo_game_menu[LINGO_GAME_MENU_MAIN].current_item = -1;
	lingo_game_menu[LINGO_GAME_MENU_OVER].x = -320 - 150;
	lingo_game_menu[LINGO_GAME_MENU_OVER].current_item = -1;
	lingo_state = LINGO_STATE_TUTORIAL_TRANSITION_IN;
}

void lingo_menu_proc_options_upload(void)
{
	if(lingo_option[LINGO_OPTION_UPLOAD])
	{
		lingo_option[LINGO_OPTION_UPLOAD] = 0;
	}
	else
	{
		lingo_option[LINGO_OPTION_UPLOAD] = 1;
	}
	sprintf(lingo_menu[LINGO_MENU_OPTIONS].item[1].name, "%s", lingo_option[LINGO_OPTION_UPLOAD] ? "Yes" : "No");
	al_set_config_value(t3f_config, "Game", "Upload Scores", lingo_option[LINGO_OPTION_UPLOAD] ? "Yes" : "No");
}

void lingo_menu_proc_options_back(void)
{
	lingo_current_menu = LINGO_MENU_MAIN;
}

void lingo_menu_proc_main_quit(void)
{
	lingo_state = LINGO_STATE_TITLE_TRANSITION_OUT;
//	lingo_quit = 1;
}

void lingo_menu_proc_game_main_bonus_letter(void)
{
	lingo_game_state = LINGO_GAME_STATE_P_BONUS_LETTER;
}

void lingo_menu_proc_game_main_quit(void)
{
	al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	lingo_state = LINGO_STATE_GAME_TRANSITION_OUT;
//	lingo_play_music("data/titlemusic.ogg");
}

void lingo_menu_proc_game_over_play_again(void)
{
	al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	lingo_game_start(LINGO_GAME_MODE_1P_SURVIVAL);
	lingo_state = LINGO_STATE_GAME;
}

void lingo_menu_proc_game_over_exit(void)
{
	al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	lingo_state = LINGO_STATE_GAME_TRANSITION_OUT;
}

void lingo_menu_proc_leaderboard_play_again(void)
{
	al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	lingo_game_start(LINGO_GAME_MODE_1P_SURVIVAL);
	lingo_state = LINGO_STATE_GAME;
}

void lingo_menu_proc_leaderboard_exit(void)
{
	al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	lingo_state = LINGO_STATE_GAME_TRANSITION_OUT;
}

void lingo_menu_proc_leaderboard_back(void)
{
	al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	lingo_state = LINGO_STATE_TITLE;
	lingo_current_menu = LINGO_MENU_MAIN;
}
