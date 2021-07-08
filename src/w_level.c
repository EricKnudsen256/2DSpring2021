#include <stdlib.h>

#include "simple_json.h"
#include "simple_logger.h"

#include "p_player.h"

#include "w_level.h"
#include "w_building.h"
#include "w_interactable.h"

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
	slog("level system initialized");

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

void level_load_all_templates(Level *level)
{

	Room *roomTemplate;
	int i;
	char filename[50];
	char filestart[] = "templates/room_template_";
	char filenum[5];
	char fileend[] = ".json";

	level->loadedRooms = 0;


	for (i = 0; i < level->maxTemplates; i++)
	{
		strcpy(filename, "");

		itoa(i, filenum, 10);


		strcat(filename, filestart);
		strcat(filename, filenum);
		strcat(filename, fileend);

		//slog("%s", filename);

		roomTemplate = level_room_new_template(level);

		if (!room_template_load(&filename, roomTemplate))
		{
			//slog("last room = %i", i);
			break;
		}

		level->loadedRooms++;

	}

}

void level_manager_save_template(Room *room)
{

	SJson *json;
	int i = 0;
	char filename[50];
	char filestart[] = "templates/room_template_";
	char filenum[5];
	char fileend[] = ".json";

	strcpy(filename, "");

	itoa(i, filenum, 10);

	strcat(filename, filestart);
	strcat(filename, filenum);
	strcat(filename, fileend);



	while (sj_load(&filename))
	{
		strcpy(filename, "");

		itoa(i, filenum, 10);


		strcat(filename, filestart);
		strcat(filename, filenum);
		strcat(filename, fileend);
		i++;
	}

	room_template_save(&filename, room);

	slog("File saved as: %s", filename);
}

Level *level_manager_get_current()
{
	for (int i = 0; i < level_manager.max_levels; i++)
	{
		if (level_manager.level_list[i]._current)
		{
			return &level_manager.level_list[i];
		}
	}
}

Room *level_template_load_random_from_list(Vector2D gridPos, Level *level)  //loads random template from template list
{
	int rnd;
	Room *templateRoom, *room;
	Vector2D tilePos;
	Vector2D tempVec;
	Vector2D spawnPos;

	int temp;

	room = level_room_empty(gridPos, level);

	return room;

	if (level->loadedRooms == 0)
	{
		slog("No rooms loaded");
		return room;
	}
	if (room->roomType == 5)
	{
		return room;
	}

	rnd = random_int_range(0, level->loadedRooms - 1);

	if (level->template_list[rnd])
	{

		templateRoom = level->template_list[rnd];

		//Bool		_inuse;

		room->_inuse = 1;



		//Vector2D	roomSize;   /**<how large, in pixels, the level is*/

		tempVec = templateRoom->roomSize;

		room->roomSize = tempVec;


		//Vector2D	roomPos;	// gridPos for where the level is, 0, 0 will be hub room

		room->roomPos = gridPos;

		//int			roomType;	//0: side room, not solution, 1: room with left + right exit, 2: room with left right bottom exit, 3: room with left right top exit

		room->roomType = 0;

		//Uint32      roomWidth;  /**<how many tiles per row the level has*/

		temp = templateRoom->roomWidth;
		room->roomWidth = temp;

		//Uint32      roomHeight; /**<how many tiles per column the level has*/

		temp = templateRoom->roomHeight;
		room->roomHeight = temp;

		//Bool		leftDoor, topDoor, rightDoor, botDoor;			//true means has door on that side

		room->leftDoor = false;
		room->topDoor = false;
		room->rightDoor = false;
		room->botDoor = false;



		//Vector2D	position;

		room->position = vector2d((room->tileWidth * room->roomWidth * gridPos.x),
			(room->tileHeight * room->roomHeight * gridPos.y));

		//int         tileWidth;   /**<now many pixels wide the tiles are*/

		temp = templateRoom->tileWidth;
		room->tileWidth = temp;

		//int         tileHeight;  /**<how many pixels tall each tile is*/

		temp = templateRoom->tileHeight;
		room->tileHeight = temp;



		//Tile		***tileArray;  // manually set each tile position because can't do it in the template


		for (int x = 0; x < room->roomWidth; x++)
		{
			for (int y = 0; y < room->roomWidth; y++)
			{
				tilePos = vector2d((room->tileWidth * room->roomWidth * gridPos.x) + x * room->tileWidth,
					(room->tileHeight * room->roomHeight * gridPos.y) + y * room->tileHeight);

				if (templateRoom->tileArray[x][y] && x > 3 && x < room->roomWidth - 4 && y > 3 && y < room->roomHeight - 4)
				{
					spawnPos = vector2d(x, y);

					room_new_tile(room, tilePos, spawnPos);
				}

			}
		}

		for (int x = 0; x < templateRoom->roomWidth; x++)
		{
			for (int y = 0; y < templateRoom->roomWidth; y++)
			{
				tilePos = vector2d((room->tileWidth * room->roomWidth * gridPos.x) + x * room->tileWidth,
					(room->tileHeight * room->roomHeight * gridPos.y) + y * room->tileHeight);

				if (room->tileArray[x][y])
				{
					room->tileArray[x][y]->pos = tilePos;
					room->tileArray[x][y]->hitbox.x = tilePos.x;
					room->tileArray[x][y]->hitbox.y = tilePos.y;
				}

			}
		}


		//Entity		**entityArray; //array of all enemies and their spawn positions, again subject to change


		room->entityArrayLen = 50;
		room->entityArray = (Entity **)gfc_allocate_array(sizeof(Entity*), room->entityArrayLen);



		//Sprite     *tileSet;     /**<sprite for the tileset*/
		//Sprite	   *bgSprite;

		room->bgSprite = gf2d_sprite_load_image("assets/sprites/backgrounds/cave.png");

		//templateRoom->_inuse = 0;

		return room;
	}


	//slog("Template not found");
	return room;

}

Room *level_room_new(Vector2D gridPos, Level *level)
{
	if (level->room_list == NULL)
	{
		slog("level does not have a room list!");
		return NULL;
	}

	int x, y;
	x = gridPos.x;
	y = gridPos.y;

	if (x >= level->maxColumns || y >= level->maxRows)
	{
		slog("Room not in range of grid x:%i, y:%i", x, y);
		return NULL;
	}

	if (!level->room_list[x][y])
	{
		level->room_list[x][y] = room_new();
		level->room_list[x][y]->_inuse = 1;
		level->loadedRooms++;
		return level->room_list[x][y];
	}

	slog("room already exists in that spot");
	return NULL;
}

void level_room_free(int x, int y, Level *level)
{
	if (!level)
	{
		slog("no level given");
		return;
	}

	if (!level->room_list[x][y])
	{
		slog("no room to free");
		return;
	}


	memset(level->room_list[x][y], 0, sizeof(Room));

	level->room_list[x][y]->_inuse = false;
	level->room_list[x][y] = NULL;
}


Room *level_room_empty(Vector2D gridPos, Level *level)
{
	Vector2D temp;
	Vector2D tilePos;
	const char *string;
	Room *room;
	int count;
	int tileIndex;
	int i;
	int r, c;


	room = level_room_new(gridPos, level);
	if (!room)
	{
		return NULL;
	}

	//slog("creating room");

	room->roomWidth = 38;
	room->roomHeight = 38;


	//Test tileset, add paramter to function for this


	room->tileWidth = 32;
	room->tileHeight = 32;


	//System to initialize random level



	room->tileArray = (Tile *)gfc_allocate_array(sizeof(Tile*), room->roomHeight);

	for (int i = 0; i < room->roomWidth; i++)
	{
		room->tileArray[i] = (Tile *)gfc_allocate_array(sizeof(Tile*), room->roomWidth);
	}

	//Initializes tileArray to the size needed if all tiles were used in the level



	room->entityArrayLen = 50;
	room->entityArray = (Entity **)gfc_allocate_array(sizeof(Entity*), room->entityArrayLen);



	//loop to initialize the tiles in the level
	for (r = 0; r < room->roomHeight; r++)
	{
		for (c = 0; c < room->roomWidth; c++)
		{

			//test code to make sure that the tiles are being created properly
			if (r == 3 || r == room->roomHeight - 4 || c == 3 || c == room->roomWidth - 4)
			{

				if (r < 3 || r > room->roomHeight - 4 || c < 3 || c > room->roomWidth - 4)
				{
					continue;
				}

				//slog("creating tile at x:%i, y:%i", c, r);
				temp.x = c;
				temp.y = r;

				tilePos = vector2d((room->tileWidth * room->roomWidth * gridPos.x) + c * room->tileWidth,
					(room->tileHeight * room->roomHeight * gridPos.y) + r * room->tileHeight);

				room_new_tile(room, tilePos, temp);
			}

		} //end of loop 1

	} //end of loop 2

	//open doors


	room->roomSize.x = room->roomWidth * room->tileWidth;
	room->roomSize.y = room->roomHeight * room->tileHeight;

	room->_inuse = 1;

	room->position = vector2d((room->tileWidth * room->roomWidth * gridPos.x),
		(room->tileHeight * room->roomHeight * gridPos.y));
	room->roomPos = gridPos;

	room->bgSprite = gf2d_sprite_load_image("assets/sprites/backgrounds/cave.png");

	room->roomType = 0;

	return room;
}

Room *level_room_new_template(Level *level)
{
	if (level->template_list == NULL)
	{
		slog("room system does not exist");
		return NULL;
	}

	for (int i = 0; i < level->maxTemplates; i++)
	{
		if (!level->template_list[i])
		{
			level->template_list[i] = room_new();
			return level->template_list[i];
		}
	}

	slog("template list full");
	return NULL;
}

Level *level_new(int maxRows, int maxColumns, Uint32 max_rooms, Uint32 max_templates, Uint32 max_buildings, Uint32 max_interactables)
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

	level->gridSize = vector2d(32, 32);

	level->template_list = (Room *)gfc_allocate_array(sizeof (Room), max_templates);
	level->maxTemplates = max_templates;

	level->interactable_list = (Interactable *)gfc_allocate_array(sizeof(Interactable), max_interactables);
	level->max_interactables = max_interactables;

	level->building_list = (Building *)gfc_allocate_array(sizeof (Building), max_buildings);
	level->max_buildings = max_buildings;

	level->ore_list = (Ore_Node*)gfc_allocate_array(sizeof(Ore_Node), 1000);
	level->max_ore = 1000;

	level_load_all_templates(level);

	return level;
}

void level_update(Level *level)
{
	int x, y;
	if (level->room_list == NULL)
	{
		//slog("level does not exist");
		return;
	}
	for (x = 0; x < level->maxColumns; x++)
	{
		for (y = 0; y < level->maxRows; y++)
		{
			if (!level->room_list[x][y])continue;
			room_update(level->room_list[x][y]);
		}
	}

	for (int i = 0; i < level->max_ore; i++)
	{
		if (!level->ore_list[i])continue;
		ore_node_update(level->ore_list[i]);
	}

	for (int i = 0; i < level->max_interactables; i++)
	{
		if (!level->interactable_list[i])continue;
		interactable_update(level->interactable_list[i]);
	}

	
}

void level_draw(Level *level)
{
	int x, y, i;
	if (level->room_list == NULL)
	{
		return;
	}
	for (x = 0; x < level->maxColumns; x++)
	{
		for (y = 0; y < level->maxRows; y++)
		{
			if (!level->room_list[x][y])continue;

			room_draw(level->room_list[x][y]);
		}
	}

	for (i = 0; i < level->max_buildings; i++)
	{

		if (!level->building_list[i])continue;

		building_draw(level->building_list[i]);
	}

	for (i = 0; i < level->max_ore; i++)
	{

		if (!level->ore_list[i])continue;

		ore_node_draw(level->ore_list[i]);
	}

	for (i = 0; i < level->max_interactables; i++)
	{

		if (!level->interactable_list[i])continue;

		interactable_draw(level->interactable_list[i]);
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

void level_init_all(Level *level)
{
	Bool endPlaced = false;
	Room *room, *lastRoom, *start, *end;
	Vector2D spawnPos;
	int rnd;
	int x, y;
	int count = 0;
	int roomTypeToApply, lastRoomTypeToApply;

	x = 0;
	y = 3;

	spawnPos = vector2d(x, y);
	//start = room_empty(spawnPos);
	start = level_room_empty(spawnPos, level);
	start->roomType = 4;

	Vector2D enemyPos = vector2d(start->position.x + 500, start->position.y + 500);

	//enemy_spawn_random(enemyPos);
	//enemy_spawn_random(enemyPos);

	

	spawnPos.x++;

	room = level_room_empty(spawnPos, level);
	room->roomType = 3;
	room_open_door(true, false, false, false, room);

	lastRoom = room;

	while (!endPlaced && count < 200) //worry about room types later, get the gen working properly first
	{
		roomTypeToApply = 1;
		lastRoomTypeToApply = -1;

		rnd = random_int_range(0, 4);

		if (rnd == 0 || rnd == 1)
		{

			roomTypeToApply = 1;

			spawnPos.y--;

			if (spawnPos.y < 0)
			{
				spawnPos.y++;
				spawnPos.x++;

				roomTypeToApply = 3;

				level->room_list[x][y]->roomType = 2;

			}

			if (spawnPos.x > level->maxColumns - 1)
			{
				//change room type to exit

				spawnPos.x--;
				endPlaced = true;
				lastRoomTypeToApply = 5;
			}

			x = spawnPos.x;
			y = spawnPos.y;

		}
		else if (rnd == 2 || rnd == 3)
		{

			roomTypeToApply = 1;

			spawnPos.y++;

			if (spawnPos.y > level->maxRows - 1)
			{
				spawnPos.y--;
				spawnPos.x++;

				roomTypeToApply = 3;

				level->room_list[x][y]->roomType = 2;

			}

			if (spawnPos.x > level->maxColumns - 1)
			{
				//change room type to exit
				spawnPos.x--;
				endPlaced = true;
				lastRoomTypeToApply = 5;
			}
		}
		else if (rnd == 4)
		{
			if (lastRoom->roomType == 3)
			{
				continue;
			}

			spawnPos.x++;



			if (spawnPos.x > level->maxColumns - 1)
			{
				spawnPos.x--;
				//change room type to exit
				endPlaced = true;
				lastRoomTypeToApply = 5;
			}
			else
			{
				roomTypeToApply = 3;

				level->room_list[x][y]->roomType = 2;

			}

			x = spawnPos.x;
			y = spawnPos.y;

		}


		x = spawnPos.x;
		y = spawnPos.y;

		if (level->room_list[x][y])
		{
			if (lastRoomTypeToApply == 5)
			{
				level->room_list[x][y]->roomType = lastRoomTypeToApply;
			}
			if (x == 7)
			{
				lastRoom->roomType = 5;
				endPlaced = true;
			}
			continue;
		}

		

		room = level_template_load_random_from_list(spawnPos, level);



		//slog("Roomtype = %i", roomTypeToApply);

		room->roomType = roomTypeToApply;

		if (lastRoom)
		{
			if (lastRoom->roomType != 4 && lastRoomTypeToApply != -1)
			{
				lastRoom->roomType = lastRoomTypeToApply;
			}
		}

		lastRoom = level->room_list[x][y];
		count++;

	}

	

	if (count > 200)
	{
		slog("count reached max, level not fully completed");
		return;
	}

	//code to open all doors


	for (x = 0; x < level->maxColumns; x++)
	{
		for (y = 0; y < level->maxRows; y++)
		{
			Room *room, *lastRoom;
			Bool left, right, top, bot;

			left = false;
			right = false;
			bot = false;
			top = false;

			room = level->room_list[x][y];

			if (!room)
			{
				continue;
			}
			
			if ((room->roomType == 2) && x != 0)
			{
				
				lastRoom = level->room_list[x - 1][y];

				if (lastRoom && (lastRoom->roomType == 2 || lastRoom->roomType == 5))
				{
					left = true;
				}

			}

			if ((room->roomType == 3) && x != 0)
			{
				left = true;
			}

			if ((room->roomType == 2 || room->roomType == 4) && x != level->maxColumns - 1)
			{
				right = true;
			}

			if ((room->roomType == 1 || room->roomType == 2 || room->roomType == 3) && y != 0)
			{
				top = true;
			}

			if ((room->roomType == 1 || room->roomType == 2 || room->roomType == 3) && y != level->maxRows - 1)
			{
				bot = true;
			}

			if (room->roomType == 5)
			{
				lastRoom = room;
			}

			if (x == 1)
			{
				if (level->room_list[x - 1][y] && level->room_list[x - 1][y]->roomType == 4)
				{
					left = true;
				}
			}

			room_open_door(left, top, right, bot, room);
		}
	}


	

	//last room not working well, just manually check if next to anyother room with open door towards it

	x = lastRoom->roomPos.x;
	y = lastRoom->roomPos.y;

	if (y > 0 && y < level->maxRows - 1)
	{
		if (level->room_list[x][y])
		{
			if (level->room_list[x][y + 1] && level->room_list[x][y + 1]->topDoor)
			{
				room_open_door(false, false, false, true, lastRoom);
			}
		}

		if (level->room_list[x][y - 1] && level->room_list[x][y - 1])
		{
			if (level->room_list[x][y - 1]->botDoor)
			{
				room_open_door(false, true, false, false, lastRoom);
			}
		}
	}


	if (level->room_list[x - 1][y] && level->room_list[x - 1][y])
	{
		if (level->room_list[x - 1][y]->rightDoor)
		{
			room_open_door(true, false, false, false, lastRoom);
		}
	}

	level_build_branches(level);

	//spawn random enemies for demo


	for (x = 0; x < level->maxColumns; x++)
	{
		for (y = 0; y < level->maxRows; y++)
		{
			room = level->room_list[x][y];

			if (!room)
			{
				continue;
			}

			Vector2D enemyPos = vector2d(room->position.x + 500, room->position.y + 500);

			if (room->roomType != 4)
			{
				//enemy_spawn_random(enemyPos);
				//enemy_spawn_random(enemyPos);
				//enemy_spawn_random(enemyPos);
			}

		}
	}

	level_layout_ore(1, 100, level);
}

void level_build_branches(Level *level)
{
	int x, y;
	Room *room1, *room2;
	Vector2D roomPos;


	for (x = 0; x < level->maxColumns; x++)
	{
		for (y = 0; y < level->maxRows; y++)
		{

			if (x != 0) //check to make sure not on left wall
			{
				room1 = level->room_list[x][y];

				if (!room1)
				{
					continue;
				}

				if (room1->leftDoor) //if has left door, but no room there
				{
					if (!level->room_list[x - 1][y])
					{
						//slog("Left");
						roomPos = vector2d(x - 1, y);
						room2 = level_template_load_random_from_list(roomPos, level);

						room2->roomType = 0;

						room_open_door(false, false, true, false, room2);

						level_build_branch_room(room2, level);
					}
				}
			}


			if (y != 0) //check to make sure not on top wall
			{
				room1 = level->room_list[x][y];

				if (!room1)
				{
					continue;
				}

				if (room1->topDoor) //if has top door, but no room there
				{
					if (!level->room_list[x][y - 1])
					{
						//slog("Top");
						roomPos = vector2d(x, y - 1);
						room2 = level_template_load_random_from_list(roomPos, level);

						room2->roomType = 0;

						room_open_door(false, false, false, true, room2);

						level_build_branch_room(room2, level);
					}
				}
			}

			if (x != level->maxColumns - 1) //check to make sure not on right wall
			{
				room1 = level->room_list[x][y];

				if (!room1)
				{
					continue;
				}

				if (room1->rightDoor) //if has right door, but no room there
				{
					if (!level->room_list[x + 1][y])
					{
						//slog("Right");
						roomPos = vector2d(x + 1, y);
						room2 = level_template_load_random_from_list(roomPos, level);

						room2->roomType = 0;

						room_open_door(true, false, false, false, room2);

						level_build_branch_room(room2, level);
					}
				}
			}

			if (y != level->maxRows - 1) //check to make sure not on right wall
			{
				room1 = level->room_list[x][y];

				if (!room1)
				{
					continue;
				}

				if (room1->botDoor) //if has right door, but no room there
				{
					if (!level->room_list[x][y + 1])
					{
						//slog("Bottom");
						roomPos = vector2d(x, y + 1);
						room2 = level_template_load_random_from_list(roomPos, level);

						room2->roomType = 0;

						room_open_door(false, true, false, false, room2);

						level_build_branch_room(room2, level);
					}
				}
			}
		}
	}

	//last check to make sure every door is connected
	for (x = 0; x < level->maxColumns; x++)
	{
		for (y = 0; y < level->maxRows; y++)
		{
			if (x != 0) //check to make sure not on left wall
			{
				room1 = level->room_list[x][y];

				if (!room1)
				{
					continue;
				}

				if (room1->leftDoor) //if has left door, but no room there
				{
					if (level->room_list[x - 1][y] && !level->room_list[x - 1][y]->rightDoor)
					{
						room2 = level->room_list[x - 1][y];

						room_open_door(false, false, true, false, room2);
					}
				}
			}


			if (y != 0) //check to make sure not on top wall
			{
				room1 = level->room_list[x][y];

				if (!room1)
				{
					continue;
				}

				if (room1->topDoor) //if has top door, but no room there
				{
					if (level->room_list[x][y - 1] && !level->room_list[x][y - 1]->botDoor)
					{

						room2 = level->room_list[x][y - 1];

						room_open_door(false, false, false, true, room2);

					}
				}
			}

			if (x != level->maxColumns - 1) //check to make sure not on right wall
			{
				room1 = level->room_list[x][y];

				if (!room1)
				{
					continue;
				}

				if (room1->rightDoor) //if has right door, but no room there
				{
					if (level->room_list[x + 1][y] && !level->room_list[x + 1][y]->leftDoor)
					{
						room2 = level->room_list[x + 1][y];

						room_open_door(true, false, false, false, room2);
					}
				}
			}

			if (y != level->maxRows - 1) //check to make sure not on bot wall
			{
				room1 = level->room_list[x][y];

				if (!room1)
				{
					continue;
				}

				if (room1->botDoor) //if has bot door, but no room there
				{
					if (level->room_list[x][y + 1] && !level->room_list[x][y + 1]->topDoor)
					{
						room2 = level->room_list[x][y + 1];

						room_open_door(false, true, false, false, room2);
					}
				}
			}
		}
	}
}

void level_build_branch_room(Room *startRoom, Level *level)
{
	int x, y;
	int rnd;
	Bool built = false;
	Bool triedLeft, triedTop, triedRight, triedBot;
	Vector2D newRoomPos;

	Room *room;

	x = startRoom->roomPos.x;
	y = startRoom->roomPos.y;

	triedLeft = false;
	triedTop = false;
	triedRight = false;
	triedBot = false;


	while (!built && !(triedLeft && triedTop && triedRight && triedBot))
	{
		rnd = random_int_range(0, 3);

		if (rnd == 0) //left
		{
			triedLeft = true;

			if (x > 0)
			{
				if (!level->room_list[x - 1][y])
				{

					built = true;
					newRoomPos = vector2d(x - 1, y);

					room = level_template_load_random_from_list(newRoomPos, level);
					room->roomType = 0;

					room_open_door(false, false, true, false, room);
					room_open_door(true, false, false, false, startRoom);

					level_build_branch_room(room, level);

				}
			}
		}

		if (rnd == 1) //top
		{
			triedTop = true;
			if (y > 0)
			{
				if (!level->room_list[x][y - 1])
				{

					built = true;
					newRoomPos = vector2d(x, y - 1);

					room = level_template_load_random_from_list(newRoomPos,level);
					room->roomType = 0;

					room_open_door(false, false, false, true, room);
					room_open_door(false, true, false, false, startRoom);

					level_build_branch_room(room, level);

				}
			}
		}

		if (rnd == 2) //right
		{
			triedRight = true;
			if (x < level->maxColumns - 1)
			{
				if (!level->room_list[x + 1][y])
				{

					built = true;
					newRoomPos = vector2d(x + 1, y);

					room = level_template_load_random_from_list(newRoomPos, level);
					room->roomType = 0;

					room_open_door(true, false, false, false, room);
					room_open_door(false, false, true, false, startRoom);

					level_build_branch_room(room, level);

				}
			}
		}

		if (rnd == 3) //bot
		{
			triedBot = true;
			if (y < level->maxRows - 1)
			{
				if (!level->room_list[x][y + 1])
				{

					built = true;
					newRoomPos = vector2d(x, y + 1);

					room = level_template_load_random_from_list(newRoomPos, level);
					room->roomType = 0;

					room_open_door(false, true, false, false, room);
					room_open_door(false, false, false, true, startRoom);

					level_build_branch_room(room, level);

				}
			}
		}
	}
}

Building *level_building_new(Vector2D gridPos, Vector2D size, Level *level)
{
	Building *building;

	int i;
	if (level->building_list == NULL)
	{
		slog("level does not have a room list!");
		return NULL;
	}
	for (i = 0; i < level->max_buildings; i++)
	{
		if (level->building_list[i])continue;// someone else is using this one
		level->building_list[i] = building_new(gridPos, size);
		level->building_list[i]->_inuse = 1;
		level->building_list[i]->id = i;

		return level->building_list[i];
	}
	slog("no building slots available");
	return NULL;
}

Bool level_building_check_if_placable(Vector2D position, Vector2D size, Level *level)
{
	//position is the top left of the object

	Vector2D gridPos, tempGridPos;
	Vector2D *gridPosArray;
	int count = 0;


	gridPos = vector2d((int)position.x / 32, (int)position.y / 32);

	if (!level_check_for_tiles(gridPos, size, level))
	{
		return false;
	}

	gridPosArray = gfc_allocate_array(sizeof(Vector2D), size.x * size.y);


	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			gridPosArray[count] = vector2d(gridPos.x + x, gridPos.y + y);
			count++;
		}
	}

	for (int i = 0; i < level->max_buildings; i++)
	{
		if (level->building_list[i])
		{
			for (int j = 0; j < size.x * size.y; j++)
			{
				//slog("Building x:%f, y:%f. Test x:%f, y:%f", level->building_list[i]->gridPos.x, level->building_list[i]->gridPos.y, gridPosArray[j].x, gridPosArray[j].y);

				if (gridPosArray[j].x >= level->building_list[i]->gridPos.x && gridPosArray[j].x < level->building_list[i]->gridPos.x + level->building_list[i]->size.x)
				{
					if (gridPosArray[j].y >= level->building_list[i]->gridPos.y && gridPosArray[j].y < level->building_list[i]->gridPos.y + level->building_list[i]->size.y)
					{
						free(gridPosArray);
						return false;
					}
				}
			}
		}
	}

	free(gridPosArray);
	return true;

}

void level_building_free(Building *building, Level *level)
{
	if (!building)
	{
		slog("no building given");
		return;
	}
	if (!level)
	{
		slog("no level given");
		return;
	}

	if (!building->id)
	{
		slog("no building has no id, cannot free. How was this created?");
		return;
	}

	memset(level->building_list[building->id], 0, sizeof(Building));

	level->building_list[building->id]->_inuse = false;
	level->building_list[building->id] = NULL;
}

void level_test_building(Vector2D buildPos, Level *level)
{
	Building *building;

	building = level_building_new(vector2d((int)(buildPos.x / 32), (int)(buildPos.y / 32)), vector2d(4, 4), level);
	building->sprite = gf2d_sprite_load_image("assets/sprites/buildings/test4x4.png");
}

Ore_Node *level_ore_node_new(Vector2D gridPos, char *type, Level *level)
{
	Ore_Node *node;

	int i;
	if (level->ore_list == NULL)
	{
		slog("level does not have an ore list!");
		return NULL;
	}

	/*
	if (!level_building_check_if_placable(gridPos, vector2d(2, 2), level))
	{
		slog("can't place ore in wall");
		return;
	}
	*/

	for (i = 0; i < level->max_ore; i++)
	{
		if (level->ore_list[i])continue;// someone else is using this one
		level->ore_list[i] = ore_node_new(gridPos, type);
		level->ore_list[i]->_inuse = 1;
		level->ore_list[i]->id = i;

		level->ore_list[i]->interact = level_interact_new(NULL, &level->ore_list[i]->gridPos, 64, 64, level);
		level->ore_list[i]->interact->parent = level->ore_list[i];
		level->ore_list[i]->interact->onInteract = ore_node_interact;


		return level->ore_list[i];
	}

	slog("no ore node slots available");
	return NULL;
}

void level_ore_node_free(Ore_Node *node, Level *level)
{
	if (!node)
	{
		slog("no node given");
		return;
	}
	if (!level)
	{
		slog("no level given");
		return;
	}

	if (!node->id)
	{
		slog("node has no id, cannot free. How was this created?");
		return;
	}


	memset(level->building_list[node->id], 0, sizeof(Building));
	
	level->building_list[node->id]->_inuse = false;
	level->building_list[node->id] = NULL;
}

void level_test_node(Vector2D gridPos, Level *level)
{
	Ore_Node *node;

	node = level_ore_node_new(gridPos, "random", level);
}

void level_layout_ore(int orePerRoom, int orePerLevel, Level *level)
{

	//edit to use ore per room/ore per level


	int placedCount;
	int toPlace;
	Room *room;
	int x, y;
	int count;
	int left, right;
	int randomOre;
	int oreName;

	left = 0; right = 0;

	for (int c = 0; c < level->maxColumns; c++)
	{
		for (int r = 0; r < level->maxColumns; r++)
		{
			toPlace = random_int_range(3, 5);

			placedCount = 0;
			room = level->room_list[c][r];

			count = 0;

			randomOre = random_int_range(0, 4);

			switch (randomOre)
			{
				case 0:
					oreName = "copper";
					break;
				case 1:
					oreName = "iron";
					break;
				case 2:
					oreName = "gold";
					break;
				case 3:
					oreName = "aluminum";
					break;
				case 4:
					oreName = "coal";
					break;
			}

			if (random_int_range(0, 1))
			{
				continue;
			}

			while (placedCount < toPlace && room)
			{
				x = random_int_range(4, room->roomWidth - 6);
				y = random_int_range(6, room->roomHeight - 4);

				//check to see if there are tiles under the ore

				

				if (room->tileArray[x][y] && room->tileArray[x + 1][y])
				{


					if (level_check_for_tiles(vector2d(x + c * room->roomHeight, (y + r * room->roomWidth) - 2), vector2d(2, 2), level))
					{
						level_ore_node_new(vector2d(x + c * room->roomHeight, (y + r * room->roomWidth) - 2), oreName, level);
						placedCount++;
					}

					for (int offset = 2; placedCount < toPlace; offset += 2)
					{
						if (level_check_for_tiles(vector2d(x + offset + c * room->roomHeight, (y + r * room->roomWidth) - 2), vector2d(2, 2), level))
						{
							level_ore_node_new(vector2d(x + offset + c * room->roomHeight, (y + r * room->roomWidth) - 2), oreName, level);
							placedCount++;
						}

						if (placedCount >= toPlace)
						{
							break;
						}

						if (level_check_for_tiles(vector2d(x - offset + c * room->roomHeight, (y + r * room->roomWidth) - 2), vector2d(2, 2), level))
						{
							level_ore_node_new(vector2d(x - offset + c * room->roomHeight, (y + r * room->roomWidth) - 2), oreName, level);
							placedCount++;
						}
					}

					
				}

				count++;

				if (count > 100)
				{
					slog("ore could not be placed randomly");
					break;
				}
			}
		}
	}
}

Interactable *level_interact_new(Vector2D *position, Vector2D *gridPos, int width, int height, Level *level)
{
	Interactable *interact;

	int i;
	if (level->interactable_list == NULL)
	{
		slog("level does not have an interactable list!");
		return NULL;
	}


	for (i = 0; i < level->max_interactables; i++)
	{
		if (level->interactable_list[i])continue;// someone else is using this one
		level->interactable_list[i] = interactable_new(width, height, position, gridPos);

		if (level->interactable_list[i])
		{
			level->interactable_list[i]->_inuse = 1;
			level->interactable_list[i]->id = i;


			return level->interactable_list[i];

		}
		return NULL;
	}

	slog("no interactable slots available");
	return NULL;
}


void level_interact_free(Interactable *interact, Level *level)
{
	int id;
	if (!interact)
	{
		slog("no interact given");
		return;
	}
	if (!level)
	{
		slog("no level given");
		return;
	}

	if (!interact->id)
	{
		slog("node has no id, cannot free. How was this created?");
		return;
	}

	id = interact->id;

	memset(level->building_list[id], 0, sizeof(Building));

	level->building_list[id]->_inuse = false;
	level->building_list[id] = NULL;
}

Bool level_check_interact(Level *level)
{
	int ids[10];
	int idsFound = 0;

	SDL_Rect largest;
	int largestID;
	

	for (int i = 0; i < level->max_interactables; i++)
	{
		if (level->interactable_list[i] && level->interactable_list[i]->playerInside)
		{
			ids[idsFound] = i;
			idsFound++;
		}
		if (idsFound >= 10)
		{
			break;
		}
	}

	if (idsFound == 0)
	{
		return false;
	}

	largest = level->interactable_list[ids[0]]->intersectAmount;
	largestID = ids[0];

	for (int i = 1; i < 10; i++)
	{
		if (ids[i] >= 0 && level->interactable_list[ids[i]]->intersectAmount.w * level->interactable_list[ids[i]]->intersectAmount.h > largest.w * largest.h)
		{
			largest = level->interactable_list[ids[i]]->intersectAmount;
			largestID = ids[i];
		}
	}

	//do interaction?

	level->interactable_list[largestID]->onInteract(level->interactable_list[largestID]->parent);

	return true;

}


//uses gridposes
Bool level_check_for_tiles(Vector2D position, Vector2D size, Level *level)
{
	//position is the top left of the object

	Vector2D roomGridPos;
	Vector2D *gridPosArray;
	int count = 0;

	gridPosArray = gfc_allocate_array(sizeof(Vector2D), size.x * size.y);

	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			gridPosArray[count] = vector2d((int)position.x % 38 + x, (int)position.y % 38 + y);
			count++;
		}
	}

	roomGridPos = vector2d((int)(position.x / 38), (int)(position.y / 38));


	if (position.x < 0 || position.x > level->maxColumns * 38 || position.y < 0 || position.y > level->maxRows * 38)
	{
		//slog("X:%f, Y:%f", position.x, position.y);
		free(gridPosArray);
		return false;
	}

	if (roomGridPos.x < 0 || roomGridPos.x > level->maxColumns - 1 || roomGridPos.y < 0 || roomGridPos.y > level->maxRows - 1)
	{
		free(gridPosArray);
		return false;
	}

	//slog("X:%f, Y:%f", roomGridPos.x, roomGridPos.y);
	Room *room = level->room_list[(int)(roomGridPos.x)][(int)(roomGridPos.y)];

	if (!room)
	{
		free(gridPosArray);
		return false;
	}

	for (int j = 0; j < size.x * size.y; j++)
	{
		if ((int)gridPosArray[j].x > room->roomWidth - 1 || gridPosArray[j].y > room->roomHeight - 1)
		{
			free(gridPosArray);
			return false;
		}

		if (room->tileArray[(int)gridPosArray[j].x][(int)gridPosArray[j].y])
		{
			free(gridPosArray);
			return false;
		}

		if (gridPosArray[j].x < 3 || gridPosArray[j].x > room->roomWidth - 4 || gridPosArray[j].y < 3 || gridPosArray[j].y > room->roomHeight - 4)
		{
			free(gridPosArray);
			return false;
		}
	}

	

	free(gridPosArray);
	return true;

}

Vector2D level_get_start_pos(Level *level)
{
	if (!level)
	{
		slog("No room given");
		return vector2d(0, 0);
	}
	for (int x = 0; x < level->maxColumns; x++)
	{
		for (int y = 0; y < level->maxRows; y++)
		{
			if (level && level->room_list[x][y] && level->room_list[x][y]->roomType == 4)
			{
				//slog("Start room: x:%i, y:%i", x, y);
				return level->room_list[x][y]->position;
			}
		}
	}

	slog("Could not find start room");
	return vector2d(0, 0);
}

void room_slog(Level *level)
{
	for (int x = 0; x < level->maxColumns; x++)
	{
		for (int y = 0; y < level->maxRows; y++)
		{
			if (level && level->room_list[x][y])
			{
				slog("Room: x:%i, y:%i, type:%i", x, y, level->room_list[x][y]->roomType);
			}
		}
	}
}


/*file footer*/