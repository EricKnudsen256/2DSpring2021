#include "simple_logger.h"

#include "projectile.h"


Projectile *projectile_spawn(int damage, Entity *owner, Vector2D position)
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
	//proj->update = projectile_update;
	//proj->think = projectile_think;
	proj->hitbox.x = 0;
	proj->hitbox.y = 0;
	proj->hitbox.w = 64;
	proj->hitbox.h = 64;


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
	slog("no free entities available");
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
	atexit(projectile_manager_free);
	slog("entity system initialized");
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
	// DO ANY GENERIC UPDATE CODE

	//THIS LINE UPDATES POSITION BASED ON VELOCITY V2
	vector2d_add(self->position, self->position, self->velocity);

	// IF THERE IS A CUSTOM UPDATE, DO THAT NOW
	if (self->update)self->update(self);

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
		//projectile_check_collisions(&projectile_manager.projectile_list[i]);
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
		//projectile_draw(&projectile_manager.projectile_list[i]);
	}
}

