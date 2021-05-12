#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "e_entity.h"



/**
* @brief spawn a player entity
* @param position the screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
Entity *enemy_spawn(Vector2D position);

/**
* @brief checks if it has been longer than 350 ms since the player's last jump
* @param self the player
* @return true if it has been longer than 350 ms, false otherwise
*/

Entity *enemy_spawn_random(Vector2D position);


void enemy_draw(Entity *self);

/**
* @brief checks every frame to see if enemy is colliding with the player
* @param self the enemy
*/
Bool enemy_check_player_collision(Entity *ent);

/**
* @brief called when update sees enemy is out of health, destroys the enemy
* @param self the enemy to kill
*/

void enemy_die(Entity *self);

/**
* @brief special draw function for enemies that also draws the health bar
* @param ent the enemy to draw
*/

void enemy_draw(Entity *ent);


#endif