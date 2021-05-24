#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gfc_types.h"
#include "gfc_text.h"
#include "gf2d_sprite.h"

#include "w_tile.h"
#include "w_room.h"

#include "e_entity.h"


typedef struct
{
	Bool _inuse;
	Bool _current;



	int		maxRows;
	int		maxColumns;
	int		maxTemplates;
	int		loadedRooms;  //total number of loaded room templates
	Room	***room_list;
	Room	**template_list;
	Uint32  max_rooms;
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


Level *level_new(int maxRows, int maxColumns, Uint32 max_rooms, Uint32 max_templates);


void level_update(Level *level);

  
void level_draw(Level *level);


void level_free(Level *level);


void level_init_all(Level *level);


void level_build_branches(Level *level);


void level_build_branch_room(Room *startRoom, Level *level);


Vector2D level_get_start_pos(Level *level);


void room_slog(Level *level);

#endif