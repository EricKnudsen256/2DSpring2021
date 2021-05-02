#ifndef __INVENTORY_MENU_H__
#define __INVENTORY_MENU_H__

#include "g_menu.h"

Menu *inventory_new(int buttonMax);

void inventory_create_slots(Menu *inventoryMenu);

void inventory_set_active(Menu *inventoryMenu);

void inventory_set_inactive(Menu *inventoryMenu);

void inventory_think(Menu *inventoryMenu);

void inventory_button_select(Button *slot, Menu *inventoryMenu);

void inventory_deselect_all(Menu *inventoryMenu);

void inventory_draw(Menu *inventoryMenu);

#endif