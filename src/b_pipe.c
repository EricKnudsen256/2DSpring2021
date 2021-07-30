#include "simple_logger.h"

#include "b_pipe.h"

#include "g_anim.h"
#include "g_camera.h"

#include "w_level.h"


Pipe *pipe_new(Vector2D gridPos, Vector2D size, int direction)
{
	Pipe *pipe;

	pipe = (Pipe*)malloc(sizeof(Pipe));

	memset(pipe, 0, sizeof(Pipe));

	pipe->building = level_building_new(gridPos, size, level_manager_get_current());

	pipe->animListLen = 2;
	pipe->animList = anim_list_new(2);

	//load anims from list

	Anim **animTemplate = building_list_get_by_name("itemPipeSlow")->animList;
	int	templateLength = building_list_get_by_name("itemPipeSlow")->maxAnims;

	if (direction == 0)
	{
		pipe->animList[0] = anim_list_get_by_name(animTemplate, "left_pipe_idle", templateLength);
		pipe->animList[0]->_current = true;
	}
	else if (direction == 1)
	{
		pipe->animList[0] = anim_list_get_by_name(animTemplate, "up_pipe_idle", templateLength);
		pipe->animList[0]->_current = true;
	}
	else if (direction == 2)
	{
		pipe->animList[0] = anim_list_get_by_name(animTemplate, "right_pipe_idle", templateLength);
		pipe->animList[0]->_current = true;

	}
	else if (direction == 3)
	{
		pipe->animList[0] = anim_list_get_by_name(animTemplate, "down_pipe_idle", templateLength);
		pipe->animList[0]->_current = true;
	}

	/*
	slog("animName:%s", pipe->animList[0]->animName);
	slog("animType:%i", pipe->animList[0]->animType);
	slog("startframe:%i", pipe->animList[0]->startframe);
	slog("endframe:%i", pipe->animList[0]->endframe);
	slog("framerate:%i", pipe->animList[0]->framerate);
	slog("frameWidth:%i", pipe->animList[0]->frameWidth);
	slog("frameHeight:%i", pipe->animList[0]->frameHeight);
	slog("fpl:%i", pipe->animList[0]->fpl);
	*/
	


	pipe->building->parent = pipe;
	pipe->building->update = pipe_update;
	pipe->building->draw = pipe_draw;
	pipe->building->type = BUILDING_PIPE;

	//slog("Drawpos.x:%f, Drawpos.y:%f", pipe->building->position.x, pipe->building->position.x);

	return pipe;
}

void pipe_update(Pipe *pipe)
{
	anim_list_update(pipe->animList, pipe->animListLen);
}

void pipe_draw(Pipe *pipe)
{
	Vector2D drawPosition, offset;
	Vector2D drawScale = camera_get_scale();

	if (!pipe)
	{
		slog("cannot draw a NULL pipe");
		return;
	}
	else
	{
		offset = camera_get_offset();

		drawPosition.x = pipe->building->position.x + offset.x;
		drawPosition.y = pipe->building->position.y + offset.y;

		
		
		anim_list_draw(pipe->animList, pipe->animListLen, drawPosition);
	}
	if (pipe->building->hitbox.w > 0 && pipe->building->hitbox.h > 0)
	{
		SDL_Rect tempDraw;

		tempDraw.x = (pipe->building->hitbox.x + offset.x) * drawScale.x;
		tempDraw.y = (pipe->building->hitbox.y + offset.y) * drawScale.y;
		tempDraw.w = pipe->building->hitbox.w * drawScale.x;
		tempDraw.h = pipe->building->hitbox.h * drawScale.y;

		SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 0, 255, 0, 255);

		SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &tempDraw);
	}
}

void pipe_free(Pipe *pipe)
{

}