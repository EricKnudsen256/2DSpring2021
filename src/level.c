#include <stdlib.h>

#include "simple_json.h"
#include "simple_logger.h"

#include "camera.h"
#include "level.h"

typedef struct
{
	Level *level_list;
	Uint32  max_levels;
}LevelManager;

static LevelManager level_list = { 0 };

Level *level_new()
{
	Level *level;
	level = (Level *)malloc(sizeof(Level));
	if (!level)
	{
		slog("failed to allocate memory for the game level");
		return NULL;
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
	int i;
	int r, c;

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

	

	for (r = 0; r < level->levelWidth; r++)
	{
		for (c = 0; c < level->levelWidth; c++)
		{
			//test code to make sure that the tiles are being created properly
			if (r == 0 || r == level->levelWidth - 1 || c == 0 || c == level->levelHeight - 1)
			{
				Bool tilePlaced = false;
				slog("creating tile at x:%i, y:%i", r, c);
				temp.x = r;
				temp.y = c;

				//for loop checking if is any space in the tileArray
				for (i = 0; i < level->tileArrayLen; i++)
				{
					
					if (level->tileArray[i])continue;// someone else is using this one
					
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
		}
	}


	level->levelSize.x = level->levelWidth * level->tileWidth;
	level->levelSize.y = level->levelHeight * level->tileHeight;
	
	return level;

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
		/*
		drawPosition.x = (level->tileArray[i]->gridPos.x * 32);
		drawPosition.y = (level->tileArray[i]->gridPos.y * 32);
		if (!camera_rect_on_screen(gfc_sdl_rect(drawPosition.x, drawPosition.y, level->tileArray[i]->sprite->frame_w, level->tileArray[i]->sprite->frame_h)))
		{
			//tile is off camera, skip
			slog("tile offscreen");
			continue;
		}
		drawPosition.x += offset.x;
		drawPosition.y += offset.y;
		gf2d_sprite_draw(
			level->tileArray[i]->sprite,
			drawPosition,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);
		slog("tile drawn at x:%i, y:%i", level->tileArray[i]->gridPos.x, level->tileArray[i]->gridPos.y);
		*/
	}
}




/*file footer*/