#include <stdlib.h>
#include "simple_logger.h"

#include "e_entity.h"

#include "g_camera.h"

#include "w_room.h"
#include "w_level.h"

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
	//vector2d_add(self->position, self->position, self->velocity);
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
	int maxBuildings;
	Room ***roomlist;
	Building **building_list;
	Building *building;
	SDL_bool isIntersectX, isIntersectY;
	Tile *tile;
	SDL_Rect testMoveX, testMoveY;
	Bool foundBot, foundAbove, foundLeft, foundRight;

	Bool isCollision = false;

	SDL_Rect tileCheck;


	foundBot = false;
	foundAbove = false;
	foundLeft = false;
	foundRight = false;

	roomlist = level_manager_get_current()->room_list;
	building_list = level_manager_get_current()->building_list;
	maxBuildings = level_manager_get_current()->max_buildings;

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

	if (ent->velocity.x == 0 && ent->velocity.y == 0)
	{
		return;
	}

	testMoveX = gfc_sdl_rect(ent->hitbox.x + ent->velocity.x, ent->hitbox.y, ent->hitbox.w, ent->hitbox.h);
	testMoveY = gfc_sdl_rect(ent->hitbox.x, ent->hitbox.y + ent->velocity.y, ent->hitbox.w, ent->hitbox.h);

	//check collision on all tiles
	for (roomX = 0; roomX < level_manager_get_current()->maxColumns; roomX++)
	{
		for (roomY = 0; roomY < level_manager_get_current()->maxRows; roomY++)
		{
			if (!roomlist[roomX][roomY])
			{
				continue;
			}

				

			//slog("Room x:%i, y:%i", roomX, roomY);

			Room *room = roomlist[roomX][roomY];
			
			if (!SDL_HasIntersection(&ent->hitbox, &room->roomBounds))
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


					//if collision found, check which side of the ent it was on and adjust accordingly


					isIntersectX = SDL_HasIntersection(&testMoveX, &tile->hitbox);
					isIntersectY = SDL_HasIntersection(&testMoveY, &tile->hitbox);

					if (!isIntersectX && !isIntersectY)
					{

						tileCheck = ent->hitbox;
						tileCheck.x+= 1;

						if (SDL_HasIntersection(&tileCheck, &tile->hitbox))
						{
							foundRight = true;
						}

						tileCheck = ent->hitbox;
						tileCheck.x -= 1;

						if (SDL_HasIntersection(&tileCheck, &tile->hitbox))
						{
							foundLeft = true;
						}

						tileCheck = ent->hitbox;
						tileCheck.y += 1;

						if (SDL_HasIntersection(&tileCheck, &tile->hitbox))
						{
							foundBot = true;
						}

						continue;
					}

					if (isIntersectX)
					{
						isCollision = true;
						

						if (ent->velocity.x > 0)
						{
							ent->position.x = tile->hitbox.x - ent->hitbox.w;
							ent->onRight = true;
							foundRight = true;
						}
						if (ent->velocity.x < 0)
						{
							ent->position.x = tile->hitbox.x + tile->hitbox.w;
							ent->onLeft = true;
							foundLeft = true;
						}

						ent->velocity.x = 0;
						ent->hitbox.x = ent->position.x;

					}
					if (isIntersectY)
					{
						isCollision = true;
						ent->platformCollide = false;

						if (ent->velocity.y > 0)
						{
							ent->position.y = tile->hitbox.y - ent->hitbox.h;
							ent->onGround = true;
							foundBot = true;
						}
						if (ent->velocity.y < 0)
						{
							ent->position.y = tile->hitbox.y + tile->hitbox.h;
						}

						ent->velocity.y = 0;
						ent->hitbox.y = ent->position.y;
					}

				}
			}
			
		}
	}

	for (int b = 0; b < maxBuildings; b++)
	{
		if (building_list[b] == NULL)
		{
			continue;
		}

		building = building_list[b];


		//if collision found, check which side of the ent it was on and adjust accordingly




		if (ent->velocity.y < 0)
		{
			continue;
		}

		
		isIntersectX = SDL_HasIntersection(&testMoveX, &building->hitbox);
		isIntersectY = SDL_HasIntersection(&testMoveY, &building->hitbox);

		if (entity_check_below_collision(ent->hitbox, building->hitbox) && isIntersectY)
		{
			ent->position.y = building->hitbox.y - ent->hitbox.h;
			ent->onGround = true;
			foundBot = true;
			ent->velocity.y = 0;
			ent->hitbox.y = ent->position.y;
		}

		if (!isIntersectX && !isIntersectY)
		{

			tileCheck = ent->hitbox;
			tileCheck.x += 1;

			if (SDL_HasIntersection(&tileCheck, &building->hitbox))
			{
				foundRight = true;
			}

			tileCheck = ent->hitbox;
			tileCheck.x -= 1;

			if (SDL_HasIntersection(&tileCheck, &building->hitbox))
			{
				foundLeft = true;
			}

			tileCheck = ent->hitbox;
			tileCheck.y += 1;

			if (SDL_HasIntersection(&tileCheck, &building->hitbox))
			{
				foundBot = true;
			}

			continue;
		}

		/*

		if (isIntersectX)
		{
			isCollision = true;


			if (ent->velocity.x > 0)
			{
				ent->position.x = building->hitbox.x - ent->hitbox.w;
				ent->onRight = true;
				foundRight = true;
			}
			if (ent->velocity.x < 0)
			{
				ent->position.x = building->hitbox.x + building->hitbox.w;
				ent->onLeft = true;
				foundLeft = true;
			}


			ent->velocity.x = 0;
			ent->hitbox.x = ent->position.x;


		}
		if (isIntersectY)
		{

			isCollision = true;


			if (ent->velocity.y > 0)
			{
				ent->position.y = building->hitbox.y - ent->hitbox.h;
				ent->onGround = true;
				foundBot = true;
			}
			if (ent->velocity.y < 0)
			{
				ent->platformCollide = false;
				//continue;
			}


			ent->velocity.y = 0;
			ent->hitbox.y = ent->position.y;


		}
		*/
	}

	//slog("Collide: %i", ent->platformCollide);

	if (!isCollision)
	{
		//ent->platformCollide = true;
		
		vector2d_add(ent->position, ent->position, ent->velocity);
		ent->hitbox.x = ent->position.x;
		ent->hitbox.y = ent->position.y;
	}
	if (!foundLeft)
	{
		ent->onLeft = false;
	}
	if (!foundRight)
	{
		ent->onRight = false;
	}
	if (!foundBot)
	{
		ent->onGround = false;
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
	Vector2D drawScale = camera_get_scale();
	Vector2D scalePos = vector2d(0, 0);

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
		if (ent->sprite == NULL && ent->animList == NULL)
		{
			
			return;// nothing to draw
		}
		offset = camera_get_offset();
		/*
		if (!camera_rect_on_screen(gfc_sdl_rect(ent->position.x, ent->position.y, ent->sprite->frame_w, ent->sprite->frame_h)))
		{
			//entity is off camera, skip
			//return;
		}
		*/
		drawPosition.x = ent->position.x + offset.x;
		drawPosition.y = ent->position.y + offset.y;

		if (ent->animList)
		{
			anim_list_draw(ent->animList, ent->animListLen, drawPosition);
			return;
		}
		if (ent->sprite)
		{
			
			//slog("X:%f, Y:%f", drawPosition.x, drawPosition.y);

			gf2d_sprite_draw(
				ent->sprite,
				drawPosition,
				&drawScale,
				NULL,
				NULL,
				NULL,
				NULL,
				(Uint32)ent->frame);
		}

		

		//test code to draw the hitboxes for an ent that has one

		/*

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
		*/

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
	return ent.y + ent.h > tile.y - 5 && ent.y + ent.h < tile.y + 5;
}
Bool entity_check_above_collision(SDL_Rect ent, SDL_Rect tile)
{
	return tile.y + tile.h > ent.y && ent.y > tile.y;
}

/*eol@eof*/