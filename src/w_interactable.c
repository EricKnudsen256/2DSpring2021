#include "simple_logger.h"

#include "gfc_types.h"
#include "gfc_vector.h"

#include "g_camera.h"

#include "e_entity.h"

#include "w_interactable.h"



Interactable *interactable_new(int width, int height, Vector2D *position, Vector2D *gridPos)
{
	Interactable *interact;


	if (!position && !gridPos)
	{
		slog("No position given, must supply position or gridpos");
		return NULL;
	}

	interact = (Interactable*)malloc(sizeof(Interactable));

	memset(interact, 0, sizeof(Interactable));

	if (!interact)
	{
		slog("Interactable not created properly");
	}


	else if (position && !gridPos)
	{
		interact->pos.x = position->x;
		interact->pos.y = position->y;
		interact->gridPos = vector2d((int)interact->pos.x / 32, (int)interact->pos.y / 32);

	}
	else if (!position && gridPos)
	{
		interact->gridPos.x = gridPos->x;
		interact->gridPos.y = gridPos->y;
		interact->pos = vector2d((int)interact->gridPos.x * 32, (int)interact->gridPos.y * 32);

	}
	else if (position && gridPos)
	{
		interact->gridPos.x = gridPos->x;
		interact->gridPos.y = gridPos->y;
		interact->pos.x = position->x;
		interact->pos.y = position->y;
	}

	interact->interactionBox = gfc_sdl_rect(interact->pos.x, interact->pos.y, width, height);

	//set oninteract in item that creates interactable

	//slog("Pos.x:%f, Pos.y:%f, GridPos.x:%f, GridPos.y:%f", interact->pos.x, interact->pos.y, interact->gridPos.x, interact->gridPos.y);


	return interact;
}


void interactable_update(Interactable *interact)
{
	Entity *player = entity_manager_get_player_ent();

	if (SDL_IntersectRect(&interact->interactionBox, &player->hitbox, &interact->intersectAmount))
	{
		interact->playerInside = true;
	}
	else
	{
		interact->playerInside = false;
	}
}


void interactable_draw(Interactable *interact)
{
	Vector2D offset;
	Vector2D drawScale = camera_get_scale();
	Vector2D scalePos = vector2d(0, 0);

	SDL_Rect drawRect;

	if (!interact)
	{
		slog("cannot draww a NULL rock");
		return;
	}
	else
	{
		offset = camera_get_offset();
		if (!camera_rect_on_screen(gfc_sdl_rect(interact->pos.x, interact->pos.y, interact->pos.x + 128, interact->pos.y + 64)))
		{
			//entity is off camera, skip
			//return;
		}


		drawRect = gfc_sdl_rect((interact->pos.x + offset.x) * drawScale.x, 
			(interact->pos.y + offset.y) * drawScale.y, 
			interact->interactionBox.w * drawScale.x, 
			interact->interactionBox.h * drawScale.y);

		SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 255, 0, 0, 255);

		if (interact->playerInside)
		{
			SDL_RenderFillRect(gf2d_graphics_get_renderer(), &drawRect);
		}
		else
		{
			SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &drawRect);
		}
	

		//slog("Drawn");
	}
}


void interactable_free(Interactable *interact)
{
	if (!interact)return;// nothing to do

	memset(interact, 0, sizeof(Interactable));

	interact->_inuse = false;
	interact = NULL;
}

