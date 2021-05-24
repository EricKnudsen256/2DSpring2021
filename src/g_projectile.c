#include "simple_logger.h"


#include "w_level.h"

#include "g_camera.h"
#include "g_projectile.h"


Projectile *projectile_spawn(int damage, int ttl, Entity *owner, Vector2D position, Vector2D velocity)
{
	Projectile *proj;

	proj = projectile_new();

	if (!proj)
	{
		slog("failed to create projectile");
		return NULL;
	}

	proj->owner = owner;

	proj->sprite = gf2d_sprite_load_all("assets/sprites/projectile.png", 24, 12, 1);


	proj->update = projectile_update;
	
	
	proj->hitbox.x = position.x;
	proj->hitbox.y = position.y;
	proj->hitbox.w = 24;
	proj->hitbox.h = 12;

	vector2d_copy(proj->position, position);
	vector2d_copy(proj->velocity, velocity);

	proj->frameRate = 0.1;
	proj->frameCount = 1;

	proj->ttl = ttl;
	proj->spawnedTime = SDL_GetTicks();

	proj->_inuse = true;

	proj->damage = damage;

	return proj;
}

Projectile *projectile_new()
{
	int i;
	if (projectile_manager.projectile_list == NULL)
	{
		slog("projectile system does not exist");
		return NULL;
	}
	for (i = 0; i < projectile_manager.max_projectiles; i++)
	{
		if (projectile_manager.projectile_list[i]._inuse)continue;// someone else is using this one
		memset(&projectile_manager.projectile_list[i], 0, sizeof(Projectile));
		projectile_manager.projectile_list[i]._inuse = 1;
		return &projectile_manager.projectile_list[i];
	}
	slog("no free projectiles available");
	return NULL;
}

void projectile_manager_init(Uint32 max_projectiles)
{
	if (max_projectiles == 0)
	{
		slog("cannot allocate 0 projectiles!");
		return;
	}
	if (projectile_manager.projectile_list != NULL)
	{
		projectile_manager_free();
	}
	projectile_manager.projectile_list = (Projectile *)gfc_allocate_array(sizeof (Projectile), max_projectiles);
	if (projectile_manager.projectile_list == NULL)
	{
		slog("failed to allocate projectile list!");
		return;
	}
	projectile_manager.max_projectiles = max_projectiles;
	slog("projectile system initialized");
}

void projectile_manager_free()
{
	if (projectile_manager.projectile_list != NULL)
	{
		free(projectile_manager.projectile_list);
	}
	memset(&projectile_manager, 0, sizeof(ProjectileManager));
	slog("projectile system closed");
}


void projectile_manager_think_projectiles()
{
	int i;
	if (projectile_manager.projectile_list == NULL)
	{
		slog("projectile system does not exist");
		return;
	}
	for (i = 0; i < projectile_manager.max_projectiles; i++)
	{
		if (projectile_manager.projectile_list[i]._inuse == 0)continue;
		if (projectile_manager.projectile_list[i].think != NULL)
		{
			projectile_manager.projectile_list[i].think(&projectile_manager.projectile_list[i]);
		}
	}
}

void projectile_update(Projectile *self)
{
	if (!self)return;

	if (self->spawnedTime + self->ttl < SDL_GetTicks())
	{
		projectile_free(self);
	}

	// DO ANY GENERIC UPDATE CODE

	//THIS LINE UPDATES POSITION BASED ON VELOCITY V2
	vector2d_add(self->position, self->position, self->velocity);

	self->frame += self->frameRate;
	if (self->frame >= self->frameCount)self->frame = 0;

	// IF THERE IS A CUSTOM UPDATE, DO THAT NOW
	//if (self->update)self->update(self);

	if (&self->hitbox)
	{
		self->hitbox.x = self->position.x;
		self->hitbox.y = self->position.y;
	}

	

}

void projectile_manager_update_projectiles()
{
	int i;
	if (projectile_manager.projectile_list == NULL)
	{
		slog("projectile system does not exist");
		return;
	}
	for (i = 0; i < projectile_manager.max_projectiles; i++)
	{
		if (projectile_manager.projectile_list[i]._inuse == 0)continue;
		if (projectile_manager.projectile_list[i].update != NULL)
		{
			projectile_manager.projectile_list[i].update(&projectile_manager.projectile_list[i]);
		}
	}

}


void projectile_manager_check_collisions()
{
	int i;
	if (projectile_manager.projectile_list == NULL)
	{
		slog("projectile system does not exist");
		return;
	}
	for (i = 0; i < projectile_manager.max_projectiles; i++)
	{
		if (projectile_manager.projectile_list[i]._inuse == 0)continue;
		if (projectile_manager.projectile_list[i].check_collision != NULL)
		{
			//projectile_manager.projectile_list[i].check_collision(&projectile_manager.projectile_list[i]);
			continue;
		}
		projectile_check_collisions(&projectile_manager.projectile_list[i]);
	}
}

void projectile_manager_draw_projectiles()
{
	int i;
	if (projectile_manager.projectile_list == NULL)
	{
		slog("projectile system does not exist");
		return;
	}
	for (i = 0; i < projectile_manager.max_projectiles; i++)
	{
		if (projectile_manager.projectile_list[i]._inuse == 0)continue;
		projectile_draw(&projectile_manager.projectile_list[i]);
	}
}

void projectile_check_collisions(Projectile *proj)
{
	/*
	int i;
	Level *level;
	SDL_bool isIntersect;
	Tile *tile;
	Entity *ent;

	EntityManager entManager = entity_manager_get_manager();

	level = level_manager_get_current();

	if (!proj)
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

		//if collision found, check which side of the ent it was on and adjust accordingly

		isIntersect = SDL_HasIntersection(&proj->hitbox, &level->tileArray[i]->hitbox);

		if (isIntersect)
		{
			//slog("proj x:%f, y:%f\nent x:%f, y:%f", proj->position.x, proj->position.y, tile->hitbox.x, tile->hitbox.y);
			projectile_free(proj);
			return;
		}
	}

	for (i = 0; i < entManager.max_entities; i++)
	{
		if (!entManager.entity_list[i]._inuse)continue;

		ent = &entManager.entity_list[i];
		isIntersect = SDL_HasIntersection(&proj->hitbox, &ent->hitbox);

		if (!isIntersect)
		{
			continue;
		}

		if (!proj->owner->isPlayer && !ent->isPlayer)
		{
			continue;
		}

		else if (proj->owner->isPlayer && ent->isPlayer)
		{
			continue;
		}


		enemy_damage(ent, proj->damage);
		projectile_free(proj);
	}

	*/

}

void projectile_free(Projectile *proj)
{
	if (!proj)
	{
		slog("cannot free a NULL projectile");
		return;
	}

	gf2d_sprite_free(proj->sprite);
	proj->sprite = NULL;
	proj->_inuse = 0;
}

void projectile_draw(Projectile *proj)
{

	Vector2D drawPosition, offset;
	Vector2D drawScale = camera_get_scale();

	if (!proj)
	{
		slog("cannot draww a NULL entity");
		return;
	}
	if (proj->draw)
	{
		proj->draw(proj);
	}
	else
	{
		if (proj->sprite == NULL)
		{
			return;// nothing to draw
		}
		offset = camera_get_offset();

		drawPosition.x = proj->position.x + offset.x;
		drawPosition.y = proj->position.y + offset.y;


		gf2d_sprite_draw(
			proj->sprite,
			drawPosition,
			&drawScale,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);

		//test code to draw the hitboxes for an ent that has one

		if (!&proj->hitbox)
		{

			SDL_Rect tempDraw;

			tempDraw.x = proj->hitbox.x + offset.x;
			tempDraw.y = proj->hitbox.y + offset.y;
			tempDraw.w = proj->hitbox.w;
			tempDraw.h = proj->hitbox.h;

			SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 255, 0, 0, 255);

			SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &tempDraw);

		}

	}
	
}

