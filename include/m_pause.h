#ifndef __PAUSE_MENU_H__
#define __PAUSE_MENU_H__

#include "g_menu.h"

/**
* @brief creates a new pause menu
* @param buttonMax the max amount of buttons in the button list
* @return a pointer to the new pause menu
*/
Menu *pause_menu_new(int buttonMax);

/**
* @brief creates exit button for the menu
* @param pauseMenu the pointer to the menu
* @return pointer to the new button
*/
Button *pause_menu_new_exit(Menu *pauseMenu);

/**
* @brief creates back button for the menu
* @param pauseMenu the pointer to the menu
* @return pointer to the new button
*/
Button *pause_menu_new_back(Menu *pauseMenu);

/**
* @brief sets the menu as active
* @param button the button that was pressed
* @param pause the pause menu
*/
void *pause_menu_set_active(Button* button, Menu *pause);

/**
* @brief sets the menu as inactive
* @param button the button that was pressed
* @param pause the pause menu
*/
void *pause_menu_set_inactive(Button* button, Menu *pause);

/**
* @brief sets the menu data to true to signal to go back to main menu
* @param button the button that was pressed
* @param pause the pause menu
*/
void *pause_menu_end_game(Button* button, Menu *pause);

/**
* @brief gets data value of the menu
* @param pause the menu
* @returns the value of the data field
*/
Bool *pause_menu_check_end_game(Menu *pause);

#endif