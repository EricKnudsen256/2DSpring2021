#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "simple_json.h"

#include "e_enemy.h"

#include "g_camera.h"
#include "g_random.h"

#include "w_room.h"
#include "w_tile.h"


Room *room_new()
{
	Room *room;
	
	room = malloc(sizeof(Room));

	memset(room, 0, sizeof(Room));

	if (room)
	{
		room->_inuse = 1;
		return room;
	}
	
	slog("room not created properly");
	return NULL;
}


void room_template_save(const char *filename, Room *room)
{
	SJson *json, *jRoom, *toInsert;
	SJson *array1, *array2;

	if (!room)
	{
		return;
	}

	json = sj_object_new();

	jRoom = sj_object_new();

	//Vector2D	roomSize;   
	toInsert = sj_new_int(room->roomSize.x);
	sj_object_insert(jRoom, "roomSizeX", toInsert);

	toInsert = sj_new_int(room->roomSize.y);
	sj_object_insert(jRoom, "roomSizeY", toInsert);

	//Vector2D	roomPos;	
	toInsert = sj_new_int(room->roomPos.x);
	sj_object_insert(jRoom, "roomPosX", toInsert);

	toInsert = sj_new_int(room->roomPos.y);
	sj_object_insert(jRoom, "roomPosY", toInsert);

	//int			roomType;	

	toInsert = sj_new_int(room->roomType);
	sj_object_insert(jRoom, "roomType", toInsert);

	//Uint32      roomWidth;  

	toInsert = sj_new_int(room->roomWidth);
	sj_object_insert(jRoom, "roomWidth", toInsert);

	//Uint32      roomHeight; 

	toInsert = sj_new_int(room->roomHeight);
	sj_object_insert(jRoom, "roomHeight", toInsert);

	//Bool		leftDoor, topDoor, rightDoor, botDoor;	

	toInsert = sj_new_bool(false);
	sj_object_insert(jRoom, "leftDoor", toInsert);

	toInsert = sj_new_bool(false);
	sj_object_insert(jRoom, "topDoor", toInsert);

	toInsert = sj_new_bool(false);
	sj_object_insert(jRoom, "rightDoor", toInsert);

	toInsert = sj_new_bool(false);
	sj_object_insert(jRoom, "botDoor", toInsert);		

	//Vector2D	position;

	toInsert = sj_new_int(room->position.x);
	sj_object_insert(jRoom, "positionX", toInsert);

	toInsert = sj_new_int(room->position.y);
	sj_object_insert(jRoom, "positionY", toInsert);

	//Tile		***tileArray;  

	array1 = sj_array_new();
	array2 = sj_array_new();

	for (int y = 0; y < room->roomWidth; y++)
	{
		array2 = sj_array_new();
		for (int x = 0; x < room->roomHeight; x++)
		{

			if (x < 3 || x > room->roomWidth - 4 || y < 3 || y > room->roomWidth - 4)
			{
				toInsert = sj_new_int(0);
			}
			else if (x == 3 || x == room->roomWidth - 4 || y == 3 || y == room->roomHeight - 4)
			{
				toInsert = sj_new_int(1);
			}
			else if (room->tileArray[x][y])
			{
				toInsert = sj_new_int(1);
			}
			else
			{
				toInsert = sj_new_int(0);
			}

			sj_array_append(array2, toInsert);
		}
		sj_array_append(array1, array2);
	}
	
	sj_object_insert(jRoom, "tileArray", array1);

	//Entity		**entityArray; 


	//int			entityArrayLen;

	toInsert = sj_new_int(room->entityArrayLen);
	sj_object_insert(jRoom, "entityArrayLen", toInsert);

	//Sprite     *tileSet;   not in use right now, no need to save yet  
	//Sprite	   *bgSprite;



	//int         tileWidth;   

	toInsert = sj_new_int(room->tileWidth);
	sj_object_insert(jRoom, "tileWidth", toInsert);

	//int         tileHeight;  

	toInsert = sj_new_int(room->tileHeight);
	sj_object_insert(jRoom, "tileHeight", toInsert);


	sj_object_insert(json, "room", jRoom);
	sj_save(json, filename); //use as last line to make sure this shit actually saves

	//sj_free(json);
	//sj_free(toInsert);
	//sj_free(array1);
	//sj_free(array2);
}

Room *room_template_load(const char *filename, Room *room) //loads template from file
{

	SJson *json, *toInsert, *roomJS, *array1, *array2, *arrayObj;
	Vector2D v2dRtn, tilePos, spawnPos;

	int *out;
	int x, y;
	int hasTile;

	x = 0;
	y = 0;
	out = 0;
	hasTile = 0;

	if (!filename)
	{
		slog("No filename to load found");
		return NULL;
	}

	

	json = sj_load(filename);
	if (!json)return NULL;

	if (!room)
	{
		sj_free(json);
		return NULL;
	}

	roomJS = sj_object_get_value(json, "room");

	if (!roomJS)
	{
		slog("room json missing level object");
		sj_free(json);
		return NULL;
	}




	//Vector2D	roomSize;  


	sj_get_integer_value(sj_object_get_value(roomJS, "roomSizeX"), &x);

	sj_get_integer_value(sj_object_get_value(roomJS, "roomSizeY"), &y);

	room->roomSize = vector2d(x, y);



	//slog("x:%f, y:%f", room->roomSize.x, room->roomSize.y);

	//int			roomType;	

	sj_get_integer_value(sj_object_get_value(roomJS, "roomType"), &room->roomType);

	//Uint32      roomWidth;  

	sj_get_integer_value(sj_object_get_value(roomJS, "roomWidth"), &room->roomWidth);

	//Uint32      roomHeight; 

	sj_get_integer_value(sj_object_get_value(roomJS, "roomHeight"), &room->roomHeight);



	//Bool		leftDoor, topDoor, rightDoor, botDoor;			

	sj_get_bool_value(sj_object_get_value(roomJS, "leftDoor"), &room->leftDoor);
	sj_get_bool_value(sj_object_get_value(roomJS, "topDoor"), &room->topDoor);
	sj_get_bool_value(sj_object_get_value(roomJS, "rightDoor"), &room->rightDoor);
	sj_get_bool_value(sj_object_get_value(roomJS, "botDoor"), &room->botDoor);



	//Vector2D	position;

	sj_get_integer_value(sj_object_get_value(roomJS, "positionX"), &x);
	sj_get_integer_value(sj_object_get_value(roomJS, "positionY"), &y);
	room->position = vector2d(x, y);


	//int         tileWidth;   

	sj_get_integer_value(sj_object_get_value(roomJS, "tileWidth"), &room->tileWidth);

	//int         tileHeight;

	sj_get_integer_value(sj_object_get_value(roomJS, "tileHeight"), &room->tileHeight);

	//Tile		***tileArray;  

	room->tileArray = (Tile *)gfc_allocate_array(sizeof(Tile*), room->roomHeight);

	for (int i = 0; i < room->roomWidth; i++)
	{
		room->tileArray[i] = (Tile *)gfc_allocate_array(sizeof(Tile*), room->roomWidth);
	}


	array1 = sj_object_get_value(roomJS, "tileArray");

	for (y = 0; y < room->roomWidth; y++)
	{
		array2 = sj_array_get_nth(array1, y);
		for (x = 0; x < room->roomHeight; x++)
		{
			arrayObj = sj_array_get_nth(array2, x);
			sj_get_integer_value(arrayObj, &hasTile);

			if (hasTile == 1)
			{
				spawnPos = vector2d(x, y);

				room_new_tile(room, spawnPos, spawnPos);
			}
			
		}
	}


	//Entity		**entityArray; 
	//int			tileArrayLen;
	//int			entityArrayLen;
	//Sprite     *tileSet;     
	//Sprite	   *bgSprite;


	room->_inuse = 1;


	room->bgSprite = gf2d_sprite_load_image("assets/sprites/backgrounds/cave.png");

	return room;
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


Tile *room_new_tile(Room *room, Vector2D pos, Vector2D gridPos)
{
	int x, y;
	
	x = gridPos.x;
	y = gridPos.y;
	
	if (x > room->roomWidth || y > room->roomHeight)
	{
		slog("tile out of range");
		return;
	}

	if (room->tileArray[x][y])
	{
		//tile_free(&room->tileArray[x][y]);
		slog("tile already exists");
		return;
	}

	room->tileArray[x][y] = tile_new(room->tileWidth, room->tileHeight, pos, gridPos);

	if (!room->tileArray[x][y])
	{
		slog("tile not created properly");
	}

	return NULL;
}


void room_free_tile(int x, int y, Room* room)
{
	if (!room->tileArray[x][y])
	{
		slog("no tile to free");
		return;
	}
	if (!room)
	{
		slog("no room to free tiles in");
	}

	memset(room->tileArray[x][y], 0, sizeof(Tile));

	room->tileArray[x][y]->sprite = NULL;

	room->tileArray[x][y]->_inuse = false;
	room->tileArray[x][y] = NULL;
}

void room_free(Room *room)
{
	int i;
	if (!room)return;// nothing to do
	memset(room, 0, sizeof(Room));

	room->_inuse = 0;
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
	Vector2D drawScale = camera_get_scale();
	Sprite *hallFix = gf2d_sprite_load_image("assets/sprites/backgrounds/caveHall.png");
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

		gf2d_sprite_draw(room->bgSprite, drawPosition, &drawScale, NULL, NULL, NULL, NULL, NULL);
	}

	if (room->leftDoor && hallFix)
	{
		drawPosition.x = room->position.x + offset.x;
		drawPosition.y = room->position.y + room->tileHeight * 17 + offset.y;


		gf2d_sprite_draw(hallFix, drawPosition, &drawScale, NULL, NULL, NULL, NULL, NULL);
	}
	if (room->rightDoor && hallFix)
	{
		drawPosition.x = room->position.x + room->tileHeight * 35 + offset.x;
		drawPosition.y = room->position.y + room->tileHeight * 17 + offset.y;


		gf2d_sprite_draw(hallFix, drawPosition, &drawScale, NULL, NULL, NULL, NULL, NULL);
	}
	if (room->topDoor && hallFix)
	{
		drawPosition.x = room->position.x + room->tileHeight * 17 + offset.x;
		drawPosition.y = room->position.y + offset.y;


		gf2d_sprite_draw(hallFix, drawPosition, &drawScale, NULL, NULL, NULL, NULL, NULL);
	}
	if (room->botDoor && hallFix)
	{
		drawPosition.x = room->position.x + room->tileHeight * 17 + offset.x;
		drawPosition.y = room->position.y + room->tileHeight * 35 + offset.y;


		gf2d_sprite_draw(hallFix, drawPosition, &drawScale, NULL, NULL, NULL, NULL, NULL);
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

	if (!room)
	{
		slog("Room is null");
		return;
	}

	//slog("Room x:%f, y:%f, left:%i, right:%i, top:%i, bot:%i", room->roomPos.x, room->roomPos.y, left, right, top, bot);

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
	if (room->botDoor == false)
	{
		room->botDoor = bot;
	}

	if (left)
	{
		int x = 3;
		int y = room->roomHeight / 2 - 1;

		room_free_tile(x, y, room);

		y+=1;

		room_free_tile(x, y, room);

		y-=2;

		room_free_tile(x, y, room);

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

		room_free_tile(x, y, room);

		y += 1;

		room_free_tile(x, y, room);

		y -= 2;

		room_free_tile(x, y, room);

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

		room_free_tile(x, y, room);

		x += 1;

		room_free_tile(x, y, room);

		x -= 2;

		room_free_tile(x, y, room);

		

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

		room_free_tile(x, y, room);

		x += 1;

		room_free_tile(x, y, room);

		x -= 2;

		room_free_tile(x, y, room);


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