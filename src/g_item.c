#include "simple_logger.h"

#include "g_item.h"


void item_desc_list_init()
{

}

void item_desc_load_from_file();

void item_desc_list_free();

Item *item_new(char *name, int stackNum, char* spritePath)
{

	Item *item;
	item = (Item *)malloc(sizeof(Item));
	if (!item)
	{
		slog("Failed to allocate items");
		return NULL;
	}
	memset(item, 0, sizeof(Item));

	item->name = name;
	item->stackNum = stackNum;
	item->sprite = gf2d_sprite_load_image(spritePath);

	return item;
}

void item_delete(Item *item);
