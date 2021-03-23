#include "simple_logger.h"

#include "bouncer.h"
#include "level.h"
#include "tile.h"

void bouncer_update(Entity *self);
void bouncer_think(Entity *self);

Entity *bouncer_spawn(Vector2D position)
{
	Entity *ent;

	ent = enemy_spawn(position);
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}

	ent->enemy_type = ENEMY_BOUNCER;
	ent->sprite = gf2d_sprite_load_all("assets/sprites/enemy4.png", 64, 64, 1);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = bouncer_update;
	ent->think = bouncer_think;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->hitbox.x = 0;
	ent->hitbox.y = 0;
	ent->hitbox.w = 64;
	ent->hitbox.h = 64;


	ent->gridPos = vector2d(position.x / 32, position.y / 32);

	ent->check_collision = bouncer_check_player_collision;

	ent->health = 10;
	ent->contactDamage = 20;


	return ent;
}


void bouncer_update(Entity *self)
{
	if (!self)return;

	enemy_update(self);
	enemy_check_player_collision(self);

}

void bouncer_think(Entity *self)
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
		self->velocity.x = 2;
	}
	if (self->velocity.y == 0)
	{
		self->velocity.y = 2;
	}
}


Bool bouncer_check_player_collision(Entity *ent)
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

	if (!ent)
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

		isIntersect = SDL_HasIntersection(&ent->hitbox, &level->tileArray[i]->hitbox);

		//slog("checked");


		if (!isIntersect)
		{

			continue;
		}
		else
		{
			testMove.x = ent->hitbox.x;
			testMove.y = ent->hitbox.y;
			testMove.w = ent->hitbox.w;
			testMove.h = ent->hitbox.h;
		}


		//if below
		if (entity_check_below_collision(ent->hitbox, tile->hitbox) && ent->velocity.y > 0)
		{

			testMove.y = tile->hitbox.y - testMove.h;
			if (!entity_check_below_collision(testMove, tile->hitbox))
			{
				//slog("collision on bottom");
				ent->position.y = testMove.y;
				ent->hitbox.y = testMove.y;
				ent->velocity.y = -2;
			}
		}


		isIntersect = SDL_HasIntersection(&ent->hitbox, &level->tileArray[i]->hitbox);

		//slog("checked");

		if (!isIntersect)
		{

			continue;
		}
		else
		{
			testMove.x = ent->hitbox.x;
			testMove.y = ent->hitbox.y;
			testMove.w = ent->hitbox.w;
			testMove.h = ent->hitbox.h;
		}

		//if above
		if (entity_check_above_collision(ent->hitbox, tile->hitbox) && ent->velocity.y < 0)
		{

			testMove.y = tile->hitbox.y + tile->hitbox.h;

			if (!entity_check_above_collision(testMove, tile->hitbox) && !(entity_check_right_collision(testMove, tile->hitbox) || entity_check_left_collision(testMove, tile->hitbox)))
			{
				//slog("collision on top");
				ent->position.y = testMove.y;
				ent->hitbox.y = testMove.y;
				ent->velocity.y = 2;
			}
		}

		isIntersect = SDL_HasIntersection(&ent->hitbox, &level->tileArray[i]->hitbox);

		//slog("checked")


		if (!isIntersect)
		{

			continue;
		}
		else
		{
			testMove.x = ent->hitbox.x;
			testMove.y = ent->hitbox.y;
			testMove.w = ent->hitbox.w;
			testMove.h = ent->hitbox.h;
		}

		//if on left
		isIntersect = SDL_HasIntersection(&ent->hitbox, &level->tileArray[i]->hitbox);
		if (!isIntersect)
		{
			continue;
		}
		else
		{
			testMove.x = ent->hitbox.x;
			testMove.y = ent->hitbox.y;

		}
		if (entity_check_left_collision(ent->hitbox, tile->hitbox) && ent->velocity.x < 0)
		{

			testMove.x = tile->hitbox.x + tile->hitbox.w;
			if (!entity_check_left_collision(testMove, tile->hitbox) && !entity_check_above_collision(testMove, tile->hitbox))
			{
				//slog("collision on left");
				ent->position.x = testMove.x;
				ent->hitbox.x = testMove.x;
				ent->velocity.x = 2;
			}
		}


		isIntersect = SDL_HasIntersection(&ent->hitbox, &level->tileArray[i]->hitbox);

		//slog("checked");


		if (!isIntersect)
		{

			continue;
		}
		else
		{
			testMove.x = ent->hitbox.x;
			testMove.y = ent->hitbox.y;
			testMove.w = ent->hitbox.w;
			testMove.h = ent->hitbox.h;
		}

		//if on right
		if (entity_check_right_collision(ent->hitbox, tile->hitbox) && ent->velocity.x > 0)
		{

			testMove.x = tile->hitbox.x - testMove.w;
			if (!entity_check_right_collision(testMove, tile->hitbox) && !entity_check_above_collision(testMove, tile->hitbox))
			{
				//slog("collision on right");
				ent->position.x = testMove.x;
				ent->hitbox.x = testMove.x;
				ent->velocity.x = -2;

			}
		}

	}
}


/**/