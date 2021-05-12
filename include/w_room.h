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

/**
* @brief starts the level_manager and allocates the memory for it to run
* @params the number of level that will be in the level_list
*/

void room_manager_init(int maxRows, int maxColumns, Uint32 max_rooms, Uint32 max_templates);


/**
* @brief frees all resources used by the room manager
*/
void room_manager_free();

/**
* @brief returns the 2d array of all rooms in the manager
* @return the room list array
*/
Room **room_manager_get_room_list();

/**
* @brief gets the max room columns
* @return max columns
*/
int room_manager_get_max_columns();

/**
* @brief gets the max room rows
* @return max rows
*/
int room_manager_get_max_rows();

/**
* @brief returns room at a specific grid position
* @param x the x of the room
* @param y the y of the room
* @return a pointer to the room at that x and y, NULL if no room found
*/
Room *room_manager_get_room(int x, int y);

/**
* @brief loads all room template files found in the templates folder
*/
void room_manager_load_all_templates();

/**
* @brief saves a specific room as a template file
* @param room the room to save as a template, provided by the editor
*/
void room_manager_save_template(Room *room);

/**
* @brief specific code to save room template as json, called by room_manager_save_template
* @param filename the name of the template file to save
* @param the room to save as a template
*/
void room_template_save(const char *filename, Room *room);

/**
* @brief loads a specific room template to the template list
* @param filename the name of the template to load from file
* @return pointer to the room if file found, NULL otherwise
*/
Room *room_template_load(const char *filename);

/**
* @brief creates a new room object in the template list
* @return pointer to the room
*/
Room *room_new_template();

/**
* @brief loads a random template from the template list at the specific position
* @param gridPos the position to spawn the room
* @return pointer to the room
*/
Room *room_template_load_random_from_list(Vector2D gridPos);


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

/**
* @brief creates a new room shell with no objects or doors
* @param position the grid position of the room to creates
* @return pointer to the room
*/
Room *room_empty(Vector2D position);

/**
* @brief lays out the entire level
*/
void room_init_all();

/**
* @brief creates a new tile in the tile manager
* @param the position to create the tile
*/

Tile *room_new_tile(Room *room, Vector2D pos, Vector2D gridPos);

Entity *room_new_enemy(Room * room, Vector2D gridPos, int enemy);

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
* @brief not used anymore with new room system
* @param
* @return
*/
int room_find_tile_by_pos(Room *room, int x, int y);

/**
* @brief opens the selected doors on each side of the leve
* @param room the room to open doors of
*/
void room_open_door(Bool left, Bool top, Bool right, Bool bot, Room *room);

/**
* @brief get position to spawn the player at
* @return Vector2d position to spawn the player at
*/
Vector2D room_manager_get_start_pos();

/**
* @brief checks every room to see if it has an open door, starts recursive banch algorithm if no room found
*/
void room_build_branches();

/**
* @brief recursive algortithm to randomly layout extra rooms that are not part of the standard room path
* @param the room to start the path at
*/
void room_build_branch_room(Room *startRoom);

/**
* @brief debug output to log of all spawned rooms

*/
void room_slog();

#endif