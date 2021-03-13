#include "simple_logger.h"

#include "tile.h"
#include "camera.h"


Tile *tile_new(int width, int height, Vector2D position)
{

	Tile *tile;
	tile = (Tile *)malloc(sizeof(Tile));
	if (!tile)
	{
		slog("failed to allocate memory for tile at grid x:%i, y:%i", position.x, position.y);
		return NULL;
	}
	memset(tile, 0, sizeof(Tile));
	tile->sprite = gf2d_sprite_load_image("assets/sprites/testTile.png");

	tile->tileWidth = width;
	tile->tileHeight = height;

	tile->gridPos.x = position.x;
	tile->gridPos.y = position.y;

	tile->hitbox.x = position.x * width;
	tile->hitbox.y = position.y * height;
	tile->hitbox.w = tile->tileWidth;
	tile->hitbox.h = tile->tileHeight;

	return tile;
}


void tile_free(Tile *tile)
{
	int i;
	if (!tile)return;// nothing to do

	gf2d_sprite_free(tile->sprite);

	tile->sprite = NULL;

	free(tile);
}


void tile_draw(Tile *tile)
{
	Vector2D drawPosition, offset;
	if (!tile)
	{
		slog("cannot draww a NULL tile");
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
		if (!camera_rect_on_screen(gfc_sdl_rect((tile->gridPos.x * tile->tileWidth), (tile->gridPos.y * tile->tileHeight), tile->sprite->frame_w, tile->sprite->frame_h)))
		{
			//entity is off camera, skip
			//return;
		}
		drawPosition.x = (tile->gridPos.x * 32) + offset.x;
		drawPosition.y = (tile->gridPos.y * 32) + offset.y;

		gf2d_sprite_draw(
			tile->sprite,
			drawPosition,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);
	}

	//test code to draw the hitboxes for an ent that has one

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
}