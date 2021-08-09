#include "simple_logger.h"

#include "g_inventory.h"

#include "g_menu.h"

Inventory *inventory_new(Uint32 max_items)
{
	Inventory *inventory;

	inventory = (Inventory*)malloc(sizeof(Inventory));

	memset(inventory, 0, sizeof(Inventory));

	inventory->item_list = gfc_allocate_array(sizeof(Item*), max_items);

	inventory->max_items = max_items;

	inventory->selected = -1;

	return inventory;
}


void inventory_add_item(Item *item, Inventory *inventory)
{
	if (!item)
	if (inventory->item_list == NULL)
	{
		slog("Inventory not created, call player_inventory_init first");
		return;
	}

	for (int i = 0; i < inventory->max_items; i++)
	{
		if (inventory->item_list[i] && strcmp(inventory->item_list[i]->name, item->name) == 0)
		{
			if (inventory->item_list[i]->stackNum >= 100)
			{
				continue;
			}
			inventory->item_list[i]->stackNum += item->stackNum;
			return;
		}
	}

	for (int i = 0; i < inventory->max_items; i++)
	{
		if (inventory->item_list[i] == NULL)
		{
			inventory->item_list[i] = item;
			return;
		}
	}
}


void inventory_remove_item(int slot, Inventory *inventory)
{
	if (slot < inventory->max_items)
	{
		inventory->item_list[slot] = NULL;
	}
}


Item *inventory_get_item(int slot, Inventory *inventory)
{
	if (slot < inventory->max_items)
	{
		return inventory->item_list[slot];
	}
	return NULL;
}


void inventory_free(Inventory *inventory)
{
	if (inventory->item_list != NULL)
	{
		free(inventory->item_list);
	}
	memset(inventory, 0, sizeof(Inventory));
	//slog("inventory deleted");
}


void inventory_slog(Inventory *inventory)
{

	slog("maxItems: %i", inventory->max_items);
	for (int i = 0; i < inventory->max_items; i++)
	{


		if (!inventory->item_list[i])
		{
			slog("Slot %i: empty", i);
		}
		else
		{
			slog("Slot %i: %s:%i", i, inventory->item_list[i]->name, inventory->item_list[i]->stackNum);
		}
	}
}


void inventory_swap(int slot1, int slot2, Inventory *inventory1, Inventory *inventory2)
{
	Item *temp;
	if (slot1 < inventory1->max_items && slot2 < inventory2->max_items)
	{
		slog("Slot1: %i, Slot2: %i", slot1, slot2);

		temp = inventory1->item_list[slot1];
		inventory1->item_list[slot1] = inventory2->item_list[slot2];
		inventory2->item_list[slot2] = temp;
		//inventory_slog(inventory);
		return;
	}

	slog("Slots not in possible range. Slot1: %i, Slot2: %i", slot1, slot2);
	return;
}


Uint32 inventory_get_max(Inventory *inventory)
{
	return inventory->max_items;
}

