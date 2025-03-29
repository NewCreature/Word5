#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "t3f/t3f.h"
#include "instance.h"
#include "main.h"
#include "title.h"
#include "menu_proc.h"
#include "draw_text.h"

void lingo_menu_create(LINGO_MENU * mp, char * name, int parent, int x, int y, int flags)
{
	mp->items = 0;
	mp->x = x;
	mp->y = y;
	mp->flags = flags;
	strcpy(mp->name, name);
	mp->parent_menu = parent;
	mp->current_item = -1;
}

void lingo_menu_add_item(LINGO_MENU * mp, T3F_FONT * fp, char * name, int child, void (*proc)(), int ox, int oy, int flags)
{
	mp->item[mp->items].child_menu = child;
	mp->item[mp->items].proc = proc;
	mp->item[mp->items].ox = ox;
	mp->item[mp->items].oy = oy;
	strcpy(mp->item[mp->items].name, name);
	mp->item[mp->items].font = fp;
	mp->item[mp->items].flags = flags;
	mp->items++;
}

void lingo_title_initialize(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	lingo_menu_create(&instance->menu[LINGO_MENU_MAIN], "", -1, 320, 230, 0);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_MAIN], instance->font[LINGO_FONT_SPRINT_20], "Play", -1, lingo_menu_proc_main_play, 0, 0, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_MAIN], instance->font[LINGO_FONT_SPRINT_20], "Options", -1, lingo_menu_proc_main_options, 0, 32, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_MAIN], instance->font[LINGO_FONT_SPRINT_20], "Leaderboard", -1, lingo_menu_proc_main_leaderboard, 0, 64, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_MAIN], instance->font[LINGO_FONT_SPRINT_20], "Tutorial", -1, lingo_menu_proc_main_tutorial, 0, 96, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_MAIN], instance->font[LINGO_FONT_SPRINT_20], "Quit", -1, lingo_menu_proc_main_quit, 0, 128, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&instance->game_menu[LINGO_GAME_MENU_MAIN], "", -1, 0, 0, 0);
	lingo_menu_add_item(&instance->game_menu[LINGO_GAME_MENU_MAIN], instance->font[LINGO_FONT_SPRINT_20], "Use Hint", -1, lingo_menu_proc_game_main_bonus_letter, 150, 170 + 118 + 24 + 12, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->game_menu[LINGO_GAME_MENU_MAIN], instance->font[LINGO_FONT_SPRINT_20], "Exit Game", -1, lingo_menu_proc_game_main_quit, 150, 170 + 118 + 130, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&instance->game_menu[LINGO_GAME_MENU_MAIN_DUMMY], "", -1, 0, 0, 0);
	lingo_menu_add_item(&instance->game_menu[LINGO_GAME_MENU_MAIN_DUMMY], instance->font[LINGO_FONT_SPRINT_20], "Use Hint", -1, NULL, 150, 170 + 118 + 24 + 12, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->game_menu[LINGO_GAME_MENU_MAIN_DUMMY], instance->font[LINGO_FONT_SPRINT_20], "Exit Game", -1, NULL, 150, 170 + 118 + 130, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&instance->game_menu[LINGO_GAME_MENU_OVER], "", -1, 0, 0, 0);
	lingo_menu_add_item(&instance->game_menu[LINGO_GAME_MENU_OVER], instance->font[LINGO_FONT_SPRINT_20], "Play Again", -1, lingo_menu_proc_game_over_play_again, 150, 170 + 118 + 130 - 36, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->game_menu[LINGO_GAME_MENU_OVER], instance->font[LINGO_FONT_SPRINT_20], "Exit", -1, lingo_menu_proc_game_over_exit, 150, 170 + 118 + 130, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&instance->menu[LINGO_MENU_LEADERBOARD], "", -1, 0, 0, 0);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_LEADERBOARD], instance->font[LINGO_FONT_SPRINT_20], "Play Again", -1, lingo_menu_proc_leaderboard_play_again, 320, 170 + 118 + 130 - 16, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_LEADERBOARD], instance->font[LINGO_FONT_SPRINT_20], "Exit", -1, lingo_menu_proc_leaderboard_exit, 320, 170 + 118 + 130 + 20, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&instance->menu[LINGO_MENU_LEADERBOARD_VIEW], "", -1, 0, 0, 0);
//	lingo_menu_add_item(&instance->menu[LINGO_MENU_LEADERBOARD_VIEW], instance->font[LINGO_FONT_SPRINT_20], "Play Again", -1, lingo_menu_proc_leaderboard_play_again, 320, 170 + 118 + 130 - 16, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_LEADERBOARD_VIEW], instance->font[LINGO_FONT_SPRINT_20], "Back", -1, lingo_menu_proc_leaderboard_back, 320, 170 + 118 + 130 + 20, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&instance->menu[LINGO_MENU_ENTER_NAME], "", -1, 0, 0, 0);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_ENTER_NAME], instance->font[LINGO_FONT_SPRINT_20], "Enter Name", -1, NULL, 320, 170 + 118 - 36, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_ENTER_NAME], instance->font[LINGO_FONT_SPRINT_20], instance->player[0].name, -1, NULL, 320, 170 + 118, LINGO_MENU_ITEM_FLAG_ALT);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_ENTER_NAME], instance->font[LINGO_FONT_SPRINT_20], "Start Game", -1, lingo_menu_proc_main_start_game, 320, 170 + 118 + 36, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_ENTER_NAME], instance->font[LINGO_FONT_SPRINT_20], "Cancel", -1, lingo_menu_proc_main_cancel, 320, 170 + 118 + 72, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&instance->menu[LINGO_MENU_OPTIONS], "", -1, 320, 230, 0);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_OPTIONS], instance->font[LINGO_FONT_SPRINT_20], "Upload Scores", -1, NULL, 0, 0, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_OPTIONS], instance->font[LINGO_FONT_SPRINT_20], instance->upload_option_text, -1, lingo_menu_proc_options_upload, 0, 32, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&instance->menu[LINGO_MENU_OPTIONS], instance->font[LINGO_FONT_SPRINT_20], "Back", -1, lingo_menu_proc_options_back, 0, 64, LINGO_MENU_ITEM_FLAG_CENTER);
}

static void _lingo_activate_current_menu_item(LINGO_MENU * mp, void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	if(mp->item[mp->current_item].proc)
	{
		al_play_sample(instance->sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		mp->item[mp->current_item].proc(data);
	}
	else if(mp->item[mp->current_item].child_menu != -1)
	{
		al_play_sample(instance->sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		instance->current_menu = mp->item[mp->current_item].child_menu;
		 mp->current_item = -1;
	}
}

void lingo_title_menu_mouse_logic(LINGO_MENU * mp, void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i, ilen;
	int mx, mex;
	int last_item = mp->current_item;

	mp->current_item = -1;
	for(i = 0; i < mp->items; i++)
	{
		if(mp->item[i].proc)
		{
			ilen = t3f_get_text_width(mp->item[i].font, mp->item[i].name);
			if(mp->item[i].flags & LINGO_MENU_ITEM_FLAG_CENTER)
			{
				mx = mp->x + mp->item[i].ox - ilen / 2;
				mex = mp->x + mp->item[i].ox + ilen / 2;
			}
			else
			{
				mx = mp->x + mp->item[i].ox;
				mex = mp->x + mp->item[i].ox + ilen;
			}
			if(t3f_get_mouse_x() >= mx && t3f_get_mouse_x() <= mex && t3f_get_mouse_y() >= mp->y + mp->item[i].oy && t3f_get_mouse_y() <= mp->y + mp->item[i].oy + t3f_get_font_line_height(mp->item[i].font))
			{
				if(last_item != i)
				{
					al_play_sample(instance->sample[LINGO_SAMPLE_MENU_HOVER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				mp->current_item = i;
				break;
			}
		}
	}
	if(t3f_mouse_button_pressed(0))
	{
		if(mp->current_item >= 0)
		{
			_lingo_activate_current_menu_item(mp, data);
		}
		t3f_use_mouse_button_press(0);
	}
}

static void _lingo_title_mouse_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i, ilen;
	int mx, mex;
	int last_item = instance->menu[instance->current_menu].current_item;

	instance->menu[instance->current_menu].current_item = -1;
	for(i = 0; i < instance->menu[instance->current_menu].items; i++)
	{
		if(instance->menu[instance->current_menu].item[i].proc)
		{
			ilen = t3f_get_text_width(instance->menu[instance->current_menu].item[i].font, instance->menu[instance->current_menu].item[i].name);
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
			if(t3f_get_mouse_x() >= mx && t3f_get_mouse_x() <= mex && t3f_get_mouse_y() >= instance->menu[instance->current_menu].y + instance->menu[instance->current_menu].item[i].oy && t3f_get_mouse_y() <= instance->menu[instance->current_menu].y + instance->menu[instance->current_menu].item[i].oy + t3f_get_font_line_height(instance->menu[instance->current_menu].item[i].font))
			{
				if(last_item != i)
				{
					al_play_sample(instance->sample[LINGO_SAMPLE_MENU_HOVER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				instance->menu[instance->current_menu].current_item = i;
				break;
			}
		}
	}
	if(t3f_mouse_button_pressed(0))
	{
		if(instance->menu[instance->current_menu].current_item >= 0)
		{
			_lingo_activate_current_menu_item(&instance->menu[instance->current_menu], data);
		}
		t3f_use_mouse_button_press(0);
	}
}

static void _lingo_title_keyboard_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int current_item = instance->menu[instance->current_menu].current_item;

	if(t3f_key_pressed(ALLEGRO_KEY_DOWN) || instance->menu[instance->current_menu].current_item < 0)
	{
		do
		{
			instance->menu[instance->current_menu].current_item++;
			if(instance->menu[instance->current_menu].current_item >= instance->menu[instance->current_menu].items)
			{
				instance->menu[instance->current_menu].current_item = 0;
			}
			if(instance->menu[instance->current_menu].item[instance->menu[instance->current_menu].current_item].proc || instance->menu[instance->current_menu].item[instance->menu[instance->current_menu].current_item].child_menu >= 0)
			{
				break;
			}
		} while(instance->menu[instance->current_menu].current_item != current_item);
		t3f_use_key_press(ALLEGRO_KEY_DOWN);
	}
	if(t3f_key_pressed(ALLEGRO_KEY_UP))
	{
		do
		{
			instance->menu[instance->current_menu].current_item--;
			if(instance->menu[instance->current_menu].current_item < 0)
			{
				instance->menu[instance->current_menu].current_item = instance->menu[instance->current_menu].items - 1;
			}
			if(instance->menu[instance->current_menu].item[instance->menu[instance->current_menu].current_item].proc || instance->menu[instance->current_menu].item[instance->menu[instance->current_menu].current_item].child_menu >= 0)
			{
				break;
			}
		} while(instance->menu[instance->current_menu].current_item != current_item);
		t3f_use_key_press(ALLEGRO_KEY_UP);
	}
	if(t3f_key_pressed(ALLEGRO_KEY_ENTER))
	{
		_lingo_activate_current_menu_item(&instance->menu[instance->current_menu], data);
		t3f_use_key_press(ALLEGRO_KEY_ENTER);
	}
}

void lingo_menu_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	if(t3f_key_pressed(ALLEGRO_KEY_ESCAPE))
	{
		instance->menu[instance->current_menu].current_item = instance->menu[instance->current_menu].items - 1;
		_lingo_activate_current_menu_item(&instance->menu[instance->current_menu], data);
		t3f_use_key_press(ALLEGRO_KEY_ESCAPE);
	}
	else
	{
		switch(instance->input_type)
		{
			case LINGO_INPUT_TYPE_MOUSE:
			{
				_lingo_title_mouse_logic(data);
				break;
			}
			case LINGO_INPUT_TYPE_KEYBOARD:
			{
				_lingo_title_keyboard_logic(data);
				break;
			}
		}
	}
}

void lingo_title_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	lingo_menu_logic(data);
	if((instance->logic_counter / 15) % 2 == 0)
	{
		sprintf(instance->menu[LINGO_MENU_ENTER_NAME].item[1].name, "%s_", instance->player[0].name);
	}
	else
	{
		sprintf(instance->menu[LINGO_MENU_ENTER_NAME].item[1].name, "%s", instance->player[0].name);
	}
	instance->menu[LINGO_MENU_ENTER_NAME].item[1].ox = instance->view->virtual_width / 2.0 - t3f_get_text_width(instance->menu[LINGO_MENU_ENTER_NAME].item[1].font, instance->player[0].name) / 2.0;
}

void lingo_title_transition_in_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	if(instance->title_logo_z < 0)
	{
		instance->title_logo_z += 20.0;
		if(instance->title_logo_z >= 0)
		{
			al_play_sample(instance->sample[LINGO_SAMPLE_LOGO], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			instance->state = LINGO_STATE_TITLE;
		}
	}
}

void lingo_title_transition_out_logic(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;

	if(instance->title_logo_z > -640)
	{
		instance->title_logo_z -= 20.0;
		if(instance->title_logo_z <= -640)
		{
			t3f_exit();
		}
	}
}

void lingo_title_transition_in_render(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	float alpha;

	t3f_select_view(t3f_default_view);
	alpha = 1.0 - (-instance->title_logo_z) / 640.0;
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_BG], t3f_color_white, 0, 0, 0, 0);
	lingo_select_view(data);
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_LOGO], al_map_rgba_f(alpha, alpha, alpha, alpha), 320 - instance->image[LINGO_IMAGE_LOGO]->target_width / 2.0, 70, instance->title_logo_z, 0);
}

void lingo_title_transition_out_render(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	float alpha;

	t3f_select_view(t3f_default_view);
	alpha = 1.0 - (-instance->title_logo_z) / 640.0;
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_BG], t3f_color_white, 0, 0, 0, 0);
	lingo_select_view(data);
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_LOGO], al_map_rgba_f(alpha, alpha, alpha, alpha), 320 - instance->image[LINGO_IMAGE_LOGO]->target_width / 2.0, 70, instance->title_logo_z, 0);
}

void lingo_title_render(void * data)
{
	APP_INSTANCE * instance = (APP_INSTANCE *)data;
	int i, x, y;
	char * text;
	int flags = 0;
	ALLEGRO_COLOR color;

	t3f_select_view(t3f_default_view);
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_BG], t3f_color_white, 0, 0, 0, 0);
	lingo_select_view(data);
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_LOGO], t3f_color_white, 320 - instance->image[LINGO_IMAGE_LOGO]->target_width / 2.0, 70, instance->title_logo_z, 0);
	for(i = 0; i < instance->menu[instance->current_menu].items; i++)
	{
		flags = 0;
		if(instance->menu[instance->current_menu].item[i].proc)
		{
			color = al_map_rgba(255, 244, 141, 255);
		}
		else
		{
			if(instance->menu[instance->current_menu].item[i].flags & LINGO_MENU_ITEM_FLAG_ALT)
			{
				color = al_map_rgba(255, 255, 0, 255);
			}
			else
			{
				color = al_map_rgba(96, 255, 96, 255);
			}
		}
		text = instance->menu[instance->current_menu].item[i].name;
		x = instance->menu[instance->current_menu].x + instance->menu[instance->current_menu].item[i].ox;
		y = instance->menu[instance->current_menu].y + instance->menu[instance->current_menu].item[i].oy;
		if(instance->menu[instance->current_menu].item[i].flags & LINGO_MENU_ITEM_FLAG_CENTER)
		{
			flags = T3F_FONT_ALIGN_CENTER;
		}
		if(i == instance->menu[instance->current_menu].current_item)
		{
			lingo_draw_text_with_shadow(instance->menu[instance->current_menu].item[i].font, color, al_map_rgba(0, 0, 0, 128), x - 2, y - 2, 0, 4, 4, flags, text);
		}
		else
		{
			lingo_draw_text_with_shadow(instance->menu[instance->current_menu].item[i].font, color, al_map_rgba(0, 0, 0, 128), x, y, 0, 2, 2, flags, text);
		}
	}
	lingo_draw_text_with_shadow(instance->font[LINGO_FONT_SPRINT_10], t3f_color_white, al_map_rgba(0, 0, 0, 128), instance->view->virtual_width / 2, instance->view->virtual_height - t3f_get_font_line_height(instance->font[LINGO_FONT_SPRINT_10]) - 2, 0, 2, 2, T3F_FONT_ALIGN_CENTER, T3F_APP_COPYRIGHT);
}
