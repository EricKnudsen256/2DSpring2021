#include <SDL.h>

#include "gfc_types.h"

#include "simple_logger.h"

#include "g_input.h"
#include "g_menu.h"

#include "p_player.h"

#include "m_pause.h"
#include "m_inventory.h"

static Bool input_mosue_down = 0;

void input_update()
{
	input_mosue_down = false;

	SDL_Event e;
	Menu *pauseMenu, *inventoryMenu, *mainMenu;

	mainMenu = menu_manager_get_by_tag("main");

	if (!mainMenu) return;

	if (main_get_data(mainMenu) == "start")
	{
		while (SDL_PollEvent(&e))
		{

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					input_mosue_down = true;

					if (!menu_manager_check_click())
					{
						player_attack(entity_manager_get_player_ent());
					}
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					if (entity_manager_get_player_ent())
					{
						player_ranged(entity_manager_get_player_ent());
					}


				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				pauseMenu = menu_manager_get_by_tag("pause");
				inventoryMenu = menu_manager_get_by_tag("inventory");

				if (pauseMenu)
				{
					if (e.button.button == SDL_SCANCODE_ESCAPE && pauseMenu->_active == 0)
					{

						pause_menu_set_active(NULL, pauseMenu);
						inventory_set_inactive(NULL, inventoryMenu);

					}
					else if (e.button.button == SDL_SCANCODE_ESCAPE && pauseMenu->_active == 1)
					{

						pause_menu_set_inactive(NULL, pauseMenu);

					}
				}

				if (inventoryMenu)
				{
					if (e.button.button == SDL_SCANCODE_TAB && inventoryMenu->_active == 0)
					{
						inventory_set_active(NULL, inventoryMenu);
					}
					else if (e.button.button == SDL_SCANCODE_TAB && inventoryMenu->_active == 1)
					{
						inventory_set_inactive(NULL, inventoryMenu);
					}
				}
			}
		}
	}
	else if (main_get_data(mainMenu) == "editor")
	{
		while (SDL_PollEvent(&e))
		{

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					input_mosue_down = true;

					if (!menu_manager_check_click())
					{
						//place tile
					}
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					if (entity_manager_get_player_ent())
					{
						//delete tile
					}


				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				pauseMenu = menu_manager_get_by_tag("pause");

				if (pauseMenu)
				{
					if (e.button.button == SDL_SCANCODE_ESCAPE && pauseMenu->_active == 0)
					{

						pause_menu_set_active(NULL, pauseMenu);

					}
					else if (e.button.button == SDL_SCANCODE_ESCAPE && pauseMenu->_active == 1)
					{

						pause_menu_set_inactive(NULL, pauseMenu);

					}
				}
			}
		}
	}
	else
	{
		while (SDL_PollEvent(&e))
		{

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (!menu_manager_check_click())
					{
						//nothing
					}
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					if (entity_manager_get_player_ent())
					{
						//nothing
					}


				}
			}
			else if (e.type == SDL_KEYDOWN)
			{

			}
		}
	}
}

Bool input_get_mouse_down()
{
	return input_mosue_down;
}