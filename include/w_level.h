#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gfc_types.h"
#include "gfc_text.h"
#include "gf2d_sprite.h"

#include "w_tile.h"

#include "e_entity.h"


typedef struct
{
	Bool		_current; /**<set to true for the level that is currently in use*/
	Bool		_inuse;
	Sprite     **bgImage;    /**<the background image for the level*/
	int         bgImageCount;/**<how many we have*/
	Vector2D    levelSize;   /**<how large, in pixels, the level is*/
	Vector2D	levelPos;	// gridPos for where the level is, 0, 0 will be hub room
	Vector2D	door1, door2, door3, door4;  //door1 on left, door2 on top, door3 on right, door4 on bottom
	Tile		**tileArray;  // array of all tiles that are in the level
	Entity		**entityArray; //array of all enemies and their spawn positions
	int			tileArrayLen;
	int			entityArrayLen;
	Sprite     *tileSet;     /**<sprite for the tileset*/
	TileTypes  *tileMap;     /**<the tiles for the level*/
	Uint32      tileCount;
	Uint32      levelWidth;  /**<how many tiles per row the level has*/
	Uint32      levelHeight; /**<how many tiles per column the level has*/
	int         tileWidth;   /**<now many pixels wide the tiles are*/
	int         tileHeight;  /**<how many pixels tall each tile is*/
	int         tileFPL;


}Level;

/**
* @brief starts the level_manager and allocates the memory for it to run
* @params the number of level that will be in the level_list
*/

void level_manager_init(Uint32 max_levels);


/**
* @brief frees all resources used by the level manager
*/

void level_manager_free();


/**
* @brief allocate and initialize a level
* @return NULL on error, else a pointer to a new level
*/
Level *level_new();


/**
* @brief load a level from a json file
* @param filename the file to load
* @return NULL on error (See the logs) a loaded level otherwise
*/
Level *level_load(const char *filename);

/**
* @brief create a new random level
* @params width of level in tiles, height of level in tiles
* @return the newly created level
*/

Level *level_hub();


Level *level_random(int width, int height, Vector2D levelPos);

/**
* @brief creates a new tile in the tile manager
* @param the position to create the tile
*/

Tile *level_new_tile(Level * level, Vector2D pos);

Entity *level_new_enemy(Level * level, Vector2D gridPos, int enemy);

/**
* @brief free up a previously loaded level
* @param level a pointer to the level to free
*/

void level_free(Level *level);

/**
* @brief perform maintenance for the level.  should be called once a frame
* @param level the level to update
*/
void level_update(Level *level);


/**
* @brief draw a level
* @param level the level the draw
*/
void level_draw(Level *level);

/**
* @brief finds and returns the index of a tile in the tile array with the given gridpos
* @param x and y of the tile to find, the level to search
* @return the index of the tile in tileArray, -1 if no tile found
*/
int level_find_tile_by_pos(Level *level, int x, int y);

Bool create_random_platform(int number, int minWidth, int maxWidth, Level *level);

Bool spawn_platform(Vector2D gridPos, int width, Level *level);

Bool level_create_doors(Level *level, int y);

void level_change(Entity *player, int door, int y);

/**
* @brief finds and returns the level with the given gridpos
* @param x and y of the level to find
* @return the level in level_manager, NULL if no level found
*/

Level *level_find_level_by_pos(Vector2D position);

void level_layout_random_enemies(int minEnemies, int maxEnemies, Level *level);
#endif