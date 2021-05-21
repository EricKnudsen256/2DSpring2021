#include "simple_logger.h"

#include "gf2d_mouse.h"
#include "gf2d_shape.h"

#include "g_camera.h"
#include "g_menu.h"

#include "m_pause.h"
#include "m_inventory.h"

void menu_manager_init(Uint32 max_menus)
{
	if (max_menus == 0)
	{
		slog("cannot allocate 0 menus!");
		return;
	}
	if (menu_manager.menu_list != NULL)
	{
		menu_manager_free();
	}
	menu_manager.menu_list = (Menu *)gfc_allocate_array(sizeof (Menu), max_menus);
	if (menu_manager.menu_list == NULL)
	{
		slog("failed to allocate menu list!");
		return;
	}
	menu_manager.max_menus = max_menus;
	atexit(menu_manager_free);
	slog("menu system initialized");
}

//probably not going to be needed for a while, for animations i guess?
void menu_manager_update_menus()
{
	int i;
	if (menu_manager.menu_list == NULL)
	{
		slog("menu system does not exist");
		return;
	}
	for (i = 0; i < menu_manager.max_menus; i++)
	{
		if (menu_manager.menu_list[i]._inuse == 0)continue;
		if (menu_manager.menu_list[i]._active == 0)continue;
		if (menu_manager.menu_list[i].update != NULL)
		{
			menu_manager.menu_list[i].update(&menu_manager.menu_list[i]);
		}
	}
}

void menu_manager_think_menus()
{
	int i;
	if (menu_manager.menu_list == NULL)
	{
		slog("menu system does not exist");
		return;
	}
	for (i = 0; i < menu_manager.max_menus; i++)
	{
		if (menu_manager.menu_list[i]._inuse == 0)continue;
		if (menu_manager.menu_list[i]._active == 0)continue;
		if (menu_manager.menu_list[i].think != NULL)
		{
			menu_manager.menu_list[i].think(&menu_manager.menu_list[i]);
		}
	}
}

Bool menu_manager_check_click()
{
	int i;

	if (menu_manager.menu_list == NULL)
	{
		slog("menu system does not exist");
		return false;
	}

	for (i = 0; i < menu_manager.max_menus; i++)
	{
		if (menu_manager.menu_list[i]._inuse == 0)continue;
		if (menu_manager.menu_list[i]._active == 0)continue;
		if (menu_check_click(&menu_manager.menu_list[i]))
		{
			return true;
		}
	}

	return false;
}

void menu_manager_draw_menus()
{
	int i;

	if (menu_manager.menu_list == NULL)
	{
		slog("menu system does not exist");
		return;
	}

	for (i = 0; i < menu_manager.max_menus; i++)
	{
		if (menu_manager.menu_list[i]._inuse == 0)continue;
		if (menu_manager.menu_list[i]._active == 0)continue;
		menu_draw(&menu_manager.menu_list[i]);
	}
}

Menu *menu_manager_get_by_tag(char* tag)
{
	int i;
	for (i = 0; i < menu_manager.max_menus; i++)
	{
		if (menu_manager.menu_list[i]._inuse == 0)continue;
		if (menu_manager.menu_list[i].tag == tag)
		{
			return &menu_manager.menu_list[i];
		}
	}
	//slog("cannot find menu with tag");
	return NULL;
}

void menu_manager_free()
{
	if (menu_manager.menu_list != NULL)
	{
		free(menu_manager.menu_list);
	}
	memset(&menu_manager, 0, sizeof(MenuManager));
	slog("menu system closed");
}

Menu *menu_new()
{
	int i;
	if (menu_manager.menu_list == NULL)
	{
		slog("menu system does not exist");
		return NULL;
	}
	for (i = 0; i < menu_manager.max_menus; i++)
	{
		if (menu_manager.menu_list[i]._inuse)continue;// someone else is using this one
		memset(&menu_manager.menu_list[i], 0, sizeof(Menu));
		menu_manager.menu_list[i]._inuse = 1;
		menu_manager.menu_list[i].managerIndex = i;
		return &menu_manager.menu_list[i];
	}
	slog("no free menus available");
	return NULL;
}

Bool menu_check_click(Menu *menu)
{
	int i;
	Rect windowRect = gf2d_rect_from_sdl_rect(menu->windowSize);

	SDL_Event e;

	
	if (gf2d_mouse_in_rect(windowRect))
	{
		menu_buttons_check_click(menu);
		return true;
	}

	return false;

}

void menu_free(Menu *menu)
{
	

	if (!menu)
	{
		slog("cannot free a NULL sprite");
		return;
	}
	//gf2d_sprite_free(menu->sprite);
	//menu->sprite = NULL;
	memset(&menu_manager.menu_list[menu->managerIndex], 0, sizeof(Menu));
	menu->_inuse = 0;
}

void menu_draw(Menu *menu)
{
	Vector2D drawPosition;
	if (!menu)
	{
		slog("cannot draw a NULL menu");
		return;
	}

	else
	{
		if (menu->sprite == NULL)
		{
			SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 255, 0, 0, 255);

			SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &menu->windowSize);
			return;
		}
	}

	//currently not drawing animations, edit this if working on that in the future
	gf2d_sprite_draw(
		menu->sprite,
		menu->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		0);


	menu_buttons_draw(menu);

	if (menu->draw)
	{
		menu->draw(menu);
	}
}


int menu_button_new(Menu *menu)
{
	int i;
	Button *button;

	if (!menu)
	{
		slog("no menu provided");
		return -1;
	}

	
	button = button_new();
	
	for (i = 0; i < menu->buttonMax; i++)
	{
		if (!menu->buttonList[i])
		{
			menu->buttonList[i] = button;
			return i;
		}
	}
	
	slog("No free buttons in the menu!");
	
	return -1;

}

void menu_free_buttons(Menu *menu)
{
	int i;

	if (!menu)
	{
		slog("no menu provided");
		return;
	}


	for (i = 0; i < menu->buttonMax; i++)
	{
		if (!menu->buttonList[i]->_inuse)
		{
			button_free(menu->buttonList[i]);
		}
	}
}

void menu_buttons_check_click(Menu *menu)
{
	int i;
	Rect buttonRect;

	if (!menu)
	{
		slog("no menu provided");
		return;
	}
	if (!menu->buttonList)
	{
		slog("menu has no button list");
		return;
	}

	//check buttons for click as well
	for (i = 0; i < menu->buttonMax; i++)
	{

		if (!menu->buttonList[i])
		{
			continue;
		}

		buttonRect = gf2d_rect_from_sdl_rect(menu->buttonList[i]->buttonSize);

		if (gf2d_mouse_in_rect(buttonRect) && menu->buttonList[i]->onPress)
		{
			menu->buttonList[i]->onPress(menu->buttonList[i], menu);
		}
	}
}

void menu_buttons_draw(Menu *menu)
{
	int i;

	if (menu->buttonList == NULL)
	{
		slog("menu does not have any buttons");
		return;
	}

	for (i = 0; i < menu->buttonMax; i++)
	{
		if (!menu->buttonList[i])continue;

		button_draw(menu->buttonList[i]);
	}
}
