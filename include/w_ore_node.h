#ifndef __ORE_NODE_H__
#define __ORE_NODE_H__

#include "gfc_types.h"
#include "gf2d_sprite.h"

typedef struct Ore_Node_s
{
	Bool		_inuse;
	Vector2D	pos;
	Vector2D	gridPos;
	SDL_Rect	interactionBox;

	void(*onInteract)(struct Interactable *interactible);

}Ore_Node_S;


#endif