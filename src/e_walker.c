#include "simple_logger.h"

#include "e_walker.h"
#include "g_camera.h"
 
void walker_update(Entity *self);
void walker_think(Entity *self);

Entity *walker_spawn(Vector2D position)
{
	Entity *ent;

	ent = enemy_spawn(position);
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}

	ent->enemy_type = ENEMY_WALKER;
	ent->sprite = gf2d_sprite_load_all("assets/sprites/slimeSheet2.png", 64, 64, 12);
	ent->frameRate = 0.2;
	ent->frameCount = 12;

	ent->update = walker_update;
	ent->think = walker_think;
	ent->draw = enemy_draw;

	ent->rotation.x = 64;
	ent->rotation.y = 64;

	ent->hitbox.w = 64;
	ent->hitbox.h = 64;



	//slog("x:%f, y:%f", position.x, position.y);

	ent->position.x = position.x;
	ent->position.y = position.y;

	ent->hitbox.x = position.x;
	ent->hitbox.y = position.y;
	
	ent->gridPos = vector2d(position.x / 32, position.y / 32);
	


	ent->startingHealth = 50;
	ent->health = 50;
	ent->contactDamage = 10;


	return ent;
}


void walker_update(Entity *self)
{
	if (!self)return;

	enemy_update(self);
	enemy_check_player_collision(self);

	//slog("x:%f, y:%f", self->position.x, self->position.y);

}

void walker_think(Entity *self)
{
	//return;

	enemy_think(self);
	Vector2D gravity;
	float angle;
	int mx, my;
	if (!self)return;
	SDL_GetMouseState(&mx, &my);

	//put loop to check for floor collision on gravity

	if (!self->onGround)
	{
		self->velocity.y += .075;
	}



	//edit this to change max fall speed
	if (self->velocity.y > 4)
	{
		self->velocity.y = 4;
	}

	if (self->onGround)
	{
		self->doubleJumped = false;
	}

	if (self->velocity.x == 0)
	{
		self->velocity.x = 1.5;
	}
	if (self->onRight == true)
	{
		self->velocity.x = -1.5;
	}
	if (self->onLeft == true)
	{
		self->velocity.x = 1.5;
	}
}




/**/