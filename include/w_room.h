#ifndef __ROOM_H__
#define __ROOM_H__

#include "gfc_types.h"
#include "gfc_text.h"
#include "gf2d_sprite.h"

#include "w_tile.h"

#include "e_entity.h"


typedef struct
{
	Bool		_inuse;
	Vector2D	roomSize;   /**<how large, in pixels, the level is*/
	Vector2D	roomPos;	// gridPos for where the level is, 0, 0 will be hub room
	int			roomType;	//0: side room, not solution, 1: room with left + right exit, 2: room with left right bottom exit, 3: room with left right top exit
	Uint32      roomWidth;  /**<how many tiles per row the level has*/
	Uint32      roomHeight; /**<how many tiles per column the level has*/

	int			leftDoor, topDoor, rightDoor, botDoor;			//used to check if a room has a specific door open without a connection to an adjacent room

	Vector2D	position;

	Tile		***tileArray;  // subject to change to accomodate new room structure in the future
	Entity		**entityArray; //array of all enemies and their spawn positions, again subject to change
	int			tileArrayLen;
	int			entityArrayLen;
	Sprite     *tileSet;     /**<sprite for the tileset*/
	Sprite	   *bgSprite;
	Uint32      tileCount;

	int         tileWidth;   /**<now many pixels wide the tiles are*/
	int         tileHeight;  /**<how many pixels tall each tile is*/
}Room;

typedef struct
{
	int		maxRows;
	int		maxColumns;
	Room	**room_list;
	Uint32  max_rooms;
}RoomManager;

/**
* @brief starts the level_manager and allocates the memory for it to run
* @params the number of level that will be in the level_list
*/

void room_manager_init(int maxRows, int maxColumns, Uint32 max_rooms);


/**
* @brief frees all resources used by the level manager
*/

void room_manager_free();

Room **room_manager_get_room_list();

int room_manager_get_max_columns();

int room_manager_get_max_rows();

Room *room_manager_get_room(int x, int y);




/**
* @brief allocate and initialize a level
* @return NULL on error, else a pointer to a new level
*/
Room *room_new(Vector2D gridPos);


/**
* @brief load a level from a json file
* @param filename the file to load
* @return NULL on error (See the logs) a loaded level otherwise
*/
Room *room_load(const char *filename);


Room *room_empty(Vector2D position);

void room_init_all();

/**
* @brief creates a new tile in the tile manager
* @param the position to create the tile
*/

Tile *room_new_tile(Room * room, Vector2D pos);

Entity *room_new_enemy(Room * room, Vector2D gridPos, int enemy);

/**
* @brief free up a previously loaded level
* @param level a pointer to the level to free
*/

void room_free(Room *room);

/**
* @brief perform maintenance for the level.  should be called once a frame
* @param level the level to update
*/
void room_update(Room *room);


/**
* @brief draw a level
* @param level the level the draw
*/
void room_draw(Room *room);


int room_find_tile_by_pos(Room *room, int x, int y);

void room_open_door(Bool left, Bool top, Bool right, Bool bot, Room *room);

Vector2D room_manager_get_start_pos();

void room_slog();


/**
* @brief finds and returns the index of a tile in the tile array with the given gridpos
* @param x and y of the tile to find, the level to search
* @return the index of the tile in tileArray, -1 if no tile found
*/

/*
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

/*
Level *level_find_level_by_pos(Vector2D position);

void level_layout_random_enemies(int minEnemies, int maxEnemies, Level *level);

*/


#endif