#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "g_menu.h"

Menu *main_new(int buttonMax);

void main_create_buttons(Menu *mainMenu);

void main_new_game(Menu *mainMenu);

void main_exit(Menu *mainMenu);

void start_game(Menu *mainMenu);

void close_game(Menu *mainMenu);

Bool check_start_game(Menu *mainMenu);


#endif