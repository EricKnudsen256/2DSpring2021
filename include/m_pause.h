#ifndef __PAUSE_MENU_H__
#define __PAUSE_MENU_H__

#include "g_menu.h"

Menu *pause_menu_new(int buttonMax);

void *pause_menu_set_active(Menu *pause);

void *pause_menu_set_inactive(Menu *pause);

Menu *pause_menu_free(Menu *pause);

#endif