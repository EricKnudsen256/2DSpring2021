#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gfc_types.h"
#include "gfc_text.h"
#include "gf2d_sprite.h"

#include "e_entity.h"
#include "e_drop.h"

#include "w_building.h"
#include "w_tile.h"
#include "w_room.h"
#include "w_interactable.h"
#include "w_ore_node.h"



typedef struct
{
	Bool		_inuse;
	Bool		_current;

	Vector2D	gridSize; //vector2d with x and y size for the tile grid of each level, default is 32 by 32 pixels

	int			maxRows;
	int			maxColumns;
	int			maxTemplates;
	int			loadedRooms;  //total number of loaded room templates
	Room		***room_list;
	Room		**template_list;

	int			max_interactables;
	Interactable **interactable_list; //not going to attach to grid, because this gives me more control over it

	int			max_buildings;
	Building	**building_list;

	int			max_ore;
	Ore_Node	**ore_list;

	int			max_drops; //should be less drops than possible entities
	Drop		**drop_list;

	Uint32		max_rooms;
}Level;

typedef struct
{
	int max_levels;
	Level *level_list;

}LevelManager;

void level_manager_init(Uint32 max_levels);


void level_manager_update();


void level_manager_draw();


void level_manager_free();


Level *level_manager_get_level_list();


void level_manager_load_all_templates(Level *level);


void level_manager_save_template(Room *room);


Level *level_manager_get_current();


Room *level_template_load_random_from_list(Vector2D gridPos, Level *level);


Room *level_room_new(Vector2D gridPos, Level *level);


Room *level_room_empty(Vector2D gridPos, Level *level);


Room *level_room_new_template(Level *level);


Level *level_new(int maxRows, int maxColumns, Uint32 max_rooms, Uint32 max_templates, Uint32 max_buildings, Uint32 max_interactables, Uint32 max_drops);


void level_update(Level *level);

  
void level_draw(Level *level);


void level_free(Level *level);


void level_init_all(Level *level);


void level_build_branches(Level *level);


void level_build_branch_room(Room *startRoom, Level *level);

//building functions
Building *level_building_new(Vector2D gridPos, Vector2D size, Level *level);


void level_building_free(Building *building, Level *level);


Bool level_building_check_if_placable(Vector2D position, Vector2D size, Level *level);


void level_test_building(Vector2D buildPos, Level *level);

//ore functions
Ore_Node *level_ore_node_new(Vector2D gridPos, char *type, Level *level);


void level_ore_node_free(Ore_Node *node, Level *level);


void level_test_node(Vector2D gridPos, Level *level);


void level_layout_ore(int orePerRoom, int maxOre, Level *level);


//interactable functions
Interactable *level_interact_new(Vector2D *position, Vector2D *gridPos, int width, int height, Level *level);


void level_interact_free(Interactable *interact, Level *level);


Bool level_check_interact(Level *level);


//drop functions

Drop *level_new_drop(Vector2D position, Item *item, Level *level);


void level_drop_free(Drop *drop, Level *level);


//uses gridposes
Bool level_check_for_tiles(Vector2D position, Vector2D size, Level *level);


Vector2D level_get_start_pos(Level *level);


void room_slog(Level *level);

#endif