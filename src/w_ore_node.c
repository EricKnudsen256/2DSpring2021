#include "simple_logger.h"

#include "g_camera.h"
#include "g_random.h"

#include "w_ore_node.h"


//update so you can specify what ore type to create, or do that in each specific ore class


Ore_Node *ore_node_new(Vector2D gridPos)
{
	Ore_Node *node;

	node = (Ore_Node*)malloc(sizeof(Ore_Node));

	memset(node, 0, sizeof(Ore_Node));

	node->_inuse = 1;
	node->pos = vector2d(gridPos.x * 32, gridPos.y * 32);
	node->gridPos = gridPos;

	node->oreTotal = 100;


	node->animListLen = 4;
	node->animList = anim_list_new(node->animListLen);


	node->animList[0] = anim_new("assets/sprites/map/copperNode.png", "copper_idle", ANIM_IDLE, 0, 0, 15, 256, 128, 3);
	node->animList[0]->_current = 1;

	node->animList[1] = anim_new("assets/sprites/map/copperNode.png", "copper_shine", ANIM_ACTION, 1, 16, 15, 256, 128, 3);

	node->nextShine = random_int_range(5000, 10000);

	slog("Node spawned at x:%f, y:%f", node->pos.x, node->pos.y);

	return node;
}

void ore_node_update(Ore_Node *node)
{
	if (node->animList)
	{
		node->nextShine -= (SDL_GetTicks() - node->lastTick);

		node->lastTick = SDL_GetTicks();

		if (node->nextShine <= 0)
		{
			anim_list_change_anim(node->animList, 0, 1);

			node->nextShine = random_int_range(5000, 10000);
		}

		anim_list_update(node->animList, node->animListLen);
	}
	
}

void ore_node_draw(Ore_Node *node)
{
	Vector2D drawPosition, offset;
	Vector2D drawScale = camera_get_scale();
	Vector2D scalePos = vector2d(0, 0);

	if (!node)
	{
		slog("cannot draww a NULL rock");
		return;
	}
	else
	{
		offset = camera_get_offset();
		if (!camera_rect_on_screen(gfc_sdl_rect(node->pos.x, node->pos.y, node->pos.x + 128, node->pos.y + 64)))
		{
			//entity is off camera, skip
			//return;
		}
		drawPosition.x = node->pos.x + offset.x;
		drawPosition.y = node->pos.y + offset.y;

		if (node->animList)
		{

			anim_list_draw(node->animList, node->animListLen, drawPosition);
		}
	}
}

void ore_node_free(Ore_Node *node)
{
	if (!node)return;// nothing to do

	memset(node, 0, sizeof(Ore_Node));

	//anim_list_free(node->animList);

	node->_inuse = false;
	node = NULL;
}

//test