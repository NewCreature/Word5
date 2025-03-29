#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "instance.h"
#include "main.h"
#include "player.h"
#include "menu_proc.h"
#include "game.h"
#include "tutorial.h"

void lingo_tutorial_clear_message(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i;

	for(i = 0; i < 16; i++)
	{
		instance->tutorial_message[i][0] = '\0';
	}
}

void lingo_tutorial_proc_step_0(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	instance->tutorial_step = 1;
	instance->tutorial_freeze_clock = 1;
	lingo_tutorial_update_message(data);
	lingo_tutorial_center_message(data);
}

void lingo_tutorial_proc_step_1(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i;

	strcpy(instance->player[instance->current_player].word, "WEIRD");
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

	instance->tutorial_step = 2;
	instance->tutorial_freeze_clock = 1;
	lingo_tutorial_update_message(data);
	lingo_tutorial_center_message(data);
	instance->tutorial_show_text = 0;
}

void lingo_tutorial_proc_step_2(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	instance->tutorial_step = 3;
	instance->tutorial_freeze_clock = 1;
	lingo_tutorial_update_message(data);
	lingo_tutorial_center_message(data);
}

void lingo_tutorial_proc_step_3(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i;

	strcpy(instance->player[instance->current_player].word, "WATER");
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

	instance->tutorial_step = 4;
	instance->tutorial_freeze_clock = 1;
	lingo_tutorial_update_message(data);
	lingo_tutorial_center_message(data);
	instance->tutorial_show_text = 0;
}

void lingo_tutorial_proc_step_4(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	lingo_menu_proc_game_main_bonus_letter(data);
	instance->tutorial_timer = 30;

	instance->tutorial_step = 5;
	instance->tutorial_freeze_clock = 1;
	lingo_tutorial_update_message(data);
	lingo_tutorial_center_message(data);
	instance->tutorial_show_text = 0;
}

void lingo_tutorial_proc_step_5(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	instance->tutorial_step = 6;
	instance->tutorial_freeze_clock = 1;
	lingo_tutorial_update_message(data);
	lingo_tutorial_center_message(data);
}

void lingo_tutorial_proc_step_6(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	instance->tutorial_step = 7;
	instance->tutorial_freeze_clock = 1;
	lingo_tutorial_update_message(data);
	lingo_tutorial_center_message(data);
}

void lingo_tutorial_proc_step_7(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i;

	strcpy(instance->player[instance->current_player].word, "WORDS");
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

	instance->tutorial_step = 8;
	instance->tutorial_freeze_clock = 1;
	lingo_tutorial_update_message(data);
	lingo_tutorial_center_message(data);
	instance->tutorial_show_text = 0;
}

void lingo_tutorial_proc_step_8(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	lingo_menu_proc_game_main_quit(data);
	instance->tutorial_step = 9;
	instance->tutorial_freeze_clock = 1;
	lingo_tutorial_update_message(data);
	lingo_tutorial_center_message(data);
}

void lingo_tutorial_update_message(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	switch(instance->tutorial_step)
	{
		case 0:
		{
			lingo_tutorial_clear_message(data);
			strcpy(instance->tutorial_message[0], "Welcome to Word5!");
			strcpy(instance->tutorial_message[2], "This tutorial will teach you how");
			strcpy(instance->tutorial_message[3], "to play the game.");
			strcpy(instance->tutorial_message[5], "Click anywhere to proceed.");
			instance->tutorial_proc = lingo_tutorial_proc_step_0;
			break;
		}
		case 1:
		{
			lingo_tutorial_clear_message(data);
			strcpy(instance->tutorial_message[0], "The object of the game is to try");
			strcpy(instance->tutorial_message[1], "to guess the word so we start by");
			strcpy(instance->tutorial_message[2], "taking a guess.");
			strcpy(instance->tutorial_message[4], "Note that entering an invalid");
			strcpy(instance->tutorial_message[5], "word will cost you a life and");
			strcpy(instance->tutorial_message[6], "will not reveal anything about");
			strcpy(instance->tutorial_message[7], "the word.");
			instance->tutorial_proc = lingo_tutorial_proc_step_1;
			break;
		}
		case 2:
		{
			lingo_tutorial_clear_message(data);
			strcpy(instance->tutorial_message[0], "Notice the highlighted areas of");
			strcpy(instance->tutorial_message[1], "the game board. Correct letters");
			strcpy(instance->tutorial_message[2], "are highlighted in red. Letters");
			strcpy(instance->tutorial_message[3], "that are in the word but not in");
			strcpy(instance->tutorial_message[4], "the correct place are highlighted");
			strcpy(instance->tutorial_message[5], "in yellow.");
			instance->tutorial_proc = lingo_tutorial_proc_step_2;
			break;
		}
		case 3:
		{
			lingo_tutorial_clear_message(data);
			strcpy(instance->tutorial_message[0], "Using this information we can");
			strcpy(instance->tutorial_message[1], "narrow down the possible words");
			strcpy(instance->tutorial_message[2], "until we find the correct word.");
			instance->tutorial_proc = lingo_tutorial_proc_step_3;
			break;
		}
		case 4:
		{
			lingo_tutorial_clear_message(data);
			strcpy(instance->tutorial_message[0], "If you are drawing a blank you");
			strcpy(instance->tutorial_message[1], "might want to use a Hint. A");
			strcpy(instance->tutorial_message[2], "Hint will fill in the next");
			strcpy(instance->tutorial_message[3], "unknown letter for you. You");
			strcpy(instance->tutorial_message[4], "only have 5 Hints so use them");
			strcpy(instance->tutorial_message[5], "wisely.");
			instance->tutorial_proc = lingo_tutorial_proc_step_4;
			break;
		}
		case 5:
		{
			lingo_tutorial_clear_message(data);
			strcpy(instance->tutorial_message[0], "You only have 5 guesses to");
			strcpy(instance->tutorial_message[1], "figure out the word. If you");
			strcpy(instance->tutorial_message[2], "do not successfully guess the");
			strcpy(instance->tutorial_message[3], "word you lose 1 life. The");
			strcpy(instance->tutorial_message[4], "game ends when you have lost");
			strcpy(instance->tutorial_message[5], "all 5 of your lives.");
			instance->tutorial_proc = lingo_tutorial_proc_step_5;
			break;
		}
		case 6:
		{
			lingo_tutorial_clear_message(data);
			strcpy(instance->tutorial_message[0], "The timer in the lower-right");
			strcpy(instance->tutorial_message[1], "corner of the screen tells you");
			strcpy(instance->tutorial_message[2], "how much time you have left to");
			strcpy(instance->tutorial_message[3], "make a guess. If the timer");
			strcpy(instance->tutorial_message[4], "reaches 0 you not only lose a");
			strcpy(instance->tutorial_message[5], "life, but a new word is chosen");
			strcpy(instance->tutorial_message[6], "as well so act quickly.");
			strcpy(instance->tutorial_message[8], "Lets take another guess.");
			instance->tutorial_proc = lingo_tutorial_proc_step_7;
			break;
		}
		case 7:
		{
			lingo_tutorial_clear_message(data);
			strcpy(instance->tutorial_message[0], "Lets take another guess.");
			instance->tutorial_proc = lingo_tutorial_proc_step_7;
			break;
		}
		case 8:
		{
			lingo_tutorial_clear_message(data);
			strcpy(instance->tutorial_message[0], "Excellent, we guessed the");
			strcpy(instance->tutorial_message[1], "correct word!");
			strcpy(instance->tutorial_message[3], "Each correct word earns you");
			strcpy(instance->tutorial_message[4], "25 points and a new word to");
			strcpy(instance->tutorial_message[5], "guess. Try and get the high");
			strcpy(instance->tutorial_message[6], "score!");
			strcpy(instance->tutorial_message[8], "Have fun and thank you for");
			strcpy(instance->tutorial_message[9], "playing :)");
			instance->tutorial_proc = lingo_tutorial_proc_step_8;
			break;
		}
	}
}

void lingo_tutorial_center_message(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i;
	int height = 0;
	int width = 0;
	int newwidth = 0;

	for(i = 15; i >= 0; i--)
	{
		if(height)
		{
			height += 20;
		}
		else if(instance->tutorial_message[i][0] != '\0')
		{
			height += 20;
		}
		newwidth = t3f_get_text_width(instance->font[LINGO_FONT_SPRINT_10], instance->tutorial_message[i]);
		if(newwidth > width)
		{
			width = newwidth;
		}
	}
	instance->tutorial_rtlx = 320 - width / 2 - 8;
	instance->tutorial_rbrx = instance->tutorial_rtlx + width + 8 + 7;
	instance->tutorial_rtly = 240 - height / 2 - 8;
	instance->tutorial_rbry = instance->tutorial_rtly + height + 8 + 7;
}

void lingo_tutorial_start(int mode, void * data)
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
			strcpy(instance->word, "WORDS");
			instance->gameboard[0][0] = instance->word[0];
			for(i = 0; i < 5; i++)
			{
				instance->game_is_correct[i] = 0;
			}
			instance->game_is_correct[0] = 1;
			instance->game_ticker = 0;
			instance->game_state = LINGO_GAME_STATE_FIRST_LETTER;
			instance->game_clock = 1200;

			instance->tutorial_step = 0;
			instance->tutorial_show_text = 1;
			instance->tutorial_freeze_clock = 1;
			lingo_tutorial_update_message(data);
			lingo_tutorial_center_message(data);

			break;
		}
	}
}

void lingo_tutorial_core_logic(void * data)
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

			if(!instance->tutorial_freeze_clock)
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
			}
/*			instance->player[instance->current_player].letter = instance->keybuffer_read_key(LINGO_KEYBUFFER_FLAG_FORCE_UPPER);
			if(instance->player[instance->current_player].letter)
			{
				if(instance->player[instance->current_player].letter == 1)
				{
					if(instance->player[instance->current_player].word_pos > 0)
					{
						al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						instance->player[instance->current_player].word_pos--;
						instance->player[instance->current_player].word[instance->player[instance->current_player].word_pos] = '\0';
					}
				}
				else if(instance->player[instance->current_player].letter == 2)
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
				}
				else if(instance->player[instance->current_player].letter == 3)
				{
					instance->menu_proc_game_main_bonus_letter();
					break;
				}
				else
				{
					if(instance->player[instance->current_player].word_pos < 5)
					{
						al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						instance->player[instance->current_player].word[instance->player[instance->current_player].word_pos] = instance->player[instance->current_player].letter;
						instance->player[instance->current_player].word_pos++;
						instance->player[instance->current_player].word[instance->player[instance->current_player].word_pos] = '\0';
					}
				}
			} */
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
					instance->game_state = LINGO_GAME_STATE_OVER;
					instance->current_menu = LINGO_MENU_GAME_OVER;
				}
				else
				{
					strcpy(instance->last_word, instance->word);
					strcpy(instance->word, "PLAYS");
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
					strcpy(instance->player[instance->current_player].word, "");
					instance->player[instance->current_player].word_pos = 0;
					instance->game_state = LINGO_GAME_STATE_FIRST_LETTER;
					if(instance->tutorial_step == 8)
					{
						instance->tutorial_show_text = 1;
					}
				}
			}
			break;
		}
	}
}

void lingo_tutorial_mode_logic(void * data)
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
						instance->current_menu = LINGO_MENU_GAME_OVER;
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
							instance->current_menu = LINGO_MENU_GAME_OVER;
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
					if(instance->tutorial_step == 2)
					{
						instance->tutorial_show_text = 1;
					}
					else if(instance->tutorial_step == 4)
					{
						instance->tutorial_show_text = 1;
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

void lingo_tutorial_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	instance->menu[instance->current_menu].current_item = -1;
	if(t3f_mouse_button_pressed(0))
	{
		if(instance->tutorial_proc && instance->tutorial_show_text)
		{
			al_play_sample(instance->sample[LINGO_SAMPLE_MENU_HOVER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			instance->tutorial_proc(data);
		}
		t3f_use_mouse_button_press(0);
	}
	if(instance->tutorial_timer > 0)
	{
		instance->tutorial_timer--;
		if(instance->tutorial_timer <= 0)
		{
			instance->tutorial_show_text = 1;
		}
	}
	instance->tutorial_ticker++;
	lingo_tutorial_core_logic(data);
	lingo_tutorial_mode_logic(data);
}

void lingo_tutorial_transition_out_logic(void * data)
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
	}
}

void lingo_tutorial_transition_in_logic(void * data)
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
		instance->state = LINGO_STATE_TUTORIAL;
	}
}

void lingo_tutorial_render_message(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i;

	al_draw_filled_rectangle(0, 0, 639, 479, al_map_rgba(0, 0, 0, 64));
	al_draw_rectangle(instance->tutorial_rtlx, instance->tutorial_rtly, instance->tutorial_rbrx, instance->tutorial_rbry, al_map_rgba(0, 0, 0, 255), 0);
	al_draw_filled_rectangle(instance->tutorial_rtlx, instance->tutorial_rtly, instance->tutorial_rbrx, instance->tutorial_rbry, al_map_rgba(0, 0, 0, 128));
	for(i = 0; i < 16; i++)
	{
		lingo_draw_text(instance->font[LINGO_FONT_SPRINT_10], instance->tutorial_rtlx + 8 + 2, instance->tutorial_rtly + 8 + i * 20 + 2, al_map_rgba(0, 0, 0, 128), instance->tutorial_message[i]);
		lingo_draw_text(instance->font[LINGO_FONT_SPRINT_10], instance->tutorial_rtlx + 8, instance->tutorial_rtly + 8  + i * 20, al_map_rgba(255, 255, 255, 255), instance->tutorial_message[i]);
	}
	if((instance->tutorial_ticker / 10) % 2 == 0)
	{
		al_draw_filled_rectangle(instance->tutorial_rbrx - 10, instance->tutorial_rbry - 10, instance->tutorial_rbrx - 4, instance->tutorial_rbry - 4, al_map_rgba(255, 255, 255, 255));
	}
}

void lingo_tutorial_render(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	lingo_game_render(data);
	al_hold_bitmap_drawing(false);
	if(instance->state != LINGO_STATE_TUTORIAL_TRANSITION_IN && instance->state != LINGO_STATE_TUTORIAL_TRANSITION_OUT && instance->tutorial_show_text)
	{
		lingo_tutorial_render_message(data);
	}
}
