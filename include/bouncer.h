#ifndef __BOUNCER_H__
#define __BOUNCER_H__

#include "enemy.h"


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
* @brief spawn a player entity
* @param position the screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
Entity *bouncer_spawn(Vector2D position);

/**
* @brief checks if it has been longer than 350 ms since the player's last jump
* @param self the player
* @return true if it has been longer than 350 ms, false otherwise
*/

void bouncer_draw(Entity* self);


/**
* @brief checks every frame to see if enemy is colliding with the player
* @param self the enemy
*/
Bool bouncer_check_player_collision(Entity *ent);



#endif