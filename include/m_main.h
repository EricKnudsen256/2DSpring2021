#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "g_menu.h"

/**
* @brief creates a new main menu
* @param buttonMax the max amount of buttons in the button list
* @return a pointer to the new main menu
*/
Menu *main_new(int buttonMax);

/**
* @brief creates all buttons for the menu
* @param mainMenu pointer to the main menu
*/
void main_create_buttons(Menu *mainMenu);

/**
* @brief creates new game button for the menu
* @param mainMenu the pointer to the menu
*/
void main_new_game(Menu *mainMenu);

/**
* @brief creates edit game button for the menu
* @param mainMenu the pointer to the menu
*/
void main_exit(Menu *mainMenu);

/**
* @brief creates editor button for the menu
* @param mainMenu the pointer to the menu
*/
void main_editor(Menu *mainMenu);

/**
* @brief onClick to go to the main game
* @param button the button that was pressed
* @param mainMenu pointer to the main menu
*/
void start_game(Button* button, Menu *mainMenu);

/**
* @brief onClick to go to the editor
* @param button the button that was pressed
* @param mainMenu pointer to the main menu
*/
void start_editor(Button* button, Menu *mainMenu);

/**
* @brief onClick to go to exit the game
* @param button the button that was pressed
* @param mainMenu pointer to the main menu
*/
void close_game(Button* button, Menu *mainMenu);

/**
* @brief called to reset values to go back to the main menu
* @param mainMenu pointer to the main menu
*/
void back_to_main(Menu *mainMenu);

/**
* @brief checks to see if the game should start,
* @param mainMenu pointer to the main menu
* @return true if the game should start, false otherwise
*/
Bool check_start_game(Menu *mainMenu);


#endif