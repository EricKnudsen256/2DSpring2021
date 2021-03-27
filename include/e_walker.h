#ifndef __WALKER_H__
#define __WALKER_H__

#include "e_enemy.h"


typedef struct Walker_s
{
	Entity		*ent;
	void(*update)(struct Walker_s *self);
	void(*think)(struct Walker_s *self);
	void(*draw)(struct Walker_s *self);
	void(*free)(struct Walker_s *self);
	void       *data;

}Walker;

/**
* @brief spawn a walker enemy
* @param position the screen position to spawn walker
* @return NULL on error, or a pointer to a new walker entity
*/
Entity *walker_spawn(Vector2D position);

/**
* @brief function to update values on the walker
* @param self the walker entity
*/
void walker_update(Entity *self);

/**
* @brief function to update the walker, move position, etc
* @param self the walker entity
*/
void walker_think(Entity *self);

/**
* @brief checks every frame to see if enemy is colliding with the player
* @param self the enemy
*/

#endif