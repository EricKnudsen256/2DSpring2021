#ifndef __WALKER_H__
#define __WALKER_H__

#include "enemy.h"


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
* @brief spawn a player entity
* @param position the screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
Entity *walker_spawn(Vector2D position);

/**
* @brief checks if it has been longer than 350 ms since the player's last jump
* @param self the player
* @return true if it has been longer than 350 ms, false otherwise
*/

void walker_draw(Walker * self);


/**
* @brief checks every frame to see if enemy is colliding with the player
* @param self the enemy
*/
Bool walker_check_player_collision(Entity *ent);



#endif