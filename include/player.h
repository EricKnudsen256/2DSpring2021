#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"


typedef struct Player_s
{
	Entity playerEnt;
	//void(*update)(struct Entity_s *self);
	//void(*think)(struct Entity_s *self);
	//void(*draw)(struct Entity_s *self);
	//void(*free)(struct Entity_s *self);
	void       *data;
}Player;

/**
* @brief spawn a player entity
* @param position the screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
Entity *player_spawn(Vector2D position);

/**
* @brief checks if it has been longer than 350 ms since the player's last jump
* @param self the player
* @return true if it has been longer than 350 ms, false otherwise
*/
Bool player_is_allowed_jump(Entity *self);

void player_draw(Entity *self);

#endif