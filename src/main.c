#include "t3f/t3f.h"
#include "t3net/leaderboard.h"
#include "instance.h"
#include "main.h"
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
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int letter = 0;

	t3f_select_input_view(instance->view);
	if(!t3f_mouse_button_pressed(0))
	{
		t3f_use_mouse_button_press(0);
		instance->mouse_clicked = 0;
	}
	if(instance->text_buffer_state == 1)
	{
		letter = t3f_get_char(0);
		if(letter)
		{
			if(letter == '\b' || letter == 127)
			{
				if(instance->text_buffer_pos > 0)
				{
					al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					instance->text_buffer_pos--;
					instance->player[instance->current_player].name[instance->text_buffer_pos] = '\0';
				}
			}
			else if(letter == '\r')
			{
				lingo_menu_proc_main_start_game();
			}
			else if(letter >= 32 && letter < 127)
			{
				al_play_sample(instance->sample[LINGO_SAMPLE_TYPE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				instance->player[instance->current_player].name[instance->text_buffer_pos] = letter;
				instance->text_buffer_pos++;
				instance->player[instance->current_player].name[instance->text_buffer_pos] = '\0';
			}
			strcpy(instance->menu[LINGO_MENU_ENTER_NAME].item[1].name, instance->player[0].name);
		}
	}
	switch(instance->state)
	{
		case LINGO_STATE_TITLE:
		{
			lingo_title_logic(data);
			break;
		}
		case LINGO_STATE_TITLE_TRANSITION_IN:
		{
			lingo_title_transition_in_logic(data);
			break;
		}
		case LINGO_STATE_TITLE_TRANSITION_OUT:
		{
			lingo_title_transition_out_logic(data);
			break;
		}
		case LINGO_STATE_GAME:
		{
			lingo_game_logic(data);
			break;
		}
		case LINGO_STATE_GAME_TRANSITION_IN:
		{
			lingo_game_transition_in_logic(data);
			break;
		}
		case LINGO_STATE_GAME_TRANSITION_OUT:
		{
			lingo_game_transition_out_logic(data);
			break;
		}
		case LINGO_STATE_TUTORIAL:
		{
			lingo_tutorial_logic(data);
			break;
		}
		case LINGO_STATE_TUTORIAL_TRANSITION_IN:
		{
			lingo_tutorial_transition_in_logic(data);
			break;
		}
		case LINGO_STATE_TUTORIAL_TRANSITION_OUT:
		{
			lingo_tutorial_transition_out_logic(data);
			break;
		}
		case LINGO_STATE_LEADERBOARD:
		{
			lingo_leaderboard_logic(data);
			break;
		}
	}
	instance->logic_counter++;
}

void lingo_draw_text(T3F_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text)
{
	t3f_draw_text(fp, color, x, y, 0, 0, text);
}

void lingo_draw_text_center(T3F_FONT * fp, float x, float y, ALLEGRO_COLOR color, char * text)
{
	t3f_draw_text(fp, color, x - t3f_get_text_width(fp, text) / 2, y, 0, 0, text);
}

void lingo_select_view(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int vx, vy, vw, vh;

	vx = t3f_default_view->left;
	vy = t3f_default_view->top;
	vw = t3f_default_view->right - t3f_default_view->left;
	vh = t3f_default_view->bottom - t3f_default_view->top;
	t3f_adjust_view(instance->view, vx, vy, vw, vh, 320, 240, T3F_FORCE_ASPECT);
	t3f_select_view(instance->view);
	al_set_clipping_rectangle(0, 0, al_get_display_width(t3f_display), al_get_display_height(t3f_display));
}

/* main rendering routine */
void app_render(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	al_hold_bitmap_drawing(true);
	switch(instance->state)
	{
		case LINGO_STATE_TITLE:
		{
			lingo_title_render(data);
			break;
		}
		case LINGO_STATE_TITLE_TRANSITION_IN:
		{
			lingo_title_transition_in_render(data);
			break;
		}
		case LINGO_STATE_TITLE_TRANSITION_OUT:
		{
			lingo_title_transition_out_render(data);
			break;
		}
		case LINGO_STATE_GAME:
		case LINGO_STATE_GAME_TRANSITION_IN:
		case LINGO_STATE_GAME_TRANSITION_OUT:
		{
			lingo_game_render(data);
			break;
		}
		case LINGO_STATE_TUTORIAL:
		case LINGO_STATE_TUTORIAL_TRANSITION_IN:
		case LINGO_STATE_TUTORIAL_TRANSITION_OUT:
		{
			lingo_tutorial_render(data);
			break;
		}
		case LINGO_STATE_LEADERBOARD:
		{
			lingo_leaderboard_render(data);
			break;
		}
	}
	al_hold_bitmap_drawing(false);
}

void lingo_draw_load_bar(T3F_BITMAP * bp[3], int step, void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
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
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_LOAD], t3f_color_white, w / 2- instance->image[LINGO_IMAGE_LOAD]->target_width / 2.0, h / 2 - instance->image[LINGO_IMAGE_LOAD]->target_height / 2.0, 0.0, 0);
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

static void _lingo_setup_urls(void)
{
	const char * val;

	val = al_get_config_value(t3f_config, "Config", "leaderboard_upload_url");
	if(!val)
	{
		al_set_config_value(t3f_config, "Config", "leaderboard_upload_url", "https://www.tcubedsoftware.com/scripts/leaderboards/update.php");
	}
	val = al_get_config_value(t3f_config, "Config", "leaderboard_download_url");
	if(!val)
	{
		al_set_config_value(t3f_config, "Config", "leaderboard_download_url", "https://www.tcubedsoftware.com/scripts/leaderboards/query.php");
	}
	val = al_get_config_value(t3f_config, "Config", "leaderboard_key_url");
	if(!val)
	{
		al_set_config_value(t3f_config, "Config", "leaderboard_key_url", "https://www.tcubedsoftware.com/scripts/leaderboards/get_user_key.php");
	}
	val = al_get_config_value(t3f_config, "Config", "leaderboard_username_url");
	if(!val)
	{
		al_set_config_value(t3f_config, "Config", "leaderboard_username_url", "https://www.tcubedsoftware.com/scripts/leaderboards/set_user_name.php");
	}
}

static void _lingo_set_optimal_display_size(void)
{
	ALLEGRO_MONITOR_INFO info;
	int width, height;
	int current_width = 640;
	int current_height = 480;
	int width_increment = 640 / 4;
	int height_increment = 480 / 4;
	int c = 0;

	al_get_monitor_info(0, &info);
	width = info.x2 - info.x1 - 64;
	height = info.y2 - info.y1 - 64;
	while(current_width < width && current_height < height)
	{
		current_width += width_increment;
		current_height += height_increment;
		c++;
	}
	current_width -= width_increment;
	current_height -= height_increment;
	if(c > 1)
	{
		current_width -= width_increment;
		current_height -= height_increment;
	}
	if(current_width != al_get_display_width(t3f_display) || current_height != al_get_display_height(t3f_display))
	{
		t3f_set_gfx_mode(current_width, current_height, t3f_flags);
		al_set_window_position(t3f_display, (info.x2 - info.x1) / 2 - current_width / 2, (info.y2 - info.y1) / 2 - current_height / 2);
	}
}

int lingo_initialize(APP_INSTANCE * instance)
{
	int i;
	T3F_BITMAP * loadbar[3];
	const char * val = NULL;
	char buf[1024];

	if(!t3f_initialize(T3F_APP_TITLE, 640, 480, 60.0, app_logic, app_render, T3F_DEFAULT | T3F_RESIZABLE | T3F_FILL_SCREEN, instance))
	{
		printf("Error initializing T3F\n");
		return false;
	}
	if(!al_get_config_value(t3f_config, "T3F", "display_width"))
	{
		_lingo_set_optimal_display_size();
	}

	#ifdef ALLEGRO_ANDROID
		t3net_setup(t3f_run_url, al_path_cstr(t3f_temp_path, '/'));
	#else
		t3net_setup(NULL, al_path_cstr(t3f_temp_path, '/'));
	#endif
	_lingo_setup_urls();

	instance->view = t3f_create_view(0, 0, 640, 480, 320, 240, t3f_flags);
	if(!instance->view)
	{
		return 0;
	}
	instance->image[LINGO_IMAGE_LOAD] = t3f_load_bitmap("data/graphics/loading.png", 0, false);
	if(!instance->image[LINGO_IMAGE_LOAD])
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

	lingo_draw_load_bar(loadbar, 0, instance);

	al_show_mouse_cursor(NULL);
	lingo_draw_load_bar(loadbar, 2, instance);

	instance->atlas = t3f_create_atlas(128, 64);
	lingo_draw_load_bar(loadbar, 4, instance);
	instance->image[LINGO_IMAGE_LOGO] = t3f_load_bitmap("data/graphics/logoimage.png", 0, false);
	lingo_draw_load_bar(loadbar, 16, instance);
	instance->image[LINGO_IMAGE_GAMEBOARD] = t3f_load_bitmap("data/graphics/gameboard.png", T3F_BITMAP_FLAG_PADDED, false);
	lingo_draw_load_bar(loadbar, 18, instance);
	instance->image[LINGO_IMAGE_RED_SQUARE] = t3f_load_bitmap("data/graphics/right.pcx", T3F_BITMAP_FLAG_PADDED, false);
	t3f_add_bitmap_to_atlas(instance->atlas, &instance->image[LINGO_IMAGE_RED_SQUARE]->bitmap, T3F_ATLAS_TILE);
	lingo_draw_load_bar(loadbar, 20, instance);
	instance->image[LINGO_IMAGE_YELLOW_CIRCLE] = t3f_load_bitmap("data/graphics/right2.pcx", T3F_BITMAP_FLAG_PADDED, false);
	t3f_add_bitmap_to_atlas(instance->atlas, &instance->image[LINGO_IMAGE_YELLOW_CIRCLE]->bitmap, T3F_ATLAS_TILE);
	lingo_draw_load_bar(loadbar, 22, instance);
	instance->image[LINGO_IMAGE_BG] = t3f_load_bitmap("data/graphics/bg.png", 0, false);
	lingo_draw_load_bar(loadbar, 24, instance);

	instance->sample[LINGO_SAMPLE_MENU_HOVER] = al_load_sample("data/sounds/hover.ogg");
	lingo_draw_load_bar(loadbar, 28, instance);
	instance->sample[LINGO_SAMPLE_MENU_CLICK] = al_load_sample("data/sounds/click.ogg");
	lingo_draw_load_bar(loadbar, 30, instance);
	instance->sample[LINGO_SAMPLE_CORRECT] = al_load_sample("data/sounds/correct.ogg");
	lingo_draw_load_bar(loadbar, 32, instance);
	instance->sample[LINGO_SAMPLE_INCORRECT] = al_load_sample("data/sounds/incorrect.ogg");
	lingo_draw_load_bar(loadbar, 34, instance);
	instance->sample[LINGO_SAMPLE_FIRST_LETTER] = al_load_sample("data/sounds/firstletter.ogg");
	lingo_draw_load_bar(loadbar, 36, instance);
	instance->sample[LINGO_SAMPLE_CORRECT_LETTER] = al_load_sample("data/sounds/correctletter.ogg");
	lingo_draw_load_bar(loadbar, 38, instance);
	instance->sample[LINGO_SAMPLE_INCORRECT_LETTER] = al_load_sample("data/sounds/incorrectletter.ogg");
	lingo_draw_load_bar(loadbar, 40, instance);
	instance->sample[LINGO_SAMPLE_YELLOW_LETTER] = al_load_sample("data/sounds/yellowletter.ogg");
	lingo_draw_load_bar(loadbar, 42, instance);
	instance->sample[LINGO_SAMPLE_HINT] = al_load_sample("data/sounds/hint.ogg");
	lingo_draw_load_bar(loadbar, 44, instance);
	instance->sample[LINGO_SAMPLE_LOGO] = al_load_sample("data/sounds/logo.ogg");
	lingo_draw_load_bar(loadbar, 46, instance);
	instance->sample[LINGO_SAMPLE_TYPE] = al_load_sample("data/sounds/type.ogg");
	lingo_draw_load_bar(loadbar, 48, instance);
	instance->sample[LINGO_SAMPLE_CLOCK] = al_load_sample("data/sounds/clock.ogg");
	lingo_draw_load_bar(loadbar, 50, instance);

	for(i = LINGO_IMAGE_LOGO; i <= LINGO_IMAGE_LOAD; i++)
	{
		if(!instance->image[i])
		{
			return 0;
		}
	}
	lingo_draw_load_bar(loadbar, 52, instance);

	instance->font[LINGO_FONT_SPRINT_10] = t3f_load_font("data/fonts/font_sprint10.png", T3F_FONT_TYPE_AUTO, 0, 0, false);
	lingo_draw_load_bar(loadbar, 54, instance);
	instance->font[LINGO_FONT_SPRINT_20] = t3f_load_font("data/fonts/font_sprint20.png", T3F_FONT_TYPE_AUTO, 0, 0, false);
	lingo_draw_load_bar(loadbar, 56, instance);
	instance->font[LINGO_FONT_SPRINT_36] = t3f_load_font("data/fonts/font_sprint36.png", T3F_FONT_TYPE_AUTO, 0, 0, false);
	lingo_draw_load_bar(loadbar, 58, instance);
	instance->font[LINGO_FONT_ARIAL_36] = t3f_load_font("data/fonts/font_arial36.png", T3F_FONT_TYPE_AUTO, 0, 0, false);
	lingo_draw_load_bar(loadbar, 60, instance);
	for(i = LINGO_FONT_SPRINT_10; i <= LINGO_FONT_ARIAL_36; i++)
	{
		if(!instance->font[i])
		{
			return 0;
		}
	}

	instance->dictionary = lingo_load_dictionary("data/word5.dictionary");
	if(!instance->dictionary)
	{
		return 0;
	}
	lingo_load_dictionary_config(instance->dictionary, t3f_get_filename(t3f_data_path, "word5.dictionary.cfg", buf, 1024));
	lingo_draw_load_bar(loadbar, 62, instance);

	lingo_load_score(t3f_get_filename(t3f_data_path, "word5.highscore", buf, 1024), &instance->high_score);
	lingo_draw_load_bar(loadbar, 64, instance);

	lingo_title_initialize(instance);
	lingo_draw_load_bar(loadbar, 66, instance);

	for(i = 0; i < 3; i++)
	{
		t3f_destroy_bitmap(loadbar[i]);
	}
	t3f_destroy_bitmap(instance->image[LINGO_IMAGE_LOAD]);

	val = al_get_config_value(t3f_config, "Game", "Player Name");
	if(val)
	{
		strcpy(instance->player[0].name, val);
	}
	else
	{
		strcpy(instance->player[0].name, "Player 1");
	}
	al_set_config_value(t3f_config, "Game", "Player Name", instance->player[0].name);
	val = al_get_config_value(t3f_config, "Game", "Upload Scores");
	if(val)
	{
		if(!strcmp(val, "Yes"))
		{
			instance->option[LINGO_OPTION_UPLOAD] = 1;
		}
		else
		{
			instance->option[LINGO_OPTION_UPLOAD] = 0;
		}
	}
	else
	{
		instance->option[LINGO_OPTION_UPLOAD] = 1;
		al_set_config_value(t3f_config, "Game", "Upload Scores", instance->option[LINGO_OPTION_UPLOAD] ? "Yes" : "No");
	}

	srand(time(0));
	return 1;
}

int lingo_exit(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	char buf[1024];

	lingo_save_score(t3f_get_filename(t3f_data_path, "word5.highscore", buf, 1024), instance->high_score);
	lingo_save_dictionary_config(instance->dictionary, t3f_get_filename(t3f_data_path, "word5.dictionary.cfg", buf, 1024));
	return 1;
}

int main(int argc, char * argv[])
{
	APP_INSTANCE * instance;

	instance = w5_create_instance(argc, argv);
	if(!instance)
	{
		printf("Could not create instance!\n");
		return -1;
	}
	if(lingo_initialize(instance))
	{
		t3f_run();
		lingo_exit(instance);
	}
	else
	{
		printf("Error: could not initialize T3F!\n");
	}
	t3f_finish();
	return 0;
}
