#ifndef __ANIM_H__
#define __ANIM_H__

#include "gfc_types.h"
#include "gfc_text.h"
#include "gf2d_sprite.h"

typedef enum
{
	ANIM_IDLE,
	ANIM_ACTION,
	ANIM_DAMAGE,
	ANIM_DEATH

}AnimType;

typedef struct Anim_s
{
	Bool		_current;

	Sprite		*spriteSheet;
	AnimType	animType;

	char		*animName;

	int			startframe;
	int			endframe;
	int			currentframe;

	int			frameWidth;
	int			frameHeight;
	int			fpl;

	int			lastframe;

	int       framerate;

}Anim;

Anim **anim_list_new(int length);

Anim *anim_new(char *spriteName, char *animName, AnimType animType, int startframe, int endframe, float framerate, int frameWidth, int frameHeight, int fpl);

void anim_list_update(Anim *animList, int animListLen);

void anim_list_draw(Anim **animList, int animListLen, Vector2D drawPos);


#endif