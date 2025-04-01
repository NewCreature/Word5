#include "t3f/t3f.h"
#include "modules/draw_text.h"
#include "main.h"
#include "title.h"
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

int lingo_load_high_score(void)
{
	const char * val;
	int score = 0;

	val = al_get_config_value(t3f_user_data, _lingo_leaderboards_config_section, _lingo_leaderboards_config_high_score);
	if(val)
	{
		score = atoi(val);
		if(score)
		{
			score = lingo_unobfuscate_score(score);
		}
	}
	return score;
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
	lingo_menu_logic(data);
}

static void lingo_render_leaderboard_name(int i, const char * name, int score, ALLEGRO_COLOR color, void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	char buf[64];

	if(strlen(name) > 0)
	{
		lingo_draw_text_with_shadow(instance->font[LINGO_FONT_NORMAL], color, al_map_rgba(0, 0, 0, 128), 320 - 240, i * 32 + LINGO_LEADERBOARD_POS_Y, 0, 2, 2, 0, name);
		sprintf(buf, "%d", lingo_unobfuscate_score(score));
		lingo_draw_text_with_shadow(instance->font[LINGO_FONT_NORMAL], color, al_map_rgba(0, 0, 0, 128), 320 + 240, i * 32 + LINGO_LEADERBOARD_POS_Y, 0, 2, 2, T3F_FONT_ALIGN_RIGHT, buf);
	}
	else
	{
		lingo_draw_text_with_shadow(instance->font[LINGO_FONT_NORMAL], color, al_map_rgba(0, 0, 0, 128), 320, i * 32 + LINGO_LEADERBOARD_POS_Y, 0, 2, 2, T3F_FONT_ALIGN_CENTER, "...");
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
	al_hold_bitmap_drawing(false);
	
	al_draw_filled_rectangle(0, 0, 640, 480, al_map_rgba_f(0.0, 0.0, 0.0, 0.75));
	
	lingo_draw_text_with_shadow(instance->font[LINGO_FONT_NORMAL], al_map_rgba(96, 255, 96, 255), al_map_rgba(0, 0, 0, 128), 320, 4, 0, 2, 2, T3F_FONT_ALIGN_CENTER, "Global Leaderboard");
	
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
		lingo_render_leaderboard_name(i, "", 0, color, data);
	}
	
	/* draw the in-game menu */
	lingo_menu_render(data, 1.0);
}
