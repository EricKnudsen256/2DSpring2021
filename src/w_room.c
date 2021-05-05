#include "simple_logger.h"

#include "g_camera.h"

#include "w_room.h"
#include "w_tile.h"


static RoomManager room_manager = { 0 };

void room_manager_init(int maxRows, int maxColumns, Uint32 max_rooms)
{
	if (max_rooms == 0)
	{
		slog("cannot allocate 0 rooms!");
		return;
	}
	if (room_manager.room_list != NULL)
	{
		room_manager_free();
	}
	room_manager.room_list = (Room *)gfc_allocate_array(sizeof (Room), maxRows);

	for (int i = 0; i < maxColumns; i++)
	{
		room_manager.room_list[i] = (Room *)gfc_allocate_array(sizeof (Room), maxColumns);
	}

	if (room_manager.room_list == NULL)
	{
		slog("failed to allocate room list!");
		return;
	}
	room_manager.maxRows = maxRows;
	room_manager.maxColumns = maxColumns;
	room_manager.max_rooms = max_rooms;
	atexit(room_manager_free);
	slog("room system initialized");
}

void room_manager_update()
{
	int x, y;
	if (room_manager.room_list == NULL)
	{
		slog("entity system does not exist");
		return;
	}
	for (x = 0; x < room_manager.maxColumns; x++)
	{
		for (y = 0; y < room_manager.maxRows; y++)
		{
			if (room_manager.room_list[x][y]._inuse == 0)continue;
			
			room_update(&room_manager.room_list[x][y]);
		}
	}
}

void room_manager_draw()
{
	int x, y;
	if (room_manager.room_list == NULL)
	{
		slog("entity system does not exist");
		return;
	}
	for (x = 0; x < room_manager.maxColumns; x++)
	{
		for (y = 0; y < room_manager.maxRows; y++)
		{
			if (room_manager.room_list[x][y]._inuse == 0)continue;

			room_draw(&room_manager.room_list[x][y]);
		}
	}
}

void room_manager_free()
{
	if (room_manager.room_list != NULL)
	{
		free(room_manager.room_list);
	}
	memset(&room_manager, 0, sizeof(RoomManager));
	slog("room system closed");
}

Room **room_manager_get_room_list()
{
	return room_manager.room_list;
}

int room_manager_get_max_columns()
{
	return room_manager.maxColumns;
}

int room_manager_get_max_rows()
{
	return room_manager.maxRows;
}

Room *room_new(Vector2D gridPos)
{
	if (room_manager.room_list == NULL)
	{
		slog("room system does not exist");
		return NULL;
	}

	int x, y;
	x = gridPos.x;
	y = gridPos.y;

	if (x >= room_manager.maxColumns || y >= room_manager.maxRows)
	{
		slog("Room not in range of grid");
		return NULL;
	}

	if (!room_manager.room_list[x][y]._inuse)
	{
		memset(&room_manager.room_list[x][y], 0, sizeof(Room));
		room_manager.room_list[x][y]._inuse = 1;
		return &room_manager.room_list[x][y];
	}

	slog("room already exists in that spot");
	return NULL;
}

Room *room_load(const char *filename)
{

}

Room *room_empty()
{
	Vector2D temp;
	const char *string;
	Room *room;
	int count;
	int tileIndex;
	int i;
	int r, c;

	temp = vector2d(0, 0);
	room = room_new(temp);
	if (!room)
	{
		return NULL;
	}

	slog("creating room");

	room->roomWidth = 32;
	room->roomHeight = 32;


	//Test tileset, add paramter to function for this


	room->tileWidth = 32;
	room->tileHeight = 32;


	//System to initialize random level

	//Initializes tileArray to the size needed if all tiles were used in the level
	room->tileArrayLen = (room->roomWidth * room->roomHeight);
	room->tileArray = (Tile **)gfc_allocate_array(sizeof(Tile*), room->tileArrayLen);

	room->entityArrayLen = 50;
	room->entityArray = (Entity **)gfc_allocate_array(sizeof(Entity*), room->entityArrayLen);


	//loop to initialize the tiles in the level
	for (r = 0; r < room->roomHeight; r++)
	{
		for (c = 0; c < room->roomWidth; c++)
		{

			//test code to make sure that the tiles are being created properly
			if (r == 0 || r == room->roomHeight - 1 || c == 0 || c == room->roomWidth - 1)
			{
				Bool tilePlaced = false;
				//slog("creating tile at x:%i, y:%i", c, r);
				temp.x = c;
				temp.y = r;

				room_new_tile(room, temp);
			}

		} //end of loop 1

	} //end of loop 2

	//open doors


	room->roomSize.x = room->roomWidth * room->tileWidth;
	room->roomSize.y = room->roomHeight * room->tileHeight;

	room->_inuse = 1;

	room->roomPos = vector2d(0, 0);


	return room;
}

Tile *room_new_tile(Room * room, Vector2D pos)
{
	int i;
	//for loop checking if is any space in the tileArray
	for (i = 0; i < room->tileArrayLen; i++)
	{

		if (room->tileArray[i])continue;// someone else is using this one

		memset(&room->tileArray[i], 0, sizeof(Tile));
		room->tileArray[i] = tile_new(room->tileWidth, room->tileHeight, pos);
		return room->tileArray[i];
	}

	slog("no free tiles available");
	return NULL;
}

Entity *room_new_enemy(Room * room, Vector2D gridPos, int enemy)
{

}

void room_free(Room *room)
{
	int i;
	if (!room)return;// nothing to do


	free(room);
}

void room_update(Room *room)
{
	SDL_Rect camera;
	if (!room)return;
	camera = camera_get_rect();
	//snap camera to the level bounds
	/*
	if ((camera.x + camera.w) > (int)level->levelSize.x)
	{
	camera.x = level->levelSize.x - camera.w;
	}
	if ((camera.y + camera.h) > (int)level->levelSize.y)
	{
	camera.y = level->levelSize.y - camera.h;
	}
	if (camera.x < 0)camera.x = 0;
	if (camera.y < 0)camera.y = 0;
	*/
	camera_set_position(vector2d(camera.x, camera.y));
}

void room_draw(Room *room)
{
	SDL_Rect camera;
	Vector2D offset, drawPosition, parallax;
	int i = 0;
	if (!room)
	{
		slog("cannot draw room, NULL pointer provided");
		return;
	}
	// draw the background first
	offset = camera_get_offset();
	
	//then draw the tiles, this needs to be updated for the new tile system

	if (!room->tileArray)
	{
		//slog("not tiles loaded for the level, cannot draw it");
		return;
	}

	for (i = 0; i < room->tileArrayLen; i++)
	{

		if (!room->tileArray[i])continue;

		tile_draw(room->tileArray[i]);
	}
}

void room_test()
{
	Vector2D pos = vector2d(0, 0);

	Room * testRoom = room_new(pos);
}

