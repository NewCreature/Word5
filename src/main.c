#include "t3f/t3f.h"
#include "t3net/leaderboard.h"
#include "main.h"
#include "data.h"
#include "dictionary.h"
#include "player.h"
#include "title.h"
#include "menu_proc.h"
#include "game.h"
#include "tutorial.h"
#include "highscore.h"
#include "leaderboard.h"

/* main logic routine */
void app_logic(void * data)
{
	int letter = 0;

	t3f_select_input_view(lingo_view);
	if(!t3f_mouse_button_pressed(0))
	{
		t3f_use_mouse_button_press(0);
		lingo_mouse_clicked = 0;
	}
	if(lingo_text_buffer_state == 1)
	{
		letter = t3f_get_char(0);
		if(letter)
		{
			if(letter == '\b' || letter == 127)
			{
				if(lingo_text_buffer_pos > 0)
				{
					al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					lingo_text_buffer_pos--;
					lingo_player[lingo_current_player].name[lingo_text_buffer_pos] = '\0';
				}
			}
			else if(letter == '\r')
			{
				lingo_menu_proc_main_start_game();
			}
			else if(letter >= 32 && letter < 127)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				lingo_player[lingo_current_player].name[lingo_text_buffer_pos] = letter;
				lingo_text_buffer_pos++;
				lingo_player[lingo_current_player].name[lingo_text_buffer_pos] = '\0';
			}
			strcpy(lingo_menu[LINGO_MENU_ENTER_NAME].item[1].name, lingo_player[0].name);
		}
	}
	switch(lingo_state)
	{
		case LINGO_STATE_TITLE:
		{
			lingo_title_logic();
			break;
		}
		case LINGO_STATE_TITLE_TRANSITION_IN:
		{
			lingo_title_transition_in_logic();
			break;
		}
		case LINGO_STATE_TITLE_TRANSITION_OUT:
		{
			lingo_title_transition_out_logic();
			break;
		}
		case LINGO_STATE_GAME:
		{
			lingo_game_logic();
			break;
		}
		case LINGO_STATE_GAME_TRANSITION_IN:
		{
			lingo_game_transition_in_logic();
			break;
		}
		case LINGO_STATE_GAME_TRANSITION_OUT:
		{
			lingo_game_transition_out_logic();
			break;
		}
		case LINGO_STATE_TUTORIAL:
		{
			lingo_tutorial_logic();
			break;
		}
		case LINGO_STATE_TUTORIAL_TRANSITION_IN:
		{
			lingo_tutorial_transition_in_logic();
			break;
		}
		case LINGO_STATE_TUTORIAL_TRANSITION_OUT:
		{
			lingo_tutorial_transition_out_logic();
			break;
		}
		case LINGO_STATE_LEADERBOARD:
		{
			lingo_leaderboard_logic();
			break;
		}
	}
	lingo_logic_counter++;
}

void lingo_draw_text(ALLEGRO_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text)
{
	al_draw_text(fp, color, x, y, 0, text);
}

void lingo_draw_text_center(ALLEGRO_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text)
{
	al_draw_text(fp, color, x - al_get_text_width(fp, text) / 2, y, 0, text);
}

void lingo_select_view(void)
{
	int vx, vy, vw, vh;

	vx = t3f_default_view->left;
	vy = t3f_default_view->top;
	vw = t3f_default_view->right - t3f_default_view->left;
	vh = t3f_default_view->bottom - t3f_default_view->top;
	t3f_adjust_view(lingo_view, vx, vy, vw, vh, 320, 240, T3F_FORCE_ASPECT);
	t3f_select_view(lingo_view);
	al_set_clipping_rectangle(0, 0, al_get_display_width(t3f_display), al_get_display_height(t3f_display));
}

/* main rendering routine */
void app_render(void * data)
{
	al_hold_bitmap_drawing(true);
	switch(lingo_state)
	{
		case LINGO_STATE_TITLE:
		{
			lingo_title_render();
			break;
		}
		case LINGO_STATE_TITLE_TRANSITION_IN:
		{
			lingo_title_transition_in_render();
			break;
		}
		case LINGO_STATE_TITLE_TRANSITION_OUT:
		{
			lingo_title_transition_out_render();
			break;
		}
		case LINGO_STATE_GAME:
		case LINGO_STATE_GAME_TRANSITION_IN:
		case LINGO_STATE_GAME_TRANSITION_OUT:
		{
			lingo_game_render();
			break;
		}
		case LINGO_STATE_TUTORIAL:
		case LINGO_STATE_TUTORIAL_TRANSITION_IN:
		case LINGO_STATE_TUTORIAL_TRANSITION_OUT:
		{
			lingo_tutorial_render();
			break;
		}
		case LINGO_STATE_LEADERBOARD:
		{
			lingo_leaderboard_render();
			break;
		}
	}
	al_hold_bitmap_drawing(false);
}

void lingo_draw_load_bar(T3F_BITMAP * bp[3], int step)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_TRANSFORM identity;
	int w, h;
	int i;

	w = al_get_display_width(t3f_display);
	h = al_get_display_height(t3f_display);
	al_store_state(&old_state, ALLEGRO_STATE_TRANSFORM);
	al_identity_transform(&identity);
	al_use_transform(&identity);
	al_clear_to_color(al_map_rgb(64, 64, 128));
	al_hold_bitmap_drawing(true);
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_LOAD], t3f_color_white, w / 2- lingo_image[LINGO_IMAGE_LOAD]->target_width / 2.0, h / 2 - lingo_image[LINGO_IMAGE_LOAD]->target_height / 2.0, 0.0, 0);
	for(i = 0; i < step; i++)
	{
//		al_draw_bitmap(bp[2], 319 - i + 2, 300 + 2, 0);
//		al_draw_bitmap(bp[2], 320 + i + 2, 300 + 2, 0);
		t3f_draw_bitmap(bp[0], t3f_color_white, w / 2 - 1 - i, h / 2 + 60, 0.0, 0);
		t3f_draw_bitmap(bp[0], t3f_color_white, w / 2 + i, h / 2 + 60, 0.0, 0);
	}
//	al_draw_bitmap(bp[2], 319 - step - 1 + 2, 300 + 2, 0);
//	al_draw_bitmap(bp[2], 320 + step + 1 + 2, 300 + 2, 0);
	t3f_draw_bitmap(bp[1], t3f_color_white, w / 2 - 1 - step - 1, h / 2 + 60, 0.0, 0);
	t3f_draw_bitmap(bp[1], t3f_color_white, w / 2 + step + 1, h / 2 + 60, 0.0, 0);
	al_hold_bitmap_drawing(false);
	al_flip_display();
	al_restore_state(&old_state);
}

static void ocd_convert_grey_to_alpha(ALLEGRO_BITMAP *bitmap)
{
	int x, y;
	unsigned char ir, ig, ib, ia;
	ALLEGRO_COLOR pixel;
	ALLEGRO_STATE old_state;

	if(!al_lock_bitmap(bitmap, al_get_bitmap_format(bitmap), 0))
	{
		return;
	}

	al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP);
	al_set_target_bitmap(bitmap);

	for(y = 0; y < al_get_bitmap_height(bitmap); y++)
	{
		for(x = 0; x < al_get_bitmap_width(bitmap); x++)
		{
			pixel = al_get_pixel(bitmap, x, y);
			al_unmap_rgba(pixel, &ir, &ig, &ib, &ia);
			if(ir == 255 && ig == 0 && ib == 255)
			{
				pixel = al_map_rgba(0, 0, 0, 0);
				al_put_pixel(x, y, pixel);
			}
			else if(ia > 0 && !(ir == 255 && ig == 255 && ib == 0))
			{
				pixel = al_map_rgba(ir, ir, ir, ir);
				al_put_pixel(x, y, pixel);
			}
		}
	}

	al_restore_state(&old_state);
	al_unlock_bitmap(bitmap);
}

static ALLEGRO_FONT * load_font(char * fn)
{
	int ranges[] = {32, 126};
	ALLEGRO_FONT * fp;
	ALLEGRO_STATE old_state;
	ALLEGRO_BITMAP * fimage;
//	int old_format = al_get_new_bitmap_format();

	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
//	al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA);
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP | ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	fimage = al_load_bitmap(fn);
	if(!fimage)
	{
		return NULL;
	}
	ocd_convert_grey_to_alpha(fimage);
	al_restore_state(&old_state);
	fp = al_grab_font_from_bitmap(fimage, 1, ranges);
/*	if(!fp)
	{
		al_destroy_bitmap(fimage);
		return NULL;
	} */
	al_destroy_bitmap(fimage);
//	al_set_new_bitmap_format(old_format);
	return fp;
}
int lingo_initialize(void)
{
	int i;
	T3F_BITMAP * loadbar[3];
	const char * val = NULL;
	char buf[1024];

	if(!t3f_initialize(T3F_APP_TITLE, 640, 480, 60.0, app_logic, app_render, T3F_DEFAULT | T3F_RESIZABLE | T3F_FILL_SCREEN, NULL))
	{
		printf("Error initializing T3F\n");
		return false;
	}

	lingo_view = t3f_create_view(0, 0, 640, 480, 320, 240, t3f_flags);
	if(!lingo_view)
	{
		return 0;
	}
	lingo_image[LINGO_IMAGE_LOAD] = t3f_load_bitmap("data/graphics/loading.png", 0, false);
	if(!lingo_image[LINGO_IMAGE_LOAD])
	{
		return 0;
	}
	loadbar[0] = t3f_load_bitmap("data/graphics/loadbar.png", 0, false);
	loadbar[1] = t3f_load_bitmap("data/graphics/loadbaredge.png", 0, false);
	loadbar[2] = t3f_load_bitmap("data/graphics/loadbarshadow.png", 0, false);
	for(i = 0; i < 3; i++)
	{
		if(!loadbar[i])
		{
			return 0;
		}
	}

	lingo_draw_load_bar(loadbar, 0);

	al_show_mouse_cursor(NULL);
	lingo_draw_load_bar(loadbar, 2);

	lingo_atlas = t3f_create_atlas(256, 64);
	lingo_draw_load_bar(loadbar, 4);
	lingo_image[LINGO_IMAGE_LOGO] = t3f_load_bitmap("data/graphics/logoimage.png", 0, false);
	lingo_draw_load_bar(loadbar, 16);
	lingo_image[LINGO_IMAGE_GAMEBOARD] = t3f_load_bitmap("data/graphics/gameboard.png", 0, false);
	lingo_draw_load_bar(loadbar, 18);
	lingo_image[LINGO_IMAGE_RED_SQUARE] = t3f_load_bitmap("data/graphics/right.pcx", 0, false);
	t3f_add_bitmap_to_atlas(lingo_atlas, &lingo_image[LINGO_IMAGE_RED_SQUARE]->bitmap, T3F_ATLAS_TILE);
	lingo_draw_load_bar(loadbar, 20);
	lingo_image[LINGO_IMAGE_YELLOW_CIRCLE] = t3f_load_bitmap("data/graphics/right2.pcx", 0, false);
	t3f_add_bitmap_to_atlas(lingo_atlas, &lingo_image[LINGO_IMAGE_YELLOW_CIRCLE]->bitmap, T3F_ATLAS_TILE);
	lingo_draw_load_bar(loadbar, 22);
	lingo_image[LINGO_IMAGE_BG] = t3f_load_bitmap("data/graphics/bg.png", 0, false);
	lingo_draw_load_bar(loadbar, 24);

	lingo_sample[LINGO_SAMPLE_MENU_HOVER] = al_load_sample("data/sounds/hover.ogg");
	lingo_draw_load_bar(loadbar, 28);
	lingo_sample[LINGO_SAMPLE_MENU_CLICK] = al_load_sample("data/sounds/click.ogg");
	lingo_draw_load_bar(loadbar, 30);
	lingo_sample[LINGO_SAMPLE_CORRECT] = al_load_sample("data/sounds/correct.ogg");
	lingo_draw_load_bar(loadbar, 32);
	lingo_sample[LINGO_SAMPLE_INCORRECT] = al_load_sample("data/sounds/incorrect.ogg");
	lingo_draw_load_bar(loadbar, 34);
	lingo_sample[LINGO_SAMPLE_FIRST_LETTER] = al_load_sample("data/sounds/firstletter.ogg");
	lingo_draw_load_bar(loadbar, 36);
	lingo_sample[LINGO_SAMPLE_CORRECT_LETTER] = al_load_sample("data/sounds/correctletter.ogg");
	lingo_draw_load_bar(loadbar, 38);
	lingo_sample[LINGO_SAMPLE_INCORRECT_LETTER] = al_load_sample("data/sounds/incorrectletter.ogg");
	lingo_draw_load_bar(loadbar, 40);
	lingo_sample[LINGO_SAMPLE_YELLOW_LETTER] = al_load_sample("data/sounds/yellowletter.ogg");
	lingo_draw_load_bar(loadbar, 42);
	lingo_sample[LINGO_SAMPLE_HINT] = al_load_sample("data/sounds/hint.ogg");
	lingo_draw_load_bar(loadbar, 44);
	lingo_sample[LINGO_SAMPLE_LOGO] = al_load_sample("data/sounds/logo.ogg");
	lingo_draw_load_bar(loadbar, 46);
	lingo_sample[LINGO_SAMPLE_TYPE] = al_load_sample("data/sounds/type.ogg");
	lingo_draw_load_bar(loadbar, 48);
	lingo_sample[LINGO_SAMPLE_CLOCK] = al_load_sample("data/sounds/clock.ogg");
	lingo_draw_load_bar(loadbar, 50);

	for(i = LINGO_IMAGE_LOGO; i <= LINGO_IMAGE_LOAD; i++)
	{
		if(!lingo_image[i])
		{
			return 0;
		}
	}
	lingo_draw_load_bar(loadbar, 52);

	lingo_font[LINGO_FONT_SPRINT_10] = load_font("data/fonts/font_sprint10.pcx");
	lingo_draw_load_bar(loadbar, 54);
	lingo_font[LINGO_FONT_SPRINT_20] = load_font("data/fonts/font_sprint20.pcx");
	lingo_draw_load_bar(loadbar, 56);
	lingo_font[LINGO_FONT_SPRINT_36] = load_font("data/fonts/font_sprint36.pcx");
	lingo_draw_load_bar(loadbar, 58);
	lingo_font[LINGO_FONT_ARIAL_36] = load_font("data/fonts/font_arial36.pcx");
	lingo_draw_load_bar(loadbar, 60);
	for(i = LINGO_FONT_SPRINT_10; i <= LINGO_FONT_ARIAL_36; i++)
	{
		if(!lingo_font[i])
		{
			return 0;
		}
	}

	lingo_dictionary = lingo_load_dictionary("data/word5.dictionary");
	if(!lingo_dictionary)
	{
		return 0;
	}
	lingo_load_dictionary_config(lingo_dictionary, t3f_get_filename(t3f_data_path, "word5.dictionary.cfg", buf, 1024));
	lingo_draw_load_bar(loadbar, 62);

	lingo_load_score(t3f_get_filename(t3f_data_path, "word5.highscore", buf, 1024));
	lingo_draw_load_bar(loadbar, 64);

	lingo_title_initialize();
	lingo_draw_load_bar(loadbar, 66);

	for(i = 0; i < 3; i++)
	{
		t3f_destroy_bitmap(loadbar[i]);
	}
	t3f_destroy_bitmap(lingo_image[LINGO_IMAGE_LOAD]);

	val = al_get_config_value(t3f_config, "Game", "Player Name");
	if(val)
	{
		strcpy(lingo_player[0].name, val);
	}
	else
	{
		strcpy(lingo_player[0].name, "Player 1");
	}
	al_set_config_value(t3f_config, "Game", "Player Name", lingo_player[0].name);
	val = al_get_config_value(t3f_config, "Game", "Upload Scores");
	if(val)
	{
		if(!strcmp(val, "Yes"))
		{
			lingo_option[LINGO_OPTION_UPLOAD] = 1;
		}
		else
		{
			lingo_option[LINGO_OPTION_UPLOAD] = 0;
		}
	}
	else
	{
		lingo_option[LINGO_OPTION_UPLOAD] = 1;
		al_set_config_value(t3f_config, "Game", "Upload Scores", lingo_option[LINGO_OPTION_UPLOAD] ? "Yes" : "No");
	}

	srand(time(0));
	return 1;
}

int lingo_exit(void)
{
	char buf[1024];

	lingo_save_score(t3f_get_filename(t3f_data_path, "word5.highscore", buf, 1024));
	lingo_save_dictionary_config(lingo_dictionary, t3f_get_filename(t3f_data_path, "word5.dictionary.cfg", buf, 1024));
	return 1;
}

int main(int argc, char * argv[])
{
	if(lingo_initialize())
	{
		t3f_run();
		lingo_exit();
	}
	else
	{
		printf("Error: could not initialize T3F!\n");
	}
	t3f_finish();
	return 0;
}
