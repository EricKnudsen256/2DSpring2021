#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "e_entity.h"

#include "g_item.h"
#include "g_inventory.h"

//struct that isn't used lol
typedef struct Player_s
{
	//void(*update)(struct Entity_s *self);
	//void(*think)(struct Entity_s *self);
	//void(*draw)(struct Entity_s *self);
	//void(*free)(struct Entity_s *self);
	void       *data;

}Player;

static Inventory *playerInventory = {0};

Inventory *player_inventory_get();

void player_inventory_free();

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


void player_think(Bool left, Bool right, Bool jump, Bool down, Bool sprint, Entity *self);

/**
* @brief loads all predefined sounds for the player
*/
void player_load_sounds();

/**
* @brief draws the player
* @param self the player entity
*/
void player_draw(Entity *self);

/**
* @brief makes a melee attack in the direction the player is facing
* @param self the player entity
*/
void player_attack(Entity *self);

/**
* @brief fires a projectile in the direction the player is facing
* @param self the player entity
*/
void player_ranged(Entity *self);

/**
* @brief kills the player when health = 0
* @param self the player entity
*/
void player_die(Entity *self);

/**
* @brief deals damage to the player without going past 0 health
* @param self the player entity
* @param damage how much damage the player should take
*/
void player_damage(Entity *player, int damage);



#endif