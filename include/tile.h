#ifndef __TILE_H__
#define __TILE_H__


#include "gf2d_sprite.h"

typedef enum
{
	TT_Empty,
	TT_Brick,
	TT_MAX
}TileTypes;

typedef struct
{
	Vector2D	gridPos;	//position of tile on tilegrid
	Sprite		*sprite;
	SDL_Rect	hitbox;
	int			tileWidth;
	int			tileHeight;

	void(*update)(struct Tile *tile);
	void(*think)(struct Tile *tile);
	void(*draw)(struct Tile *tile);
	void(*free)(struct Tile *tile);

}Tile;


/**
* @brief creates a new tile at the selected position
* @params width width of the tile, height height of the tile, position vector2d representation of the tile's grid position
* @return a reference to a new tile, or null if failed
*/
Tile *tile_new(int width, int height, Vector2D position);

/**
* @brief frees a tile from memeory
* @params the tile to be freed
*/
void tile_free(Tile *tile);

/**
* @brief draws a single tile
* @params the tile to be drawn
*/
void tile_draw(Tile *tile);


#endif