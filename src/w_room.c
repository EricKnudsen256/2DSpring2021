#include "simple_logger.h"

#include "g_camera.h"
#include "g_random.h"

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


Room *room_manager_get_room(int x, int y)
{
	if (&room_manager.room_list[x][y]._inuse)
	{
		return &room_manager.room_list[x][y];
	}
	return NULL;
	
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
		slog("Room not in range of grid x:%i, y:%i", x, y);
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

Room *room_empty(Vector2D gridPos)
{
	Vector2D temp;
	Vector2D tilePos;
	const char *string;
	Room *room;
	int count;
	int tileIndex;
	int i;
	int r, c;


	room = room_new(gridPos);
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

void room_init_all()
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
	start = room_empty(spawnPos);
	start->roomType = 4;


	spawnPos.x++;

	room = room_empty(spawnPos);
	room->roomType = 3;
	room_open_door(true, false, false, false, room);

	lastRoom = room;


	while (!endPlaced && count < 200) //worry about room types later, get the gen working properly first
	{
		roomTypeToApply = 1;
		lastRoomTypeToApply = -1;

		rnd = random_int_range(0, 5);

		if (rnd == 0 || rnd == 1)
		{

			roomTypeToApply = 1;

			spawnPos.y--;

			if (spawnPos.y < 0)
			{
				spawnPos.y++;
				spawnPos.x++;

				roomTypeToApply = 3;

				room_manager.room_list[x][y].roomType = 2;

			}

			if (spawnPos.x > room_manager.maxColumns - 1)
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

			if (spawnPos.y > room_manager.maxRows - 1)
			{
				spawnPos.y--;
				spawnPos.x++;

				roomTypeToApply = 3;

				room_manager.room_list[x][y].roomType = 2;

			}

			if (spawnPos.x > room_manager.maxColumns - 1)
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



			if (spawnPos.x > room_manager.maxColumns - 1)
			{
				spawnPos.x--;
				//change room type to exit
				endPlaced = true;
				lastRoomTypeToApply = 5;
			}
			else
			{
				roomTypeToApply = 3;

				room_manager.room_list[x][y].roomType = 2;

			}

			x = spawnPos.x;
			y = spawnPos.y;

		}
		else if (rnd == 5)
		{
			continue;
		}


		x = spawnPos.x;
		y = spawnPos.y;

		if (room_manager.room_list[x][y]._inuse)
		{
			if (lastRoomTypeToApply == 5)
			{
				room_manager.room_list[x][y].roomType = lastRoomTypeToApply;
			}
			if (x == 7)
			{
				lastRoom->roomType = 5;
				endPlaced = true;
			}
			continue;
		}

		room = room_empty(spawnPos);


		slog("Roomtype = %i", roomTypeToApply);

		room->roomType = roomTypeToApply;

		if (lastRoom)
		{
			if (lastRoom->roomType != 4 && lastRoomTypeToApply != -1)
			{
				lastRoom->roomType = lastRoomTypeToApply;
			}
		}

		lastRoom = &room_manager.room_list[x][y];
		count++;

	}

	if (count > 200)
	{
		slog("count reached max, room not fully completed");
		return;
	}

	//code to open all doors

	for (x = 0; x < room_manager.maxColumns; x++)
	{
		for (y = 0; y < room_manager.maxRows; y++)
		{
			Room *room, *lastRoom;
			Bool left, right, top, bot;

			left = false;
			right = false;
			bot = false;
			top = false;

			room = &room_manager.room_list[x][y];

			if ((room->roomType == 2) && x != 0)
			{
				lastRoom = &room_manager.room_list[x - 1][y];

				if (lastRoom->roomType == 2 || lastRoom->roomType == 5)
				{
					left = true;
				}
				
			}

			if ((room->roomType == 3) && x != 0)
			{
				left = true;
			}

			if ((room->roomType == 2 || room->roomType == 4) && x != room_manager.maxColumns - 1)
			{
				right = true;
			}

			if ((room->roomType == 1 || room->roomType == 2 || room->roomType == 3) && y != 0)
			{
				top = true;
			}

			if ((room->roomType == 1 || room->roomType == 2 || room->roomType == 3) && y != room_manager.maxRows - 1)
			{
				bot = true;
			}

			if (room->roomType == 5)
			{
				lastRoom = room;
			}

			if (x == 1)
			{
				if (&room_manager.room_list[x - 1][y].roomType == 4)
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

	if (room_manager.room_list[x][y + 1]._inuse)
	{
		if (room_manager.room_list[x][y + 1].topDoor)
		{
			room_open_door(false, false, false, true, lastRoom);
		}
	}

	if (room_manager.room_list[x][y - 1]._inuse)
	{
		if (room_manager.room_list[x][y - 1].botDoor)
		{
			room_open_door(false, true, false, false, lastRoom);
		}
	}

	if (room_manager.room_list[x - 1][y]._inuse)
	{
		if (room_manager.room_list[x - 1][y].rightDoor)
		{
			room_open_door(true, false, false, false, lastRoom);
		}
	}

	room_build_branches();

}

void room_build_branches()
{
	int x, y;
	Room *room1, *room2;
	Vector2D roomPos;


	for (x = 0; x < room_manager.maxColumns; x++)
	{
		for (y = 0; y < room_manager.maxRows; y++)
		{

			if (x != 0) //check to make sure not on left wall
			{
				
				room1 = &room_manager.room_list[x][y];

				if (room1->leftDoor && !room_manager.room_list[x - 1][y]._inuse) //if has left door, but no room there
				{

					roomPos = vector2d(x - 1, y);
					room2 = room_empty(roomPos);

					room2->roomType = 0;

					room_open_door(false, false, true, false, room2);

					//room_build_branch_room(room2);
				}
			}


			if (y != 0) //check to make sure not on top wall
			{
				room1 = &room_manager.room_list[x][y];

				if (room1->topDoor && !room_manager.room_list[x][y - 1]._inuse) //if has top door, but no room there
				{

					roomPos = vector2d(x, y - 1);
					room2 = room_empty(roomPos);

					room2->roomType = 0;

					room_open_door(false, false, false, true, room2);

					//room_build_branch_room(room2);
				}
			}

			if (x != room_manager.maxColumns - 1) //check to make sure not on right wall
			{
				room1 = &room_manager.room_list[x][y];

				if (room1->rightDoor && !room_manager.room_list[x + 1][y]._inuse) //if has right door, but no room there
				{

					roomPos = vector2d(x + 1, y);
					room2 = room_empty(roomPos);

					room2->roomType = 0;

					room_open_door(true, false, false, false, room2);

					//room_build_branch_room(room2);
				}
			}

			if (y != room_manager.maxRows - 1) //check to make sure not on bot wall
			{
				slog("");
				room1 = &room_manager.room_list[x][y];

				if (room1->botDoor && !room_manager.room_list[x][y + 1]._inuse) //if has bot door, but no room there
				{

					roomPos = vector2d(x, y + 1);
					room2 = room_empty(roomPos);

					room2->roomType = 0;

					room_open_door(false, true, false, false, room2);

					//room_build_branch_room(room2);
				}
			}
			
		}
	}
}

void room_build_branch_room(Room *startRoom)
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

			if (x >= 0)
			{
				if (!room_manager.room_list[x - 1][y]._inuse)
				{
					built = true;
					newRoomPos = vector2d(x - 1, y);

					room = room_empty(newRoomPos);
					room->roomType = 0;

					room_open_door(false, false, true, false, room);
					room_open_door(true, false, false, false, startRoom);

					room_build_branch_room(room);
				}
			}	
		}

		if (rnd == 1) //top
		{
			triedTop = true;
			if (y >= 0)
			{
				if (!room_manager.room_list[x][y - 1]._inuse)
				{
					built = true;
					newRoomPos = vector2d(x, y - 1);

					room = room_empty(newRoomPos);
					room->roomType = 0;

					room_open_door(false, false, false, true, room);
					room_open_door(false, true, false, false, startRoom);

					room_build_branch_room(room);
				}
			}
		}

		if (rnd == 2) //right
		{
			triedRight = true;
			if (x <= room_manager.maxColumns - 1)
			{
				if (!room_manager.room_list[x + 1][y]._inuse)
				{
					built = true;
					newRoomPos = vector2d(x + 1, y);

					room = room_empty(newRoomPos);
					room->roomType = 0;

					room_open_door(true, false, false, false, room);
					room_open_door(false, false, true, false, startRoom);

					room_build_branch_room(room);
				}
			}
		}

		if (rnd == 3) //bot
		{
			triedBot = true;
			if (y <= room_manager.maxRows - 1)
			{
				if (!room_manager.room_list[x][y + 1]._inuse)
				{
					built = true;
					newRoomPos = vector2d(x, y + 1);

					room = room_empty(newRoomPos);
					room->roomType = 0;

					room_open_door(false, true, false, false, room);
					room_open_door(false, false, false, true, startRoom);

					room_build_branch_room(room);
				}
			}
		}
	}
	
}



Tile *room_new_tile(Room *room, Vector2D pos, Vector2D gridPos)
{
	int x, y;
	
	x = gridPos.x;
	y = gridPos.y;
	
	if (room->tileArray[x][y])
	{
		tile_free(&room->tileArray[x][y]);
	}

	room->tileArray[x][y] = tile_new(room->tileWidth, room->tileHeight, pos, gridPos);

	if (!room->tileArray[x][y])
	{
		slog("tile not created properly");
	}

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
	//camera_set_position(vector2d(camera.x, camera.y));
}

void room_draw(Room *room)
{
	SDL_Rect camera;
	Vector2D offset, drawPosition, parallax;
	int x, y;
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

	if (room->bgSprite)
	{
		drawPosition.x = room->position.x + room->tileWidth * 3 + offset.x;
		drawPosition.y = room->position.y + room->tileHeight * 3 + offset.y;

		gf2d_sprite_draw(room->bgSprite, drawPosition, NULL, NULL, NULL, NULL, NULL, NULL);
	}

	for (int x = 0; x < room->roomHeight; x++)
	{
		for (int y = 0; y < room->roomWidth; y++)
		{

			if (!room->tileArray[x][y])continue;

			tile_draw(room->tileArray[x][y]);
		}
	}
}


void room_open_door(Bool left, Bool top, Bool right, Bool bot, Room *room)
{
	Vector2D position, gridPos;

	if (room->leftDoor == false)
	{
		room->leftDoor = left;
	}
	if (room->rightDoor == false)
	{
		room->rightDoor = right;
	}
	if (room->topDoor == false)
	{
		room->topDoor = top;
	}
	if (room->leftDoor == false)
	{
		room->botDoor = bot;
	}

	if (left)
	{
		int x = 3;
		int y = room->roomHeight / 2 - 1;

		tile_free(room->tileArray[x][y]);

		y+=1;

		tile_free(room->tileArray[x][y]);

		y-=2;

		tile_free(room->tileArray[x][y]);

		//build hall

		x = 3;
		y = room->roomHeight / 2 - 1;

		y-=2;

		for (int i = 0; i < 3; i++)
		{
			x--;
			gridPos = vector2d(x, y);

			position = vector2d((room->tileWidth * room->roomWidth * room->roomPos.x) + x * room->tileWidth,
				(room->tileHeight * room->roomHeight * room->roomPos.y) + y * room->tileHeight);

			room_new_tile(room, position, gridPos);
		}

		x = 3;
		y = room->roomHeight / 2 - 1;

		y += 2;

		for (int i = 0; i < 3; i++)
		{
			x--;
			gridPos = vector2d(x, y);

			position = vector2d((room->tileWidth * room->roomWidth * room->roomPos.x) + x * room->tileWidth,
				(room->tileHeight * room->roomHeight * room->roomPos.y) + y * room->tileHeight);

			room_new_tile(room, position, gridPos);
		}
		
	}

	if (right)
	{
		int x = room->roomWidth - 4;
		int y = room->roomHeight / 2 - 1;

		tile_free(room->tileArray[x][y]);

		y += 1;

		tile_free(room->tileArray[x][y]);

		y -= 2;

		tile_free(room->tileArray[x][y]);

		//build hall

		x = room->roomWidth - 4;
		y = room->roomHeight / 2 - 1;

		y -= 2;

		for (int i = 0; i < 3; i++)
		{
			x++;
			gridPos = vector2d(x, y);

			position = vector2d((room->tileWidth * room->roomWidth * room->roomPos.x) + x * room->tileWidth,
				(room->tileHeight * room->roomHeight * room->roomPos.y) + y * room->tileHeight);

			room_new_tile(room, position, gridPos);
		}

		x = room->roomWidth - 4;
		y = room->roomHeight / 2 - 1;

		y += 2;

		for (int i = 0; i < 3; i++)
		{
			x++;
			gridPos = vector2d(x, y);

			position = vector2d((room->tileWidth * room->roomWidth * room->roomPos.x) + x * room->tileWidth,
				(room->tileHeight * room->roomHeight * room->roomPos.y) + y * room->tileHeight);

			room_new_tile(room, position, gridPos);
		}

	}

	if (top)
	{
		int x = room->roomWidth / 2 - 1;
		int y = 3;

		tile_free(room->tileArray[x][y]);

		x += 1;

		tile_free(room->tileArray[x][y]);

		x -= 2;

		tile_free(room->tileArray[x][y]);

		//build hall

		x = room->roomWidth / 2 - 1;
		y = 3;

		x -= 2;

		for (int i = 0; i < 3; i++)
		{
			y--;
			gridPos = vector2d(x, y);

			position = vector2d((room->tileWidth * room->roomWidth * room->roomPos.x) + x * room->tileWidth,
				(room->tileHeight * room->roomHeight * room->roomPos.y) + y * room->tileHeight);

			room_new_tile(room, position, gridPos);
		}

		x = room->roomWidth / 2 - 1;
		y = 3;

		x += 2;

		for (int i = 0; i < 3; i++)
		{
			y--;
			gridPos = vector2d(x, y);

			position = vector2d((room->tileWidth * room->roomWidth * room->roomPos.x) + x * room->tileWidth,
				(room->tileHeight * room->roomHeight * room->roomPos.y) + y * room->tileHeight);

			room_new_tile(room, position, gridPos);
		}
	}

	if (bot)
	{
		int x = room->roomWidth / 2 - 1;
		int y = room->roomHeight - 4;

		tile_free(room->tileArray[x][y]);

		x += 1;

		tile_free(room->tileArray[x][y]);

		x -= 2;

		tile_free(room->tileArray[x][y]);

		//build hall

		x = room->roomWidth / 2 - 1;
		y = room->roomHeight - 4;

		x -= 2;

		for (int i = 0; i < 3; i++)
		{
			y++;
			gridPos = vector2d(x, y);

			position = vector2d((room->tileWidth * room->roomWidth * room->roomPos.x) + x * room->tileWidth,
				(room->tileHeight * room->roomHeight * room->roomPos.y) + y * room->tileHeight);

			room_new_tile(room, position, gridPos);
		}

		x = room->roomWidth / 2 - 1;
		y = room->roomHeight - 4;

		x += 2;

		for (int i = 0; i < 3; i++)
		{
			y++;
			gridPos = vector2d(x, y);

			position = vector2d((room->tileWidth * room->roomWidth * room->roomPos.x) + x * room->tileWidth,
				(room->tileHeight * room->roomHeight * room->roomPos.y) + y * room->tileHeight);

			room_new_tile(room, position, gridPos);
		}
	}
}

Vector2D room_manager_get_start_pos()
{
	for (int x = 0; x < room_manager.maxColumns; x++)
	{
		for (int y = 0; y < room_manager.maxRows; y++)
		{
			if (room_manager.room_list[x][y].roomType == 4)
			{
				slog("Start room: x:%i, y:%i", x, y);
				return room_manager.room_list[x][y].position;
			}
		}
	}
}

void room_slog()
{
	for (int x = 0; x < room_manager.maxColumns; x++)
	{
		for (int y = 0; y < room_manager.maxRows; y++)
		{
			if (room_manager.room_list[x][y]._inuse)
			{
				slog("Room: x:%i, y:%i, type:%i", x, y, room_manager.room_list[x][y].roomType);
			}
			
		}
	}
}

