#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_sprite.h"

#include "g_item.h"


void item_load_from_file()
{
	SJson *json, *json2, *itemJS, *currentItemJS, *craftingJS, *currentRecipeJS, *recipeArrayJS;

	int itemTypes;
	int baseRecipies;

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


	for (int i = 0; i < itemTypes; i++)
	{
		currentItemJS = sj_array_get_nth(itemJS, i);

		itemList.item_list[i].name = sj_get_string_value(sj_object_get_value(currentItemJS, "name"));
		itemList.item_list[i].sprite = gf2d_sprite_load_image(sj_get_string_value(sj_object_get_value(currentItemJS, "sprite")));
		itemList.item_list[i].description = sj_get_string_value(sj_object_get_value(currentItemJS, "description"));
		sj_get_integer_value(sj_object_get_value(currentItemJS, "maxStacks"), &itemList.item_list[i].maxStacks);

		//slog("Desc %i: %i", i, itemList.item_list[i].maxStacks);

	}

	//end item list create

	//create recipie list

	
	json2 = sj_load("config/crafting.cfg");

	if (!json2)return;

	craftingJS = sj_object_get_value(json2, "baseRecipies");

	baseRecipies = sj_array_get_count(craftingJS);

	

	if (baseRecipies == 0)
	{
		slog("0 items in recipe list, something is very fucking wrong");
		return;
	}
	if (recipeList.recipe_list != NULL)
	{
		item_list_free();
	}
	recipeList.recipe_list = (Recipe *)gfc_allocate_array(sizeof (Recipe), baseRecipies);
	if (recipeList.recipe_list == NULL)
	{
		slog("failed to allocate recipe list!");
		return;
	}
	recipeList.total_recipies = baseRecipies;
	slog("recipe list initialized. Size: %i", baseRecipies);

	
	for (int i = 0; i < baseRecipies; i++)
	{
		
		currentRecipeJS = sj_array_get_nth(craftingJS, i);

		recipeList.recipe_list[i].name = sj_get_string_value(sj_object_get_value(currentRecipeJS, "name"));
		sj_get_integer_value(sj_object_get_value(currentRecipeJS, "amount"), &recipeList.recipe_list[i].outputNum);

		//fill recipe array with items

		recipeArrayJS = sj_object_get_value(currentRecipeJS, "recipe");

		
		if (sj_array_get_count(recipeArrayJS) > 0)
		{
			recipeList.recipe_list[i].components = (char *)gfc_allocate_array(sizeof (char), sj_array_get_count(recipeArrayJS));

			for (int j = 0; j < sj_array_get_count(recipeArrayJS); j++)
			{
				recipeList.recipe_list[i].components[j] = sj_get_string_value(sj_array_get_nth(recipeArrayJS, j));

				//slog("Item %s: Component: %s", i, recipeList.recipe_list[i].name, recipeList.recipe_list[i].components[j]);
			}
		}
		

	}
	

	//end recipie list create

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
