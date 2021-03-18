#include "simple_logger.h"

#include "enemy.h"

void enemy_update(Entity *self);
void enemy_think(Entity *self);

Entity *enemy_spawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for selected enemy");
		return NULL;
	}
	//ent->sprite = gf2d_sprite_load_all("assets/sprites/enemy1.png", 64, 64, 1);
	vector2d_copy(ent->position, position);

	return ent;
}


void enemy_update(Entity *self)
{
	if (!self)return;

	if (self->health <= 0)
	{
		enemy_die(self);
	}

}

void enemy_think(Entity *self)
{

}

Bool enemy_check_player_collision(Entity *ent)
{
	int i;
	Entity *player;
	SDL_bool isIntersect;


	player = entity_manager_get_player_ent();


	if (!ent)
	{
		slog("No entity provided");
		return;
	}
	if (!player)
	{
		slog("No player found");
		return;
	}

	isIntersect = SDL_HasIntersection(&ent->hitbox, &player->hitbox);

	if (!isIntersect)
	{
		return;
	}

	player->health -= 1;


	return true;
	

}

void enemy_die(Entity *self)
{
	int i;
	if (!self)return;// nothing to do



	gf2d_sprite_free(self->sprite);

	self->sprite = NULL;

	self->_inuse = false;
	self = NULL;
}


/**/