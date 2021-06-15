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
	Sprite		*spriteSheet;
	AnimType	animType;

	int			startframe;
	int			endframe;
	float       frameRate;


}Anim;

#endif