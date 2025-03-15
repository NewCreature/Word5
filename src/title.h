#ifndef LINGO_TITLE_H
#define LINGO_TITLE_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#define LINGO_MAX_MENU_ITEMS 32
#define LINGO_MAX_MENUS      32

#define LINGO_MENU_MAIN             0
#define LINGO_MENU_PROFILE_1P       1
#define LINGO_MENU_LEADERBOARD      2
#define LINGO_MENU_ENTER_NAME       3
#define LINGO_MENU_LEADERBOARD_VIEW 4
#define LINGO_MENU_OPTIONS          5

#define LINGO_MENU_ITEM_FLAG_CENTER 1
#define LINGO_MENU_ITEM_FLAG_RIGHT  2
#define LINGO_MENU_ITEM_FLAG_ALT    4

typedef struct
{
	
	int ox, oy;
	
	char name[128];
	void (*proc)(void * data);
	ALLEGRO_FONT * font;
	
	int child_menu;
	
	int flags;
	
} LINGO_MENU_ITEM;

typedef struct
{
	
	int x, y;
	
	char name[128];
	LINGO_MENU_ITEM item[LINGO_MAX_MENU_ITEMS];
	short items;
	short current_item;
	
	int parent_menu;
	
	int flags;
	
} LINGO_MENU;

/* menu utility functions */
void lingo_menu_create(LINGO_MENU * mp, char * name, int parent, int x, int y, int flags);
void lingo_menu_add_item(LINGO_MENU * mp, ALLEGRO_FONT * fp, char * name, int child, void (*proc)(), int ox, int oy, int flags);

void lingo_title_build_profile_menus(void * data);

void lingo_title_initialize(void * data);
void lingo_title_transition_in_logic(void * data);
void lingo_title_transition_out_logic(void * data);
void lingo_title_logic(void * data);
void lingo_title_transition_in_render(void * data);
void lingo_title_transition_out_render(void * data);
void lingo_title_render(void * data);
void lingo_title_enter_name_logic(void * data);
void lingo_title_enter_name_render(void * data);

#endif
