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
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_all("assets/sprites/enemy1.png", 64, 64, 1);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = enemy_update;
	ent->think = enemy_think;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->hitbox.x = 0;
	ent->hitbox.y = 0;
	ent->hitbox.w = 64;
	ent->hitbox.h = 64;


	return ent;
}


void enemy_update(Entity *self)
{
	if (!self)return;

}

void enemy_think(Entity *self)
{
	
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

	//check for any keys pressed
	/*
	if (keys[SDL_SCANCODE_D])
	{
		if (self->onRight == false)
		{
			self->velocity.x = 3;
			self->onLeft = false;
		}

	}
	else if (keys[SDL_SCANCODE_A])
	{
		if (self->onLeft == false)
		{
			self->velocity.x = -3;
			self->onRight = false;
		}
	}
	else
	{
		self->onRight = false;
		self->onLeft = false;
		self->velocity.x = 0;
	}

	if (keys[SDL_SCANCODE_SPACE] && player_is_allowed_jump(self) && (self->onGround == true || self->doubleJumped == false))
	{
		self->velocity.y = -5;
		self->position.y -= .1;

		if (!self->onGround)
		{
			self->doubleJumped = true;
		}

		self->onGround = false;
		self->lastJump = SDL_GetTicks();
	}

	*/
}


/**/