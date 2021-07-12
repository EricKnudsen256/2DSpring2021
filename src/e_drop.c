#include "simple_logger.h"

#include "e_drop.h"

#include "g_camera.h"
#include "g_random.h"

#include "w_level.h"


Drop *drop_new(Vector2D position, Bool randomVelocity, Item *item)
{
	Drop *drop;

	drop = (Drop*)malloc(sizeof(Drop));

	memset(drop, 0, sizeof(Drop));


	drop->ent = entity_new();

	drop->item = item;

	drop->ent->sprite = item->sprite;

	drop->ent->hitbox.w = 64;
	drop->ent->hitbox.h = 64;

	drop->ent->position = position;

	drop->spawnTime = SDL_GetTicks();
	drop->collectTime = 2000;

	if (randomVelocity)
	{
		drop->ent->velocity.x = random_int_range(-5, 5) / 2.0;
		drop->ent->velocity.y = random_int_range(-5, 0) / 2.0;
	}

	return drop;
}


void drop_update(Drop *drop)
{

	if (!drop->ent->onGround)
	{
		drop->ent->velocity.y += .1;
	}

	else
	{
		drop->ent->velocity.y = 0;

		if (drop->ent->velocity.x < .05 || drop->ent->velocity.x > -.05)
		{
			drop->ent->velocity.x = 0;
		}
		else if (drop->ent->velocity.x > 0)
		{
			drop->ent->velocity.x -= .005;
		}
		else if (drop->ent->velocity.x < 0)
		{
			drop->ent->velocity.x += .005;
		}
		
	}
	

	drop_check_player_collision(drop);

}


Bool drop_check_player_collision(Drop *drop)
{
	Entity *player = entity_manager_get_player_ent();

	if (SDL_HasIntersection(&player->hitbox, &drop->ent->hitbox) && drop->collectTime + drop->spawnTime < SDL_GetTicks())
	{
		player_inventory_add_item(drop->item);
		level_drop_free(drop, level_manager_get_current());
	}
}