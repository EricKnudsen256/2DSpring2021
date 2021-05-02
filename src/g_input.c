#include <SDL.h>

#include "simple_logger.h"

#include "g_input.h"
#include "g_menu.h"

#include "p_player.h"

#include "m_pause.h"
#include "m_inventory.h"

void input_update()
{
	SDL_Event e;
	Menu *pauseMenu, *inventoryMenu;

	pauseMenu = menu_manager_get_by_tag("pause");
	inventoryMenu = menu_manager_get_by_tag("inventory");

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				if (!menu_manager_check_click())
				{
					player_attack(entity_manager_get_player_ent());
				}
			}
			if (e.button.button == SDL_BUTTON_RIGHT)
			{

				player_ranged(entity_manager_get_player_ent());
			
			}
		}
		else if (e.type == SDL_KEYDOWN)
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