#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "t3net/t3net.h"
#include "main.h"
#include "data.h"
#include "player.h"
#include "menu_proc.h"
#include "game.h"
#include "leaderboard.h"

LINGO_PLAYER lingo_player[LINGO_MAX_PLAYERS];
char lingo_word[16] = {0};
char lingo_last_word[16] = {0};
char lingo_gameboard[6][6] = {{0}};
char lingo_gameboard_color[6][6] = {{0}};
ALLEGRO_COLOR lingo_gameboard_font_color[6][6];
int lingo_high_score = 0;

int lingo_current_player = 0;
int lingo_game_ticker;
int lingo_game_state;
int lingo_game_clock;

int lingo_game_check_pos;
int lingo_game_check_current_guess;
int lingo_game_check_correct;
char lingo_game_check_used[5];
char lingo_game_is_correct[5];

/* transition data */
int lingo_game_logo_y;
int lingo_game_stats_x;
int lingo_game_board_z;

void lingo_game_shift_board_up(void)
{
	int i, j;

	for(i = 1; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
		{
			lingo_gameboard[i - 1][j] = lingo_gameboard[i][j];
			lingo_gameboard_color[i - 1][j] = lingo_gameboard_color[i][j];
		}
	}
	for(i = 0; i < 5; i++)
	{
		lingo_gameboard[4][i] = 0;
		lingo_gameboard_color[4][i] = 0;
	}
}

int lingo_game_give_bonus_letter(void)
{
	int ccount = 0;
	int i;

	for(i = 0; i < 5; i++)
	{
		if(!lingo_game_is_correct[i])
		{
			ccount++;
		}
	}
	if(ccount > 1)
	{
		for(i = 0; i < 5; i++)
		{
			if(!lingo_game_is_correct[i])
			{
				lingo_game_is_correct[i] = 1;
				break;
			}
		}
		return 1;
	}
	return 0;
}

void lingo_game_start(int mode)
{
	int i, j;

	for(i = 0; i < 4; i++)
	{
		lingo_reset_player(&lingo_player[i]);
	}
	lingo_current_game_menu = LINGO_GAME_MENU_MAIN;
	lingo_game_settings.mode = mode;
	switch(lingo_game_settings.mode)
	{
		case LINGO_GAME_MODE_1P_SURVIVAL:
		{
			/* set up player */
			lingo_game_settings.players = 1;
			lingo_player[0].chances = 5;
			lingo_player[0].bonus_letters = 5;
			lingo_current_player = 0;

			/* set up board */
			strcpy(lingo_last_word, "");
			for(i = 0; i < 6; i++)
			{
				for(j = 0; j < 6; j++)
				{
					lingo_gameboard[i][j] = 0;
					lingo_gameboard_color[i][j] = 0;
					lingo_gameboard_font_color[i][j] = al_map_rgba(255, 255, 255, 255);
				}
			}
			strcpy(lingo_word, lingo_dictionary->word[lingo_dictionary_choose_word(lingo_dictionary)].letter);
			lingo_gameboard[0][0] = lingo_word[0];
			for(i = 0; i < 5; i++)
			{
				lingo_game_is_correct[i] = 0;
			}
			lingo_game_is_correct[0] = 1;
			lingo_game_ticker = 0;
			lingo_game_state = LINGO_GAME_STATE_FIRST_LETTER;
			lingo_game_clock = 1200;

			break;
		}

		case LINGO_GAME_MODE_2P_STANDARD:
		{
			/* set up player */
			lingo_game_settings.players = 2;
			lingo_current_player = 0;

			/* set up board */
			strcpy(lingo_word, lingo_dictionary->word[lingo_dictionary_choose_word(lingo_dictionary)].letter);
			lingo_gameboard[0][0] = lingo_word[0];
//			lingo_gameboard_color[0][0] = 1;
			for(i = 0; i < 5; i++)
			{
				lingo_game_is_correct[i] = 0;
			}
			lingo_game_is_correct[0] = 1;
			lingo_game_ticker = 0;
			lingo_game_state = LINGO_GAME_STATE_FIRST_LETTER;

			break;
		}
	}
}

void lingo_game_core_logic(void)
{
	int i, j;

	switch(lingo_game_state)
	{
		case LINGO_GAME_STATE_FIRST_LETTER:
		{
			t3f_clear_chars();
			al_play_sample(lingo_sample[LINGO_SAMPLE_FIRST_LETTER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			lingo_game_state = LINGO_GAME_STATE_TYPING;
			lingo_game_ticker = 0;
			lingo_game_clock = 1200;
			break;
		}
		case LINGO_GAME_STATE_TYPING:
		{

			lingo_game_clock--;
			if(lingo_game_clock == 300 || lingo_game_clock == 240 || lingo_game_clock == 180 || lingo_game_clock == 120 || lingo_game_clock == 60)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_CLOCK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			else if(lingo_game_clock <= 0)
			{
				strcpy(lingo_player[lingo_current_player].word, "");
				lingo_player[lingo_current_player].word_pos = 0;
				al_play_sample(lingo_sample[LINGO_SAMPLE_INCORRECT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_player[lingo_current_player].chances--;
				lingo_game_check_current_guess = lingo_player[lingo_current_player].guess_count;
				lingo_game_state = LINGO_GAME_STATE_SHOW_CORRECT;
				break;
			}

			/* read player input */
			lingo_player[lingo_current_player].letter = t3f_get_char(T3F_KEY_BUFFER_FORCE_UPPER);
			if(lingo_player[lingo_current_player].letter)
			{
				if(lingo_player[lingo_current_player].letter == '\b' || lingo_player[lingo_current_player].letter == 127)
				{
					if(lingo_player[lingo_current_player].word_pos > 0)
					{
						al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						lingo_player[lingo_current_player].word_pos--;
						lingo_player[lingo_current_player].word[lingo_player[lingo_current_player].word_pos] = '\0';
					}
				}
				else if(lingo_player[lingo_current_player].letter == '\r')
				{

					lingo_game_check_pos = 0;
					lingo_game_check_current_guess = lingo_player[lingo_current_player].guess_count;
					lingo_player[lingo_current_player].guess_count++;
					lingo_game_check_correct = 0;
					for(i = 0; i < 5; i++)
					{
						lingo_game_check_used[i] = 0;
					}
					lingo_game_state = LINGO_GAME_STATE_CHECK_LETTERS;
					lingo_game_ticker = 0;
				}
				else if(lingo_player[lingo_current_player].letter == '\t')
				{
					lingo_menu_proc_game_main_bonus_letter();
//					lingo_game_state = LINGO_GAME_STATE_P_BONUS_LETTER;
					break;
				}
				else if(lingo_player[lingo_current_player].letter >= 'A' && lingo_player[lingo_current_player].letter <= 'Z')
				{
					if(lingo_player[lingo_current_player].word_pos < 5)
					{
						al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						lingo_player[lingo_current_player].word[lingo_player[lingo_current_player].word_pos] = lingo_player[lingo_current_player].letter;
						lingo_player[lingo_current_player].word_pos++;
						lingo_player[lingo_current_player].word[lingo_player[lingo_current_player].word_pos] = '\0';
					}
				}
			}
			break;
		}
		case LINGO_GAME_STATE_CHECK_LETTERS:
		{
			lingo_game_ticker++;
			if(lingo_game_ticker % 12 == 0)
			{

				/* check for real word before proceeding */
				if(lingo_game_check_pos == 0)
				{
					/* wrong word */
					if(!lingo_dictionary_check_word(lingo_dictionary, lingo_player[lingo_current_player].word))
					{
						strcpy(lingo_player[lingo_current_player].word, "");
						lingo_player[lingo_current_player].word_pos = 0;
						al_play_sample(lingo_sample[LINGO_SAMPLE_INCORRECT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						lingo_game_state = LINGO_GAME_STATE_P_INVALID;
						break;
					}
				}

				lingo_gameboard[lingo_game_check_current_guess][lingo_game_check_pos] = lingo_player[lingo_current_player].word[lingo_game_check_pos];

				/* correct letter */
				if(lingo_player[lingo_current_player].word[lingo_game_check_pos] == lingo_word[lingo_game_check_pos])
				{
					lingo_gameboard_color[lingo_game_check_current_guess][lingo_game_check_pos] = 1;
					lingo_game_check_used[lingo_game_check_pos] = 1;
					lingo_game_is_correct[lingo_game_check_pos] = 1;
					lingo_game_check_correct++;
					al_play_sample(lingo_sample[LINGO_SAMPLE_CORRECT_LETTER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else
				{
					char used[5] = {0};

					/* check if letter is somewhere in the puzzle */
					for(i = 0; i < 5; i++)
					{
						if(lingo_player[lingo_current_player].word[i] == lingo_word[i])
						{
							used[i] = 1;
						}
					}
					int yp = 0;
					for(i = 0; i < 5; i++)
					{
						if(!lingo_game_check_used[i] && !used[i] && lingo_player[lingo_current_player].word[lingo_game_check_pos] != lingo_word[lingo_game_check_pos] && lingo_player[lingo_current_player].word[lingo_game_check_pos] == lingo_word[i])
						{
							lingo_gameboard_color[lingo_game_check_current_guess][lingo_game_check_pos] = 2;
							lingo_game_check_used[i] = 1;
							al_play_sample(lingo_sample[LINGO_SAMPLE_YELLOW_LETTER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							yp = 1;
						}
					}
					if(!yp)
					{
						al_play_sample(lingo_sample[LINGO_SAMPLE_INCORRECT_LETTER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				lingo_game_check_pos++;

				/* done checking */
				if(lingo_game_check_pos >= 5)
				{
					if(lingo_game_check_correct >= 5)
					{
						lingo_player[lingo_current_player].score += 25;
						lingo_game_state = LINGO_GAME_STATE_P_CORRECT;
						lingo_game_ticker = 0;
					}
					else
					{
						lingo_game_state = LINGO_GAME_STATE_P_INCORRECT;
					}
				}
			}
			break;
		}
		case LINGO_GAME_STATE_SHOW_CORRECT:
		{
			lingo_game_ticker++;
			if(lingo_game_ticker == 5)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_gameboard_font_color[lingo_game_check_current_guess][0] = al_map_rgba(255, 244, 141, 255);
				lingo_gameboard[lingo_game_check_current_guess][0] = lingo_word[0];
//				lingo_add_particle(lingo_image[LINGO_IMAGE_PARTICLE], al_map_rgba(255, 244, 141, 255), LINGO_GAMEBOARD_X_OFFSET + 0 * 56 + 1 + 28, LINGO_GAMEBOARD_Y_OFFSET + lingo_game_check_current_guess * 56 + 1 - 7 + 16, 0, t3f_drand() * 2, t3f_drand() * 4 - 2, -(t3f_drand() * 2 + 8), 30);
//				lingo_draw_text_center(lingo_font[LINGO_FONT_ARIAL_36], LINGO_GAMEBOARD_X_OFFSET + j * 56 + 1 + 28, LINGO_GAMEBOARD_Y_OFFSET + i * 56 + 1 - 7, lingo_gameboard_font_color[i][j], buf);
			}
			if(lingo_game_ticker == 10)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_gameboard_font_color[lingo_game_check_current_guess][1] = al_map_rgba(255, 244, 141, 255);
				lingo_gameboard[lingo_game_check_current_guess][1] = lingo_word[1];
//				lingo_add_particle(lingo_image[LINGO_IMAGE_PARTICLE], al_map_rgba(255, 244, 141, 255), LINGO_GAMEBOARD_X_OFFSET + 1 * 56 + 1 + 28, LINGO_GAMEBOARD_Y_OFFSET + lingo_game_check_current_guess * 56 + 1 - 7 + 16, 0, t3f_drand() * 2, t3f_drand() * 4 - 2, -(t3f_drand() * 2 + 8), 30);
			}
			if(lingo_game_ticker == 15)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_gameboard_font_color[lingo_game_check_current_guess][2] = al_map_rgba(255, 244, 141, 255);
				lingo_gameboard[lingo_game_check_current_guess][2] = lingo_word[2];
//				lingo_add_particle(lingo_image[LINGO_IMAGE_PARTICLE], al_map_rgba(255, 244, 141, 255), LINGO_GAMEBOARD_X_OFFSET + 2 * 56 + 1 + 28, LINGO_GAMEBOARD_Y_OFFSET + lingo_game_check_current_guess * 56 + 1 - 7 + 16, 0, t3f_drand() * 2, t3f_drand() * 4 - 2, -(t3f_drand() * 2 + 8), 30);
			}
			if(lingo_game_ticker == 20)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_gameboard_font_color[lingo_game_check_current_guess][3] = al_map_rgba(255, 244, 141, 255);
				lingo_gameboard[lingo_game_check_current_guess][3] = lingo_word[3];
//				lingo_add_particle(lingo_image[LINGO_IMAGE_PARTICLE], al_map_rgba(255, 244, 141, 255), LINGO_GAMEBOARD_X_OFFSET + 3 * 56 + 1 + 28, LINGO_GAMEBOARD_Y_OFFSET + lingo_game_check_current_guess * 56 + 1 - 7 + 16, 0, t3f_drand() * 2, t3f_drand() * 4 - 2, -(t3f_drand() * 2 + 8), 30);
			}
			if(lingo_game_ticker == 25)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_gameboard_font_color[lingo_game_check_current_guess][4] = al_map_rgba(255, 244, 141, 255);
				lingo_gameboard[lingo_game_check_current_guess][4] = lingo_word[4];
//				lingo_add_particle(lingo_image[LINGO_IMAGE_PARTICLE], al_map_rgba(255, 244, 141, 255), LINGO_GAMEBOARD_X_OFFSET + 4 * 56 + 1 + 28, LINGO_GAMEBOARD_Y_OFFSET + lingo_game_check_current_guess * 56 + 1 - 7 + 16, 0, t3f_drand() * 2, t3f_drand() * 4 - 2, -(t3f_drand() * 2 + 8), 30);
			}
			if(lingo_game_ticker == 15)
			{
				lingo_gameboard_font_color[lingo_game_check_current_guess][0] = al_map_rgba(255, 255, 255, 255);
			}
			if(lingo_game_ticker == 20)
			{
				lingo_gameboard_font_color[lingo_game_check_current_guess][1] = al_map_rgba(255, 255, 255, 255);
			}
			if(lingo_game_ticker == 25)
			{
				lingo_gameboard_font_color[lingo_game_check_current_guess][2] = al_map_rgba(255, 255, 255, 255);
			}
			if(lingo_game_ticker == 30)
			{
				lingo_gameboard_font_color[lingo_game_check_current_guess][3] = al_map_rgba(255, 255, 255, 255);
			}
			if(lingo_game_ticker == 35)
			{
				lingo_gameboard_font_color[lingo_game_check_current_guess][4] = al_map_rgba(255, 255, 255, 255);
			}
			if(lingo_game_ticker > 90)
			{
				if(lingo_player[lingo_current_player].chances <= 0)
				{
					if(lingo_option[LINGO_OPTION_UPLOAD])
					{
						al_stop_timer(t3f_timer);
						t3net_upload_score("http://www.t3-i.com/t3net2/leaderboards/insert.php", "word5", "1.2", "normal", "0", lingo_player[lingo_current_player].name, lingo_player[lingo_current_player].score * 2 + 'v' + 'g' + 'o' + 'l' + 'f', NULL);
						if(lingo_leaderboard)
						{
							t3net_destroy_leaderboard(lingo_leaderboard);
						}
						lingo_leaderboard = t3net_get_leaderboard("http://www.t3-i.com/t3net2/leaderboards/query.php", "word5", "1.2", "normal", "0", 10, 0);
						if(lingo_leaderboard)
						{
							lingo_leaderboard_place = -1;
							for(i = 0; i < lingo_leaderboard->entries; i++)
							{
								if(lingo_player[lingo_current_player].score * 2 + 'v' + 'g' + 'o' + 'l' + 'f' == lingo_leaderboard->entry[i]->score && !strcmp(lingo_player[lingo_current_player].name, lingo_leaderboard->entry[i]->name))
								{
									lingo_leaderboard_place = i;
									break;
								}
							}
							lingo_state = LINGO_STATE_LEADERBOARD;
							lingo_current_menu = LINGO_MENU_LEADERBOARD;
						}
						else
						{
							lingo_game_state = LINGO_GAME_STATE_OVER;
							lingo_current_game_menu = LINGO_GAME_MENU_OVER;
						}
						al_start_timer(t3f_timer);
					}
					else
					{
						lingo_game_state = LINGO_GAME_STATE_OVER;
						lingo_current_game_menu = LINGO_GAME_MENU_OVER;
					}
				}
				else
				{
					strcpy(lingo_last_word, lingo_word);
					strcpy(lingo_word, lingo_dictionary->word[lingo_dictionary_choose_word(lingo_dictionary)].letter);
					lingo_player[lingo_current_player].guess_count = 0;
					for(i = 0; i < 5; i++)
					{
						for(j = 0; j < 5; j++)
						{
							lingo_gameboard[i][j] = 0;
							lingo_gameboard_color[i][j] = 0;
							lingo_gameboard_font_color[i][j] = al_map_rgba(255, 255, 255, 255);
						}
						lingo_game_is_correct[i] = 0;
					}
					lingo_game_is_correct[0] = 1;
					lingo_gameboard[0][0] = lingo_word[0];
					if(lingo_player[lingo_current_player].score > lingo_high_score)
					{
						lingo_high_score = lingo_player[lingo_current_player].score;
					}
					strcpy(lingo_player[lingo_current_player].word, "");
					lingo_player[lingo_current_player].word_pos = 0;
					lingo_game_state = LINGO_GAME_STATE_FIRST_LETTER;
				}
			}
			break;
		}
	}
}

void lingo_game_mode_logic(void)
{
	int i, b;

	switch(lingo_game_state)
	{
		case LINGO_GAME_STATE_P_BONUS_LETTER:
		{
			switch(lingo_game_settings.mode)
			{
				case LINGO_GAME_MODE_1P_SURVIVAL:
				{
					if(lingo_player[lingo_current_player].bonus_letters > 0)
					{
						b = lingo_game_give_bonus_letter();
						for(i = 0; i < 5; i++)
						{
							if(lingo_game_is_correct[i])
							{
								lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = lingo_word[i];
							}
						}
						lingo_game_state = LINGO_GAME_STATE_TYPING;
						if(b)
						{
							al_play_sample(lingo_sample[LINGO_SAMPLE_HINT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							lingo_player[lingo_current_player].bonus_letters--;
							lingo_game_ticker = 0;
							lingo_game_clock = 1200;
						}
					}
					break;
				}
			}
			lingo_game_state = LINGO_GAME_STATE_TYPING;
			lingo_game_ticker = 0;
			break;
		}
		case LINGO_GAME_STATE_P_INVALID:
		{
			switch(lingo_game_settings.mode)
			{
				case LINGO_GAME_MODE_1P_SURVIVAL:
				{
					lingo_player[0].chances--;
					if(lingo_player[0].chances <= 0)
					{
						if(lingo_game_check_current_guess >= 4)
						{
							lingo_game_shift_board_up();
						}
						else
						{
							lingo_game_check_current_guess++;
						}
						lingo_game_state = LINGO_GAME_STATE_SHOW_CORRECT;
						lingo_game_ticker = -30;
						lingo_current_game_menu = LINGO_GAME_MENU_MAIN_DUMMY;
					}
					else
					{
						lingo_player[lingo_current_player].guess_count--;
						for(i = 0; i < 5; i++)
						{
							if(lingo_game_is_correct[i])
							{
								lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = lingo_word[i];
							}
							else
							{
								lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = '.';
							}
						}
						lingo_game_state = LINGO_GAME_STATE_TYPING;
						lingo_game_clock = 1200;
					}
					lingo_game_ticker = 0;
					break;
				}
				case LINGO_GAME_MODE_2P_STANDARD:
				{
					lingo_game_give_bonus_letter();
					lingo_player[lingo_current_player].guess_count--;
					for(i = 0; i < 5; i++)
					{
						if(lingo_game_is_correct[i])
						{
							lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = lingo_word[i];
						}
						else
						{
							lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = '.';
						}
					}
					lingo_current_player = 1 - lingo_current_player;
					strcpy(lingo_player[lingo_current_player].word, "");
					lingo_player[lingo_current_player].word_pos = 0;
					lingo_game_state = LINGO_GAME_STATE_TYPING;
					lingo_game_ticker = 0;
					lingo_game_clock = 1200;
					break;
				}
			}
			break;
		}
		case LINGO_GAME_STATE_P_CORRECT:
		{
			switch(lingo_game_settings.mode)
			{
				case LINGO_GAME_MODE_1P_SURVIVAL:
				{
					al_play_sample(lingo_sample[LINGO_SAMPLE_CORRECT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					lingo_game_state = LINGO_GAME_STATE_SHOW_CORRECT;
					break;
				}
				case LINGO_GAME_MODE_2P_STANDARD:
				{
					lingo_game_state = LINGO_GAME_STATE_FIRST_LETTER;
					break;
				}
			}
			break;
		}
		case LINGO_GAME_STATE_P_INCORRECT:
		{
			switch(lingo_game_settings.mode)
			{
				case LINGO_GAME_MODE_1P_SURVIVAL:
				{
					if(lingo_player[lingo_current_player].guess_count >= 5)
					{
						lingo_player[lingo_current_player].chances--;
						if(lingo_player[lingo_current_player].chances <= 0)
						{
							lingo_game_shift_board_up();
							al_play_sample(lingo_sample[LINGO_SAMPLE_INCORRECT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							lingo_game_state = LINGO_GAME_STATE_SHOW_CORRECT;
							lingo_current_game_menu = LINGO_GAME_MENU_MAIN_DUMMY;
							lingo_game_ticker = -30;
							break;
						}

						lingo_game_shift_board_up();
						al_play_sample(lingo_sample[LINGO_SAMPLE_INCORRECT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						lingo_game_state = LINGO_GAME_STATE_SHOW_CORRECT;
						lingo_game_ticker = -30;
					}
					else
					{
						strcpy(lingo_player[lingo_current_player].word, "");
						lingo_player[lingo_current_player].word_pos = 0;
						for(i = 0; i < 5; i++)
						{
							if(lingo_game_is_correct[i] == 1)
							{
								lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = lingo_word[i];
							}
							else
							{
								lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = '.';
							}
						}
						lingo_game_state = LINGO_GAME_STATE_TYPING;
						lingo_game_ticker = 0;
						lingo_game_clock = 1200;
					}
					break;
				}
				case LINGO_GAME_MODE_2P_STANDARD:
				{
					if(lingo_player[lingo_current_player].guess_count >= 5)
					{
						strcpy(lingo_player[lingo_current_player].word, "");
						lingo_player[lingo_current_player].word_pos = 0;
						lingo_game_shift_board_up();
						lingo_game_give_bonus_letter();

						/* switch players */
						if(lingo_game_settings.players == 2)
						{
							lingo_current_player = 1 - lingo_current_player;
							strcpy(lingo_player[lingo_current_player].word, "");
							lingo_player[lingo_current_player].word_pos = 0;
							lingo_player[lingo_current_player].guess_count = 4;
						}
						for(i = 0; i < 5; i++)
						{
							if(lingo_game_is_correct[i] == 1)
							{
								lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = lingo_word[i];
							}
							else
							{
								lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = '.';
							}
						}
						lingo_game_state = LINGO_GAME_STATE_TYPING;
						lingo_game_ticker = 0;
						lingo_game_clock = 1200;
					}
					else
					{
						strcpy(lingo_player[lingo_current_player].word, "");
						lingo_player[lingo_current_player].word_pos = 0;
						for(i = 0; i < 5; i++)
						{
							if(lingo_game_is_correct[i] == 1)
							{
								lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = lingo_word[i];
							}
							else
							{
								lingo_gameboard[lingo_player[lingo_current_player].guess_count][i] = '.';
							}
						}
						lingo_game_state = LINGO_GAME_STATE_TYPING;
						lingo_game_ticker = 0;
						lingo_game_clock = 1200;
					}
					break;
				}
			}
			break;
		}
	}
}

void lingo_game_logic(void)
{
	int i;
	int mx, mex, ilen;
	int last_item = lingo_game_menu[lingo_current_game_menu].current_item;

	lingo_game_menu[lingo_current_game_menu].current_item = -1;
	for(i = 0; i < lingo_game_menu[lingo_current_game_menu].items; i++)
	{
		ilen = al_get_text_width(lingo_game_menu[lingo_current_game_menu].item[i].font, lingo_game_menu[lingo_current_game_menu].item[i].name);
		if(lingo_game_menu[lingo_current_game_menu].item[i].flags & LINGO_MENU_ITEM_FLAG_CENTER)
		{
			mx = lingo_game_menu[lingo_current_game_menu].x + lingo_game_menu[lingo_current_game_menu].item[i].ox - ilen / 2;
			mex = lingo_game_menu[lingo_current_game_menu].x + lingo_game_menu[lingo_current_game_menu].item[i].ox + ilen / 2;
		}
		else
		{
			mx = lingo_game_menu[lingo_current_game_menu].x + lingo_game_menu[lingo_current_game_menu].item[i].ox;
			mex = lingo_game_menu[lingo_current_game_menu].x + lingo_game_menu[lingo_current_game_menu].item[i].ox + ilen;
		}
		if(t3f_get_mouse_x() >= mx && t3f_get_mouse_x() <= mex && t3f_get_mouse_y() >= lingo_game_menu[lingo_current_game_menu].y + lingo_game_menu[lingo_current_game_menu].item[i].oy && t3f_get_mouse_y() <= lingo_game_menu[lingo_current_game_menu].y + lingo_game_menu[lingo_current_game_menu].item[i].oy + al_get_font_line_height(lingo_game_menu[lingo_current_game_menu].item[i].font))
		{
			if(last_item != i)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_HOVER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			lingo_game_menu[lingo_current_game_menu].current_item = i;
			break;
		}
	}
	if(t3f_mouse_button_pressed(0) && !lingo_mouse_clicked && lingo_game_menu[lingo_current_game_menu].current_item >= 0)
	{
		if(lingo_game_menu[lingo_current_game_menu].item[lingo_game_menu[lingo_current_game_menu].current_item].proc)
		{
//			al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			lingo_game_menu[lingo_current_game_menu].item[lingo_game_menu[lingo_current_game_menu].current_item].proc();
		}
		if(lingo_game_menu[lingo_current_game_menu].item[lingo_game_menu[lingo_current_game_menu].current_item].child_menu != -1)
		{
//			al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			lingo_current_game_menu = lingo_game_menu[lingo_current_game_menu].item[lingo_game_menu[lingo_current_game_menu].current_item].child_menu;
			lingo_game_menu[lingo_current_game_menu].current_item = -1;
		}
		lingo_mouse_clicked = 1;
		t3f_use_mouse_button_press(0);
	}
	lingo_game_core_logic();
	lingo_game_mode_logic();
}

void lingo_game_transition_out_logic(void)
{
	int done = 1;
	if(lingo_game_logo_y < 70)
	{
		lingo_game_logo_y++;
		done = 0;
	}
	if(lingo_game_stats_x > -320)
	{
		lingo_game_stats_x -= 14;
		lingo_game_menu[LINGO_GAME_MENU_MAIN].x -= 14;
		lingo_game_menu[LINGO_GAME_MENU_OVER].x -= 14;
		lingo_game_menu[LINGO_GAME_MENU_MAIN_DUMMY].x -= 14;
		done = 0;
	}
	if(lingo_game_board_z > -640)
	{
		lingo_game_board_z -= 20;
		done = 0;
	}
	if(done)
	{
		lingo_state = LINGO_STATE_TITLE;
		lingo_current_menu = LINGO_MENU_MAIN;
	}
}

void lingo_game_transition_in_logic(void)
{
	int done = 1;
	if(lingo_game_logo_y > 35)
	{
		lingo_game_logo_y--;
		done = 0;
	}
	if(lingo_game_stats_x < 150)
	{
		lingo_game_stats_x += 14;
		lingo_game_menu[LINGO_GAME_MENU_MAIN].x += 14;
		lingo_game_menu[LINGO_GAME_MENU_OVER].x += 14;
		lingo_game_menu[LINGO_GAME_MENU_MAIN_DUMMY].x += 14;
		done = 0;
	}
	if(lingo_game_board_z < 0)
	{
		lingo_game_board_z += 20;
		done = 0;
	}
	if(done)
	{
//		al_play_sample(lingo_sample[LINGO_SAMPLE_LOGO], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		lingo_state = LINGO_STATE_GAME;
	}
}

void lingo_game_render(void)
{
	char buf[16];
	int i, j, x, y;
	ALLEGRO_COLOR col[2];
	ALLEGRO_COLOR gcol[2];
	char * text;
	int tw, tnw;
	int mx, mex, ilen;
	float a;

	t3f_select_view(t3f_default_view);
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_BG], LINGO_COLOR_WHITE, 0, 0, 0, 0);
	lingo_select_view();
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_LOGO], LINGO_COLOR_WHITE, 320 - lingo_image[LINGO_IMAGE_LOGO]->target_width / 2.0, lingo_game_logo_y, 0, 0);
	a = 1.0 + lingo_game_board_z / 640.0;
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_GAMEBOARD], al_map_rgba_f(a, a, a, a), LINGO_GAMEBOARD_X_OFFSET, LINGO_GAMEBOARD_Y_OFFSET, lingo_game_board_z, 0);

	if(lingo_current_player == 0)
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
	lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x + 2, 170 + 2, al_map_rgba(0, 0, 0, 128), "Score");
	lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x, 170, gcol[0], "Score");
	sprintf(buf, "%04d", lingo_player[0].score);
	lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x + 2, 170 + 24 + 2, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x, 170 + 24, col[0], buf);

	/* draw hints */
	sprintf(buf, "Hints: %d", lingo_player[0].bonus_letters);
	tw = al_get_text_width(lingo_font[LINGO_FONT_SPRINT_10], buf);
	sprintf(buf, "%d", lingo_player[0].bonus_letters);
	tnw = al_get_text_width(lingo_font[LINGO_FONT_SPRINT_10], buf);
	sprintf(buf, "Hints:");
	lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_10], lingo_game_stats_x + 1 - tw / 2, 170 + 60 + 1, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_10], lingo_game_stats_x - tw / 2, 170 + 60, gcol[0], buf);
	sprintf(buf, "%d", lingo_player[0].bonus_letters);
	lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_10], lingo_game_stats_x + 1 - tw / 2 + (tw - tnw), 170 + 60 + 1, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_10], lingo_game_stats_x - tw / 2 + (tw - tnw), 170 + 60, col[0], buf);

	/* draw chances */
	sprintf(buf, "Chances: %d", lingo_player[0].chances);
	tw = al_get_text_width(lingo_font[LINGO_FONT_SPRINT_10], buf);
	sprintf(buf, "%d", lingo_player[0].chances);
	tnw = al_get_text_width(lingo_font[LINGO_FONT_SPRINT_10], buf);
	sprintf(buf, "Chances:");
	lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_10], lingo_game_stats_x + 1 - tw / 2, 170 + 72 + 1, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_10], lingo_game_stats_x - tw / 2, 170 + 72, gcol[0], buf);
	sprintf(buf, "%d", lingo_player[0].chances);
	lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_10], lingo_game_stats_x + 1 - tw / 2 + (tw - tnw), 170 + 72 + 1, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_10], lingo_game_stats_x - tw / 2 + (tw - tnw), 170 + 72, col[0], buf);

	/* draw high score */
	lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x + 2, 170 + 94 + 2, al_map_rgba(0, 0, 0, 128), "High");
	lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x, 170 + 94, gcol[0], "High");
	sprintf(buf, "%04d", lingo_high_score);
	lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x + 2, 170 + 94 + 24 + 2, al_map_rgba(0, 0, 0, 128), buf);
	lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x, 170 + 94 + 24, col[0], buf);

	/* draw current guess */
	if(lingo_game_state != LINGO_GAME_STATE_OVER && lingo_current_game_menu != LINGO_GAME_MENU_OVER)
	{
		lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x + 2, 170 + 118 + 12 + 50 + 2, al_map_rgba(0, 0, 0, 128), "Guess");
		lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x, 170 + 118 + 12 + 50, gcol[0], "Guess");
		strcpy(buf, lingo_player[lingo_current_player].word);
		lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x + 2, 170 + 118 + 39 + 50 + 2, al_map_rgba(0, 0, 0, 128), buf);
		lingo_draw_text_center(lingo_font[LINGO_FONT_SPRINT_20], lingo_game_stats_x, 170 + 118 + 39 + 50, al_map_rgba(255, 255, 0, 255), buf);
	}

	/* draw clock */
	if(lingo_state == LINGO_STATE_GAME && lingo_game_state == LINGO_GAME_STATE_TYPING)
	{
		sprintf(buf, "%2d", (lingo_game_clock + 59) / 60);
		if(lingo_game_clock <= 300)
		{
			lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_20], 596 + 2, 444 + 2, al_map_rgba(0, 0, 0, 128), buf);
			lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_20], 596, 444, al_map_rgba(255, 0, 0, 255), buf);
		}
		else
		{
			lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_20], 592 + 2, 444 + 2, al_map_rgba(0, 0, 0, 128), buf);
			lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_20], 592, 444, col[0], buf);
		}
	}

	/* draw the game board */
	if(lingo_state == LINGO_STATE_GAME)
	{
		for(i = 0; i < 5; i++)
		{
			for(j = 0; j < 5; j++)
			{
				if(lingo_gameboard[i][j])
				{
					buf[0] = lingo_gameboard[i][j];
					buf[1] = '\0';
					if(lingo_gameboard_color[i][j] == 1)
					{
						t3f_draw_bitmap(lingo_image[LINGO_IMAGE_RED_SQUARE], t3f_color_white, LINGO_GAMEBOARD_X_OFFSET + j * 56 + 1, LINGO_GAMEBOARD_Y_OFFSET + i * 56 + 1, 0.0, 0);
					}
					else if(lingo_gameboard_color[i][j] == 2)
					{
						t3f_draw_bitmap(lingo_image[LINGO_IMAGE_YELLOW_CIRCLE], t3f_color_white, LINGO_GAMEBOARD_X_OFFSET + j * 56 + 1, LINGO_GAMEBOARD_Y_OFFSET + i * 56 + 1, 0.0, 0);
					}
					lingo_draw_text_center(lingo_font[LINGO_FONT_ARIAL_36], LINGO_GAMEBOARD_X_OFFSET + j * 56 + 1 + 28 + 2, LINGO_GAMEBOARD_Y_OFFSET + i * 56 + 1 - 7 + 2, al_map_rgba(0, 0, 0, 128), buf);
					lingo_draw_text_center(lingo_font[LINGO_FONT_ARIAL_36], LINGO_GAMEBOARD_X_OFFSET + j * 56 + 1 + 28, LINGO_GAMEBOARD_Y_OFFSET + i * 56 + 1 - 7, lingo_gameboard_font_color[i][j], buf);
				}
			}
		}
	}

	/* draw the in-game menu */
	for(i = 0; i < lingo_game_menu[lingo_current_game_menu].items; i++)
	{
		text = lingo_game_menu[lingo_current_game_menu].item[i].name;
		x = lingo_game_menu[lingo_current_game_menu].x + lingo_game_menu[lingo_current_game_menu].item[i].ox;
		y = lingo_game_menu[lingo_current_game_menu].y + lingo_game_menu[lingo_current_game_menu].item[i].oy;
		ilen = al_get_text_width(lingo_game_menu[lingo_current_game_menu].item[i].font, text);
		if(lingo_game_menu[lingo_current_game_menu].item[i].flags & LINGO_MENU_ITEM_FLAG_CENTER)
		{
			mx = x - al_get_text_width(lingo_game_menu[lingo_current_game_menu].item[i].font, text) / 2;
			mex = lingo_menu[lingo_current_menu].x + lingo_menu[lingo_current_menu].item[i].ox + ilen / 2;
		}
		else
		{
			mx = x;
			mex = lingo_menu[lingo_current_menu].x + lingo_menu[lingo_current_menu].item[i].ox + ilen;
		}
		if(i == lingo_game_menu[lingo_current_game_menu].current_item)
		{
			al_draw_text(lingo_game_menu[lingo_current_game_menu].item[i].font, al_map_rgba(0, 0, 0, 128), mx + 2, y + 2, 0, text);
			al_draw_text(lingo_game_menu[lingo_current_game_menu].item[i].font, al_map_rgba(255, 255, 255, 255), mx - 2, y - 2, 0, text);
		}
		else
		{
			al_draw_text(lingo_game_menu[lingo_current_game_menu].item[i].font, al_map_rgba(0, 0, 0, 128), mx + 2, y + 2, 0, text);
			al_draw_text(lingo_game_menu[lingo_current_game_menu].item[i].font, al_map_rgba(255, 244, 141, 255), mx, y, 0, text);
		}
	}
}
