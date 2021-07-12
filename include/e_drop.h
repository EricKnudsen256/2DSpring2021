#ifndef __DROP_H__
#define __DROP_H__

#include "e_entity.h"

#include "g_item.h"

typedef struct Drop_s
{
	int id;
	Bool _inuse;

	Item *item;
	Entity *ent;

	int spawnTime;
	int collectTime;

}Drop;


Drop *drop_new(Vector2D position, Bool randomVelocity, Item *item);


void drop_update(Drop *drop);


//free function in level class


Bool drop_check_player_collision(Drop *drop);


#endif