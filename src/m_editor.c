#include "simple_logger.h"

#include "w_room.h"

#include "m_editor.h"


Menu *editor_new(int buttonMax)
{
	Menu *editorMenu;

	editorMenu = menu_new();

	editorMenu->position = vector2d(1520, 700);
	gfc_rect_set(editorMenu->windowSize, editorMenu->position.x, editorMenu->position.y, 300, 300);


	editorMenu->buttonMax = buttonMax;
	editorMenu->buttonList = (Button *)gfc_allocate_array(sizeof (Button), editorMenu->buttonMax);
	editorMenu->sprite = gf2d_sprite_load_image("assets/sprites/menus/editorMenu.png");

	editorMenu->tag = "editor";

	editorMenu->_active = true;

	editorMenu->data = "none";

	editor_create_buttons(editorMenu);

	return editorMenu;
}

void editor_create_buttons(Menu *editorMenu)
{
	editor_save_button(editorMenu);
	editor_clear_button(editorMenu);
}

void editor_save_button(Menu *editorMenu)
{
	int editorIndex;
	editorIndex = menu_button_new(editorMenu);


	if (editorIndex == -1)
	{
		slog("Exit Button Not Created");
	}


	Button *button = editorMenu->buttonList[editorIndex];

	button->position.x = editorMenu->position.x + 50;
	button->position.y = editorMenu->position.y + 30;
	gfc_rect_set(button->buttonSize, button->position.x, button->position.y, 188, 96);
	button->sprite = gf2d_sprite_load_image("assets/sprites/menus/editorSave.png");


	button->buttonTag = "editor_save";
	button->onPress = editor_save;
}

void editor_clear_button(Menu *editorMenu)
{
	int editorIndex;
	editorIndex = menu_button_new(editorMenu);


	if (editorIndex == -1)
	{
		slog("Exit Button Not Created");
	}


	Button *button = editorMenu->buttonList[editorIndex];

	button->position.x = editorMenu->position.x + 50;
	button->position.y = editorMenu->position.y + 160;
	gfc_rect_set(button->buttonSize, button->position.x, button->position.y, 188, 96);
	button->sprite = gf2d_sprite_load_image("assets/sprites/menus/editorClear.png");


	button->buttonTag = "editor_clear";
	button->onPress = editor_clear;
}

void editor_save(Button* button, Menu *editorMenu)
{

	room_manager_save_template(room_manager_get_room(0, 0));
}

void editor_clear(Button* button, Menu *editorMenu)
{
	editorMenu->data = "clear";
}

void editor_clear_done(Button* button, Menu *editorMenu)
{
	editorMenu->data = "none";
}

char *editor_get_data(Menu *editorMenu)
{
	return editorMenu->data;
}

