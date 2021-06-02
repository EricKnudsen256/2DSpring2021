#include "simple_logger.h"

#include "g_item.h"
#include "g_font.h"

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
	inventoryMenu->draw = inventory_draw;


	inventoryMenu->tag = "inventory";


	inventory_create_slots(inventoryMenu);



	return inventoryMenu;
}

void inventory_create_slots(Menu *inventoryMenu)
{
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
		button->onPress = inventory_button_select;
	}

}

void inventory_think(Menu *inventoryMenu)
{
	int selectedSlot1 = -1;
	int	selectedSlot2 = -1;

	//need to check if 2 slots are selected at the same time, if so, swap items
	for (int i = 0; i < player_inventory_get_max(); i++)
	{
		if (player_inventory_get_item(i))
		{
			inventoryMenu->buttonList[i]->sprite2 = player_inventory_get_item(i)->sprite;
		}
		else
		{
			inventoryMenu->buttonList[i]->sprite2 = NULL;
		}
		if (inventoryMenu->buttonList[i]->_selceted == true)
		{
			if (selectedSlot1 == -1)
			{
				selectedSlot1 = i;
			}
			else if (selectedSlot2 == -1)
			{
				selectedSlot2 = i;
				slog("Select 1: %i", selectedSlot1);
				slog("Select 2: %i", selectedSlot2);
			}
			inventoryMenu->buttonList[i]->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventorySlotSelected.png");
		}
		else
		{
			inventoryMenu->buttonList[i]->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventorySlot.png");
		}
	}

	if (selectedSlot1 != -1 && selectedSlot2 != -1)
	{
		inventory_deselect_all(inventoryMenu);

		player_inventory_swap(selectedSlot1, selectedSlot2);
		selectedSlot1 = -1;
		selectedSlot2 = -1;
	}
	
}
void inventory_set_active(Button *button, Menu *inventoryMenu)
{
	inventoryMenu->_active = 1;
}

void inventory_set_inactive(Button *button, Menu *inventoryMenu)
{
	inventoryMenu->_active = 0;

	for (int i = 0; i < player_inventory_get_max(); i++)
	{
		inventoryMenu->buttonList[i]->_selceted = false;
		inventoryMenu->buttonList[i]->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventorySlot.png");

	}
}

void inventory_button_select(Button *slot, Menu *inventoryMenu)
{
	Bool isSelected = false;
	for (int i = 0; i < player_inventory_get_max(); i++)
	{
		if (inventoryMenu->buttonList[i]->_selceted == true)
		{
			isSelected = true;
			break;
		}

	}

	if (slot->_selceted == true)
	{
		slot->_selceted = false;
	}
	else if (slot->sprite2 || isSelected)
	{
		slot->_selceted = true;
	}
}

void inventory_deselect_all(Menu *inventoryMenu)
{
	for (int i = 0; i < player_inventory_get_max(); i++)
	{
		if (inventoryMenu->buttonList[i]->_selceted == true)
		{
			inventoryMenu->buttonList[i]->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventorySlotSelected.png");
		}
		inventoryMenu->buttonList[i]->_selceted = false;
	}
}

void inventory_draw(Menu *inventoryMenu)
{
	Font *font = font_load("assets/fonts/DotGothic16-Regular.ttf", 18);
	Color color = gfc_color(255, 255, 255, 255);
	Vector2D drawPos;

	Item *item;

	TextLine itemNum;

	//slog("Draw");

	for (int i = 0; i < player_inventory_get_max(); i++)
	{
		if (player_inventory_get_item(i))
		{
			drawPos = vector2d(inventoryMenu->buttonList[i]->position.x + 4, inventoryMenu->buttonList[i]->position.y);

			item = player_inventory_get_item(i);

			gfc_line_sprintf(itemNum, "%i", item->stackNum);

			font_render(font, itemNum, drawPos, color);
		}
	}
}

