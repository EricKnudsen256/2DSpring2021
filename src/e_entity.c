#include <stdlib.h>
#include "simple_logger.h"

#include "e_entity.h"

#include "g_camera.h"

#include "w_room.h"

void entity_check_collisions(Entity *ent);


void entity_manager_init(Uint32 max_entities)
{
	if (max_entities == 0)
	{
		slog("cannot allocate 0 entities!");
		return;
	}
	if (entity_manager.entity_list != NULL)
	{
		entity_manager_free();
	}
	entity_manager.entity_list = (Entity *)gfc_allocate_array(sizeof (Entity), max_entities);
	if (entity_manager.entity_list == NULL)
	{
		slog("failed to allocate entity list!");
		return;
	}
	entity_manager.max_entities = max_entities;
	slog("entity system initialized");
}

void entity_manager_free()
{
	if (entity_manager.entity_list != NULL)
	{
		free(entity_manager.entity_list);
	}
	memset(&entity_manager, 0, sizeof(EntityManager));
	slog("entity system closed");
}


void entity_manager_think_entities()
{
	int i;
	if (entity_manager.entity_list == NULL)
	{
		slog("entity system does not exist");
		return;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse == 0)continue;
		if (entity_manager.entity_list[i].think != NULL)
		{
			entity_manager.entity_list[i].think(&entity_manager.entity_list[i]);
		}
	}
}

void entity_update(Entity *self)
{
	if (!self)return;

	// DO ANY GENERIC UPDATE CODE

	//THIS LINE UPDATES POSITION BASED ON VELOCITY V2
	vector2d_add(self->position, self->position, self->velocity);
	self->frame += self->frameRate;
	if (self->frame >= self->frameCount)self->frame = 0;
	// IF THERE IS A CUSTOM UPDATE, DO THAT NOW
	if (self->update)self->update(self);

	if (&self->hitbox)
	{
		self->hitbox.x = self->position.x;
		self->hitbox.y = self->position.y;
	}

}

void entity_manager_update_entities()
{
	int i;
	if (entity_manager.entity_list == NULL)
	{
		slog("entity system does not exist");
		return;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse == 0)continue;
		entity_update(&entity_manager.entity_list[i]);
	}
	
}


void entity_manager_check_collions()
{
	int i;
	if (entity_manager.entity_list == NULL)
	{
		slog("entity system does not exist");
		return;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse == 0)continue;
		if (entity_manager.entity_list[i].check_collision)
		{
			entity_manager.entity_list[i].check_collision(&entity_manager.entity_list[i]);
			continue;
		}
		entity_check_collisions(&entity_manager.entity_list[i]);
	}
}

void entity_manager_draw_entities()
{
	int i;
	if (entity_manager.entity_list == NULL)
	{
		slog("entity system does not exist");
		return;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse == 0)continue;
		entity_draw(&entity_manager.entity_list[i]);
	}
}

Entity *entity_manager_get_player_ent()
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i].isPlayer == true)
		{
			return &entity_manager.entity_list[i];
		}

	}
	slog("Player entity not found");
	return NULL;
}

EntityManager entity_manager_get_manager()
{
	return entity_manager;
}

int entity_manager_get_max_ents()
{
	return entity_manager.max_entities;
}


Entity *entity_new()
{
	int i;
	if (entity_manager.entity_list == NULL)
	{
		slog("entity system does not exist");
		return NULL;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse)continue;// someone else is using this one
		memset(&entity_manager.entity_list[i], 0, sizeof(Entity));
		entity_manager.entity_list[i]._inuse = 1;
		return &entity_manager.entity_list[i];
	}
	slog("no free entities available");
	return NULL;
}


void entity_check_collisions(Entity *ent)
{
	int roomX, roomY, x, y;
	Room **roomlist;
	SDL_bool isIntersect;
	Tile *tile;
	SDL_Rect testMove;
	Bool foundBot, foundAbove, foundLeft, foundRight;



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
		return;
	}
	if (!roomlist)
	{
		slog("Room list not found");
		return;
	}
	for (roomX = 0; roomX < room_manager_get_max_columns(); roomX++)
	{
		for (roomY = 0; roomY < room_manager_get_max_rows(); roomY++)
		{
			if (roomlist[roomX][roomY]._inuse == 0)continue;

			if (!roomlist[roomX][roomY].tileArray)
			{
				slog("Level does not have a tile array");
				return;
			}

			//slog("Room x:%i, y:%i", roomX, roomY);

			Room *room = &roomlist[roomX][roomY];

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
							ent->velocity.y = 0;
							ent->onGround = true;
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
							ent->velocity.y = 0;
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
							ent->velocity.x = 0;
							ent->onLeft = true;
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
							ent->velocity.x = 0;
							ent->onRight = true;
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
	//slog("foundBot:%i", foundBot);
}

void entity_free(Entity *ent)
{
	if (!ent)
	{
		slog("cannot free a NULL entity");
		return;
	}
	gf2d_sprite_free(ent->sprite);
	ent->sprite = NULL;
	memset(&ent, 0, sizeof(Entity));
	ent->_inuse = 0;
}

void entity_draw(Entity *ent)
{
	Vector2D drawPosition, offset;
	if (!ent)
	{
		slog("cannot draww a NULL entity");
		return;
	}
	if (ent->draw)
	{
		ent->draw(ent);
	}
	else
	{
		if (ent->sprite == NULL)
		{
			return;// nothing to draw
		}
		offset = camera_get_offset();
		if (!camera_rect_on_screen(gfc_sdl_rect(ent->position.x, ent->position.y, ent->sprite->frame_w, ent->sprite->frame_h)))
		{
			//entity is off camera, skip
			//return;
		}
		drawPosition.x = ent->position.x + offset.x;
		drawPosition.y = ent->position.y + offset.y;


		gf2d_sprite_draw(
			ent->sprite,
			drawPosition,
			NULL,
			NULL,
			&ent->rotation,
			NULL,
			NULL,
			(Uint32)ent->frame);

		//test code to draw the hitboxes for an ent that has one

		if (&ent->hitbox)
		{

			SDL_Rect tempDraw;

			tempDraw.x = ent->hitbox.x + offset.x;
			tempDraw.y = ent->hitbox.y + offset.y;
			tempDraw.w = ent->hitbox.w;
			tempDraw.h = ent->hitbox.h;

			SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 255, 255, 255, 255);

			SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &tempDraw);

		}

		if (ent->isPlayer)
		{
			SDL_Rect attackbox;

			gfc_rect_set(attackbox, ent->hitbox.x - 50 + offset.x, ent->hitbox.y + offset.y, 50, ent->hitbox.h);

			SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 255, 255, 0, 255);

			SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &attackbox);

			gfc_rect_set(attackbox, ent->hitbox.x + ent->hitbox.w + offset.x, ent->hitbox.y + offset.y, 50, ent->hitbox.h);

			SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &attackbox);
		}

		if (&ent->hitbox2)
		{
			//slog("x:%i, y:%i, w:%i, h:%i", ent->hitbox2.x, ent->hitbox2.y, ent->hitbox2.w, ent->hitbox2.h);
			SDL_Rect tempDraw;

			tempDraw.x = ent->hitbox2.x + offset.x;
			tempDraw.y = ent->hitbox2.y + offset.y;
			tempDraw.w = ent->hitbox2.w;
			tempDraw.h = ent->hitbox2.h;

			gfc_rect_set(tempDraw, ent->hitbox2.x + offset.x, ent->hitbox2.y + offset.y, ent->hitbox2.w,ent->hitbox2.h);

			SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 0, 0, 0, 255);

			SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &tempDraw);
		}

	}
}

Bool entity_check_left_collision(SDL_Rect ent, SDL_Rect tile)
{
	return tile.x + tile.w > ent.x && ent.x > tile.x;
}
Bool entity_check_right_collision(SDL_Rect ent, SDL_Rect tile)
{
	return tile.x < ent.x + ent.w && ent.x + ent.w < tile.x + tile.w;
}
Bool entity_check_below_collision(SDL_Rect ent, SDL_Rect tile)
{
	return tile.y < ent.y + ent.h && ent.y + ent.h < tile.y + tile.h;
}
Bool entity_check_above_collision(SDL_Rect ent, SDL_Rect tile)
{
	return tile.y + tile.h > ent.y && ent.y > tile.y;
}

/*eol@eof*/