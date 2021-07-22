#ifndef __H_PIPE__
#define __H_PIPE__

#include "e_drop.h"

#include "w_building.h"

typedef struct Pipe_s
{
	Building *building;

	Item *currentItem;


	Anim **animList;
	int animListLen;


	
	//other things needed in struct are just the attributes not in the building struct

}Pipe;

Pipe *pipe_new(Vector2D gridPos, Vector2D size, int direction);

void pipe_update(Pipe *pipe);

void pipe_draw(Pipe *pipe);

void pipe_free(Pipe *pipe);




#endif