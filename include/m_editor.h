#ifndef __EDITOR_MENU_H__
#define __EDITOR_MENU_H__

#include "g_menu.h"

Menu *editor_new(int buttonMax);

void editor_create_buttons(Menu *editorMenu);

void editor_save_button(Menu *editorMenu);

void editor_clear_button(Menu *editorMenu);

void editor_save(Button* button, Menu *editorMenu);

void editor_clear(Button* button, Menu *editorMenu);

void editor_clear_done(Button* button, Menu *editorMenu);

char *editor_get_data(Menu *editorMenu);




#endif