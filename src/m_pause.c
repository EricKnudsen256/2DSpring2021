#include "m_pause.h"

Menu *pause_menu_new(int buttonMax)
{
	Menu *pauseMenu;

	pauseMenu = menu_new();



	pauseMenu->position = vector2d(720, 400);
	gfc_rect_set(pauseMenu->windowSize, pauseMenu->position.x, pauseMenu->position.y, 480, 256);


	pauseMenu->sprite = gf2d_sprite_load_image("assets/sprites/menus/pauseMenu.png");

	pauseMenu->buttonMax = buttonMax;
	pauseMenu->buttonList = (Button *)gfc_allocate_array(sizeof (Button), pauseMenu->buttonMax);

	pauseMenu->_active = 1;

	return pauseMenu;

}

void *pause_menu_set_active(Menu *pause)
{

}

void *pause_menu_set_inactive(Menu *pause)
{

}

Menu *pause_menu_free(Menu *pause)
{

}