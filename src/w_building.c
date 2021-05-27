#include "simple_logger.h"

#include "g_camera.h"

#include "w_building.h"
#include "w_level.h"

Building *building_new(Vector2D gridPos, Vector2D size)
{

	Building *building;
	building = (Building *)malloc(sizeof(Building));
	if (!building)
	{
		slog("failed to allocate memory for new building");
		return NULL;
	}
	memset(building, 0, sizeof(Building));


	//32 is always size of tiles, should maybe be more robust but its fine for now
	building->position.x = gridPos.x * 32;
	building->position.y = gridPos.y * 32;



	building->gridPos.x = gridPos.x;
	building->gridPos.y = gridPos.y;

	building->size.x = size.x;
	building->size.y = size.y;


	building->_inuse = true;

	slog("Building built at x:%f, y:%f", building->gridPos.x, building->gridPos.y);

	return building;
}


void building_free(Building *building)
{
	if (!building)return;// nothing to do

	memset(building, 0, sizeof(Building));

	gf2d_sprite_free(building->sprite);

	building->sprite = NULL;

	building->_inuse = false;
	building = NULL;

}


void building_draw(Building *building)
{
	Vector2D drawPosition, offset;
	Vector2D drawScale = camera_get_scale();

	if (!building)
	{
		slog("cannot draw a NULL tile");
		return;
	}
	if (!building->_inuse)
	{
		return;
	}

	if (building->draw)
	{
		building->draw(building);
	}
	else
	{
		if (building->sprite == NULL)
		{
			return;// nothing to draw
		}
		offset = camera_get_offset();
		if (!camera_rect_on_screen(gfc_sdl_rect((building->position.x), (building->position.y), building->sprite->frame_w, building->sprite->frame_h)))
		{
			//entity is off camera, skip
			//return;
		}
		drawPosition.x = building->position.x + offset.x;
		drawPosition.y = building->position.y + offset.y;



		gf2d_sprite_draw(
			building->sprite,
			drawPosition,
			&drawScale,
			NULL,
			NULL,
			NULL,
			NULL,
			0);
	}

	//test code to draw the hitboxes for an ent that has one

	/*
	if (&tile->hitbox)
	{

	SDL_Rect tempDraw;

	tempDraw.x = tile->hitbox.x + offset.x;
	tempDraw.y = tile->hitbox.y + offset.y;
	tempDraw.w = tile->hitbox.w;
	tempDraw.h = tile->hitbox.h;

	SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 0, 255, 0, 255);

	SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &tempDraw);
	}
	*/
}