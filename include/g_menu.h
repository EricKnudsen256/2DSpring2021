#ifndef __MENU_H__
#define __MENU_H__

#include "gf2d_sprite.h"
#include "g_menu_objects.h"


typedef struct Menu_s
{
	Bool        _inuse;				/** flag for if menu is currently used */
	Bool		_active;			/** flag for if menu is currently active and should be drawn/updated */
	Vector2D	position;			
	SDL_Rect	windowSize;			/** rect showing the menu size */
	Sprite		*sprite;
	int			buttonMax;			/** max buttons for buttonList*/
	Button		**buttonList;

	int			managerIndex;		/** index of menu in menu manager */

	char		*tag;				/** string tag for each menu, used to find menus in the manager using menu_manager_get_by_tag()*/

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

/**
* @brief creates the menu manager
* @param max_menus number of maximum menus to support
*/
void menu_manager_init(Uint32 max_menus);


/**
* @brief called every frame to update all menus
*/
void menu_manager_update_menus();


/**
* @brief thinks all menues, called every frame
*/
void menu_manager_think_menus();


/**
* @brief checks every menu to see if it was clicked by the mouse
* @return true if there was a clicked menu, false if none were clicked
*/
Bool menu_manager_check_click();

/**
* @brief called every frame to draw each menu
*/
void menu_manager_draw_menus();


/**
* @brief gets a pointer to a menu with the specified tag
* @param tag the menu tag to check for
* @return NULL if not found, or a pointer to a menu
*/
Menu *menu_manager_get_by_tag(char* tag);


/**
* @brief frees the menu manager
*/
void menu_manager_free();


/**
* @brief allocates a new menu into the menu manager
* @return a pointer to the menu, NULL on fail
*/
Menu *menu_new();


/**
* @brief checks specific menu to see if it was clicked, called by manager. Checks buttons for click if true
* @param menu to check for a click
* @return true if menu was clicked, otherwise false
*/
Bool menu_check_click(Menu *menu);


/**
* @brief frees a menu fron the manager
* @param the menu to free
*/
void menu_free(Menu *menu);

/**
* @brief draws a menu, called by manager
* @param the menu to draw
*/
void menu_draw(Menu *menu);

/**
* @brief creates a new button in a menu's button list
* @param the menu to create a button for
* @return the int of the button's index in the button list
*/
int menu_button_new(Menu *menu);

/**
* @brief frees all buttons in a menu
* @param the menu to free the buttons of
*/
void menu_free_buttons(Menu *menu);

/**
* @brief checks if a specific button in a menu was clicked
* @param the menu to check
*/
void menu_buttons_check_click(Menu *menu);

/**
* @brief draws the buttons in a menu
* @param the menu to draw the buttons of
*/
void menu_buttons_draw(Menu *menu);


#endif