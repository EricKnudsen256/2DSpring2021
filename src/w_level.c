#include <stdlib.h>

#include "simple_json.h"
#include "simple_logger.h"


#include "w_level.h"

static LevelManager level_manager = { 0 };


void level_manager_init(Uint32 max_levels)
{
	if (max_levels == 0)
	{
		slog("cannot allocate 0 rooms!");
		return;
	}
	if (level_manager.level_list != NULL)
	{
		level_manager_free();
	}

	level_manager.level_list = (Level *)gfc_allocate_array(sizeof (Level), max_levels);


	if (level_manager.level_list == NULL)
	{
		slog("failed to allocate room list!");
		return;
	}

	level_manager.max_levels = max_levels;
	slog("room system initialized");

}

void level_manager_update()
{
	int x;
	if (level_manager.level_list == NULL)
	{
		slog("level system does not exist");
		return;
	}
	for (x = 0; x < level_manager.max_levels; x++)
	{
		if (level_manager.level_list[x]._inuse == 0)continue;
		level_update(&level_manager.level_list[x]);
	}
}

void level_manager_draw()
{
	int x;
	if (level_manager.level_list == NULL)
	{
		slog("level system does not exist");
		return;
	}
	for (x = 0; x < level_manager.max_levels; x++)
	{
		if (level_manager.level_list[x]._inuse == 0)continue;
		level_draw(&level_manager.level_list[x]);
	}
}

void level_manager_free()
{
	if (level_manager.level_list != NULL)
	{
		free(level_manager.level_list);
	}
	memset(&level_manager, 0, sizeof(LevelManager));
	slog("level system closed");
}

Level *level_manager_get_level_list()
{
	return level_manager.level_list;
}


Level *level_new(int maxRows, int maxColumns, Uint32 max_rooms, Uint32 max_templates)
{
	Level *level;
	int i;
	if (level_manager.level_list == NULL)
	{
		slog("level system does not exist");
		return NULL;
	}

	level = NULL;

	for (i = 0; i < level_manager.max_levels; i++)
	{
		if (level_manager.level_list[i]._inuse)continue;// someone else is using this one
		memset(&level_manager.level_list[i], 0, sizeof(Level));
		level_manager.level_list[i]._inuse = 1;
		level = &level_manager.level_list[i];
	}

	if (!level)
	{
		slog("no free levels available");
		return NULL;
	}

	level->room_list = (Room *)gfc_allocate_array(sizeof (Room), maxRows);

	for (int i = 0; i < maxColumns; i++)
	{
		level->room_list[i] = (Room *)gfc_allocate_array(sizeof (Room), maxColumns);
	}

	if (level->room_list == NULL)
	{
		slog("failed to allocate room list for level!");
		return;
	}

	level->maxRows = maxRows;
	level->maxColumns = maxColumns;
	level->max_rooms = max_rooms;

	level->template_list = (Room *)gfc_allocate_array(sizeof (Room), max_templates);
	level->maxTemplates = max_templates;

	level_load_all_templates(level);

	return level;
}

void level_update(Level *level)
{
	int x, y;
	if (level->room_list == NULL)
	{
		slog("level does not exist");
		return;
	}
	for (x = 0; x < level->maxColumns; x++)
	{
		for (y = 0; y < level->maxRows; y++)
		{
			if (level->room_list[x][y]->_inuse == 0)continue;

			room_update(level->room_list[x][y]);
		}
	}
}

void level_draw(Level *level)
{
	int x, y;
	if (level->room_list == NULL)
	{
		slog("level does not exist");
		return;
	}
	for (x = 0; x < level->maxColumns; x++)
	{
		for (y = 0; y < level->maxRows; y++)
		{
			if (level->room_list[x][y]->_inuse == 0)continue;

			room_draw(level->room_list[x][y]);
		}
	}
}

void level_free(Level *level)
{
	if (level->room_list != NULL)
	{
		free(level->room_list);
	}
	memset(&level, 0, sizeof(Level));
}

void level_init_all()
{

}



/*file footer*/