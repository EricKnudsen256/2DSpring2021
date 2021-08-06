#include <SDL.h>

#include "gfc_types.h"

#include "simple_logger.h"

#include "g_input.h"
#include "g_menu.h"
#include "g_camera.h"

#include "p_player.h"

#include "m_pause.h"
#include "m_inventory.h"

#include "w_building.h"
#include "w_level.h"

static Bool input_mosue_down = 0;

void input_game()
{
	input_mosue_down = false;

	SDL_Event e;
	Menu *pauseMenu, *inventoryMenu, *mainMenu, *buildMenu;
	Level *level = level_manager_get_current();

	pauseMenu = menu_manager_get_by_tag("pause");
	inventoryMenu = menu_manager_get_by_tag("inventory");
	buildMenu = menu_manager_get_by_tag("build");

	while (SDL_PollEvent(&e))
	{

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				input_mosue_down = true;

				if (!menu_manager_check_click())
				{
					if (inventory_is_selected(inventoryMenu))
					{
						level_new_drop(entity_manager_get_player_ent()->position, inventory_get_selected(inventoryMenu), level);
						inventory_remove_item(inventory_get_selected_slot(inventoryMenu), player_inventory_get());
						inventory_deselect_all(inventoryMenu);
					}
					else if (building_list_is_current())
					{
						building_list_place_current();
					}
					else
					{
						player_attack(entity_manager_get_player_ent());
					}
				}
			}
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				if (entity_manager_get_player_ent())
				{
					if (building_list_is_current())
					{
						building_list_remove_current_build();
					}
					else
					{
						player_ranged(entity_manager_get_player_ent());
					}

				}


			}
		}
		else if (e.type == SDL_KEYDOWN)
		{

			if (pauseMenu)
			{
				switch (e.button.button)
				{
					case SDL_SCANCODE_ESCAPE:
						if (pauseMenu->_active == 0)
						{
							build_set_inactive(NULL, buildMenu);
							pause_menu_set_active(NULL, pauseMenu);
							inventory_set_inactive(NULL, inventoryMenu);

						}
						else if (pauseMenu->_active == 1)
						{

							pause_menu_set_inactive(NULL, pauseMenu);

						}
						break;

					case SDL_SCANCODE_B:
						if (buildMenu->_active == 0)
						{
							build_set_active(NULL, buildMenu);
						}
						else if (buildMenu->_active == 1)
						{
							build_set_inactive(NULL, buildMenu);
						}
						break;

					case SDL_SCANCODE_E:
						level_check_interact(level);
						break;

					case SDL_SCANCODE_R:
						if (building_list_is_current())
						{
							building_list_rotate_current();
						}
						break;

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

			if (e.button.button == SDL_SCANCODE_L)
			{
				Level *level = level_manager_get_current();
				Vector2D playerPos = entity_manager_get_player_ent()->position;

				level_interact_new(&playerPos, NULL, 200, 200, level);
			}
		}
		else if (e.type == SDL_MOUSEWHEEL)
		{
			if (e.wheel.y > 0)
			{
				camera_zoom_in();
			}
			if (e.wheel.y < 0)
			{
				camera_zoom_out();
			}
		}

	}
}

void input_editor()
{
	input_mosue_down = false;

	SDL_Event e;
	Menu *pauseMenu, *inventoryMenu, *mainMenu;

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

void input_main_menu()
{
	input_mosue_down = false;

	SDL_Event e;
	Menu *pauseMenu, *inventoryMenu, *mainMenu;

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
	}
}


void input_update()
{
	input_mosue_down = false;

	SDL_Event e;
	Menu *pauseMenu, *inventoryMenu, *mainMenu;

	//check mouse input
	mainMenu = menu_manager_get_by_tag("main");

	if (!mainMenu) return;

	if (main_get_data(mainMenu) == "start")
	{
		input_game();
	}
	else if (main_get_data(mainMenu) == "editor")
	{
		input_editor();
	}
	else
	{
		input_main_menu();
	}
}

Bool input_get_mouse_down()
{
	return input_mosue_down;
}