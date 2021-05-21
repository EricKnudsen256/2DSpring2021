#ifndef __MINIMAP_MENU_H__
#define __MINIMAP_MENU_H__

#include "g_menu.h"

/**
* @brief creates a new minimap menu
* @param buttonMax the max amount of buttons in the button list
* @return a pointer to the new pause menu
*/
Menu *minimap_menu_new(int buttonMax);

/**
* @brief sets the menu as active
* @param button the button that was pressed
* @param minimap the minimap menu
*/
void minimap_menu_set_active(Button* button, Menu *minimap);

/**
* @brief sets the menu as inactive
* @param button the button that was pressed
* @param minimap the minimap menu
*/
void minimap_menu_set_inactive(Button* button, Menu *minimap);

/**
* @brief special draw function for the minimap
* @param minimap the minimap menu
*/
void minimap_draw(Menu *minimap);

#endif