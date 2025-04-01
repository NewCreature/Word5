#include "t3f/t3f.h"
#include "instance.h"
#include "main.h"
#include "game.h"
#include "tutorial.h"
#include "title.h"
#include "leaderboard.h"

void lingo_menu_proc_main_play(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	instance->current_menu = LINGO_MENU_ENTER_NAME;
	instance->text_buffer_state = 1;
	instance->text_buffer_pos = strlen(instance->player[0].name);
	strcpy(instance->menu[LINGO_MENU_ENTER_NAME].item[1].name, instance->player[0].name);
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

void lingo_menu_proc_main_start_game(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

//	instance->stop_music();
	instance->text_buffer_state = 0;
	lingo_game_start(LINGO_GAME_MODE_1P_SURVIVAL, data);

	/* set up transition */
	instance->game_logo_y = LINGO_LOGO_POS_Y;
	instance->game_stats_x = -320;
	instance->game_board_z = -640;
	instance->menu[LINGO_MENU_GAMEPLAY].x = -320 - 150;
	instance->menu[LINGO_MENU_GAMEPLAY].current_item = -1;
	instance->menu[LINGO_MENU_GAME_OVER].x = -320 - 150;
	instance->menu[LINGO_MENU_GAME_OVER].current_item = -1;
	instance->menu[LINGO_MENU_GAMEPLAY_DUMMY].x = -320 - 150;
	instance->menu[LINGO_MENU_GAMEPLAY_DUMMY].current_item = -1;
	instance->state = LINGO_STATE_GAME_TRANSITION_IN;

	if(blank_string(instance->player[0].name))
	{
		strcpy(instance->player[0].name, "Player 1");
	}
	al_set_config_value(t3f_config, "Game", "Player Name", instance->player[0].name);
}

void lingo_menu_proc_main_cancel(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	instance->current_menu = LINGO_MENU_MAIN;
	instance->text_buffer_state = 0;
}

void lingo_menu_proc_main_options(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	sprintf(instance->menu[LINGO_MENU_OPTIONS].item[1].name, "%s", instance->option[LINGO_OPTION_UPLOAD] ? "Yes" : "No");
	instance->current_menu = LINGO_MENU_OPTIONS;
}

void lingo_menu_proc_main_leaderboard(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	al_stop_timer(t3f_timer);
	if(lingo_get_leaderboard(data))
	{
		instance->leaderboard_place = -1;
		instance->state = LINGO_STATE_LEADERBOARD;
		instance->current_menu = LINGO_MENU_LEADERBOARD_VIEW;
	}
	al_start_timer(t3f_timer);
}

void lingo_menu_proc_main_tutorial(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	//	instance->stop_music();
	lingo_tutorial_start(LINGO_GAME_MODE_1P_SURVIVAL, data);

	/* set up transition */
	instance->game_logo_y = LINGO_LOGO_POS_Y;
	instance->game_stats_x = -320;
	instance->game_board_z = -640;
	instance->menu[LINGO_MENU_GAMEPLAY].x = -320 - 150;
	instance->menu[LINGO_MENU_GAMEPLAY].current_item = -1;
	instance->menu[LINGO_MENU_GAME_OVER].x = -320 - 150;
	instance->menu[LINGO_MENU_GAME_OVER].current_item = -1;
	instance->state = LINGO_STATE_TUTORIAL_TRANSITION_IN;
}

void lingo_menu_proc_main_credits(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	if(lingo_setup_credits(data))
	{
		lingo_start_credits(instance->credits, 0, instance->view->virtual_height, instance->view->virtual_width / 2.0, instance->view->virtual_height, -1.0);
		instance->state = LINGO_STATE_CREDITS;
	}
}

void lingo_menu_proc_options_upload(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	if(instance->option[LINGO_OPTION_UPLOAD])
	{
		instance->option[LINGO_OPTION_UPLOAD] = 0;
	}
	else
	{
		instance->option[LINGO_OPTION_UPLOAD] = 1;
	}
	sprintf(instance->menu[LINGO_MENU_OPTIONS].item[1].name, "%s", instance->option[LINGO_OPTION_UPLOAD] ? "Yes" : "No");
	al_set_config_value(t3f_config, "Game", "Upload Scores", instance->option[LINGO_OPTION_UPLOAD] ? "Yes" : "No");
}

void lingo_menu_proc_options_back(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	instance->current_menu = LINGO_MENU_MAIN;
}

void lingo_menu_proc_main_quit(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	instance->state = LINGO_STATE_TITLE_TRANSITION_OUT;
//	instance->quit = 1;
}

void lingo_menu_proc_game_main_bonus_letter(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	instance->game_state = LINGO_GAME_STATE_P_BONUS_LETTER;
}

void lingo_menu_proc_game_main_quit(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	al_play_sample(instance->sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	instance->state = LINGO_STATE_GAME_TRANSITION_OUT;
//	instance->play_music("data/titlemusic.ogg");
}

void lingo_menu_proc_game_over_play_again(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	al_play_sample(instance->sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	lingo_game_start(LINGO_GAME_MODE_1P_SURVIVAL, data);
	instance->state = LINGO_STATE_GAME;
}

void lingo_menu_proc_game_over_exit(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	al_play_sample(instance->sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	instance->state = LINGO_STATE_GAME_TRANSITION_OUT;
}

void lingo_menu_proc_leaderboard_play_again(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	al_play_sample(instance->sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	lingo_game_start(LINGO_GAME_MODE_1P_SURVIVAL, data);
	instance->state = LINGO_STATE_GAME;
}

void lingo_menu_proc_leaderboard_exit(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	t3net_destroy_leaderboard(instance->leaderboard);
	instance->leaderboard = NULL;
	al_play_sample(instance->sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	instance->state = LINGO_STATE_GAME_TRANSITION_OUT;
}

void lingo_menu_proc_leaderboard_back(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	t3net_destroy_leaderboard(instance->leaderboard);
	instance->leaderboard = NULL;
	al_play_sample(instance->sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	instance->state = LINGO_STATE_TITLE;
	instance->current_menu = LINGO_MENU_MAIN;
}
