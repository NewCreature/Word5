#include "data.h"
#include "main.h"

int lingo_leaderboard_place = -1;

void lingo_leaderboard_logic(void)
{
	int i, ilen;
	int mx, mex;
	int last_item = lingo_menu[lingo_current_menu].current_item;
	
	lingo_menu[lingo_current_menu].current_item = -1;
	for(i = 0; i < lingo_menu[lingo_current_menu].items; i++)
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
		if(t3f_mouse_x >= mx && t3f_mouse_x <= mex && t3f_mouse_y >= lingo_menu[lingo_current_menu].y + lingo_menu[lingo_current_menu].item[i].oy && t3f_mouse_y <= lingo_menu[lingo_current_menu].y + lingo_menu[lingo_current_menu].item[i].oy + al_get_font_line_height(lingo_menu[lingo_current_menu].item[i].font))
		{
			if(last_item != i)
			{
				al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_HOVER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			lingo_menu[lingo_current_menu].current_item = i;
			break;
		}
	}
	if(t3f_mouse_button[0] && !lingo_mouse_clicked && lingo_menu[lingo_current_menu].current_item >= 0)
	{
		if(lingo_menu[lingo_current_menu].item[lingo_menu[lingo_current_menu].current_item].proc)
		{
//			al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			lingo_menu[lingo_current_menu].item[lingo_menu[lingo_current_menu].current_item].proc();
		}
		if(lingo_menu[lingo_current_menu].item[lingo_menu[lingo_current_menu].current_item].child_menu != -1)
		{
//			al_play_sample(lingo_sample[LINGO_SAMPLE_MENU_CLICK], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			lingo_current_menu = lingo_menu[lingo_current_menu].item[lingo_menu[lingo_current_menu].current_item].child_menu;
			lingo_menu[lingo_current_menu].current_item = -1;
		}
		lingo_mouse_clicked = 1;
	}
}

void lingo_leaderboard_render(void)
{
	int i;
	char * text;
	int x, y;
	int ilen;
	int mx, mex;
	ALLEGRO_COLOR color;
	
	if(lingo_current_menu == LINGO_MENU_LEADERBOARD_VIEW)
	{
		lingo_title_render();
	}
	else
	{
		lingo_game_render();
	}
/*	al_hold_bitmap_drawing(true);
	t3f_3d_draw_bitmap(lingo_image[LINGO_IMAGE_BG], LINGO_COLOR_WHITE, 0, 0, 0, 0);
	t3f_3d_draw_bitmap(lingo_image[LINGO_IMAGE_LOGO], LINGO_COLOR_WHITE, 320 - al_get_bitmap_width(lingo_image[LINGO_IMAGE_LOGO]) / 2, lingo_game_logo_y, 0, 0);
	t3f_3d_draw_bitmap(lingo_image[LINGO_IMAGE_GAMEBOARD], LINGO_COLOR_WHITE, LINGO_GAMEBOARD_X_OFFSET, LINGO_GAMEBOARD_Y_OFFSET, lingo_game_board_z, 0); */
	al_hold_bitmap_drawing(false);
	
	al_draw_filled_rectangle(0, 0, 640, 480, al_map_rgba_f(0.0, 0.0, 0.0, 0.75));
	
	al_draw_text(lingo_font[LINGO_FONT_SPRINT_20], al_map_rgba(0, 0, 0, 128), 320 + 2, 8 + 2, ALLEGRO_ALIGN_CENTRE, "Global Leaderboard");
	al_draw_text(lingo_font[LINGO_FONT_SPRINT_20], al_map_rgba(96, 255, 96, 255), 320, 8, ALLEGRO_ALIGN_CENTRE, "Global Leaderboard");
	
	for(i = 0; i < 10; i++)
	{
		if(i == lingo_leaderboard_place && (lingo_logic_counter / 6) % 2 == 0)
		{
			color = al_map_rgba(255, 244, 141, 255);
		}
		else
		{
			color = al_map_rgba(255, 255, 255, 255);
		}
		if(strlen(lingo_leaderboard->entry[i]->name) > 0)
		{
			al_draw_text(lingo_font[LINGO_FONT_SPRINT_20], al_map_rgba(0, 0, 0, 128), 320 + 2 - 240, i * 32 + 2 + 64, ALLEGRO_ALIGN_LEFT, lingo_leaderboard->entry[i]->name);
			al_draw_text(lingo_font[LINGO_FONT_SPRINT_20], color, 320 - 240, i * 32 + 64, ALLEGRO_ALIGN_LEFT, lingo_leaderboard->entry[i]->name);
			al_draw_textf(lingo_font[LINGO_FONT_SPRINT_20], al_map_rgba(0, 0, 0, 128), 320 + 2 + 240, i * 32 + 2 + 64, ALLEGRO_ALIGN_RIGHT, "%lu", (lingo_leaderboard->entry[i]->score - 'f' - 'l' - 'o' - 'g' - 'v') / 2);
			al_draw_textf(lingo_font[LINGO_FONT_SPRINT_20], color, 320 + 240, i * 32 + 64, ALLEGRO_ALIGN_RIGHT, "%lu", (lingo_leaderboard->entry[i]->score - 'f' - 'l' - 'o' - 'g' - 'v') / 2);
		}
		else
		{
			al_draw_text(lingo_font[LINGO_FONT_SPRINT_20], al_map_rgba(0, 0, 0, 128), 320 + 2, i * 32 + 2 + 64, ALLEGRO_ALIGN_CENTRE, "...");
			al_draw_text(lingo_font[LINGO_FONT_SPRINT_20], color, 320, i * 32 + 64, ALLEGRO_ALIGN_CENTRE, "...");
		}
	}
	
	/* draw the in-game menu */
	for(i = 0; i < lingo_menu[lingo_current_menu].items; i++)
	{
		text = lingo_menu[lingo_current_menu].item[i].name;
		x = lingo_menu[lingo_current_menu].x + lingo_menu[lingo_current_menu].item[i].ox;
		y = lingo_menu[lingo_current_menu].y + lingo_menu[lingo_current_menu].item[i].oy;
		ilen = al_get_text_width(lingo_menu[lingo_current_menu].item[i].font, text);
		if(lingo_menu[lingo_current_menu].item[i].flags & LINGO_MENU_ITEM_FLAG_CENTER)
		{
			mx = x - al_get_text_width(lingo_menu[lingo_current_menu].item[i].font, text) / 2;
			mex = lingo_menu[lingo_current_menu].x + lingo_menu[lingo_current_menu].item[i].ox + ilen / 2;
		}
		else
		{
			mx = x;
			mex = lingo_menu[lingo_current_menu].x + lingo_menu[lingo_current_menu].item[i].ox + ilen;
		}
		if(i == lingo_menu[lingo_current_menu].current_item)
		{
			al_draw_text(lingo_menu[lingo_current_menu].item[i].font, al_map_rgba(0, 0, 0, 128), mx + 2, y + 2, 0, text);
			al_draw_text(lingo_menu[lingo_current_menu].item[i].font, al_map_rgba(255, 255, 255, 255), mx - 2, y - 2, 0, text);
		}
		else
		{
			al_draw_text(lingo_menu[lingo_current_menu].item[i].font, al_map_rgba(0, 0, 0, 128), mx + 2, y + 2, 0, text);
			al_draw_text(lingo_menu[lingo_current_menu].item[i].font, al_map_rgba(255, 244, 141, 255), mx, y, 0, text);
		}
	}
}
