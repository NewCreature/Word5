#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "main.h"
#include "data.h"
#include "player.h"
#include "menu_proc.h"
#include "game.h"
#include "tutorial.h"

int lingo_tutorial_step;
int lingo_tutorial_freeze_clock = 0;
int lingo_tutorial_timer = 0;
int lingo_tutorial_ticker = 0;
char lingo_tutorial_message[16][128] = {{0}};
void (*lingo_tutorial_proc)() = NULL;
int lingo_tutorial_rtlx, lingo_tutorial_rtly, lingo_tutorial_rbrx, lingo_tutorial_rbry;
int lingo_tutorial_show_text = 0;

void lingo_tutorial_clear_message(void)
{
	int i;

	for(i = 0; i < 16; i++)
	{
		lingo_tutorial_message[i][0] = '\0';
	}
}

void lingo_tutorial_proc_step_0(void)
{
	lingo_tutorial_step = 1;
	lingo_tutorial_freeze_clock = 1;
	lingo_tutorial_update_message();
	lingo_tutorial_center_message();
}

void lingo_tutorial_proc_step_1(void)
{
	int i;

	strcpy(lingo_player[lingo_current_player].word, "WEIRD");
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

	lingo_tutorial_step = 2;
	lingo_tutorial_freeze_clock = 1;
	lingo_tutorial_update_message();
	lingo_tutorial_center_message();
	lingo_tutorial_show_text = 0;
}

void lingo_tutorial_proc_step_2(void)
{
	lingo_tutorial_step = 3;
	lingo_tutorial_freeze_clock = 1;
	lingo_tutorial_update_message();
	lingo_tutorial_center_message();
}

void lingo_tutorial_proc_step_3(void)
{
	int i;

	strcpy(lingo_player[lingo_current_player].word, "WATER");
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

	lingo_tutorial_step = 4;
	lingo_tutorial_freeze_clock = 1;
	lingo_tutorial_update_message();
	lingo_tutorial_center_message();
	lingo_tutorial_show_text = 0;
}

void lingo_tutorial_proc_step_4(void)
{
	lingo_menu_proc_game_main_bonus_letter();
	lingo_tutorial_timer = 30;

	lingo_tutorial_step = 5;
	lingo_tutorial_freeze_clock = 1;
	lingo_tutorial_update_message();
	lingo_tutorial_center_message();
	lingo_tutorial_show_text = 0;
}

void lingo_tutorial_proc_step_5(void)
{
	lingo_tutorial_step = 6;
	lingo_tutorial_freeze_clock = 1;
	lingo_tutorial_update_message();
	lingo_tutorial_center_message();
}

void lingo_tutorial_proc_step_6(void)
{
	lingo_tutorial_step = 7;
	lingo_tutorial_freeze_clock = 1;
	lingo_tutorial_update_message();
	lingo_tutorial_center_message();
}

void lingo_tutorial_proc_step_7(void)
{
	int i;

	strcpy(lingo_player[lingo_current_player].word, "WORDS");
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

	lingo_tutorial_step = 8;
	lingo_tutorial_freeze_clock = 1;
	lingo_tutorial_update_message();
	lingo_tutorial_center_message();
	lingo_tutorial_show_text = 0;
}

void lingo_tutorial_proc_step_8(void)
{
	lingo_menu_proc_game_main_quit();
	lingo_tutorial_step = 9;
	lingo_tutorial_freeze_clock = 1;
	lingo_tutorial_update_message();
	lingo_tutorial_center_message();
}

void lingo_tutorial_update_message(void)
{
	switch(lingo_tutorial_step)
	{
		case 0:
		{
			lingo_tutorial_clear_message();
			strcpy(lingo_tutorial_message[0], "Welcome to Word5!");
			strcpy(lingo_tutorial_message[2], "This tutorial will teach you how");
			strcpy(lingo_tutorial_message[3], "to play the game.");
			strcpy(lingo_tutorial_message[5], "Click anywhere to proceed.");
			lingo_tutorial_proc = lingo_tutorial_proc_step_0;
			break;
		}
		case 1:
		{
			lingo_tutorial_clear_message();
			strcpy(lingo_tutorial_message[0], "The object of the game is to try");
			strcpy(lingo_tutorial_message[1], "to guess the word so we start by");
			strcpy(lingo_tutorial_message[2], "taking a guess.");
			strcpy(lingo_tutorial_message[4], "Note that entering an invalid");
			strcpy(lingo_tutorial_message[5], "word will cost you a chance and");
			strcpy(lingo_tutorial_message[6], "will not reveal anything about");
			strcpy(lingo_tutorial_message[7], "the word.");
			lingo_tutorial_proc = lingo_tutorial_proc_step_1;
			break;
		}
		case 2:
		{
			lingo_tutorial_clear_message();
			strcpy(lingo_tutorial_message[0], "Notice the highlighted areas of");
			strcpy(lingo_tutorial_message[1], "the game board. Correct letters");
			strcpy(lingo_tutorial_message[2], "are highlighted in red. Letters");
			strcpy(lingo_tutorial_message[3], "that are in the word but not in");
			strcpy(lingo_tutorial_message[4], "the correct place are highlighted");
			strcpy(lingo_tutorial_message[5], "in yellow.");
			lingo_tutorial_proc = lingo_tutorial_proc_step_2;
			break;
		}
		case 3:
		{
			lingo_tutorial_clear_message();
			strcpy(lingo_tutorial_message[0], "Using this information we can");
			strcpy(lingo_tutorial_message[1], "narrow down the possible words");
			strcpy(lingo_tutorial_message[2], "until we find the correct word.");
			lingo_tutorial_proc = lingo_tutorial_proc_step_3;
			break;
		}
		case 4:
		{
			lingo_tutorial_clear_message();
			strcpy(lingo_tutorial_message[0], "If you are drawing a blank you");
			strcpy(lingo_tutorial_message[1], "might want to use a Hint. A");
			strcpy(lingo_tutorial_message[2], "Hint will fill in the next");
			strcpy(lingo_tutorial_message[3], "unknown letter for you. You");
			strcpy(lingo_tutorial_message[4], "only have 5 Hints so use them");
			strcpy(lingo_tutorial_message[5], "wisely.");
			lingo_tutorial_proc = lingo_tutorial_proc_step_4;
			break;
		}
		case 5:
		{
			lingo_tutorial_clear_message();
			strcpy(lingo_tutorial_message[0], "You only have 5 guesses to");
			strcpy(lingo_tutorial_message[1], "figure out the word. If you");
			strcpy(lingo_tutorial_message[2], "do not successfully guess the");
			strcpy(lingo_tutorial_message[3], "word you lose 1 chance. The");
			strcpy(lingo_tutorial_message[4], "game ends when you have lost");
			strcpy(lingo_tutorial_message[5], "all 5 of your chances.");
			lingo_tutorial_proc = lingo_tutorial_proc_step_5;
			break;
		}
		case 6:
		{
			lingo_tutorial_clear_message();
			strcpy(lingo_tutorial_message[0], "The timer in the lower-right");
			strcpy(lingo_tutorial_message[1], "corner of the screen tells you");
			strcpy(lingo_tutorial_message[2], "how much time you have left to");
			strcpy(lingo_tutorial_message[3], "make a guess. If the timer");
			strcpy(lingo_tutorial_message[4], "reaches 0 you not only lose a");
			strcpy(lingo_tutorial_message[5], "chance, but a new word is chosen");
			strcpy(lingo_tutorial_message[6], "as well so act quickly.");
			strcpy(lingo_tutorial_message[8], "Lets take another guess.");
			lingo_tutorial_proc = lingo_tutorial_proc_step_7;
			break;
		}
		case 7:
		{
			lingo_tutorial_clear_message();
			strcpy(lingo_tutorial_message[0], "Lets take another guess.");
			lingo_tutorial_proc = lingo_tutorial_proc_step_7;
			break;
		}
		case 8:
		{
			lingo_tutorial_clear_message();
			strcpy(lingo_tutorial_message[0], "Excellent, we guessed the");
			strcpy(lingo_tutorial_message[1], "correct word!");
			strcpy(lingo_tutorial_message[3], "Each correct word earns you");
			strcpy(lingo_tutorial_message[4], "25 points and a new word to");
			strcpy(lingo_tutorial_message[5], "guess. Try and get the high");
			strcpy(lingo_tutorial_message[6], "score!");
			strcpy(lingo_tutorial_message[8], "Have fun and thank you for");
			strcpy(lingo_tutorial_message[9], "playing :)");
			lingo_tutorial_proc = lingo_tutorial_proc_step_8;
			break;
		}
	}
}

void lingo_tutorial_center_message(void)
{
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
		else if(lingo_tutorial_message[i][0] != '\0')
		{
			height += 20;
		}
		newwidth = al_get_text_width(lingo_font[LINGO_FONT_SPRINT_10], lingo_tutorial_message[i]);
		if(newwidth > width)
		{
			width = newwidth;
		}
	}
	lingo_tutorial_rtlx = 320 - width / 2 - 8;
	lingo_tutorial_rbrx = lingo_tutorial_rtlx + width + 8 + 7;
	lingo_tutorial_rtly = 240 - height / 2 - 8;
	lingo_tutorial_rbry = lingo_tutorial_rtly + height + 8 + 7;
}

void lingo_tutorial_start(int mode)
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
			strcpy(lingo_word, "WORDS");
			lingo_gameboard[0][0] = lingo_word[0];
			for(i = 0; i < 5; i++)
			{
				lingo_game_is_correct[i] = 0;
			}
			lingo_game_is_correct[0] = 1;
			lingo_game_ticker = 0;
			lingo_game_state = LINGO_GAME_STATE_FIRST_LETTER;
			lingo_game_clock = 1200;

			lingo_tutorial_step = 0;
			lingo_tutorial_show_text = 1;
			lingo_tutorial_freeze_clock = 1;
			lingo_tutorial_update_message();
			lingo_tutorial_center_message();

			break;
		}
	}
}

void lingo_tutorial_core_logic(void)
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

			if(!lingo_tutorial_freeze_clock)
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
			}
/*			lingo_player[lingo_current_player].letter = lingo_keybuffer_read_key(LINGO_KEYBUFFER_FLAG_FORCE_UPPER);
			if(lingo_player[lingo_current_player].letter)
			{
				if(lingo_player[lingo_current_player].letter == 1)
				{
					if(lingo_player[lingo_current_player].word_pos > 0)
					{
						al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						lingo_player[lingo_current_player].word_pos--;
						lingo_player[lingo_current_player].word[lingo_player[lingo_current_player].word_pos] = '\0';
					}
				}
				else if(lingo_player[lingo_current_player].letter == 2)
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
				else if(lingo_player[lingo_current_player].letter == 3)
				{
					lingo_menu_proc_game_main_bonus_letter();
					break;
				}
				else
				{
					if(lingo_player[lingo_current_player].word_pos < 5)
					{
						al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						lingo_player[lingo_current_player].word[lingo_player[lingo_current_player].word_pos] = lingo_player[lingo_current_player].letter;
						lingo_player[lingo_current_player].word_pos++;
						lingo_player[lingo_current_player].word[lingo_player[lingo_current_player].word_pos] = '\0';
					}
				}
			} */
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
			}
			if(lingo_game_ticker == 10)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_gameboard_font_color[lingo_game_check_current_guess][1] = al_map_rgba(255, 244, 141, 255);
				lingo_gameboard[lingo_game_check_current_guess][1] = lingo_word[1];
			}
			if(lingo_game_ticker == 15)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_gameboard_font_color[lingo_game_check_current_guess][2] = al_map_rgba(255, 244, 141, 255);
				lingo_gameboard[lingo_game_check_current_guess][2] = lingo_word[2];
			}
			if(lingo_game_ticker == 20)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_gameboard_font_color[lingo_game_check_current_guess][3] = al_map_rgba(255, 244, 141, 255);
				lingo_gameboard[lingo_game_check_current_guess][3] = lingo_word[3];
			}
			if(lingo_game_ticker == 25)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_gameboard_font_color[lingo_game_check_current_guess][4] = al_map_rgba(255, 244, 141, 255);
				lingo_gameboard[lingo_game_check_current_guess][4] = lingo_word[4];
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
					lingo_game_state = LINGO_GAME_STATE_OVER;
					lingo_current_game_menu = LINGO_GAME_MENU_OVER;
				}
				else
				{
					strcpy(lingo_last_word, lingo_word);
					strcpy(lingo_word, "PLAYS");
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
					strcpy(lingo_player[lingo_current_player].word, "");
					lingo_player[lingo_current_player].word_pos = 0;
					lingo_game_state = LINGO_GAME_STATE_FIRST_LETTER;
					if(lingo_tutorial_step == 8)
					{
						lingo_tutorial_show_text = 1;
					}
				}
			}
			break;
		}
	}
}

void lingo_tutorial_mode_logic(void)
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
						lingo_current_game_menu = LINGO_GAME_MENU_OVER;
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
							lingo_current_game_menu = LINGO_GAME_MENU_OVER;
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
					if(lingo_tutorial_step == 2)
					{
						lingo_tutorial_show_text = 1;
					}
					else if(lingo_tutorial_step == 4)
					{
						lingo_tutorial_show_text = 1;
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

void lingo_tutorial_logic(void)
{
	lingo_game_menu[lingo_current_game_menu].current_item = -1;
	if(t3f_mouse_button_pressed(0) && !lingo_mouse_clicked)
	{
		lingo_mouse_clicked = 1;
		if(lingo_tutorial_proc && lingo_tutorial_show_text)
		{
			al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_HOVER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			lingo_tutorial_proc();
		}
		t3f_use_mouse_button_press(0);
	}
	if(lingo_tutorial_timer > 0)
	{
		lingo_tutorial_timer--;
		if(lingo_tutorial_timer <= 0)
		{
			lingo_tutorial_show_text = 1;
		}
	}
	lingo_tutorial_ticker++;
	lingo_tutorial_core_logic();
	lingo_tutorial_mode_logic();
}

void lingo_tutorial_transition_out_logic(void)
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
	}
}

void lingo_tutorial_transition_in_logic(void)
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
		lingo_state = LINGO_STATE_TUTORIAL;
	}
}

void lingo_tutorial_render_message(void)
{
	int i;

	al_draw_filled_rectangle(0, 0, 639, 479, al_map_rgba(0, 0, 0, 64));
	al_draw_rectangle(lingo_tutorial_rtlx, lingo_tutorial_rtly, lingo_tutorial_rbrx, lingo_tutorial_rbry, al_map_rgba(0, 0, 0, 255), 0);
	al_draw_filled_rectangle(lingo_tutorial_rtlx, lingo_tutorial_rtly, lingo_tutorial_rbrx, lingo_tutorial_rbry, al_map_rgba(0, 0, 0, 128));
	for(i = 0; i < 16; i++)
	{
		lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_10], lingo_tutorial_rtlx + 8 + 2, lingo_tutorial_rtly + 8 + i * 20 + 2, al_map_rgba(0, 0, 0, 128), lingo_tutorial_message[i]);
		lingo_draw_text(lingo_font[LINGO_FONT_SPRINT_10], lingo_tutorial_rtlx + 8, lingo_tutorial_rtly + 8  + i * 20, al_map_rgba(255, 255, 255, 255), lingo_tutorial_message[i]);
	}
	if((lingo_tutorial_ticker / 10) % 2 == 0)
	{
		al_draw_filled_rectangle(lingo_tutorial_rbrx - 10, lingo_tutorial_rbry - 10, lingo_tutorial_rbrx - 4, lingo_tutorial_rbry - 4, al_map_rgba(255, 255, 255, 255));
	}
}

void lingo_tutorial_render(void)
{
	char buf[16];
	int i, j, x, y;
	ALLEGRO_COLOR col[2];
	ALLEGRO_COLOR gcol[2];
	char * text;
	int tw, tnw;
	int mx, mex, ilen;

//	al_clear(al_map_rgb(64, 64, 128));
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_BG], LINGO_COLOR_WHITE, 0, 0, 0, 0);
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_LOGO], LINGO_COLOR_WHITE, 320 - lingo_image[LINGO_IMAGE_LOGO]->target_width / 2.0, lingo_game_logo_y, 0, 0);
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_GAMEBOARD], LINGO_COLOR_WHITE, LINGO_GAMEBOARD_X_OFFSET, LINGO_GAMEBOARD_Y_OFFSET, lingo_game_board_z, 0);

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
	if(lingo_state == LINGO_STATE_TUTORIAL && lingo_game_state == LINGO_GAME_STATE_TYPING)
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
	if(lingo_state == LINGO_STATE_TUTORIAL)
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
	al_hold_bitmap_drawing(false);
	if(lingo_state != LINGO_STATE_TUTORIAL_TRANSITION_IN && lingo_state != LINGO_STATE_TUTORIAL_TRANSITION_OUT && lingo_tutorial_show_text)
	{
		lingo_tutorial_render_message();
	}
}
