#include "simple_logger.h"

#include "b_chest.h"

#include "g_item.h"
#include "g_font.h"
#include "g_inventory.h"

#include "p_player.h"

#include "m_inventory.h"

#include "w_building.h"
#include "w_level.h"

Menu *inventory_menu_new(int buttonMax, Vector2D position, Inventory *inventory)
{
	Menu *inventoryMenu;

	inventoryMenu = menu_new();



	inventoryMenu->position = position;
	gfc_rect_set(inventoryMenu->windowSize, inventoryMenu->position.x, inventoryMenu->position.y, 1000, 400);


	//inventoryMenu->sprite = gf2d_sprite_load_image("");

	inventoryMenu->buttonMax = buttonMax;
	inventoryMenu->buttonList = (Button *)gfc_allocate_array(sizeof (Button), inventoryMenu->buttonMax);
	inventoryMenu->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventoryMenu.png");

	inventoryMenu->think = inventory_think;
	inventoryMenu->draw = inventory_draw;


	inventoryMenu->tag = "inventory";

	inventoryMenu->data = inventory;


	inventory_create_slots(inventoryMenu);

	//slog("Inventory Created");

	return inventoryMenu;
}

void inventory_create_slots(Menu *inventoryMenu)
{
	if (!inventoryMenu->data)
	{
		slog("Slots not created");
		return;
	}

	Inventory *inventory = inventoryMenu->data;

	for (int i = 0; i < inventory->max_items; i++)
	{
		int testIndex;
		testIndex = menu_button_new(inventoryMenu);

		if (testIndex == -1)
		{
			slog("button not created");
			return NULL;
		}


		Button *button = inventoryMenu->buttonList[testIndex];
		button->_id = testIndex;

		button->position.x = inventoryMenu->position.x + 16 + 80 * (i % 8);
		button->position.y = inventoryMenu->position.y + 16 + (80 * (int)(i/8));
		button->sprite2Position.x = button->position.x + 8;
		button->sprite2Position.y = button->position.y + 8;

		gfc_rect_set(button->buttonSize, button->position.x, button->position.y, 64, 64);

		button->buttonTag = "inventoryButton";
		button->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventorySlot.png");
		button->onPress = inventory_button_select;
	}

}

void inventory_think(Menu *inventoryMenu)
{
	int selectedSlot1 = -1;
	int	selectedSlot2 = -1;

	Inventory *inventory = inventoryMenu->data;

	//need to check if 2 slots are selected at the same time, if so, swap items
	for (int i = 0; i < inventory->max_items; i++)
	{
		if (inventory_get_item(i, inventory))
		{
			inventoryMenu->buttonList[i]->sprite2 = inventory_get_item(i, inventory)->sprite;
		}
		else
		{
			inventoryMenu->buttonList[i]->sprite2 = NULL;
		}

		if (inventory->selected == i)
		{
			inventoryMenu->buttonList[i]->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventorySlotSelected.png");
		}
		else
		{
			inventoryMenu->buttonList[i]->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventorySlot.png");
		}
	}

	
}
void inventory_set_active(Button *button, Menu *inventoryMenu)
{
	inventoryMenu->_active = 1;
}

void inventory_set_inactive(Button *button, Menu *inventoryMenu)
{
	inventoryMenu->_active = 0;

	Inventory *inventory = inventoryMenu->data;


	inventory->selected = -1;

}

void inventory_button_select(Button *slot, Menu *inventoryMenu)
{
	Bool isSelected = false;

	Inventory *inventory = inventoryMenu->data;
	Inventory *checkInventory;
	Level *level = level_manager_get_current();

	int checkInventorySlot = -1;

	//iterate through all chest inventories/player inventory, if another selected slot if found, swap items
	//then deselect slots

	int i;

	checkInventory = player_inventory_get();


	if (checkInventory->selected != -1)
	{
		//slog("test1");
		checkInventorySlot = checkInventory->selected;
	}
	else if (inventory->selected != -1)
	{
		//slog("test2");
		checkInventory = inventory;
		checkInventorySlot = checkInventory->selected;
	}
	else  //this check not working properly
	{
		for (i = 0; i < level->max_buildings; i++)
		{
			if (level->building_list[i] && level->building_list[i]->type && level->building_list[i]->type == BUILDING_CHEST)
			{
				Chest *chest = level->building_list[i]->parent;

				if (chest->inventory->selected != -1)
				{
					//slog("test3");
					checkInventory = chest->inventory;
					checkInventorySlot = chest->inventory->selected;
				}
			}
		}
	}

	if (checkInventorySlot != -1)
	{
		inventory_swap(slot->_id, checkInventorySlot, inventory, checkInventory);

		checkInventory->selected = -1;
	}
	else 	//else select slot
	{
		inventory->selected = slot->_id;
	}
	
}

void inventory_deselect_all(Menu *inventoryMenu)
{

	Inventory *inventory = inventoryMenu->data;

	for (int i = 0; i < inventory->max_items; i++)
	{
		if (inventoryMenu->buttonList[i]->_selceted == true)
		{
			inventoryMenu->buttonList[i]->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventorySlotSelected.png");
		}
		inventoryMenu->buttonList[i]->_selceted = false;
	}
}

Bool inventory_is_selected(Menu *inventoryMenu)
{
	Inventory *inventory = inventoryMenu->data;

	for (int i = 0; i < inventory->max_items; i++)
	{
		if (inventoryMenu->buttonList[i]->_selceted == true)
		{
			return true;
		}
	}
	return false;
}

Item *inventory_get_selected(Menu *inventoryMenu)
{
	Inventory *inventory = inventoryMenu->data;

	for (int i = 0; i < inventory->max_items; i++)
	{
		if (inventoryMenu->buttonList[i]->_selceted == true)
		{
			return inventory_get_item(i, inventory);
		}
	}
	return NULL;
}

int inventory_get_selected_slot(Menu *inventoryMenu)
{
	Inventory *inventory = inventoryMenu->data;

	for (int i = 0; i < inventory->max_items; i++)
	{
		if (inventoryMenu->buttonList[i]->_selceted == true)
		{
			return i;
		}
	}
	return -1;
}

void inventory_draw(Menu *inventoryMenu)
{
	Font *font = font_load("assets/fonts/DotGothic16-Regular.ttf", 18);
	Color color = gfc_color(255, 255, 255, 255);
	Vector2D drawPos;

	Item *item;

	TextLine itemNum;

	//slog("Active:%i", inventoryMenu->managerIndex);

	Inventory *inventory = inventoryMenu->data;



	if (inventoryMenu->_active == 0)
	{
		return;
	}

	for (int i = 0; i < inventory->max_items; i++)
	{
		if (inventory_get_item(i, inventory))
		{
			drawPos = vector2d(inventoryMenu->buttonList[i]->position.x + 4, inventoryMenu->buttonList[i]->position.y);

			item = inventory_get_item(i, inventory);

			gfc_line_sprintf(itemNum, "%i", item->stackNum);

			font_render(font, itemNum, drawPos, color);
		}
	}
}



