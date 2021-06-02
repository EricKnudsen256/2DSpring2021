#ifndef __BUILD_MENU_H__
#define __BUILD_MENU_H__

#include "g_menu.h"

/**
* @brief creates a new inventory menu
* @param buttonMax the max amount of buttons in the button list
* @return a pointer to the new inventory menu
*/
Menu *build_new(int buttonMax);

/**
* @brief creates all slots for the inventory
* @param inventoryMenu pointer to the inventory menu
*/
void build_create_slots(Menu *buildMenu);


void build_choose_item(Button *button, Menu *buildMenu);

/**
* @brief sets inventory as active
* @param button the button that was pressed
* @param inventoryMenu pointer to the inventory menu
*/
void build_set_active(Button *button, Menu *buildMenu);

/**
* @brief sets inventory as inactive
* @param button the button that was pressed
* @param inventoryMenu pointer to the inventory menu
*/
void build_set_inactive(Button *button, Menu *buildMenu);

/**
* @brief called every frame to update the inventory based on player interaction
* @param inventoryMenu pointer to the inventory menu
*/
void build_think(Menu *buildMenu);

/**
* @brief sets an inventory slot as selected, will check if 2 slots are selected, if so, swaps items
* @param slot the inventory slot the was clicked
* @param inventoryMenu pointer to the inventory menu
*/
void build_button_select(Button *slot, Menu *buildMenu);

/**
* @brief sets all inventory buttons as not selected
* @param inventoryMenu pointer to the inventory menu
*/
void build_deselect_all(Menu *buildMenu);

/**
* @brief draws the menu, special for the inventory
* @param inventoryMenu pointer to the inventory menu
*/
void build_draw(Menu *buildMenu);

#endif