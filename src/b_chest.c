#include "simple_logger.h"

#include "b_chest.h"

#include "g_camera.h"
#include "g_menu.h"

#include "m_inventory.h"

#include "w_level.h"


Chest *chest_new(Vector2D gridPos, Vector2D size, int direction)
{
	Chest *chest;

	chest = (Chest*)malloc(sizeof(Chest));

	memset(chest, 0, sizeof(Chest));

	chest->building = level_building_new(gridPos, size, level_manager_get_current());

	chest->animListLen = 2;
	chest->animList = anim_list_new(2);

	//load anims from list

	Anim **animTemplate = building_list_get_by_name("chest")->animList;
	int	templateLength = building_list_get_by_name("chest")->maxAnims;


	chest->animList[0] = anim_list_get_by_name(animTemplate, "chest_closed", templateLength);
	chest->animList[0]->_current = true;

	chest->animList[1] = anim_list_get_by_name(animTemplate, "chest_opening", templateLength);
	chest->animList[2] = anim_list_get_by_name(animTemplate, "chest_opened", templateLength);


	chest->building->parent = chest;
	chest->building->update = chest_update;
	chest->building->draw = chest_draw;
	chest->building->type = BUILDING_CHEST;

	chest->interact = level_interact_new(NULL, &gridPos, size.x * 32, 64, level_manager_get_current());
	chest->interact->onInteract = chest_interact;
	chest->interact->parent = chest;

	chest->inventory = inventory_new(30);

	chest->state = CHEST_CLOSED;

	chest->chestMenu = inventory_menu_new(chest->inventory->max_items, vector2d(100, 600), chest->inventory);

	//slog("Drawpos.x:%f, Drawpos.y:%f", pipe->building->position.x, pipe->building->position.x);

	return chest;
}

void chest_update(Chest *chest)
{
	Entity *player = entity_manager_get_player_ent();

	Vector2D playerCenter = vector2d(player->position.x + player->hitbox.w / 2, player->position.y + player->hitbox.h / 2);
	Vector2D chestCenter = vector2d(chest->building->position.x + chest->building->hitbox.w / 2, chest->building->position.y + chest->building->hitbox.h / 2);

	if (!vector2d_distance_between_less_than(playerCenter, chestCenter, 100.0) && chest->state == CHEST_OPEN)
	{
		chest_close_inventory(chest);
	}

	anim_list_update(chest->animList, chest->animListLen);
}

void chest_draw(Chest *chest)
{
	Vector2D drawPosition, offset;
	Vector2D drawScale = camera_get_scale();

	if (!chest)
	{
		slog("cannot draw a NULL pipe");
		return;
	}
	else
	{
		offset = camera_get_offset();

		drawPosition.x = chest->building->position.x + offset.x;
		drawPosition.y = chest->building->position.y + offset.y;



		anim_list_draw(chest->animList, chest->animListLen, drawPosition);
	}
	if (chest->building->hitbox.w > 0 && chest->building->hitbox.h > 0)
	{
		SDL_Rect tempDraw;

		tempDraw.x = (chest->building->hitbox.x + offset.x) * drawScale.x;
		tempDraw.y = (chest->building->hitbox.y + offset.y) * drawScale.y;
		tempDraw.w = chest->building->hitbox.w * drawScale.x;
		tempDraw.h = chest->building->hitbox.h * drawScale.y;

		SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 0, 255, 0, 255);

		SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &tempDraw);
	}
}

void chest_interact(Chest *chest)
{
	if (chest->state == CHEST_OPEN)
	{
		chest_close_inventory(chest);
	}
	else if (chest->state == CHEST_CLOSED)
	{
		chest_open_inventory(chest);
	}
}

void chest_open_inventory(Chest *chest)
{
	inventory_set_active(NULL, chest->chestMenu);
	chest->state = CHEST_OPEN;

	Menu* inventoryMenu = menu_manager_get_by_tag("player_inventory");
	inventory_set_active(NULL, inventoryMenu);
}

void chest_close_inventory(Chest *chest)
{
	inventory_set_inactive(NULL, chest->chestMenu);
	chest->state = CHEST_CLOSED;

	Menu* inventoryMenu = menu_manager_get_by_tag("player_inventory");
	inventory_set_inactive(NULL, inventoryMenu);
	
	//slog("test");
}

void chest_free(Chest *chest)
{

}