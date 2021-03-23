#include "simple_logger.h"

#include "flyer.h"
#include "level.h"
#include "tile.h"

void flyer_update(Entity *self);
void flyer_think(Entity *self);

Entity *flyer_spawn(Vector2D position)
{
	Entity *ent;

	ent = enemy_spawn(position);
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}

	ent->enemy_type = ENEMY_FLYER;
	ent->sprite = gf2d_sprite_load_all("assets/sprites/enemy2.png", 64, 64, 1);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = flyer_update;
	ent->think = flyer_think;
	ent->check_collision = flyer_check_flight_collision;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->hitbox.x = 0;
	ent->hitbox.y = 0;
	ent->hitbox.w = 64;
	ent->hitbox.h = 64;

	ent->gridPos = vector2d(position.x / 32, position.y / 32);


	gfc_rect_set(ent->hitbox2, ent->hitbox.x, ent->hitbox.y + ent->hitbox.h, ent->hitbox.w, 75);

	ent->health = 25;
	ent->contactDamage = 15;


	return ent;
}


void flyer_update(Entity *self)
{
	if (!self)return;

	enemy_update(self);
	enemy_check_player_collision(self);


	if (&self->hitbox2)
	{

		self->hitbox2.x = self->hitbox.x;
		self->hitbox2.y = self->hitbox.y + self->hitbox.h;
	}

}

void flyer_think(Entity *self)
{
	enemy_think(self);
	Vector2D gravity;
	float angle;
	int mx, my;
	if (!self)return;
	SDL_GetMouseState(&mx, &my);

	//put loop to check for floor collision on gravity


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

void flyer_check_flight_collision(Entity *self)
{
	int i;
	Level *level;
	SDL_bool isIntersect;
	Tile *tile;
	SDL_Rect testMove;
	Bool shouldFly = false;
	Bool foundBot, foundAbove, foundLeft, foundRight;



	foundBot = false;
	foundAbove = false;
	foundLeft = false;
	foundRight = false;

	level = level_manager_get_current();

	if (!self)
	{
		slog("No entity provided");
		return;
	}
	if (!level)
	{
		slog("Current level not found");
		return;
	}
	if (!level->tileArray)
	{
		slog("Level does not have a tile array");
		return;
	}

	for (i = 0; i < level->tileArrayLen; i++)
	{


		if (level->tileArray[i] == NULL)
		{
			continue;
		}
		if (!level->tileArray[i]->_inuse)
		{
			continue;
		}

		tile = level->tileArray[i];


		isIntersect = SDL_HasIntersection(&self->hitbox2, &level->tileArray[i]->hitbox);

		//slog("checked");

		//if below
		if (isIntersect)
		{
			shouldFly = true;
			self->velocity.y -= .07;
		}
		else if (!isIntersect && !shouldFly)
		{
			self->velocity.y += .0005;

			if (self->velocity.y > 2)
			{
				self->velocity.y = 2;
			}

		}

		//if collision found, check which side of the ent it was on and adjust accordingly

		isIntersect = SDL_HasIntersection(&self->hitbox, &level->tileArray[i]->hitbox);

		if (!isIntersect)
		{

			continue;
		}
		else
		{
			testMove.x = self->hitbox.x;
			testMove.y = self->hitbox.y;
			testMove.w = self->hitbox.w;
			testMove.h = self->hitbox.h;
		}


		//if below
		if (entity_check_below_collision(self->hitbox, tile->hitbox) && self->velocity.y > 0)
		{

			testMove.y = tile->hitbox.y - testMove.h;
			if (!entity_check_below_collision(testMove, tile->hitbox))
			{
				//slog("collision on bottom");
				self->position.y = testMove.y;
				self->hitbox.y = testMove.y;
				self->velocity.y = 0;
				self->onGround = true;
			}
		}

		//if above
		if (entity_check_above_collision(self->hitbox, tile->hitbox) && self->velocity.y < 0)
		{

			testMove.y = tile->hitbox.y + tile->hitbox.h;
			if (!entity_check_above_collision(testMove, tile->hitbox))
			{
				//slog("collision on top");
				self->position.y = testMove.y;
				self->hitbox.y = testMove.y;
				self->velocity.y = 0;
			}
		}

		//if on left
		isIntersect = SDL_HasIntersection(&self->hitbox, &level->tileArray[i]->hitbox);
		if (!isIntersect)
		{
			continue;
		}
		else
		{
			testMove.x = self->hitbox.x;
			testMove.y = self->hitbox.y;

		}
		if (entity_check_left_collision(self->hitbox, tile->hitbox) && self->velocity.x < 0)
		{

			testMove.x = tile->hitbox.x + tile->hitbox.w;
			if (!entity_check_left_collision(testMove, tile->hitbox))
			{
				//slog("collision on left");
				self->position.x = testMove.x;
				self->hitbox.x = testMove.x;
				self->velocity.x = 0;
				self->onLeft = true;
			}
		}


		//if on right
		if (entity_check_right_collision(self->hitbox, tile->hitbox) && self->velocity.x > 0)
		{

			testMove.x = tile->hitbox.x - testMove.w;
			if (!entity_check_left_collision(testMove, tile->hitbox))
			{
				//slog("collision on right");
				self->position.x = testMove.x;
				self->hitbox.x = testMove.x;
				self->velocity.x = 0;
				self->onRight = true;
			}
		}

		//collision checking on hitbox 2

		//if collision found, check which side of the ent it was on and adjust accordingly

	}

	for (i = 0; i < level->tileArrayLen; i++)
	{

		if (!level->tileArray[i])
		{
			continue;
		}

		tile = level->tileArray[i];

		//check if there are tiles in the way of the player, adjust onX values if none are found
		//change move values for tolerance

		testMove.x = self->hitbox.x;
		testMove.y = self->hitbox.y;
		testMove.w = self->hitbox.w;
		testMove.h = self->hitbox.h;

		//check bottom side
		testMove.y += 1;
		if (SDL_HasIntersection(&testMove, &tile->hitbox))
		{

			foundBot = true;
		}

		testMove.y = self->hitbox.y;

		//check right side
		testMove.x += 1;
		if (SDL_HasIntersection(&testMove, &tile->hitbox))
		{
			foundRight = true;
		}

		testMove.x = self->hitbox.x;

		//check right side
		testMove.x -= 1;
		if (SDL_HasIntersection(&testMove, &tile->hitbox))
		{
			foundLeft = true;
		}
	}

	if (!foundBot)
	{
		self->onGround = false;
	}
	if (!foundLeft)
	{
		self->onLeft = false;
	}
	if (!foundRight)
	{
		self->onRight = false;
	}
	//slog("foundBot:%i", foundBot);




}



/**/