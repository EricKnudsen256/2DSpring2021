#include "simple_logger.h"

#include "e_drop.h"


Drop *drop_new(Vector2D position, Bool randomVelocity)
{
	Drop *drop;

	drop = (Drop*)malloc(sizeof(Drop));

	memset(drop, 0, sizeof(Drop));


	drop->ent = entity_new();
}


void drop_update(Drop drop)
{

}


void drop_draw(Drop drop)
{

}


Bool drop_check_player_collision(Drop *drop)
{

}