#include <stdlib.h>
#include "simple_logger.h"

#include "camera.h"
#include "entity.h"
#include "level.h"

typedef struct
{
	Entity *entity_list;
	Uint32  max_entities;
}EntityManager;

static EntityManager entity_manager = { 0 };

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
	atexit(entity_manager_free);
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
	int i;
	Level *level;
	SDL_bool isIntersect;
	Tile *tile;
	SDL_Rect testMove;

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

		if (!level->tileArray[i])
		{
			continue;
		}

		tile = level->tileArray[i];

		//if collision found, check which side of the ent it was on and adjust accordingly

		isIntersect = SDL_HasIntersection(&ent->hitbox, &level->tileArray[i]->hitbox);

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
				ent->velocity.y = 0;
				ent->onGround = true;
			}
		}
		//if above
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
		if (entity_check_above_collision(ent->hitbox, tile->hitbox) && ent->velocity.y < 0)
		{

			testMove.y = tile->hitbox.y + tile->hitbox.h;
			if (!entity_check_above_collision(testMove, tile->hitbox))
			{
				slog("collision on top");
				ent->position.y = testMove.y;
				ent->hitbox.y = testMove.y;
				ent->velocity.y = 0;
			}
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
			if (!entity_check_left_collision(testMove, tile->hitbox))
			{
				//slog("collision on left");
				ent->position.x = testMove.x;
				ent->hitbox.x = testMove.x;
				ent->velocity.x = 0;
			}
		}


		//if on right
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
		if (entity_check_right_collision(ent->hitbox, tile->hitbox) && ent->velocity.x > 0)
		{
			
			testMove.x = tile->hitbox.x - testMove.w;
			if (!entity_check_left_collision(testMove, tile->hitbox))
			{
				//slog("collision on right");
				ent->position.x = testMove.x;
				ent->hitbox.x = testMove.x;
				ent->velocity.x = 0;
			}
		}
	}
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
			return;
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
	return tile.y + tile.h > ent.y && ent.y < tile.y;
}

/*eol@eof*/