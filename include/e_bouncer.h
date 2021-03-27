#ifndef __BOUNCER_H__
#define __BOUNCER_H__

#include "e_enemy.h"


typedef struct Bouncer_s
{
	Entity		*ent;
	void(*update)(struct Bouncer_s *self);
	void(*think)(struct Bouncer_s *self);
	void(*draw)(struct Bouncer_s *self);
	void(*free)(struct Bouncer_s *self);
	void       *data;

}Bouncer;

/**
* @brief spawn a bouncer entity
* @param position the screen position to spawn the bouncer at
* @return NULL on error, or a pointer to a new bouncer entity
*/
Entity *bouncer_spawn(Vector2D position);

/**
* @brief function to update values on the bouncer
* @param self the bouncer entity
*/
void bouncer_update(Entity *self);

/**
* @brief function to update the bouncer, move position, etc
* @param self the bouncer entity
*/
void bouncer_think(Entity *self);

/**
* @brief checks every frame to see if enemy is colliding with the player or walls
* @param self the enemy
*/
Bool bouncer_check_player_collision(Entity *self);



#endif