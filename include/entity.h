#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

typedef struct 
{
	Bool _inuse;
	Vector2D position;
	Sprite *sprite;
	float frame;
}Entity;

/**
* @brief initialize internal entity management system
* @param max_entities how many concurent entities to support
*/
void entity_manager_init(Uint32 max_entities);

/**
* @brief free all entities in the system and destroy entity manager
*/
void entity_manager_free();

/**
* @brief allocate an entity, initialize it to zero and return a pointer to it
* @return NULL on error (see logs) or a pointer to an initialized entity.
*/
Entity *entity_new();

/**
* @brief frees provided entity
* @param ent the entity to free
*/
void entity_free(Entity *ent);

/**
* @brief draws provided entity
* @param ent the entity to draw
*/
void entitty_draw(Entity *ent);

#endif

