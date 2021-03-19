#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

typedef enum 
{
	ENEMY_WALKER,
	ENEMY_FLYER,
	ENEMY_ARCHER,
	ENEMY_BOUNCER,
	ENEMY_MINIBOSS1

}ENEMY_TYPE;

typedef struct Entity_s
{
	ENEMY_TYPE	enemy_type;		//enum of what type an enemy is
	Bool        _inuse;
	Bool		onGround, onLeft, onRight, onRoof;
	Bool		isPlayer;
	Bool		doubleJumped;
	int			health;
	int			contactDamage;
	int			attackDamage;
	int			lastAttack;		//measured in ms
	int			lastDamaged;
	int			facing;			//1 = left, 2 = up, 3 = right, 4 = down
	Vector2D    position;
	Vector2D    velocity;
	SDL_Rect	hitbox;	
	SDL_Rect	hitbox2;		//used for secondary purposes. For flyer, box to check distance from the ground. For archer, used for player aim box
	Vector3D    rotation;		//(x,y) = rotation center, z = degrees of rotation
	Sprite     *sprite;
	float       frame;
	float       frameRate;
	int         frameCount;
	Uint32		lastJump;
	void(*update)(struct Entity_s *self);
	void(*think)(struct Entity_s *self);
	void(*draw)(struct Entity_s *self);
	void(*free)(struct Entity_s *self);
	void(*check_collision)(struct Entity_s *self);
	void       *data;
}Entity;

typedef struct
{
	Entity *entity_list;
	Uint32  max_entities;
}EntityManager;

static EntityManager entity_manager = { 0 };

/**
* @brief initialize internal entity management system
* @param max_entities how many concurrent entities to support
*/
void entity_manager_init(Uint32 max_entities);

/**
* @brief calls update function on all entities
*/
void entity_manager_update_entities();

/**
* @brief call think function for all entities
*/
void entity_manager_think_entities();

/**
* @brief checks collions on all entities in the entity manager
* @param ent the entity to check for collisions
*/

void entity_manager_check_collions();


/**
* @brief call draw on all entities
*/
void entity_manager_draw_entities();


/**
* @brief free all entities in the system and destroy entity manager
*/
void entity_manager_free();


/**
* @brief returns the current player if there is one
* @return the player entity if found, NULL otherwise
*/
Entity *entity_manager_get_player_ent();

EntityManager entity_manager_get_manager();

int entity_manager_get_max_ents();


/**
* @brief allocate an entity, initialize it to zero and return a pointer to it
* @return NULL on error (see logs) or a pointer to an initialized entity.
*/
Entity *entity_new();

/**
* @brief checks collions on entities against all tiles, adjusts 
* @param ent the entity to check for collisions
*/

void entity_check_collisions(Entity *ent);

/**
* @brief frees provided entity
* @param ent the entity to free
*/

void entity_free(Entity *ent);

/**
* @brief draws an entity to the current render frame
* @param ent the entity to draw
*/
void entity_draw(Entity *ent);

/**
* @brief checks to see if left side of rect 1 is intersecting right side of rect 2
* @param ent the hitbox of the entity, tile the hitbox of the tile
* @return a true if there is a left side collision, false otherwise
*/
Bool entity_check_left_collision(SDL_Rect ent, SDL_Rect tile);

/**
* @brief checks to see if right side of rect 1 is intersecting left side of rect 2
* @param ent the hitbox of the entity, tile the hitbox of the tile
* @return a true if there is a right side collision, false otherwise
*/
Bool entity_check_right_collision(SDL_Rect ent, SDL_Rect tile);

/**
* @brief checks to see if bottom side of rect 1 is intersecting top side of rect 2
* @param ent the hitbox of the entity, tile the hitbox of the tile
* @return a true if there is a bottom side collision, false otherwise
*/
Bool entity_check_below_collision(SDL_Rect ent, SDL_Rect tile);

/**
* @brief checks to see if top side of rect 1 is intersecting bottom side of rect 2
* @param ent the hitbox of the entity, tile the hitbox of the tile
* @return a true if there is a bottom side collision, false otherwise
*/
Bool entity_check_above_collision(SDL_Rect ent, SDL_Rect tile);
#endif