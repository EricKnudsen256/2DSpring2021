#ifndef __ITEM_H__
#define __ITEM_H__

#include "gf2d_sprite.h"

typedef struct Item_s
{
	char *name;
	Sprite *sprite;
	char *description;
	int stackNum;

}Item;

typedef struct ItemTemplate_s
{
	char *name;
	Sprite *sprite;
	char *description;
	int maxStacks;

}ItemTemplate;

typedef struct ItemList_s
{
	ItemTemplate *item_list;
	Uint32  max_item_types;

}ItemList;

typedef struct Recipe_s
{
	char *name;
	char **components;
	int outputNum;

}Recipe;

typedef struct RecipeList_s
{
	Recipe *recipe_list;
	Uint32  total_recipies;

}RecipeList;

static ItemList itemList = { 0 };

static RecipeList recipeList = { 0 };

/**
*  @brief Creates list of all items with their discriptions from config file
*/

void item_load_from_file();

void item_list_free();

Item *item_new(char *name, int stackNum, char* spritePath);

void item_delete(Item *item);

#endif