#include "simple_logger.h"

#include "g_camera.h"
#include "g_random.h"

#include "p_player.h"

#include "w_ore_node.h"
#include "w_level.h"


//update so you can specify what ore type to create, or do that in each specific ore class



Ore_Node *ore_node_new(Vector2D gridPos, char *oreType)
{
	Ore_Node *node;

	int rockSprite = random_int_range(1, 8);
	int oreNum = 0;

	char filename[50];
	char filenum[5];

	char filestart[] = "assets/sprites/map/";
	char fileend[] = ".png";
	strcpy(filename, "");

	node = (Ore_Node*)malloc(sizeof(Ore_Node));

	memset(node, 0, sizeof(Ore_Node));

	node->_inuse = 1;
	node->pos = vector2d(gridPos.x * 32, gridPos.y * 32);
	node->gridPos = gridPos;

	node->oreTotal = 100;


	node->animListLen = 4;
	node->animList = anim_list_new(node->animListLen);



	if (strcmp(oreType, "random") == 0)
	{
		oreNum = random_int_range(0, 4);
	}
	else if (strcmp(oreType, "copper") == 0)
	{
		oreNum = 0;
	}
	else if (strcmp(oreType, "iron") == 0)
	{
		oreNum = 1;
	}
	else if (strcmp(oreType, "gold") == 0)
	{
		oreNum = 2;
	}
	else if (strcmp(oreType, "aluminum") == 0)
	{
		oreNum = 3;
	}
	else if (strcmp(oreType, "coal") == 0)
	{
		oreNum = 4;
	}


	switch (oreNum)
	{
		case 0:



			itoa(rockSprite, &filenum, 10);

			strcat(filename, filestart);
			strcat(filename, "copperSmall");
			strcat(filename, filenum);
			strcat(filename, fileend);

			node->animList[0] = anim_new(filename, "copper_idle", ANIM_IDLE, 0, 0, 15, 64, 64, 5);
			node->animList[0]->_current = 1;

			node->animList[1] = anim_new(filename, "copper_shine", ANIM_ACTION, 1, 13, 15, 64, 64, 5);

			node->oreType = ORE_COPPER;
			break;

		case 1:


			itoa(rockSprite, filenum, 10);

			strcat(filename, filestart);
			strcat(filename, "ironSmall");
			strcat(filename, filenum);
			strcat(filename, fileend);

			node->animList[0] = anim_new(filename, "iron_idle", ANIM_IDLE, 0, 0, 15, 64, 64, 5);
			node->animList[0]->_current = 1;

			node->animList[1] = anim_new(filename, "iron_shine", ANIM_ACTION, 1, 13, 15, 64, 64, 5);
			node->oreType = ORE_IRON;
			break;

		case 2:


			itoa(rockSprite, filenum, 10);

			strcat(filename, filestart);
			strcat(filename, "goldSmall");
			strcat(filename, filenum);
			strcat(filename, fileend);

			node->animList[0] = anim_new(filename, "gold_idle", ANIM_IDLE, 0, 0, 15, 64, 64, 5);
			node->animList[0]->_current = 1;

			node->animList[1] = anim_new(filename, "gold_shine", ANIM_ACTION, 1, 13, 15, 64, 64, 5);
			node->oreType = ORE_GOLD;
			break;

		case 3:

			itoa(rockSprite, filenum, 10);

			strcat(filename, filestart);
			strcat(filename, "aluminumSmall");
			strcat(filename, filenum);
			strcat(filename, fileend);

			node->animList[0] = anim_new(filename, "aluminum_idle", ANIM_IDLE, 0, 0, 15, 64, 64, 5);
			node->animList[0]->_current = 1;

			node->animList[1] = anim_new(filename, "aluminum_shine", ANIM_ACTION, 1, 13, 15, 64, 64, 5);
			node->oreType = ORE_ALUMINUM;
			break;

		case 4:


			itoa(rockSprite, filenum, 10);

			strcat(filename, filestart);
			strcat(filename, "coalSmall");
			strcat(filename, filenum);
			strcat(filename, fileend);

			node->animList[0] = anim_new(filename, "coal_idle", ANIM_IDLE, 0, 0, 15, 64, 64, 5);
			node->animList[0]->_current = 1;

			node->animList[1] = anim_new(filename, "coal_shine", ANIM_ACTION, 1, 13, 15, 64, 64, 5);
			node->oreType = ORE_COAL;
			break;

	}



	

	node->nextShine = random_int_range(5000, 10000);

	//slog("Node spawned at x:%f, y:%f", node->pos.x, node->pos.y);

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

void ore_node_interact(Ore_Node *node)
{
	char *itemName;

	if (!node)
	{
		return;
	}

	switch (node->oreType)
	{
		case ORE_COPPER:
			itemName = "copperOre";
			break;

		case ORE_IRON:
			itemName = "ironOre";
			break;

		case ORE_GOLD:
			itemName = "goldOre";
			break;

		case ORE_ALUMINUM:
			itemName = "aluminumOre";
			break;

		case ORE_COAL:
			itemName = "coalOre";
			break;

		default:
			itemName = "";
			break;
	}
	
	player_inventory_add_item(item_new_by_name(itemName, 1));

	node->oreTotal--;

	if (node->oreTotal <= 0)
	{
		level_ore_node_free(node, level_manager_get_current());
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