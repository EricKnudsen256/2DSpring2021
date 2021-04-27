#include "simple_logger.h"

#include "m_pause.h"


Menu *pause_menu_new(int buttonMax)
{
	Menu *pauseMenu;
	Button *exit, *back;

	pauseMenu = menu_new();



	pauseMenu->position = vector2d(720, 400);
	gfc_rect_set(pauseMenu->windowSize, pauseMenu->position.x, pauseMenu->position.y, 480, 256);


	pauseMenu->sprite = gf2d_sprite_load_image("assets/sprites/menus/pauseMenu.png");

	pauseMenu->buttonMax = buttonMax;
	pauseMenu->buttonList = (Button *)gfc_allocate_array(sizeof (Button), pauseMenu->buttonMax);

	exit = pause_menu_new_exit(pauseMenu);
	//back = pause_menu_new_back(pauseMenu);


	return pauseMenu;

}

Button *pause_menu_new_exit(Menu *pauseMenu)
{
	
	int exitIndex;
	exitIndex = menu_button_new(pauseMenu);


	if (exitIndex == -1)
	{
		slog("EXIT BUTTON NOT ALLOCATED FOR PAUSE MENU");
	}

	
	Button *button = pauseMenu->buttonList[exitIndex];

	button->position.x = pauseMenu->position.x + 40;
	button->position.y = pauseMenu->position.y + 80;
	gfc_rect_set(button->buttonSize, button->position.x, button->position.y, 188, 96);
	button->sprite = gf2d_sprite_load_image("assets/sprites/menus/pauseMenuExit.png");


	button->buttonTag = "main_exit";
	button->onPress = pause_menu_set_inactive;

	return button;
}

Button *pause_menu_new_back(Menu *pauseMenu)
{
	int backIndex;
	backIndex = menu_button_new(pauseMenu);

	if (!backIndex)
	{
		slog("BACK BUTTON NOT ALLOCATED FOR PAUSE MENU");
	}

	Button *button = pauseMenu->buttonList[backIndex];

	button->buttonTag = "main_back";
	return button;
}

void *pause_menu_set_active(Menu *pause)
{
	slog("pressed 1");
	pause->_active = 1;
}

void *pause_menu_set_inactive(Menu *pause)
{
	slog("pressed 2");
	pause->_active = 0;

}

Menu *pause_menu_free(Menu *pause)
{

}