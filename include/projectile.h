#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"

typedef struct Projectile_s
{
	Bool		_inuse;
	Entity		*owner;
	Vector2D	position;
	Vector2D	velocity;
	SDL_Rect	hitbox;
	Sprite		*sprite;
	int			damage;
	int			ttl;
	int			spawnedTime;

	float       frame;
	float       frameRate;
	int         frameCount;

	void(*update)(struct Projectile_s *self);
	void(*think)(struct Projectile_s *self);
	void(*draw)(struct Projectile_s *self);
	void(*free)(struct Projectile_s *self);
	void(*check_collision)(struct Projectile_s *self);
	void       *data;


}Projectile;


typedef struct
{
	Projectile *projectile_list;
	Uint32  max_projectiles;
}ProjectileManager;

static ProjectileManager projectile_manager = { 0 };


Projectile *projectile_spawn(int damage, int ttl, Entity *owner, Vector2D position, Vector2D velocity);

Projectile *projectile_new();


void projectile_manager_init(Uint32 max_projectiles);


void projectile_manager_free();



void projectile_manager_think_projectiles();


void projectile_manager_update_projectiles();


void projectile_manager_check_collisions();


void projectile_manager_draw_projectiles();


void projectile_update(Projectile *self);


void projectile_check_collisions(Projectile *proj);


void projectile_free(Projectile *proj);


void projectile_draw(Projectile *proj);



#endif