#include "simple_logger.h"

#include "g_camera.h"

#include "w_tile.h"


Tile *tile_new(int width, int height, Vector2D position, Vector2D gridPos)
{

	Tile *tile;
	tile = (Tile *)malloc(sizeof(Tile));
	if (!tile)
	{
		slog("failed to allocate memory for tile at grid x:%i, y:%i", position.x, position.y);
		return NULL;
	}
	memset(tile, 0, sizeof(Tile));
	tile->sprite = gf2d_sprite_load_image("assets/sprites/stoneTile.png");

	tile->tileWidth = width;
	tile->tileHeight = height;

	tile->pos.x = position.x;
	tile->pos.y = position.y;

	tile->gridPos.x = gridPos.x;
	tile->gridPos.y = gridPos.y;

	tile->hitbox.x = position.x;
	tile->hitbox.y = position.y;
	tile->hitbox.w = tile->tileWidth;
	tile->hitbox.h = tile->tileHeight;

	tile->_inuse = true;

	return tile;
}


void tile_free(Tile *tile)
{
	if (!tile)return;// nothing to do

	memset(tile, 0, sizeof(Tile));

	gf2d_sprite_free(tile->sprite);

	tile->sprite = NULL;

	tile->_inuse = false;
	tile = NULL;

}


void tile_draw(Tile *tile)
{
	Vector2D drawPosition, offset;
	Vector2D drawScale = camera_get_scale();

	if (!tile)
	{
		slog("cannot draww a NULL tile");
		return;
	}
	if (!tile->_inuse)
	{
		return;
	}

	if (tile->draw)
	{
		tile->draw(tile);
	}
	else
	{
		if (tile->sprite == NULL)
		{
			return;// nothing to draw
		}
		offset = camera_get_offset();
		if (!camera_rect_on_screen(gfc_sdl_rect((tile->pos.x * tile->tileWidth), (tile->pos.y * tile->tileHeight), tile->sprite->frame_w, tile->sprite->frame_h)))
		{
			//entity is off camera, skip
			//return;
		}
		drawPosition.x = tile->pos.x + offset.x;
		drawPosition.y = tile->pos.y + offset.y;

		

		gf2d_sprite_draw(
			tile->sprite,
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

void tile_set_tile_pos(Vector2D tilePos, Tile *tile)
{
	tile->pos.x = tilePos.x;
	tile->pos.y = tilePos.y;
}