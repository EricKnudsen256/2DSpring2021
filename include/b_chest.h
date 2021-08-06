#ifndef __H_CHEST__
#define __H_CHEST__

#include "gfc_types.h"

#include "e_drop.h"

#include "w_interactable.h"

#include "g_anim.h"
#include "g_menu.h"
#include "g_inventory.h"

#include "w_building.h"

typedef enum
{
	CHEST_OPEN,
	CHEST_OPENING,
	CHEST_CLOSED

}ChestState;

typedef struct Chest_s
{
	ChestState state;

	Building *building;

	Inventory *inventory;

	Menu *chestMenu;

	Interactable *interact;

	Anim **animList;
	int animListLen;



	//other things needed in struct are just the attributes not in the building struct

}Chest;

Chest *chest_new(Vector2D gridPos, Vector2D size, int direction);

void chest_update(Chest *chest);

void chest_draw(Chest *chest);

void chest_open_inventory(Chest *chest);

void chest_close_inventory(Chest *chest);

void chest_interact(Chest *chest);

void chest_free(Chest *chest);


#endif