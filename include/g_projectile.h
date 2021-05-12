#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "e_entity.h"

typedef struct Projectile_s
{
	Bool		_inuse;		/** flag if the slot in the projectile manager is in use */
	Entity		*owner;		/** entity that created the projectile */
	Vector2D	position;
	Vector2D	velocity;
	SDL_Rect	hitbox;
	Sprite		*sprite;
	int			damage;
	int			ttl;			/** time to live flag, if it hits 0, free the projectile */
	int			spawnedTime;	/** when the projectile was spawned */

	float       frame;
	float       frameRate;
	int         frameCount;

	void(*update)(struct Projectile_s *self);
	void(*think)(struct Projectile_s *self);
	void(*draw)(struct Projectile_s *self);
	void(*free)(struct Projectile_s *self);
	void(*check_collision)(struct Projectile_s *self);
	void       *data;


}Projectile;


typedef struct
{
	Projectile *projectile_list;
	Uint32  max_projectiles;
}ProjectileManager;

static ProjectileManager projectile_manager = { 0 };

/**
* @brief creates a new projectile
* @param damage how much damage the projectile deals on contact
* @param ttl how long the projectile should last in ms
* @param owner the entity that created the projectile
* @param position where to spawn the projectile
* @param velocity speed of the projectile
* @return pointer to the projectile
*/
Projectile *projectile_spawn(int damage, int ttl, Entity *owner, Vector2D position, Vector2D velocity);


/**
* @brief makes new projectile instance in the projectile manager
* @return pointer to the projectile
*/
Projectile *projectile_new();


/**
* @brief creates the projectile manager
* @param max_projectiles max projectiles to support in the manager
*/
void projectile_manager_init(Uint32 max_projectiles);


/**
* @brief frees the projectile manager
*/
void projectile_manager_free();

/**
* @brief calls think function on all projectiles
*/
void projectile_manager_think_projectiles();

/**
* @brief calls update function on all projectiles
*/
void projectile_manager_update_projectiles();

/**
* @brief checks all projectiles for collisions
*/
void projectile_manager_check_collisions();

/**
* @brief draws all projectiles
*/
void projectile_manager_draw_projectiles();

/**
* @brief updates the values on a projectile
* @param self the projectile to update
*/
void projectile_update(Projectile *self);

/**
* @brief checks a projectile for any collisions
* @param proj the projectile to check
*/
void projectile_check_collisions(Projectile *proj);

/**
* @brief frees a projectile
* @param proj the projectile to free
*/
void projectile_free(Projectile *proj);

/**
* @brief draws a projectile
* @param proj the projectile to draw
*/
void projectile_draw(Projectile *proj);

#endif