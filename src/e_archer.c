#include "simple_logger.h"

#include "e_archer.h"

#include "w_level.h"
#include "w_tile.h"

void archer_update(Entity *self);
void archer_think(Entity *self);

Entity *archer_spawn(Vector2D position)
{
	Entity *ent;

	ent = enemy_spawn(position);
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}

	ent->enemy_type = ENEMY_ARCHER;
	ent->sprite = gf2d_sprite_load_all("assets/sprites/enemy3.png", 64, 64, 1);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = archer_update;
	ent->think = archer_think;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->hitbox.x = 0;
	ent->hitbox.y = 0;
	ent->hitbox.w = 64;
	ent->hitbox.h = 64;


	ent->draw = enemy_draw;

	ent->gridPos = vector2d(position.x / 32, position.y / 32);

	gfc_rect_set(ent->hitbox2, ent->hitbox.x, ent->hitbox.y, 700, 50);


	ent->startingHealth = 50;
	ent->health = 50;
	ent->contactDamage = 0;

	ent->lastDamaged = SDL_GetTicks();
	ent->lastAttack = SDL_GetTicks();



	return ent;
}


void archer_update(Entity *self)
{
	if (!self)return;

	enemy_update(self);
	enemy_check_player_collision(self);

	archer_aim(self);

	if (&self->hitbox2)
	{
		if (self->velocity.x > 0)
		{
			self->hitbox2.x = self->hitbox.x;
			self->hitbox2.y = self->hitbox.y;
		}

		if (self->velocity.x < 0)
		{
			self->hitbox2.x = self->hitbox.x - self->hitbox2.w;
			self->hitbox2.y = self->hitbox.y;
		}

	}

}

void archer_think(Entity *self)
{
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

void archer_ranged(Entity *self)
{
	Entity *ent;
	EntityManager entManager;
	int i;

	Vector2D position, velocity;
	vector2d_copy(position, self->position);

	position.y += 12;

	velocity.y = 0;

	entManager = entity_manager_get_manager();


	if (SDL_GetTicks() >= self->lastAttack + 2000)
	{

		if (self->velocity.x < 0)
		{
			velocity.x = -8;
			projectile_spawn(25, 2000, self, position, velocity);
		}
		else if (self->velocity.x > 0)
		{
			velocity.x = 8;
			projectile_spawn(25, 2000, self, position, velocity);
		}
		else
		{
			slog("ranged direction not found");

		}

		self->lastAttack = SDL_GetTicks();
	}

}

void archer_aim(Entity *self)
{
	Entity *player = entity_manager_get_player_ent();

	Bool isIntersect;

	isIntersect = SDL_HasIntersection(&self->hitbox2, &player->hitbox);

	if (isIntersect)
	{
		self->targetingFrames++;
	}
	else
	{
		self->targetingFrames--;
	}

	if (self->targetingFrames < 0)
	{
		self->targetingFrames = 0;
	}

	if (self->targetingFrames > 60)
	{
		self->targetingFrames = 0;
		archer_ranged(self);

	}
}
/**/