#include "g_item.h"


void item_desc_list_init();

void item_desc_load_from_file();

void item_desc_list_free();

Item *item_new(char *name);

void item_delete(Item *item);
