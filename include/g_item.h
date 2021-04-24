#ifndef __ITEM_H__
#define __ITEM_H__

#include "gf2d_sprite.h"

typedef struct Item_s
{
	char *name;
	Sprite sprite;
	char *description;
	int stackNum;

}Item;

typedef struct
{
	Item *item_list;
	Uint32  max_item_types;

}ItemDescList;

static ItemDescList itemDescList = { 0 };

void item_desc_list_init();

void item_desc_load_from_file();

void item_desc_list_free();

Item *item_new(char *name, int stackNum);

void item_delete(Item *item);

#endif