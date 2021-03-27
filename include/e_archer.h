#ifndef __ARCHER_H__
#define __ARCHER_H__

#include "e_enemy.h"


typedef struct Archer_s
{
	Entity		*ent;
	void(*update)(struct Archer_s *self);
	void(*think)(struct Archer_s *self);
	void(*draw)(struct Archer_s *self);
	void(*free)(struct Archer_s *self);
	void       *data;

}Archer;

/**
* @brief spawn a archer entity
* @param position the screen position to spawn the archer at
* @return NULL on error, or a pointer to a new archer entity
*/
Entity *archer_spawn(Vector2D position);

/**
* @brief function to update values on the archer
* @param self the bouncer entity
*/
void archer_update(Entity *self);

/**
* @brief function to update the archer, move position, etc
* @param self the archer entity
*/
void archer_think(Entity *self);

/**
* @brief called every frame, checks to see if there is the player is in front of the archer
* @param self the archer entity
*/
void archer_aim(Entity *self);

#endif