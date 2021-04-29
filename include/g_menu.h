#ifndef __MENU_H__
#define __MENU_H__

#include "gf2d_sprite.h"
#include "g_menu_objects.h"


typedef struct Menu_s
{
	Bool        _inuse;
	Bool		_active;
	Vector2D	position;
	SDL_Rect	windowSize;
	Sprite		*sprite;
	int			buttonMax;
	Button		**buttonList;

	void		*data;				//void pointer for whatever I need to put in this. Example, pause menu has a boolean for if the game is active;


	void(*update)(struct Menu_s *menu);
	void(*think)(struct Menu_s *menu);
	void(*draw)(struct Menu_s *menu);
	void(*free)(struct Menu_s *menu);

}Menu;

typedef struct
{
	Menu *menu_list;
	Uint32  max_menus;
}MenuManager;

static MenuManager menu_manager = { 0 };

void menu_manager_init(Uint32 max_menus);

void menu_manager_update_menus();

void menu_manager_think_menus();

void menu_manager_check_click();

void menu_manager_draw_menus();

void menu_manager_free();

Menu *menu_new();

void menu_check_click(Menu *menu);

void menu_free(Menu *menu);

void menu_draw(Menu *menu);

int menu_button_new(Menu *menu);

void menu_free_buttons(Menu *menu);

void menu_buttons_check_click(Menu *menu);

void menu_buttons_draw(Menu *menu);

#endif