#ifndef __PAUSE_MENU_H__
#define __PAUSE_MENU_H__

#include "g_menu.h"

Menu *pause_menu_new(int buttonMax);

Button *pause_menu_new_exit(Menu *pauseMenu);

Button *pause_menu_new_back(Menu *pauseMenu);

void *pause_menu_set_active(Menu *pause);

void *pause_menu_set_inactive(Menu *pause);

void *pause_menu_end_game(Menu *pause);

Bool *pause_menu_check_end_game(Menu *pause);

#endif