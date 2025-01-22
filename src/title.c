#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "t3f/t3f.h"
#include "main.h"
#include "data.h"
#include "title.h"
#include "menu_proc.h"

float lingo_title_logo_z = -640.0;

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

void lingo_menu_add_item(LINGO_MENU * mp, ALLEGRO_FONT * fp, char * name, int child, void (*proc)(), int ox, int oy, int flags)
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

void lingo_title_initialize(void)
{
	lingo_menu_create(&lingo_menu[LINGO_MENU_MAIN], "", -1, 320, 230, 0);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_MAIN], lingo_font[LINGO_FONT_SPRINT_20], "Play", -1, lingo_menu_proc_main_play, 0, 0, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_MAIN], lingo_font[LINGO_FONT_SPRINT_20], "Options", -1, lingo_menu_proc_main_options, 0, 32, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_MAIN], lingo_font[LINGO_FONT_SPRINT_20], "Leaderboard", -1, lingo_menu_proc_main_leaderboard, 0, 64, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_MAIN], lingo_font[LINGO_FONT_SPRINT_20], "Tutorial", -1, lingo_menu_proc_main_tutorial, 0, 96, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_MAIN], lingo_font[LINGO_FONT_SPRINT_20], "Quit", -1, lingo_menu_proc_main_quit, 0, 128, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&lingo_game_menu[LINGO_GAME_MENU_MAIN], "", -1, 0, 0, 0);
	lingo_menu_add_item(&lingo_game_menu[LINGO_GAME_MENU_MAIN], lingo_font[LINGO_FONT_SPRINT_20], "Use Hint", -1, lingo_menu_proc_game_main_bonus_letter, 150, 170 + 118 + 24 + 12, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_game_menu[LINGO_GAME_MENU_MAIN], lingo_font[LINGO_FONT_SPRINT_20], "Exit Game", -1, lingo_menu_proc_game_main_quit, 150, 170 + 118 + 130, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&lingo_game_menu[LINGO_GAME_MENU_MAIN_DUMMY], "", -1, 0, 0, 0);
	lingo_menu_add_item(&lingo_game_menu[LINGO_GAME_MENU_MAIN_DUMMY], lingo_font[LINGO_FONT_SPRINT_20], "Use Hint", -1, NULL, 150, 170 + 118 + 24 + 12, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_game_menu[LINGO_GAME_MENU_MAIN_DUMMY], lingo_font[LINGO_FONT_SPRINT_20], "Exit Game", -1, NULL, 150, 170 + 118 + 130, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&lingo_game_menu[LINGO_GAME_MENU_OVER], "", -1, 0, 0, 0);
	lingo_menu_add_item(&lingo_game_menu[LINGO_GAME_MENU_OVER], lingo_font[LINGO_FONT_SPRINT_20], "Play Again", -1, lingo_menu_proc_game_over_play_again, 150, 170 + 118 + 130 - 36, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_game_menu[LINGO_GAME_MENU_OVER], lingo_font[LINGO_FONT_SPRINT_20], "Exit", -1, lingo_menu_proc_game_over_exit, 150, 170 + 118 + 130, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&lingo_menu[LINGO_MENU_LEADERBOARD], "", -1, 0, 0, 0);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_LEADERBOARD], lingo_font[LINGO_FONT_SPRINT_20], "Play Again", -1, lingo_menu_proc_leaderboard_play_again, 320, 170 + 118 + 130 - 16, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_LEADERBOARD], lingo_font[LINGO_FONT_SPRINT_20], "Exit", -1, lingo_menu_proc_leaderboard_exit, 320, 170 + 118 + 130 + 20, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&lingo_menu[LINGO_MENU_LEADERBOARD_VIEW], "", -1, 0, 0, 0);
//	lingo_menu_add_item(&lingo_menu[LINGO_MENU_LEADERBOARD_VIEW], lingo_font[LINGO_FONT_SPRINT_20], "Play Again", -1, lingo_menu_proc_leaderboard_play_again, 320, 170 + 118 + 130 - 16, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_LEADERBOARD_VIEW], lingo_font[LINGO_FONT_SPRINT_20], "Back", -1, lingo_menu_proc_leaderboard_back, 320, 170 + 118 + 130 + 20, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&lingo_menu[LINGO_MENU_ENTER_NAME], "", -1, 0, 0, 0);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_ENTER_NAME], lingo_font[LINGO_FONT_SPRINT_20], "Enter Name", -1, NULL, 320, 170 + 118 - 36, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_ENTER_NAME], lingo_font[LINGO_FONT_SPRINT_20], lingo_player[0].name, -1, NULL, 320, 170 + 118, LINGO_MENU_ITEM_FLAG_CENTER | LINGO_MENU_ITEM_FLAG_ALT);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_ENTER_NAME], lingo_font[LINGO_FONT_SPRINT_20], "Start Game", -1, lingo_menu_proc_main_start_game, 320, 170 + 118 + 36, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_ENTER_NAME], lingo_font[LINGO_FONT_SPRINT_20], "Cancel", -1, lingo_menu_proc_main_cancel, 320, 170 + 118 + 72, LINGO_MENU_ITEM_FLAG_CENTER);

	lingo_menu_create(&lingo_menu[LINGO_MENU_OPTIONS], "", -1, 320, 230, 0);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_OPTIONS], lingo_font[LINGO_FONT_SPRINT_20], "Upload Scores", -1, NULL, 0, 0, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_OPTIONS], lingo_font[LINGO_FONT_SPRINT_20], lingo_upload_option_text, -1, lingo_menu_proc_options_upload, 0, 32, LINGO_MENU_ITEM_FLAG_CENTER);
	lingo_menu_add_item(&lingo_menu[LINGO_MENU_OPTIONS], lingo_font[LINGO_FONT_SPRINT_20], "Back", -1, lingo_menu_proc_options_back, 0, 64, LINGO_MENU_ITEM_FLAG_CENTER);
}

void lingo_title_logic(void)
{
	int i, ilen;
	int mx, mex;
	int last_item = lingo_menu[lingo_current_menu].current_item;

	lingo_menu[lingo_current_menu].current_item = -1;
	for(i = 0; i < lingo_menu[lingo_current_menu].items; i++)
	{
		if(lingo_menu[lingo_current_menu].item[i].proc)
		{
			ilen = al_get_text_width(lingo_menu[lingo_current_menu].item[i].font, lingo_menu[lingo_current_menu].item[i].name);
			if(lingo_menu[lingo_current_menu].item[i].flags & LINGO_MENU_ITEM_FLAG_CENTER)
			{
				mx = lingo_menu[lingo_current_menu].x + lingo_menu[lingo_current_menu].item[i].ox - ilen / 2;
				mex = lingo_menu[lingo_current_menu].x + lingo_menu[lingo_current_menu].item[i].ox + ilen / 2;
			}
			else
			{
				mx = lingo_menu[lingo_current_menu].x + lingo_menu[lingo_current_menu].item[i].ox;
				mex = lingo_menu[lingo_current_menu].x + lingo_menu[lingo_current_menu].item[i].ox + ilen;
			}
			if(t3f_get_mouse_x() >= mx && t3f_get_mouse_x() <= mex && t3f_get_mouse_y() >= lingo_menu[lingo_current_menu].y + lingo_menu[lingo_current_menu].item[i].oy && t3f_get_mouse_y() <= lingo_menu[lingo_current_menu].y + lingo_menu[lingo_current_menu].item[i].oy + al_get_font_line_height(lingo_menu[lingo_current_menu].item[i].font))
			{
				if(last_item != i)
				{
					al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_HOVER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				lingo_menu[lingo_current_menu].current_item = i;
				break;
			}
		}
	}
	if(t3f_mouse_button_pressed(0) && !lingo_mouse_clicked && lingo_menu[lingo_current_menu].current_item >= 0)
	{
		if(lingo_menu[lingo_current_menu].item[lingo_menu[lingo_current_menu].current_item].proc)
		{
			al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			lingo_menu[lingo_current_menu].item[lingo_menu[lingo_current_menu].current_item].proc();
		}
		else if(lingo_menu[lingo_current_menu].item[lingo_menu[lingo_current_menu].current_item].child_menu != -1)
		{
			al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			lingo_current_menu = lingo_menu[lingo_current_menu].item[lingo_menu[lingo_current_menu].current_item].child_menu;
			lingo_menu[lingo_current_menu].current_item = -1;
		}
		lingo_mouse_clicked = 1;
		t3f_use_mouse_button_press(0);
	}
	if((lingo_logic_counter / 15) % 2 == 0)
	{
		sprintf(lingo_menu[LINGO_MENU_ENTER_NAME].item[1].name, "%s_", lingo_player[0].name);
	}
	else
	{
		sprintf(lingo_menu[LINGO_MENU_ENTER_NAME].item[1].name, "%s ", lingo_player[0].name);
	}
}

void lingo_title_transition_in_logic(void)
{
	if(lingo_title_logo_z < 0)
	{
		lingo_title_logo_z += 20.0;
		if(lingo_title_logo_z >= 0)
		{
			al_play_sample(lingo_sample[LINGO_SAMPLE_LOGO], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			lingo_state = LINGO_STATE_TITLE;
		}
	}
}

void lingo_title_transition_out_logic(void)
{
	if(lingo_title_logo_z > -640)
	{
		lingo_title_logo_z -= 20.0;
		if(lingo_title_logo_z <= -640)
		{
			t3f_exit();
		}
	}
}

void lingo_title_transition_in_render(void)
{
	float alpha;

	t3f_select_view(t3f_default_view);
	alpha = 1.0 - (-lingo_title_logo_z) / 640.0;
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_BG], LINGO_COLOR_WHITE, 0, 0, 0, 0);
	lingo_select_view();
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_LOGO], al_map_rgba_f(alpha, alpha, alpha, alpha), 320 - lingo_image[LINGO_IMAGE_LOGO]->target_width / 2.0, 70, lingo_title_logo_z, 0);
}

void lingo_title_transition_out_render(void)
{
	float alpha;

	t3f_select_view(t3f_default_view);
	alpha = 1.0 - (-lingo_title_logo_z) / 640.0;
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_BG], LINGO_COLOR_WHITE, 0, 0, 0, 0);
	lingo_select_view();
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_LOGO], al_map_rgba_f(alpha, alpha, alpha, alpha), 320 - lingo_image[LINGO_IMAGE_LOGO]->target_width / 2.0, 70, lingo_title_logo_z, 0);
}

void lingo_title_render(void)
{
	int i, x, y;
	char * text;
	int mx;
	ALLEGRO_COLOR color;

	t3f_select_view(t3f_default_view);
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_BG], LINGO_COLOR_WHITE, 0, 0, 0, 0);
	lingo_select_view();
	t3f_draw_bitmap(lingo_image[LINGO_IMAGE_LOGO], LINGO_COLOR_WHITE, 320 - lingo_image[LINGO_IMAGE_LOGO]->target_width / 2.0, 70, lingo_title_logo_z, 0);
	for(i = 0; i < lingo_menu[lingo_current_menu].items; i++)
	{
		if(lingo_menu[lingo_current_menu].item[i].proc)
		{
			color = al_map_rgba(255, 244, 141, 255);
		}
		else
		{
			if(lingo_menu[lingo_current_menu].item[i].flags & LINGO_MENU_ITEM_FLAG_ALT)
			{
				color = al_map_rgba(255, 255, 0, 255);
			}
			else
			{
				color = al_map_rgba(96, 255, 96, 255);
			}
		}
		text = lingo_menu[lingo_current_menu].item[i].name;
		x = lingo_menu[lingo_current_menu].x + lingo_menu[lingo_current_menu].item[i].ox;
		y = lingo_menu[lingo_current_menu].y + lingo_menu[lingo_current_menu].item[i].oy;
		if(lingo_menu[lingo_current_menu].item[i].flags & LINGO_MENU_ITEM_FLAG_CENTER)
		{
			mx = x - al_get_text_width(lingo_menu[lingo_current_menu].item[i].font, text) / 2;
		}
		else
		{
			mx = x;
		}
		if(i == lingo_menu[lingo_current_menu].current_item)
		{
			al_draw_text(lingo_menu[lingo_current_menu].item[i].font, al_map_rgba(0, 0, 0, 128), mx + 2, y + 2, 0, text);
			al_draw_text(lingo_menu[lingo_current_menu].item[i].font, al_map_rgba(255, 255, 255, 255), mx - 2, y - 2, 0, text);
		}
		else
		{
			al_draw_text(lingo_menu[lingo_current_menu].item[i].font, al_map_rgba(0, 0, 0, 128), mx + 2, y + 2, 0, text);
			al_draw_text(lingo_menu[lingo_current_menu].item[i].font, color, mx, y, 0, text);
		}
	}
}
