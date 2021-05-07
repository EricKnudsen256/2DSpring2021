#ifndef __MINIMAP_MENU_H__
#define __MINIMAP_MENU_H__

#include "g_menu.h"

Menu *minimap_menu_new(int buttonMax);

void *minimap_menu_set_active(Button* button, Menu *minimap);

void *minimap_menu_set_inactive(Button* button, Menu *minimap);

void *minimap_draw(Menu *minimap);

#endif