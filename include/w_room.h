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

	Bool		leftDoor, topDoor, rightDoor, botDoor;			//true means has door on that side

	Vector2D	position;

	Tile		***tileArray;  // subject to change to accomodate new room structure in the future
	Entity		**entityArray; //array of all enemies and their spawn positions, again subject to change
	int			tileArrayLen;
	int			entityArrayLen;
	Sprite     *tileSet;     /**<sprite for the tileset*/
	Sprite	   *bgSprite;

	int         tileWidth;   /**<now many pixels wide the tiles are*/
	int         tileHeight;  /**<how many pixels tall each tile is*/
}Room;

typedef struct
{
	int		maxRows;
	int		maxColumns;
	int		maxTemplates;
	int		loadedRooms;  //total number of loaded room templates
	Room	**room_list;
	Room	*template_list;
	Uint32  max_rooms;
}RoomManager;


void room_template_save(const char *filename, Room *room);

/**
* @brief loads a specific room template to the template list
* @param filename the name of the template to load from file
* @return pointer to the room if file found, NULL otherwise
*/
Room *room_template_load(const char *filename, Room *room);


/**
* @brief allocate and initialize a level
* @return NULL on error, else a pointer to a new level
*/
Room *room_new();

/**
* @brief creates a new room shell with no objects or doors
* @param position the grid position of the room to creates
* @return pointer to the room
*/
Room *room_empty(Vector2D position);


/**
* @brief creates a new tile in the tile manager
* @param the position to create the tile
*/

Tile *room_new_tile(Room *room, Vector2D pos, Vector2D gridPos);

/**
* @brief free up a previously loaded level
* @param level a pointer to the level to free
*/

void room_free_tile(int x, int y, Room* room);

/**
* @brief frees a room from memory
* @param x the x coord of the rrom
* @param y the y coord of the room
* @param room pointer to the room to free for backwards compatibility
*/
void room_free(int x, int y, Room *room);

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

/**
* @brief opens the selected doors on each side of the leve
* @param room the room to open doors of
*/
void room_open_door(Bool left, Bool top, Bool right, Bool bot, Room *room);

#endif