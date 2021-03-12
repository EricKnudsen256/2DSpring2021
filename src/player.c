#include "simple_logger.h"

#include "player.h"
#include "camera.h"

void player_update(Entity *self);
void player_think(Entity *self);

Entity *player_spawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = player_update;
	ent->think = player_think;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->hitbox.x = 0;
	ent->hitbox.y = 0;
	ent->hitbox.w = 128;
	ent->hitbox.h = 128;


	return ent;
}


void player_update(Entity *self)
{
	Vector2D camera;
	Vector2D cameraSize;

	if (!self)return;

	cameraSize = camera_get_dimensions();
	camera.x = (self->position.x + 64) - (cameraSize.x * 0.5);
	camera.y = (self->position.y + 64) - (cameraSize.y * 0.5);
	camera_set_position(camera);
}

void player_think(Entity *self)
{
	const Uint8 *keys;
	Vector2D camera, gravity;
	float angle;
	int mx, my;
	if (!self)return;
	keys = SDL_GetKeyboardState(NULL);
	SDL_GetMouseState(&mx, &my);
	camera = camera_get_position();
	mx += camera.x;
	my += camera.y;

	//put loop to check for floor collision on gravity

	self->velocity.y += 1;
	
	self->velocity.y += .1;

	if (self->velocity.y > 2)
	{
		self->velocity.y = 2;
	}


	if (keys[SDL_SCANCODE_D])
	{
		self->velocity.x = 2;
	}
	else if (keys[SDL_SCANCODE_A])
	{
		self->velocity.x = -2;
	}
	else
	{
		self->velocity.x = 0;
	}

}


/**/