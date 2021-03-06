#include "tile.h"


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
	return tile;
}


void tile_free(Tile *tile)
{
	int i;
	if (!tile)return;// nothing to do

	gf2d_sprite_free(tile->sprite);

	tile->sprite = NULL;
	tile->_inuse = 0;

	free(tile);
}


void tile_draw(Tile *tile)
{

}