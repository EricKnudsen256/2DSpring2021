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

	//load anims from list

	Anim **animTemplate = building_list_get_by_name("itemPipeSlow")->animList;
	int	templateLength = building_list_get_by_name("itemPipeSlow")->maxAnims;

	if (strcmp(direction, "left") == 0)
	{
		pipe->animList[0] = anim_list_get_by_name(animTemplate, "left_pipe_idle", templateLength);
		pipe->animList[0]->_current = true;
	}
	else if (strcmp(direction, "right") == 0)
	{
		pipe->animList[0] = anim_list_get_by_name(animTemplate, "right_pipe_idle", templateLength);
		pipe->animList[0]->_current = true;
	}
	else if (strcmp(direction, "down") == 0)
	{
		pipe->animList[0] = anim_list_get_by_name(building_list_get_by_name("itemPipeSlow")->animList, "down_pipe_idle", building_list_get_by_name("itemPipeSlow")->maxAnims);
		pipe->animList[0]->_current = true;
	}
	else if (strcmp(direction, "up") == 0)
	{
		pipe->animList[0] = anim_list_get_by_name(building_list_get_by_name("itemPipeSlow")->animList, "up_pipe_idle", building_list_get_by_name("itemPipeSlow")->maxAnims);
		pipe->animList[0]->_current = true;
	}

	pipe->building->parent = pipe;
	pipe->building->update = pipe_update;

	return pipe;
}

void pipe_update(Pipe *pipe)
{
	anim_list_update(pipe->animList, pipe->animListLen);
}

void pipe_draw(Pipe *pipe)
{

}

void pipe_free(Pipe *pipe)
{

}