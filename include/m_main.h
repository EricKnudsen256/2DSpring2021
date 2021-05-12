#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "g_menu.h"

Menu *main_new(int buttonMax);

void main_create_buttons(Menu *mainMenu);

void main_new_game(Menu *mainMenu);

void main_exit(Menu *mainMenu);

void main_editor(Menu *mainMenu);

void start_game(Button* button, Menu *mainMenu);

void start_editor(Button* button, Menu *mainMenu);

void close_game(Button* button, Menu *mainMenu);

void back_to_main(Menu *mainMenu);

Bool check_start_game(Menu *mainMenu);


#endif