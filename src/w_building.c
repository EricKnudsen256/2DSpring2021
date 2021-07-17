#include "simple_logger.h"
#include "simple_json.h"

#include "g_camera.h"
#include "g_mouse.h"

#include "w_building.h"
#include "w_level.h"

Building_List building_list = { 0 };

void building_list_init()
{
	SJson *json, *json2, *buildingJS, *currentBuildingJS;

	int buildingTypes;
	int x, y;


	json = sj_load("config/buildings.cfg");

	if (!json)return;

	buildingJS = sj_object_get_value(json, "buildings");

	buildingTypes = sj_array_get_count(buildingJS);

	//create item list with size of itemJS

	if (buildingTypes == 0)
	{
		slog("0 buildings in buildings list, something is very fucking wrong");
		return;
	}
	if (building_list.building_list != NULL)
	{
		building_list_free();
	}
	building_list.building_list = (Building_List_Item *)gfc_allocate_array(sizeof (Building_List_Item), buildingTypes);
	if (building_list.building_list == NULL)
	{
		slog("failed to allocate building list!");
		return;
	}
	building_list.total_buildings = buildingTypes;
	slog("building list initialized. Size: %i", buildingTypes);


	for (int i = 0; i < buildingTypes; i++)
	{
		currentBuildingJS = sj_array_get_nth(buildingJS, i);

		building_list.building_list[i] = (Building_List_Item *)malloc(sizeof(Building_List_Item));

		memset(building_list.building_list[i], 0, sizeof(Building_List_Item));

		building_list.building_list[i]->buildingName = sj_get_string_value(sj_object_get_value(currentBuildingJS, "name"));

		if (sj_is_array(sj_object_get_value(currentBuildingJS, "sprite")))
		{
			building_list.building_list[i]->spriteList = gfc_allocate_array(sizeof(Sprite), sj_array_get_count(sj_object_get_value(currentBuildingJS, "sprite")));

			for (int j = 0; j < sj_array_get_count(sj_object_get_value(currentBuildingJS, "sprite")); j++)
			{
				building_list.building_list[i]->spriteList[j] = sj_array_get_nth(sj_object_get_value(currentBuildingJS, "sprite"), j);
			}
		}
		else
		{
			building_list.building_list[i]->sprite = gf2d_sprite_load_image(sj_get_string_value(sj_object_get_value(currentBuildingJS, "sprite")));
		}
		
		building_list.building_list[i]->description = sj_get_string_value(sj_object_get_value(currentBuildingJS, "description"));

		sj_get_integer_value(sj_object_get_value(currentBuildingJS, "sizeX"), &x);
		sj_get_integer_value(sj_object_get_value(currentBuildingJS, "sizeY"), &y);

		building_list.building_list[i]->size = vector2d(x, y);

		//slog("Name %s", itemList.item_list[i].name);

	}

	building_list.currentBuild = malloc(sizeof(Building_List_Item));
	building_list.currentBuild->sprite = NULL;


}

void building_list_set_current_build(const char *buildingName)
{

	//for testing purposes

	if (!buildingName)
	{
		return;
	}


	if (strcmp(buildingName, "testBuilding") == 0)
	{
		memset(building_list.currentBuild, 0, sizeof(Building_List_Item));

		building_list.currentBuild->buildingName = "testBuilding";
		building_list.currentBuild->sprite = gf2d_sprite_load_image("assets/sprites/buildings/testBuilding2x2.png");
		building_list.currentBuild->size = vector2d(2, 2);

		building_list.currentBuild->_inuse = 1;

		return;
	}
	else if (strcmp(buildingName, "testBuilding2") == 0)
	{
		memset(building_list.currentBuild, 0, sizeof(Building_List_Item));

		building_list.currentBuild->buildingName = "testBuilding2";
		building_list.currentBuild->sprite = gf2d_sprite_load_image("assets/sprites/buildings/test4x4.png");
		building_list.currentBuild->size = vector2d(4, 4);

		building_list.currentBuild->_inuse = 1;

		return;
	}
	else if (buildingName == "testBuilding2")
	{
		memset(building_list.currentBuild, 0, sizeof(Building_List_Item));

		building_list.currentBuild->buildingName = "testBuilding2";
		building_list.currentBuild->sprite = gf2d_sprite_load_image("assets/sprites/buildings/test4x4.png");
		building_list.currentBuild->size = vector2d(4, 4);

		building_list.currentBuild->_inuse = 1;

		return;
	}


	for (int i = 0; i < building_list.total_buildings; i++)
	{
		if (strcmp(building_list.building_list[i]->buildingName, buildingName) == 0)
		{
			memset(building_list.currentBuild, 0, sizeof(Building_List_Item));

			building_list.currentBuild->buildingName = building_list.building_list[i]->buildingName;
			building_list.currentBuild->sprite = building_list.building_list[i]->sprite;
			building_list.currentBuild->size = building_list.building_list[i]->size;

			building_list.currentBuild->_inuse = 1;
			return;
		}
	}

	building_list.currentBuild = NULL;
}


void building_list_remove_current_build()
{
	memset(building_list.currentBuild, 0, sizeof(Building_List_Item));
}

void building_list_load_buildings()
{

}

void building_list_update_current()
{
}

void building_list_draw_current()
{
	Vector2D drawPos, offset, mousePos;
	Vector2D drawScale = camera_get_scale();
	Vector4D colorShift;


	offset = camera_get_offset();
	mousePos = mouse_get_world_position();

	if (building_list.currentBuild && building_list.currentBuild->sprite)
	{
		drawPos.x = (int)mousePos.x - ((int)mousePos.x % 32) + offset.x;
		drawPos.y = (int)mousePos.y - ((int)mousePos.y % 32) + offset.y;

		//check to see if spot has building, if so, change color

		if (level_building_check_if_placable(mousePos, building_list.currentBuild->size, level_manager_get_current()))
		{
			colorShift = vector4d(0, 255, 0, 175);
		}
		else
		{
			colorShift = vector4d(255, 0, 0, 175);
		}

		

		gf2d_sprite_draw(building_list.currentBuild->sprite, drawPos, &drawScale, NULL, NULL, NULL, &colorShift, 0);
	}
}


void building_list_place_current()
{
	Vector2D spawnPos, offset, mousePos, gridPos;

	offset = camera_get_offset();
	mousePos = mouse_get_world_position();

	//check to see if placing building in this spot is possible

	if (level_building_check_if_placable(mousePos, building_list.currentBuild->size, level_manager_get_current()))
	{
		level_test_building(mousePos, level_manager_get_current());
	}

	

	//slog("Placed at x:%f, y:%f", spawnPos.x, spawnPos.y);
}

Bool building_list_is_current()
{
	if (building_list.currentBuild->_inuse == 1)
	{
		return true;
	}

	return false;
}

Building_List_Item *building_list_get_by_name(char *buildingName)
{
	for (int i = 0; i < building_list.total_buildings; i++)
	{
		if (strcmp(buildingName, building_list.building_list[i]->buildingName) == 0)
		{
			return building_list.building_list[i];
		}
	}
	return NULL;
}

void building_list_free()
{
	if (building_list.building_list != NULL)
	{
		free(building_list.building_list);
	}
	memset(&building_list, 0, sizeof(Building_List));
	slog("building list closed");
}


Building *building_new(Vector2D gridPos, Vector2D size)
{

	Building *building;
	building = (Building *)malloc(sizeof(Building));
	if (!building)
	{
		slog("failed to allocate memory for new building");
		return NULL;
	}
	memset(building, 0, sizeof(Building));


	//32 is always size of tiles, should maybe be more robust but its fine for now
	building->position.x = gridPos.x * 32;
	building->position.y = gridPos.y * 32;



	building->gridPos.x = gridPos.x;
	building->gridPos.y = gridPos.y;

	building->size.x = size.x;
	building->size.y = size.y;


	building->_inuse = true;

	slog("Building built at x:%f, y:%f", building->gridPos.x, building->gridPos.y);

	return building;
}


void building_free(Building *building)
{
	if (!building)return;// nothing to do

	memset(building, 0, sizeof(Building));

	gf2d_sprite_free(building->sprite);

	building->sprite = NULL;

	building->_inuse = false;
	building = NULL;

}


void building_draw(Building *building)
{
	Vector2D drawPosition, offset;
	Vector2D drawScale = camera_get_scale();

	if (!building)
	{
		slog("cannot draw a NULL tile");
		return;
	}
	if (!building->_inuse)
	{
		return;
	}

	if (building->draw)
	{
		building->draw(building);
	}
	else
	{
		if (building->sprite == NULL)
		{
			return;// nothing to draw
		}
		offset = camera_get_offset();
		if (!camera_rect_on_screen(gfc_sdl_rect((building->position.x), (building->position.y), building->sprite->frame_w, building->sprite->frame_h)))
		{
			//entity is off camera, skip
			//return;
		}
		drawPosition.x = building->position.x + offset.x;
		drawPosition.y = building->position.y + offset.y;



		gf2d_sprite_draw(
			building->sprite,
			drawPosition,
			&drawScale,
			NULL,
			NULL,
			NULL,
			NULL,
			0);
	}

	//test code to draw the hitboxes for an ent that has one

	/*
	if (&tile->hitbox)
	{

	SDL_Rect tempDraw;

	tempDraw.x = tile->hitbox.x + offset.x;
	tempDraw.y = tile->hitbox.y + offset.y;
	tempDraw.w = tile->hitbox.w;
	tempDraw.h = tile->hitbox.h;

	SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 0, 255, 0, 255);

	SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &tempDraw);
	}
	*/
}

Building_List building_list_get()
{
	return building_list;
}