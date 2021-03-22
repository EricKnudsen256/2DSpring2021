#ifndef __ARCHER_H__
#define __ARCHER_H__

#include "enemy.h"


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
* @brief spawn a player entity
* @param position the screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
Entity *archer_spawn(Vector2D position);

/**
* @brief checks if it has been longer than 350 ms since the player's last jump
* @param self the player
* @return true if it has been longer than 350 ms, false otherwise
*/

void archer_draw(Entity* self);


/**
* @brief checks every frame to see if enemy is colliding with the player
* @param self the enemy
*/
Bool archer_check_player_collision(Entity *ent);


void archer_aim(Entity *self);



#endif