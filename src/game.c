#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "t3net/t3net.h"
#include "instance.h"
#include "main.h"
#include "player.h"
#include "menu_proc.h"
#include "game.h"
#include "leaderboard.h"

void lingo_game_shift_board_up(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i, j;

	for(i = 1; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
		{
			instance->gameboard[i - 1][j] = instance->gameboard[i][j];
			instance->gameboard_color[i - 1][j] = instance->gameboard_color[i][j];
		}
	}
	for(i = 0; i < 5; i++)
	{
		instance->gameboard[4][i] = 0;
		instance->gameboard_color[4][i] = 0;
	}
}

int lingo_game_give_bonus_letter(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int ccount = 0;
	int i;

	for(i = 0; i < 5; i++)
	{
		if(!instance->game_is_correct[i])
		{
			ccount++;
		}
	}
	if(ccount > 1)
	{
		for(i = 0; i < 5; i++)
		{
			if(!instance->game_is_correct[i])
			{
				instance->game_is_correct[i] = 1;
				break;
			}
		}
		return 1;
	}
	return 0;
}

void lingo_game_start(int mode, void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i, j;

	for(i = 0; i < 4; i++)
	{
		lingo_reset_player(&instance->player[i]);
	}
	instance->current_menu = LINGO_MENU_GAMEPLAY;
	instance->game_settings.mode = mode;
	switch(instance->game_settings.mode)
	{
		case LINGO_GAME_MODE_1P_SURVIVAL:
		{
			/* set up player */
			instance->game_settings.players = 1;
			instance->player[0].lives = 5;
			instance->player[0].bonus_letters = 5;
			instance->current_player = 0;

			/* set up board */
			strcpy(instance->last_word, "");
			for(i = 0; i < 6; i++)
			{
				for(j = 0; j < 6; j++)
				{
					instance->gameboard[i][j] = 0;
					instance->gameboard_color[i][j] = 0;
					instance->gameboard_font_color[i][j] = al_map_rgba(255, 255, 255, 255);
				}
			}
			strcpy(instance->word, instance->dictionary->word[lingo_dictionary_choose_word(instance->dictionary)].letter);
			instance->gameboard[0][0] = instance->word[0];
			for(i = 0; i < 5; i++)
			{
				instance->game_is_correct[i] = 0;
			}
			instance->game_is_correct[0] = 1;
			instance->game_ticker = 0;
			instance->game_state = LINGO_GAME_STATE_FIRST_LETTER;
			instance->game_clock = 1200;

			break;
		}

		case LINGO_GAME_MODE_2P_STANDARD:
		{
			/* set up player */
			instance->game_settings.players = 2;
			instance->current_player = 0;

			/* set up board */
			strcpy(instance->word, instance->dictionary->word[lingo_dictionary_choose_word(instance->dictionary)].letter);
			instance->gameboard[0][0] = instance->word[0];
//			instance->gameboard_color[0][0] = 1;
			for(i = 0; i < 5; i++)
			{
				instance->game_is_correct[i] = 0;
			}
			instance->game_is_correct[0] = 1;
			instance->game_ticker = 0;
			instance->game_state = LINGO_GAME_STATE_FIRST_LETTER;

			break;
		}
	}
}

void lingo_game_core_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i, j;

	switch(instance->game_state)
	{
		case LINGO_GAME_STATE_FIRST_LETTER:
		{
			t3f_clear_chars();
			al_play_sample(instance->sample[LINGO_SAMPLE_FIRST_LETTER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			instance->game_state = LINGO_GAME_STATE_TYPING;
			instance->game_ticker = 0;
			instance->game_clock = 1200;
			break;
		}
		case LINGO_GAME_STATE_TYPING:
		{

			instance->game_clock--;
			if(instance->game_clock == 300 || instance->game_clock == 240 || instance->game_clock == 180 || instance->game_clock == 120 || instance->game_clock == 60)
			{
				al_play_sample(instance->sample[LINGO_SAMPLE_CLOCK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			else if(instance->game_clock <= 0)
			{
				strcpy(instance->player[instance->current_player].word, "");
				instance->player[instance->current_player].word_pos = 0;
				al_play_sample(instance->sample[LINGO_SAMPLE_INCORRECT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				instance->player[instance->current_player].lives--;
				instance->game_check_current_guess = instance->player[instance->current_player].guess_count;
				instance->game_state = LINGO_GAME_STATE_SHOW_CORRECT;
				break;
			}

			/* read player input */
			instance->player[instance->current_player].letter = t3f_get_char(T3F_KEY_BUFFER_FORCE_UPPER);
			if(instance->player[instance->current_player].letter)
			{
				if(instance->player[instance->current_player].letter == '\b' || instance->player[instance->current_player].letter == 127)
				{
					if(instance->player[instance->current_player].word_pos > 0)
					{
						al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						instance->player[instance->current_player].word_pos--;
						instance->player[instance->current_player].word[instance->player[instance->current_player].word_pos] = '\0';
					}
				}
				else if(instance->player[instance->current_player].letter == '\r')
				{

					instance->game_check_pos = 0;
					instance->game_check_current_guess = instance->player[instance->current_player].guess_count;
					instance->player[instance->current_player].guess_count++;
					instance->game_check_correct = 0;
					for(i = 0; i < 5; i++)
					{
						instance->game_check_used[i] = 0;
					}
					instance->game_state = LINGO_GAME_STATE_CHECK_LETTERS;
					instance->game_ticker = 0;
					t3f_use_key_press(ALLEGRO_KEY_ENTER);
				}
				else if(instance->player[instance->current_player].letter == '\t')
				{
					lingo_menu_proc_game_main_bonus_letter(data);
					break;
				}
				else if(instance->player[instance->current_player].letter >= 'A' && instance->player[instance->current_player].letter <= 'Z')
				{
					if(instance->player[instance->current_player].word_pos < 5)
					{
						al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						instance->player[instance->current_player].word[instance->player[instance->current_player].word_pos] = instance->player[instance->current_player].letter;
						instance->player[instance->current_player].word_pos++;
						instance->player[instance->current_player].word[instance->player[instance->current_player].word_pos] = '\0';
					}
				}
			}
			break;
		}
		case LINGO_GAME_STATE_CHECK_LETTERS:
		{
			instance->game_ticker++;
			if(instance->game_ticker % 12 == 0)
			{

				/* check for real word before proceeding */
				if(instance->game_check_pos == 0)
				{
					/* wrong word */
					if(!lingo_dictionary_check_word(instance->dictionary, instance->player[instance->current_player].word))
					{
						strcpy(instance->player[instance->current_player].word, "");
						instance->player[instance->current_player].word_pos = 0;
						al_play_sample(instance->sample[LINGO_SAMPLE_INCORRECT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						instance->game_state = LINGO_GAME_STATE_P_INVALID;
						break;
					}
				}

				instance->gameboard[instance->game_check_current_guess][instance->game_check_pos] = instance->player[instance->current_player].word[instance->game_check_pos];

				/* correct letter */
				if(instance->player[instance->current_player].word[instance->game_check_pos] == instance->word[instance->game_check_pos])
				{
					instance->gameboard_color[instance->game_check_current_guess][instance->game_check_pos] = 1;
					instance->game_check_used[instance->game_check_pos] = 1;
					instance->game_is_correct[instance->game_check_pos] = 1;
					instance->game_check_correct++;
					al_play_sample(instance->sample[LINGO_SAMPLE_CORRECT_LETTER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else
				{
					char used[5] = {0};

					/* check if letter is somewhere in the puzzle */
					for(i = 0; i < 5; i++)
					{
						if(instance->player[instance->current_player].word[i] == instance->word[i])
						{
							used[i] = 1;
						}
					}
					int yp = 0;
					for(i = 0; i < 5; i++)
					{
						if(!instance->game_check_used[i] && !used[i] && instance->player[instance->current_player].word[instance->game_check_pos] != instance->word[instance->game_check_pos] && instance->player[instance->current_player].word[instance->game_check_pos] == instance->word[i])
						{
							instance->gameboard_color[instance->game_check_current_guess][instance->game_check_pos] = 2;
							instance->game_check_used[i] = 1;
							al_play_sample(instance->sample[LINGO_SAMPLE_YELLOW_LETTER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							yp = 1;
						}
					}
					if(!yp)
					{
						al_play_sample(instance->sample[LINGO_SAMPLE_INCORRECT_LETTER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				instance->game_check_pos++;

				/* done checking */
				if(instance->game_check_pos >= 5)
				{
					if(instance->game_check_correct >= 5)
					{
						instance->player[instance->current_player].score += 25;
						instance->game_state = LINGO_GAME_STATE_P_CORRECT;
						instance->game_ticker = 0;
					}
					else
					{
						instance->game_state = LINGO_GAME_STATE_P_INCORRECT;
					}
				}
			}
			break;
		}
		case LINGO_GAME_STATE_SHOW_CORRECT:
		{
			instance->game_ticker++;
			if(instance->game_ticker == 5)
			{
				al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				instance->gameboard_font_color[instance->game_check_current_guess][0] = al_map_rgba(255, 244, 141, 255);
				instance->gameboard[instance->game_check_current_guess][0] = instance->word[0];
			}
			if(instance->game_ticker == 10)
			{
				al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				instance->gameboard_font_color[instance->game_check_current_guess][1] = al_map_rgba(255, 244, 141, 255);
				instance->gameboard[instance->game_check_current_guess][1] = instance->word[1];
			}
			if(instance->game_ticker == 15)
			{
				al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				instance->gameboard_font_color[instance->game_check_current_guess][2] = al_map_rgba(255, 244, 141, 255);
				instance->gameboard[instance->game_check_current_guess][2] = instance->word[2];
			}
			if(instance->game_ticker == 20)
			{
				al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				instance->gameboard_font_color[instance->game_check_current_guess][3] = al_map_rgba(255, 244, 141, 255);
				instance->gameboard[instance->game_check_current_guess][3] = instance->word[3];
			}
			if(instance->game_ticker == 25)
			{
				al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				instance->gameboard_font_color[instance->game_check_current_guess][4] = al_map_rgba(255, 244, 141, 255);
				instance->gameboard[instance->game_check_current_guess][4] = instance->word[4];
			}
			if(instance->game_ticker == 15)
			{
				instance->gameboard_font_color[instance->game_check_current_guess][0] = al_map_rgba(255, 255, 255, 255);
			}
			if(instance->game_ticker == 20)
			{
				instance->gameboard_font_color[instance->game_check_current_guess][1] = al_map_rgba(255, 255, 255, 255);
			}
			if(instance->game_ticker == 25)
			{
				instance->gameboard_font_color[instance->game_check_current_guess][2] = al_map_rgba(255, 255, 255, 255);
			}
			if(instance->game_ticker == 30)
			{
				instance->gameboard_font_color[instance->game_check_current_guess][3] = al_map_rgba(255, 255, 255, 255);
			}
			if(instance->game_ticker == 35)
			{
				instance->gameboard_font_color[instance->game_check_current_guess][4] = al_map_rgba(255, 255, 255, 255);
			}
			if(instance->game_ticker > 90)
			{
				if(instance->player[instance->current_player].lives <= 0)
				{
					if(instance->option[LINGO_OPTION_UPLOAD])
					{
						al_stop_timer(t3f_timer);
						if(lingo_get_leaderboard(data))
						{
							instance->leaderboard_place = -1;
							for(i = 0; i < instance->leaderboard->entries; i++)
							{
								if(lingo_obfuscate_score(instance->player[instance->current_player].score) == instance->leaderboard->entry[i]->score && !strcmp(instance->player[instance->current_player].name, instance->leaderboard->entry[i]->name))
								{
									instance->leaderboard_place = i;
									break;
								}
							}
							instance->state = LINGO_STATE_LEADERBOARD;
							instance->current_menu = LINGO_MENU_LEADERBOARD;
						}
						else
						{
							instance->game_state = LINGO_GAME_STATE_OVER;
							instance->current_menu = LINGO_MENU_GAME_OVER;
						}
						al_start_timer(t3f_timer);
					}
					else
					{
						instance->game_state = LINGO_GAME_STATE_OVER;
						instance->current_menu = LINGO_MENU_GAME_OVER;
					}
				}
				else
				{
					strcpy(instance->last_word, instance->word);
					strcpy(instance->word, instance->dictionary->word[lingo_dictionary_choose_word(instance->dictionary)].letter);
					instance->player[instance->current_player].guess_count = 0;
					for(i = 0; i < 5; i++)
					{
						for(j = 0; j < 5; j++)
						{
							instance->gameboard[i][j] = 0;
							instance->gameboard_color[i][j] = 0;
							instance->gameboard_font_color[i][j] = al_map_rgba(255, 255, 255, 255);
						}
						instance->game_is_correct[i] = 0;
					}
					instance->game_is_correct[0] = 1;
					instance->gameboard[0][0] = instance->word[0];
					if(instance->player[instance->current_player].score > instance->high_score)
					{
						instance->high_score = instance->player[instance->current_player].score;
					}
					strcpy(instance->player[instance->current_player].word, "");
					instance->player[instance->current_player].word_pos = 0;
					instance->game_state = LINGO_GAME_STATE_FIRST_LETTER;
				}
			}
			break;
		}
	}
}

void lingo_game_mode_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i, b;

	switch(instance->game_state)
	{
		case LINGO_GAME_STATE_P_BONUS_LETTER:
		{
			switch(instance->game_settings.mode)
			{
				case LINGO_GAME_MODE_1P_SURVIVAL:
				{
					if(instance->player[instance->current_player].bonus_letters > 0)
					{
						b = lingo_game_give_bonus_letter(data);
						for(i = 0; i < 5; i++)
						{
							if(instance->game_is_correct[i])
							{
								instance->gameboard[instance->player[instance->current_player].guess_count][i] = instance->word[i];
							}
						}
						instance->game_state = LINGO_GAME_STATE_TYPING;
						if(b)
						{
							al_play_sample(instance->sample[LINGO_SAMPLE_HINT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							instance->player[instance->current_player].bonus_letters--;
							instance->game_ticker = 0;
							instance->game_clock = 1200;
						}
					}
					break;
				}
			}
			instance->game_state = LINGO_GAME_STATE_TYPING;
			instance->game_ticker = 0;
			break;
		}
		case LINGO_GAME_STATE_P_INVALID:
		{
			switch(instance->game_settings.mode)
			{
				case LINGO_GAME_MODE_1P_SURVIVAL:
				{
					instance->player[0].lives--;
					if(instance->player[0].lives <= 0)
					{
						if(instance->game_check_current_guess >= 4)
						{
							lingo_game_shift_board_up(data);
						}
						else
						{
							instance->game_check_current_guess++;
						}
						instance->game_state = LINGO_GAME_STATE_SHOW_CORRECT;
						instance->game_ticker = -30;
						instance->current_menu = LINGO_MENU_GAMEPLAY_DUMMY;
					}
					else
					{
						instance->player[instance->current_player].guess_count--;
						for(i = 0; i < 5; i++)
						{
							if(instance->game_is_correct[i])
							{
								instance->gameboard[instance->player[instance->current_player].guess_count][i] = instance->word[i];
							}
							else
							{
								instance->gameboard[instance->player[instance->current_player].guess_count][i] = '.';
							}
						}
						instance->game_state = LINGO_GAME_STATE_TYPING;
						instance->game_clock = 1200;
					}
					instance->game_ticker = 0;
					break;
				}
				case LINGO_GAME_MODE_2P_STANDARD:
				{
					lingo_game_give_bonus_letter(data);
					instance->player[instance->current_player].guess_count--;
					for(i = 0; i < 5; i++)
					{
						if(instance->game_is_correct[i])
						{
							instance->gameboard[instance->player[instance->current_player].guess_count][i] = instance->word[i];
						}
						else
						{
							instance->gameboard[instance->player[instance->current_player].guess_count][i] = '.';
						}
					}
					instance->current_player = 1 - instance->current_player;
					strcpy(instance->player[instance->current_player].word, "");
					instance->player[instance->current_player].word_pos = 0;
					instance->game_state = LINGO_GAME_STATE_TYPING;
					instance->game_ticker = 0;
					instance->game_clock = 1200;
					break;
				}
			}
			break;
		}
		case LINGO_GAME_STATE_P_CORRECT:
		{
			switch(instance->game_settings.mode)
			{
				case LINGO_GAME_MODE_1P_SURVIVAL:
				{
					al_play_sample(instance->sample[LINGO_SAMPLE_CORRECT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					instance->game_state = LINGO_GAME_STATE_SHOW_CORRECT;
					break;
				}
				case LINGO_GAME_MODE_2P_STANDARD:
				{
					instance->game_state = LINGO_GAME_STATE_FIRST_LETTER;
					break;
				}
			}
			break;
		}
		case LINGO_GAME_STATE_P_INCORRECT:
		{
			switch(instance->game_settings.mode)
			{
				case LINGO_GAME_MODE_1P_SURVIVAL:
				{
					if(instance->player[instance->current_player].guess_count >= 5)
					{
						instance->player[instance->current_player].lives--;
						if(instance->player[instance->current_player].lives <= 0)
						{
							lingo_game_shift_board_up(data);
							al_play_sample(instance->sample[LINGO_SAMPLE_INCORRECT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							instance->game_state = LINGO_GAME_STATE_SHOW_CORRECT;
							instance->current_menu = LINGO_MENU_GAMEPLAY_DUMMY;
							instance->game_ticker = -30;
							break;
						}

						lingo_game_shift_board_up(data);
						al_play_sample(instance->sample[LINGO_SAMPLE_INCORRECT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						instance->game_state = LINGO_GAME_STATE_SHOW_CORRECT;
						instance->game_ticker = -30;
					}
					else
					{
						strcpy(instance->player[instance->current_player].word, "");
						instance->player[instance->current_player].word_pos = 0;
						for(i = 0; i < 5; i++)
						{
							if(instance->game_is_correct[i] == 1)
							{
								instance->gameboard[instance->player[instance->current_player].guess_count][i] = instance->word[i];
							}
							else
							{
								instance->gameboard[instance->player[instance->current_player].guess_count][i] = '.';
							}
						}
						instance->game_state = LINGO_GAME_STATE_TYPING;
						instance->game_ticker = 0;
						instance->game_clock = 1200;
					}
					break;
				}
				case LINGO_GAME_MODE_2P_STANDARD:
				{
					if(instance->player[instance->current_player].guess_count >= 5)
					{
						strcpy(instance->player[instance->current_player].word, "");
						instance->player[instance->current_player].word_pos = 0;
						lingo_game_shift_board_up(data);
						lingo_game_give_bonus_letter(data);

						/* switch players */
						if(instance->game_settings.players == 2)
						{
							instance->current_player = 1 - instance->current_player;
							strcpy(instance->player[instance->current_player].word, "");
							instance->player[instance->current_player].word_pos = 0;
							instance->player[instance->current_player].guess_count = 4;
						}
						for(i = 0; i < 5; i++)
						{
							if(instance->game_is_correct[i] == 1)
							{
								instance->gameboard[instance->player[instance->current_player].guess_count][i] = instance->word[i];
							}
							else
							{
								instance->gameboard[instance->player[instance->current_player].guess_count][i] = '.';
							}
						}
						instance->game_state = LINGO_GAME_STATE_TYPING;
						instance->game_ticker = 0;
						instance->game_clock = 1200;
					}
					else
					{
						strcpy(instance->player[instance->current_player].word, "");
						instance->player[instance->current_player].word_pos = 0;
						for(i = 0; i < 5; i++)
						{
							if(instance->game_is_correct[i] == 1)
							{
								instance->gameboard[instance->player[instance->current_player].guess_count][i] = instance->word[i];
							}
							else
							{
								instance->gameboard[instance->player[instance->current_player].guess_count][i] = '.';
							}
						}
						instance->game_state = LINGO_GAME_STATE_TYPING;
						instance->game_ticker = 0;
						instance->game_clock = 1200;
					}
					break;
				}
			}
			break;
		}
	}
}

void lingo_game_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i;
	int mx, mex, ilen;

	if(t3f_key_pressed(ALLEGRO_KEY_ESCAPE))
	{
		lingo_menu_proc_game_main_quit(data);
		t3f_use_key_press(ALLEGRO_KEY_ESCAPE);
	}
	else
	{
		lingo_title_menu_mouse_logic(&instance->menu[instance->current_menu], data);
		lingo_game_core_logic(data);
		lingo_game_mode_logic(data);
	}
}

void lingo_game_transition_out_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int done = 1;
	if(instance->game_logo_y < 70)
	{
		instance->game_logo_y++;
		done = 0;
	}
	if(instance->game_stats_x > -320)
	{
		instance->game_stats_x -= 14;
		instance->menu[LINGO_MENU_GAMEPLAY].x -= 14;
		instance->menu[LINGO_MENU_GAME_OVER].x -= 14;
		instance->menu[LINGO_MENU_GAMEPLAY_DUMMY].x -= 14;
		done = 0;
	}
	if(instance->game_board_z > -640)
	{
		instance->game_board_z -= 20;
		done = 0;
	}
	if(done)
	{
		instance->state = LINGO_STATE_TITLE;
		instance->current_menu = LINGO_MENU_MAIN;
	}
}

void lingo_game_transition_in_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int done = 1;
	if(instance->game_logo_y > 35)
	{
		instance->game_logo_y--;
		done = 0;
	}
	if(instance->game_stats_x < 150)
	{
		instance->game_stats_x += 14;
		instance->menu[LINGO_MENU_GAMEPLAY].x += 14;
		instance->menu[LINGO_MENU_GAME_OVER].x += 14;
		instance->menu[LINGO_MENU_GAMEPLAY_DUMMY].x += 14;
		done = 0;
	}
	if(instance->game_board_z < 0)
	{
		instance->game_board_z += 20;
		done = 0;
	}
	if(done)
	{
//		al_play_sample(instance->sample[LINGO_SAMPLE_LOGO], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		instance->state = LINGO_STATE_GAME;
	}
}

void lingo_game_render(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	char buf[16];
	int i, j, x, y;
	ALLEGRO_COLOR col[2];
	ALLEGRO_COLOR gcol[2];
	char * text;
	int tw, tnw;
	int mx;
	float a;

	t3f_select_view(t3f_default_view);
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_BG], t3f_color_white, 0, 0, 0, 0);
	lingo_select_view(data);
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_LOGO], t3f_color_white, 320 - instance->image[LINGO_IMAGE_LOGO]->target_width / 2.0, instance->game_logo_y, 0, 0);
	a = 1.0 + instance->game_board_z / 640.0;
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_GAMEBOARD], al_map_rgba_f(a, a, a, a), LINGO_GAMEBOARD_X_OFFSET, LINGO_GAMEBOARD_Y_OFFSET, instance->game_board_z, 0);

	if(instance->current_player == 0)
	{
		col[0] = al_map_rgba(255, 255, 255, 255);
		gcol[0] = al_map_rgba(96, 255, 96, 255);
		col[1] = al_map_rgba(128, 128, 128, 255);
		gcol[1] = al_map_rgba(0, 128, 0, 255);
	}
	else
	{
		col[0] = al_map_rgba(128, 128, 128, 255);
		gcol[0] = al_map_rgba(0, 128, 0, 255);
		col[1] = al_map_rgba(255, 255, 255, 255);
		gcol[1] = al_map_rgba(0, 255, 0, 255);
	}

	/* draw the score */
	lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x + 2, 170 + 2, al_map_rgba(0, 0, 0, 128), "Score");
	lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x, 170, gcol[0], "Score");
	sprintf(buf, "%04d", instance->player[0].score);
	lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x + 2, 170 + 24 + 2, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x, 170 + 24, col[0], buf);

	/* draw hints */
	sprintf(buf, "Hints: %d", instance->player[0].bonus_letters);
	tw = t3f_get_text_width(instance->font[LINGO_FONT_SPRINT_10], buf);
	sprintf(buf, "%d", instance->player[0].bonus_letters);
	tnw = t3f_get_text_width(instance->font[LINGO_FONT_SPRINT_10], buf);
	sprintf(buf, "Hints:");
	lingo_draw_text(instance->font[LINGO_FONT_SPRINT_10], instance->game_stats_x + 1 - tw / 2, 170 + 60 + 1, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text(instance->font[LINGO_FONT_SPRINT_10], instance->game_stats_x - tw / 2, 170 + 60, gcol[0], buf);
	sprintf(buf, "%d", instance->player[0].bonus_letters);
	lingo_draw_text(instance->font[LINGO_FONT_SPRINT_10], instance->game_stats_x + 1 - tw / 2 + (tw - tnw), 170 + 60 + 1, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text(instance->font[LINGO_FONT_SPRINT_10], instance->game_stats_x - tw / 2 + (tw - tnw), 170 + 60, col[0], buf);

	/* draw lives */
	sprintf(buf, "Lives: %d", instance->player[0].lives);
	tw = t3f_get_text_width(instance->font[LINGO_FONT_SPRINT_10], buf);
	sprintf(buf, "%d", instance->player[0].lives);
	tnw = t3f_get_text_width(instance->font[LINGO_FONT_SPRINT_10], buf);
	sprintf(buf, "Lives:");
	lingo_draw_text(instance->font[LINGO_FONT_SPRINT_10], instance->game_stats_x + 1 - tw / 2, 170 + 72 + 1, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text(instance->font[LINGO_FONT_SPRINT_10], instance->game_stats_x - tw / 2, 170 + 72, gcol[0], buf);
	sprintf(buf, "%d", instance->player[0].lives);
	lingo_draw_text(instance->font[LINGO_FONT_SPRINT_10], instance->game_stats_x + 1 - tw / 2 + (tw - tnw), 170 + 72 + 1, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text(instance->font[LINGO_FONT_SPRINT_10], instance->game_stats_x - tw / 2 + (tw - tnw), 170 + 72, col[0], buf);

	/* draw high score */
	lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x + 2, 170 + 94 + 2, al_map_rgba(0, 0, 0, 128), "High");
	lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x, 170 + 94, gcol[0], "High");
	sprintf(buf, "%04d", instance->high_score);
	lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x + 2, 170 + 94 + 24 + 2, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x, 170 + 94 + 24, col[0], buf);

	/* draw current guess */
	if(instance->game_state != LINGO_GAME_STATE_OVER && instance->current_menu != LINGO_MENU_GAME_OVER)
	{
		lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x + 2, 170 + 118 + 12 + 50 + 2, al_map_rgba(0, 0, 0, 128), "Guess");
		lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x, 170 + 118 + 12 + 50, gcol[0], "Guess");
		strcpy(buf, instance->player[instance->current_player].word);
		lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x + 2, 170 + 118 + 39 + 50 + 2, al_map_rgba(0, 0, 0, 128), buf);
		lingo_draw_text_center(instance->font[LINGO_FONT_SPRINT_20], instance->game_stats_x, 170 + 118 + 39 + 50, al_map_rgba(255, 255, 0, 255), buf);
	}

	/* draw clock */
	if(instance->state == LINGO_STATE_GAME && instance->game_state == LINGO_GAME_STATE_TYPING)
	{
		sprintf(buf, "%2d", (instance->game_clock + 59) / 60);
		if(instance->game_clock <= 300)
		{
			lingo_draw_text(instance->font[LINGO_FONT_SPRINT_20], 596 + 2, 444 + 2, al_map_rgba(0, 0, 0, 128), buf);
			lingo_draw_text(instance->font[LINGO_FONT_SPRINT_20], 596, 444, al_map_rgba(255, 0, 0, 255), buf);
		}
		else
		{
			lingo_draw_text(instance->font[LINGO_FONT_SPRINT_20], 592 + 2, 444 + 2, al_map_rgba(0, 0, 0, 128), buf);
			lingo_draw_text(instance->font[LINGO_FONT_SPRINT_20], 592, 444, col[0], buf);
		}
	}

	/* draw the game board */
	if(instance->state == LINGO_STATE_GAME)
	{
		for(i = 0; i < 5; i++)
		{
			for(j = 0; j < 5; j++)
			{
				if(instance->gameboard[i][j])
				{
					buf[0] = instance->gameboard[i][j];
					buf[1] = '\0';
					if(instance->gameboard_color[i][j] == 1)
					{
						t3f_draw_bitmap(instance->image[LINGO_IMAGE_RED_SQUARE], t3f_color_white, LINGO_GAMEBOARD_X_OFFSET + j * 56 + 1, LINGO_GAMEBOARD_Y_OFFSET + i * 56 + 1, 0.0, 0);
					}
					else if(instance->gameboard_color[i][j] == 2)
					{
						t3f_draw_bitmap(instance->image[LINGO_IMAGE_YELLOW_CIRCLE], t3f_color_white, LINGO_GAMEBOARD_X_OFFSET + j * 56 + 1, LINGO_GAMEBOARD_Y_OFFSET + i * 56 + 1, 0.0, 0);
					}
					lingo_draw_text_center(instance->font[LINGO_FONT_ARIAL_36], LINGO_GAMEBOARD_X_OFFSET + j * 56 + 1 + 28 + 2, LINGO_GAMEBOARD_Y_OFFSET + i * 56 + 1 + LINGO_GAME_BOARD_TEXT_OFFSET_Y + 2, al_map_rgba(0, 0, 0, 128), buf);
					lingo_draw_text_center(instance->font[LINGO_FONT_ARIAL_36], LINGO_GAMEBOARD_X_OFFSET + j * 56 + 1 + 28, LINGO_GAMEBOARD_Y_OFFSET + i * 56 + 1 + LINGO_GAME_BOARD_TEXT_OFFSET_Y, instance->gameboard_font_color[i][j], buf);
				}
			}
		}
	}

	/* draw the in-game menu */
	lingo_menu_render(data);
}
