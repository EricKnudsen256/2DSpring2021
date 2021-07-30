#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "gf2d_sprite.h"

#include "g_anim.h"

#include "w_interactable.h"

typedef enum
{
	BUILDING_PIPE,
	BUILDING_CHEST

}BuildingType;

typedef struct Building_s
{
	int			id;				//location in building list of level

	BuildingType type;

	SDL_Rect	hitbox;

	char		*buildingName;

	Bool		_inuse;

	Vector2D	gridPos;		//top left of building is 0,0
	Vector2D	position;
	Vector2D	size;			//size measured in x and y as ints. 2, 2 means the builidng is 2 tiles by 2 tiles
	Bool		**gridList;		//boolean 2d array, shows what parts of the building are on the grid, and where other objects can be placed
	Bool		**inputList;	//list of where each input face is for the machine
	Bool		**outputList;	//list of where each output face is for the machine

	Sprite		*sprite;
	
	Interactable *interact;

	Anim		**animList;
	int			maxAnims;

	void		*parent;
	
	void(*update)(struct Building *building);
	void(*think)(struct Building *building);
	void(*draw)(struct Building *building);
	void(*free)(struct Building *building);

}Building;

typedef struct Building_List_Item_s
{
	Bool _inuse;
	Sprite *sprite;
	Sprite **spriteList;	//used for building with multiple sprites, such as the pipe

	char *buildingName;
	char *description;

	Anim		**animList;
	int			**maxAnims;

	int			rotation;
	int			maxRotation;
	
	Vector2D size;

}Building_List_Item;

typedef struct Building_List_s
{
	Building_List_Item **building_list;
	Building_List_Item *currentBuild;
	Uint32  total_buildings;

}Building_List;




void building_list_init();

/**
* @brief free all entities in the system and destroy entity manager
*/

void building_list_load_buildings();

void building_list_set_current_build(const char *buildingName);

void building_list_update_current();

void building_list_draw_current();

void building_list_place_current();

void building_list_rotate_current();

Bool building_list_is_current();

void building_list_remove_current_build();

Building_List_Item *building_list_get_by_name(char * buildingName);

void building_list_free();

Building_List building_list_get();


Building *building_new(Vector2D gridPos, Vector2D size);

/**
* @brief frees a tile from memeory
* @params the tile to be freed
*/
void building_free(Building *building);

/**
* @brief draws a single tile
* @params the tile to be drawn
*/
void building_draw(Building *building);



#endif