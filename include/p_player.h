#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "e_entity.h"
#include "g_item.h"


typedef struct Player_s
{
	//void(*update)(struct Entity_s *self);
	//void(*think)(struct Entity_s *self);
	//void(*draw)(struct Entity_s *self);
	//void(*free)(struct Entity_s *self);
	void       *data;

}Player;

typedef struct
{
	Item **item_list;
	Uint32  max_items;
}PlayerInventory;

static PlayerInventory inventory = { 0 };


void player_inventory_init(Uint32 max_items);

void player_inventory_add_item(Item *item);

void player_inventory_free();

void player_inventory_slog();



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

void player_attack(Entity *self);

void player_ranged(Entity *self);

void player_die(Entity *self);

void player_damage(Entity *player, int damage);


#endif