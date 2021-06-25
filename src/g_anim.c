#include "simple_logger.h"

#include "g_anim.h"
#include "g_camera.h"

Anim **anim_list_new(int length)
{
	Anim **list = gfc_allocate_array(sizeof(Anim), length);
	return list;
}

Anim *anim_new(char *spriteName, char *animName, AnimType animType, int startframe, int endframe, float framerate, int frameWidth, int frameHeight, int fpl)
{
	int i;

	Anim* anim;

	anim = (Anim*)malloc(sizeof(Anim));

	memset(anim, 0, sizeof(Anim));

	anim->spriteSheet = gf2d_sprite_load_all(spriteName, frameWidth, frameHeight, fpl);

	anim->animType = animType;
	anim->startframe = startframe;
	anim->endframe = endframe;
	anim->framerate = framerate;
	anim->frameWidth = frameWidth;
	anim->frameHeight = frameHeight;
	anim->fpl = fpl;

	anim->animName = animName;

	anim->lastframe = 0;

	return anim;

}

void anim_list_update(Anim **animList, int animListLen)
{
	Anim *currentAnim = NULL;
	int currentTime = SDL_GetTicks();

	for (int i = 0; i < animListLen; i++)
	{
		if (animList[i] && animList[i]->_current)
		{
			currentAnim = animList[i];
		}
	}

	if (!currentAnim)
	{
		if (animList[0] && animList[0]->animType == ANIM_IDLE)
		{
			currentAnim = animList[0];
		}
		else
		{
			return;
		}

	}

	slog("CurrentTime: %i, LastTime: %i", currentTime, currentAnim->lastframe);

	if (currentTime > currentAnim->lastframe + (int)(1000.0 / currentAnim->framerate))
	{
		currentAnim->currentframe++;
		currentAnim->lastframe = currentTime;
	}

	if (currentAnim->currentframe >= currentAnim->endframe)
	{
		currentAnim->currentframe = currentAnim->startframe;
		currentAnim->lastframe = currentAnim->currentframe;
	}
}

void anim_list_draw(Anim **animList, int animListLen, Vector2D drawPos)
{
	Vector2D drawPosition, offset;
	Vector2D drawScale = camera_get_scale();

	Anim *currentAnim = NULL;

	

	for (int i = 0; i < animListLen; i++)
	{
		if (animList[i] && animList[i]->_current)
		{
			currentAnim = animList[i];
		}
	}

	if (!currentAnim)
	{
		if (animList[0] && animList[0]->animType == ANIM_IDLE)
		{
			currentAnim = animList[0];
		}
		else
		{
			return;
		}
		
	}

	gf2d_sprite_draw(
		currentAnim->spriteSheet,
		drawPos,
		&drawScale,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)currentAnim->currentframe);
}