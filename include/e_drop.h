#ifndef __DROP_H__
#define __DROP_H__

#include "e_entity.h"

#include "g_item.h"

typedef struct Drop_s
{
	Item *item;
	Entity *ent;

}Drop;


Drop *drop_new(Vector2D position, Bool randomVelocity);


void drop_update(Drop drop);


void drop_draw(Drop drop);


//free function in level class


Bool drop_check_player_collision(Drop *drop);


#endif