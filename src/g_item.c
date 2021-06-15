#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_sprite.h"

#include "g_item.h"


void item_load_from_file()
{
	SJson *json, *itemJS, *currentItemJS;

	int itemTypes;

	json = sj_load("config/items.cfg");

	if (!json)return;

	itemJS = sj_object_get_value(json, "items");

	itemTypes = sj_array_get_count(itemJS);

	//create item list with size of itemJS

	if (itemTypes == 0)
	{
		slog("0 items in item list, something is very fucking wrong");
		return;
	}
	if (itemList.item_list != NULL)
	{
		item_list_free();
	}
	itemList.item_list = (ItemTemplate *)gfc_allocate_array(sizeof (ItemTemplate), itemTypes);
	if (itemList.item_list == NULL)
	{
		slog("failed to allocate item list!");
		return;
	}
	itemList.max_item_types = itemTypes;
	slog("item list initialized. Size: %i", itemTypes);

	//end item list create

	for (int i = 0; i < itemTypes; i++)
	{
		currentItemJS = sj_array_get_nth(itemJS, i);

		itemList.item_list[i].name = sj_get_string_value(sj_object_get_value(currentItemJS, "name"));
		itemList.item_list[i].sprite = gf2d_sprite_load_image(sj_get_string_value(sj_object_get_value(currentItemJS, "sprite")));
		itemList.item_list[i].description = sj_get_string_value(sj_object_get_value(currentItemJS, "description"));

		//slog("Desc %i: %s", i, itemList.item_list[i].description);

	}

}


void item_list_free()
{

}


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
