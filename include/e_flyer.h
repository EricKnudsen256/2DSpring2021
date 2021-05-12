#ifndef __FLYER_H__
#define __FLYER_H__

#include "e_enemy.h"

/**
* @brief struct currently not in use
*/
typedef struct Flyer_s
{
	Entity		*ent;
	void(*update)(struct Flyer_s *self);
	void(*think)(struct Flyer_s *self);
	void(*draw)(struct Flyer_s *self);
	void(*free)(struct Flyer_s *self);
	void       *data;

}Flyer;

/**
* @brief spawn a flyer entity
* @param position the screen position to spawn the flyer at
* @return NULL on error, or a pointer to a new flyer entity
*/
Entity *flyer_spawn(Vector2D position);

/**
* @brief function to update values on the walker
* @param self the walker entity
*/
void flyer_update(Entity *self);

/**
* @brief function to update the flyer, move position, etc
* @param self the flyer entity
*/
void flyer_think(Entity *self);

/**
* @brief checks if flyer is colliding with any tiles, and if it should be flying up
* @params the flyer entity
*/
void flyer_check_flight_collision(Entity *ent);



#endif