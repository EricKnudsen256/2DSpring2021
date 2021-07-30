#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "e_entity.h"
#include "g_item.h"

//struct that isn't used lol
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

/**
* @brief creates the player's inventory
* @param max_items the total number of items in the inventory
*/
void player_inventory_init(Uint32 max_items);

/**
* @brief adds an item to the first open slot of the inventory
* @param item the item to be added
*/
void player_inventory_add_item(Item *item);


void player_inventory_remove_item(int slot);

/**
* @brief gets the item in the provided inventory slot
* @param slot the index of the inventory slot to check
* @return NULL if no item found, a pointer to the item otherwise
*/
Item *player_inventory_get_item(int slot);

/**
* @brief frees the inventory
*/
void player_inventory_free();

/**
* @brief posts the entire player inventory to console for error checking
*/
void player_inventory_slog();

/**
* @brief swaps two items in the player's inventory
* @param slot1 the slot of the first item
* @param slot2 the slot of the second item
*/
void player_inventory_swap(int slot1, int slot2);

/**
* @brief returns the size of the inventory
* @return int if the inventory size
*/
Uint32 player_inventory_get_max();

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