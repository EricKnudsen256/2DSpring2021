#ifndef __EDITOR_MENU_H__
#define __EDITOR_MENU_H__

#include "g_menu.h"

/**
* @brief creates a new editor menu
* @param buttonMax the max amount of buttons in the button list
* @return a pointer to the new editor menu
*/
Menu *editor_new(int buttonMax);

/**
* @brief creates all buttons for the menu
* @param editorMenu pointer to the editor menu
*/
void editor_create_buttons(Menu *editorMenu);

/**
* @brief creates save button for the menu
* @param editorMenu the pointer to the menu
*/
void editor_save_button(Menu *editorMenu);

/**
* @brief creates button to clear the editor
* @param editorMenu pointer to the editor menu
*/
void editor_clear_button(Menu *editorMenu);

/**
* @brief onClick for the save button
* @param button the button that was pressed
* @param editorMenu pointer to the editor menu
*/
void editor_save(Button* button, Menu *editorMenu);

/**
* @brief onClick for the clear button
* @param button the button that was pressed
* @param editorMenu pointer to the editor menu
*/
void editor_clear(Button* button, Menu *editorMenu);

/**
* @brief called to change the data of the menu to normal
* @param button the button that was pressed
* @param editorMenu pointer to the editor menu
*/
void editor_clear_done(Button* button, Menu *editorMenu);

/**
* @brief returns the data string of the menu
* @param editorMenu pointer to the menu
* @return data string of the menu
*/
char *editor_get_data(Menu *editorMenu);




#endif