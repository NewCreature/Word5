#include "main.h"
#include "instance.h"

static const char * _lingo_leaderboards_config_section = "Leaderboards";
static const char * _lingo_leaderboards_config_key_user_key = "user_key";
static const char * _lingo_leaderboards_config_key_user_name = "user_name";
static const char * _lingo_leaderboards_config_high_score = "high_score";
static const char * _lingo_leaderboards_config_uploaded_score = "leaderboard_score";

static const char * _lingo_get_new_leaderboard_user_key(void)
{
	const char * url = NULL;
	char * user_key = NULL;

	url = al_get_config_value(t3f_config, "Config", "leaderboard_key_url");
	if(!url)
	{
		goto fail;
	}
	user_key = t3net_get_new_leaderboard_user_key(url, NULL);
	if(!user_key)
	{
		goto fail;
	}
	al_set_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_key_user_key, user_key);
	free(user_key);

	return al_get_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_key_user_key);

	fail:
	{
		if(user_key)
		{
			free(user_key);
		}
		return NULL;
	}
}

int lingo_obfuscate_score(int score)
{
	return score * 3 + 'w' + 'o' + 'r' + 'd' + '5';
}

int lingo_unobfuscate_score(int score)
{
	return (score - 'w' - 'o' - 'r' - 'd' - '5') / 3;
}

bool lingo_verify_score(int score)
{
	return (score - 'w' - 'o' - 'r' - 'd' - '5') % 3 == 0;
}

void lingo_store_high_score(int high_score)
{
	const char * val;
	char buf[64];
	int stored_high_score = 0;

	val = al_get_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_high_score);
	if(val)
	{
		stored_high_score = atoi(val);
	}
	high_score = lingo_obfuscate_score(high_score);
	if(high_score > stored_high_score)
	{
		sprintf(buf, "%d", high_score);
		al_set_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_high_score, buf);
	}
}

bool lingo_get_leaderboard(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	const char * val;
	const char * url;
	const char * user_key;
	const char * user_name;
	int high_score = 0;
	int leaderboard_score = 0;
	char buf[64];

	if(instance->leaderboard)
	{
		t3net_destroy_leaderboard(instance->leaderboard);
		instance->leaderboard = NULL;
	}
	/* ensure we have a user key */
	user_key = al_get_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_key_user_key);
	if(!user_key)
	{
		user_key = _lingo_get_new_leaderboard_user_key();
	}
	if(!user_key)
	{
		goto fail;
	}

	/* update leaderboard user name if changed */
	user_name = al_get_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_key_user_name);
	if(!user_name || strcmp(user_name, instance->player[0].name))
	{
		url = al_get_config_value(t3f_config, "Config", "leaderboard_username_url");
		if(!url)
		{
			goto fail;
		}
		if(!t3net_update_leaderboard_user_name(url, user_key, instance->player[0].name))
		{
			goto fail;
		}
		al_set_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_key_user_name, instance->player[0].name);
	}

	/* upload unuploaded high score */
	val = al_get_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_high_score);
	if(val)
	{
		high_score = atoi(val);
	}
	val = al_get_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_uploaded_score);
	if(val)
	{
		leaderboard_score = atoi(val);
	}
	if(high_score > leaderboard_score)
	{
		if(lingo_verify_score(high_score))
		{
			url = al_get_config_value(t3f_config, "Config", "leaderboard_upload_url");
			if(!url)
			{
				goto fail;
			}
			if(!t3net_upload_score(url, "word5", "1.4", "normal", "0", user_key, high_score, NULL))
			{
				goto fail;
			}
		}
		sprintf(buf, "%d", high_score);
		al_set_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_uploaded_score, buf);
	}

	/* download leaderboard if above steps completed successfully */
	url = al_get_config_value(t3f_config, "Config", "leaderboard_download_url");
	if(!url)
	{
		goto fail;
	}
	instance->leaderboard = t3net_get_leaderboard(url, "word5", "1.4", "normal", "0", 10, 0);

	return instance->leaderboard;

	fail:
	{
		return false;
	}
}

void lingo_leaderboard_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i, ilen;
	int mx, mex;
	int last_item = instance->menu[instance->current_menu].current_item;
	
	instance->menu[instance->current_menu].current_item = -1;
	for(i = 0; i < instance->menu[instance->current_menu].items; i++)
	{
		ilen = al_get_text_width(instance->menu[instance->current_menu].item[i].font, instance->menu[instance->current_menu].item[i].name);
		if(instance->menu[instance->current_menu].item[i].flags & LINGO_MENU_ITEM_FLAG_CENTER)
		{
			mx = instance->menu[instance->current_menu].x + instance->menu[instance->current_menu].item[i].ox - ilen / 2;
			mex = instance->menu[instance->current_menu].x + instance->menu[instance->current_menu].item[i].ox + ilen / 2;
		}
		else
		{
			mx = instance->menu[instance->current_menu].x + instance->menu[instance->current_menu].item[i].ox;
			mex = instance->menu[instance->current_menu].x + instance->menu[instance->current_menu].item[i].ox + ilen;
		}
		if(t3f_get_mouse_x() >= mx && t3f_get_mouse_x() <= mex && t3f_get_mouse_y() >= instance->menu[instance->current_menu].y + instance->menu[instance->current_menu].item[i].oy && t3f_get_mouse_y() <= instance->menu[instance->current_menu].y + instance->menu[instance->current_menu].item[i].oy + al_get_font_line_height(instance->menu[instance->current_menu].item[i].font))
		{
			if(last_item != i)
			{
				al_play_sample(instance->sample[LINGO_SAMPLE_MENU_HOVER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			instance->menu[instance->current_menu].current_item = i;
			break;
		}
	}
	if(t3f_mouse_button_pressed(0) && !instance->mouse_clicked && instance->menu[instance->current_menu].current_item >= 0)
	{
		if(instance->menu[instance->current_menu].item[instance->menu[instance->current_menu].current_item].proc)
		{
//			al_play_sample(instance->sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			instance->menu[instance->current_menu].item[instance->menu[instance->current_menu].current_item].proc(data);
		}
		if(instance->menu[instance->current_menu].item[instance->menu[instance->current_menu].current_item].child_menu != -1)
		{
//			al_play_sample(instance->sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			instance->current_menu = instance->menu[instance->current_menu].item[instance->menu[instance->current_menu].current_item].child_menu;
			instance->menu[instance->current_menu].current_item = -1;
		}
		instance->mouse_clicked = 1;
		t3f_use_mouse_button_press(0);
	}
}

static void lingo_render_leaderboard_name(int i, const char * name, int score, ALLEGRO_COLOR color, void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	if(strlen(name) > 0)
	{
		al_draw_text(instance->font[LINGO_FONT_SPRINT_20], al_map_rgba(0, 0, 0, 128), 320 + 2 - 240, i * 32 + 2 + 64, ALLEGRO_ALIGN_LEFT, name);
		al_draw_text(instance->font[LINGO_FONT_SPRINT_20], color, 320 - 240, i * 32 + 64, ALLEGRO_ALIGN_LEFT, name);
		al_draw_textf(instance->font[LINGO_FONT_SPRINT_20], al_map_rgba(0, 0, 0, 128), 320 + 2 + 240, i * 32 + 2 + 64, ALLEGRO_ALIGN_RIGHT, "%d", lingo_unobfuscate_score(score));
		al_draw_textf(instance->font[LINGO_FONT_SPRINT_20], color, 320 + 240, i * 32 + 64, ALLEGRO_ALIGN_RIGHT, "%d", lingo_unobfuscate_score(score));
	}
	else
	{
		al_draw_text(instance->font[LINGO_FONT_SPRINT_20], al_map_rgba(0, 0, 0, 128), 320 + 2, i * 32 + 2 + 64, ALLEGRO_ALIGN_CENTRE, "...");
		al_draw_text(instance->font[LINGO_FONT_SPRINT_20], color, 320, i * 32 + 64, ALLEGRO_ALIGN_CENTRE, "...");
	}
}

void lingo_leaderboard_render(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i;
	char * text;
	int x, y;
	int mx;
	ALLEGRO_COLOR color;
	
	if(instance->current_menu == LINGO_MENU_LEADERBOARD_VIEW)
	{
		lingo_title_render(data);
	}
	else
	{
		lingo_game_render(data);
	}
/*	al_hold_bitmap_drawing(true);
	t3f_3d_draw_bitmap(instance->image[LINGO_IMAGE_BG], LINGO_COLOR_WHITE, 0, 0, 0, 0);
	t3f_3d_draw_bitmap(instance->image[LINGO_IMAGE_LOGO], LINGO_COLOR_WHITE, 320 - al_get_bitmap_width(instance->image[LINGO_IMAGE_LOGO]) / 2, instance->game_logo_y, 0, 0);
	t3f_3d_draw_bitmap(instance->image[LINGO_IMAGE_GAMEBOARD], LINGO_COLOR_WHITE, LINGO_GAMEBOARD_X_OFFSET, LINGO_GAMEBOARD_Y_OFFSET, instance->game_board_z, 0); */
	al_hold_bitmap_drawing(false);
	
	al_draw_filled_rectangle(0, 0, 640, 480, al_map_rgba_f(0.0, 0.0, 0.0, 0.75));
	
	al_draw_text(instance->font[LINGO_FONT_SPRINT_20], al_map_rgba(0, 0, 0, 128), 320 + 2, 8 + 2, ALLEGRO_ALIGN_CENTRE, "Global Leaderboard");
	al_draw_text(instance->font[LINGO_FONT_SPRINT_20], al_map_rgba(96, 255, 96, 255), 320, 8, ALLEGRO_ALIGN_CENTRE, "Global Leaderboard");
	
	for(i = 0; i < instance->leaderboard->entries; i++)
	{
		if(i == instance->leaderboard_place && (instance->logic_counter / 6) % 2 == 0)
		{
			color = al_map_rgba(255, 244, 141, 255);
		}
		else
		{
			color = al_map_rgba(255, 255, 255, 255);
		}
		lingo_render_leaderboard_name(i, instance->leaderboard->entry[i]->name, instance->leaderboard->entry[i]->score, color, data);
	}
	for(i = instance->leaderboard->entries; i < 10; i++)
	{
		color = al_map_rgba(255, 255, 255, 255);
		lingo_render_leaderboard_name(i, "", instance->leaderboard->entry[i]->score, color, data);
	}
	
	/* draw the in-game menu */
	for(i = 0; i < instance->menu[instance->current_menu].items; i++)
	{
		text = instance->menu[instance->current_menu].item[i].name;
		x = instance->menu[instance->current_menu].x + instance->menu[instance->current_menu].item[i].ox;
		y = instance->menu[instance->current_menu].y + instance->menu[instance->current_menu].item[i].oy;
		if(instance->menu[instance->current_menu].item[i].flags & LINGO_MENU_ITEM_FLAG_CENTER)
		{
			mx = x - al_get_text_width(instance->menu[instance->current_menu].item[i].font, text) / 2;
		}
		else
		{
			mx = x;
		}
		if(i == instance->menu[instance->current_menu].current_item)
		{
			al_draw_text(instance->menu[instance->current_menu].item[i].font, al_map_rgba(0, 0, 0, 128), mx + 2, y + 2, 0, text);
			al_draw_text(instance->menu[instance->current_menu].item[i].font, al_map_rgba(255, 255, 255, 255), mx - 2, y - 2, 0, text);
		}
		else
		{
			al_draw_text(instance->menu[instance->current_menu].item[i].font, al_map_rgba(0, 0, 0, 128), mx + 2, y + 2, 0, text);
			al_draw_text(instance->menu[instance->current_menu].item[i].font, al_map_rgba(255, 244, 141, 255), mx, y, 0, text);
		}
	}
}
