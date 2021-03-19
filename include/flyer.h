#ifndef __FLYER_H__
#define __FLYER_H__

#include "enemy.h"


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
* @brief spawn a player entity
* @param position the screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
Entity *flyer_spawn(Vector2D position);

/**
* @brief checks if it has been longer than 350 ms since the player's last jump
* @param self the player
* @return true if it has been longer than 350 ms, false otherwise
*/

void flyer_draw(Entity *self);


/**
* @brief checks every frame to see if enemy is colliding with the player
* @param self the enemy
*/
Bool flyer_check_player_collision(Entity *ent);


void flyer_check_flight_collision(Entity *self);



#endif