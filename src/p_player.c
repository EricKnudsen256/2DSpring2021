#include "simple_logger.h"

#include "g_projectile.h"
#include "g_camera.h"
#include "g_globals.h"

#include "p_player.h"

#include "w_level.h"

void player_update(Entity *self);
void player_think(Entity *self);


void player_inventory_init(Uint32 max_items)
{
	if (max_items == 0)
	{
		slog("cannot create player inventory with 0 items");
		return;
	}
	if (inventory.item_list != NULL)
	{
		player_inventory_free();
	}
	inventory.item_list = (Item *)gfc_allocate_array(sizeof (Item), max_items);
	if (inventory.item_list == NULL)
	{
		slog("failed to allocate inventory!");
		return;
	}
	inventory.max_items = max_items;
	slog("inventory initialized");
}

void player_inventory_add_item(Item *item)
{
	if (!item)
	if (inventory.item_list == NULL)
	{
		slog("Inventory not created, call player_inventory_init first");
		return;
	}

	for (int i = 0; i < inventory.max_items; i++)
	{
		if (inventory.item_list[i] == NULL)
		{
			inventory.item_list[i] = item;
			return;
		}
	}
}

Item *player_inventory_get_item(int slot)
{
	if (slot < inventory.max_items)
	{
		return inventory.item_list[slot];
	}
	return NULL;
}

void player_inventory_free()
{
	if (inventory.item_list != NULL)
	{
		free(inventory.item_list);
	}
	memset(&inventory, 0, sizeof(PlayerInventory));
	slog("inventory deleted");
}

void player_inventory_slog()
{

	slog("maxItems: %i", player_inventory_get_max());
	for (int i = 0; i < inventory.max_items; i++)
	{


		if (!inventory.item_list[i])
		{
			slog("Slot %i: empty", i);
		}
		else
		{
			slog("Slot %i: %s:%i", i, inventory.item_list[i]->name, inventory.item_list[i]->stackNum);
		}
	}
}

void player_inventory_swap(int slot1, int slot2)
{
	Item *temp;
	if (slot1 < inventory.max_items && slot2 < inventory.max_items)
	{
		slog("Slot1: %i, Slot2: %i", slot1, slot2);

		temp = inventory.item_list[slot1];
		inventory.item_list[slot1] = inventory.item_list[slot2];
		inventory.item_list[slot2] = temp;
		player_inventory_slog();
		return;
	}

	slog("Slots not in possible range. Slot1: %i, Slot2: %i", slot1, slot2);
	return;

}

Uint32 player_inventory_get_max()
{
	return inventory.max_items;
}




Entity *player_spawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_all("assets/sprites/newPlayer.png", 64, 64, 1);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = player_update;
	ent->think = player_think;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->hitbox.x = 0;
	ent->hitbox.y = 0;
	ent->hitbox.w = 64;
	ent->hitbox.h = 64;

	ent->isPlayer = true;
	ent->health = 1000000;
	ent->attackDamage = 100;

	ent->max_sounds = 16;

	player_load_sounds(ent);

	return ent;
}


void player_load_sounds(Entity *self)
{
	self->soundList = (Sound *)gfc_allocate_array(sizeof(Sound), self->max_sounds);

	self->soundList[1] = gfc_sound_load("assets/audio/animal melee sound.wav", 1, 2);
	self->soundList[2] = gfc_sound_load("assets/audio/melee sound.wav", 1, 2);
	self->soundList[3] = gfc_sound_load("assets/audio/sword sound.wav", 1, 2);
	self->soundList[4] = gfc_sound_load("assets/audio/178872__hanbaal__bow.wav", 1, 2);
}

void player_update(Entity *self)
{
	Vector2D camera;
	Vector2D cameraSize;

	if (!self)return;

	//slog("Player: x:%f, y:%f", self->position.x, self->position.y);

	cameraSize = camera_get_dimensions();
	camera.x = (self->position.x + 64) - (cameraSize.x * 0.5);
	camera.y = (self->position.y + 64) - (cameraSize.y * 0.5);
	camera_set_position(camera);

	if (self->health <= 0)
	{
		player_die(self);
	}
	if (self->facing < 0 || self->facing > 5)
	{
		slog("player cannot face this direction, defualting to right");
		self->facing = 3;
	}


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

	//slog("onGround:%i", self->onGround);


	//put loop to check for floor collision on gravity

	if (!self->onGround)
	{
		self->velocity.y += .075;
	}



	//edit this to change max fall speed
	if (self->velocity.y > 4)
	{
		self->velocity.y = 4;
	}


	if (self->onGround)
	{
		self->doubleJumped = false;
	}

	if ((self->onRight || self->onLeft) && !self->onGround)
	{
		if (self->velocity.y > 1)
		{
			self->velocity.y = 1;
		}
	}

	//check for any keys pressed
	if (keys[SDL_SCANCODE_D])
	{
		self->facing = 3;
		if (self->onRight == false)
		{
			self->velocity.x = 3;
			self->onLeft = false;
		}
		if (keys[SDL_SCANCODE_LSHIFT])
		{
			self->velocity.x *= 1.5;;
		}
		
	}
	else if (keys[SDL_SCANCODE_A])
	{
		self->facing = 1;
		if (self->onLeft == false)
		{
			self->velocity.x = -3;
			self->onRight = false;
		}
		if (keys[SDL_SCANCODE_LSHIFT])
		{
			self->velocity.x *= 1.5;;
		}
	}

	else
	{
		self->onRight = false;
		self->onLeft = false;
		self->velocity.x = 0;
	}


	if (keys[SDL_SCANCODE_SPACE] && player_is_allowed_jump(self))
	{
		//if (self->onGround == true || self->doubleJumped == false)
		if (true)
		{
			self->velocity.y = -5;
			self->position.y -= .1;
		
			if (!self->onGround)
			{
				self->doubleJumped = true;
			}

			self->onGround = false;
			self->lastJump = SDL_GetTicks();

			//slog("onGround: %i", self->onGround);
			//slog("doubleJumped: %i", self->doubleJumped);
		}
		else if (self->onRight)
		{
			self->velocity.y = -5;
			self->position.y -= .1;

			if (!self->onGround)
			{
				self->doubleJumped = true;
			}

			self->onGround = false;
			self->lastJump = SDL_GetTicks();
		}
		else if (self->onLeft)
		{
			self->velocity.y = -5;
			self->position.y -= .1;

			if (!self->onGround)
			{
				self->doubleJumped = true;
			}

			self->onGround = false;
			self->lastJump = SDL_GetTicks();
		}
	}

	/*
	if (self->position.x <= -60)
	{
		Level *level = level_manager_get_current();


		if (self->position.y <= 180)
		{
			level_change(self, 1, -1);

		}
		else if (self->position.y >= 900)
		{
			level_change(self, 1, 1);
		}
		else
		{
			level_change(self, 1, level->levelPos.y);
		}
		

		
		
	}

	if (self->position.x >= 1532)
	{

		Level *level = level_manager_get_current();

		if (self->position.y <= 180)
		{
			level_change(self, 3, -1);
		}
		else if (self->position.y >= 900)
		{
			level_change(self, 3, 1);
		}
		else
		{
			level_change(self, 3, level->levelPos.y);
			
		}
	}
	*/
}

void player_attack(Entity *self)
{
	SDL_Rect attackbox;
	Entity *ent;
	EntityManager entManager;
	int i;

	entManager = entity_manager_get_manager();

	if (self->facing == 1)
	{
		gfc_rect_set(attackbox, self->hitbox.x - 50, self->hitbox.y, 50, self->hitbox.h);
	}
	else if (self->facing == 3)
	{
		gfc_rect_set(attackbox, self->hitbox.x + self->hitbox.w, self->hitbox.y, 50, self->hitbox.h);
	}
	else
	{
		//slog("attack direction not found");
		gfc_rect_set(attackbox, self->hitbox.x + self->hitbox.w, self->hitbox.y, 50, self->hitbox.h);
	}

	if (SDL_GetTicks() >= self->lastAttack + 500)
	{
		int sound = random_int_range(1, 3);
		gfc_sound_play(self->soundList[sound], 0, .05, -1, -1);

		for (i = 0; i < entManager.max_entities; i++)
		{

			if (entManager.entity_list[i]._inuse == 0)continue;

			if (entManager.entity_list[i].isPlayer == 1)continue;

			ent = &entManager.entity_list[i];
			if (SDL_HasIntersection(&attackbox, &ent->hitbox))
			{
				ent->health -= self->attackDamage;
			}
		}

		self->lastAttack = SDL_GetTicks();
	}
	
}

void player_ranged(Entity *self)
{
	Entity *ent;
	EntityManager entManager;
	int i;

	Vector2D position, velocity;
	vector2d_copy(position, self->position);

	velocity.y = 0;

	entManager = entity_manager_get_manager();


	if (SDL_GetTicks() >= self->lastAttack + 500)
	{
		gfc_sound_play(self->soundList[4], 0, .05, -1, -1);

		if (self->facing == 1)
		{
			velocity.x = -8;
			projectile_spawn(10, 2000, self, position, velocity);
		}
		else if (self->facing == 3)
		{
			velocity.x = 8;
			projectile_spawn(10, 2000, self, position, velocity);
		}
		else
		{
			projectile_spawn(10, 2000, self, position, velocity);

		}

		self->lastAttack = SDL_GetTicks();
	}

}

Bool player_is_allowed_jump(Entity *self)
{
	Uint32 currentTime;
	currentTime = SDL_GetTicks();

	if (currentTime >= self->lastJump + 350)
	{
		return true;
	}
	return false;


}

void player_die(Entity *self)
{
	if (!self)return;// nothing to do

	gf2d_sprite_free(self->sprite);

	gfc_rect_set(self->hitbox, -1, -1, -1, -1);
	self->position.x = -1;
	self->position.y = -1;

	self->sprite = NULL;

	self->_inuse = false;
	self = NULL;
}

void player_damage(Entity *player, int damage)
{
	if (damage == 0)
	{
		return;
	}

	if (SDL_GetTicks() >= player->lastDamaged + 1000)
	{
		player->health -= damage;
		
		if (player->health <= 0)
		{
			player_die(player);
		}

		player->lastDamaged = SDL_GetTicks();
	}
}


/**/