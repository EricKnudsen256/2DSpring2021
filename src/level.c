#include <stdlib.h>

#include "simple_json.h"
#include "simple_logger.h"

#include "camera.h"
#include "level.h"
#include "random.h"

typedef struct
{
	Level *level_list;
	Uint32  max_levels;
}LevelManager;

static LevelManager level_manager = { 0 };


void level_manager_init(Uint32 max_levels)
{
	if (max_levels == 0)
	{
		slog("cannot allocate 0 levels!");
		return;
	}
	if (level_manager.level_list != NULL)
	{
		entity_manager_free();
	}
	level_manager.level_list = (Level *)gfc_allocate_array(sizeof (Level), max_levels);
	if (level_manager.level_list == NULL)
	{
		slog("failed to allocate entity list!");
		return;
	}
	level_manager.max_levels = max_levels;
	atexit(level_manager_free);
	slog("level system initialized");
}

void level_manager_free()
{
	if (level_manager.level_list != NULL)
	{
		free(level_manager.level_list);
	}
	memset(&level_manager, 0, sizeof(LevelManager));
	slog("entity system closed");
}


Level *level_new()
{
	int i;
	Level *level;
	level = (Level *)malloc(sizeof(Level));
	if (!level)
	{
		slog("failed to allocate memory for the game level");
		return NULL;
	}
	for (i = 0; i < level_manager.max_levels; i++)
	{
		if (level_manager.level_list[i]._inuse)continue;// someone else is using this one
		memset(&level_manager.level_list[i], 0, sizeof(Level));
		level_manager.level_list[i]._inuse = 1;
		return &level_manager.level_list[i];
	}
	memset(level, 0, sizeof(Level));
	return level;
}

Level *level_load(const char *filename)
{
	const char *string;
	Level *level;
	SJson *json, *levelJS, *levelMap, *row, *array;
	int rows, columns;
	int count, tileindex;
	int i, j;
	int tempInt;

	if (!filename)
	{
		slog("filename is NULL, cannot load the level");
		return NULL;
	}
	json = sj_load(filename);
	if (!json)return NULL;

	level = level_new();
	if (!level)
	{
		sj_free(json);
		return NULL;
	}

	levelJS = sj_object_get_value(json, "level");
	if (!levelJS)
	{
		slog("level json missing level object");
		level_free(level);
		sj_free(json);
		return NULL;
	}

	array = sj_object_get_value(levelJS, "bgImage");
	count = sj_array_get_count(array);
	level->bgImageCount = count;
	if (count)
	{
		level->bgImage = (Sprite **)gfc_allocate_array(sizeof(Sprite*), count);
		for (i = 0; i < count; i++)
		{
			string = sj_get_string_value(sj_array_get_nth(array, i));
			if (string)
			{
				level->bgImage[i] = gf2d_sprite_load_image((char *)string);
			}
		}
	}
	string = sj_get_string_value(sj_object_get_value(levelJS, "tileSet"));
	if (string)
	{
		slog("loading tile set %s", string);
		sj_get_integer_value(sj_object_get_value(levelJS, "tileWidth"), &level->tileWidth);
		sj_get_integer_value(sj_object_get_value(levelJS, "tileHeight"), &level->tileHeight);
		sj_get_integer_value(sj_object_get_value(levelJS, "tileFPL"), &level->tileFPL);
		level->tileSet = gf2d_sprite_load_all(
			(char *)string,
			level->tileWidth,
			level->tileHeight,
			level->tileFPL);
	}
	levelMap = sj_object_get_value(levelJS, "tileMap");
	if (!levelMap)
	{
		slog("missing tileMap data");
		level_free(level);
		sj_free(json);
		return NULL;
	}
	rows = sj_array_get_count(levelMap);
	row = sj_array_get_nth(levelMap, 0);
	columns = sj_array_get_count(row);
	count = rows * columns;
	level->levelWidth = columns;
	level->levelHeight = rows;
	level->tileMap = (TileTypes*)gfc_allocate_array(sizeof(TileTypes), count);
	if (!level->tileMap)
	{
		level_free(level);
		sj_free(json);
		return NULL;
	}
	level->tileCount = count;

	tileindex = 0;
	slog("tilemap data:");
	for (j = 0; j < rows; j++)
	{
		row = sj_array_get_nth(levelMap, j);
		if (!row)continue;// skip it, its bad
		if (columns != sj_array_get_count(row))
		{
			slog("row %i, column count mismatch", j);
			continue;
		}
		for (i = 0; i < columns; i++)
		{
			sj_get_integer_value(sj_array_get_nth(row, i), &tempInt);
			level->tileMap[tileindex] = tempInt;
			printf("%i,", level->tileMap[tileindex++]);
		}
		printf("\n");
	}
	level->levelSize.x = level->levelWidth * level->tileWidth;
	level->levelSize.y = level->levelHeight * level->tileHeight;
	slog("map width: %f, with %i tiles wide, each %i pixels wide", level->levelSize.x, level->levelWidth, level->tileWidth);
	slog("map height: %f, with %i tiles high, each %i pixels tall", level->levelSize.y, level->levelHeight, level->tileHeight);

	sj_free(json);
	return level;
}

Level *level_random(int width, int height)
{
	Vector2D temp;
	const char *string;
	Level *level;
	int count;
	int tileIndex;
	int i;
	int r, c;
	int platNum;
	

	platNum = random_int_range(16, 30);

	level = level_new();
	if (!level)
	{
		slog("level cannot be initialized");
		return NULL;
	}

	level->levelWidth = width;
	level->levelHeight = height;
	count = 1;
	level->bgImageCount = count;

	//testing values with 1, change when paralax required, for loop will need to be changed as well

	if (count)
	{
		level->bgImage = (Sprite **)gfc_allocate_array(sizeof(Sprite*), count);
		for (i = 0; i < count; i++)
		{
			//string = sj_get_string_value(sj_array_get_nth(array, i));
			//if (string)
			//{
				level->bgImage[i] = gf2d_sprite_load_image("images/backgrounds/layers0.png");
			//}
		}
	}

	//Test tileset, add paramter to function for this

	string = "images/basetileset.png";
	slog("loading tile set %s", string);

	level->tileWidth = 32;
	level->tileHeight = 32;
	level->tileFPL= 16;


	level->tileSet = gf2d_sprite_load_all(
		(char *)string,
		level->tileWidth,
		level->tileHeight,
		level->tileFPL);

	//System to initialize random level

	//Initializes tileArray to the size needed if all tiles were used in the level
	level->tileArrayLen = (level->levelWidth * level->levelHeight);
	level->tileArray = (Tile **)gfc_allocate_array(sizeof(Tile*), level->tileArrayLen);


	//loop to initialize the tiles in the level
	for (r = 0; r < level->levelHeight; r++)
	{
		for (c = 0; c < level->levelWidth; c++)
		{

			//test code to make sure that the tiles are being created properly
			if (r == 0 || r == level->levelHeight - 1 || c == 0 || c == level->levelWidth - 1)
			{
				Bool tilePlaced = false;
				//slog("creating tile at x:%i, y:%i", c, r);
				temp.x = c;
				temp.y = r;

				level_new_tile(level, temp);
			}		



			//code for placing platforms

			if (false)//(r % 5 == 0 && c < 30)
			{
				Bool tilePlaced = false;
				slog("creating tile at x:%i, y:%i", c, r);
				temp.x = c;
				temp.y = r;

				//for loop checking if is any space in the tileArray
				for (i = 0; i < level->tileArrayLen; i++)
				{

					if (level->tileArray[i]->_inuse)continue;// someone else is using this one

					memset(&level->tileArray[i], 0, sizeof(Tile));
					level->tileArray[i] = tile_new(level->tileWidth, level->tileHeight, temp);

					tilePlaced = true;

					break;
				}
				if (!tilePlaced)
				{
					slog("no free tiles available");
					level_free(level);
					return NULL;
				}
			}
		} //end of loop 1

	} //end of loop 2

	//open doors

	level->door1.x = 0;
	level->door1.y = level->levelHeight / 2;


	tileIndex = level_find_tile_by_pos(level, level->door1.x, level->door1.y);
	if (tileIndex != -1)
	{
		tile_free(level->tileArray[tileIndex]);
	}


	tileIndex = level_find_tile_by_pos(level, level->door1.x, level->door1.y - 1);
	if (tileIndex != -1)
	{
		tile_free(level->tileArray[tileIndex]);
	}


	tileIndex = level_find_tile_by_pos(level, level->door1.x, level->door1.y - 2);
	if (tileIndex != -1)
	{
		tile_free(level->tileArray[tileIndex]);
	}


	level->door1.x += 1;
	level->door1.y += 1;

	level_new_tile(level, level->door1);

	level->door1.x += 1;

	level_new_tile(level, level->door1);

	//door 2

	level->door2.x = level->levelWidth - 1;
	level->door2.y = level->levelHeight / 2;

	tileIndex = level_find_tile_by_pos(level, level->door2.x, level->door2.y);
	if (tileIndex != -1)
	{
		tile_free(level->tileArray[tileIndex]);
	}


	tileIndex = level_find_tile_by_pos(level, level->door2.x, level->door2.y - 1);
	if (tileIndex != -1)
	{
		tile_free(level->tileArray[tileIndex]);
	}


	tileIndex = level_find_tile_by_pos(level, level->door2.x, level->door2.y - 2);
	if (tileIndex != -1)
	{
		tile_free(level->tileArray[tileIndex]);
	}


	level->door2.x -= 1;
	level->door2.y += 1;

	level_new_tile(level, level->door2);

	level->door2.x -= 1;

	level_new_tile(level, level->door2);


	//call to create random platforms
	create_random_platform(platNum, 4, 10, level);

	level->levelSize.x = level->levelWidth * level->tileWidth;
	level->levelSize.y = level->levelHeight * level->tileHeight;

	level->_inuse = 1;
	level->_current = 1;

	return level;

}

Tile *level_new_tile(Level * level, Vector2D pos)
{
	int i;
	//for loop checking if is any space in the tileArray
	for (i = 0; i < level->tileArrayLen; i++)
	{

		if (level->tileArray[i])continue;// someone else is using this one

		memset(&level->tileArray[i], 0, sizeof(Tile));
		level->tileArray[i] = tile_new(level->tileWidth, level->tileHeight, pos);
		return level->tileArray[i];
	}

	slog("no free tiles available");
	return NULL;

}

Level *level_manager_get_current()
{
	int i;
	for (i = 0; i < level_manager.max_levels; i++)
	{
		if (level_manager.level_list[i]._current)
		{
			return &level_manager.level_list[i];
		}
	}
	slog("no current level found, returning NULL");

	return NULL;
}


void level_update(Level *level)
{
	SDL_Rect camera;
	if (!level)return;
	camera = camera_get_rect();
	//snap camera to the level bounds
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
	camera_set_position(vector2d(camera.x, camera.y));
}

void level_free(Level *level)
{
	int i;
	if (!level)return;// nothing to do

	if (level->tileMap != NULL)
	{
		free(level->tileSet);
		level->tileMap = NULL;
	}
	if (level->bgImageCount)
	{
		for (i = 0; i < level->bgImageCount; i++)
		{
			gf2d_sprite_free(level->bgImage[i]);
		}
		free(level->bgImage);
	}
	gf2d_sprite_free(level->tileSet);

	free(level);
}

void level_draw(Level *level)
{
	SDL_Rect camera;
	Vector2D offset, drawPosition, parallax;
	int i;
	if (!level)
	{
		slog("cannot draw level, NULL pointer provided");
		return;
	}
	// draw the background first
	offset = camera_get_offset();
	if (level->bgImageCount)
	{
		camera = camera_get_rect();
		for (i = 0; i < level->bgImageCount; i++)
		{
			parallax.x = (float)(level->bgImage[i]->frame_w - camera.w) / (level->levelSize.x - camera.w);
			parallax.y = (float)(level->bgImage[i]->frame_h - camera.h) / (level->levelSize.y - camera.h);

			gf2d_sprite_draw_image(level->bgImage[i], vector2d(offset.x * parallax.x, offset.y * parallax.y));
		}

	}
	//then draw the tiles, this needs to be updated for the new tile system

	if (!level->tileArray)
	{
		slog("not tiles loaded for the level, cannot draw it");
		return;
	}
	
	for (i = 0; i < level->tileArrayLen; i++)
	{
		
		if (!level->tileArray[i])continue;

		tile_draw(level->tileArray[i]);
	}
}

int level_find_tile_by_pos(Level *level, int x, int y)
{
	int i;

	if (!level)
	{
		slog("no level given to search");
		return -1;
	}

	for (i = 0; i < level->tileArrayLen; i++)
	{
		//slog("i: %i\ntileArrayLen: %i", i, level->tileArrayLen);

		if (!level->tileArray[i])
		{
			continue;
		}

		if (level->tileArray[i]->gridPos.x == x && level->tileArray[i]->gridPos.y == y)
		{
			return i;
		}
	}
	
	return -1;
}

Bool create_random_platform(int number, int minWidth, int maxWidth, Level *level)
{
	int platLength;
	int i, j, x ,y;
	int tileIndex;
	int cursorIndex;

	Bool foundBlock;

	Vector2D cursor;

	//loop number of times equal to platforms to be created, loop again on failure

	for (i = 0; i < number; i++)
	{
		//slog("loop1");

		foundBlock = false;

		platLength = random_int_range(minWidth, maxWidth);

		x = random_int_range(4 + (minWidth / 2), level->levelWidth - 4 - (minWidth / 2));
		y = random_int_range(4, level->levelHeight);

		tileIndex = level_find_tile_by_pos(level, x, y);

		//slog("platLength:%i", platLength);

		if (tileIndex == -1)
		{
			//check if enought tiles nearby are available to make the platform

			//first loop for left side of block

			cursor = vector2d(x, y);
			for (j = 0; j < platLength / 2; j++)
			{

				//slog("loop2");

				cursor.x--;

				cursorIndex = level_find_tile_by_pos(level, cursor.x, cursor.y);
				{
					if (cursorIndex != -1)
					{
						foundBlock = true;
						break;
					}
				}
			}

			cursor = vector2d(x, y);
			for (j = 0; j < platLength / 2; j++)
			{

				//slog("loop3");
				if (foundBlock)
				{
					break;
				}
				cursor.x++;

				cursorIndex = level_find_tile_by_pos(level, cursor.x, cursor.y);
				{
					if (cursorIndex != -1)
					{
						foundBlock = true;
						break;
					}
				}
			}

			if (!foundBlock)
			{
				spawn_platform(vector2d(x, y), platLength, level);
			}
			else
			{
				i--;
			}
		}
	}

}

Bool spawn_platform(Vector2D gridPos, int width, Level *level)
{
	int i;
	Vector2D cursor, adjustedGridPos;



	vector2d_copy(adjustedGridPos, gridPos);

	adjustedGridPos.y = (int)adjustedGridPos.y - ((int)adjustedGridPos.y % 4);

	vector2d_copy(cursor, adjustedGridPos);

	level_new_tile(level, cursor);

	if (width % 2 == 1)
	{
		vector2d_copy(cursor, adjustedGridPos);
		for (i = 0; i < width / 2 + 1; i++)
		{
			//slog("tile x:%f, y%f", cursor.x, cursor.y);
			cursor.x--;
			level_new_tile(level, cursor);
			
		}
		
		vector2d_copy(cursor, adjustedGridPos);
		for (i = 0; i < width / 2 + 1; i++)
		{
			//slog("tile x:%f, y%f", cursor.x, cursor.y);
			cursor.x++;
			level_new_tile(level, cursor);
			
		}
	}
	else if (width % 2 == 0)
	{
		vector2d_copy(cursor, adjustedGridPos);
		for (i = 0; i < (width / 2); i++)
		{
			//slog("tile x:%f, y%f", cursor.x, cursor.y);
			cursor.x--;
			level_new_tile(level, cursor);
			
		}

		vector2d_copy(cursor, adjustedGridPos);
		for (i = 0; i < width / 2 + 1; i++)
		{
			//slog("tile x:%f, y%f", cursor.x, cursor.y);
			cursor.x++;
			level_new_tile(level, cursor);
			
		}
	}
}


/*file footer*/