#include "simple_logger.h"

#include "b_pipe.h"

#include "w_level.h"


Pipe *pipe_new(Vector2D gridPos, Vector2D size, char *direction)
{
	Pipe *pipe;

	pipe = (Pipe*)malloc(sizeof(Pipe));

	memset(pipe, 0, sizeof(Pipe));

	pipe->building = level_building_new(gridPos, size, level_manager_get_current());

	pipe->animListLen = 2;
	pipe->animList = anim_list_new(2);

	if (strcmp(direction, "left") == 0)
	{
		pipe->animList[0] = anim_new("assets/sprites/building/pipes/LPipeSheet.png", "left_pipe_idle", ANIM_IDLE, 0, 15, 15, 64, 64, 4);
		pipe->animList[0]->_current = 1;
	}
	else if (strcmp(direction, "right") == 0)
	{
		pipe->animList[0] = anim_new("assets/sprites/building/pipes/RPipeSheet.png", "right_pipe_idle", ANIM_IDLE, 0, 15, 15, 64, 64, 4);
		pipe->animList[0]->_current = 1;
	}
	else if (strcmp(direction, "down") == 0)
	{
		pipe->animList[0] = anim_new("assets/sprites/building/pipes/DPipeSheet.png", "downt_pipe_idle", ANIM_IDLE, 0, 15, 15, 64, 64, 4);
		pipe->animList[0]->_current = 1;
	}
	else if (strcmp(direction, "up") == 0)
	{
		pipe->animList[0] = anim_new("assets/sprites/building/pipes/UPipeSheet.png", "up_pipe_idle", ANIM_IDLE, 0, 15, 15, 64, 64, 4);
		pipe->animList[0]->_current = 1;
	}

	pipe->building->parent = pipe;
	pipe->building->update = pipe_update;

	return pipe;
}

void pipe_update(Pipe *pipe)
{

}

void pipe_draw(Pipe *pipe)
{

}

void pipe_free(Pipe *pipe)
{

}