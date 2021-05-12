#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gfc_audio.h"
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
	ENEMY_TYPE	enemy_type;								/** enum type for the enemy, defined above */
	Bool        _inuse;									/** flag for if the entity is currently being used */
	Bool		onGround, onLeft, onRight, onRoof;		/** flags for if the entity has collided with a certain side */
	Bool		isPlayer;								/** flag for player enetity */
	Bool		doubleJumped;			/** flag for if the player has double jumped */
	Bool		hasTarget;				/** not in use, was for pathfinding? */
	int			health;					/** current health of entity. if 0, call entity_die */
	int			startingHealth;			/** health the entity starts with for health bars */
	int			contactDamage;			/** damage to deal on contact with player */
	int			attackDamage;			/** damage to deal on enemy specific attack */
	int			lastAttack;				/** ms since last attack */
	int			lastDamaged;			/** ms since last damaged */
	int			facing;					/** 1 = left, 2 = up, 3 = right, 4 = down */
	int			targetingFrames;		/** I have no fucking idea what this is for */
	Vector2D    position;				
	Vector2D    velocity;
	Vector2D	gridPos;				/** girdPos that this entity was spawned at, will be used in rooms */
	SDL_Rect	hitbox;					/** hurtbox for entity */
	SDL_Rect	hitbox2;				/**used for secondary purposes. For flyer, box to check distance from the ground. For archer, used for player aim box */
	Vector3D    rotation;				/**(x,y) = rotation center, z = degrees of rotation, not currently in use */
	Sprite     *sprite;
	float       frame;					/** current animation frame */
	float       frameRate;				/** frames per second for animations */
	int         frameCount;				/** total frames */
	Uint32		lastJump;				/** used for player to determine if can double jump again */
	int			max_sounds;				/** maximum number of sounds in the entities' sound list */
	Sound		**soundList;
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


/**
* @brief returns current entitiy manager
* @return the entity manager
*/
EntityManager entity_manager_get_manager();


/**
* @brief returns max entities for the entity list
* @return max ents
*/
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