#include "simple_logger.h"

#include "e_enemy.h"
#include "e_walker.h"
#include "e_flyer.h"
#include "e_archer.h"
#include "e_bouncer.h"

#include "g_camera.h"
#include "g_globals.h"
#include "g_random.h"

void enemy_update(Entity *self);
void enemy_think(Entity *self);

Entity *enemy_spawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for selected enemy");
		return NULL;
	}
	//ent->sprite = gf2d_sprite_load_all("assets/sprites/enemy1.png", 64, 64, 1);
	vector2d_copy(ent->position, position);

	return ent;
}

Entity *enemy_spawn_random(Vector2D position)
{
	Entity *ent;

	int rnd = random_int_range(0, 3);

	if (rnd == 0)
	{
		ent = walker_spawn(position);
	}
	else if (rnd == 1)
	{
		ent = flyer_spawn(position);
	}
	else if (rnd == 2)
	{
		ent = archer_spawn(position);
	}
	else
	{
		ent = bouncer_spawn(position);
	}

	if (!ent)
	{
		slog("failed to create entity for selected enemy");
		return NULL;
	}
	//ent->sprite = gf2d_sprite_load_all("assets/sprites/enemy1.png", 64, 64, 1);
	vector2d_copy(ent->position, position);

	slog("spawn x:%f, spawn y:%f", position.x, position.y);
	return ent;
}


void enemy_update(Entity *self)
{
	if (!self)return;

	if (self->health <= 0)
	{
		enemy_die(self);
	}

}

void enemy_think(Entity *self)
{

}

Bool enemy_check_player_collision(Entity *ent)
{
	Entity *player;
	SDL_bool isIntersect;


	player = entity_manager_get_player_ent();


	if (!ent)
	{
		slog("No entity provided");
		return NULL;
	}
	if (!player)
	{
		slog("No player found");
		return NULL;
	}

	isIntersect = SDL_HasIntersection(&ent->hitbox, &player->hitbox);

	if (!isIntersect)
	{
		return NULL;
	}

	if (ent->contactDamage)
	{
		player_damage(player, ent->contactDamage);
	}

	return true;
	

}

void enemy_die(Entity *self)
{
	if (!self)return;// nothing to do



	gf2d_sprite_free(self->sprite);

	self->sprite = NULL;

	self->_inuse = false;
	self = NULL;


}

void enemy_damage(Entity *ent, int damage)
{

	ent->health -= damage;

	if (ent->health <= 0)
	{
		enemy_die(ent);
	}

}

void enemy_draw(Entity *ent)
{
	Vector2D offset, drawPosition;

	SDL_Rect healthBG, health;


	if (ent->sprite == NULL)
	{
		return;// nothing to draw
	}
	offset = camera_get_offset();
	if (!camera_rect_on_screen(gfc_sdl_rect((Sint32)ent->position.x, (Sint32)ent->position.y, (Sint32)ent->sprite->frame_w, (Sint32)ent->sprite->frame_h)))
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

	//drawing health bars

	gfc_rect_set(healthBG, ent->position.x + offset.x, ent->position.y - 20 + offset.y, ent->hitbox.w, 15);

	SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 215, 215, 215, 255);

	SDL_RenderFillRect(gf2d_graphics_get_renderer(), &healthBG);


	gfc_rect_set(healthBG, ent->position.x + offset.x, ent->position.y - 20 + offset.y, ent->hitbox.w * ((float)ent->health / (float)ent->startingHealth), 15);

	SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 232, 67, 67, 255);

	SDL_RenderFillRect(gf2d_graphics_get_renderer(), &healthBG);


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
	*/

	if (&ent->hitbox2)
	{
		//slog("x:%i, y:%i, w:%i, h:%i", ent->hitbox2.x, ent->hitbox2.y, ent->hitbox2.w, ent->hitbox2.h);
		SDL_Rect tempDraw;

		tempDraw.x = ent->hitbox2.x + offset.x;
		tempDraw.y = ent->hitbox2.y + offset.y;
		tempDraw.w = ent->hitbox2.w;
		tempDraw.h = ent->hitbox2.h;

		gfc_rect_set(tempDraw, ent->hitbox2.x + offset.x, ent->hitbox2.y + offset.y, ent->hitbox2.w, ent->hitbox2.h);

		SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 0, 0, 0, 255);

		SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &tempDraw);
	}

}



/**/