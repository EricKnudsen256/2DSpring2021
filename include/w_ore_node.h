#ifndef __ORE_NODE_H__
#define __ORE_NODE_H__

#include "gfc_types.h"
#include "gf2d_sprite.h"

#include "g_anim.h"
#include "w_interactable.h"

typedef enum
{
	ORE_COPPER,
	ORE_IRON,
	ORE_ALUMINUM,
	ORE_GOLD,
	ORE_COAL,
	ORE_IGNEUM,
	ORE_GLACIUM,
	ORE_ELECTRAE,
	ORE_ARCANUM

}OreType;

typedef struct Ore_Node_s
{
	Bool		_inuse;
	Vector2D	pos;
	Vector2D	gridPos;
	Interactable	*interact;

	OreType		oreType;

	int			id;

	int			oreTotal;

	Anim		**animList;
	int			animListLen;

	int			nextShine;

	int			lastTick;

	int			idleframe;

	//void(*onInteract)(struct Interactable *interactible);

}Ore_Node;

Ore_Node *ore_node_new(Vector2D gridPos, char *oreType);

void ore_node_update(Ore_Node *node);

void ore_node_draw(Ore_Node *node);

void ore_node_free(Ore_Node *node);

void ore_node_interact(Ore_Node *node);


#endif