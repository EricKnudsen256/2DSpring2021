#include "simple_logger.h"

#include "e_bouncer.h"

#include "w_room.h"
#include "w_tile.h"

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


	ent->draw = enemy_draw;

	ent->gridPos = vector2d(position.x / 32, position.y / 32);

	ent->check_collision = bouncer_check_player_collision;
	

	ent->startingHealth = 10;
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
	int roomX, roomY, x, y;
	Room **roomlist;
	SDL_bool isIntersect;
	Tile *tile;
	SDL_Rect testMove;
	Bool foundBot, foundAbove, foundLeft, foundRight;
	Bool shouldFly = false;



	foundBot = false;
	foundAbove = false;
	foundLeft = false;
	foundRight = false;

	roomlist = room_manager_get_room_list();

	//turn off collision lol
	//return;


	if (!ent)
	{
		slog("No entity provided");
		return false;
	}
	if (!roomlist)
	{
		slog("Room list not found");
		return false;
	}
	for (roomX = 0; roomX < room_manager_get_max_columns(); roomX++)
	{
		for (roomY = 0; roomY < room_manager_get_max_rows(); roomY++)
		{
			if (roomlist[roomX][roomY]._inuse == 0)continue;

			if (!roomlist[roomX][roomY].tileArray)
			{
				slog("Level does not have a tile array");
				return false;
			}

			//slog("Room x:%i, y:%i", roomX, roomY);

			Room *room = &roomlist[roomX][roomY];

			if (ent->position.x > room->tileWidth * room->roomWidth * (room->roomPos.x + 1) || ent->position.x < room->tileWidth * room->roomWidth * room->roomPos.x)
			{
				continue;
			}
			if (ent->position.y > room->tileHeight * room->roomHeight * (room->roomPos.y + 1) || ent->position.y < room->tileHeight * room->roomHeight * room->roomPos.y)
			{
				continue;
			}

			for (x = 0; x < room->roomHeight; x++)
			{
				for (y = 0; y < room->roomWidth; y++)
				{



					if (room->tileArray[x][y] == NULL)
					{
						continue;
					}
					if (!room->tileArray[x][y]->_inuse)
					{
						continue;
					}



					tile = room->tileArray[x][y];

					if (ent->position.x - tile->pos.x < 200 && ent->position.x - tile->pos.x > -200 && ent->position.y - tile->pos.y < 200 && ent->position.y - tile->pos.y > -200)
					{

					}
					else
					{
						continue;
					}

					isIntersect = SDL_HasIntersection(&ent->hitbox2, &tile->hitbox);

					//slog("checked");

					//if below
					if (isIntersect)
					{
						shouldFly = true;
						ent->velocity.y -= .07;
					}
					else if (!isIntersect && !shouldFly)
					{
						ent->velocity.y += .0005;

						if (ent->velocity.y > 2)
						{
							ent->velocity.y = 2;
						}

					}

					//if collision found, check which side of the ent it was on and adjust accordingly



					isIntersect = SDL_HasIntersection(&ent->hitbox, &tile->hitbox);


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
					if (entity_check_below_collision(ent->hitbox, tile->hitbox))
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



					//if above
					if (entity_check_above_collision(ent->hitbox, tile->hitbox))
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

					//if on left
					isIntersect = SDL_HasIntersection(&ent->hitbox, &tile->hitbox);
					if (!isIntersect)
					{
						continue;
					}
					else
					{
						testMove.x = ent->hitbox.x;
						testMove.y = ent->hitbox.y;

					}
					if (entity_check_left_collision(ent->hitbox, tile->hitbox))
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


					//if on right
					if (entity_check_right_collision(ent->hitbox, tile->hitbox))
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

			for (x = 0; x < room->roomHeight; x++)
			{
				for (y = 0; y < room->roomWidth; y++)
				{

					tile = room->tileArray[x][y];

					if (!tile)
					{
						continue;
					}

					if (!tile->_inuse)
					{
						continue;
					}


					//check if there are tiles in the way of the player, adjust onX values if none are found
					//change move values for tolerance

					testMove.x = ent->hitbox.x;
					testMove.y = ent->hitbox.y;
					testMove.w = ent->hitbox.w;
					testMove.h = ent->hitbox.h;

					//check bottom side
					testMove.y += 1;
					if (SDL_HasIntersection(&testMove, &tile->hitbox))
					{
						foundBot = true;
					}

					testMove.y = ent->hitbox.y;

					//check right side
					testMove.x += 1;
					if (SDL_HasIntersection(&testMove, &tile->hitbox))
					{
						foundRight = true;
					}

					testMove.x = ent->hitbox.x;

					//check right side
					testMove.x -= 1;
					if (SDL_HasIntersection(&testMove, &tile->hitbox))
					{
						foundLeft = true;
					}

				}
			}
		}
	}

	if (!foundBot)
	{
		ent->onGround = false;
	}
	if (!foundLeft)
	{
		ent->onLeft = false;
	}
	if (!foundRight)
	{
		ent->onRight = false;
	}

	/*

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

		isIntersect = SDL_HasIntersection(&self->hitbox, &level->tileArray[i]->hitbox);

		//slog("checked");


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
				self->velocity.y = -2;
			}
		}


		isIntersect = SDL_HasIntersection(&self->hitbox, &level->tileArray[i]->hitbox);

		//slog("checked");

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

		//if above
		if (entity_check_above_collision(self->hitbox, tile->hitbox) && self->velocity.y < 0)
		{

			testMove.y = tile->hitbox.y + tile->hitbox.h;

			if (!entity_check_above_collision(testMove, tile->hitbox) && !(entity_check_right_collision(testMove, tile->hitbox) || entity_check_left_collision(testMove, tile->hitbox)))
			{
				//slog("collision on top");
				self->position.y = testMove.y;
				self->hitbox.y = testMove.y;
				self->velocity.y = 2;
			}
		}

		isIntersect = SDL_HasIntersection(&self->hitbox, &level->tileArray[i]->hitbox);

		//slog("checked")


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
			if (!entity_check_left_collision(testMove, tile->hitbox) && !entity_check_above_collision(testMove, tile->hitbox))
			{
				//slog("collision on left");
				self->position.x = testMove.x;
				self->hitbox.x = testMove.x;
				self->velocity.x = 2;
			}
		}


		isIntersect = SDL_HasIntersection(&self->hitbox, &level->tileArray[i]->hitbox);

		//slog("checked");


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

		//if on right
		if (entity_check_right_collision(self->hitbox, tile->hitbox) && self->velocity.x > 0)
		{

			testMove.x = tile->hitbox.x - testMove.w;
			if (!entity_check_right_collision(testMove, tile->hitbox) && !entity_check_above_collision(testMove, tile->hitbox))
			{
				//slog("collision on right");
				self->position.x = testMove.x;
				self->hitbox.x = testMove.x;
				self->velocity.x = -2;

			}
		}

	}
	*/
}


/**/