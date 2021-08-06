#ifndef __H_INVENTORY__
#define __H_INVENTORY__

#include "g_item.h"


typedef struct
{
	Item **item_list;
	Uint32  max_items;

	int *selected;
}Inventory;

/**
* @brief creates the inventory
* @param max_items the total number of items in the inventory
*/
Inventory *inventory_new(Uint32 max_items);

/**
* @brief adds an item to the first open slot of the inventory
* @param item the item to be added
*/
void inventory_add_item(Item *item, Inventory *inventory);


void inventory_remove_item(int slot, Inventory *inventory);

/**
* @brief gets the item in the provided inventory slot
* @param slot the index of the inventory slot to check
* @return NULL if no item found, a pointer to the item otherwise
*/
Item *inventory_get_item(int slot, Inventory *inventory);

/**
* @brief frees the inventory
*/
void inventory_free(Inventory *inventory);

/**
* @brief posts the entire player inventory to console for error checking
*/
void inventory_slog(Inventory *inventory);

/**
* @brief swaps two items in the player's inventory
* @param slot1 the slot of the first item
* @param slot2 the slot of the second item
*/
void inventory_swap(int slot1, int slot2, Inventory *inventory);

/**
* @brief returns the size of the inventory
* @return int if the inventory size
*/
Uint32 inventory_get_max(Inventory *inventory);




#endif