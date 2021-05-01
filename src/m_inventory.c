#include "simple_logger.h"

#include "g_item.h"

#include "p_player.h"

#include "m_inventory.h"

Menu *inventory_new(int buttonMax)
{
	Menu *inventoryMenu;

	inventoryMenu = menu_new();



	inventoryMenu->position = vector2d(100, 100);
	gfc_rect_set(inventoryMenu->windowSize, inventoryMenu->position.x, inventoryMenu->position.y, 1000, 400);


	//inventoryMenu->sprite = gf2d_sprite_load_image("");

	inventoryMenu->buttonMax = buttonMax;
	inventoryMenu->buttonList = (Button *)gfc_allocate_array(sizeof (Button), inventoryMenu->buttonMax);
	inventoryMenu->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventoryMenu.png");
	inventoryMenu->think = inventory_think;

	inventoryMenu->tag = "inventory";

	inventory_create_slots(inventoryMenu);

	return inventoryMenu;
}

void inventory_create_slots(Menu *inventoryMenu)
{
	//32 is current inventory size, dont like hard coding it but accessing it isn't working properly

	for (int i = 0; i < player_inventory_get_max(); i++)
	{
		int testIndex;
		testIndex = menu_button_new(inventoryMenu);

		if (testIndex == -1)
		{
			slog("button not created");
			return NULL;
		}


		Button *button = inventoryMenu->buttonList[testIndex];

		button->position.x = inventoryMenu->position.x + 16 + 80 * (i % 8);
		button->position.y = inventoryMenu->position.y + 16 + (80 * (int)(i/8));
		button->sprite2Position.x = button->position.x + 8;
		button->sprite2Position.y = button->position.y + 8;

		gfc_rect_set(button->buttonSize, button->position.x, button->position.y, 64, 64);

		button->buttonTag = "testButton";
		button->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventorySlot.png");
	}

}

void inventory_think(Menu *inventoryMenu)
{
	//slog("UPDATE");


	for (int i = 0; i < player_inventory_get_max(); i++)
	{
		if (player_inventory_get_item(i))
		{
			inventoryMenu->buttonList[i]->sprite2 = player_inventory_get_item(i)->sprite;
		}
	}
	
}
void inventory_set_active(Menu *inventoryMenu)
{
	inventoryMenu->_active = 1;
}

void inventory_set_inactive(Menu *inventoryMenu)
{
	inventoryMenu->_active = 0;
}

